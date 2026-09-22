#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>
#include "driver/gpio.h"

#define BUZZER_PIN GPIO_NUM_25
#define LOW_TEMP_LIMIT  18.0f
#define HIGH_TEMP_LIMIT 30.0f

// Section 30: Testable Alarm Logic
enum class AlarmState {
    NORMAL,
    LOW_TEMPERATURE,
    HIGH_TEMPERATURE
};

// Pure decision logic (Section 30 & Section 42)
AlarmState evaluateTemperature(float temperature);

void init_buzzer(void);
void set_buzzer_state(bool active);
void AlarmTask(void *pvParameters);

#endif // ALARM_H
