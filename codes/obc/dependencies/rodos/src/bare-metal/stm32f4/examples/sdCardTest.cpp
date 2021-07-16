#include "rodos.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>

extern "C"{
	#include "stm324xg_eval_sdio_sd.h"
	#include "sd_tests.h"
}

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif
extern	SD_Error Status; 				// defined in sd_tests.c
extern	__IO uint32_t SDCardOperation;	// defined in sd_tests.c
#ifndef NO_RODOS_NAMESPACE
}
#endif


class SDCardThread : public StaticThread<> {
public:
	SDCardThread(const char* name):StaticThread<>(name){}

	void run(){
		  /* Interrupt Config */
		  NVIC_Configuration();

		  /*------------------------------ SD Init ---------------------------------- */
		  if((Status = SD_Init()) != SD_OK)
		  {
			  xprintf("Init failed\n");
		  }

		  while((Status == SD_OK) && (SDCardOperation != SD_OPERATION_END) && (SD_Detect()== SD_PRESENT))
		  {
		    switch(SDCardOperation)
		    {
		      /*-------------------------- SD Erase Test ---------------------------- */
		      case (SD_OPERATION_ERASE):
		      {
		        SD_EraseTest();
		        SDCardOperation = SD_OPERATION_BLOCK;
		        break;
		      }
		      /*-------------------------- SD Single Block Test --------------------- */
		      case (SD_OPERATION_BLOCK):
		      {
		        SD_SingleBlockTest();
		        SDCardOperation = SD_OPERATION_MULTI_BLOCK;
		        break;
		      }
		      /*-------------------------- SD Multi Blocks Test --------------------- */
		      case (SD_OPERATION_MULTI_BLOCK):
		      {
		        SD_MultiBlockTest();
		        SDCardOperation = SD_OPERATION_END;
		        break;
		      }
		    }
		  }

		  /* Infinite loop */
		  while (1)
		  {}
	}
};

SDCardThread sdTest1("SD Card Test");

/******************/

