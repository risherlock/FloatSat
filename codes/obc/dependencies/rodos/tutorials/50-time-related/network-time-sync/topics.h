/** Definitions of the topics used in the time sync demo. */

#pragma once

struct NtspTimes {
    int64_t sendTS; ///< request and/or response delivery time
    int64_t rcvTS;  ///< request and/or response arrival time
};

struct TimeSyncResponse {
    int32_t   clientNodeNr;
    int32_t   clientRequestCnt;
    NtspTimes ntspTimes;
};


extern Topic<TimeSyncResponse> serverResponse;
extern Topic<int32_t>          clientRequest;

inline double utcMillisecsNow() { return (double)sysTime.getUTC() / (double)MILLISECONDS; }
inline double toMillis(int64_t time) { return (double)time / (double)MILLISECONDS; }
