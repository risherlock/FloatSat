
/**
* @file application.cc
* @date 2008/06/04 11:41
* @author Sergio Montenegro, Lutz Dittrich
*
*
* \brief Collection of information about an application (header)
*
*/


#include "application.h"
#include "listelement.h"
#include "rodos-debug.h"
#include "misc-rodos-funcs.h"

namespace RODOS {

/* list of applications registered */
List Application::applicationList = 0;

Application::Application(const char* name_, const int32_t id) :
                         ListElement(applicationList, name_) {
  applicationId = id;
  isActivated = true;
}


Application::~Application() {
  if(isShuttingDown) return;
  RODOS_ERROR("Application deleted");
}

/** more code to come from user */
void Application::activate() {
	isActivated = true;
}

/** more code to come from user */
void Application::deactivate() {
	isActivated = false;
}

int32_t Application::request(void*, size_t, void*, size_t) {
	return -1;
}


/**
* Prints all applications names and ids
*/
void Application::printApplications() {
  ITERATE_LIST(Application, applicationList) {
    PRINTF("      %5d -> '%s'\n", (int)iter->applicationId, iter->getName());
  }
}


/**
* returns a reference to the associated application, if not found 0
*/
Application* Application::findApplication(const int32_t id) {
  ITERATE_LIST(Application, applicationList) {
    if (iter->applicationId == id) {
      return iter;
    }
  }
  return 0;
}


} // namespace


