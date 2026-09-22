#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sensors.h"

// The Consumer Task
void ProcessingTask(void *pvParameters) {
    QueueHandle_t sensorQueue = (QueueHandle_t)pvParameters;
    SensorData_t receivedData;

    for (;;) {
        // Block indefinitely (portMAX_DELAY) until data is available in the queue
        if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY) == pdPASS) {
            printf("Queue Received -> Temp: %.2f C | Hum: %.2f %% | Light: %d %%\n", 
                   receivedData.temperature, 
                   receivedData.humidity, 
                   receivedData.lightLevel);
        }
    }
}

extern "C" void app_main() {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    // Create a queue capable of holding 5 SensorData_t items
    QueueHandle_t sensorQueue = xQueueCreate(5, sizeof(SensorData_t));

    if (sensorQueue != NULL) {
        // Pass the queue handle to both tasks via the 4th parameter
        xTaskCreate(SensorTask, "SensorTask", 2048, (void *)sensorQueue, 2, NULL);
        xTaskCreate(ProcessingTask, "ProcessingTask", 2048, (void *)sensorQueue, 1, NULL);
    } else {
        printf("System Error: Failed to create sensor queue!\n");
    }
}