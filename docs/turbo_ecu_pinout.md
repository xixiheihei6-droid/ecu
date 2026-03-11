# Turbo ECU Pinout

## Current Pin Assignments

| Pin | Function | Mode | AF | Notes |
|-----|----------|------|----|-------|
| PA1 | Rear headlights | GPIO OUT | - | |
| PA3 | Front headlights | GPIO OUT | - | |
| PA8 | I2C3 SCL | AF OD | AF4 | |
| PB0 | Steering PWM | AF PP | AF2 | TIM3_CH3 |
| PB4 | I2C3 SDA | AF OD | AF9 | Different AF than SCL |
| PB8 | UART5 RX | AF PP | AF11 | |
| PB9 | UART5 TX | AF PP | AF11 | |
| PB12 | CAN2 RX | AF PP | AF9 | |
| PB13 | CAN2 TX | AF PP | AF9 | |
| PC6 | LED Blue | GPIO OUT | - | Active-low |
| PC7 | LED Green | GPIO OUT | - | Active-low |
| PC9 | LED Red | GPIO OUT | - | Active-low |

For full STM32F413 alternate-function details, see `docs/stm32f413_af_table.md`.
