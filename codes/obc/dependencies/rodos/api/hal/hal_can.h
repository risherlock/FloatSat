/*!
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Michael Ruffer, Philip von Keiser (ILR TU Berlin)
 */


#pragma once

#include <stdint.h>
#include "hal/genericIO.h"
#include "hal/hal_gpio.h"

namespace RODOS {

enum CAN_PARAMETER_TYPE {
    CAN_PARAMETER_BAUDRATE
};

enum CAN_STATUS_TYPE {
    CAN_STATUS_RX_LEVEL,
    CAN_STATUS_RX_ERROR,
    CAN_STATUS_TX_ERROR
};

enum class CAN_ERROR_TYPE : uint8_t {
    NO_ERROR = 0,
    UNKNOWN,
    ERROR_ACTIVE,
    ERROR_PASSIVE,
    BOF,
    STUFF,
    NO_ACK
};

enum CAN_IDX {
    CAN_IDX0 = 0,
    CAN_IDX1,
    CAN_IDX2,
    CAN_IDX3,
    CAN_IDX4,
    CAN_IDX5
};    

struct CanErrorMsg {
    int canErrorCnt;
    CAN_ERROR_TYPE errorType;
};

class HW_HAL_CAN;

class HAL_CAN : public GenericIOInterface {
private:
    HW_HAL_CAN* context;

public:
    HAL_CAN(CAN_IDX canIdx, GPIO_PIN rxPin = GPIO_INVALID, GPIO_PIN txPin = GPIO_INVALID);

    /*!
     * Initialize the CAN interface. 
     *
     * @param baudrate The desired CAN baud rate.
     * @return 0
     */
    int32_t init(uint32_t baudrate);

    /*!
     * Reset and disable the interface. Set all its registers and pins to its 
     * reset state.
     */
    void reset();

    /*!
     * Configure the CAN interface after initialization.
     * 
     * @param type Type of the desired CAN parameter to configure.
     * @param paramVal The new value of the parameter.
     * @return -1 if type does not exist, else 0.
     */
    int32_t config(CAN_PARAMETER_TYPE type, uint32_t paramVal);

    /*!
     * Get the status of the CAN interface.
     * 
     * @param type Type of the desired CAN status.
     * @return CanErrorMsg with CAN_ERROR_TYPE::UNKNOWN if type does not exist 
     * or is CAN_STATUS_RX_LEVEL, else CAN error count and specific CAN_ERROR_TYPE.
     */
    CanErrorMsg status(CAN_STATUS_TYPE type);

    /*!
     * Called when all data has been transmitted
     * 
     * @return True when transmission is finished, else false.
     */
    bool isWriteFinished();
    
    /*!
     * Called when a message is ready to read 
     * 
     * @return True when data was received, else false.
     */
    bool isDataReady();

    /*!
     * Add a filter for incoming messages. Only message with pass a filter 
     * are received. Filters may be implemented in hardware an can be 
     * limited. Also one ID should only match with one filter.
     * 
     * @param ID CAN ID (11bit for standard frame 29 bit for 
     * extended frame, always right justified).
     * @param IDmask Every bit that is 1 here is don't care 
     * (11bit for standard frame 29 bit for extended frame, 
     * always right justified).
     * @param extID Set filter to use extended frame format (true) or standard 
     * frame format (false)
     * @param RTR Set filter to only accept RTR messages (true) or regular 
     * messages (false)
     * @return True when filter was created, false if not 
     * (no more HW resources?).
     */
    bool addIncomingFilter(uint32_t ID, uint32_t IDMask = 0, bool extID = true, bool rtr = false);

    /*!
     * Write a CAN Message, non-blocking.
     * 
     * @param sendBuf Pointer to transmit data.
     * @param len Length of transmit data( max. 8 bytes).
     * @param canID CAN ID (11bit for standard frame 29 bit for extended 
     * frame, always right justified).
     * @param extID Mark CAN messages as extended frame format (true) or 
     * standard frame format (false)
     * @param rtr Mark CAN message as Remote Transmission Request (true) or 
     * regular message (false)
     * @return -1 on error, 0 on success.
     */
    int8_t write(const uint8_t* sendBuf, uint8_t len, uint32_t canID, bool extID = true, bool rtr = false);
    
    /*!
     * Read a CAN Message, non-blocking.
     * 
     * @param recBuf DataBuffer, must have space for 8 bytes.
     * @param canID CAN ID of received frame (11bit for standard frame 
     * 29 bit for extended frame, always right justified).
     * @param isExtID Was it an extended frame format?
     * @param rtr Was it a RTR frame?
     * @return Number of data bytes or -1 if nothing was received.
     */
    int8_t read(uint8_t* recBuf, uint32_t* canID = 0, bool* isExtID = 0, bool* rtr = 0);

    // void setDEfaultTransmitID(...) //dann kann auch write und read aus GenericIO implementiert werden.

    // read und write aus GenericIO nicht implementiert
};

}


