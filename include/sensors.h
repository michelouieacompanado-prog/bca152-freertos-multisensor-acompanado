#ifndef SENSORS_H
#define SENSORS_H

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

// Section 24: SensorData definition
struct SensorData {
    float temperature;
    float humidity;
    int lightLevel;
    bool motionDetected;
};

// GPIO Definitions
#define DHT22_PIN   GPIO_NUM_4
#define LDR_ADC1_CH ADC1_CHANNEL_6 // GPIO 34

// Sensor functions
void init_sensors(void);
bool read_dht22(float *temperature, float *humidity);
int read_ldr_percentage(void);
void SensorTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif // SENSORS_H