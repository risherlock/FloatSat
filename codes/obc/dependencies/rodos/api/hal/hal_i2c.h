#pragma once

#include "genericIO.h"
#include "hal_gpio.h"
#include <stdint.h>


namespace RODOS {

/*enum I2C_PARAMETER_TYPE {
	I2C_MASTER,
	I2C_SLAVE,
	I2C_SLAVE_ADDR,
	I2C_SPEED
};*/

enum I2C_IDX {
	I2C_IDX1 = 1,
	I2C_IDX2,
	I2C_IDX3
};

enum HAL_I2C_ERR {
	HAL_I2C_ERR_START = -1,
	HAL_I2C_ERR_STOP = -2,
	HAL_I2C_ERR_WRITE = -3,
	HAL_I2C_ERR_READ = -4,
	HAL_I2C_ERR_ADDR = -5,
	HAL_I2C_ERR_NOT_MASTER = -6,
	HAL_I2C_ERR_MAX = HAL_I2C_ERR_NOT_MASTER
};

class HW_HAL_I2C;

class HAL_I2C : public GenericIOInterface {
	HW_HAL_I2C *context;
public:
	HAL_I2C(I2C_IDX idx);
	HAL_I2C(I2C_IDX idx, GPIO_PIN sclPin, GPIO_PIN sdaPin);
   /**
	 * Initialize I2C interface
	 * default: master, 7Bit address
	 * @param speed I2C bus speed
	 * @retval int32_t zero on success
	 */
	int32_t init(uint32_t speed = 100000);

	//int32_t config(I2C_PARAMETER_TYPE type, int32_t paramVal);

	void reset();

	bool isWriteFinished();
	bool isReadFinished();

   /**
	 * Transmit data.  Does not return until transfer is finished(blocking).
	 * @param addr I2C address of chip
	 * @param txBuf pointer to transmit buffer
	 * @param txBufSize size of transmit buffer
	 * @retval int32_t number of transmitted bytes
	 */
	int32_t write(const uint8_t addr, const uint8_t *txBuf, uint32_t txBufSize);

   /**
     * Receive data.  Does not return until transfer is finished(blocking).
     * @param addr I2C address of chip
     * @param rxBuf pointer to receive buffer
     * @param rxBufSize size of receive buffer
     * @retval int32_t number of received bytes
     */
	int32_t read(const uint8_t addr, uint8_t *rxBuf, uint32_t rxBufSize);

   /**
	 * Send and receive data without a STOP condition between.  Does not return until transfer is finished(blocking).
	 * @param addr I2C address of chip
	 * @param txBuf	pointer to transmit buffer
	 * @param txBufSize size of transmit buffer
	 * @param rxBuf	point32_ter to receive buffer
	 * @param rxBufSize size of receive buffer
	 * @retval int32_t number of received bytes
	 */
	int32_t writeRead(const uint8_t addr, const uint8_t *txBuf, uint32_t txBufSize, uint8_t *rxBuf, uint32_t rxBufSize);
};


}
