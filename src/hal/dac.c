#include "hal/dac.h"

#include <stdlib.h>
#include "sam.h"

static void DAC_Reset() {
    DAC_REGS->DAC_CTRLA = DAC_CTRLA_SWRST_Msk;
    while ((DAC_REGS->DAC_CTRLA & DAC_CTRLA_SWRST_Msk) != 0);
}

void DAC_Setup(void) {
    DAC_Reset();

    DAC_REGS->DAC_CTRLB = DAC_CTRLB_REFSEL_AVCC | DAC_CTRLB_EOEN_Msk | DAC_CTRLB_LEFTADJ(0);

    DAC_REGS->DAC_CTRLA = DAC_CTRLA_ENABLE_Msk;
    while((DAC_REGS->DAC_STATUS & DAC_STATUS_SYNCBUSY_Msk) != 0);
}

void DAC_SetValue(uint16_t value) {
    DAC_REGS->DAC_DATA = DAC_DATA_DATA(value);
}
