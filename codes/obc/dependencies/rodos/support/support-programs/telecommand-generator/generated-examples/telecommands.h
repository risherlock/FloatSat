

//--- generated file, DO NOT modify
//--- see telecommand-generator.cpp
#pragma once

namespace Common {

struct TeleCommandCodes {

    static const int XXX                            =  110;           // service for all XXX_* Commands
    static const int XXX_NOP                        = (110 << 8) |  1;
    static const int XXX_SET_HISTORY_RATE           = (110 << 8) |  2;
    static const int XXX_SET_PARAM                  = (110 << 8) |  3;
    static const int XXX_CLEAR_VARS                 = (110 << 8) |  4;
    static const int XXX_SEND_EXTENDTELEM           = (110 << 8) |  5;
    static const int XXX_WHICH_NEEDS_LONG_TIME      = (110 << 8) |  6;

    static const int TST                            =   17;           // service for all TST_* Commands
    static const int TST_NOP                        = ( 17 << 8) |  1;

    static const int GLB                            =  120;           // service for all GLB_* Commands
    static const int GLB_NOP                        = (120 << 8) |  1;
    static const int GLB_SEND_EXTENDED_TM           = (120 << 8) |  2;

    static const int CMM                            =  121;           // service for all CMM_* Commands
    static const int CMM_NOP                        = (121 << 8) |  1;
    static const int CMM_SET_VERBOSITY              = (121 << 8) |  2;
    static const int CMM_SET_IN_LEOP                = (121 << 8) |  3;
    static const int CMM_SET_OUT_OF_LEOP            = (121 << 8) |  4;
    static const int CMM_GOTOSAFEMODE               = (121 << 8) |  5;
    static const int CMM_SET_MODE                   = (121 << 8) |  6;

    static const int CMD                            =    1;           // service for all CMD_* Commands
    static const int CMD_NOP                        = (  1 << 8) |  1;

    static const int HKE                            =    3;           // service for all HKE_* Commands
    static const int HKE_NOP                        = (  3 << 8) |  1;
    static const int HKE_SET_RATE                   = (  3 << 8) |  2;
    static const int HKE_START_DOWNLINK             = (  3 << 8) |  3;
    static const int HKE_STOP_DOWNLINK              = (  3 << 8) |  4;

    static const int TCM                            =   18;           // service for all TCM_* Commands
    static const int TCM_NOP                        = ( 18 << 8) |  1;
    static const int TCM_CLEAR_TT_LIST              = ( 18 << 8) |  2;
    static const int TCM_ACTIVATE_PENDING_CMDS      = ( 18 << 8) |  3;
    static const int TCM_DELETE_PENDING_CMDS        = ( 18 << 8) |  4;
    static const int TCM_SEND_TT_LIST               = ( 18 << 8) |  5;
    static const int TMC_APPEND_TIME_TIMED_CMD      = ( 13 << 8) |  6;

    static const int ANM                            =  116;           // service for all ANM_* Commands
    static const int ANM_NOP                        = (116 << 8) |  1;
    static const int ANM_SEND_REPORT                = (116 << 8) |  2;
    static const int ANM_SEND_RINGBUFFER            = (116 << 8) |  4;
    static const int ANM_CLEAR_ERR_COUNTER          = (116 << 8) |  5;

    static const int BOM                            =  123;           // service for all BOM_* Commands
    static const int BOM_NOP                        = (123 << 8) |  1;
    static const int BOM_JUMP_TO_BOOTIMAGE          = (123 << 8) |  2;
    static const int BOM_REINIT_BOOT_LIST           = (123 << 8) |  3;
    static const int BOM_APPEND_BOOT_IDYX           = (123 << 8) |  4;
    static const int BOM_SEND_BOOTLIST              = (123 << 8) |  5;
    static const int BOM_REBOOT                     = (123 << 8) |  6;
    static const int BOM_CHECK_BOOTIMAGE            = (123 << 8) |  7;

    static const int POD                            =  124;           // service for all POD_* Commands
    static const int POD_NOP                        = (124 << 8) |  1;
    static const int POD_ENABLE_CONTROL             = (124 << 8) |  2;
    static const int POD_DISABLE_CONTROL            = (124 << 8) |  3;
    static const int POD_UNIT_ON                    = (124 << 8) |  4;
    static const int POD_UNIT_OFF                   = (124 << 8) |  5;
    static const int POD_SETCONF_BITMAP             = (124 << 8) |  6;
    static const int POD_SET_RANGE                  = (124 << 8) |  7;
    static const int POD_SEND_PWR_SETTINGS          = (124 << 8) |  8;

    static const int TER                            =  125;           // service for all TER_* Commands
    static const int TER_NOP                        = (125 << 8) |  1;
    static const int TER_ENABLE_CONTROL             = (125 << 8) |  2;
    static const int TER_DISABLE_CONTROL            = (125 << 8) |  3;
    static const int TER_HEATER_ON                  = (125 << 8) |  4;
    static const int TER_HEATER_OFF                 = (125 << 8) |  5;
    static const int TER_HEATPIPE_CNTRL             = (125 << 8) |  6;
    static const int TER_SET_RANGE                  = (125 << 8) |  7;

    static const int RED                            =  126;           // service for all RED_* Commands
    static const int RED_NOP                        = (126 << 8) |  1;
    static const int RED_SEND_DUL_TO_OTHERNODE      = (126 << 8) |  2;
    static const int RED_ACCEPT_DUL_OTHERNODE       = (126 << 8) |  3;
    static const int RED_RESET_PINPOG_CNT           = (126 << 8) |  4;
    static const int RED_TOGGLE_WORKER_MONITOR      = (126 << 8) |  5;
    static const int RED_ENALBE_RECOVER_OTHER       = (126 << 8) |  6;
    static const int RED_DISABLE_RECOVER_OTHER      = (126 << 8) |  7;

    static const int WAT                            =  107;           // service for all WAT_* Commands
    static const int WAT_NOP                        = (107 << 8) |  1;
    static const int WAT_RESET_NODE                 = (107 << 8) |  2;
    static const int WAT_NOT_ALLOW_RESET            = (107 << 8) |  3;
    static const int WAT_ALLOW_RESET                = (107 << 8) |  4;

    static const int TIM                            =    9;           // service for all TIM_* Commands
    static const int TIM_NOP                        = (  9 << 8) |  1;
    static const int TIM_SET_UTC                    = (  9 << 8) |  2;
    static const int TIM_SET_LEAP                   = (  9 << 8) |  3;
    static const int TIM_SET_UTC_DELTA              = (  9 << 8) |  4;
    static const int TIM_ENABLE_DISTR_UTC           = (  9 << 8) |  5;
    static const int TIM_DISABLE_DISTR_UTC          = (  9 << 8) |  6;
    static const int TIM_ENABLE_1PPS_SYNC           = (  9 << 8) |  7;
    static const int TIM_DISABLE_1PPS_SYNC          = (  9 << 8) |  8;
    static const int TIM_SET_CLOCK_DRIFT            = (  9 << 8) |  9;
    static const int TIM_SEND_TIMEMODEL             = (  9 << 8) | 10;

    static const int DWL                            =  111;           // service for all DWL_* Commands
    static const int DWL_NOP                        = (111 << 8) |  1;
    static const int DWL_SET_LOW_RATE               = (111 << 8) |  2;
    static const int DWL_SET_HI_RATE                = (111 << 8) |  3;
    static const int DWL_START_SBAND                = (111 << 8) |  4;
    static const int DWL_STOP_SBAND                 = (111 << 8) |  5;
    static const int DWL_START_XBAND                = (111 << 8) |  6;
    static const int DWL_STOP_XBAND                 = (111 << 8) |  7;
    static const int DWL_CONFIG_SBAND               = (111 << 8) |  8;
    static const int DWL_CONFIG_XBAND               = (111 << 8) |  9;
    static const int DWL_CONFIG_SBAND_RED           = (111 << 8) | 10;
    static const int DWL_CONFIG_XBAND_RED           = (111 << 8) | 11;

    static const int SFL                            =  114;           // service for all SFL_* Commands
    static const int SFL_NOP                        = (114 << 8) |  1;
    static const int SFL_GOTOSAFEMODE               = (114 << 8) |  2;
    static const int SFL_SET_MODE                   = (114 << 8) |  3;
    static const int SFL_SEND_SFLIST                = (114 << 8) |  4;
    static const int SFL_UPLOADLST                  = (114 << 8) |  5;
    static const int SFL_NEXT_START_IN_LEOP         = (114 << 8) |  6;
    static const int SFL_OUT_OF_LEOP                = (114 << 8) |  7;
    static const int SFL_REQUEST_STARTLIST          = (114 << 8) |  8;

    static const int DUL                            =   14;           // service for all DUL_* Commands
    static const int DUL_NOP                        = ( 14 << 8) |  1;
    static const int DUL_RESET_DUL_PROTOCOL         = ( 14 << 8) |  2;
    static const int DUL_DISTRIBUTE_DATA            = ( 14 << 8) |  3;
    static const int DUL_WRITE_SEGMENT              = ( 14 << 8) |  4;
    static const int DUL_UPLOADBLOCK                = ( 14 << 8) |  5;
    static const int DUL_CHECK_UPLOAD_SEG           = ( 14 << 8) |  6;

    static const int SUR                            =  117;           // service for all SUR_* Commands
    static const int SUR_NOP                        = (117 << 8) |  1;
    static const int SUR_SET_RANGES                 = (117 << 8) |  2;
    static const int SUR_ENABLE_THREAD_CHECK        = (117 << 8) |  3;
    static const int SUR_DISABLE_THREAD_CHECK       = (117 << 8) |  4;
    static const int SUR_ENABLE_MEM_CHECK           = (117 << 8) |  5;
    static const int SUR_DISABLE_MEM_CHECK          = (117 << 8) |  6;
    static const int SUR_SEND_ANALOGVALS            = (117 << 8) |  7;
    static const int SUR_ENABLE_LATCHUPPROT         = (117 << 8) |  8;
    static const int SUR_DISABLE_LATCHUPPROT        = (117 << 8) |  9;

    static const int ROV                            =  118;           // service for all ROV_* Commands
    static const int ROV_NOP                        = (118 << 8) |  1;
    static const int ROV_SEND_EXTENDED_TM           = (118 << 8) |  2;
    static const int ROV_START_ANT_SCAN             = (118 << 8) |  3;
    static const int ROV_STOP_ANT_SCAN              = (118 << 8) |  4;
    static const int ROV_SET_ANT_POS                = (118 << 8) |  5;
    static const int ROV_STOP                       = (118 << 8) |  6;
    static const int ROV_CNTR_WHEEL                 = (118 << 8) |  7;
    static const int ROV_DRIVE                      = (118 << 8) |  8;

    static const int ATC                            =  119;           // service for all ATC_* Commands
    static const int ATC_NOP                        = (119 << 8) |  1;
    static const int ATC_SET_ATC_MODE               = (119 << 8) |  2;
    static const int ATC_GOTO_ATC_SAFEMODE          = (119 << 8) |  3;
    static const int ATC_GOTO_ATC_SUSPENMODE        = (119 << 8) |  4;

    static const int GUI                            =  121;           // service for all GUI_* Commands
    static const int GUI_NOP                        = (121 << 8) |  1;

    static const int NAV                            =  120;           // service for all NAV_* Commands
    static const int NAV_NOP                        = (120 << 8) |  1;

    static const int LND                            =  122;           // service for all LND_* Commands
    static const int LND_NOP                        = (122 << 8) |  1;

    static const int PAY                            =  123;           // service for all PAY_* Commands
    static const int PAY_NOP                        = (123 << 8) |  1;
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
