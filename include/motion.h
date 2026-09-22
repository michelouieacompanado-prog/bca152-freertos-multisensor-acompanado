#ifndef MOTION_H
#define MOTION_H

#include <stdbool.h>
#include "driver/gpio.h"

#define PIR_PIN GPIO_NUM_27

void init_pir(void);
bool read_pir(void);
void MotionTask(void *pvParameters);

#endif // MOTION_H
