/*
 * Copyright (c) 2012 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Sergio Montenegro, Michael Ruffer, Erik Dilger
 */

#include "hal/genericIO.h"
//#include "misc.h"

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


GenericIOInterface::GenericIOInterface() {
    ioEventReceiver=0;

} // create all drivers static but do not init there


void GenericIOInterface::suspendUntilReadFinished(int64_t reactivationTime) {

}


void GenericIOInterface::suspendUntilWriteFinished(int64_t reactivationTime) {
	while(!isWriteFinished()){ }

}


void GenericIOInterface::suspendUntilDataReady(int64_t reactivationTime) {

}


void GenericIOInterface::upCallWriteFinished() { // last transmit order is concluded

    if(ioEventReceiver) ioEventReceiver->onWriteFinished();
}


void GenericIOInterface::upCallReadFinished() { // last receive order is concluded

    if(ioEventReceiver) ioEventReceiver->onReadFinished();
}


void GenericIOInterface::upCallDataReady() {  // data has arrived

    if(ioEventReceiver) ioEventReceiver->onDataReady();
}


void GenericIOInterface::setIoEventReceiver(IOEventReceiver* receiver){
	ioEventReceiver=receiver;
}


#ifndef NO_RODOS_NAMESPACE
}
#endif
