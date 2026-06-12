
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
#if defined(__ATSAMD21G__) || defined(__ATSAMD21J__)
        case SERCOM4: return SERCOM4_REGS;
        case SERCOM5: return SERCOM5_REGS;
#endif
    }
    return NULL;
}

IRQn_Type SERCOM_GetInterrupt(uint8_t sercom) {
    switch (sercom) {
        case SERCOM0: return SERCOM0_IRQn;
        case SERCOM1: return SERCOM1_IRQn;
        case SERCOM2: return SERCOM2_IRQn;
        case SERCOM3: return SERCOM3_IRQn;
#if defined(__ATSAMD21G__) || defined(__ATSAMD21J__)
        case SERCOM4: return SERCOM4_IRQn;
        case SERCOM5: return SERCOM5_IRQn;
#endif
    }
    // TODO: handle?
    return PERIPH_MAX_IRQn + 1;
}

static void _DummyInterruptHandler(void) {}

void SERCOM0_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM1_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM2_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM3_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

#if defined(__ATSAMD21G__) || defined(__ATSAMD21J__)
void SERCOM4_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void SERCOM5_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));
#endif

// TODO: shouldn't be done like this, because I2C and SPI use different flags

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

#if defined(__ATSAMD21G__) || defined(__ATSAMD21J__)
void SERCOM4_Handler(void) {
    SERCOM4_Interrupt();

    SERCOM4_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

void SERCOM5_Handler(void) {
    SERCOM5_Interrupt();

    SERCOM5_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}
#endif
