#include "hal/nvic.h"

#include "sam.h"

void NVIC_Initialize(void) {
    __DMB();
    __enable_irq();
}

void NVIC_Reset(void) {
    NVIC_SystemReset();
}
