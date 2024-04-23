#if !defined(BSP_SPI_H_)
#define BSP_SPI_H_

#include <stdint.h>
#include "hal/sercom_common.h"

/**
 * @brief OUT[0], SCK[1], SS[2]
 */
#define SERCOM_SPI_MOSI_PAD0 0

/**
 * @brief OUT[2], SCK[3], SS[1]
 */
#define SERCOM_SPI_MOSI_PAD1 1

/**
 * @brief OUT[3], SCK[1], SS[2]
 */
#define SERCOM_SPI_MOSI_PAD2 2

/**
 * @brief OUT[0], SCK[3], SS[1]
 */
#define SERCOM_SPI_MOSI_PAD3 3

#define SERCOM_SPI_MISO_PAD0 0
#define SERCOM_SPI_MISO_PAD1 1
#define SERCOM_SPI_MISO_PAD2 2
#define SERCOM_SPI_MISO_PAD3 3

typedef enum {
    sercom_spi_cpha_leading = 0,
    sercom_spi_cpha_trailing = 1
} sercom_spi_cpha_t;

typedef enum {
    sercom_spi_cpol_idle_low = 0,
    sercom_spi_cpol_idle_high = 1
} sercom_spi_cpol_t;

typedef enum {
    sercom_spi_dataorder_msb = 0,
    sercom_spi_dataorder_lsb = 1
} sercom_spi_dataorder_t;

void SERCOM_SPI_SetupMaster(uint8_t sercom, uint32_t clock_in, uint32_t datarate,
                            sercom_spi_dataorder_t dataorder, sercom_spi_cpha_t phase,
                            sercom_spi_cpol_t polarity,
                            uint8_t mosi_pad, uint8_t miso_pad);

void SERCOM_SPI_Enable(uint8_t sercom);

void SERCOM_SPI_Disable(uint8_t sercom);

uint8_t SERCOM_SPI_TransferByte(uint8_t sercom, uint8_t data);

#endif // BSP_SPI_H_
