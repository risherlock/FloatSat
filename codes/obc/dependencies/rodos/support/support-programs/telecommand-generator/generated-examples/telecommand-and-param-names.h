

//--- generated file, DO NOT modify
//--- see telecommand-and-param-names.cpp
#pragma once
namespace Common {


struct TelecommandCodes {
    const char* app; const char* cmdName; const char* cmdMnemonic; int service; int subservice; const char* paramDef[20];
};
TelecommandCodes telecommands[] = {
    { "XXX", "XXX_NOP", "XXXNOP", 110, 1, { 0, 0 } },
    { "XXX", "XXX_SET_HISTORY_RATE", "XXXHIRA", 110, 2, { "int8_t", "RATE", 0, 0 } },
    { "XXX", "XXX_SET_PARAM", "XXXSETPA", 110, 3, { "int8_t", "INDEX", "int32_t", "VALUE", 0, 0 } },
    { "XXX", "XXX_CLEAR_VARS", "XXXCLRV", 110, 4, { 0, 0 } },
    { "XXX", "XXX_SEND_EXTENDTELEM", "XXXEXTM", 110, 5, { 0, 0 } },
    { "XXX", "XXX_WHICH_NEEDS_LONG_TIME", "XXXLONG", 110, 6, { 0, 0 } },
    { "TEST", "TST_NOP", "TSTNOP", 17, 1, { 0, 0 } },
    { "GLOBAL", "GLB_NOP", "GLBNOP", 120, 1, { 0, 0 } },
    { "GLOBAL", "GLB_SEND_EXTENDED_TM", "GLBEXTM", 120, 2, { 0, 0 } },
    { "COMMON", "CMM_NOP", "CMMNOP", 121, 1, { 0, 0 } },
    { "COMMON", "CMM_SET_VERBOSITY", "CMMVERV", 121, 2, { "int8_t", "VERBOSITY", 0, 0 } },
    { "COMMON", "CMM_SET_IN_LEOP", "CMMINLE", 121, 3, { 0, 0 } },
    { "COMMON", "CMM_SET_OUT_OF_LEOP", "CMMOUTLE", 121, 4, { 0, 0 } },
    { "COMMON", "CMM_GOTOSAFEMODE", "CMMSAFE", 121, 5, { 0, 0 } },
    { "COMMON", "CMM_SET_MODE", "CMMMODE", 121, 6, { "int8_t", "MODE", 0, 0 } },
    { "COMMANDER", "CMD_NOP", "CMDNOPE", 1, 1, { 0, 0 } },
    { "HOUSEKEEPER", "HKE_NOP", "HKENOP", 3, 1, { 0, 0 } },
    { "HOUSEKEEPER", "HKE_SET_RATE", "HKESETRA", 3, 2, { "int8_t", "RATE", 0, 0 } },
    { "HOUSEKEEPER", "HKE_START_DOWNLINK", "HKEDOWON", 3, 3, { 0, 0 } },
    { "HOUSEKEEPER", "HKE_STOP_DOWNLINK", "HKEDOWOF", 3, 4, { 0, 0 } },
    { "TIMEDCMDMGR", "TCM_NOP", "TCMNOP", 18, 1, { 0, 0 } },
    { "TIMEDCMDMGR", "TCM_CLEAR_TT_LIST", "TCMCLRLS", 18, 2, { 0, 0 } },
    { "TIMEDCMDMGR", "TCM_ACTIVATE_PENDING_CMDS", "TCMACTVC", 18, 3, { 0, 0 } },
    { "TIMEDCMDMGR", "TCM_DELETE_PENDING_CMDS", "TCMDELC", 18, 4, { 0, 0 } },
    { "TIMEDCMDMGR", "TCM_SEND_TT_LIST", "TCMSNDLS", 18, 5, { 0, 0 } },
    { "TIMEDCMDMGR", "TMC_APPEND_TIME_TIMED_CMD", "TMCAPPCM", 13, 6, { "int32_t", "SECS", "char*", "TTC_SP", 0, 0 } },
    { "ADNOMALYREP", "ANM_NOP", "ANMNOP", 116, 1, { 0, 0 } },
    { "ADNOMALYREP", "ANM_SEND_REPORT", "ANMREPR", 116, 2, { 0, 0 } },
    { "ADNOMALYREP", "ANM_SEND_RINGBUFFER", "ANMSENRG", 116, 4, { 0, 0 } },
    { "ADNOMALYREP", "ANM_CLEAR_ERR_COUNTER", "ANMCLRER", 116, 5, { 0, 0 } },
    { "BOOTMANAGER", "BOM_NOP", "BOMNOP", 123, 1, { 0, 0 } },
    { "BOOTMANAGER", "BOM_JUMP_TO_BOOTIMAGE", "BOMJMP", 123, 2, { "int8_t", "INDEX", 0, 0 } },
    { "BOOTMANAGER", "BOM_REINIT_BOOT_LIST", "BOMREIN", 123, 3, { "int8_t", "INDEX", "int8_t", "INDEX2", "int8_t", "INDEX3", 0, 0 } },
    { "BOOTMANAGER", "BOM_APPEND_BOOT_IDYX", "BOMAPP", 123, 4, { "int8_t", "INDEX", 0, 0 } },
    { "BOOTMANAGER", "BOM_SEND_BOOTLIST", "BOMSEND", 123, 5, { 0, 0 } },
    { "BOOTMANAGER", "BOM_REBOOT", "BOMREBO", 123, 6, { 0, 0 } },
    { "BOOTMANAGER", "BOM_CHECK_BOOTIMAGE", "BOMCHECK", 123, 7, { "int8_t", "INDEX", 0, 0 } },
    { "POWDIST", "POD_NOP", "PODNOP", 124, 1, { 0, 0 } },
    { "POWDIST", "POD_ENABLE_CONTROL", "PODON", 124, 2, { 0, 0 } },
    { "POWDIST", "POD_DISABLE_CONTROL", "PODUNTOF", 124, 3, { 0, 0 } },
    { "POWDIST", "POD_UNIT_ON", "PODUNTON", 124, 4, { "int8_t", "INDEX", 0, 0 } },
    { "POWDIST", "POD_UNIT_OFF", "PODOFF", 124, 5, { "int8_t", "INDEX", 0, 0 } },
    { "POWDIST", "POD_SETCONF_BITMAP", "PODBMAP", 124, 6, { "int32_t", "BITMAP1", "int32_t", "BITMAP2", "int32_t", "BITMAP3", "int32_t", "BITMAP4", 0, 0 } },
    { "POWDIST", "POD_SET_RANGE", "PODRANG", 124, 7, { "int8_t", "INDEX", "int16_t", "MIN", "int16_t", "MAX", 0, 0 } },
    { "POWDIST", "POD_SEND_PWR_SETTINGS", "PODSEND", 124, 8, { 0, 0 } },
    { "THEMRALCNTR", "TER_NOP", "TERNOP", 125, 1, { 0, 0 } },
    { "THEMRALCNTR", "TER_ENABLE_CONTROL", "TERON", 125, 2, { 0, 0 } },
    { "THEMRALCNTR", "TER_DISABLE_CONTROL", "TEROFF", 125, 3, { 0, 0 } },
    { "THEMRALCNTR", "TER_HEATER_ON", "TERHEA1", 125, 4, { "int8_t", "INDEX", 0, 0 } },
    { "THEMRALCNTR", "TER_HEATER_OFF", "TERHEA0", 125, 5, { "int8_t", "INDEX", 0, 0 } },
    { "THEMRALCNTR", "TER_HEATPIPE_CNTRL", "TERHEAP", 125, 6, { "int8_t", "HP_CONDU1", "int8_t", "HP_CONDU2", 0, 0 } },
    { "THEMRALCNTR", "TER_SET_RANGE", "TERRANG", 125, 7, { "int8_t", "INDEX", "int16_t", "MIN", "int16_t", "MAX", 0, 0 } },
    { "REDUNDANZMNG", "RED_NOP", "REDNOP", 126, 1, { 0, 0 } },
    { "REDUNDANZMNG", "RED_SEND_DUL_TO_OTHERNODE", "REDSDULS", 126, 2, { "int32_t", "LEN", 0, 0 } },
    { "REDUNDANZMNG", "RED_ACCEPT_DUL_OTHERNODE", "REDSDULA", 126, 3, { "int32_t", "LEN", 0, 0 } },
    { "REDUNDANZMNG", "RED_RESET_PINPOG_CNT", "REDRSTP", 126, 4, { 0, 0 } },
    { "REDUNDANZMNG", "RED_TOGGLE_WORKER_MONITOR", "REDTOG", 126, 5, { 0, 0 } },
    { "REDUNDANZMNG", "RED_ENALBE_RECOVER_OTHER", "RECREC1", 126, 6, { 0, 0 } },
    { "REDUNDANZMNG", "RED_DISABLE_RECOVER_OTHER", "REDREC0", 126, 7, { 0, 0 } },
    { "WATCHDOG", "WAT_NOP", "WATNOP", 107, 1, { 0, 0 } },
    { "WATCHDOG", "WAT_RESET_NODE", "WATRSTND", 107, 2, { 0, 0 } },
    { "WATCHDOG", "WAT_NOT_ALLOW_RESET", "WATDISBL", 107, 3, { 0, 0 } },
    { "WATCHDOG", "WAT_ALLOW_RESET", "WATENABL", 107, 4, { 0, 0 } },
    { "TIMECONTROL", "TIM_NOP", "TIMNOPE", 9, 1, { 0, 0 } },
    { "TIMECONTROL", "TIM_SET_UTC", "TIMSETUT", 9, 2, { "int32_t", "SECS", "int32_t", "MILLIS", 0, 0 } },
    { "TIMECONTROL", "TIM_SET_LEAP", "TIMLEAP", 9, 3, { "int32_t", "SECS", 0, 0 } },
    { "TIMECONTROL", "TIM_SET_UTC_DELTA", "TIMDELT", 9, 4, { "int32_t", "SECS", "int32_t", "MILLIS", 0, 0 } },
    { "TIMECONTROL", "TIM_ENABLE_DISTR_UTC", "MENDIS", 9, 5, { 0, 0 } },
    { "TIMECONTROL", "TIM_DISABLE_DISTR_UTC", "TIMDISDI", 9, 6, { 0, 0 } },
    { "TIMECONTROL", "TIM_ENABLE_1PPS_SYNC", "TIM1PPS1", 9, 7, { 0, 0 } },
    { "TIMECONTROL", "TIM_DISABLE_1PPS_SYNC", "TIM1PPS0", 9, 8, { 0, 0 } },
    { "TIMECONTROL", "TIM_SET_CLOCK_DRIFT", "TIMDRIF", 9, 9, { 0, 0 } },
    { "TIMECONTROL", "TIM_SEND_TIMEMODEL", "TIMSENDM", 9, 10, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_NOP", "DWLNOP", 111, 1, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_SET_LOW_RATE", "DWLLOW", 111, 2, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_SET_HI_RATE", "DWLHI", 111, 3, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_START_SBAND", "DWLSB1", 111, 4, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_STOP_SBAND", "DWLSB0", 111, 5, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_START_XBAND", "DWLXB1", 111, 6, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_STOP_XBAND", "DWLXB0", 111, 7, { 0, 0 } },
    { "DOWNLINKMNG", "DWL_CONFIG_SBAND", "DWLSBCON", 111, 8, { "int32_t", "UNKNOWN", 0, 0 } },
    { "DOWNLINKMNG", "DWL_CONFIG_XBAND", "DWLXBCON", 111, 9, { "int32_t", "UNKNOWN", 0, 0 } },
    { "DOWNLINKMNG", "DWL_CONFIG_SBAND_RED", "DWLSRED", 111, 10, { "int32_t", "UNKNOWN", 0, 0 } },
    { "DOWNLINKMNG", "DWL_CONFIG_XBAND_RED", "DWLXRED", 111, 11, { "int32_t", "UNKNOWN", 0, 0 } },
    { "SAFEMODELIST", "SFL_NOP", "SFLNOP", 114, 1, { 0, 0 } },
    { "SAFEMODELIST", "SFL_GOTOSAFEMODE", "SFLSAFEM", 114, 2, { 0, 0 } },
    { "SAFEMODELIST", "SFL_SET_MODE", "SFLSETMO", 114, 3, { "int8_t", "NEW_MODE", 0, 0 } },
    { "SAFEMODELIST", "SFL_SEND_SFLIST", "SFLSNDLS", 114, 4, { 0, 0 } },
    { "SAFEMODELIST", "SFL_UPLOADLST", "SFLUPLLS", 114, 5, { 0, 0 } },
    { "SAFEMODELIST", "SFL_NEXT_START_IN_LEOP", "SFLINLEO", 114, 6, { 0, 0 } },
    { "SAFEMODELIST", "SFL_OUT_OF_LEOP", "SFLOUTLE", 114, 7, { 0, 0 } },
    { "SAFEMODELIST", "SFL_REQUEST_STARTLIST", "SFLREQLS", 114, 8, { 0, 0 } },
    { "DATAUPLOAD", "DUL_NOP", "DULNOP", 14, 1, { 0, 0 } },
    { "DATAUPLOAD", "DUL_RESET_DUL_PROTOCOL", "DULRSTPR", 14, 2, { 0, 0 } },
    { "DATAUPLOAD", "DUL_DISTRIBUTE_DATA", "DULDISTR", 14, 3, { 0, 0 } },
    { "DATAUPLOAD", "DUL_WRITE_SEGMENT", "DULWRITE", 14, 4, { "int8_t", "INDEX", 0, 0 } },
    { "DATAUPLOAD", "DUL_UPLOADBLOCK", "DULUPBLK", 14, 5, { "int16_t", "ENTRY_NR", "int16_t", "BLK_LEN", "int32_t", "BLK_CR", 0, 0 } },
    { "DATAUPLOAD", "DUL_CHECK_UPLOAD_SEG", "DULCKSEG", 14, 6, { "int32_t", "LEN", "int32_t", "EXPT_CRC", 0, 0 } },
    { "SURVEILLANCE", "SUR_NOP", "SURNOP", 117, 1, { 0, 0 } },
    { "SURVEILLANCE", "SUR_SET_RANGES", "SURRANG", 117, 2, { "int8_t", "INDEX", "int16_t", "MIN", "int16_t", "MAX", 0, 0 } },
    { "SURVEILLANCE", "SUR_ENABLE_THREAD_CHECK", "SURTHR1", 117, 3, { 0, 0 } },
    { "SURVEILLANCE", "SUR_DISABLE_THREAD_CHECK", "SURTHR0", 117, 4, { 0, 0 } },
    { "SURVEILLANCE", "SUR_ENABLE_MEM_CHECK", "SURMEM1", 117, 5, { 0, 0 } },
    { "SURVEILLANCE", "SUR_DISABLE_MEM_CHECK", "SURMEM0", 117, 6, { 0, 0 } },
    { "SURVEILLANCE", "SUR_SEND_ANALOGVALS", "SURSENDA", 117, 7, { 0, 0 } },
    { "SURVEILLANCE", "SUR_ENABLE_LATCHUPPROT", "SURLATC1", 117, 8, { 0, 0 } },
    { "SURVEILLANCE", "SUR_DISABLE_LATCHUPPROT", "SURLATC0", 117, 9, { 0, 0 } },
    { "ROVERCNTR", "ROV_NOP", "ROVNOP", 118, 1, { 0, 0 } },
    { "ROVERCNTR", "ROV_SEND_EXTENDED_TM", "ROVEXTM", 118, 2, { 0, 0 } },
    { "ROVERCNTR", "ROV_START_ANT_SCAN", "ROVSCNB", 118, 3, { 0, 0 } },
    { "ROVERCNTR", "ROV_STOP_ANT_SCAN", "ROVSCNS", 118, 4, { 0, 0 } },
    { "ROVERCNTR", "ROV_SET_ANT_POS", "ROVSTAPO", 118, 5, { "int32_t", "ANT_ANGLE", 0, 0 } },
    { "ROVERCNTR", "ROV_STOP", "ROVSTOP", 118, 6, { 0, 0 } },
    { "ROVERCNTR", "ROV_CNTR_WHEEL", "ROVWHEE", 118, 7, { "int8_t", "INDEX", "int16_t", "ANGLE", "int16_t", "SPEED", 0, 0 } },
    { "ROVERCNTR", "ROV_DRIVE", "ROVDRIVE", 118, 8, { "int16_t", "X_SPEED", "int16_t", "Y_SPEED", "int16_t", "ROT_SPEED", "int16_t", "TIMOUT", 0, 0 } },
    { "ATTITUDECNTR", "ATC_NOP", "ATCNOP", 119, 1, { 0, 0 } },
    { "ATTITUDECNTR", "ATC_SET_ATC_MODE", "ATCMOD", 119, 2, { "int8_t", "ATC_MODE", 0, 0 } },
    { "ATTITUDECNTR", "ATC_GOTO_ATC_SAFEMODE", "ATCSAFE", 119, 3, { 0, 0 } },
    { "ATTITUDECNTR", "ATC_GOTO_ATC_SUSPENMODE", "ATCSUSP", 119, 4, { 0, 0 } },
    { "GUINDANCE", "GUI_NOP", "GUINOP", 121, 1, { 0, 0 } },
    { "NAVIGATION", "NAV_NOP", "NAVNOP", 120, 1, { 0, 0 } },
    { "LANDERCONTROL", "LND_NOP", "LNDNOP", 122, 1, { 0, 0 } },
    { "PAYLOADCNTR", "PAY_NOP", "PAYNOP", 123, 1, { 0, 0 } },
    { 0, 0, 0, 0, 0 }
};

#ifndef CMD_PARAMETER_STRUCT
#define CMD_PARAMETER_STRUCT

struct CmdParameter {
    int8_t     RATE; 
    int8_t     INDEX; 
    int32_t    VALUE; 
    int8_t     VERBOSITY; 
    int8_t     MODE; 
    int32_t    SECS; 
    char*      TTC_SP; 
    int8_t     INDEX2; 
    int8_t     INDEX3; 
    int32_t    BITMAP1; 
    int32_t    BITMAP2; 
    int32_t    BITMAP3; 
    int32_t    BITMAP4; 
    int16_t    MIN; 
    int16_t    MAX; 
    int8_t     HP_CONDU1; 
    int8_t     HP_CONDU2; 
    int32_t    LEN; 
    int32_t    MILLIS; 
    int32_t    UNKNOWN; 
    int8_t     NEW_MODE; 
    int16_t    ENTRY_NR; 
    int16_t    BLK_LEN; 
    int32_t    BLK_CR; 
    int32_t    EXPT_CRC; 
    int32_t    ANT_ANGLE; 
    int16_t    ANGLE; 
    int16_t    SPEED; 
    int16_t    X_SPEED; 
    int16_t    Y_SPEED; 
    int16_t    ROT_SPEED; 
    int16_t    TIMOUT; 
    int8_t     ATC_MODE; 

    inline void deserialize(char* buf) {
        BasicSerializers::deserialize(RATE,      buf+ 0);
        BasicSerializers::deserialize(INDEX,     buf+ 0);
        BasicSerializers::deserialize(VALUE,     buf+ 1);
        BasicSerializers::deserialize(VERBOSITY, buf+ 0);
        BasicSerializers::deserialize(MODE,      buf+ 0);
        BasicSerializers::deserialize(SECS,      buf+ 0);
        TTC_SP = buf+ 4;
        BasicSerializers::deserialize(INDEX2,    buf+ 1);
        BasicSerializers::deserialize(INDEX3,    buf+ 2);
        BasicSerializers::deserialize(BITMAP1,   buf+ 0);
        BasicSerializers::deserialize(BITMAP2,   buf+ 4);
        BasicSerializers::deserialize(BITMAP3,   buf+ 8);
        BasicSerializers::deserialize(BITMAP4,   buf+12);
        BasicSerializers::deserialize(MIN,       buf+ 1);
        BasicSerializers::deserialize(MAX,       buf+ 3);
        BasicSerializers::deserialize(HP_CONDU1, buf+ 0);
        BasicSerializers::deserialize(HP_CONDU2, buf+ 1);
        BasicSerializers::deserialize(LEN,       buf+ 0);
        BasicSerializers::deserialize(MILLIS,    buf+ 4);
        BasicSerializers::deserialize(UNKNOWN,   buf+ 0);
        BasicSerializers::deserialize(NEW_MODE,  buf+ 0);
        BasicSerializers::deserialize(ENTRY_NR,  buf+ 0);
        BasicSerializers::deserialize(BLK_LEN,   buf+ 2);
        BasicSerializers::deserialize(BLK_CR,    buf+ 4);
        BasicSerializers::deserialize(EXPT_CRC,  buf+ 4);
        BasicSerializers::deserialize(ANT_ANGLE, buf+ 0);
        BasicSerializers::deserialize(ANGLE,     buf+ 1);
        BasicSerializers::deserialize(SPEED,     buf+ 3);
        BasicSerializers::deserialize(X_SPEED,   buf+ 0);
        BasicSerializers::deserialize(Y_SPEED,   buf+ 2);
        BasicSerializers::deserialize(ROT_SPEED, buf+ 4);
        BasicSerializers::deserialize(TIMOUT,    buf+ 6);
        BasicSerializers::deserialize(ATC_MODE,  buf+ 0);
    }
};
#endif
} // namespace
