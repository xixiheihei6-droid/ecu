#ifndef ECU_H
#define ECU_H

ecu_t ecu;

void ecu_init(void) {
    ecu.steering_pwm_pin = GPIO_PIN_0;     // B0
    ecu.led_red_pin = GPIO_PIN_9;          // C9
    ecu.led_green_pin = GPIO_PIN_7;        // C7
    ecu.led_blue_pin = GPIO_PIN_6;         // C6
    ecu.front_headlights_pin = GPIO_PIN_3; // A3
    ecu.rear_headlights_pin = GPIO_PIN_1;  // A1
    ecu.uart_rx = GPIO_PIN_8;              // B8
    ecu.uart_tx = GPIO_PIN_9;              // B9
    ecu.i2c_sda = GPIO_PIN_4;              // B4
    ecu.i2c_scl = GPIO_PIN_8;              // A8
    ecu.can_rx = GPIO_PIN_12;              // B12
    ecu.can_tx = GPIO_PIN_13;              // B13
}

#endif
