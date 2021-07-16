
#pragma once
#include "stdint.h"



/*** just Invented fields! **/

namespace Common {

//____________________________________________________________________________
struct TranferFrameHeader {

    static const int HEADER_SIZE = 7;
    static const int MAX_TF_SITE = 590;

    uint32_t sync;       ///< const = 0x1ACFFC1D
    uint16_t satId; 
    uint8_t  virtualChan; 

    int serialize(char * const) const; 
    int deserialize(char const * const);
}; // NOT required any more:  __attribute__ ((packed));


struct TranferFrameTrailer {
    uint32_t acks;
    uint16_t crc;  

    int serialize(char * const) const; 
    int deserialize(char const * const);
}; // NOT required any more:  __attribute__ ((packed));



//____________________________________________________________________________
struct SourcePacketHeader {

    static const int HEADER_SIZE = 16;

    uint8_t service;
    uint8_t subservice;
    uint16_t topic; 

    int serialize(char * const) const; 
    int deserialize(char const * const);

}; // NOT required any more:  __attribute__ ((packed));


} // namespace Common


