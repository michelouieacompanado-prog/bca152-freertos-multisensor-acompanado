#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include "sensors.h"

#ifdef __cplusplus
extern "C" {
#endif

// Hardware Configuration
#define I2C_MASTER_NUM     I2C_NUM_0
#define I2C_MASTER_SDA_IO  GPIO_NUM_21
#define I2C_MASTER_SCL_IO  GPIO_NUM_22
#define I2C_MASTER_FREQ_HZ 400000
#define SSD1306_I2C_ADDR   0x3C

// SSD1306 Native ESP-IDF Driver Functions
void init_i2c(void);
void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_draw_string(int col, int page, const char *text);
void ssd1306_update(void);
void ssd1306_set_power(bool on);

// FreeRTOS Task (Priority 1)
void DisplayTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif // DISPLAY_H
