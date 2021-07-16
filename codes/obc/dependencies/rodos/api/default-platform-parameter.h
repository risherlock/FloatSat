
#pragma once

/**
* @file platform-parameter.h
* @date 2019/09/24 
* @author Sergio Montenegro
*
* @brief default configuration parameters for all platforms
*
* Each platform which differs has to undefine the corresponding
* parameter an define again
* eg.
* #undef  XMALLOC_SIZE
* #define XMALLOC_SIZE 10000
*
*/


// ___________________  only defines, not in namespace


/*************** System Configuration *********/

#define XMALLOC_SIZE                2000000 //< Total memory for allocation (xmalloc) eg for all stacks
#define DEFAULT_STACKSIZE             32000 //< in bytes
#define SCHEDULER_STACKSIZE           DEFAULT_STACKSIZE  //< for the scheduler

#define PARAM_TIMER_INTERVAL          100000 //< in microseconds
#define TIME_SLICE_FOR_SAME_PRIORITY (100*MILLISECONDS) //< for switch threads with same priority

#define DEFAULT_THREAD_PRIORITY        100
#define MAX_THREAD_PRIORITY           1000

#define NETWORKREADER_PRIORITY        (MAX_THREAD_PRIORITY + 2) //< for priority ceiling
#define CEILING_PRIORITY              (NETWORKREADER_PRIORITY + 1)
#define FLOORING_PRIORITY             10

#define MAX_NUMBER_OF_NODES            10 //< for gateway if it has to forward msgs
#define MAX_NETWORK_MESSAGE_LENGTH   1300
#define MAX_SUBSCRIBERS                60 //< per node, for the gateway topic reporter

#define SPRINTF_MAX_SIZE              1000 

#define UDP_INCOMMIG_BUF_LEN          500 //number of "NetworkMessage (1300 Bytes each)" in FIFO


//___________________________________________________ Important: Correct platform dependent parameter
//____________________________________________________It would be terrible if you forget to include it! Therefore I do it here. 
#include "platform-parameter.h"   // This comes from the platform directory (eg. stm32f4, sf2, ppc, linux-x85)




