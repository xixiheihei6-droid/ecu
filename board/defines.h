// Define to prevent recursive inclusion
#ifndef DEFINES_H
#define DEFINES_H

#include "config.h"
#include "stm32f4xx_hal.h"

typedef struct {
    uint16_t steering_pwm_pin;
    uint16_t led_red_pin;
    uint16_t led_green_pin;
    uint16_t led_blue_pin;
    uint16_t front_headlights_pin;
    uint16_t rear_headlights_pin;
    uint16_t uart_rx;
    uint16_t uart_tx;
    uint16_t i2c_sda;
    uint16_t i2c_scl;
    uint16_t can_rx;
    uint16_t can_tx;
} ecu_t;

#endif // DEFINES_H
