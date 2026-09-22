#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"

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

    for (;;) {
        // Block until new sensor data arrives for alarm evaluation (Section 59)
        if (alarmQueue != NULL && xQueueReceive(alarmQueue, &data, portMAX_DELAY) == pdPASS) {
            AlarmState state = evaluateTemperature(data.temperature);

            if (state != AlarmState::NORMAL) {
                set_buzzer_state(true);

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
                set_buzzer_state(false);

                if (systemEventGroup != NULL) {
                    xEventGroupClearBits(systemEventGroup, EVENT_ALARM);
                }
            }
        }
    }
}
