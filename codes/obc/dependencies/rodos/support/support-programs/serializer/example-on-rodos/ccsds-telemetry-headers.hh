
#pragma once
#include "stdint.h"


/** This is the typical declaration of CCSDS Headers for Telemetry **/


namespace Common {

//____________________________________________________________________________
struct TranferFrameHeader {

    static const int HEADER_SIZE = 10;
    static const int MAX_TF_SIZE = 590;


    uint32_t sync;       ///< const = 0x1ACFFC1D
    uint16_t verId_frid; ///< for : 0x20A  OR ((VCNr & 0x07) << 1) TODO: Get TET ID
    uint8_t  mcfc;       ///< Master-Channel-Frame-Count (set by Hardware while sending)
    uint8_t  vcfc;	 ///< Vitual-Channel-Frame-Count (set by  Software before sending)
    uint16_t fdfs;	 ///< First-Header-Pointer (const = 0x1800)

    //___________________________________________________________ For Impelemtation ask S. Montenegro
    //TranferFrameHeader();
    //void setVcid(uint16_t val); 
    //uint16_t getVcid();
    //___________________________________________________________

    int serialize(char * const) const; 
    int deserialize(char const * const);
}; // NOT required any more:  __attribute__ ((packed));


struct TranferFrameTrailer {
    uint32_t clcw;      ///< Uplink-Acks (set by Hardware)
    uint16_t crc;       ///< cyclic redundancy code (as a checksum, set by Hardware)

    int serialize(char * const) const; 
    int deserialize(char const * const);
}; // NOT required any more:  __attribute__ ((packed));



//____________________________________________________________________________
struct SourcePacketHeader {

    static const int HEADER_SIZE = 16;

    /// Primary Source-Packet-Header
    uint16_t apid_rest;  ///< 0x0800 OR (APID & 0x07FF)
    uint16_t gf_ssc;     ///< 0xC000 OR (SSC  & 0x3FFF)
    uint16_t pdl;        ///< for HKDs (Service=3): 490 - 1 sourcepaketlen = pdl + 7

    /// Secondary Source-Packet-Header (PUS-Header)
    uint8_t version;         ///< const = 0x10
    uint8_t service;	     ///< Service/SubService: 1/1..8  = Acknowledge, 3/23  = Commanded HKD,
    uint8_t subservice;      ///<                     3/25 = Automatic HKD, 163/128 = paload Data, 164/128 = Memory-Dump
    uint8_t destination;     ///< 0=Groundstation

    uint32_t timeStampSeconds;	///< UTC in seconds since 1.1.2000  0:00, set by Housekeeper
    uint16_t timeStampFraction;	///< (1/2^16) of the second

    //___________________________________________________________ For Impelemtation ask S. Montenegro
    //SourcePacketHeader();
    //void initHeader(short apid); ///< inits constant values
    //void setUTC();
    //void setSsc(uint16_t  val); 
    //void setApid(uint16_t val); 
    //uint16_t getApid();
    //uint16_t getSsc();
    //uint8_t  getVcfc();
    //___________________________________________________________

    int serialize(char * const) const; 
    int deserialize(char const * const);

}; // NOT required any more:  __attribute__ ((packed));


} // namespace Common


