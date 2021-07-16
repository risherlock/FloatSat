#ifndef _HW_HAL_I2C_H_
#define _HW_HAL_I2C_H_


#include "stm32f4xx_i2c.h"
#include "hw_hal_gpio.h"
#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

class HAL_I2C;

class HW_HAL_I2C {
	friend class HAL_I2C;
protected: // private?
	HW_HAL_I2C(I2C_IDX i2cIdx){
		I2C_idx = i2cIdx;
	}

	HW_HAL_I2C(){
		// this constructor is called to init I2C_contextArray-objects
		// !!! don't set member variables here !!! -> this might overwrite values already set by constructor call of placement new
	};

	I2C_IDX I2C_idx;
	I2C_TypeDef* I2Cx;
	uint32_t I2C_CLK; // RCC_APB1Periph_I2C1

	uint16_t I2C_SCL_PIN;
	GPIO_TypeDef* I2C_SCL_GPIO_PORT;
	uint32_t I2C_SCL_GPIO_CLK; //RCC_AHB1Periph_GPIOA
	uint8_t I2C_SCL_SOURCE;

	uint16_t I2C_SDA_PIN;
	GPIO_TypeDef* I2C_SDA_GPIO_PORT;
	uint32_t I2C_SDA_GPIO_CLK;
	uint8_t I2C_SDA_SOURCE;

	uint8_t I2C_AF;
	uint16_t I2C_SLAVE_ADDRESS7;
	uint32_t I2C_SPEED; /*!< Specifies the SCL frequency in Hz.
	 This parameter must be set to a value lower than 400kHz */
	bool IsMaster;

	int32_t mstrWriteNoStop(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize);
	int32_t mstrWrite(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize);
	int32_t mstrReadNoStart(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize);
	int32_t mstrRead(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize);

  void initMembers(HAL_I2C* halI2C, I2C_IDX i2cIdx, GPIO_PIN sclPin, GPIO_PIN sdaPin);
	//int32_t errorCheck();

	static HW_HAL_I2C I2C_contextArray[3];

private:
	int32_t sendAddr(const uint8_t addr, uint8_t rwFlag);
	int32_t start();
	int32_t stop();
};

#ifndef NO_RODOS_NAMESPACE
}
#endif
#endif // _HW_HAL_I2C_H_
