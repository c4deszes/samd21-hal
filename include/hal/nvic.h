#if !defined(HAL_NVIC_H_)
#define HAL_NVIC_H_

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Enables interrupts
 */
void NVIC_Initialize(void);

void NVIC_Reset(void);

#if defined(__cplusplus)
}
#endif

#endif // HAL_NVIC_H_
