#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"

#include "motion.h"
#include "rtos_objects.h"

void init_pir(void) {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << PIR_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

bool read_pir(void) {
    return gpio_get_level(PIR_PIN) == 1;
}

void MotionTask(void *pvParameters) {
    (void)pvParameters;
    init_pir();

    bool last_motion = false;

    for (;;) {
        bool motion = read_pir();

        if (motion != last_motion) {
            last_motion = motion;
            if (systemEventGroup != NULL) {
                if (motion) {
                    xEventGroupSetBits(systemEventGroup, EVENT_MOTION);
                    if (serialMutex != NULL && xSemaphoreTake(serialMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                        printf("[MOTION] PIR Sensor: Motion Detected!\n");
                        xSemaphoreGive(serialMutex);
                    }
                } else {
                    xEventGroupClearBits(systemEventGroup, EVENT_MOTION);
                    if (serialMutex != NULL && xSemaphoreTake(serialMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                        printf("[MOTION] PIR Sensor: Motion Cleared.\n");
                        xSemaphoreGive(serialMutex);
                    }
                }
            }
        }

        // Section 59: Short periodic delay for MotionTask (Priority 3)
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
