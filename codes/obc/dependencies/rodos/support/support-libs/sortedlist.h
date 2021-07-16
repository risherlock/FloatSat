
#pragma once

namespace RODOS {

/** SortedChainable are entries in a sortedList.
 *
 *   SortField has to be initialized
 *   before inserting into a SortedList. Def 0.
 *
 *   @autor Sergio Montenegro
 *
*/

class SortedList;

class SortedChainable  {

    friend class SortedList;

    /// Only SortedList can access this field
    int64_t   sortField;

    /// The list where the entry is currently chained - ONLY ONE!, or 0: in no list.
    SortedList* currentList;

    SortedChainable  *next;
    SortedChainable  *prev;


public:

    /// Any object to be administrated (for your use only). For BOSS not required/used
    void  *associatedObject;

    /** NO constructor to be able to habe 1000's static elements **/

    void init() {
        next = 0;
        prev = 0;
        sortField = 0;
        currentList = 0;
        associatedObject = this;
    }

    void init(void* ownedObj) {
        init();
        associatedObject = ownedObj;
    }


    ///
    inline int64_t getSortField() { return sortField; }

    inline SortedChainable *getNext() { return next; }

    /** In which list am I registered, 0 -> no list */
    inline SortedList* getCurrentList() { return currentList; }
};


/***********************************************************************************/

/** SortedList administrates a list of SortedChainables.
    <P>
    The list will be sorted using a int64_t sortField in increasing order,
    the smallest at the beginning, the biggest at the end.
    Using append() the list works like a FIFO without sorting.
    For long lists this is faster than insert(). Such entries get
    MAX_SORTVAL as sortField.
    <p>
    Several entries with the same value in sortField are sorted
    first come first serve: First come is ahead, last come is at the end.
    <P>
    Each SortedChainable can be only in one list at a time.
    Before one is inserted in a list it has to be removed from any other list.
    <p>
    <b>Warning 1<\b>: Data in sorted chainables will not be copied. Do not modify
    it after inserting in the list
    <p>
    <b>Warning 2<\b>: not Threadsafe, Protect using external semaphores
    <p>
    <b>Warning 3<\b>: sorted valus testes only for positive values

    @see SyncCommFifo
    @autor Sergio Montenegro, 1999
*/


class SortedList {

    /// READ THIS!!!!!!!

    /* WARNING: This implementation of a linked Liste is some how different.
     * from outside it is a normal sorted list, but inside more simple.
     * I do not like exeptions, to avoid exptions when handling empty lists,
     * the first element of the list and the last element of the list I do:
     * The list is never empty, there is no last and no first!
     * 1. There is no first and last pointer.
     * 2. There is a fiexe member in the list which can not be remobed nor read
     *    its name is headAndTail
     * 3. if the list is empty headAndTail.next = headAndTail.prev = &headAndTail
     * 4. headAndTail.next points to the first in the list
     * 5. headAndTile.prev points to the last in the list
     */

    SortedChainable headAndTail;

    int32_t numberOfElements;

    void insertBefore(SortedChainable* beforeThis, SortedChainable* newEntry);

public:

    int32_t getNumberOfElements() { return numberOfElements; }

    bool endOfList(SortedChainable* item)  {
        if(item == 0) return true;
        return item == &headAndTail;
    }

    ///
    SortedList() {
        numberOfElements = 0;
        headAndTail.init();
        headAndTail.next = headAndTail.prev = &headAndTail;
    }

    SortedChainable* getFirst(void) {
        if(headAndTail.next == &headAndTail) return 0;
        return headAndTail.next;
    }
    SortedChainable* getLast(void)  {
        if(headAndTail.prev == &headAndTail) return 0;
        return headAndTail.prev;
    }

   int64_t getLastValue()  { return headAndTail.prev->sortField; }
   int64_t getFirstValue() { return headAndTail.next->sortField; }

    SortedChainable* getAndRemoveTheFirst(void) {
        SortedChainable* entry = getFirst();
        remove(entry);
        return entry;
    }

    SortedChainable* find(int64_t sortItem);

    /// Removes entry from the list, Its pointer will not be cleared to be able to continiu iteractions
    bool remove(SortedChainable* entry);

    /// inserts newEntry sorted by sortVal
    bool insert(SortedChainable* newEntry, int64_t sortVal);


    /// adds an entry at the end of the list. For long lists this is faster than insert.
    /// If not set,the sortval will be set automaticaly = last->sortedValud + 1
    bool append(SortedChainable* newEntry, int64_t sortVal);
    bool appendForce(SortedChainable *newEntry, int64_t sortVal);

};

#define ITERATE_SORTED_LIST(__list) \
    for (SortedChainable* iter = (__list).getFirst(); !(__list).endOfList(iter); iter = iter->getNext())

#define ITERATE_SORTED_TYPED_LIST(__list, __type) \
    for (__type* iter = (__type*)(__list).getFirst(); !(__list).endOfList(iter); iter = (__type*)iter->getNext())


#define FOR_ALL_i(_limit) for(int i = 0; i < _limit; i++)

}

