#include "hal/tcc.h"

#include <stdlib.h>
#include "sam.h"

static tcc_registers_t* get_peripheral(uint8_t timer) {
    switch (timer) {
        case TCC0: return TCC0_REGS;
        case TCC1: return TCC1_REGS;
        case TCC2: return TCC2_REGS;
    }
    // TODO: is this good?
    return NULL;
}

static IRQn_Type get_interrupt(uint8_t timer) {
    switch(timer) {
        case TCC0: return TCC0_IRQn;
        case TCC1: return TCC1_IRQn;
        case TCC2: return TCC2_IRQn;
    }
    return PERIPH_MAX_IRQn + 1;
}

void TCC_SetupTrigger(uint8_t timer, uint32_t period) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1;
    peripheral->TCC_INTENSET = TCC_INTENSET_OVF_Msk;
    peripheral->TCC_INTFLAG = TCC_INTFLAG_Msk;
    peripheral->TCC_PER = period;                   // TODO: is this right? period bits seem to be offset

    NVIC_SetPriority(get_interrupt(timer), 10);
    NVIC_EnableIRQ(get_interrupt(timer));
}

void TCC_SetupNormalPwm(uint8_t timer, uint32_t period, tcc_channel_setting_t channels[4]) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1 |
                           TCC_CTRLA_PRESCSYNC_PRESC ;
    peripheral->TCC_WEXCTRL = TCC_WEXCTRL_OTMX(0x00);

    uint8_t drv_inv = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (channels[i].drv_inv) {
            drv_inv |= (1 << i);
        }
    }

    peripheral->TCC_DRVCTRL = TCC_DRVCTRL_INVEN(drv_inv);
    peripheral->TCC_PER = TCC_PER_PER(period);
    peripheral->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM;

    peripheral->TCC_CC[0] = TCC_CC_CC(channels[0].cc);
    peripheral->TCC_CC[1] = TCC_CC_CC(channels[1].cc);
    // TODO: why this goes to dummy handler?
    //peripheral->TCC_CC[2] = TCC_CC_CC(channels[2].cc);
    //peripheral->TCC_CC[3] = TCC_CC_CC(channels[3].cc);

    peripheral->TCC_INTFLAG = TCC_INTFLAG_Msk;
}

void TCC_Enable(uint8_t timer) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while((peripheral->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) != 0);
}

void TCC_Reset(uint8_t timer) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CTRLA |= TCC_CTRLA_SWRST_Msk;
    while((peripheral->TCC_SYNCBUSY & TCC_SYNCBUSY_SWRST_Msk) != 0);
}

void TCC_SetPeriod(uint8_t timer, uint32_t period) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_PERBUF = period;
    while((peripheral->TCC_SYNCBUSY & TCC_SYNCBUSY_PER_Msk) != 0);
}

void TCC_SetCompareCapture(uint8_t timer, uint8_t channel, uint32_t compare) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CCBUF[channel] = TCC_CCB_CCB(compare);
    while((peripheral->TCC_SYNCBUSY & TCC_SYNCBUSY_CC_Msk) != 0);
}

static void _DummyInterruptHandler(void) {}

void TCC0_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void TCC0_Handler(void) {
    TCC0_Interrupt();

    TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;
}

void TCC1_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void TCC1_Handler(void) {
    TCC1_Interrupt();

    TCC1_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;
}

void TCC2_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void TCC2_Handler(void) {
    TCC2_Interrupt();

    TCC2_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;
}
