#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Task A function
void taskA(void *pvParameters) {
    for (;;) {
        printf("Task A running\n");
        // Block for 1000 milliseconds
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task B function
void taskB(void *pvParameters) {
    for (;;) {
        printf("Task B running\n");
        // Block for 1500 milliseconds
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

extern "C" void app_main() {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    // Instantiate the tasks
    // xTaskCreate(TaskFunction, "TaskName", StackSize, Parameters, Priority, TaskHandle)
    xTaskCreate(taskA, "Task A", 2048, NULL, 1, NULL);
    xTaskCreate(taskB, "Task B", 2048, NULL, 1, NULL);
}