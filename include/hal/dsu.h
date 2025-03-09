#if !defined(HAL_DSU_H_)
#define HAL_DSU_H_

#include <stdint.h>

#define DSU_CRC32_INITIAL 0xFFFFFFFFUL
#define DSU_CRC32_POLYNOMIAL 0xEDB88320UL

/**
 * @brief Calculate CRC32 using the DSU hardware, uses DSU_CRC32_POLYNOMIAL as the polynomial
 * 
 * @param initial Initial CRC value, usually 0xFFFFFFFF
 * @param start Start address of the data, must be 32-bit aligned
 * @param length Length of the data in bytes, must be a multiple of 4
 * @return uint32_t Calculated CRC32
 */
uint32_t DSU_CalculateCRC32(uint32_t initial, void* start, uint32_t length);

/**
 * @brief Calculate CRC32 using software implementation, uses DSU_CRC32_POLYNOMIAL as the polynomial
 * 
 * @param initial Initial CRC value, usually 0xFFFFFFFF
 * @param start Start address of the data, must be 32-bit aligned
 * @param length Length of the data in bytes, must be a multiple of 4
 * @return uint32_t Calculated CRC32
 */
uint32_t DSU_SoftwareCRC32(uint32_t initial, void* start, uint32_t length);

/**
 * @brief Returns a 32-bit serial number from the device, it's a combination of 4 32-bit words.
 * 
 * Not guaranteed to be unique.
 * 
 * @return uint32_t Serial number
 */
uint32_t DSU_GetSerialNumber32(void);

#endif // HAL_DSU_H_
