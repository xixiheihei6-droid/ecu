#include "early_init.h"
#include "stm32f4xx_hal.h"

extern void *_app_start[];

void __initialize_hardware_early(void) {
    early_initialization();
}

/*
Currently empty bootstub. We immediatley launch the main application.
In the future, we may want to add an onswitch and query that pin before launching.
*/
int main(void) {
    HAL_Init();

    // Jump directly to main application
    ((void (*)(void))_app_start[1])();

    return 0;
}
