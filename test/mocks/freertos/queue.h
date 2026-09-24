#pragma once
#include "FreeRTOS.h"

#define xQueueCreate(length, itemSize)   ((QueueHandle_t)NULL)
#define xQueueSend(queue, item, ticks)   (pdPASS)
#define xQueueReceive(queue, buf, ticks) (pdFAIL)
