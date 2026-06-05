#if !defined(HAL_AC_H_)
#define HAL_AC_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

//void AC_SetupComparator(uint8_t comp, );

bool AC_GetComparatorState(uint8_t comp);

void AC_Enable(void);

void AC_Reset(void);

#if defined(__cplusplus)
}
#endif

#endif // HAL_AC_H_
