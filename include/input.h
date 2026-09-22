#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/gpio.h"

#define ENCODER_CLK_PIN GPIO_NUM_18
#define ENCODER_DT_PIN  GPIO_NUM_19
#define ENCODER_SW_PIN  GPIO_NUM_5

// Section 28: DisplayMode enumeration
enum class DisplayMode {
    TEMPERATURE,
    HUMIDITY,
    LIGHT,
    MOTION
};

enum class NavigationEvent {
    NONE,
    NEXT,
    PREVIOUS
};

// Pure hardware-independent navigation functions (Section 29 & Section 42)
DisplayMode nextDisplayMode(DisplayMode current);
DisplayMode previousDisplayMode(DisplayMode current);

void init_input(void);
void InputTask(void *pvParameters);

#endif // INPUT_H
