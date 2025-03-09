#include "hal/dsu.h"

#include "sam.h"

uint32_t DSU_CalculateCRC32(uint32_t initial, void* start, uint32_t length) {
    // TODO: use PAC constants
    PAC1_REGS->PAC_WPCLR = 0x02;
    
    DSU_REGS->DSU_ADDR = DSU_ADDR_ADDR(((uint32_t) start) >> 2);
    DSU_REGS->DSU_LENGTH = DSU_LENGTH_LENGTH(length / 4);
    DSU_REGS->DSU_DATA = initial;
    DSU_REGS->DSU_STATUSA |= (DSU_STATUSA_DONE_Msk);
    DSU_REGS->DSU_CTRL |= (DSU_CTRL_CRC_Msk);

    while ((DSU_REGS->DSU_STATUSA & DSU_STATUSA_DONE_Msk) == 0);

    // TODO: should check BERR flag in STATUSA for bus errors

    uint32_t crc = (DSU_REGS->DSU_DATA) ^ 0xFFFFFFFFUL;

    // TODO: use PAC constants
    PAC1_REGS->PAC_WPSET = 0x02;

    return crc;
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
    return *((uint32_t*)0x0080A00CUL) ^
           *((uint32_t*)0x0080A040UL) ^
           *((uint32_t*)0x0080A044UL) ^
           *((uint32_t*)0x0080A048UL);
}
