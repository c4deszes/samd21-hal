#if !defined(HAL_DSU_H_)
#define HAL_DSU_H_

#include <stdint.h>

uint32_t DSU_CalculateCRC32(uint32_t initial, void* start, uint32_t length);

uint32_t DSU_GetSerialNumber32(void);

#endif // HAL_DSU_H_
