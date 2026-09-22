#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#ifdef __cplusplus
extern "C" {
#endif

// Event Group bit definitions (Section 35)
#define EVENT_ACTIVE BIT0
#define EVENT_MOTION BIT1
#define EVENT_ALARM  BIT2

// FreeRTOS Queues
extern QueueHandle_t sensorQueue;
extern QueueHandle_t alarmQueue;
extern QueueHandle_t navQueue;

// FreeRTOS Mutex for shared Serial/UART (Section 36)
extern SemaphoreHandle_t serialMutex;

// FreeRTOS Event Group (Section 35)
extern EventGroupHandle_t systemEventGroup;

// Initialize all FreeRTOS synchronization and IPC primitives
void init_rtos_objects(void);

#ifdef __cplusplus
}
#endif

#endif // RTOS_OBJECTS_H
