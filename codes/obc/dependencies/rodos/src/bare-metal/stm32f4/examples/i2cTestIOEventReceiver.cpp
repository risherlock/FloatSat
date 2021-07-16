/*
 * i2cTestIOEeventReceiver.cpp
 *
 * Created on: 30.10.2013
 * Author: Michael Ruffer
 *
 */

#include "rodos.h"
#include "hal/hal_i2c.h"
#include "hal/hal_gpio.h"

static Application APP_I2CTest("APP_I2CTest", 2000);

#define CS43L22_DAC_ADDR    0x4A

HAL_I2C i2c1(I2C_IDX1); // uses PB6 and PB9
HAL_GPIO pd4(GPIO_052); // enable-pin of DAC "CS43L22"
HAL_GPIO ledRed(GPIO_061);
HAL_GPIO ledOrange(GPIO_062);




class I2CIOEventReceiver : public IOEventReceiver{
public:
    void onWriteFinished() {
        static uint32_t ledLevel = 0;
        ledLevel ^= 1;
        ledRed.setPins(ledLevel);
    }

    void onReadFinished() {
        static uint32_t ledLevel = 0;
        ledLevel ^= 1;
        ledOrange.setPins(ledLevel);
    }
} i2cIOEventReceiver;




class I2CTest: public StaticThread<> {
public:
	I2CTest(const char* name) : StaticThread<>(name) {}

	void init() {
		i2c1.init();
		i2c1.setIoEventReceiver(&i2cIOEventReceiver);

		pd4.init(true,1,1); // only enable chip when test is active
		ledOrange.init(true,1,0);
		ledRed.init(true,1,0);
	}


	void run() {
		uint8_t rxBuf[1]={0};
		uint8_t txBuf[1]={0x01};
		int32_t err[2] = {0};

		while (1) {
		    /** check out CS43L22 (onboard) **/
		    rxBuf[0] = 0;

			err[0] = i2c1.write(CS43L22_DAC_ADDR,txBuf,1);
			i2c1.suspendUntilWriteFinished();

			err[1] = i2c1.read(CS43L22_DAC_ADDR,rxBuf,1);
			i2c1.suspendUntilReadFinished();

            if (printError("CS43L22", err, 2) > 0) {
                PRINTF("Init I2C ...\n\n");
                i2c1.init();
            }else{
                PRINTF("CS43L22 ID[7..3], Rev[2..0] (0xE?): 0x%x\n\n",rxBuf[0]);
            }

			suspendCallerUntil(NOW()+1*SECONDS);
		}
	}


    int32_t printError(const char* dev, int32_t *err, uint32_t size){
        uint32_t i=0;
        uint32_t errCnt = 0;

        for (i=0;i<size;i++){
            if (err[i] < 0){
                PRINTF("%s I2C error:\n",dev);
                break;
            }
        }

        for (i=0;i<size;i++){
            if (err[i] < 0){
                errCnt++;
                PRINTF(" %d. %d\n",i+1,err[i]);
            }
        }

        /*if(errCnt>0){
            PRINTF("\n");
        }*/

        return errCnt;
    }
};

I2CTest i2cTest("I2CTest");
