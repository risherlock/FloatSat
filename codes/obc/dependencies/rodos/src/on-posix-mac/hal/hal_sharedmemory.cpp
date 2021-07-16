/*
 * Copyright (c) 2013 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Johannes Freitag
 */

#include "rodos.h"
#include "hal/hal_sharedmemory.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <new>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/semaphore.h>

#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#define MAX_NUM_SHM 5
#define MAGIC_WORD 1235

namespace RODOS {

void sharedmemorySignalHandler(int signum);

typedef struct {
    int32_t magicWord;
    int32_t lockFlag;
    int32_t size;
    int32_t maxMembers;
    int32_t membersCount;
    int32_t procids;
} shmHeader_t;

class HW_HAL_Sharedmemory {
  public:
    HW_HAL_Sharedmemory();
    Sharedmemory_IDX  index;
    shmHeader_t*      header;
    int32_t           iLockedIt;
    void*             userMemoryStart;
    int32_t           memberId;
    sem_t*            semaphore;
    HAL_Sharedmemory* hal_shm;
    bool              dataReady;
};

HW_HAL_Sharedmemory::HW_HAL_Sharedmemory() {
    memberId  = -1;
    dataReady = false;
}

HW_HAL_Sharedmemory sharedMemoryContext[MAX_NUM_SHM];

HAL_Sharedmemory::HAL_Sharedmemory(Sharedmemory_IDX shmIdx) {
    context          = new(&sharedMemoryContext[shmIdx]) HW_HAL_Sharedmemory(); // placement new to avoid dynamic memory allocation
    context->index   = shmIdx;
    context->hal_shm = this;
}

void sharedmemorySignalHandler(int signum) {
    for(int i = 0; i < MAX_NUM_SHM; i++) {
        HAL_Sharedmemory* shm = sharedMemoryContext[i].hal_shm;
        if(shm) {
            shm->upCallDataReady();
            sharedMemoryContext[i].dataReady = true;
        }
    }
}

void* HAL_Sharedmemory::init(int32_t* size, int32_t* maxMembers) {

    int32_t  shmid;
    intptr_t shm;
    int32_t  totalSize = *size + 5 * 4 + *maxMembers * 4;
    key_t    shmkey    = context->index * 1000 + 1001;

    // Create the segment.
    shmid = shmget(shmkey, totalSize, IPC_CREAT | 0666);
    if(shmid < 0) {
        RODOS_ASSERT_IFNOT_RETURN(errno == EINVAL, 0); //shm too big
        shmid = shmget(shmkey, 100, IPC_CREAT | 0666);
        RODOS_ASSERT_IFNOT_RETURN(shmid >= 0, 0); // could not create shared memory
    }

    // Get pointer to segment
    shm = reinterpret_cast<intptr_t>(shmat(shmid, NULL, 0));
    RODOS_ASSERT_IFNOT_RETURN(((int)shm != -1), 0); // sharedMemory: could not open shared memory

    context->header = (shmHeader_t*)shm;

    //create posix system semaphore
    char semname[]     = "RodosSHM0";
    semname[8]         = (char)(((int32_t)'0') + context->index);
    context->semaphore = sem_open(semname, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    if(context->semaphore == SEM_FAILED) {
        RODOS_ASSERT_IFNOT_RETURN(errno == EEXIST, 0);  //semaphore exists already
        context->semaphore = sem_open(semname, 0);
    }

    signal(SIGUSR1, sharedmemorySignalHandler);
    int32_t procID = getpid();

    //***********************
    // critical section start
    //***********************
    sem_wait(context->semaphore); //open semaphore

    //if not initialised yet -> magic word
    // sharedMemory already initilized?
    if(context->header->magicWord == MAGIC_WORD) {
        *maxMembers = context->header->maxMembers; // max num of members
        if(context->header->membersCount >= context->header->maxMembers) {
            sem_post(context->semaphore); //close semaphore
            RODOS_ERROR("sharedMemory: to many members");
            return 0; //error number of rodos instances == maxMembers
        }
        context->header->membersCount++; // number of rodos instances
        *size = context->header->size;   // size of shm
        for(int i = 0; i < context->header->maxMembers; ++i) {
            if(*(&context->header->procids + i) == 0) {
                context->memberId = i;
                break;
            }
        }
    } else {
        context->header->magicWord    = MAGIC_WORD;  // set lockflag
        context->header->lockFlag     = 0;           //lockflag
        context->header->size         = *size;       // size of shm
        context->header->maxMembers   = *maxMembers; // max num of members
        context->header->membersCount = 1;           // number of rodos instances
        context->memberId             = 0;
        for(int i = 0; i < context->header->maxMembers; ++i) {
            *(&context->header->procids + i) = 0;
        }
        *(&context->header->procids + context->header->maxMembers) = 0; //initialize first entry in userspace
    }
    *(&context->header->procids + context->memberId) = procID; // set procid

    sem_post(context->semaphore); //close semaphore

    //***********************
    // critical section end
    //***********************

    context->userMemoryStart = &context->header->procids + context->header->maxMembers; //start+header size

    return context->userMemoryStart;
}

int32_t HAL_Sharedmemory::lock() {

    sem_wait(context->semaphore); //open semaphore
    //read lockflag
    if(islocked()) {
        sem_post(context->semaphore); //close semaphore
        return false;
    } else {
        context->header->lockFlag = 1;
        context->iLockedIt        = 1;
        sem_post(context->semaphore); //close semaphore
        return true;
    }
}

int32_t HAL_Sharedmemory::islocked() {
    return context->header->lockFlag;
}

void HAL_Sharedmemory::unlock() {
    if(context->iLockedIt) {
        context->header->lockFlag = 0;
        context->iLockedIt        = 0;
    }
}

void HAL_Sharedmemory::raiseSharedMemoryChanged() {

    pid_t pid;
    for(int i = 0; i < context->header->membersCount; ++i) {
        if(i != context->memberId) {
            pid = *(&context->header->procids + i);
            if(pid != 0) {
                if(kill(pid, SIGUSR1)) { //remove dead members from list
                    if(lock() == false) {
                        if(context->iLockedIt) {
                            *(&context->header->procids + i) = 0;
                            context->header->membersCount--;
                        }
                    } else {
                        *(&context->header->procids + i) = 0;
                        context->header->membersCount--;
                        unlock();
                    }
                }
            }
        }
    }
}

int32_t HAL_Sharedmemory::getMemberId() {
    return context->memberId;
}

bool HAL_Sharedmemory::isWriteFinished() {
    return true; //Method does not make sense for HAL_Sharedmemory
}

bool HAL_Sharedmemory::isDataReady() {
    return context->dataReady;
}

void HAL_Sharedmemory::resetDataReady() {
    context->dataReady = false;
}

} // namespace RODOS
