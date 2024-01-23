#include "hal/pm.h"
#include "atsamd21e18a.h"

pm_rcause PM_ResetCause(void) {
    return (pm_rcause) PM_REGS->PM_RCAUSE;
}

void PM_SelectCpuDiv(uint8_t div) {
    PM_REGS->PM_CPUSEL = div;
}

void PM_SelectBusDiv(uint8_t apba_sel, uint8_t apbb_sel, uint8_t apbc_sel) {
    PM_REGS->PM_APBASEL = apba_sel;
    PM_REGS->PM_APBBSEL = apbb_sel;
    PM_REGS->PM_APBCSEL = apbc_sel;
}

void PM_EnablePeripheralClock(pm_peripheral_t peripheral) {
    switch (peripheral) {
        case PM_SERCOM0: PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM0_Msk; return;
        case PM_SERCOM1: PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM1_Msk; return;
        case PM_SERCOM2: PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM2_Msk; return;
        case PM_SERCOM3: PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM3_Msk; return;
        case PM_TCC0: PM_REGS->PM_APBCMASK |= PM_APBCMASK_TCC0_Msk; return;
        case PM_TCC1: PM_REGS->PM_APBCMASK |= PM_APBCMASK_TCC1_Msk; return;
        case PM_TCC2: PM_REGS->PM_APBCMASK |= PM_APBCMASK_TCC2_Msk; return;
        case PM_TC3: PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC3_Msk; return;
        case PM_TC4: PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC4_Msk; return;
        case PM_TC5: PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC5_Msk; return;
    default:
        break;
    }
}

void PM_IdleModeEnter(void) {
    /* Configure Idle Sleep mode */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    PM_REGS->PM_SLEEP = PM_SLEEP_IDLE(0);
    /* Wait for interrupt instruction execution */
    __WFI();
}

void PM_StandbyModeEnter(void) {
    /* Configure Standby Sleep */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    /* Wait for interrupt instruction execution */
    __WFI();
}
