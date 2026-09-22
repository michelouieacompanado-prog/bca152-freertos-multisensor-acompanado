#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensors.h"
#include "display.h"
#include "input.h"
#include "rtos_objects.h"

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    // Initialize FreeRTOS synchronization and IPC objects
    init_rtos_objects();

    if (sensorQueue != NULL && navQueue != NULL) {
        // InputTask (Priority 3)
        xTaskCreate(InputTask, "InputTask", 2048, NULL, 3, NULL);
        // SensorTask (Priority 2)
        xTaskCreate(SensorTask, "SensorTask", 3072, NULL, 2, NULL);
        // DisplayTask (Priority 1)
        xTaskCreate(DisplayTask, "DisplayTask", 4096, NULL, 1, NULL);
    } else {
        printf("System Error: Failed to create queues!\n");
    }
}