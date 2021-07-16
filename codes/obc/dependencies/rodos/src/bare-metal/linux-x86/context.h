


/**
* @file context.h
* @date 2008/04/22 16:54
* @author Lutz Dittrich
*
*
* @brief memory layout for context saving
*
* description of memory layout for context saving
* for x86 32-bit architecture 
*
*/

#pragma once

namespace RODOS {
/**
* @struct s_context
* @brief memory layout for context saving
*
* @internal
*
* This struct describes the memory layout used for context saving.
*  See INTEL IA32 EMT64 instruction set reference.
*/
typedef struct s_context {
    /*@{ The context of the thread. */
    long floatingPointContext[108 / 4];
    long EFLAGS;
    long EDI;
    long ESI;
    long EBP;
    long ESP;
    long EBX;
    long EDX;
    long ECX;
    long EAX;
    long EIP;
    /*@}*/
} TContext;
} // namespace RODOS
