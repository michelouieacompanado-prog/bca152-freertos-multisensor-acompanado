#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcpp"
#include "driver/adc.h"
#pragma GCC diagnostic pop
#include "esp_timer.h"
#include "esp_rom_sys.h"

#include "sensors.h"
#include "rtos_objects.h"

static float last_temp = 25.4f;
static float last_hum = 61.2f;

void init_sensors(void) {
    // Configure LDR on ADC1 Channel 6 (GPIO 34)
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(LDR_ADC1_CH, ADC_ATTEN_DB_12);

    // Configure DHT22 pin
    gpio_set_direction(DHT22_PIN, GPIO_MODE_OUTPUT_OD);
    gpio_set_pull_mode(DHT22_PIN, GPIO_PULLUP_ONLY);
    gpio_set_level(DHT22_PIN, 1);
}

int read_ldr_percentage(void) {
    int raw = adc1_get_raw(LDR_ADC1_CH);
    if (raw < 0) {
        raw = 0;
    }
    if (raw > 4095) {
        raw = 4095;
    }
    // Convert 12-bit ADC (0-4095) to documented 0-100% representation
    return (raw * 100) / 4095;
}

static int wait_for_level(gpio_num_t pin, int level, uint32_t timeout_us) {
    int count = 0;
    while (gpio_get_level(pin) != level) {
        if (count++ > (int)timeout_us) {
            return -1;
        }
        esp_rom_delay_us(1);
    }
    return count;
}

bool read_dht22(float *temperature, float *humidity) {
    uint8_t data[5] = {0};

    // Send Start Signal
    gpio_set_direction(DHT22_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT22_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(20)); // Keep low for 20ms

    gpio_set_level(DHT22_PIN, 1);
    esp_rom_delay_us(30);          // High for 30us
    gpio_set_direction(DHT22_PIN, GPIO_MODE_INPUT);

    // Await DHT22 response: 80us low, then 80us high
    if (wait_for_level(DHT22_PIN, 0, 100) < 0) return false;
    if (wait_for_level(DHT22_PIN, 1, 100) < 0) return false;
    if (wait_for_level(DHT22_PIN, 0, 100) < 0) return false;

    // Read 40 bits (5 bytes)
    for (int i = 0; i < 40; ++i) {
        // Wait for pin to go high
        if (wait_for_level(DHT22_PIN, 1, 100) < 0) return false;

        // Measure high duration
        int duration = 0;
        while (gpio_get_level(DHT22_PIN) == 1) {
            if (++duration > 120) {
                return false;
            }
            esp_rom_delay_us(1);
        }

        // Bit is 1 if pulse is > 40us, 0 if < 40us
        int byte_idx = i / 8;
        data[byte_idx] <<= 1;
        if (duration > 40) {
            data[byte_idx] |= 1;
        }
    }

    // Verify Checksum
    uint8_t checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
    if (checksum != data[4]) {
        return false;
    }

    // Parse Humidity and Temperature
    float raw_hum = ((data[0] << 8) | data[1]) * 0.1f;
    float raw_temp = (((data[2] & 0x7F) << 8) | data[3]) * 0.1f;
    if (data[2] & 0x80) {
        raw_temp = -raw_temp;
    }

    *humidity = raw_hum;
    *temperature = raw_temp;
    last_hum = raw_hum;
    last_temp = raw_temp;
    return true;
}

void SensorTask(void *pvParameters) {
    (void)pvParameters;
    init_sensors();

    TickType_t lastWakeTime = xTaskGetTickCount();
    struct SensorData currentData = {
        .temperature = last_temp,
        .humidity = last_hum,
        .lightLevel = 50,
        .motionDetected = false
    };

    for (;;) {
        float temp = 0.0f;
        float hum = 0.0f;
        if (read_dht22(&temp, &hum)) {
            currentData.temperature = temp;
            currentData.humidity = hum;
        } else {
            // Keep last known valid reading if read cycle timed out
            currentData.temperature = last_temp;
            currentData.humidity = last_hum;
        }

        currentData.lightLevel = read_ldr_percentage();

        if (systemEventGroup != NULL) {
            EventBits_t bits = xEventGroupGetBits(systemEventGroup);
            currentData.motionDetected = (bits & EVENT_MOTION) != 0;
        }

        if (sensorQueue != NULL) {
            xQueueSend(sensorQueue, &currentData, 0);
        }
        if (alarmQueue != NULL) {
            xQueueSend(alarmQueue, &currentData, 0);
        }

        if (serialMutex != NULL && xSemaphoreTake(serialMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
            printf("[SENSOR] Periodic Sample -> Temp: %.1f C | Hum: %.1f %% | Light: %d %% | Motion: %s\n",
                   currentData.temperature,
                   currentData.humidity,
                   currentData.lightLevel,
                   currentData.motionDetected ? "YES" : "NO");
            xSemaphoreGive(serialMutex);
        }

        // Section 22: Mandatory periodic execution using vTaskDelayUntil
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2000));
    }
}