#ifndef HAL_SYSTICK_H
#define HAL_SYSTICK_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

void SYSTICK_Setup(uint32_t ticks);

#if defined(__cplusplus)
}
#endif

#endif /* HAL_SYSTICK_H */
