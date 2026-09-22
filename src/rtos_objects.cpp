#include "rtos_objects.h"
#include "sensors.h"
#include "input.h"

QueueHandle_t sensorQueue = NULL;
QueueHandle_t alarmQueue = NULL;
QueueHandle_t navQueue = NULL;
SemaphoreHandle_t serialMutex = NULL;
EventGroupHandle_t systemEventGroup = NULL;

void init_rtos_objects(void) {
    // Create sensor queue with capacity for 5 SensorData objects (Display consumer)
    sensorQueue = xQueueCreate(5, sizeof(struct SensorData));

    // Create alarm queue with capacity for 5 SensorData objects (Alarm consumer)
    alarmQueue = xQueueCreate(5, sizeof(struct SensorData));

    // Create navigation queue with capacity for 10 NavigationEvent items
    navQueue = xQueueCreate(10, sizeof(NavigationEvent));

    // Create binary mutex for protecting serial output
    serialMutex = xSemaphoreCreateMutex();

    // Create system event group
    systemEventGroup = xEventGroupCreate();

    // Default: system starts in ACTIVE state
    if (systemEventGroup != NULL) {
        xEventGroupSetBits(systemEventGroup, EVENT_ACTIVE);
    }
}
