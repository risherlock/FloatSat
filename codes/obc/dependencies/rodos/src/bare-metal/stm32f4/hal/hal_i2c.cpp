#include <new>
#include "hal/hal_i2c.h"
#include "rodos.h"
#include "timemodel.h"

#include "hal/hw_hal_i2c.h" // bare-metal/stm32f4/hal

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "hw_hal_gpio.h"

namespace RODOS {

#ifndef I2C_TIMEOUT_IN_NS
#define I2C_TIMEOUT_IN_NS  		(2*MILLISECONDS)
#endif
#ifndef I2C_LONG_TIMEOUT_IN_NS
#define I2C_LONG_TIMEOUT_IN_NS  	(10*MILLISECONDS)
#endif

HW_HAL_I2C HW_HAL_I2C::I2C_contextArray[3];

HAL_I2C::HAL_I2C(I2C_IDX idx) {
  if(idx < I2C_IDX1) idx = I2C_IDX1;
  if(idx > I2C_IDX3) idx = I2C_IDX3;
	context = new (&HW_HAL_I2C::I2C_contextArray[idx-1]) HW_HAL_I2C(idx); // placement new to avoid dynamic memory allocation

	switch (idx){
	case I2C_IDX1:
    /* SCL pin config -> PB6*/
    /* SDA pin config -> PB9 */
    context->initMembers(this, idx, GPIO_022, GPIO_025);
    break; 
	case I2C_IDX2:
    /* SCL pin config -> PB10 */
    /* SDA pin config -> PB11 */
    context->initMembers(this, idx, GPIO_026, GPIO_027);
		break;
	case I2C_IDX3:
		/* SCL pin config -> PA8 */
    /* SDA pin config -> PC9 */
    context->initMembers(this, idx, GPIO_008, GPIO_041);
		break;
	default:
		break;
	}
}

HAL_I2C::HAL_I2C(I2C_IDX idx, GPIO_PIN sclPin, GPIO_PIN sdaPin) {
  if(idx < I2C_IDX1) idx = I2C_IDX1;
  if(idx > I2C_IDX3) idx = I2C_IDX3;
	context = new (&HW_HAL_I2C::I2C_contextArray[idx-1]) HW_HAL_I2C(idx); // placement new to avoid dynamic memory allocation
  context->initMembers(this, idx, sclPin, sdaPin);

}

int32_t HAL_I2C::init(uint32_t speed) {
    /* Reset I2C module */
    reset();

    RODOS_ASSERT_IFNOT_RETURN((speed > 0 && speed <= 20000000), -1);

	context->I2C_SPEED = speed;

	/* enable peripheral clock for I2C module */
	RCC_APB1PeriphClockCmd(context->I2C_CLK, ENABLE);

	/* enable peripheral clock for I2C GPIOs */
	RCC_AHB1PeriphClockCmd(context->I2C_SCL_GPIO_CLK | context->I2C_SDA_GPIO_CLK, ENABLE);

	/* GPIO configuration */
	/* Connect PXx to I2C_SCL*/
	GPIO_PinAFConfig(context->I2C_SCL_GPIO_PORT, context->I2C_SCL_SOURCE, context->I2C_AF);
	/* Connect PXx to I2C_SDA*/
	GPIO_PinAFConfig(context->I2C_SDA_GPIO_PORT, context->I2C_SDA_SOURCE, context->I2C_AF);

	/* Configure I2C pin: SCL */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = context->I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(context->I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/* Configure I2C pin: SDA */
	GPIO_InitStructure.GPIO_Pin = context->I2C_SDA_PIN;
	GPIO_Init(context->I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* I2C configuration */
    I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = context->I2C_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = context->I2C_SPEED;
	I2C_Init(context->I2Cx, &I2C_InitStructure);

	/* I2C module Enable */
	I2C_Cmd(context->I2Cx, ENABLE);

	return 0;
}


void HAL_I2C::reset() {
    I2C_Cmd(context->I2Cx, DISABLE);
    I2C_DeInit(context->I2Cx);
    RCC_APB1PeriphClockCmd(context->I2C_CLK, DISABLE);


    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = context->I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;


    GPIO_SetBits(context->I2C_SCL_GPIO_PORT,context->I2C_SCL_PIN);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(context->I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    if(isSchedulerRunning()){
    	Thread::suspendCallerUntil(NOW()+1*MILLISECONDS);

    	for(int i=0;i<8;i++){
			GPIO_ResetBits(context->I2C_SCL_GPIO_PORT,context->I2C_SCL_PIN);
			Thread::suspendCallerUntil(NOW()+1*MILLISECONDS);
			GPIO_SetBits(context->I2C_SCL_GPIO_PORT,context->I2C_SCL_PIN);
			Thread::suspendCallerUntil(NOW()+1*MILLISECONDS);
    	}
    }

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(context->I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = context->I2C_SDA_PIN;
    GPIO_Init(context->I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(context->I2C_SCL_GPIO_PORT,context->I2C_SCL_PIN);
}


bool HAL_I2C::isWriteFinished(){
    return true;
}


bool HAL_I2C::isReadFinished(){
    return true;
}


int32_t HAL_I2C::write(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize) {
	if (txBufSize == 0) return 0;

	if (context->IsMaster) {
	    int32_t retVal = context->mstrWrite(addr,txBuf,txBufSize);
	    upCallWriteFinished();
	    return retVal;
	} else {
	    upCallWriteFinished();
		return HAL_I2C_ERR_NOT_MASTER;
	}
}


int32_t HAL_I2C::read(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize) {
	if (rxBufSize == 0) return 0;

	if (context->IsMaster) {
		int32_t retVal = context->mstrRead(addr,rxBuf,rxBufSize);
	    upCallReadFinished();
	    return retVal;
	} else {
	    upCallReadFinished();
		return HAL_I2C_ERR_NOT_MASTER;
	}
}


int32_t HAL_I2C::writeRead(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize, uint8_t* rxBuf, uint32_t rxBufSize) {
	int32_t txRetVal = 0;
	int32_t rxRetVal = 0;

	if (context->IsMaster){
		txRetVal = context->mstrWriteNoStop(addr, txBuf, txBufSize);
		if (txRetVal < 0) {
		    upCallReadFinished();
			return txRetVal;
		}

		/* send Re-Start  */
		I2C_GenerateSTART(context->I2Cx, ENABLE);

		/* Test on EV5 and clear it */
		TTime startTime = NOW();
		while (I2C_CheckEvent(context->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS){
			    upCallReadFinished();
				return HAL_I2C_ERR_START;
			}
		}

		rxRetVal = context->mstrReadNoStart(addr, rxBuf, rxBufSize);
		if (rxRetVal < 0) {
		    upCallReadFinished();
			return rxRetVal;
		}
	}else{
	    upCallReadFinished();
		return HAL_I2C_ERR_NOT_MASTER;
	}

	upCallReadFinished();
	return rxRetVal;
}



/*****************************************************************************************************************/
/*
 * class HW_HAL_I2C
 */
int32_t HW_HAL_I2C::mstrWriteNoStop(const uint8_t addr, const uint8_t* txBuf, uint32_t txBufSize){
	TTime startTime;

	if (txBufSize == 0) return 0;

	if (start() < 0){
		return HAL_I2C_ERR_START;
	}

	if (sendAddr(addr,I2C_Direction_Transmitter) < 0){
		return HAL_I2C_ERR_ADDR;
	}

	for (uint32_t i = 0; i < txBufSize; i++) {
		/* Test on EV8 and clear it */
		startTime = NOW();
		while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING )) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_WRITE;
		}

		/* send the register value */
		I2C_SendData(I2Cx, txBuf[i]);
	}

	/* Wait till all data have been physically transferred on the bus -> EV8_2*/
	startTime = NOW();
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) {
		if ((NOW()-startTime) > I2C_LONG_TIMEOUT_IN_NS)
			return HAL_I2C_ERR_WRITE;
	}

	return static_cast<int32_t>(txBufSize);
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


int32_t HW_HAL_I2C::start(){
	TTime startTime;

	/*!< While the bus is busy */
	startTime = NOW();
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY )) {
		if ((NOW()-startTime) > I2C_LONG_TIMEOUT_IN_NS)
			return HAL_I2C_ERR_START;
	}

	/* Start the config sequence */
	I2C_GenerateSTART(I2Cx, ENABLE);

	/* Test on EV5 and clear it */
	startTime = NOW();
	while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS) {
		if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
			return HAL_I2C_ERR_START;
	}

	return 0;
}


int32_t HW_HAL_I2C::stop(){
	TTime startTime;
	/* End the configuration sequence */
	I2C_GenerateSTOP(I2Cx, ENABLE);

	/* Wait to make sure that STOP flag has been cleared */
	startTime = NOW();
	while (I2Cx->CR1 & I2C_CR1_STOP ) {
		if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
			return HAL_I2C_ERR_STOP;
	}

	return 0;
}


int32_t HW_HAL_I2C::sendAddr(const uint8_t addr, uint8_t rwFlag){
	TTime startTime;

	/* Transmit the slave address and enable writing operation */
	//I2C_Send7bitAddress wants the address left aligned
	I2C_Send7bitAddress(I2Cx, static_cast<uint8_t>(addr << 1), rwFlag);

	/* Test on EV6 and clear it */
	uint32_t i2cEvent;

	if (rwFlag == I2C_Direction_Receiver){
		i2cEvent = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED;
	}else{
		i2cEvent = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;
	}

	startTime = NOW();
	while (I2C_CheckEvent(I2Cx, i2cEvent)  != SUCCESS) {
		if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
			return HAL_I2C_ERR_ADDR;
	}

	return 0;
}


int32_t HW_HAL_I2C::mstrRead(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize){
	if (rxBufSize == 0) return 0;

	if (start() < 0){
		return HAL_I2C_ERR_START;
	}

	return mstrReadNoStart(addr,rxBuf,rxBufSize);
}


int32_t HW_HAL_I2C::mstrReadNoStart(const uint8_t addr, uint8_t* rxBuf, uint32_t rxBufSize){
	TTime startTime;

	if (rxBufSize == 0) return 0;

	if (sendAddr(addr,I2C_Direction_Receiver) < 0){
		return HAL_I2C_ERR_ADDR;
	}

	if (rxBufSize == 1) {
		/*!< Disable Acknowledgment */
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		I2Cx->SR2;

		/*!< Send STOP Condition */
		I2C_GenerateSTOP(I2Cx, ENABLE);

		/* Wait for the byte to be received */
		startTime = NOW();
		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE ) == RESET) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_READ;
		}

		/*!< Read the byte received from the slave */
		rxBuf[0] = I2C_ReceiveData(I2Cx);

		/* Wait to make sure that STOP flag has been cleared */
		startTime = NOW();
		while (I2Cx->CR1 & I2C_CR1_STOP ) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_STOP;
		}

		/*!< Re-Enable Acknowledgment to be ready for another reception */
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
	}

	else if (rxBufSize == 2) {
		/*!< Disable Acknowledgment */
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

		/*Set POS*/
		I2Cx->CR1 |= I2C_CR1_POS;

		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void) I2Cx->SR2;

		/* Wait until BTF = 1 (Data 1 in DR, Data2 in shift register, SCL stretched low until a data 1 is read) */
		startTime = NOW();
		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF ) == RESET) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_READ;
		}

		/*!< Send STOP Condition */
		I2C_GenerateSTOP(I2Cx, ENABLE);

		for (uint32_t i = 0; i < 2; i++) {
			/* Wait for the byte to be received */
			startTime = NOW();
			while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE ) == RESET) {
				if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
					return HAL_I2C_ERR_READ;
			}

			/*!< Read the byte received from the slave */
			rxBuf[i] = I2C_ReceiveData(I2Cx);
		}

		/* Wait to make sure that STOP flag has been cleared */
		startTime = NOW();
		while (I2Cx->CR1 & I2C_CR1_STOP ) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_STOP;
		}

		/*!< Re-Enable Acknowledgment to be ready for another reception */
		I2C_AcknowledgeConfig(I2Cx, ENABLE);

		/*!< Reset POS */
		I2Cx->CR1 &= (uint16_t) ~((uint16_t) I2C_CR1_POS );
	}

	else if (rxBufSize > 2) {
		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void) I2Cx->SR2;

		/* Read till the last 3 bytes*/
		for (uint32_t i = 0; i < rxBufSize - 3; i++) {
			/* Wait for the byte to be received */
			startTime = NOW();
			while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE ) == RESET) {
				if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
					return HAL_I2C_ERR_READ;
			}

			/*!< Read the byte received from the slave */
			rxBuf[i] = I2C_ReceiveData(I2Cx);
		}

		/* Wait until BTF = 1 (Data 1 in DR, Data2 in shift register, SCL stretched low until a data 1 is read) */
		startTime = NOW();
		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF ) == RESET) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_READ;
		}

		/*!< Disable Acknowledgment */
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

		/* Wait for N-2 byte to be received */
		startTime = NOW();
		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE ) == RESET) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_READ;
		}

		/*!< Read N-2 byte received from the slave */
		rxBuf[rxBufSize - 3] = I2C_ReceiveData(I2Cx);

		/* Wait until BTF = 1 (Data 1 in DR, Data2 in shift register, SCL stretched low until a data 1 is read) */
		startTime = NOW();
		while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF ) == RESET) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_READ;
		}

		/*!< Send STOP Condition */
		I2C_GenerateSTOP(I2Cx, ENABLE);

		for (uint32_t i = 2; i > 0; i--) {
			/* Wait for the byte to be received */
			startTime = NOW();
			while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE ) == RESET) {
				if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
					return HAL_I2C_ERR_READ;
			}

			/*!< Read the byte received from the slave */
			rxBuf[rxBufSize - i] = I2C_ReceiveData(I2Cx);
		}

		/* Wait to make sure that STOP flag has been cleared */
		startTime = NOW();
		while (I2Cx->CR1 & I2C_CR1_STOP ) {
			if ((NOW()-startTime) > I2C_TIMEOUT_IN_NS)
				return HAL_I2C_ERR_STOP;
		}

		/*!< Re-Enable Acknowledgment to be ready for another reception */
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
	}
	/* Clear AF flag for next communication */
	I2C_ClearFlag(I2Cx, I2C_FLAG_AF );

	return static_cast<int32_t>(rxBufSize);
}

void HW_HAL_I2C::initMembers([[gnu::unused]] HAL_I2C* halI2C, I2C_IDX i2cIdx, GPIO_PIN sclPin, GPIO_PIN sdaPin){
  IsMaster = true;
  switch(i2cIdx) {
  case I2C_IDX1:
    I2Cx = I2C1;
		I2C_CLK = RCC_APB1Periph_I2C1;
		I2C_AF = GPIO_AF_I2C1;
    break;
  case I2C_IDX2:
    I2Cx = I2C2;
		I2C_CLK = RCC_APB1Periph_I2C2;
		I2C_AF = GPIO_AF_I2C2;
    break;
  case I2C_IDX3:
		I2Cx = I2C3;
		I2C_CLK = RCC_APB1Periph_I2C3;
		I2C_AF = GPIO_AF_I2C3;
    break;
  default:
		break;
  }
  /* SCL pin config */
  I2C_SCL_GPIO_PORT = HW_HAL_GPIO::getSTM32Port(sclPin);
  I2C_SCL_GPIO_CLK = HW_HAL_GPIO::getRCC_APB1Periph_GPIOx(I2C_SCL_GPIO_PORT);
  I2C_SCL_PIN = HW_HAL_GPIO::getSTM32Pin(sclPin);
  I2C_SCL_SOURCE = HW_HAL_GPIO::getGPIO_PinSource(I2C_SCL_PIN);

  /* SDA pin config */
  I2C_SDA_GPIO_PORT = HW_HAL_GPIO::getSTM32Port(sdaPin);
  I2C_SDA_GPIO_CLK = HW_HAL_GPIO::getRCC_APB1Periph_GPIOx(I2C_SDA_GPIO_PORT);
  I2C_SDA_PIN = HW_HAL_GPIO::getSTM32Pin(sdaPin);
  I2C_SDA_SOURCE = HW_HAL_GPIO::getGPIO_PinSource(I2C_SDA_PIN);


}

//int32_t HW_HAL_I2C::errorCheck(){
//
//    // check SR1 relevant error flags
//    // I2C_FLAG_TIMEOUT  - Timeout -> SCL remained low for over 25ms
//    // I2C_FLAG_OVR      - Overrun/Underrun (slave mode)
//    // I2C_FLAG_AF       - Acknowledge failure
//    // I2C_FLAG_ARLO     - Arbitration lost (master mode)
//    // I2C_FLAG_BERR     - Bus error
//
//    uint16_t errorMask = 0;
//    if (this->IsMaster){
//        errorMask = I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_AF | I2C_FLAG_TIMEOUT;
//    }else{
//        errorMask = I2C_FLAG_BERR | I2C_FLAG_AF | I2C_FLAG_OVR | I2C_FLAG_TIMEOUT;
//    }
//
//    if ((I2Cx->SR1 & errorMask) > 0){
//        return HAL_I2C_ERR_CHECK;
//    }
//}

}

