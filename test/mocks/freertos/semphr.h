#pragma once
#include "FreeRTOS.h"

inline SemaphoreHandle_t xSemaphoreCreateMutex(void) { return NULL; }
#define xSemaphoreTake(sem, ticks) (pdTRUE)
#define xSemaphoreGive(sem)        ((void)(sem))
