#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sensors.h"
#include "rtos_objects.h"

// Phase 1 verification consumer task
void ProcessingTask(void *pvParameters) {
    (void)pvParameters;
    struct SensorData receivedData;

    for (;;) {
        if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY) == pdPASS) {
            if (serialMutex != NULL) {
                xSemaphoreTake(serialMutex, portMAX_DELAY);
            }
            printf("Queue Received -> Temp: %.2f C | Hum: %.2f %% | Light: %d %%\n",
                   receivedData.temperature,
                   receivedData.humidity,
                   receivedData.lightLevel);
            if (serialMutex != NULL) {
                xSemaphoreGive(serialMutex);
            }
        }
    }
}

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    // Initialize FreeRTOS synchronization and IPC objects
    init_rtos_objects();

    if (sensorQueue != NULL) {
        xTaskCreate(SensorTask, "SensorTask", 2048, NULL, 2, NULL);
        xTaskCreate(ProcessingTask, "ProcessingTask", 2048, NULL, 1, NULL);
    } else {
        printf("System Error: Failed to create sensor queue!\n");
    }
}