#include "rtos_objects.h"
#include "sensors.h"

QueueHandle_t sensorQueue = NULL;
SemaphoreHandle_t serialMutex = NULL;
EventGroupHandle_t systemEventGroup = NULL;

void init_rtos_objects(void) {
    // Create sensor queue with capacity for 5 SensorData objects
    sensorQueue = xQueueCreate(5, sizeof(struct SensorData));

    // Create binary mutex for protecting serial output
    serialMutex = xSemaphoreCreateMutex();

    // Create system event group
    systemEventGroup = xEventGroupCreate();

    // Default: system starts in ACTIVE state
    if (systemEventGroup != NULL) {
        xEventGroupSetBits(systemEventGroup, EVENT_ACTIVE);
    }
}
