#include "hal/sercom_i2c.h"

#include "hal/sercom_internal.h"

#include "sam.h"

#include <stddef.h>
#include <stdbool.h>

#define SERCOM_I2C_TIMEOUT_LOOPS 100000U
#define SERCOM_I2C_BUSSTATE_IDLE 1U

static inline uint32_t i2c_baud_calculate(uint32_t clock_in, uint32_t baudrate) {
    uint32_t baud = clock_in / (2U * baudrate);
    if (baud > 5U) {
        baud -= 5U;
    }
    else {
        baud = 0U;
    }

    if (baud > 255U) {
        baud = 255U;
    }
    return baud;
}

static sercom_i2c_result_t i2c_status_to_result(sercom_registers_t* peripheral) {
    uint16_t status = peripheral->I2CM.SERCOM_STATUS;

    if ((status & SERCOM_I2CM_STATUS_BUSERR_Msk) != 0U) {
        peripheral->I2CM.SERCOM_STATUS = SERCOM_I2CM_STATUS_BUSERR_Msk;
        return sercom_i2c_bus_error;
    }
    if ((status & SERCOM_I2CM_STATUS_ARBLOST_Msk) != 0U) {
        peripheral->I2CM.SERCOM_STATUS = SERCOM_I2CM_STATUS_ARBLOST_Msk;
        return sercom_i2c_arbitration_lost;
    }
    if ((status & SERCOM_I2CM_STATUS_RXNACK_Msk) != 0U) {
        peripheral->I2CM.SERCOM_STATUS = SERCOM_I2CM_STATUS_RXNACK_Msk;
        return sercom_i2c_nack;
    }

    return sercom_i2c_ok;
}

static inline sercom_i2c_result_t i2c_wait_sync(sercom_registers_t* peripheral,
                                                 uint32_t mask) {
    uint32_t timeout = SERCOM_I2C_TIMEOUT_LOOPS;
    while ((peripheral->I2CM.SERCOM_SYNCBUSY & mask) != 0U) {
        if (--timeout == 0U) {
            return sercom_i2c_timeout;
        }
    }

    return sercom_i2c_ok;
}

static sercom_i2c_result_t i2c_wait_flag(sercom_registers_t* peripheral, uint8_t mask) {
    uint32_t timeout = SERCOM_I2C_TIMEOUT_LOOPS;
    while ((peripheral->I2CM.SERCOM_INTFLAG & mask) == 0U) {
        if ((peripheral->I2CM.SERCOM_INTFLAG & SERCOM_I2CM_INTFLAG_ERROR_Msk) != 0U) {
            return i2c_status_to_result(peripheral);
        }
        if (--timeout == 0U) {
            return sercom_i2c_timeout;
        }
    }

    return i2c_status_to_result(peripheral);
}

static inline void i2c_write_ctrlb(sercom_registers_t* peripheral, uint32_t ctrlb_masked_bits) {
    uint32_t ctrlb = peripheral->I2CM.SERCOM_CTRLB;
    ctrlb &= ~(SERCOM_I2CM_CTRLB_CMD_Msk | SERCOM_I2CM_CTRLB_ACKACT_Msk);
    ctrlb |= ctrlb_masked_bits;
    peripheral->I2CM.SERCOM_CTRLB = ctrlb;
}

static sercom_i2c_result_t i2c_send_stop(sercom_registers_t* peripheral) {
    i2c_write_ctrlb(peripheral, SERCOM_I2CM_CTRLB_CMD(3U));
    return i2c_wait_sync(peripheral, SERCOM_I2CM_SYNCBUSY_SYSOP_Msk);
}

static sercom_i2c_result_t i2c_write_internal(sercom_registers_t* peripheral,
                                               uint8_t address,
                                               const uint8_t* data,
                                               uint16_t size,
                                               bool send_stop) {
    sercom_i2c_result_t result;

    peripheral->I2CM.SERCOM_ADDR = SERCOM_I2CM_ADDR_ADDR((uint32_t)address << 1U);
    result = i2c_wait_flag(peripheral, SERCOM_I2CM_INTFLAG_MB_Msk);
    if (result != sercom_i2c_ok) {
        if (send_stop) {
            (void)i2c_send_stop(peripheral);
        }
        return result;
    }

    for (uint16_t i = 0; i < size; i++) {
        peripheral->I2CM.SERCOM_DATA = data[i];
        result = i2c_wait_flag(peripheral, SERCOM_I2CM_INTFLAG_MB_Msk);
        if (result != sercom_i2c_ok) {
            if (send_stop) {
                (void)i2c_send_stop(peripheral);
            }
            return result;
        }
    }

    if (send_stop) {
        return i2c_send_stop(peripheral);
    }

    return sercom_i2c_ok;
}

void SERCOM_I2C_SetupMaster(uint8_t sercom, uint32_t clock_in, uint32_t baudrate) {
    if (baudrate == 0U) {
        return;
    }

    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    if (peripheral == NULL) {
        return;
    }

    peripheral->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
                                    SERCOM_I2CM_CTRLA_SDAHOLD_75NS;
    peripheral->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_SMEN_Msk;
    peripheral->I2CM.SERCOM_BAUD = SERCOM_I2CM_BAUD_BAUD(i2c_baud_calculate(clock_in, baudrate));
}

void SERCOM_I2C_Enable(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    if (peripheral == NULL) {
        return;
    }

    peripheral->I2CM.SERCOM_CTRLA |= SERCOM_I2CM_CTRLA_ENABLE_Msk;
    while ((peripheral->I2CM.SERCOM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_ENABLE_Msk) != 0U);

    peripheral->I2CM.SERCOM_STATUS = SERCOM_I2CM_STATUS_BUSSTATE(SERCOM_I2C_BUSSTATE_IDLE);
    while ((peripheral->I2CM.SERCOM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_SYSOP_Msk) != 0U);
}

void SERCOM_I2C_Disable(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    if (peripheral == NULL) {
        return;
    }

    peripheral->I2CM.SERCOM_CTRLA &= ~(SERCOM_I2CM_CTRLA_ENABLE_Msk);
    while ((peripheral->I2CM.SERCOM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_ENABLE_Msk) != 0U);
}

sercom_i2c_result_t SERCOM_I2C_Write(uint8_t sercom, uint8_t address,
                                     const uint8_t* data, uint16_t size) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);

    if ((size > 0U) && (data == NULL)) {
        return sercom_i2c_invalid_arg;
    }
    if (peripheral == NULL) {
        return sercom_i2c_invalid_arg;
    }

    return i2c_write_internal(peripheral, address, data, size, true);
}

sercom_i2c_result_t SERCOM_I2C_Read(uint8_t sercom, uint8_t address,
                                    uint8_t* data, uint16_t size) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    sercom_i2c_result_t result;

    if ((size > 0U) && (data == NULL)) {
        return sercom_i2c_invalid_arg;
    }
    if (peripheral == NULL) {
        return sercom_i2c_invalid_arg;
    }
    if (size == 0U) {
        return sercom_i2c_ok;
    }

    /* Reset ACK action between transactions: previous read leaves NACK set on last byte. */
    i2c_write_ctrlb(peripheral, 0U);

    peripheral->I2CM.SERCOM_ADDR = SERCOM_I2CM_ADDR_ADDR(((uint32_t)address << 1U) | 1U);

    for (uint16_t i = 0; i < size; i++) {
        result = i2c_wait_flag(peripheral, SERCOM_I2CM_INTFLAG_SB_Msk);
        if (result != sercom_i2c_ok) {
            (void)i2c_send_stop(peripheral);
            return result;
        }

        if (i == (size - 1U)) {
            i2c_write_ctrlb(peripheral, SERCOM_I2CM_CTRLB_ACKACT_Msk |
                                        SERCOM_I2CM_CTRLB_CMD(3U));
        }

        data[i] = peripheral->I2CM.SERCOM_DATA;
    }

    return i2c_wait_sync(peripheral, SERCOM_I2CM_SYNCBUSY_SYSOP_Msk);
}

sercom_i2c_result_t SERCOM_I2C_WriteRead(uint8_t sercom, uint8_t address,
                                         const uint8_t* tx_data, uint16_t tx_size,
                                         uint8_t* rx_data, uint16_t rx_size) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    sercom_i2c_result_t result;

    if ((tx_size > 0U) && (tx_data == NULL)) {
        return sercom_i2c_invalid_arg;
    }
    if ((rx_size > 0U) && (rx_data == NULL)) {
        return sercom_i2c_invalid_arg;
    }
    if (peripheral == NULL) {
        return sercom_i2c_invalid_arg;
    }

    if (tx_size == 0U) {
        return SERCOM_I2C_Read(sercom, address, rx_data, rx_size);
    }

    result = i2c_write_internal(peripheral, address, tx_data, tx_size, false);
    if (result != sercom_i2c_ok) {
        return result;
    }

    return SERCOM_I2C_Read(sercom, address, rx_data, rx_size);
}
