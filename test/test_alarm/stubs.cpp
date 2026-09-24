#if defined(NATIVE) || !defined(ESP_PLATFORM)
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

QueueHandle_t      alarmQueue       = nullptr;
SemaphoreHandle_t  serialMutex      = nullptr;
EventGroupHandle_t systemEventGroup = nullptr;
#endif
