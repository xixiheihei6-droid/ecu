#include "stm32f4xx.h"

// Early bringup
extern void *g_pfnVectors;

void early_initialization(void) {
    // Initializes the system clock and other essential hardware components
    SystemInit();
    // Sets the base address of the interrupt vector table to ensure the
    // processor uses the correct ISRs. Required because app starts at
    // 0x08004000 (after bootstub), not default 0x08000000.
    SCB->VTOR = (uint32_t)&g_pfnVectors;
}
