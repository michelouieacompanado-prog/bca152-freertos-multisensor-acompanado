#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ============================================================================
 * FreeRTOS Event Group Specifications (Section 35)
 * ============================================================================
 * 
 * Bit: EVENT_ACTIVE (BIT0)
 *   Producer: StateTask (src/system_state.cpp)
 *   Consumer: DisplayTask (src/display.cpp), AlarmTask (src/alarm.cpp)
 *   Set when: System initializes or motion activity transitions system to ACTIVE state.
 *   Cleared when: Inactivity timeout (15 seconds without PIR motion) expires.
 * 
 * Bit: EVENT_MOTION (BIT1)
 *   Producer: MotionTask (src/motion.cpp)
 *   Consumer: StateTask (src/system_state.cpp), SensorTask (src/sensors.cpp)
 *   Set when: PIR sensor detects physical motion (GPIO 27 goes HIGH).
 *   Cleared when: PIR sensor output returns to LOW (no motion detected).
 * 
 * Bit: EVENT_ALARM (BIT2)
 *   Producer: AlarmTask (src/alarm.cpp)
 *   Consumer: DisplayTask (src/display.cpp)
 *   Set when: Temperature reading violates threshold (< 18.0 °C or > 30.0 °C).
 *   Cleared when: Temperature returns to normal operating range [18.0 °C, 30.0 °C].
 */
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
