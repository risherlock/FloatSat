
#pragma once

namespace RODOS {


/**
	Definition of APIDS: Application Ids  for predefined RODOS 
	and user services

	appliation in RODOS: Ids
	0    ...   99: RODOS internal
	100  ...  999: Input Output application
	1000 ... 2G:  User applications
*/


/************ 0 ... 99: Rodos intern topics **************/

constexpr int32_t APID_ANONYM     = 0;
constexpr int32_t APID_MIDDLEWARE = 10;


/************ 100 ... 999:  Input / Output Sercies ***/

constexpr int32_t APID_GATEWAY_GENERIC = 20;
constexpr int32_t APID_GATEWAY_UDP     = 21;
constexpr int32_t APID_GATEWAY_UART    = 22;


/******************************************************/

constexpr int32_t FIRST_USER_APID = 100;


}  // namespace
