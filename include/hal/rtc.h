#if !defined(HAL_RTC_H_)
#define HAL_RTC_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

void RTC_Initialize(void);

void RTC_Start(void);

uint32_t RTC_GetCount(void);

#if defined(__cplusplus)
}
#endif

#endif // HAL_RTC_H_
