#include "hal/tcc.h"

void TCC_SetupTrigger(uint8_t timer, uint32_t period) {
    
}

void TCC_SetupNormalPwm(uint8_t timer, uint32_t period, tcc_channel_setting_t channels[4]) {
    
}

void TCC_Enable(uint8_t timer) {

}

void TCC_Reset(uint8_t timer) {

}

void TCC_SetPeriod(uint8_t timer, uint32_t period) {

}

void TCC_SetCompareCapture(uint8_t timer, uint8_t channel, uint32_t compare) {

}

static void _DummyInterruptHandler(void) {}

void TCC0_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void TCC1_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void TCC2_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));
