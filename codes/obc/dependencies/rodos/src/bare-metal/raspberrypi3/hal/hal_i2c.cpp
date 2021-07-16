#include <new>
#include <hal/hal_i2c.h>
#include <bcm2837.h>
#include "timemodel.h"

namespace RODOS {

int I2C_Index = -1;

#define I2C_TIMEOUT_IN_NS  			(1*MILLISECONDS)
#define I2C_LONG_TIMEOUT_IN_NS  	(10*MILLISECONDS)

class HW_HAL_I2C {
public:
	HW_HAL_I2C(I2C_IDX i2cIdx) {
		I2C_idx = i2cIdx;
	}

	HW_HAL_I2C() {
	}
	;

	I2C_IDX I2C_idx;
	uint32_t I2C_CLK; // RCC_APB1Periph_I2C1

	uint16_t I2C_SCL_PIN;
	uint32_t I2C_SCL_GPIO_CLK; //RCC_AHB1Periph_GPIOA
	uint8_t I2C_SCL_SOURCE;

	uint16_t I2C_SDA_PIN;
	uint32_t I2C_SDA_GPIO_CLK;
	uint8_t I2C_SDA_SOURCE;

	uint8_t I2C_AF;
	uint16_t I2C_SLAVE_ADDRESS7;
	uint32_t I2C_SPEED;
	bool IsMaster;

	int32_t mstrWriteNoStop(const uint8_t addr, const uint8_t* txBuf,
			uint32_t txBufSize);
	int32_t mstrWrite(const uint8_t addr, const uint8_t* txBuf,
			uint32_t txBufSize);
	int32_t mstrReadNoStart(const uint8_t addr, uint8_t* rxBuf,
			uint32_t rxBufSize);
	int32_t mstrRead(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize);

private:
	int32_t sendAddr(const uint8_t addr, uint8_t rwFlag);
	int32_t start();
	int32_t stop();
};

HW_HAL_I2C I2C_contextArray[1]; //there is only one

HAL_I2C::HAL_I2C(I2C_IDX idx) {
	context = new (&I2C_contextArray[idx - 1]) HW_HAL_I2C(idx); // placement new to avoid dynamic memory allocation
	context->IsMaster = true;
}

int32_t HAL_I2C::init(uint32_t speed) {

	//BSC1_ADDR
	uint32_t ra;
	ra = GET32(GPFSEL0);
	ra &= ~(7u << 6); //gpio2
	ra |= 4u << 6; //gpio2 alt0 I2C1_SDA
	PUT32(GPFSEL0, ra);

	ra = GET32(GPFSEL0);
	ra &= ~(7u << 9); //gpio3
	ra |= 4u << 9; //gpio3 alt0 I2C1_SCL
	PUT32(GPFSEL0, ra);

	//configure clocks
	PUT32(BSC1_ADDR + BSC_REG_DIV, BSC_CLOCK_FREQ / speed);

	//enable i2c
	ra = GET32(BSC1_ADDR + BSC_REG_C);
	ra |= BSC_I2CEN;
	PUT32(BSC1_ADDR + BSC_REG_C, ra);

	return 0;
}

void HAL_I2C::reset() {
}
bool HAL_I2C::isWriteFinished() {
	return true;
}
bool HAL_I2C::isReadFinished() {
	return true;
}

int32_t HAL_I2C::write(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize) {
	if (txBufSize == 0) return 0;

	if (context->IsMaster) {
		return context->mstrWrite(addr,txBuf,txBufSize);
	} else {
		return HAL_I2C_ERR_NOT_MASTER;
	}

}

int32_t HAL_I2C::read(const uint8_t addr, uint8_t *rxBuf, uint32_t rxBufSize) {
	if (rxBufSize == 0) return 0;

	if (context->IsMaster) {
		return context->mstrRead(addr,rxBuf,rxBufSize);
	} else {
		return HAL_I2C_ERR_NOT_MASTER;
	}
}

int32_t HAL_I2C::writeRead(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize, uint8_t* rxBuf, uint32_t rxBufSize) {
	int32_t txRetVal = 0;
	int32_t rxRetVal = 0;

	if (context->IsMaster){
		txRetVal = context->mstrWriteNoStop(addr, txBuf, txBufSize);
		if (txRetVal < 0) {
			return HAL_I2C_ERR_WRITE;
		}

		PUT32(BSC1_ADDR + BSC_REG_C, ( START_READ)); //start reading

		rxRetVal = context->mstrReadNoStart(addr, rxBuf, rxBufSize);
		if (rxRetVal < 0) {
			return HAL_I2C_ERR_READ;
		}
	}else{
		return HAL_I2C_ERR_NOT_MASTER;
	}

	return txRetVal + rxRetVal;
}

/*****************************************************************************************************************/
/*
 * class HW_HAL_I2C
 */

int32_t HW_HAL_I2C::mstrWriteNoStop(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize){
	TTime startTime;

	PUT32(BSC1_ADDR + BSC_REG_A, addr); //slave address
	PUT32(BSC1_ADDR + BSC_REG_DLEN, txBufSize);  //data length
	PUT32(BSC1_ADDR + BSC_REG_S, CLEAR_STATUS);  //clear status

	PUT32(BSC1_ADDR + BSC_REG_C, ( START_WRITE));

	uint32_t writeindex = 0;

	startTime = NOW();

	while (1) {

		if ((NOW() - startTime) > I2C_LONG_TIMEOUT_IN_NS)
			return HAL_I2C_ERR_WRITE;

		if (GET32(BSC1_ADDR + BSC_REG_S) & (BSC_ERR)) {
			return HAL_I2C_ERR_WRITE;  //error
		} else if (GET32(BSC1_ADDR + BSC_REG_S) & (BSC_CLKT)) {
			return HAL_I2C_ERR_WRITE; //timeout
		} else if (GET32(BSC1_ADDR + BSC_REG_S) & BSC_DONE) {
			return 0; //sending done
		} else if (GET32(BSC1_ADDR + BSC_REG_S) & BSC_TXW) {
			//write data
			while ((writeindex < txBufSize)
					&& (GET32(BSC1_ADDR + BSC_REG_S) & BSC_TXD)) {
				PUT32(BSC1_ADDR + BSC_REG_FIFO, txBuf[writeindex++]);
			}
		}
	}

	return -1;
}

int32_t HW_HAL_I2C::stop(){

	/* End the sequence */
	PUT32(BSC1_ADDR + BSC_REG_C, 0);
	PUT32(BSC1_ADDR + BSC_REG_S, (BSC_CLKT | BSC_ERR | BSC_DONE));

	return 0;
}

int32_t HW_HAL_I2C::mstrWrite(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize){


	int32_t retVal = mstrWriteNoStop(addr,txBuf,txBufSize);
	if ( retVal <= 0){
		return retVal;
	}

	if (stop() !=0){
		return HAL_I2C_ERR_STOP;
	}

	return retVal;
}

int32_t HW_HAL_I2C::mstrRead(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize){
	if (rxBufSize == 0) return 0;

	PUT32(BSC1_ADDR + BSC_REG_C, ( START_READ)); //start reading

	return mstrReadNoStart(addr,rxBuf,rxBufSize);
}

int32_t HW_HAL_I2C::mstrReadNoStart(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize){
	TTime startTime;

	PUT32(BSC1_ADDR + BSC_REG_A, addr); //slave address
	PUT32(BSC1_ADDR + BSC_REG_DLEN, rxBufSize);  //data length
	PUT32(BSC1_ADDR + BSC_REG_S, CLEAR_STATUS);  //clear status

	uint32_t readindex = 0;
	startTime = NOW();
	while (1) {

		if ((NOW() - startTime) > I2C_LONG_TIMEOUT_IN_NS)
			return HAL_I2C_ERR_READ;
		if (GET32(BSC1_ADDR + BSC_REG_S) & (BSC_ERR)) {
			return HAL_I2C_ERR_READ; /*error*/
		} else if (GET32(BSC1_ADDR + BSC_REG_S) & (BSC_CLKT)) {
			return HAL_I2C_ERR_READ; /*timeout*/
		} else if (GET32(BSC1_ADDR + BSC_REG_S) & BSC_DONE) {
			/*reading*/
			while ((GET32(BSC1_ADDR + BSC_REG_S) & BSC_RXD)&& (readindex < rxBufSize)){
				rxBuf[readindex++] = GET32(BSC1_ADDR + BSC_REG_FIFO) & 0xFFu;
			}
			PUT32(BSC1_ADDR + BSC_REG_C, 0);
			PUT32(BSC1_ADDR + BSC_REG_S, (BSC_CLKT | BSC_ERR | BSC_DONE));
			return 0; /*reading done*/
		}  else if (GET32(BSC1_ADDR + BSC_REG_S) & BSC_RXR) {
			return 0;
		}
	}
	return -1;
}

} /* namespace RODOS */
