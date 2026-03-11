#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f4xx_hal.h"

#define DEBUG 0

#define CORE_FREQ 96000000U        // MCU frequency in hertz
#define PWM_FREQ 16000             // PWM frequency in Hz / is also used for buzzer
#define SERVO_PWM_FREQ 50          // Servo PWM frequency in Hz
#define SERVO_MIN_PULSE_WIDTH 1000 // Minimum pulse width in microseconds (1 ms)
#define SERVO_MAX_PULSE_WIDTH 2000 // Maximum pulse width in microseconds (2 ms)
#define I2C_CLOCKSPEED 100         // I2C clock in kHz

#endif
