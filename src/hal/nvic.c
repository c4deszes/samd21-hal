#include "sam.h"

void NVIC_Initialize(void) {
    __DMB();
    __enable_irq();
}
