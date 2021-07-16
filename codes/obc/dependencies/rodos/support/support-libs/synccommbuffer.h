#pragma once

#include "commbuffer.h"
#include "thread.h"

namespace RODOS {

/*
 * @class SyncCommBuffer
 * @brief Commbuffer with blocking reads
 *
 * Like Commbuffer, but it provides a blocking syncGet function
 *
 * @param Type Type of the data to synchonize by CommBuffer
 */
template <class Type> class SyncCommBuffer : public CommBuffer<Type> {
protected:
  Thread* suspendedReader;

public:
  using CommBuffer<Type>::get;
  using CommBuffer<Type>::getOnlyIfNewData;

  SyncCommBuffer() : CommBuffer<Type>() {
    suspendedReader = 0;
  }

  bool putGeneric([[gnu::unused]] const uint32_t topicId, const size_t len, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
    RODOS_ASSERT_IFNOT_RETURN((len <= sizeof(Type)), false); // SyncCommBuffer wrong len
    this->put(*(const Type*)msg);
    return true;
  }

  /**
   * Same as Commbuffer put, but also resumes any currently suspended reader.
   * @param data Reference of the message data to put.
   */
  void put(const Type& data) {
    CommBuffer<Type>::put(data);
    {
      PRIORITY_CEILER_IN_SCOPE();
      if(suspendedReader) {
        suspendedReader->resume();
      }
    }
  }

  /*
   * Same as CommBuffer put, but resumes suspended reader
   */
  void putZero() {
    CommBuffer<Type>::putZero();
    {
      PRIORITY_CEILER_IN_SCOPE();
      if(suspendedReader) {
        suspendedReader->resume();
      }
    }
  }

  /**
   * Wait until new data is available or timeout has passed
   * then do a normal CommBuffer get
   * @return true if there was new data, false if not
   * @param data Reference of the message data to get.
   */
  bool syncGet(Type& data, const int64_t timeout) {
    {
      PRIORITY_CEILER_IN_SCOPE();
      if (!this->newDataAvailable) {
        suspendedReader = Thread::getCurrentThread();
        switch (timeout) {
        case 0:
          break;
        case END_OF_TIME:
          Thread::suspendCallerUntil(END_OF_TIME);
          break;
        default:
          Thread::suspendCallerUntil(NOW() + timeout);
        }
        suspendedReader = 0;
      }
    }
    bool ok = this->newDataAvailable;
    this->get(data);
    return ok;
  }

  /**
   * clears the data state
   * used to make sure that the next syncGet returns newer data than when this function is called
   */
  void clearDataState(){
    this->newDataAvailable=false;
  }
};


} // namespace

