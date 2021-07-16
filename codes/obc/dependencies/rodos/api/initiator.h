
/**
 * @file initiator.h
 * @date 2008/05/01 16:15
 * @author Sergio Montenegro, Lutz Dittrich
 *
 *
 * Different events handlers
 *
 * @brief Just an interface class to allow an application to initialize before
 * scheduling.
 */

#pragma once

#include "listelement.h"

namespace RODOS {

/**
 * @class Initiator
 *
 * @brief Just an interface class to allow an application to initialize before
 * being scheduled.
 *
 * It provides an init method which shall be overwritten by the applications
 * init() method. The init() method will be invoked before the execution of any
 * thread is started.
 */

class Initiator : public ListElement {
    friend void initSystem();
protected:
    /** default list of all events */
    static List initiatorList;

    /**
     * Called by the initiator only once for each object in the system
     * at start up.
     *
     * Called before scheduling von threads begins.
     */

    virtual void init(void) { }

public:
    /**
     * Constructor.
     * @param name The name of the initiator object.
     */
    Initiator(const char* name = "Initiator") : ListElement(initiatorList, name) { }

    /**
     * Destructor.
     */
    virtual ~Initiator()  { }

};


} //namespace

