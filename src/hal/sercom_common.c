
#include "hal/sercom_common.h"

#include "hal/sercom_internal.h"

#include "sam.h"
#include <stdlib.h>

sercom_registers_t* SERCOM_GetPeripheral(uint8_t sercom) {
    switch (sercom) {
        case SERCOM0: return SERCOM0_REGS;
        case SERCOM1: return SERCOM1_REGS;
        case SERCOM2: return SERCOM2_REGS;
        case SERCOM3: return SERCOM3_REGS;
    }
    return NULL;
}

IRQn_Type SERCOM_GetInterrupt(uint8_t sercom) {
    switch (sercom) {
        case SERCOM0: return SERCOM0_IRQn;
        case SERCOM1: return SERCOM1_IRQn;
        case SERCOM2: return SERCOM2_IRQn;
        case SERCOM3: return SERCOM3_IRQn;
    }
    // TODO: handle?
    return PERIPH_MAX_IRQn + 1;
}

static void _DummyInterruptHandler(void) {}

void SERCOM0_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM1_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM2_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM3_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM0_Handler(void) {
    SERCOM0_Interrupt();

    SERCOM0_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

void SERCOM1_Handler(void) {
    SERCOM1_Interrupt();

    SERCOM1_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

void SERCOM2_Handler(void) {
    SERCOM2_Interrupt();

    SERCOM2_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

void SERCOM3_Handler(void) {
    SERCOM3_Interrupt();

    SERCOM3_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}
