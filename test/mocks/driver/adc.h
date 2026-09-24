#pragma once
#include <stdint.h>

typedef enum { ADC_WIDTH_BIT_12 = 3 } adc_bits_width_t;
typedef enum { ADC1_CHANNEL_6   = 6 } adc1_channel_t;
typedef enum { ADC_ATTEN_DB_12  = 3 } adc_atten_t;

inline int adc1_config_width(adc_bits_width_t w) { (void)w; return 0; }
inline int adc1_config_channel_atten(adc1_channel_t ch, adc_atten_t att) { (void)ch; (void)att; return 0; }
inline int adc1_get_raw(adc1_channel_t ch) { (void)ch; return 2048; }
