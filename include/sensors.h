#ifndef SENSORS_H
#define SENSORS_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Define the data payload
typedef struct {
    float temperature;
    float humidity;
    int lightLevel;
} SensorData_t;

void SensorTask(void *pvParameters);

#endif