#include "hal/sercom_spi.h"

void SERCOM_SPI_SetupMaster(uint8_t sercom, uint32_t clock_in, uint32_t datarate,
                            sercom_spi_dataorder_t dataorder, sercom_spi_cpha_t phase,
                            sercom_spi_cpol_t polarity,
                            uint8_t mosi_pad, uint8_t miso_pad)
{
    /** This function is not implemented in the simulator. */
}

void SERCOM_SPI_Enable(uint8_t sercom)
{
    /** This function is not implemented in the simulator. */
}

void SERCOM_SPI_Disable(uint8_t sercom)
{
    /** This function is not implemented in the simulator. */
}

uint8_t SERCOM_SPI_TransferByte(uint8_t sercom, uint8_t data)
{
    /** This function is not implemented in the simulator. */
    return 0;
}
