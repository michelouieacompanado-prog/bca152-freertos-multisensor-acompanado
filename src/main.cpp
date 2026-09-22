#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "rtos_objects.h"
#include "sensors.h"
#include "display.h"
#include "input.h"
#include "alarm.h"
#include "motion.h"
#include "system_state.h"

#ifndef PIO_UNIT_TESTING
extern "C" void app_main(void) {
    // 1. Hardware & RTOS Object Creation (Section 41)
    init_rtos_objects();

    // 2. Banner Output protected by serialMutex (Section 36)
    if (serialMutex != NULL) {
        xSemaphoreTake(serialMutex, portMAX_DELAY);
    }
    printf("========================================\n");
    printf(" BCA152 FreeRTOS Multisensor Room Monitor\n");
    printf(" System starting...\n");
    printf("========================================\n");
    if (serialMutex != NULL) {
        xSemaphoreGive(serialMutex);
    }

    // 3. Task Creation with Explicit Priorities (Section 38 & Section 41)
    // MotionTask (Priority 3 - urgent motion event responsiveness)
    xTaskCreate(MotionTask, "MotionTask", 2048, NULL, 3, NULL);

    // InputTask (Priority 3 - interactive encoder responsiveness)
    xTaskCreate(InputTask, "InputTask", 2048, NULL, 3, NULL);

    // SensorTask (Priority 2 - periodic sensor measurement)
    xTaskCreate(SensorTask, "SensorTask", 3072, NULL, 2, NULL);

    // AlarmTask (Priority 2 - safety-critical alarm evaluation)
    xTaskCreate(AlarmTask, "AlarmTask", 2048, NULL, 2, NULL);

    // StateTask (Priority 2 - centralized ACTIVE/INACTIVE state machine)
    xTaskCreate(StateTask, "StateTask", 2048, NULL, 2, NULL);

    // DisplayTask (Priority 1 - UI rendering, latency-tolerant)
    xTaskCreate(DisplayTask, "DisplayTask", 4096, NULL, 1, NULL);

    // 4. Scheduler-driven operation: app_main task deletes itself
    vTaskDelete(NULL);
}
#endif