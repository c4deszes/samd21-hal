#include "hal/systick.h"
#include "sam.h"

void SYSTICK_Setup(uint32_t ticks) {
    (void)SysTick_Config(ticks);
}
