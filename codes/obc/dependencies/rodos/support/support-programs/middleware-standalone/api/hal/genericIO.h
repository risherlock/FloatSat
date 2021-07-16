/*
 * Copyright (c) 2012 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Sergio Montenegro, Michael Ruffer, Erik Dilger
 */

#ifndef GENERICIO_H_
#define GENERICIO_H_

#include <stdint.h>


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

class IOEventReceiver {
public:
	virtual void onWriteFinished() {}
    virtual void onReadFinished() {}
	virtual void onDataReady() {}
};

class GenericIOInterface {
private:

protected:
    IOEventReceiver* ioEventReceiver;
	void* context;

public:
    GenericIOInterface(); // create all drivers static but do not init there

    /**** Implemented by each interface ****/
    virtual int init(...)   {return 0;}  ///< Virtual, but do not call using a pointer, each init differs
    virtual void reset() { }
    virtual int config(...) {return 0;}  ///< Virtual, but do not call using a pointer, each config differs
    virtual int status(...) {return 0;}  ///< Virtual, but do not call using a pointer, each status differs

    virtual bool isWriteFinished() = 0;
    virtual bool isReadFinished()   {return false;}
    virtual bool isDataReady()   {return false;}

    virtual int write(...)      {return 0;}
    virtual int read(...)       {return 0;}
    virtual int writeRead(...)  {return 0;}

    void suspendUntilReadFinished(int64_t reactivationTime = 0);
    void suspendUntilWriteFinished(int64_t reactivationTime = 0);
    void suspendUntilDataReady(int64_t reactivationTime = 0);

    /******* may be overridden by the user or received through IOEventREceiver, called by corrsponding driver  *****/
    virtual void upCallWriteFinished(); // last transmit order is concluded
    virtual void upCallReadFinished();  // last receive order is concluded
    virtual void upCallDataReady();     // data has arrived

    void setIoEventReceiver(IOEventReceiver* receiver);
};


#ifndef NO_RODOS_NAMESPACE
}
#endif

#endif /* GENERICIO_H_ */
