# STM32F413 Alternate Function Table

Reference: [STM32F413 Datasheet](https://www.st.com/resource/en/datasheet/stm32f413cg.pdf) Table 12

## Port A

| Pin | AF0 | AF1 | AF2 | AF3 | AF4 | AF5 | AF6 | AF7 | AF8 | AF9 | AF10 | AF11 | AF12 |
|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|------|------|------|
| PA0 | TIM2_CH1 | TIM5_CH1 | TIM8_ETR | | | SPI6_NSS | | USART2_CTS | UART4_TX | | USB_FS | | |
| PA1 | TIM2_CH2 | TIM5_CH2 | | | | SPI6_SCK | | USART2_RTS | UART4_RX | QUADSPI_BK1_IO3 | USB_FS | | |
| PA2 | TIM2_CH3 | TIM5_CH3 | TIM9_CH1 | | | SPI6_MISO | | USART2_TX | | FSMC | USB_FS | | |
| PA3 | TIM2_CH4 | TIM5_CH4 | TIM9_CH2 | | | SPI6_MOSI | | USART2_RX | | FSMC | USB_FS | | |
| PA4 | SPI1_NSS | SPI3_NSS | | | | SPI6 | | USART2_CK | | | USB_FS | | |
| PA5 | TIM2_CH1 | | TIM8_CH1N | | SPI1_SCK | | | | | | USB_FS | | |
| PA6 | TIM1_BKIN | TIM3_CH1 | TIM8_BKIN | | SPI1_MISO | | | | | TIM13_CH1 | | | |
| PA7 | TIM1_CH1N | TIM3_CH2 | TIM8_CH1N | | SPI1_MOSI | | | | | TIM14_CH1 | | | |
| PA8 | MCO1 | TIM1_CH1 | | | **I2C3_SCL** | | | USART1_CK | | | USB_FS | | |
| PA9 | TIM1_CH2 | | | | I2C3_SMBA | | | USART1_TX | | | USB_FS | | |
| PA10 | TIM1_CH3 | | | | | | | USART1_RX | | | USB_FS | | |
| PA11 | TIM1_CH4 | | | | | | | USART1_CTS | USART6_TX | | USB_FS | | |
| PA12 | TIM1_ETR | | | | | | | USART1_RTS | USART6_RX | | USB_FS | | |
| PA15 | TIM2_CH1 | TIM2_ETR | | | | SPI1_NSS | SPI3_NSS | USART1_TX | | | | | |

## Port B

| Pin | AF0 | AF1 | AF2 | AF3 | AF4 | AF5 | AF6 | AF7 | AF8 | AF9 | AF10 | AF11 | AF12 |
|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|------|------|------|
| PB0 | TIM1_CH2N | TIM3_CH3 | **TIM8_CH2N** | | | | | | | | | | |
| PB1 | TIM1_CH3N | TIM3_CH4 | TIM8_CH3N | | | | | | | | | | |
| PB3 | TIM2_CH2 | | | SPI1_SCK | SPI3_SCK | | | USART1_RX | | | | | |
| PB4 | | TIM3_CH1 | | SPI1_MISO | SPI3_MISO | | | | | **I2C3_SDA** | | | |
| PB5 | | TIM3_CH2 | | SPI1_MOSI | SPI3_MOSI | | | | | I2C1_SMBA | | | |
| PB6 | | TIM4_CH1 | | | I2C1_SCL | | | USART1_TX | | | | | |
| PB7 | | TIM4_CH2 | | | I2C1_SDA | | | USART1_RX | | | | | |
| PB8 | | TIM4_CH3 | TIM10_CH1 | | I2C1_SCL | | | | | CAN1_RX | | **UART5_RX** | |
| PB9 | | TIM4_CH4 | TIM11_CH1 | | I2C1_SDA | | | | | CAN1_TX | | **UART5_TX** | |
| PB10 | TIM2_CH3 | | | | I2C2_SCL | | | USART3_TX | | | | | |
| PB12 | TIM1_BKIN | | | | I2C2_SMBA | | | USART3_CK | | **CAN2_RX** | | | |
| PB13 | TIM1_CH1N | | | | | | | USART3_CTS | | **CAN2_TX** | | | |
| PB14 | TIM1_CH2N | | TIM8_CH2N | | | SPI2_MISO | | USART3_RTS | | | | | |
| PB15 | TIM1_CH3N | | TIM8_CH3N | | | SPI2_MOSI | | | | | | | |

## Port C

| Pin | AF0 | AF1 | AF2 | AF3 | AF4 | AF5 | AF6 | AF7 | AF8 | AF9 | AF10 | AF11 | AF12 |
|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|------|------|------|
| PC6 | | TIM3_CH1 | TIM8_CH1 | | | | | USART6_TX | | | | | |
| PC7 | | TIM3_CH2 | TIM8_CH2 | | | | | USART6_RX | | | | | |
| PC8 | | TIM3_CH3 | TIM8_CH3 | | | | | | | | | | |
| PC9 | MCO2 | TIM3_CH4 | TIM8_CH4 | | I2C3_SDA | | | | | | | | |
| PC10 | | | | SPI3_SCK | | | | USART3_TX | | | | | |
| PC11 | | | | SPI3_MISO | | | | USART3_RX | | | | | |
| PC12 | | | | SPI3_MOSI | | | | USART3_CK | | | | | UART5_TX |

## Common Alternate Functions Quick Reference

| Peripheral | Pins | AF |
|------------|------|----|
| TIM3 | PA6-7, PB0-1, PC6-9 | AF2 |
| I2C1 | PB6-7, PB8-9 | AF4 |
| I2C2 | PB10-11 | AF4 |
| I2C3 | PA8 (SCL) | AF4 |
| I2C3 | PB4 (SDA), PC9 | AF9 |
| SPI1 | PA4-7, PB3-5 | AF5 |
| SPI3 | PB3-5, PC10-12 | AF6 |
| USART1 | PA9-12, PB6-7 | AF7 |
| USART2 | PA0-4 | AF7 |
| USART3 | PB10-14, PC10-12 | AF7 |
| USART6 | PA11-12, PC6-7 | AF8 |
| UART5 | PB8-9 | AF11 |
| CAN1 | PB8-9 | AF9 |
| CAN2 | PB12-13 | AF9 |
| USB_OTG_FS | PA11-12 | AF10 |
