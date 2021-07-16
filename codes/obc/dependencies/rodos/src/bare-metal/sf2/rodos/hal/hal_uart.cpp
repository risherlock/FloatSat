/*
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Michael Ruffer
 */

/*
 * TODO hal_uart.cpp - Es ist eine eigene Implementierung mit Fifo notwendig damit es ordentlich laeuft - Martin B.
 */
#include <rodos.h>
#include <hal/hal_uart.h>
#include <hal/hal_gpio.h>

namespace RODOS {

#include "../../drivers/mss_uart.h"
#include "../../drivers/mss_uart_regs.h"
#include "../../drivers/mss_gpio.h"
#include "../../CMSIS/hw_reg_io.h"

#define MSS_UART_DATA_READY ((uint8_t)0x01)

static constexpr UART_IDX UART_IDX_MIN = UART_IDX0;
static constexpr UART_IDX UART_IDX_MAX = UART_IDX1;

#define DMA_TIMEOUT 10 * MILLISECONDS


void rxHandler(mss_uart_instance_t* this_uart);
void txHandler(mss_uart_instance_t* this_uart);

class HW_HAL_UART {
    friend class ReceiveTrigger;
    friend class HAL_UART;

    uint8_t   idx;
    int       hwFlowCtrl;
    uint32_t  baudrate;
    HAL_UART* hal_uart;
    int       uartRxError;

    GPIO_PIN tx;
    GPIO_PIN rx;
    GPIO_PIN rts;
    GPIO_PIN cts;

    BlockFifo<uint8_t, UART_BUF_SIZE> receiveBuffer;
    BlockFifo<uint8_t, UART_BUF_SIZE> transmittBuffer;

  public:
    mss_uart_instance_t* UART;
    void                 rxInt(void); // only for receive interrupt
    void                 txInt(void); // only for send interrupt
};

HW_HAL_UART UART_contextArray[UART_IDX_MAX + 1];

HAL_UART::HAL_UART(UART_IDX uartIdx, [[gnu::unused]] GPIO_PIN txPin, [[gnu::unused]] GPIO_PIN rxPin,
                   [[gnu::unused]] GPIO_PIN rtsPin, [[gnu::unused]] GPIO_PIN ctsPin) : HAL_UART(uartIdx) {
    // as the SF2 doesn't have variable UART pins, this constructor just calls the simple one.
}


HAL_UART::HAL_UART(UART_IDX uartIdx) {
    if((uartIdx < UART_IDX_MIN) || (uartIdx > UART_IDX_MAX)) {
        RODOS_ERROR("UART number not available on SF2 - only 0 and 1");
        return;
    }

    context           = &UART_contextArray[uartIdx];
    context->idx      = uartIdx;
    context->hal_uart = this;
}


/*
 * USART
 * - all USART will be initialized in 8N1 mode
 */
int32_t HAL_UART::init(uint32_t iBaudrate) {
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        return -1;
    }
    return config(UART_PARAMETER_BAUDRATE, *reinterpret_cast<int32_t*>(&iBaudrate));
}


int32_t HAL_UART::config(UART_PARAMETER_TYPE type, int32_t paramVal) {
    uint8_t idx = context->idx;
    if(idx == 0) {
        context->UART = &g_mss_uart0;
    } else if(idx == 1) {
        context->UART = &g_mss_uart1;
    } else {
        return -1;
    }

    switch(type) {
        case UART_PARAMETER_BAUDRATE:
            if(paramVal > 0) {
                context->baudrate = static_cast<uint32_t>(paramVal);
            } else {
                return -1;
            }
            break;

        case UART_PARAMETER_HW_FLOW_CONTROL:
            /// TODO UART_PARAMETER_HW_FLOW_CONTROL - really needed?
            break; // end case UART_PARAMETER_HW_FLOW_CONTROL

        default:
            return -1;
    }

    // initialize the UART with the function provided by mss_uart.c from the sf2 drivers
    MSS_UART_init(context->UART, context->baudrate,
                  MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_set_tx_handler(context->UART, txHandler);
    MSS_UART_set_rx_handler(context->UART, rxHandler, MSS_UART_FIFO_SINGLE_BYTE);

    return 0;
}

void HAL_UART::reset(void) {
}

size_t HAL_UART::write(const void* sendBuf, size_t size) {
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        return 0;
    }
    const uint8_t* buf = static_cast<const uint8_t*>(sendBuf);
    size_t         i   = 0;
    size_t   spaceinbuffer;
    uint8_t* p = context->transmittBuffer.getBufferToWrite(spaceinbuffer);
    if(p != NULL) {
        if(spaceinbuffer < size) {
            size = spaceinbuffer;
        }
        for(i = 0; i < size; i++) {
            p[i] = buf[i];
        }
        NVIC_DisableIRQ(context->UART->irqn);
        context->transmittBuffer.writeConcluded(size);
        // enables TX interrupt
        set_bit_reg8(&(context->UART->hw_reg->IER), ETBEI);
        // Enable UART instance interrupt in Cortex-M3 NVIC.
        NVIC_EnableIRQ(context->UART->irqn);

        while(isWriteFinished() == false) {} // TODO: busy waiting is not good

        return size;
    } else {
        return 0;
    }
}


size_t HAL_UART::read(void* recBuf, size_t size) {
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        return 0;
    }

    uint8_t* buf = static_cast<uint8_t*>(recBuf);

    size_t      readCnt = 0;
    size_t      i       = 0;
    uint8_t* p       = context->receiveBuffer.getBufferToRead(readCnt);
    if(p) {
        if(readCnt > size) {
            readCnt = size;
        }
        for(i = 0; i < readCnt; i++) {
            buf[i] = p[i];
        }
        context->receiveBuffer.readConcluded(readCnt);
        return readCnt;
    } else {
        return 0;
    }
}

int16_t HAL_UART::getcharNoWait() {
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        return -1;
    }
    uint8_t c             = 0;
    bool    dataAvailible = context->receiveBuffer.get(c);

    if(dataAvailible) {
        return static_cast<int16_t>(c);
    } else {
        return -1;
    }
}

int16_t HAL_UART::putcharNoWait(uint8_t c) {
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        return -1;
    }
    bool added = context->transmittBuffer.put(c);
    if(added) {
        /* enables TX interrupt */
        set_bit_reg8(&(context->UART->hw_reg->IER), ETBEI);
        /* Enable UART instance interrupt in Cortex-M3 NVIC. */
        NVIC_EnableIRQ(context->UART->irqn);
        return static_cast<int16_t>(c & 0xFF);
    } else {
        return -1;
    }
}

int32_t HAL_UART::status(UART_STATUS_TYPE type) {
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        return -1;
    }
    int32_t temp;
    switch(type) {
        case UART_STATUS_RX_BUF_LEVEL:
            return static_cast<int32_t>(context->receiveBuffer.getElementCount());

        case UART_STATUS_RX_ERROR:
            temp                 = static_cast<int32_t>(context->uartRxError);
            context->uartRxError = 0;
            return temp;

        default: return -1;
    }
    return 0; /// added because of compiler warning
}

bool HAL_UART::isWriteFinished() {
    bool retValue;
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        retValue = true; // false would create an infinite loop
    } else {
        uint8_t status = context->UART->hw_reg->LSR;
        context->UART->status |= status;
        retValue = context->transmittBuffer.isEmpty() && ((status & MSS_UART_TEMT) != 0u);
    }
    return retValue;
}

bool HAL_UART::isDataReady() {
    bool retValue;
    if((context->idx < UART_IDX_MIN) || (context->idx > UART_IDX_MAX)) {
        retValue = false;
    } else {
        retValue = (context->receiveBuffer.isEmpty() == false);
    }
    return retValue;
}

/****************** HW_HAL_UART *******************/

void HW_HAL_UART::rxInt(void) {
    uint8_t status = UART->hw_reg->LSR;
    if((status & MSS_UART_DATA_READY) != 0u) {
        uint8_t c = UART->hw_reg->RBR & 0xFFu;
        if(receiveBuffer.put(c) == false) {
            uartRxError++; // increment FIFO Overflow count
        }
        hal_uart->upCallDataReady();
    }
}

void HW_HAL_UART::txInt(void) {
    uint8_t status;
    status = UART->hw_reg->LSR;
    UART->status |= status;
    if(status & MSS_UART_THRE) {
        uint16_t hwFifoSize = 16u;
        uint8_t  c          = 0;
        __disable_irq();
        bool tranBuffNotEmpty = transmittBuffer.get(c);
        while((hwFifoSize > 0u) && tranBuffNotEmpty) {
            // fill hardware fifo
            UART->hw_reg->THR = c;
            hwFifoSize--;
            if(hwFifoSize > 0u) {
                tranBuffNotEmpty = transmittBuffer.get(c);
            }
        }
        __enable_irq();
        // Flag Tx as complete if all data has been pushed into the Tx FIFO.
        if(tranBuffNotEmpty == false) {
            /* disables TX interrupt */
            clear_bit_reg8(&UART->hw_reg->IER, ETBEI);
            hal_uart->upCallWriteFinished();
        }
    }
}

/// Helperfunction for the ISR
void rxHandler(mss_uart_instance_t* this_uart) {
    if(UART_contextArray[0].UART == this_uart) {
        UART_contextArray[0].rxInt();
    } else if(UART_contextArray[1].UART == this_uart) {
        UART_contextArray[1].rxInt();
    }
}

void txHandler(mss_uart_instance_t* this_uart) {
    if(UART_contextArray[0].UART == this_uart) {
        UART_contextArray[0].txInt();
    } else if(UART_contextArray[1].UART == this_uart) {
        UART_contextArray[1].txInt();
    }
}

} /* namespace RODOS */
