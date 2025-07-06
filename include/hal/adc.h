#if !defined(HAL_ADC_H_)
#define HAL_ADC_H_

#include <stdint.h>

typedef void (*adc_result_callback_t)(uint16_t result);

/**
 * @brief Sets up the ADC for single-shot mode.
 * 
 * In this mode, the ADC will perform a single conversion when triggered. Use ADC_ReadSync or
 * ADC_ReadAsync to perform a measurement.
 */
void ADC_SetupSingleShot(void);

/**
 * @brief Disables the ADC.
 */
void ADC_Disable(void);

/**
 * @brief Reads the ADC value synchronously from the specified channel.
 * 
 * @param channel The ADC channel to read from (0-16).
 * @return The ADC result as a 16-bit unsigned integer.
 */
uint16_t ADC_ReadSync(uint8_t channel);

void ADC_ReadAsync(uint8_t channel, adc_result_callback_t callback);

/**
 * @brief Resets the ADC to its default state.
 */
void ADC_Reset(void);

#endif // HAL_ADC_H_
