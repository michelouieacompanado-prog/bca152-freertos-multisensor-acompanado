#if defined(NATIVE) || !defined(ESP_PLATFORM)
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include <stdbool.h>

EventGroupHandle_t systemEventGroup = nullptr;
SemaphoreHandle_t  serialMutex      = nullptr;

#ifdef __cplusplus
extern "C" {
#endif

void ssd1306_set_power(bool on) {
    (void)on;
}

#ifdef __cplusplus
}
#endif

#endif
