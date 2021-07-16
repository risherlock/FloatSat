
#include "rodos.h"
#include "topics.h"

Topic<TimeSyncResponse> serverResponse(-1, "serverResponse");
Topic<int32_t>          clientRequest(-1, "clientRequest");
