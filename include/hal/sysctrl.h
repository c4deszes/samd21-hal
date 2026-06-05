#if !defined(HAL_SYSCTRL_H_)
#define HAL_SYSCTRL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Enables the internal 32.768kHz oscillator
 */
void SYSCTRL_EnableInternalOSC32K(void);

// TODO: support for changing the prescaler
/**
 * @brief Configures and enables the internal 8MHz oscillator
 */
void SYSCTRL_ConfigureOSC8M(void);

void SYSCTRL_InitializeDFLL(uint32_t freq_in, uint32_t freq_out);

#if defined(__cplusplus)
}
#endif

#endif // HAL_SYSCTRL_H_
