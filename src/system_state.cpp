#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"

#include "system_state.h"
#include "rtos_objects.h"
#include "display.h"

static SystemState s_currentState = SystemState::ACTIVE;

SystemState evaluateSystemState(SystemState currentState, bool motionDetected, uint32_t elapsedInactiveMs, uint32_t timeoutMs) {
    if (currentState == SystemState::ACTIVE) {
        if (motionDetected) {
            return SystemState::ACTIVE;
        }
        if (elapsedInactiveMs >= timeoutMs) {
            return SystemState::INACTIVE;
        }
        return SystemState::ACTIVE;
    } else { // INACTIVE
        if (motionDetected) {
            return SystemState::ACTIVE;
        }
        return SystemState::INACTIVE;
    }
}

void init_system_state(void) {
    s_currentState = SystemState::ACTIVE;
    if (systemEventGroup != NULL) {
        xEventGroupSetBits(systemEventGroup, EVENT_ACTIVE);
    }
}

SystemState get_system_state(void) {
    return s_currentState;
}

void StateTask(void *pvParameters) {
    (void)pvParameters;
    init_system_state();

    uint32_t lastMotionTick = xTaskGetTickCount();

    for (;;) {
        bool motion = false;
        if (systemEventGroup != NULL) {
            EventBits_t bits = xEventGroupGetBits(systemEventGroup);
            motion = (bits & EVENT_MOTION) != 0;
        }

        uint32_t currentTick = xTaskGetTickCount();
        if (motion) {
            lastMotionTick = currentTick;
        }

        uint32_t elapsedMs = (currentTick - lastMotionTick) * portTICK_PERIOD_MS;
        SystemState newState = evaluateSystemState(s_currentState, motion, elapsedMs, INACTIVITY_TIMEOUT_MS);

        if (newState != s_currentState) {
            s_currentState = newState;

            if (newState == SystemState::INACTIVE) {
                // Section 34: INACTIVE behavior - OLED off or blank
                ssd1306_set_power(false);
                if (systemEventGroup != NULL) {
                    xEventGroupClearBits(systemEventGroup, EVENT_ACTIVE);
                }
                if (serialMutex != NULL && xSemaphoreTake(serialMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                    printf("[STATE] Inactivity timeout reached (%d s) -> System entered INACTIVE state (OLED blanked)\n",
                           INACTIVITY_TIMEOUT_MS / 1000);
                    xSemaphoreGive(serialMutex);
                }
            } else {
                // Section 33: ACTIVE behavior - OLED enabled, restored
                ssd1306_set_power(true);
                if (systemEventGroup != NULL) {
                    xEventGroupSetBits(systemEventGroup, EVENT_ACTIVE);
                }
                if (serialMutex != NULL && xSemaphoreTake(serialMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                    printf("[STATE] Motion detected -> System restored to ACTIVE state (OLED restored)\n");
                    xSemaphoreGive(serialMutex);
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
