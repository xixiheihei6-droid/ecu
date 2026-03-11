// Define to prevent recursive inclusion
#ifndef SETUP_H
#define SETUP_H

#define CAN_QUANTA 16U

// Define CAN peripheral clock (APB1 clock in kHz)
#define CAN_PCLK (CORE_FREQ / 2U / 1000U) // 48 MHz / 1000 = 48000 kHz
// Calculate CAN prescaler based on desired speed (in kbps)
#define can_speed_to_prescaler(x) ((CAN_PCLK / CAN_QUANTA * 10U) / (x))

#include "clock.h"
#include "config.h"
#include "libc.h"
#include "stdout.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_pcd.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include "stm32f4xx_hal_uart.h"

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart5;
I2C_HandleTypeDef hi2c3;
CAN_HandleTypeDef hcan2;

extern ecu_t ecu;

void MX_GPIO_Clocks_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
}

// Handle TIM3 clock configuration for PWM
void MX_TIM_Init(void) {
    __HAL_RCC_TIM3_CLK_ENABLE();
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = (SystemCoreClock / 4000000) - 1; // 1 MHz timer frequency
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;            // Edge-aligned mode
    htim3.Init.Period = 80000 - 1;                          // 20 ms period (50 Hz frequency)
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    htim3.Init.RepetitionCounter = 0;
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }
    // Configure PWM output channels
    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode = TIM_OCMODE_PWM1; // Edge-aligned mode
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK) {
        Error_Handler();
    }
}

void MX_GPIO_Common_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    // Headlights (GPIOA)
    GPIO_InitStruct.Pin = ecu.front_headlights_pin;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, ecu.front_headlights_pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = ecu.rear_headlights_pin;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, ecu.rear_headlights_pin, GPIO_PIN_RESET);

    // RGB LEDs (GPIOC, active-low)
    HAL_GPIO_WritePin(GPIOC, ecu.led_red_pin | ecu.led_green_pin | ecu.led_blue_pin,
                      GPIO_PIN_SET); // active-low OFF

    GPIO_InitStruct.Pin = ecu.led_red_pin | ecu.led_green_pin | ecu.led_blue_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOC, ecu.led_blue_pin, GPIO_PIN_RESET);

    // Steering PWM
    GPIO_InitStruct.Pin = ecu.steering_pwm_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Alternate function push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // High speed
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;    // Alternate function TIM3_CH3
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void MX_UART_Init(void) {
    __HAL_RCC_UART5_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    huart5.Instance = UART5;
    huart5.Init.BaudRate = 115200;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) != HAL_OK) {
        Error_Handler();
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    // PB9 = UART5_TX (AF11)
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Alternate = GPIO_AF11_UART5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PB8 = UART5_RX (AF11)
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Alternate = GPIO_AF11_UART5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void I2C3_EV_IRQHandler(void) {
    HAL_I2C_EV_IRQHandler(&hi2c3);
}

void I2C3_ER_IRQHandler(void) {
    HAL_I2C_ER_IRQHandler(&hi2c3);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    uint32_t error = HAL_I2C_GetError(hi2c);
    uart_printf("I2C Error Callback: error=%lu\n", error);

    // reset the I2C peripheral in case of bus failure
    HAL_I2C_DeInit(hi2c);
    if (HAL_I2C_Init(hi2c) != HAL_OK) {
        uart_printf("Failed to reinitialize I2C\n");
    } else {
        uart_printf("I2C reinitialized successfully\n");
    }
}

void MX_I2C_Init(void) {
    __HAL_RCC_I2C3_CLK_ENABLE();

    // GPIO must be configured before peripheral init
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // SCL - A8
    GPIO_InitStruct.Pin = ecu.i2c_scl;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    // SDA - B4
    GPIO_InitStruct.Pin = ecu.i2c_sda;
    GPIO_InitStruct.Alternate = GPIO_AF9_I2C3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    hi2c3.Instance = I2C3;
    hi2c3.Init.ClockSpeed = I2C_CLOCKSPEED * 1000U;
    hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c3.Init.OwnAddress1 = 0;
    hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2 = 0;
    hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
        Error_Handler();
    }

    // Error interrupt for bus fault detection
    HAL_NVIC_SetPriority(I2C3_ER_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
    __HAL_I2C_ENABLE_IT(&hi2c3, I2C_IT_ERR);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hcan->Instance == CAN2) {
        __HAL_RCC_CAN1_CLK_ENABLE();
        __HAL_RCC_CAN2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        // PB12 = CAN2_RX, PB13 = CAN2_TX (AF9)
        GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void MX_CAN_DEVICE_Init(void) {
    hcan2.Instance = CAN2;
    hcan2.Init.Mode = CAN_MODE_NORMAL;
    hcan2.Init.Prescaler = 6; // 48 MHz / 6 / 16 TQ = 500 kbps
    hcan2.Init.SyncJumpWidth = CAN_SJW_4TQ;
    hcan2.Init.TimeSeg1 = CAN_BS1_12TQ;
    hcan2.Init.TimeSeg2 = CAN_BS2_3TQ;
    hcan2.Init.AutoBusOff = ENABLE;
    hcan2.Init.TimeTriggeredMode = DISABLE;
    hcan2.Init.AutoWakeUp = ENABLE;
    hcan2.Init.AutoRetransmission = DISABLE;
    hcan2.Init.ReceiveFifoLocked = DISABLE;
    hcan2.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan2) != HAL_OK) {
        Error_Handler();
    }

    CAN_FilterTypeDef sFilterConfig = {0};
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;

    sFilterConfig.SlaveStartFilterBank = 14;
    sFilterConfig.FilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_TX_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);

    if (HAL_CAN_Start(&hcan2) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR | CAN_IT_BUSOFF) != HAL_OK) {
        Error_Handler();
    }
}
#endif
