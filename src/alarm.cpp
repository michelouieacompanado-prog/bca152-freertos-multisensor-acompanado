#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"

#include "alarm.h"
#include "sensors.h"
#include "rtos_objects.h"

AlarmState evaluateTemperature(float temperature) {
    if (temperature < LOW_TEMP_LIMIT) {
        return AlarmState::LOW_TEMPERATURE;
    } else if (temperature > HIGH_TEMP_LIMIT) {
        return AlarmState::HIGH_TEMPERATURE;
    }
    return AlarmState::NORMAL;
}

void init_buzzer(void) {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << BUZZER_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(BUZZER_PIN, 0);
}

void set_buzzer_state(bool active) {
    gpio_set_level(BUZZER_PIN, active ? 1 : 0);
}

void AlarmTask(void *pvParameters) {
    (void)pvParameters;
    init_buzzer();

    struct SensorData data;
    bool is_alarm = false;

    for (;;) {
        if (alarmQueue != NULL && xQueueReceive(alarmQueue, &data, pdMS_TO_TICKS(50)) == pdPASS) {
            AlarmState state = evaluateTemperature(data.temperature);

            if (state != AlarmState::NORMAL) {
                is_alarm = true;

                if (systemEventGroup != NULL) {
                    xEventGroupSetBits(systemEventGroup, EVENT_ALARM);
                }

                if (serialMutex != NULL && xSemaphoreTake(serialMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                    printf("[ALARM] WARNING: Temperature out of range! Current: %.2f C (%s)\n",
                           data.temperature,
                           state == AlarmState::HIGH_TEMPERATURE ? "HIGH" : "LOW");
                    xSemaphoreGive(serialMutex);
                }
            } else {
                is_alarm = false;
                set_buzzer_state(false);

                if (systemEventGroup != NULL) {
                    xEventGroupClearBits(systemEventGroup, EVENT_ALARM);
                }
            }
        }

        if (is_alarm) {
            // Generate 1 kHz tone burst (50ms duration) for loud speaker audio in Wokwi WebAudio
            for (int i = 0; i < 50; i++) {
                gpio_set_level(BUZZER_PIN, 1);
                esp_rom_delay_us(500);
                gpio_set_level(BUZZER_PIN, 0);
                esp_rom_delay_us(500);
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
