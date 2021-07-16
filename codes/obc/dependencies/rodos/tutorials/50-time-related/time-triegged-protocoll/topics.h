/** Definitions of the topics used in the time sync demo. */

#pragma once

const int32_t NODE_COUNT = 4; ///< Number of nodes in the network

/************** time Sync ***********/

struct NtspTimes {
    int64_t sendTS; ///< request and/or response delivery time
    int64_t rcvTS;  ///< request and/or response arrival time
};

struct TimeSyncResponse {
    int32_t   clientNodeNr;
    int32_t   clientRequestCnt;
    NtspTimes ntspTimes;
    int32_t   nodeIdAssignedByServer;
};


extern Topic<TimeSyncResponse> serverResponse;
extern Topic<int32_t>          clientRequest;

/*** TTP Messages ******************/

struct TTPMessage {
    int32_t nodeId;
    int32_t message;
};

extern Topic<TTPMessage> ttpMessage; ///< TTP message topic

/*************** Help Function **********/

inline double utcMillisecsNow() { return (double)sysTime.getUTC() / (double)MILLISECONDS; }
inline double toMillis(const int64_t& time) { return (double)time / (double)MILLISECONDS; }
inline double toSeconds(const int64_t& time) { return (double)time / (double)SECONDS; }
