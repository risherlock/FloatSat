/*
 * Copyright (c) 2013 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Johannes Freitag
 */
#pragma once
#include <stdint.h>


#include "genericIO.h"
#include "misc-rodos-funcs.h"
#include "thread.h"


namespace RODOS {

enum Sharedmemory_IDX {
    Sharedmemory_IDX0 = 0,
    Sharedmemory_IDX1,
    Sharedmemory_IDX2,
    Sharedmemory_IDX3,
    Sharedmemory_IDX4
};

class HW_HAL_Sharedmemory;

class HAL_Sharedmemory : public GenericIOInterface {
  private:
    HW_HAL_Sharedmemory* context;

  public:
    HAL_Sharedmemory(Sharedmemory_IDX shmIdx);

    /**
     * Initialize shared memory
     * @retval void * pointer to shared memory
     */
    void* init(size_t* size, int32_t* maxMembers);

    /**
     * Lock shared memory
     * @retval int32_t true if locking works false if already locked(by another member)
     */
    int32_t lock();

    /**
     * @retval true if locked, false if not locked
     */
    int32_t islocked();

    void ensureLock() {
        while(!lock()) {
            if(isSchedulerRunning()) {
                Thread::yield();
            }
        }
    }

    /**
     * Unlock if locked by this instance
     */
    void unlock();

    /**
     * @retval ID of this shared memory member
     */
    int32_t getMemberId();

    /**
     * Inform other shared memory members that something has changed
     */
    void raiseSharedMemoryChanged();

    bool isWriteFinished();

    /**
     * @retval true if new data is available
     */
    bool isDataReady();

    /**
     * reset the data ready flag
     */
    void resetDataReady();
};

} // namespace RODOS
