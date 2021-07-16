
#pragma once

namespace RODOS {


/**
	Definition of Topic IDs for predefined RODOS services
	and user services

	services in RODOS: Topic Ids
	0    ...   99: RODOS internal
	100  ...  999: Input Output Services
	1000 ... 64K:  User Services: User defined Topic Ids
*/


/************ 0 ... 99: Rodos intern topics **************/

constexpr uint32_t TOPIC_ID_SUBSCRIBERS_REPORT  = 0;
constexpr uint32_t TOPIC_ID_DEF_GATEWAY         = 0;
constexpr uint32_t TOPIC_ID_NETWORK_REPORT      = 1;

constexpr uint32_t TOPIC_ID_TAKS_DISTRIBUTION	= 2;
constexpr uint32_t TOPIC_ID_MONITORING_MSG		= 3;
constexpr uint32_t TOPIC_ID_DEBUG_CMD_MSG		= 4;


/************ 100 ... 999:  Input / Output services ***/

constexpr uint32_t TOPICID_UART_CONF     = 110;
constexpr uint32_t TOPICID_UART_IN       = 111;
constexpr uint32_t TOPICID_UART_OUT      = 112;

constexpr uint32_t TOPIC_ID_PWM_CONF     = 120;
constexpr uint32_t TOPIC_ID_PWM_WIDTH    = 121;

constexpr uint32_t TOPIC_ID_ANALOG_CONF  = 123;
constexpr uint32_t TOPIC_ID_ANALOG_CHANS = 124;

/******************************************************/

constexpr uint32_t FIRST_USER_TOPIC_ID = 1000;

}  // namespace


