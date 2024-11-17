#include "hal/sercom_spi.h"

#include <stdlib.h>
#include "sam.h"

static inline uint8_t spi_clock_calculate(uint32_t clock_in, uint32_t datarate) {
    return clock_in / (2 * datarate) - 1;
}

void SERCOM_SPI_SetupMaster(uint8_t sercom, uint32_t clock_in, uint32_t datarate,
                            sercom_spi_dataorder_t dataorder, sercom_spi_cpha_t phase,
                            sercom_spi_cpol_t polarity,
                            uint8_t mosi_pad, uint8_t miso_pad) {

    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->SPIM.SERCOM_CTRLA =	SERCOM_SPIM_CTRLA_MODE_SPI_MASTER |
                                    SERCOM_SPIM_CTRLA_DOPO(mosi_pad) |
                                    SERCOM_SPIM_CTRLA_DIPO(miso_pad) |
                                    SERCOM_SPIM_CTRLA_DORD(dataorder);

    peripheral->SPIM.SERCOM_CTRLB = SERCOM_SPIM_CTRLB_CHSIZE_8_BIT |
                                    SERCOM_SPIM_CTRLB_RXEN_Msk;

    peripheral->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_CPHA(phase) |
                                    SERCOM_SPIM_CTRLA_CPOL(polarity);

    peripheral->SPIM.SERCOM_BAUD = spi_clock_calculate(clock_in, datarate);
}

void SERCOM_SPI_Enable(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_ENABLE_Msk;

    while((peripheral->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk) != 0);
}

void SERCOM_SPI_Disable(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->SPIM.SERCOM_CTRLA &= ~(SERCOM_SPIM_CTRLA_ENABLE_Msk);

    while((peripheral->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk) != 0);
}

uint8_t SERCOM_SPI_TransferByte(uint8_t sercom, uint8_t data) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->SPIM.SERCOM_DATA = data;

    while((peripheral->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk) == 0);

    return peripheral->SPIM.SERCOM_DATA;
}
