#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/adc.h"
#include "sensors.h"

void init_ldr() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
}

int read_ldr_percentage() {
    int raw = adc1_get_raw(ADC1_CHANNEL_6);
    return (raw * 100) / 4095; 
}

void SensorTask(void *pvParameters) {
    // Cast the task parameter back to a QueueHandle_t
    QueueHandle_t sensorQueue = (QueueHandle_t)pvParameters;
    
    init_ldr();
    TickType_t lastWakeTime = xTaskGetTickCount();
    SensorData_t currentData;

    for (;;) {
        // Populate the struct
        currentData.lightLevel = read_ldr_percentage();
        currentData.temperature = 25.40; // Dummy DHT data
        currentData.humidity = 61.20;

        // Send to queue. If full, do not block (wait 0 ticks)
        if (sensorQueue != NULL) {
            xQueueSend(sensorQueue, &currentData, 0);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2000));
    }
}