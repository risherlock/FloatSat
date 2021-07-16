/**
* @file hw_datatypes.h
* @date 2008/06/19 8:46
* @author Lutz Dittrich
*
*
* @brief typedefs for hw specific data types
*
*/

#ifndef __HW_DATATYPES_H__
#define __HW_DATATYPES_H__


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


typedef unsigned int __type_uint32__;
typedef unsigned short __type_uint16__;

/** typedefs for fixed width data types */

typedef __type_uint16__ TUDPPortNr;
//typedef __type_uint32__ size_t;


#ifndef NO_RODOS_NAMESPACE
}
#endif

#endif /* __HW_DATATYPES_H__ */
