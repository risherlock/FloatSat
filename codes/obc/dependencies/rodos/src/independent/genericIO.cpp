/*
 * Copyright (c) 2012 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Sergio Montenegro, Michael Ruffer, Erik Dilger
 */

#include "hal/genericIO.h"
#include "misc-rodos-funcs.h"

namespace RODOS {


GenericIOInterface::GenericIOInterface() {
    ioEventReceiver=0;
    callerReadFinished = 0;
    callerWriteFinished = 0;
    callerDataReady = 0;
} // create all drivers static but do not init there


void GenericIOInterface::suspendUntilReadFinished(int64_t reactivationTime) {
    hwDisableInterrupts();
    if (isReadFinished()) {
        hwEnableInterrupts();
        return;
    }
    callerReadFinished =  Thread::getCurrentThread();
    callerReadFinished->suspendedUntil = reactivationTime;
    hwEnableInterrupts();
    Thread::yield();
}


void GenericIOInterface::suspendUntilWriteFinished(int64_t reactivationTime) {
    hwDisableInterrupts();
    if (isWriteFinished()) {
        hwEnableInterrupts();
        return;
    }
    callerWriteFinished =  Thread::getCurrentThread();
    callerWriteFinished->suspendedUntil = reactivationTime;
    hwEnableInterrupts();
    Thread::yield();
}


void GenericIOInterface::suspendUntilDataReady(int64_t reactivationTime) {
    hwDisableInterrupts();
    if (isDataReady()) {
        hwEnableInterrupts();
        return;
    }
    callerDataReady =  Thread::getCurrentThread();
    callerDataReady->suspendedUntil = reactivationTime;
    hwEnableInterrupts();
    Thread::yield();
}


void GenericIOInterface::upCallWriteFinished() { // last transmit order is concluded
    if (callerWriteFinished) callerWriteFinished->resume();
    callerWriteFinished = 0;
    if(ioEventReceiver) ioEventReceiver->onWriteFinished();
}


void GenericIOInterface::upCallReadFinished() { // last receive order is concluded
    if (callerReadFinished) callerReadFinished->resume();
    callerReadFinished = 0;
    if(ioEventReceiver) ioEventReceiver->onReadFinished();
}


void GenericIOInterface::upCallDataReady() {  // data has arrived
    if (callerDataReady) callerDataReady->resume();
    callerDataReady = 0;
    if(ioEventReceiver) ioEventReceiver->onDataReady();
}


void GenericIOInterface::setIoEventReceiver(IOEventReceiver* receiver){
	ioEventReceiver=receiver;
}


}
