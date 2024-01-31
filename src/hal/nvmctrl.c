#include "hal/nvmctrl.h"

#include "atsamd21e18a.h"

void NVMCTRL_EraseRow(uint32_t address) {
    NVMCTRL_REGS->NVMCTRL_ADDR = address >> 1U;
    NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER;

    while ((NVMCTRL_REGS->NVMCTRL_INTFLAG & NVMCTRL_INTFLAG_READY_Msk) == 0);
}

void NVMCTRL_PageBufferClear(void) {
    NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_PBC;
    while ((NVMCTRL_REGS->NVMCTRL_INTFLAG & NVMCTRL_INTFLAG_READY_Msk) == 0);
}

void NVMCTRL_WritePage(uint32_t address) {
    NVMCTRL_REGS->NVMCTRL_ADDR = address >> 1U;
    NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP;
    while ((NVMCTRL_REGS->NVMCTRL_INTFLAG & NVMCTRL_INTFLAG_READY_Msk) == 0);
}

void NVMCTRL_SetAutoPageWrite(bool enabled) {
    if (enabled) {
        NVMCTRL_REGS->NVMCTRL_CTRLB &= ~(NVMCTRL_CTRLB_MANW_Msk);
    }
    else {
        NVMCTRL_REGS->NVMCTRL_CTRLB |= NVMCTRL_CTRLB_MANW_Msk;
    }
}

void NVMCTRL_SetReadWaitStates(uint8_t wait_states) {
    // TODO: consider the input value
    NVMCTRL_REGS->NVMCTRL_CTRLB |= NVMCTRL_CTRLB_RWS_HALF_Val;
}

uint16_t NVMCTRL_GetPageSize(void) {
    return (1U << (3U + ((NVMCTRL_REGS->NVMCTRL_PARAM & NVMCTRL_PARAM_PSZ_Msk) >> NVMCTRL_PARAM_PSZ_Pos)));
}

uint16_t NVMCTRL_GetPageCount(void) {
    return (NVMCTRL_REGS->NVMCTRL_PARAM & NVMCTRL_PARAM_NVMP_Msk) >> NVMCTRL_PARAM_NVMP_Pos;
}