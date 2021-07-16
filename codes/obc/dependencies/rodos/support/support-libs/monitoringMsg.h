
#pragma once

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/**************************************************/

/** MessageTypeId end EOM is required just to be able
 	to signalize the End Of Mesage ussing the operator <<
	else they have no use.
 **/

struct MessageTypeId    { uint16_t id; };
extern MessageTypeId EOM(uint16_t id);


/*** Montor messages: used for both:
	interchange  monitored data (from application to monitor/debugger)
	interchagne  debug messages (from monitor/debugger to applications)
 **/

class MonitoringMsg {
public:
    int64_t sendTime;	///< Set when the message is sent

    uint16_t msgType;	///< to identify differnt types of monitor messages, defined by user

    uint8_t nodeNr; 		///< node of the sending task (monitor msgs) or target task (command msgs)
    uint8_t taskId;
    uint8_t numOfParamsToSend; 	   ///< not all will be send to safe band width and cpu time
    uint8_t numOfParamsDeserialized; ///< only a help variable for the reciever side

    static const uint8_t MAX_PARAMS = 64u;
    char params[MAX_PARAMS];
    /*** WARNING: Do not add any data field beyond this point, Params has to be the last one **/

    size_t numOfBytesToSend() {
        uintptr_t    start_of_message = reinterpret_cast<uintptr_t>(this);
        uintptr_t start_of_params  = reinterpret_cast<uintptr_t>(params);
        return (size_t)(start_of_params - start_of_message) + numOfParamsToSend;
    } 

    /**************************************************************************************/

    MonitoringMsg(uint8_t taskId);

    void clear();

    /***********************************/
    /** For the monitored/sender side **/
    /***********************************/

    bool serialize(void* src, size_t len);	///< append data to the message
    void report(uint16_t id);		///< distribute the message

    // These functions could implement byte-sex (littel/big-endian) convertions

    inline bool serialize(char src) 		{ return serialize(&src, sizeof(src)); }
    inline bool serialize(short src) 		{ return serialize(&src, sizeof(src)); }
    inline bool serialize(int src) 		{ return serialize(&src, sizeof(src)); }
    inline bool serialize(long src) 		{ return serialize(&src, sizeof(src)); }
    inline bool serialize(long long src)	{ return serialize(&src, sizeof(src)); }
    inline bool serialize(float src) 		{ return serialize(&src, sizeof(src)); }
    inline bool serialize(double src) 		{ return serialize(&src, sizeof(src)); }

    inline bool serializeString(char* s)	{
        return serialize(s, strlen(s) +1u);    // +1: include the termnating 0
    }


    /*******************************************/
    /** For the monitor/receiver side   ********/
    /*******************************************/

    bool deserialize(void* dest, size_t len);
    void* deserializeNoCopy(size_t len);

    // could implement byte-sex (littel/big-endian) convertions

    inline bool deserialize(char *a)		{ return deserialize(a, sizeof(*a)); }
    inline bool deserialize(short *a)		{ return deserialize(a, sizeof(*a)); }
    inline bool deserialize(int *a)		{ return deserialize(a, sizeof(*a)); }
    inline bool deserialize(long *a)		{ return deserialize(a, sizeof(*a)); }
    inline bool deserialize(long long *a)	{ return deserialize(a, sizeof(*a)); }
    inline bool deserialize(float *a)		{ return deserialize(a, sizeof(*a)); }
    inline bool deserialize(double *a)		{ return deserialize(a, sizeof(*a)); }

    inline bool deserializeString(char* s)	{
        return deserialize(s, strlen(&params[numOfParamsDeserialized]) +1u);    // +1: include the termnating 0
    }

    inline char* deserializeString()	{
        return (char*)deserializeNoCopy(strlen(&params[numOfParamsDeserialized])+1u);    // +1: include the termnating 0
    }


    /***********************************************************/
    /***     SHORT CUTS : the same can be done by calling     **/
    /***     serialize and deserialize methods directly       **/
    /***********************************************************/


    inline char deserializeChar() 		{ char x;      deserialize(&x, sizeof(x)); return x; }
    inline short deserializeShort() 		{ short x;     deserialize(&x, sizeof(x)); return x; }
    inline int 	deserializeInt() 		{ int x;       deserialize(&x, sizeof(x)); return x; }
    inline long deserializeLong()		{ long x;      deserialize(&x, sizeof(x)); return x; }
    inline long long deserializeLonglong() 	{ long long x; deserialize(&x, sizeof(x)); return x; }
    inline float deserializeFloat() 		{ float x;     deserialize(&x, sizeof(x)); return x; }
    inline double deserializeDouble() 		{ double x;    deserialize(&x, sizeof(x)); return x; }

    /** to send, use for example
    	monitoring << counter1 << "ist working" << 3.141592 << EOM(MSG_ID);
    **/

    inline MonitoringMsg& operator<<(char a)      { serialize(a); return *this; }
    inline MonitoringMsg& operator<<(short a)     { serialize(a); return *this; }
    inline MonitoringMsg& operator<<(int a)       { serialize(a); return *this; }
    inline MonitoringMsg& operator<<(long a)      { serialize(a); return *this; }
    inline MonitoringMsg& operator<<(long long a) { serialize(a); return *this; }
    inline MonitoringMsg& operator<<(float a)     { serialize(a); return *this; }
    inline MonitoringMsg& operator<<(double a)    { serialize(a); return *this; }

    inline MonitoringMsg& operator<<(char* a)     { serializeString(a); return *this; }

    /// Only for End Of Message
    inline void  operator<<(MessageTypeId mtid)    { report(mtid.id); }

    /*********************************************/

    /** use for example
    	reportMessage >> int_counter >> string_array >> double_val;
    **/

    inline MonitoringMsg& operator>>(char &a)      { deserialize(&a); return *this; }
    inline MonitoringMsg& operator>>(short &a)     { deserialize(&a); return *this; }
    inline MonitoringMsg& operator>>(int &a)       { deserialize(&a); return *this; }
    inline MonitoringMsg& operator>>(long &a)      { deserialize(&a); return *this; }
    inline MonitoringMsg& operator>>(long long &a) { deserialize(&a); return *this; }
    inline MonitoringMsg& operator>>(float &a)     { deserialize(&a); return *this; }
    inline MonitoringMsg& operator>>(double &a)    { deserialize(&a); return *this; }

    // inline MonitoringMsg& operator>>(char* a)      { a = deserializeString(); return *this; } ERROR?: has no effect to *a ??

};


extern Topic<MonitoringMsg> monitorMsgTopic;
extern Topic<MonitoringMsg>  dbgCommandTopic;


#ifndef NO_RODOS_NAMESPACE
}
#endif

