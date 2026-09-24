#pragma once
#include "FreeRTOS.h"

inline TickType_t xTaskGetTickCount(void) { return 0; }
#define vTaskDelay(ticks) ((void)(ticks))
#define vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement) ((void)(pxPreviousWakeTime), (void)(xTimeIncrement))
