
/**
* @file listelement.h
* @date 2008/04/28 16:13
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief linked list management (header)
*
*/

#pragma once

#include <stdint.h>

namespace RODOS {


/**
* @def ITERATE_LIST(__type,__list)
* @param[in] __type Base type of list nodes.
* @param[in] __list The list.
* a handy macro: for_each_do
*/
#define ITERATE_LIST(__type,__list) \
                                for (__type* iter = (__type*)__list; iter!=0;\
                                            iter = (__type*)(iter->getNext()))


class ListElement;

/** A list is just the pointer to the first element */
typedef ListElement* List;


/**
* @class ListElement
* @brief node of a single linked list
*
* A node of a single linked list.
* @warning Not thread safe,
* only for static elements which will never be deleted, eg. never on stacks
*/
class ListElement {

private:
  /** pointer to next element */
  ListElement* next;
  static int32_t listElementCounter;

public:

  /** name of the element */
  const char* name;
  int32_t listElementID; ///< set from listElementCounter
  void* owner; ///< free for user, rodos does not use it

  /** Creates an element and adds it to the beginning of the list.
  * @param[in,out] list The list the element shall be added to.
  * @param[in] name Clear text name.
  */
  ListElement(List &list, const char* name = "ListElement", void* myOwner = 0);

  /** destructor */
  virtual ~ListElement();

  void append(List& list);

  /**  Get next element in list.
  *  @return pointer to next element in the list
  */
  ListElement* getNext() const {
  	return next;
  }

  /**
  * Returns the name of the object as string.
  * @return pointer to name of the object
  */
  const char* getName() const {
    return name;
  }

};


} // namespace


