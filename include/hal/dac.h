#ifndef HAL_DAC_H
#define HAL_DAC_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

void DAC_Setup(void);

void DAC_SetValue(uint16_t value);

#if defined(__cplusplus)
}
#endif

#endif /* HAL_DAC_H */
