#include "hal/sercom_usart.h"
#include "hal/sercom_internal.h"

#include "sam.h"

#include <stdlib.h>

ringbuffer8_t* tx_buffers[4];
ringbuffer8_t* rx_buffers[4];

// goal for now is 9600-19200, 8N1, using interrupts
void SERCOM_USART_SetupAsync(uint8_t sercom, uint32_t clock,
                             uint32_t baudrate, uint8_t tx_pad, uint8_t rx_pad,
                             ringbuffer8_t* tx_buffer, ringbuffer8_t* rx_buffer) {

    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    tx_buffers[sercom] = tx_buffer;
    rx_buffers[sercom] = rx_buffer;

    peripheral->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | 
                                           SERCOM_USART_INT_CTRLA_RXPO(rx_pad) |
                                           SERCOM_USART_INT_CTRLA_TXPO(tx_pad) |
                                           SERCOM_USART_INT_CTRLA_DORD_Msk |    // is this needed?
                                           SERCOM_USART_INT_CTRLA_IBON_Msk |    // is this needed?
                                           SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_NO_PARITY |
                                           SERCOM_USART_INT_CTRLA_SAMPR_8X_FRACTIONAL;

    uint32_t baud = (clock * 8) / (8 * baudrate);
    peripheral->USART_INT.SERCOM_BAUD = SERCOM_USART_INT_BAUD_FRAC_FP(baud % 8) |
                                          SERCOM_USART_INT_BAUD_FRAC_BAUD(baud / 8);

    peripheral->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT |
                                           SERCOM_USART_INT_CTRLB_SBMODE_1_BIT |
                                           SERCOM_USART_INT_CTRLB_RXEN_Msk |
                                           SERCOM_USART_INT_CTRLB_TXEN_Msk;

    NVIC_EnableIRQ(SERCOM_GetInterrupt(sercom));
    NVIC_SetPriority(SERCOM_GetInterrupt(sercom), 2);
}

void SERCOM_USART_Enable(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);

    peripheral->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

    while((peripheral->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk) != 0);

    peripheral->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk | SERCOM_USART_INT_INTENSET_RXBRK_Msk;
}

static volatile bool writing = false;

static inline void SERCOM_USART_TX_INT_DISABLE(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_DRE_Msk;
}

static inline void SERCOM_USART_TX_INT_ENABLE(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_DRE_Msk;
}

static inline void SERCOM_USART_RX_INT_DISABLE(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENSET_RXC_Msk;
}

static inline void SERCOM_USART_RX_INT_ENABLE(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    peripheral->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk;
}

void SERCOM_USART_WriteData(uint8_t sercom, uint8_t* data, uint8_t size) {
    while(writing);

    for (uint8_t i = 0; i < size; i++) {
        ringbuffer8_write(tx_buffers[sercom], data[i]);
    }

    //SERCOM_USART_FlushOutput(sercom);
}

static inline void SERCOM_USART_ISR_TX_Handler(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    if (ringbuffer8_available(tx_buffers[sercom])) {
        uint8_t data = ringbuffer8_read(tx_buffers[sercom]);
        peripheral->USART_INT.SERCOM_DATA = (uint16_t) data;
    }
    else {
        SERCOM_USART_TX_INT_DISABLE(sercom);
        writing = false;
    }
}

void SERCOM_USART_FlushOutput(uint8_t sercom) {
    writing = true;
    SERCOM_USART_TX_INT_ENABLE(sercom);
    SERCOM_USART_ISR_TX_Handler(sercom);
}

uint8_t SERCOM_USART_Available(uint8_t sercom) {
    SERCOM_USART_RX_INT_DISABLE(sercom);
    uint16_t avail = ringbuffer8_available(rx_buffers[sercom]);
    SERCOM_USART_RX_INT_ENABLE(sercom);

    return avail;
}

uint8_t SERCOM_USART_Read(uint8_t sercom) {
    SERCOM_USART_RX_INT_DISABLE(sercom);
    uint8_t data = ringbuffer8_read(rx_buffers[sercom]);
    SERCOM_USART_RX_INT_ENABLE(sercom);

    return data;
}

static inline void SERCOM_USART_ISR_RX_Handler(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    if (peripheral->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXBRK_Msk) {
        /* Clear the receive break interrupt flag */
        peripheral->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_RXBRK_Msk;
        // we ignore break for now
    }
    if (peripheral->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) {
        // RXC flag is cleared after reading the data
        if (!ringbuffer8_write(rx_buffers[sercom], peripheral->USART_INT.SERCOM_DATA)) {
            // TODO: call error handler
        }
    }
}

void SERCOM_USART_InterruptHandler(uint8_t sercom) {
    sercom_registers_t* peripheral = SERCOM_GetPeripheral(sercom);
    if((peripheral->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_DRE_Msk) && (peripheral->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk))
    {
        SERCOM_USART_ISR_TX_Handler(sercom);
    }

    /* Checks for receive complete empty flag */
    if((peripheral->USART_INT.SERCOM_INTENSET & (SERCOM_USART_INT_INTENSET_RXC_Msk | SERCOM_USART_INT_INTENSET_RXBRK_Msk)) && (peripheral->USART_INT.SERCOM_INTFLAG & (SERCOM_USART_INT_INTFLAG_RXC_Msk | SERCOM_USART_INT_INTFLAG_RXBRK_Msk)))
    {
        SERCOM_USART_ISR_RX_Handler(sercom);
    }

    /* Checks for error flag */
    // if((peripheral->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_ERROR_Msk) && (peripheral->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk))
    // {
    // }
}
