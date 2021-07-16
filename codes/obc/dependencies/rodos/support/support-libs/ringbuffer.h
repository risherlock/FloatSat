
#pragma once


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/**
     RingBuffer administrates a pool of objects in a ring manner (Template class).

     with getWriteBuffer/endWrite you can rewrite the oldest entry in the buffer.
     with getfromRing you get cyckick one for one, all written entries
     in the buffer.

     Thread-safe for one writer and one reader.

     @param      t Class to be adrministrated (Template)
     @param      poolSize how many elements are to be allocated

     @author     Sergio Montenegro
     @version    Sept. 1999, March 2000, Feb 2009


*/

template <class T, uint32_t poolSize>
class RingBuffer {
public:
    uint32_t   writeIndex = 0;
    uint32_t   readIndex = 0;
    uint32_t   currentWrite = UINT32_MAX;
    T          vals[poolSize];

public:
    /// How often it was written
    uint64_t writeCnt = 0;
    /// How often it was readed
    uint64_t readCnt = 0;
    /// How many objects are in FIFO ( == writeCnt - readCnt)
    uint32_t occupiedCnt = 0;

    T* getNextEntryToPut() { return &vals[writeIndex]; }

    void put(const T& newdata) {
        currentWrite = writeIndex;
        vals[writeIndex] = newdata;
        writeIndex++;
        if (writeIndex  >= poolSize) writeIndex = 0;
        if (occupiedCnt <  poolSize) occupiedCnt++;
        writeCnt++;
        currentWrite = UINT32_MAX;
    }

    /// get the next
    void get(T& fromRing) {

        /** Jump the current being written record **/
        if (readIndex == currentWrite) readIndex++;
        if (readIndex >= poolSize) readIndex = 0;

        /** readIndex should not pass write index if there is no data after it */
        if (writeCnt < poolSize && readIndex >= writeIndex) readIndex = 0;

        fromRing = vals[readIndex];
        readIndex++;
        if (readIndex >= poolSize) readIndex = 0;

        if (occupiedCnt > 0) occupiedCnt--;
        readCnt++;
    }

    ///
    int getLen() { return poolSize; }

};



#ifndef NO_RODOS_NAMESPACE
}
#endif

