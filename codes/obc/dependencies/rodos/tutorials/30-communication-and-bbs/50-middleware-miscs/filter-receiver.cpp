#include "rodos.h"
#include "matlib.h"
#include "signalprocessing.h"
#include "filter-topics.h"

LowpassFilter doubFilter;
LowpassFilterObject<Vector3D> vectFilter;

Subscriber nameNotImportant06(topicDoub, doubFilter, "dobub06");
Subscriber nameNotImportant07(topicVect, vectFilter, "vect07");

class ReceiverFilter : public StaticThread<> {
    void run () {
        doubFilter.historyLen = 4;
        doubFilter.historyLen = 4;
        TIME_LOOP(3500*MILLISECONDS, 2*SECONDS) {
            PRINTF( "     receiver %f (%f %f %f) \n", doubFilter.mean, vectFilter.mean.x, vectFilter.mean.y, vectFilter.mean.z);
        }
    }
} receiverFilter;

