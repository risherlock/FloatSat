

/**
* @file fifo.h
* @date 2008/05/11 16:13
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief FIFO (first in first out) communication (header)
*
*/

#pragma once

#include "putter.h"
#include "rodos-debug.h"
#include "thread.h"

namespace RODOS {

/**
*  @class Fifo
*  @brief simple FIFO (realized as ring buffer)
*
*  A fifo where both sides (reader & writer) can work asynchronously.
*  If one side has more than one owner, the fifo has to be
*  protected using priority_ceiling, or semaphores.
*
*  @tparam Type    data type of fifo entries
*  @tparam len     maximal number of entries must be at least 2 (real capacity = len-1)
*
*/
template < typename Type, size_t len > class Fifo : public Putter {

protected:

    Type buffer[len];   ///< Array containing the elements of the fifo.

    volatile size_t writeX; ///< index of where to write
    volatile size_t readX;  ///< index of where to read

    /** advance index to next position
    * with overflow to 0 to implement a ring
    */
    size_t advanceIndex(size_t index) {
        return ((index < (len-1)) ? (index+1) : 0);
    }


public:

    Fifo() {
        readX = 0;
        writeX = 0;
    }

    uint32_t putErrors = 0; ///< counter of writing errors
    uint32_t getErrors = 0; ///< counter of reading errors

    /** implements the generic interface of putter */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        RODOS_ASSERT_IFNOT_RETURN(msgLen <= sizeof(Type), false);
        return put(*(const Type*)msg);
    }

    /**
     * Puts data into the fifo
     * @param[in] val The value that should be written into the fifo.
     * @return false if the fifo is full, true otherwise
     */
    bool put(const Type& val) {
        size_t index =  advanceIndex(writeX);
        if(index == readX) {
            putErrors++;
            return false; /* full! */
        }

        buffer[writeX] = val;
        writeX = index;
        return true;
    }

    /**
     * Retrieve a value from the fifo.
     * @param[out] val A variable reference whose value is overwritten with the value from the fifo.
     * @return false if fifo is empty (val is untouched), true otherwise.
     */
    bool get(Type& val) {
        if(readX == writeX) {
            getErrors++;
            return false;
        }
        val = buffer[readX];
        readX = advanceIndex(readX);
        return true;
    }

    /**
     * Returns the capacity of the fifo.
     * @return The capacity of the fifo.
     */
    size_t getLen() { return len; }

    /**
     * Returns the number of elements currently stored in the fifo.
     * @warning This method is not thread safe.
     * @return The number of elements currently stored in the fifo.
     */
    size_t getElementCount() {
        size_t r = readX;
        size_t w = writeX;
        return (r <= w) ? (w-r) : (len-r+w);
    }

    /**
     * Returns the number of unused elements.
     * @warning This method is not thread safe.
     * @return The number of unused elements.
     */
    size_t getFreeSpaceCount() {
        return len - getElementCount() - 1;
    }

    /**
     * Determines whether the fifo is full.
     * @warning This method is not thread safe.
     * @return true if the fifo is full, false otherwise.
     */
    bool isFull()  { return advanceIndex(writeX)==readX; }

    /**
     * Determines whether the fifo is empty.
     * @warning This method is not thread safe.
     * @return true if the fifo is empty, false otherwise.
     */
    bool isEmpty() { return readX == writeX;}

    void clear()   { readX = writeX = 0; }                 ///< erases all content
};

/**********************************************************************/

/**
* @class SyncFifo
* @brief A synchronized fifo.
*
* Like Fifo, but it provides syncPut which block if the fifo is
* full and syncRead which block if the fifo is empty.
* Each function may have a timeout. If no timeout is given, or timeout == 0
* it will wait until the fifo is not empty/full.
* Warning:
* Only for one writer and one reader. If more than that is required it has
* to be protected using semaphores.
*
*/
template <class Type, size_t len> class SyncFifo : public Fifo<Type, len> {

protected:
    Thread* suspendedReader;    ///< A pointer to the waiting reader thread.
    Thread* suspendedWriter;    ///< A pointer to the waiting writer thread.

public:

    SyncFifo() {
        suspendedReader = 0;
        suspendedWriter = 0;
    }


    /**
    * Implements the generic interface of putter:
    * Warning does not suspends if fifo full
    */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        RODOS_ASSERT_IFNOT_RETURN(msgLen <= sizeof(Type), false);

        bool ok = this->put(*(const Type*)msg);
        {
            PRIORITY_CEILER_IN_SCOPE();
            if (suspendedReader!=0)  suspendedReader->resume();
        }
        return ok;
    }

    /**
     * Puts data into the fifo
     * @param[in] val The value that should be written into the fifo.
     * @return false if the fifo is full, true otherwise
     */
    bool syncPut(const Type &val, const int64_t timeout = END_OF_TIME) {
        bool ok = false;
        {
            PRIORITY_CEILER_IN_SCOPE();
            ok = this->put(val);
            if (!ok) {
                suspendedWriter = 	Thread::getCurrentThread();
                switch (timeout) {
                case 0:
                    break;
                case END_OF_TIME:
                    Thread::suspendCallerUntil(END_OF_TIME);
                    break;
                default:
                    Thread::suspendCallerUntil(NOW() + timeout);
                }
                suspendedWriter = 0; // after suspend, after resume
            }
        }

        if (!ok && timeout > 0) { ok = this->put(val); }
        if (ok) {
            PRIORITY_CEILER_IN_SCOPE();
            if(suspendedReader) suspendedReader->resume();
        }
        return ok;
    }

    /**
     * Retrieve a value from the fifo.
     * @param[out] val A variable reference whose value is overwritten with the value from the fifo.
     * @return false if fifo is empty (val is untouched), true otherwise.
     */
    bool syncGet(Type &val, const int64_t timeout = END_OF_TIME) {
        bool ok = false;
        {
            PRIORITY_CEILER_IN_SCOPE();
            ok= this->get(val);
            if (!ok) {
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

        if (!ok && timeout > 0) { ok = this->get(val); }

        if(ok) {
            PRIORITY_CEILER_IN_SCOPE();
            if (suspendedWriter) suspendedWriter->resume();
        }
        return ok;
    }

};


template <typename Type, size_t len, uint32_t numOfreaders >
class MultipleReaderFifo : public Putter {

public:

    Type buffer[len];

    volatile size_t   writeX;
    volatile size_t   readX[numOfreaders];
    volatile uint32_t overflowCnt[numOfreaders];
    uint32_t readerCnt; ///< used only to generate readerId, if user wishes!

    /** advance index to next position
    * with overflow to 0 to implement a ring
    */
    size_t advanceIndex(size_t index) {
        return ((index < (len-1)) ? (index+1) : 0);
    }


    MultipleReaderFifo() {
        readerCnt = 0;
        writeX = 0;
        for(uint32_t i = 0; i < numOfreaders; i++) {
            readX[i] = 0;
            overflowCnt[i] = 0;
        }
    }

    int32_t getReaderId() { //< Warning: not thread safe!!!
        uint32_t myId = readerCnt++;
        if(myId >= numOfreaders) return -1; // no more readers!
        return static_cast<int32_t>(myId);
    }

    /** implements the generic interface of putter */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, const size_t msgLen,
            const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        RODOS_ASSERT_IFNOT_RETURN(msgLen <= sizeof(Type), false);
        put(*(const Type*)msg); // Always puts, even if it overwrites !
        return true;
    }

    /**  Always puts, even if it overwrites !  Return index where data was written */
    size_t put(const Type& val) {
        size_t writtenPos =  writeX;
        size_t nextIndex  =  advanceIndex(writeX);

        for(uint32_t i = 0; i < numOfreaders; i++) {
            if(nextIndex == readX[i]) { // one reader is to slow -> shift him!
                readX[i] = advanceIndex(readX[i]); 
                overflowCnt[i]++;
            }
        }

        buffer[writeX] = val;
        writeX         = nextIndex;
        return writtenPos;
    }

    /** return true == ok, false = fifo empty, val not modified */
    bool get(Type& val, uint32_t readerId) {
        if(readerId >= numOfreaders)  return false;
        if(readX[readerId] == writeX) return false;

        val = buffer[readX[readerId]];
        readX[readerId] = advanceIndex(readX[readerId]);
        return true;
    }

    ///
    int getLen() { return len; }
};


/**
  * A Fifo that lets you write or read continuous Blocks of the stored type.
  * Gives out pointers to the internal space, so you can copy data directly from/to there.
  * Useful for DMA transfers.
  */

template <class Type, size_t len>  class BlockFifo : public Fifo<Type,len> {
protected:
    size_t advanceIndexEx(size_t index,size_t advanceSize) {
        size_t newIndex = index + advanceSize;
        while(newIndex >= len) {
            newIndex -= len;
        }
        return newIndex;
    }


public:
    BlockFifo() { }


    /// returns pointer to write and maximal number of entries you may write
    Type* getBufferToWrite(size_t& maxLen) {
        size_t r = this->readX;
        size_t w = this->writeX;

        if(r == this->advanceIndex(w)) { //full
            maxLen =0;
            return 0;
        }

        if(r > w) { // put till r-1 possible
            maxLen = r - w - 1;
        } else { // put until end of buffer possible
            maxLen = len - w;
            if(r == 0) { maxLen--; }
        }
        return &(this->buffer[w]);
    }

    /// call this after you have written in the buffer you got from getBufferToWrite
    void writeConcluded(size_t numOfWrittenElements) {
        this->writeX = advanceIndexEx(this->writeX, numOfWrittenElements);
    }

    // returns pointer to read and maximal number of entries you may read
    Type* getBufferToRead(size_t& maxLen) {
        size_t r = this->readX;
        size_t w = this->writeX;

        if(r == w) { //empty
            maxLen =0;
            return 0;
        }

        if(r < w) { // get till w-1 possible
            maxLen = w - r;
        } else { // get until end of buffer possible
            maxLen = len - r;
        }
        return &(this->buffer)[r];
    }

    /// call this after you have read in the buffer you got from getBufferToRead
    void readConcluded(size_t sizeRed) {
        this->readX=advanceIndexEx(this->readX,sizeRed);
    }

};


}  // namespace

