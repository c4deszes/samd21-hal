#include "hal/dsu.h"

#include "atsamd21e18a.h"

uint32_t DSU_CalculateCRC32(uint32_t initial, void* start, uint32_t length) {
    DSU_REGS->DSU_ADDR = start;
    DSU_REGS->DSU_LENGTH = length;
    DSU_REGS->DSU_DATA = initial;
    DSU_REGS->DSU_CTRL |= (DSU_CTRL_CRC_Msk);

    while ((DSU_REGS->DSU_STATUSA & DSU_STATUSA_DONE_Msk) == 0);

    // TODO: should check BERR flag in STATUSA for bus errors

    return DSU_REGS->DSU_DATA;
}
