#ifndef STANDARTTM_H
#define STANDARTTM_H



#include "stdint.h"

struct StandardTelemetry {

	static const int8_t SERVICE = 3; 
	static const int8_t SUBSERVICE = 25; 

//flags

	uint32_t	CMM_NodeId; 
	uint32_t	XXX_DemoFlag; 
	uint32_t	HKE_Downlink_Running; 
	uint32_t	CMM_AmIWorker; 
	uint32_t	RED_CMM_AmIWorker; 
	uint32_t	WAT_TimingChritical; 
	uint32_t	WAT_ResetEnabled; 
	uint32_t	POD_Enabled; 
	uint32_t	TER_Enabled; 
	uint32_t	RED_Enabled; 
	uint32_t	WAT_Enabled; 
	uint32_t	DUP_Running; 
	uint32_t	ROV_Moving; 
	uint32_t	SUR_CheckingThreads; 
	uint32_t	SUR_CheckingMem; 
	uint32_t	ATC_XX; 
	uint32_t	NAV_XX; 
	uint32_t	GUI_XX; 
	uint32_t	LND_Running; 
	uint32_t	SUR_StackChritical; 
	uint32_t	SUR_LatchupProtection; 
	uint32_t	DRV_inDirectDrive; 
	uint32_t	DRV_autoLook; 
	uint32_t	ANT_autoPos; 
	uint32_t	ANT_autoTune; 
	uint32_t	SLP_autoTune; 
	uint32_t	Filler1; 
	uint32_t	Filler2; 
	uint32_t	Filler3; 

//variables


	uint8_t 	flags[4];  //Filled defined flags from flags section

	uint8_t 	XXX_Demo; 

	uint16_t	HKE_HistoryRate; 

	uint16_t	CMD_InmdiatCmdCnt;  ///< number of executed inmediat commands
	uint16_t	CMD_RejectedCmdCnt;  ///< number of accpeted but not executed commands
	uint16_t	CMD_ExecutedCmdCnt;  ///< number of accpeted but not executed commands
	uint16_t	CMD_LastCmdSeqCnt; 
	uint8_t 	CMD_cmsForOtherSat; 
	uint8_t 	CMD_tfErrsCnt; 

	uint16_t	TCM_NumOfFreeCmds; 
	uint32_t	TCM_NumOfDistrTimedCmds; 
	uint32_t	TCM_NumOfReceivedTimedCmds; 
	uint32_t	TCM_TimeOfNextCmd; 
	uint32_t	TCM_TimeOfLastCmd; 
	uint16_t	TCM_NumOfReadyCmds; 
	uint16_t	TCM_NumOfPendingCmds; 
	uint16_t	TCM_NumOfDeletedCmds; 

	uint16_t	ANM_AnomalyCnt; 
	uint16_t	ANM_LastAnomalyID; 
	uint32_t	ANM_LastAnomalyTime;  ///< UTC in Seconds
	uint8_t 	ANM_ReasonForSafeMode; 

	uint8_t 	CMM_Spacecraft_Mode; 
	int32_t 	CMM_Spacecraft_Mode_Entertime; 
	uint8_t 	CMM_BootCnt; 
	uint8_t 	CMM_SW_Version; 

	uint32_t	TIM_UTC_Secs;  ///< secods since 1.1.2000
	uint32_t	TIM_UTC_NsecsFraction;  ///< Nanosecods fraction since 1.1.2000
	uint32_t	TIM_UTC_Delta_Secs;  ///< UTC = boardTime + UTC_Delta 
	uint32_t	TIM_UTC_Delta_NsecsFraction;  ///< UTC = boardTime + UTC_Delta 
	uint32_t	TIM_BoardTime_Secs;  ///< seconds counter from last start
	uint32_t	TIM_BoardTime_NsecsFraction;  ///< nannoseconds fraction from last start

	uint8_t 	RED_noAnswerCnt; 
	uint8_t 	RED_pingPongCnt; 
	uint8_t 	RED_ANM_ReasonForSafeMode; 
	uint16_t	RED_ANM_AnomalyCnt; 
	uint8_t 	RED_CMM_Spacecraft_Mode; 
	uint8_t 	RED_CMM_BootCnt; 
	uint8_t 	RED_CMM_SW_Version; 
	uint32_t	RED_TIM_BoardTime_Secs; 
	uint32_t	RED_TIM_BoardTime_NsecsFraction; 
	uint32_t	RED_TIM_UTC_Secs; 
	uint32_t	RED_TIM_UTC_NsecsFraction; 
	uint8_t 	RED_UIN_BootAdr; 
	uint8_t 	RED_CMM_NodeId; 

	uint8_t 	UIN_BootAdr; 

	int16_t 	SUR_analogvals[64];  // WARNING hast to be consistent with ../common/analoginputs.h
	int32_t 	SUR_timeOfLastOOR; 
	int8_t  	SUR_lastChanOOR; 
	int16_t 	SUR_lastValOOR; 
	int8_t  	SUR_cpuLoad; 

	int8_t  	MOT_cmdCnt;  //only for Rover
	int8_t  	MOT_timeOutCnt;  //only for Rover

	float  	ANT_Intensity;  //only for Rover
	int16_t 	ANT_Position;  //only for Rover

	int32_t 	DRV_gotoSeqCnt;  //only for Rover

	int8_t  	SLP_Intensity;  //only for Rover
	int16_t 	SLP_MotPosition;  //only for Rover
	int16_t 	SLP_SunPositionAlpha;  //only for Rover
	int16_t 	SLP_SunPositionBeta;  //only for Rover

};



#endif // STANDARTTM_H
