/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

/* Include some files for defining library routines */
//#include <string.h>
//#include <sys/time.h>
//#include <limits.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LWIP_TIMEVAL_PRIVATE 0




/* Define platform endianness */
#ifndef BYTE_ORDER
	//#include <endian.h>
//	#if defined(__IEEE_BIG_ENDIAN)
//		#define BYTE_ORDER BIG_ENDIAN
//	#elif defined(__IEEE_LITTLE_ENDIAN)
		#define BYTE_ORDER LITTLE_ENDIAN
//	#else
//		#error "Endian determination failed"
//	#endif
#endif /* BYTE_ORDER */

/* Define generic types used in lwIP */
typedef uint8_t    u8_t;
typedef int8_t     s8_t;
typedef uint16_t   u16_t;
typedef int16_t    s16_t;
typedef uint32_t   u32_t;
typedef int32_t    s32_t;

typedef unsigned long mem_ptr_t;

/* Define (sn)printf formatters for these lwIP types */
#define X8_F  "x"
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"

#define SZT_F "u"


/* Compiler hints for packing structures */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END


/* Plaform specific diagnostic output */



void rodos_lwip_error_print(const char* format, const char* msg,int line,const char* file);
#define LWIP_PLATFORM_DIAG(x)	rodos_lwip_error_print("%s\n",x,-1,0);
#define LWIP_PLATFORM_ASSERT(x) rodos_lwip_error_print("Assertion \"%s\" failed at line %d in %s\n",x,__LINE__,__FILE__);


//#define LWIP_RAND() ((u32_t)rand())

#ifdef __cplusplus
}
#endif


#endif /* __ARCH_CC_H__ */
