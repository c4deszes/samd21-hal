#if !defined(HAL_ADC_H_)
#define HAL_ADC_H_

#include <stdint.h>

typedef void (*adc_result_callback_t)(uint16_t result);

//void ADC_SetupFreerun(void);

void ADC_SetupSingleShot(void);

void ADC_Stop(void);

uint16_t ADC_ReadSync(uint8_t channel);

void ADC_ReadAsync(uint8_t channel, adc_result_callback_t callback);

void ADC_Reset(void);

#endif // HAL_ADC_H_
