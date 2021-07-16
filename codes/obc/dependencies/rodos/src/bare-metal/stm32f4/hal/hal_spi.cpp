#include <new>
#include "rodos.h"
#include "hal/hal_spi.h"
#include "hw_hal_gpio.h"
extern "C" {
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_misc.h"
#include "stm32f4xx_dma.h"
}

namespace RODOS {

/** TODO:
 * - timeout for while loops
 * - read/write not "busy-waiting" -> interrupts
 * - reset()
 * - spi slave
 * - status()
 * - config()
 */
#define SPI_IDX_MIN SPI_IDX1
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define SPI_IDX_MAX SPI_IDX6
#else
#define SPI_IDX_MAX SPI_IDX3
#endif


class HW_HAL_SPI {
public:
	SPI_IDX idx;
	bool initialized;
	bool slave;
	SPI_TypeDef *SPIx;
	uint32_t baudrate;

	DMA_Stream_TypeDef *DMA_Stream_RX;
	DMA_Stream_TypeDef *DMA_Stream_TX;
	uint32_t DMA_Stream_RX_FLAG_TCIFx;
	uint32_t DMA_Stream_TX_FLAG_TCIFx;
	uint32_t DMA_Channel_RX;
	uint32_t DMA_Channel_TX;

	uint32_t GPIO_Pin_MISO;
	uint32_t GPIO_Pin_MOSI;
	uint32_t GPIO_Pin_SCK;
	uint32_t GPIO_Pin_NSS;
	GPIO_TypeDef* GPIO_Port_MISO;
	GPIO_TypeDef* GPIO_Port_MOSI;
	GPIO_TypeDef* GPIO_Port_SCK;
	GPIO_TypeDef* GPIO_Port_NSS;

	uint8_t GPIO_AF;

	static DMA_InitTypeDef DMA_InitStructure;

public:
  HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin);
	HW_HAL_SPI(SPI_IDX idx);

	HW_HAL_SPI() {
		// this constructor is called to init SPIcontextArray-objects
		// !!! don't set member variables -> this might overwrite values already set by constructor call of placement new
	}

	void waitOnTransferReady();

	int32_t setBaudrate(uint32_t baudrate);

  void initMembers(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin = GPIO_INVALID);
};

DMA_InitTypeDef HW_HAL_SPI::DMA_InitStructure;

HW_HAL_SPI::HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) {
  RODOS_ASSERT(idx >= SPI_IDX_MIN); // SPI index out of range
  RODOS_ASSERT(idx <= SPI_IDX_MAX); // SPI index out of range
  initMembers(idx, sckPin, misoPin, mosiPin, nssPin);
}

HW_HAL_SPI::HW_HAL_SPI(SPI_IDX idx) {

	switch(idx){
	case SPI_IDX1:
    //SCK = PA5, MISO = PA6, MOSI = PA7
    initMembers(idx, GPIO_005, GPIO_006, GPIO_007);
		break;
	case SPI_IDX2:
    //SCK = PB13, MISO = PB14, MOSI = PB15
    initMembers(idx, GPIO_029, GPIO_030, GPIO_031);
		break;
	case SPI_IDX3:
    //SCK = PC10, MISO = PC11, MOSI = PC12
    initMembers(idx, GPIO_042, GPIO_043, GPIO_044);
		break;
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
	case SPI_IDX4:
    //SCK = PE12, MISO = PE13, MOSI = PE14
    initMembers(idx, GPIO_076, GPIO_077, GPIO_078);
		break;
	case SPI_IDX5:
    //SCK = PF7, MISO = PF8, MOSI = PF9
    initMembers(idx, GPIO_087, GPIO_088, GPIO_089);
		break;
	case SPI_IDX6:
    //SCK = PG13, MISO = PG12, MOSI = PG14
    initMembers(idx, GPIO_109, GPIO_108, GPIO_110);
		break;
#endif
	default:
            RODOS_ERROR("SPI index out of range");
	}

}

void HW_HAL_SPI::initMembers(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) {
	this->idx = idx;
	this->initialized = false;

	/* set DMA parameter, which are always the same */
	DMA_InitTypeDef *pDMA_Init = &DMA_InitStructure;
	DMA_StructInit(pDMA_Init);
	pDMA_Init->DMA_MemoryInc = DMA_MemoryInc_Enable;
	pDMA_Init->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	pDMA_Init->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	pDMA_Init->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	pDMA_Init->DMA_Mode = DMA_Mode_Normal;
	pDMA_Init->DMA_Priority = DMA_Priority_High;
	pDMA_Init->DMA_FIFOMode = DMA_FIFOMode_Enable;
	pDMA_Init->DMA_MemoryBurst = DMA_MemoryBurst_Single;
	pDMA_Init->DMA_PeripheralBurst = DMA_PeripheralBurst_Single;


  GPIO_Pin_SCK   = HW_HAL_GPIO::getSTM32Pin(sckPin);
	GPIO_Port_SCK  = HW_HAL_GPIO::getSTM32Port(sckPin);
	GPIO_Pin_MISO  = HW_HAL_GPIO::getSTM32Pin(misoPin);
	GPIO_Pin_NSS  = HW_HAL_GPIO::getSTM32Pin(nssPin);
	GPIO_Port_MISO = HW_HAL_GPIO::getSTM32Port(misoPin);
	GPIO_Pin_MOSI  = HW_HAL_GPIO::getSTM32Pin(mosiPin);
	GPIO_Port_MOSI = HW_HAL_GPIO::getSTM32Port(mosiPin);
	GPIO_Port_NSS = HW_HAL_GPIO::getSTM32Port(nssPin);

	switch(idx){
	case SPI_IDX1:
		SPIx = SPI1;
		DMA_Stream_RX = DMA2_Stream0; // or DMA2_Stream2
		DMA_Stream_TX = DMA2_Stream3; // or DMA2_Stream5
		DMA_Stream_RX_FLAG_TCIFx = DMA_FLAG_TCIF0;
		DMA_Stream_TX_FLAG_TCIFx = DMA_FLAG_TCIF3;
		DMA_Channel_RX = DMA_Channel_3;
		DMA_Channel_TX = DMA_Channel_3;
		GPIO_AF = GPIO_AF_SPI1;
		break;
	case SPI_IDX2:
		SPIx = SPI2;
		DMA_Stream_RX = DMA1_Stream3;
		DMA_Stream_TX = DMA1_Stream4;
		DMA_Stream_RX_FLAG_TCIFx = DMA_FLAG_TCIF3;
		DMA_Stream_TX_FLAG_TCIFx = DMA_FLAG_TCIF4;
		DMA_Channel_RX = DMA_Channel_0;
		DMA_Channel_TX = DMA_Channel_0;
		GPIO_AF = GPIO_AF_SPI2;
		break;
	case SPI_IDX3:
		SPIx = SPI3;
		DMA_Stream_RX = DMA1_Stream0; // or DMA1_Stream2
		DMA_Stream_TX = DMA1_Stream5; // or DMA1_Stream7
		DMA_Stream_RX_FLAG_TCIFx = DMA_FLAG_TCIF0;
		DMA_Stream_TX_FLAG_TCIFx = DMA_FLAG_TCIF5;
		DMA_Channel_RX = DMA_Channel_0;
		DMA_Channel_TX = DMA_Channel_0;
		GPIO_AF = GPIO_AF_SPI3;
		break;
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
	case SPI_IDX4:
		SPIx = SPI4;
		DMA_Stream_RX = DMA2_Stream0; // or DMA2_Stream3
		DMA_Stream_TX = DMA2_Stream1; // or DMA2_Stream4
		DMA_Stream_RX_FLAG_TCIFx = DMA_FLAG_TCIF0;
		DMA_Stream_TX_FLAG_TCIFx = DMA_FLAG_TCIF1;
		DMA_Channel_RX = DMA_Channel_4;
		DMA_Channel_TX = DMA_Channel_4;
		GPIO_AF = GPIO_AF_SPI4;
		break;
	case SPI_IDX5:
		SPIx = SPI5;
		DMA_Stream_RX = DMA2_Stream3; // or DMA2_Stream5
		DMA_Stream_TX = DMA2_Stream4; // or DMA2_Stream6
		DMA_Stream_RX_FLAG_TCIFx = DMA_FLAG_TCIF3;
		DMA_Stream_TX_FLAG_TCIFx = DMA_FLAG_TCIF4;
		DMA_Channel_RX = DMA_Channel_2;
		DMA_Channel_TX = DMA_Channel_2;
		GPIO_AF = GPIO_AF_SPI5;
		break;
	case SPI_IDX6:
		SPIx = SPI6;
		DMA_Stream_RX = DMA2_Stream6;
		DMA_Stream_TX = DMA2_Stream5;
		DMA_Stream_RX_FLAG_TCIFx = DMA_FLAG_TCIF6;
		DMA_Stream_TX_FLAG_TCIFx = DMA_FLAG_TCIF5;
		DMA_Channel_RX = DMA_Channel_1;
		DMA_Channel_TX = DMA_Channel_1;
		GPIO_AF = GPIO_AF_SPI6;
		break;
#endif
	default:
		RODOS_ERROR("SPI index out of range");
	}

}

void HW_HAL_SPI::waitOnTransferReady(){
	/* wait until transmit transfer is finished */
	while (DMA_GetCmdStatus(DMA_Stream_TX ) != DISABLE) {
	}
	while ((SPIx->SR & SPI_SR_TXE ) == 0){
	}           // wait for empty TX buffer
	while ((SPIx->SR & SPI_SR_BSY ) != 0){ // never check for == 1 -> it's wrong! working statements: != 0 or == SPI_SR_BSY
	}			// wait until transmission of last byte is complete

	/* wait until receive transfer is finished */
	while (DMA_GetCmdStatus(DMA_Stream_RX ) != DISABLE) {
	}
	while ((SPIx->SR & SPI_SR_RXNE )!= 0){ //clear rx buffer
		SPIx->DR;
	}
}

int32_t HW_HAL_SPI::setBaudrate(uint32_t baudrate){
    /* get system clocks
     * -> we need the clock the SPI interface is working with to set the correct baudrate
     */
    RCC_ClocksTypeDef sysClks;
    RCC_GetClocksFreq(&sysClks);
    uint32_t pclk;

    switch(idx){
    case SPI_IDX1: pclk = sysClks.PCLK2_Frequency; break;
    case SPI_IDX2: pclk = sysClks.PCLK1_Frequency; break;
    case SPI_IDX3: pclk = sysClks.PCLK1_Frequency; break;
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
    case SPI_IDX4: pclk = sysClks.PCLK2_Frequency; break;
    case SPI_IDX5: pclk = sysClks.PCLK2_Frequency; break;
    case SPI_IDX6: pclk = sysClks.PCLK2_Frequency; break;
#endif
    default: RODOS_ERROR("SPI index out of range"); return -1;
    }

    // calculate baud rate prescaler
    uint32_t regValBRP = 0;
    if (baudrate < pclk/2){ // only if required baudrate is smaller than max. baudrate we need to calculate a prescaler != 0
        uint32_t baudRatePrescaler = pclk/baudrate;
        // to get the register value, we need to calc log2 of baudRatePrescaler first ...
        while (baudRatePrescaler >>= 1) regValBRP++;
        // ... and second subtract 1, because register value is one less the log2 of baudRatePrescaler
        if (regValBRP > 0) regValBRP--;
        // change regValBRP to get the smallest error
        uint32_t smallerBaudrate = pclk/(1lu<<(regValBRP+2));
        uint32_t biggerBaudrate = pclk/(1lu<<(regValBRP+1));
        if ((biggerBaudrate-baudrate) > ((biggerBaudrate-smallerBaudrate)/2) ){ // wished baudrate is closer to smaller baudrate
            regValBRP++;
        }
        if (regValBRP > 7)regValBRP = 7; // max. register value = 7
    }

    SPIx->CR1 = static_cast<uint16_t>(SPIx->CR1 & ~(0x07<<3));
    SPIx->CR1 = static_cast<uint16_t>(SPIx->CR1 | (regValBRP<<3));

    this->baudrate = pclk/(1lu<<(regValBRP+1));

    return static_cast<int32_t>(this->baudrate);
}


HW_HAL_SPI SPIcontextArray[SPI_IDX_MAX];

HAL_SPI::HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin) {
	if (idx < SPI_IDX_MIN || idx > SPI_IDX_MAX) {
		RODOS_ERROR("SPI index out of range");
	} else {
		context = new (&SPIcontextArray[idx - 1]) HW_HAL_SPI(idx,sckPin,misoPin,mosiPin, nssPin); // placement new to avoid dynamic memory allocation
	}
}

HAL_SPI::HAL_SPI(SPI_IDX idx) {
	if (idx < SPI_IDX_MIN || idx > SPI_IDX_MAX) {
		RODOS_ERROR("SPI index out of range");
	} else {
		context = new (&SPIcontextArray[idx - 1]) HW_HAL_SPI(idx); // placement new to avoid dynamic memory allocation
	}
}


/** init SPI interface
 * SPI1 runs at APB2 clock (max. 84MHz)
 * SPI2/3 run at APB1 clock (max. 42MHz)
 * @param baudrate
 * @return baudrate that is really configured, considering APB clock and possible baudrate prescaler (2,4,8,16,32,64,128,256)
 */
int32_t HAL_SPI::init(uint32_t baudrate, bool slave, bool tiMode) {
	context->slave = slave;
	/* enable peripheral clock */
	switch(context->idx){
	case SPI_IDX1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // used by SPI1
		break;
	case SPI_IDX2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // used by SPI2/3
		break;
	case SPI_IDX3:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // used by SPI2/3
		break;
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
  case SPI_IDX4:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		break;
  case SPI_IDX5:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		break;
  case SPI_IDX6:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		break;
#endif
	default: RODOS_ERROR("SPI index out of range"); return -1;
	}

	/* enable clock for used IO pins */
	if ((context->GPIO_Port_SCK == GPIOA)
			|| (context->GPIO_Port_MISO == GPIOA)
			|| (context->GPIO_Port_MOSI == GPIOA)
			|| (context->GPIO_Port_NSS == GPIOA)){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	}
	if ((context->GPIO_Port_SCK == GPIOB)
			|| (context->GPIO_Port_MISO == GPIOB)
			|| (context->GPIO_Port_MOSI == GPIOB)
			|| (context->GPIO_Port_NSS == GPIOB)){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	}
	if ((context->GPIO_Port_SCK == GPIOC)
			|| (context->GPIO_Port_MISO == GPIOC)
			|| (context->GPIO_Port_MOSI == GPIOC)
			|| (context->GPIO_Port_NSS == GPIOC)){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	}
	if ((context->GPIO_Port_SCK == GPIOD)
			|| (context->GPIO_Port_MISO == GPIOD)
			|| (context->GPIO_Port_MOSI == GPIOD)
			|| (context->GPIO_Port_NSS == GPIOD)){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	}
	if ((context->GPIO_Port_SCK == GPIOE)
			|| (context->GPIO_Port_MISO == GPIOE)
			|| (context->GPIO_Port_MOSI == GPIOE)
			|| (context->GPIO_Port_NSS == GPIOE)){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	}
	if ((context->GPIO_Port_SCK == GPIOF)
			|| (context->GPIO_Port_MISO == GPIOF)
			|| (context->GPIO_Port_MOSI == GPIOF)
			|| (context->GPIO_Port_NSS == GPIOF)){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	}
	if ((context->GPIO_Port_SCK == GPIOG)
			|| (context->GPIO_Port_MISO == GPIOG)
			|| (context->GPIO_Port_MOSI == GPIOG)
			|| (context->GPIO_Port_NSS == GPIOG)){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	}

	/* set GPIO parameter which are always the same */
    GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* configure each GPIO */
	GPIO_InitStruct.GPIO_Pin = context->GPIO_Pin_SCK;
	GPIO_Init(context->GPIO_Port_SCK, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = context->GPIO_Pin_MISO;
	GPIO_Init(context->GPIO_Port_MISO, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = context->GPIO_Pin_MOSI;
	GPIO_Init(context->GPIO_Port_MOSI, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = context->GPIO_Pin_NSS;
	GPIO_Init(context->GPIO_Port_NSS, &GPIO_InitStruct);

	/* pin mapping */
	uint16_t GPIO_PinSource_SCK=0;
	uint16_t GPIO_PinSource_MISO=0;
	uint16_t GPIO_PinSource_MOSI=0;
	uint16_t GPIO_PinSource_NSS=0;
	// calc GPIO_PinSourcex from GPIO_Pin_x
	uint32_t tmpGPIO_Pin = context->GPIO_Pin_SCK;
	while (tmpGPIO_Pin >>= 1) GPIO_PinSource_SCK++;
	tmpGPIO_Pin = context->GPIO_Pin_MISO;
	while (tmpGPIO_Pin >>= 1) GPIO_PinSource_MISO++;
	tmpGPIO_Pin = context->GPIO_Pin_MOSI;
	while (tmpGPIO_Pin >>= 1) GPIO_PinSource_MOSI++;
	tmpGPIO_Pin = context->GPIO_Pin_NSS;
	while (tmpGPIO_Pin >>= 1) GPIO_PinSource_NSS++;

	GPIO_PinAFConfig(context->GPIO_Port_SCK, GPIO_PinSource_SCK, context->GPIO_AF);
	GPIO_PinAFConfig(context->GPIO_Port_MISO, GPIO_PinSource_MISO, context->GPIO_AF);
	GPIO_PinAFConfig(context->GPIO_Port_MOSI, GPIO_PinSource_MOSI, context->GPIO_AF);
	GPIO_PinAFConfig(context->GPIO_Port_NSS, GPIO_PinSource_NSS, context->GPIO_AF);

	/* configure SPI in Mode 0
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, separate MOSI and MISO lines
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;                  // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;                         // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;                       // data sampled at first edge
	SPI_InitStruct.SPI_BaudRatePrescaler = 0;                       // will be set in setBaudrate()
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;                 // data is transmitted MSB first
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; 								 // set the NSS management to internal

	if(slave) {
		SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;                      // transmit in slave mode, NSS pin has to be always high
	} else {
		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;                      // transmit in master mode, NSS pin has to be always high
	}

	SPI_Init(context->SPIx, &SPI_InitStruct);

	context->setBaudrate(baudrate);

	SPI_Cmd(context->SPIx, ENABLE);
	if ( slave && tiMode ) {
	    SPI_TIModeCmd(context->SPIx, ENABLE);
	}
	context->initialized = true;
	return 0;
}


/* disable interface and set all its registers and pins to its reset state */
void HAL_SPI::reset() {
    /* disable and reset SPI interface */
    SPI_Cmd(context->SPIx, DISABLE);

    switch(context->idx){
    case SPI_IDX1: RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE); break;
    case SPI_IDX2: RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE); break;
    case SPI_IDX3: RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, DISABLE); break;
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
    case SPI_IDX4: RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, DISABLE); break;
    case SPI_IDX5: RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, DISABLE); break;
    case SPI_IDX6: RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, DISABLE); break;
#endif
    default: RODOS_ERROR("SPI index out of range"); return;
    }

    /* reset GPIOs used by SPI interface */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = context->GPIO_Pin_MISO;
    GPIO_Init(context->GPIO_Port_MISO, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = context->GPIO_Pin_MOSI;
    GPIO_Init(context->GPIO_Port_MOSI, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = context->GPIO_Pin_SCK;
    GPIO_Init(context->GPIO_Port_SCK, &GPIO_InitStruct);

    // calc GPIO_PinSourcex from GPIO_Pin_x
    uint16_t GPIO_PinSource_SCK=0;
    uint16_t GPIO_PinSource_MISO=0;
    uint16_t GPIO_PinSource_MOSI=0;
    uint16_t GPIO_PinSource_NSS=0;
    uint32_t tmpGPIO_Pin = context->GPIO_Pin_SCK;
    while (tmpGPIO_Pin >>= 1) GPIO_PinSource_SCK++;
    tmpGPIO_Pin = context->GPIO_Pin_MISO;
    while (tmpGPIO_Pin >>= 1) GPIO_PinSource_MISO++;
    tmpGPIO_Pin = context->GPIO_Pin_MOSI;
    while (tmpGPIO_Pin >>= 1) GPIO_PinSource_MOSI++;
    tmpGPIO_Pin = context->GPIO_Pin_NSS;
	while (tmpGPIO_Pin >>= 1) GPIO_PinSource_NSS++;

    GPIO_PinAFConfig(context->GPIO_Port_SCK, GPIO_PinSource_SCK, 0);
    GPIO_PinAFConfig(context->GPIO_Port_MISO, GPIO_PinSource_MISO, 0);
    GPIO_PinAFConfig(context->GPIO_Port_MOSI, GPIO_PinSource_MOSI, 0);
    GPIO_PinAFConfig(context->GPIO_Port_NSS, GPIO_PinSource_NSS, 0);
}

/* Configuration of SPI interface AFTER initialization */
int32_t HAL_SPI::config(SPI_PARAMETER_TYPE type, int32_t value) {
    if(!context->initialized) return -1;

    switch (type){
    case SPI_PARAMETER_BAUDRATE:
        if ( (value > 0) && isWriteFinished() && isReadFinished()){
            SPI_Cmd(context->SPIx, DISABLE);
            context->setBaudrate(static_cast<uint32_t>(value));
            SPI_Cmd(context->SPIx, ENABLE);
            return 0;
        }
        return -1;

    case SPI_PARAMETER_MODE: // MODE:CPOL/CPHA  0:0/0   1:0/1   2:1/0   3:1/1
        if (value >= 0 && value <= 3){
            SPI_Cmd(context->SPIx, DISABLE);
            uint16_t cr1 = context->SPIx->CR1;
            cr1          = static_cast<uint16_t>(cr1 & ~0x0003); // clear CPOL & CPHA
            cr1          = static_cast<uint16_t>(cr1 | value);   // set CPOL & CPHA
            context->SPIx->CR1 = cr1;
            SPI_Cmd(context->SPIx, ENABLE);
            return 0;
        }else{
            return -1;
        }

    default:
        return -1;
    }
}

int32_t HAL_SPI::status(SPI_STATUS_TYPE type) {
    if(!context->initialized) return -1;

    switch (type){
    case SPI_STATUS_BAUDRATE:
        return static_cast<int32_t>(context->baudrate);
    case SPI_STATUS_MODE:
    	// Only send last two bits of control register (CPOL/CPHA)
    	return (context->SPIx->CR1 & 0x0003);
    default:
        return -1;
    }
}


bool HAL_SPI::isWriteFinished(){
    return true;
}

bool HAL_SPI::isReadFinished(){
    return true;
}

/**
 * DMA write, busy waiting
 */
int32_t HAL_SPI::write(const void* sendBuf, size_t len) {

	if(!context->initialized) return -1;

	context->waitOnTransferReady();

	// DMA_DeInit(context->DMA_Stream_TX); // only for debugging necessary

	DMA_InitTypeDef *pDMA_Init = &context->DMA_InitStructure;
	pDMA_Init->DMA_PeripheralBaseAddr = (uint32_t) &(context->SPIx->DR);
	pDMA_Init->DMA_Channel = context->DMA_Channel_TX;
	pDMA_Init->DMA_DIR = DMA_DIR_MemoryToPeripheral;
	pDMA_Init->DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	pDMA_Init->DMA_Memory0BaseAddr = (uint32_t) sendBuf;
	pDMA_Init->DMA_BufferSize = (uint32_t) len;
	DMA_Init(context->DMA_Stream_TX, &context->DMA_InitStructure);

	DMA_ITConfig(context->DMA_Stream_TX, DMA_IT_TC, DISABLE);

	DMA_Cmd(context->DMA_Stream_TX, ENABLE);
	while (DMA_GetCmdStatus(context->DMA_Stream_TX) != ENABLE) {
	}

	/*start spi transmit */
	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Tx, ENABLE);

	/* poll transfer complete flag */
	while (DMA_GetFlagStatus(context->DMA_Stream_TX, context->DMA_Stream_TX_FLAG_TCIFx) == RESET) {}
	DMA_ClearFlag(context->DMA_Stream_TX, context->DMA_Stream_TX_FLAG_TCIFx);

	context->waitOnTransferReady();

	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Tx, DISABLE);

	upCallWriteFinished();

	return static_cast<int32_t>(len);
}

/**
 * read() - DMA read, busy waiting
 */
int32_t HAL_SPI::read(void* recBuf, size_t maxLen) {
	if(!context->slave)
		return writeRead(recBuf, maxLen, recBuf, maxLen);

	if(!context->initialized) return -1;

	/* configure receive DMA */
	DMA_InitTypeDef *pDMA_Init = &context->DMA_InitStructure;
	pDMA_Init->DMA_PeripheralBaseAddr = (uint32_t) &(context->SPIx->DR);
	pDMA_Init->DMA_Channel = context->DMA_Channel_RX;
	pDMA_Init->DMA_DIR = DMA_DIR_PeripheralToMemory;
	pDMA_Init->DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	pDMA_Init->DMA_Memory0BaseAddr = (uint32_t)recBuf;
	pDMA_Init->DMA_BufferSize = static_cast<uint32_t>(maxLen);
	DMA_Init(context->DMA_Stream_RX, pDMA_Init);
	DMA_ITConfig(context->DMA_Stream_RX, DMA_IT_TC, DISABLE);
	DMA_Cmd(context->DMA_Stream_RX, ENABLE);
	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Rx, ENABLE);

	while (!DMA_GetFlagStatus(context->DMA_Stream_RX, context->DMA_Stream_RX_FLAG_TCIFx )){}
	DMA_ClearFlag(context->DMA_Stream_RX, context->DMA_Stream_RX_FLAG_TCIFx );

	context->waitOnTransferReady();

	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, DISABLE);

	upCallReadFinished();

	return static_cast<int32_t>(maxLen);

/* The following read-implementation works, but generates spi clock for a few more bytes than requested,
 * because the RXONLY-flag is cleared to late.
 */
//	if(!context->initialized) return -1;
//
//	context->waitOnTransferReady();
//
//	// DMA_DeInit(context->DMA_Stream_RX); // only for debugging
//
//	DMA_InitTypeDef *pDMA_Init = &context->DMA_InitStructure;
//	pDMA_Init->DMA_PeripheralBaseAddr = (uint32_t) &(context->SPIx->DR);
//	pDMA_Init->DMA_Channel = context->DMA_Channel_RX;
//	pDMA_Init->DMA_DIR = DMA_DIR_PeripheralToMemory;
//	pDMA_Init->DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//	pDMA_Init->DMA_Memory0BaseAddr = (uint32_t) recBuf;
//	pDMA_Init->DMA_BufferSize = maxLen;
//	DMA_Init(context->DMA_Stream_RX, pDMA_Init);
//
//	DMA_ITConfig(context->DMA_Stream_RX, DMA_IT_TC, DISABLE);
//
//	DMA_Cmd(context->DMA_Stream_RX, ENABLE);
//	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
//	/* enable rx only mode -> otherwise no clock will be generated and nothing will be received */
//	context->SPIx->CR1 |= SPI_CR1_RXONLY;
//	context->SPIx->CR1 &= ~(SPI_CR1_RXONLY );
//
//	/* poll transfer complete flag */
//	while (DMA_GetFlagStatus(context->DMA_Stream_RX, context->DMA_Stream_RX_FLAG_TCIFx ) == RESET) {}
//	DMA_ClearFlag(context->DMA_Stream_RX, context->DMA_Stream_RX_FLAG_TCIFx );
//
//	context->waitOnTransferReady();
//
//	context->SPIx->CR1 &= ~(SPI_CR1_RXONLY ); // clear RXONLY flag to stop generation of spi clock
//
//	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
//
//  upCallReadFinished();
//
//	return maxLen;
}

int32_t HAL_SPI::writeRead(const void* sendBuf, size_t len, void* recBuf, size_t maxLen) {

	if(!context->initialized) return -1;

	context->waitOnTransferReady();

	DMA_InitTypeDef *pDMA_Init = &context->DMA_InitStructure;

	/* configure transmit DMA */
	pDMA_Init->DMA_PeripheralBaseAddr = (uint32_t) &(context->SPIx ->DR);
	pDMA_Init->DMA_Channel = context->DMA_Channel_TX;
	pDMA_Init->DMA_DIR = DMA_DIR_MemoryToPeripheral;
	pDMA_Init->DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	pDMA_Init->DMA_Memory0BaseAddr = (uint32_t) sendBuf;
	pDMA_Init->DMA_BufferSize = len;
	DMA_Init(context->DMA_Stream_TX, pDMA_Init);

	/* configure receive DMA */
	pDMA_Init->DMA_PeripheralBaseAddr = (uint32_t) &(context->SPIx->DR);
	pDMA_Init->DMA_Channel = context->DMA_Channel_RX;
	pDMA_Init->DMA_DIR = DMA_DIR_PeripheralToMemory;
	pDMA_Init->DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	pDMA_Init->DMA_Memory0BaseAddr = (uint32_t) recBuf; /// (uint32_t)&SPI_MASTER_Buffer_Rx->preamble;
	pDMA_Init->DMA_BufferSize = maxLen;
	DMA_Init(context->DMA_Stream_RX, pDMA_Init);

	DMA_ITConfig(context->DMA_Stream_RX, DMA_IT_TC, DISABLE);
	DMA_ITConfig(context->DMA_Stream_TX, DMA_IT_TC, DISABLE);

	/*start TX/RX*/
	DMA_Cmd(context->DMA_Stream_RX, ENABLE);
	DMA_Cmd(context->DMA_Stream_TX, ENABLE);
	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);

	while (!DMA_GetFlagStatus(context->DMA_Stream_TX, context->DMA_Stream_TX_FLAG_TCIFx )){}
	DMA_ClearFlag(context->DMA_Stream_TX, context->DMA_Stream_TX_FLAG_TCIFx);

	if (maxLen > len){
	    // start sending dummy bytes using recBuf to generate spi clock for reception of requested bytes
	    pDMA_Init->DMA_PeripheralBaseAddr = (uint32_t) &(context->SPIx ->DR);
	    pDMA_Init->DMA_Channel = context->DMA_Channel_TX;
	    pDMA_Init->DMA_DIR = DMA_DIR_MemoryToPeripheral;
	    pDMA_Init->DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	    pDMA_Init->DMA_Memory0BaseAddr = (uint32_t) recBuf;
	    pDMA_Init->DMA_BufferSize = maxLen-len;
	    DMA_Init(context->DMA_Stream_TX, pDMA_Init);
        DMA_Cmd(context->DMA_Stream_TX, ENABLE);
        SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
        while (!DMA_GetFlagStatus(context->DMA_Stream_TX, context->DMA_Stream_TX_FLAG_TCIFx )){}
        DMA_ClearFlag(context->DMA_Stream_TX, context->DMA_Stream_TX_FLAG_TCIFx);
	}

	while (!DMA_GetFlagStatus(context->DMA_Stream_RX, context->DMA_Stream_RX_FLAG_TCIFx )){}
	DMA_ClearFlag(context->DMA_Stream_RX, context->DMA_Stream_RX_FLAG_TCIFx );

	context->waitOnTransferReady();

	/*end TX/RX*/
	SPI_I2S_DMACmd(context->SPIx, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, DISABLE);

	upCallReadFinished();

	return static_cast<int32_t>(maxLen);
}

}
