#include "hal/dsu.h"
#include "sim/dsu_internal.h"

uint32_t DSU_CalculateCRC32(uint32_t initial, void* start, uint32_t length) {
    return DSU_SoftwareCRC32(initial, start, length);
}

uint32_t DSU_SoftwareCRC32(uint32_t initial, void* start, uint32_t length)
{
    uint32_t crc = initial;
    uint32_t* ptr = (uint32_t*) start;
    uint32_t* end = (uint32_t*) ((uint32_t) start + length);

    while (ptr < end) {
        crc = crc ^ *ptr++;
        for (int i = 0; i < 32; i++) {
            crc = (crc >> 1) ^ (DSU_CRC32_POLYNOMIAL & (-(crc & 1)));
        }
    }

    return crc;
}

uint32_t DSU_GetSerialNumber32(void) {
    return DSU_SerialNumber;
}
