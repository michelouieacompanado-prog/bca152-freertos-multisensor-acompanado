#pragma once
#include "FreeRTOS.h"

#define BIT0 ((EventBits_t)0x01U)
#define BIT1 ((EventBits_t)0x02U)
#define BIT2 ((EventBits_t)0x04U)

inline EventGroupHandle_t xEventGroupCreate(void) { return NULL; }
inline EventBits_t xEventGroupSetBits(EventGroupHandle_t eg, EventBits_t uxBits) { (void)eg; (void)uxBits; return 0; }
inline EventBits_t xEventGroupClearBits(EventGroupHandle_t eg, EventBits_t uxBits) { (void)eg; (void)uxBits; return 0; }
inline EventBits_t xEventGroupGetBits(EventGroupHandle_t eg) { (void)eg; return 0; }
