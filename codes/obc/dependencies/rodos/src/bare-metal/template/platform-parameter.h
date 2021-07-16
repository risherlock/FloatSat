
/**
 * @file params.h
 * @date 2019/08/20
 * @author Sergio Montenegro
 *
 * @brief configuration parameters ...
 *
 */

#pragma once

// ___________________  only defines, not in namespace

/** Version Number */
#define OSVERSION "Template"

/*************** System Configuration *********/

#undef  XMALLOC_SIZE
#define XMALLOC_SIZE		40*1024
#undef  DEFAULT_STACKSIZE
#define DEFAULT_STACKSIZE	2000
#undef  SCHEDULER_STACKSIZE
#define SCHEDULER_STACKSIZE	8
#undef  UDP_INCOMMIG_BUF_LEN
#define UDP_INCOMMIG_BUF_LEN          4 //number of "NetworkMessage (1300 Bytes each)" in FIFO

