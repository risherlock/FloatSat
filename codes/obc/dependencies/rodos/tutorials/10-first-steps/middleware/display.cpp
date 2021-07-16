#include "rodos.h"
#include "topics.h"

static CommBuffer<Pos>    posbuf;
static CommBuffer<double> speedbuf;
static Subscriber         namenotimportant1(position, posbuf,   "posreceiverbuf");
static Subscriber         namenotimportant2(speed,    speedbuf, "speedreceiverbuf");

class Display : public StaticThread<> {
    void run() {
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            Pos    p;
            double v;
            posbuf.get(p);
            speedbuf.get(v);
            PRINTF("Position (%3.2f;%3.2f;%3.2f) speed %3.2f\n", p.x, p.y, p.z, v);
        }
    }
} display;
