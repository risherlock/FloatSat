/*
 * Copyright (c) 2012 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Sergio Montenegro, Michael Ruffer, Erik Dilger
 *
 * This is only the example of every thing an IO interface shall implement
 */

#pragma once

#include <stdint.h>
#include "thread.h"

namespace RODOS {

class IOEventReceiver {
public:
    virtual void onWriteFinished() {}
    virtual void onReadFinished() {}
    virtual void onDataReady() {}
    virtual ~IOEventReceiver() = default;
};

class GenericIOInterface {
private:
    Thread* callerReadFinished;
    Thread* callerWriteFinished;
    Thread* callerDataReady;

protected:
    IOEventReceiver* ioEventReceiver;
	void* context;

public:
    GenericIOInterface(); // create all drivers static but do not init there
    virtual ~GenericIOInterface() = default;


    // Commented out in the generic interface, but shall be implemented in the real interface

#if 0
    /**** Implemented by each interface ****/
    virtual int init(...)   {return 0;}  ///< Virtual, but do not call using a pointer, each init differs
    virtual void reset() { }
    virtual int config(...) {return 0;}  ///< Virtual, but do not call using a pointer, each config differs
    virtual int status(...) {return 0;}  ///< Virtual, but do not call using a pointer, each status differs

    virtual int write(...)      {return 0;}
    virtual int read(...)       {return 0;}
    virtual int writeRead(...)  {return 0;}
#endif
    
    virtual bool isWriteFinished() = 0;
    virtual bool isReadFinished()   {return false;}
    virtual bool isDataReady()   {return false;}

    void suspendUntilReadFinished(int64_t reactivationTime = END_OF_TIME);
    void suspendUntilWriteFinished(int64_t reactivationTime = END_OF_TIME);
    void suspendUntilDataReady(int64_t reactivationTime = END_OF_TIME);

    /******* may be overridden by the user or received through IOEventREceiver, called by corrsponding driver  *****/
    virtual void upCallWriteFinished(); // last transmit order is concluded
    virtual void upCallReadFinished();  // last receive order is concluded
    virtual void upCallDataReady();     // data has arrived

    void setIoEventReceiver(IOEventReceiver* receiver);
};

}

