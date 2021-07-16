/**
* @file application.h
* @date 2008/06/04 11:46
* @author Sergio Montenegro
*
* @brief collection of information about an application (header)
*
*/
#pragma once


#include <stdint.h>
#include <stddef.h>

#include "listelement.h"

namespace RODOS {

/**
* @class Application
* @brief collection of information about an application.
*
* Registers applications names/versions to be printed and
* to be searched for (discovery).
* Each application gets a name which will be printed at system start up.
* Each application has an optional applicationID number which can be used by
* other applications to find it and to determine if it was loaded/linked or
* not.
*/
class Application: public ListElement {
    friend void initSystem();

private:
    /** List of all registered applications.
    */
    static List applicationList;

    /// free usable to identify and find loaded modules by number/id
    int32_t applicationId;

public:
    /** Constructor; Creates and registers an application with given name
      * and id.
      * @param[in] name The name of the application.
      * @param[in] id The application id.
      *
      * Please note: The name of the created will never be used. In our
      * examples we call it for example 
      *    Application nameNotImportant01(...);
      * The string name is a help ONLY to debug code.
      *
      */

    Application(const char* name, const int32_t id=1000);

    /** Destructor; Should never be called.
      */
    virtual ~Application();

    int32_t getID() const { return applicationId; }

    /** Prints all registered applications and corresponding IDs.
        (do not call before main())
    */
    static void printApplications();

    /** If the list contains an application with the given id then this
    *   functions returns a pointer to this application. If there is more
    *   than one application with that id, this function returns a pointer
    *   to the first one it encounters. In any other case
    *   this method returns null.  (do not call before main())
    *   @param[in] id The id to search for.
    *   @return pointer to application with given id or null if not found.
    */
    static Application* findApplication(const int32_t id);

    /// to signal activate / deactivate applications
    bool isActivated;

    virtual void init(void) { } ///< called by rodos::main at system start

    virtual void activate(); ///< user implemented to activate the application
    virtual void deactivate(); ///< user implemented to deactivate the application

    /** To send requests to applications, The application may answer.
      * @return number of bytes written in answerMsg, <0 -> error, -1 -> not implemented
      */
    virtual int32_t request(void* requestMsg, size_t len, void* answerMsg, size_t maxLen);

};

} // namespace

