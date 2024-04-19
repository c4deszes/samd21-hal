#if !defined(HAL_TCC_H_)
#define HAL_TCC_H_

#include <stdint.h>
#include <stdbool.h>

#define TCC0 0
#define TCC1 1
#define TCC2 2

typedef struct {
    uint32_t cc;
    bool drv_inv;
} tcc_channel_setting_t;

// TODO: support for other things, like prescaler, mode, interrupts
void TCC_SetupTrigger(uint8_t timer, uint32_t period);

// TODO: support for other things, like prescaler, mode, interrupts
void TCC_SetupNormalPwm(uint8_t timer, uint32_t period, tcc_channel_setting_t channels[4]);

void TCC_Enable(uint8_t timer);

void TCC_Disable(void);

void TCC_Reset(uint8_t timer);

void TCC_SetPeriod(uint8_t timer, uint32_t period);

void TCC_SetCompareCapture(uint8_t timer, uint8_t channel, uint32_t compare);

// TODO: update signature to include int. cause
void TCC0_Interrupt(void);

// TODO: update signature to include int. cause
void TCC1_Interrupt(void);

// TODO: update signature to include int. cause
void TCC2_Interrupt(void);

#endif // HAL_TCC_H_
