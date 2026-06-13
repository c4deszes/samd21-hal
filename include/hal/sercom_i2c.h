#if !defined(HAL_SERCOM_I2C_H_)
#define HAL_SERCOM_I2C_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "hal/sercom_common.h"

typedef enum {
    sercom_i2c_ok = 0,
    sercom_i2c_invalid_arg,
    sercom_i2c_timeout,
    sercom_i2c_nack,
    sercom_i2c_bus_error,
    sercom_i2c_arbitration_lost,
} sercom_i2c_result_t;

void SERCOM_I2C_SetupMaster(uint8_t sercom, uint32_t clock_in, uint32_t baudrate);

void SERCOM_I2C_Enable(uint8_t sercom);

void SERCOM_I2C_Disable(uint8_t sercom);

sercom_i2c_result_t SERCOM_I2C_Write(uint8_t sercom, uint8_t address,
                                     const uint8_t* data, uint16_t size);

sercom_i2c_result_t SERCOM_I2C_WriteEx(uint8_t sercom, uint8_t address,
                                       const uint8_t* data, uint16_t size,
                                       bool stop);

sercom_i2c_result_t SERCOM_I2C_Read(uint8_t sercom, uint8_t address,
                                    uint8_t* data, uint16_t size);

sercom_i2c_result_t SERCOM_I2C_ReadEx(uint8_t sercom, uint8_t address,
                                      uint8_t* data, uint16_t size,
                                      bool stop);

sercom_i2c_result_t SERCOM_I2C_WriteRead(uint8_t sercom, uint8_t address,
                                         const uint8_t* tx_data, uint16_t tx_size,
                                         uint8_t* rx_data, uint16_t rx_size);

#if defined(__cplusplus)
}
#endif

#endif // HAL_SERCOM_I2C_H_
