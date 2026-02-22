#if !defined(HAL_ADC_H_)
#define HAL_ADC_H_


#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct adc_read_job_t adc_read_job_t;
typedef struct adc_read_result_t adc_read_result_t;

typedef void (*adc_result_callback_t)(adc_read_job_t* job, adc_read_result_t* result);

typedef struct adc_read_job_t {
    uint8_t muxpos;
    uint8_t muxneg;
    adc_result_callback_t callback;
} adc_read_job_t;

typedef struct adc_read_result_t {
    uint16_t result;
} adc_read_result_t;

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


/**
 * @brief Reads the ADC value asynchronously from the specified channel.
 * 
 * @param job A pointer to an adc_read_job_t structure containing the channel and callback information.
 * @return true if the read operation was successfully started, false otherwise.
 */
bool ADC_ReadAsync(adc_read_job_t* job);

/**
 * @brief Resets the ADC to its default state.
 */
void ADC_Reset(void);

#if defined(__cplusplus)
}
#endif

#endif // HAL_ADC_H_
