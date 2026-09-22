#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "input.h"
#include "rtos_objects.h"

DisplayMode nextDisplayMode(DisplayMode current) {
    switch (current) {
        case DisplayMode::TEMPERATURE: return DisplayMode::HUMIDITY;
        case DisplayMode::HUMIDITY:    return DisplayMode::LIGHT;
        case DisplayMode::LIGHT:       return DisplayMode::MOTION;
        case DisplayMode::MOTION:      return DisplayMode::TEMPERATURE;
    }
    return DisplayMode::TEMPERATURE;
}

DisplayMode previousDisplayMode(DisplayMode current) {
    switch (current) {
        case DisplayMode::TEMPERATURE: return DisplayMode::MOTION;
        case DisplayMode::HUMIDITY:    return DisplayMode::TEMPERATURE;
        case DisplayMode::LIGHT:       return DisplayMode::HUMIDITY;
        case DisplayMode::MOTION:      return DisplayMode::LIGHT;
    }
    return DisplayMode::TEMPERATURE;
}

void init_input(void) {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ENCODER_CLK_PIN) | (1ULL << ENCODER_DT_PIN) | (1ULL << ENCODER_SW_PIN);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);
}

void InputTask(void *pvParameters) {
    (void)pvParameters;
    init_input();

    int lastClk = gpio_get_level(ENCODER_CLK_PIN);

    for (;;) {
        int currentClk = gpio_get_level(ENCODER_CLK_PIN);

        // Detect falling edge on CLK
        if (lastClk == 1 && currentClk == 0) {
            int currentDt = gpio_get_level(ENCODER_DT_PIN);
            NavigationEvent event = NavigationEvent::NONE;

            if (currentDt == 1) {
                event = NavigationEvent::NEXT; // Clockwise
            } else {
                event = NavigationEvent::PREVIOUS; // Counterclockwise
            }

            if (navQueue != NULL && event != NavigationEvent::NONE) {
                xQueueSend(navQueue, &event, 0);
            }
        }

        lastClk = currentClk;
        // InputTask short periodic polling interval (Section 59)
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
