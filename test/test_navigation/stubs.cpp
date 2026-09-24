#if defined(NATIVE) || !defined(ESP_PLATFORM)
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

QueueHandle_t     navQueue    = nullptr;
SemaphoreHandle_t serialMutex = nullptr;
#endif
