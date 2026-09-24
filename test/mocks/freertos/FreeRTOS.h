#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint32_t TickType_t;
typedef uint32_t EventBits_t;
typedef void*    EventGroupHandle_t;
typedef void*    QueueHandle_t;
typedef void*    SemaphoreHandle_t;

#define portMAX_DELAY       ((TickType_t)0xFFFFFFFFUL)
#define portTICK_PERIOD_MS  1U
#define pdMS_TO_TICKS(ms)   ((TickType_t)(ms))
#define pdTRUE              1
#define pdFALSE             0
#define pdPASS              pdTRUE
#define pdFAIL              pdFALSE
