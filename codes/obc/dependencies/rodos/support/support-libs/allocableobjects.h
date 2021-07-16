
/**
* @file allocableobjects.h
* @date 2021/02/11
* @author Sergio Montenegro
*
* @brief AllocableObject malloc/free with reference counter
*
*/


#pragma once
#include "stdint.h"
#include "rodos-result.h"
  
namespace RODOS {

/**
 * Memory allocation with reference counter, for a "Reference Counting Garbage Collection"
 *
 * Each object has a count of the number of references to it. 
 * object can be allocated with alloc. You can copy references ONLY by using the function copyReference
 * which will increment the reference counter.
 * the allocated object and copies of references shall be released using the function free.
 * when the reference counters reaches 0, free will put the object back to the free list.
 *
 * An object's reference count is incremented when a reference to it is created, and decremented when a reference is destroyed.
 * When the count reaches zero, the object's memory is reclaimed (not automatically, but when calling free)
 *
 * WARNING! Not Thread safe, if required protect with a semaphore 
 *
 */

template <typename Type, uint32_t len> class AllocableObjects {

    Type      buffer[len];
    uint32_t  referenceCnt[len];
    uint32_t  freeCnt;

public:
    Result<uint32_t> indexOf(Type* item)  { 
        uint32_t index = (uint32_t)(item - buffer); 
        if(index > len) return ErrorCode::BAD_POINTER;
        return index;
    }

    uint32_t getNumOfFreeItems()  { return freeCnt; }

    void init() {
        freeCnt = len;
        for(uint32_t i = 0; i < len; i++)  referenceCnt[i] = 0;
    }
  
    // Returns pointer to the new object or 0 if no object is free
    Type* alloc() {
        for(uint32_t i = 0; i < len; i++) {
            if(referenceCnt[i] == 0) {
                freeCnt--;
                referenceCnt[i] = 1;
                return &buffer[i];
            }
        }
        return nullptr;
    }
    
    bool free(Type* item) {
        Result<uint32_t> index = indexOf(item);
        if(!index.isOk())               return false;
        if(referenceCnt[index.val] < 1) return false;
        referenceCnt[index.val]--; 
        if(referenceCnt[index.val] == 0) freeCnt++;
        return true;
    }

    // Returns pointer to the same item, or 0 if invalid
    Type* copyReference(Type* item) {
        Result<uint32_t> index = indexOf(item);
        if(!index.isOk())                return nullptr;
        if(referenceCnt[index.val] == 0) return nullptr;
        referenceCnt[index.val]++; 
        return item;
    }
};
} // namespace

