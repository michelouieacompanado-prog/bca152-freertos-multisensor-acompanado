#pragma once
#include <stdint.h>

typedef enum {
    GPIO_NUM_0  = 0,
    GPIO_NUM_4  = 4,
    GPIO_NUM_5  = 5,
    GPIO_NUM_18 = 18,
    GPIO_NUM_19 = 19,
    GPIO_NUM_21 = 21,
    GPIO_NUM_22 = 22,
    GPIO_NUM_25 = 25,
    GPIO_NUM_27 = 27,
    GPIO_NUM_34 = 34,
    GPIO_NUM_MAX
} gpio_num_t;

typedef enum { GPIO_INTR_DISABLE = 0 } gpio_int_type_t;
typedef enum { GPIO_MODE_INPUT = 0, GPIO_MODE_OUTPUT, GPIO_MODE_OUTPUT_OD } gpio_mode_t;
typedef enum { GPIO_PULLUP_DISABLE = 0, GPIO_PULLUP_ENABLE = 1 } gpio_pullup_t;
typedef enum { GPIO_PULLDOWN_DISABLE = 0, GPIO_PULLDOWN_ENABLE = 1 } gpio_pulldown_t;
typedef enum { GPIO_PULLUP_ONLY = 0 } gpio_pull_mode_t;

typedef struct {
    gpio_int_type_t intr_type;
    gpio_mode_t mode;
    uint64_t pin_bit_mask;
    gpio_pulldown_t pull_down_en;
    gpio_pullup_t pull_up_en;
} gpio_config_t;

inline int gpio_config(const gpio_config_t *cfg) { (void)cfg; return 0; }
inline int gpio_set_direction(gpio_num_t pin, gpio_mode_t mode) { (void)pin; (void)mode; return 0; }
inline int gpio_set_pull_mode(gpio_num_t pin, gpio_pull_mode_t m) { (void)pin; (void)m; return 0; }
inline int gpio_set_level(gpio_num_t pin, uint32_t level) { (void)pin; (void)level; return 0; }
inline int gpio_get_level(gpio_num_t pin) { (void)pin; return 0; }
