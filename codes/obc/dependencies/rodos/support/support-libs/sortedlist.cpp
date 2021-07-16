
#include <stdint.h>
#include "sortedlist.h"

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/***************************************************************************************************************/

SortedChainable* SortedList::find(int64_t sortItem) {
    ITERATE_SORTED_LIST(*this)  {
        if (sortItem == iter->sortField) return iter;
    }
    return 0;
}

bool SortedList::remove(SortedChainable *entry) {

    if (entry == 0) return false;
    if (entry->currentList != this) return false;
    if (entry == &headAndTail) return false;

    entry->currentList = 0;

    (entry->next)->prev = entry->prev;
    (entry->prev)->next = entry->next;
    numberOfElements--;

    return true;
}

void SortedList::insertBefore(SortedChainable* beforeThis, SortedChainable* newEntry) {
    newEntry->next = beforeThis;
    newEntry->prev = beforeThis->prev;
    newEntry->currentList = this; 

    (beforeThis->prev)->next = newEntry;
    beforeThis->prev = newEntry;
    numberOfElements++;
}

/// inserts sorted by sortVal
bool SortedList::insert(SortedChainable *newEntry, int64_t sortVal) {

    if (newEntry == 0) return false;
    if (newEntry == &headAndTail) return false;
    if (newEntry->currentList != 0) {
        newEntry->currentList->remove(newEntry); // Else it would damage the other list
    }

    newEntry->sortField = sortVal;

    ITERATE_SORTED_LIST(*this)  {
        if (iter->sortField > sortVal) { // insert before this point
            insertBefore(iter, newEntry);
            return true;
        } // if found
    } // interatelist

    /** not found -> liste empty or time is bigger than any one else -> append ***/
    appendForce(newEntry, sortVal);
    return true;
}

/** adds an entry at the end of the list.
    For long lists it is faster than insert, but sortVal is restricted to be maxLongLong.
	if SortVal < sortField from last item, sortval will be set to sortFild  + 1
*/

bool SortedList::appendForce(SortedChainable *newEntry, int64_t sortVal) {
    if (sortVal < headAndTail.prev->sortField) sortVal = headAndTail.prev->sortField;
    return append(newEntry, sortVal);
}


bool SortedList::append(SortedChainable *newEntry, int64_t sortVal) {

    if (newEntry == 0) return false;
    if (newEntry == &headAndTail) return false;
    if (sortVal < headAndTail.prev->sortField) return false;

    if (newEntry->currentList != 0) {
        newEntry->currentList->remove(newEntry); // Else it would damage the other list
    }

    newEntry->currentList = this;
    newEntry->sortField   = sortVal;

    insertBefore(&headAndTail, newEntry);
    return true;
}



#ifndef NO_RODOS_NAMESPACE
}
#endif

