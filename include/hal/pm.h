#ifndef PM_H_
#define PM_H_

#include <stdint.h>

typedef enum {
    PM_RCAUSE_INVALID,
    PM_RCAUSE_SYSTEM_RESET,                        /**< Cortex System reset */
    PM_RCAUSE_WATCHDOG,                            /**< Watchdog reset */
    PM_RCAUSE_EXTERNAL,                            /**< External reset */
    PM_RCAUSE_NVM,
    PM_RCAUSE_BOD33,
    PM_RCAUSE_BOD12,
    PM_RCAUSE_POR
} pm_rcause;

typedef enum {
    PM_SERCOM0,
    PM_SERCOM1,
    PM_SERCOM2,
    PM_SERCOM3,

    PM_TCC0,
    PM_TCC1,
    PM_TCC2,
    PM_TC3,
    PM_TC4,
    PM_TC5

    // TODO: add the rest
} pm_peripheral_t;

/**
 * @brief Returns the cause of the last reset
 * 
 * @return pm_rcause Reset type
 */
pm_rcause PM_ResetCause(void);

void PM_SelectCpuDiv(uint8_t div);

void PM_SelectBusDiv(uint8_t apba_sel, uint8_t apbb_sel, uint8_t apbc_sel);

void PM_EnablePeripheralClock(pm_peripheral_t peripheral);

//void PM_DisablePeripheralClock(pm_peripheral_t peripheral);

/**
 * @brief CPU enters Idle mode, in this mode only a few interrupt types
 *        can wakeup the CPU
 */
void PM_IdleModeEnter(void);

/**
 * @brief CPU enters Standby mode, in this mode the peripherals that have
 *        standby operations and interrupts enabled can wakeup the CPU
 */
void PM_StandbyModeEnter(void);

#endif
