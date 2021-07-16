#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "threadtimings.h"

namespace Common {
extern ThreadTiming* threadTimingList[];
}
using namespace Common;

int gcd( int a, int b) {
    if ( a == b ) return a;
    if ( a < b ) return gcd ( a, b - a );
    return gcd ( a - b, b);
}

int lcm( int a, int b) {
    return a * b / gcd( a, b );
}

int checkOverlap ( ThreadTiming t1, ThreadTiming t2 ) {

    int64_t time = t1.startTime;
    while ( time < t2.startTime ) time += t1.period;
    int timeLimit = time + lcm ( t1.period, t2.period);
    for (; time < timeLimit; time+= t1.period )
        if( t2.runningAt( time )) return time;
    return 0;

}

int main() {
    //ThreadTiming t1 = {15, 11, 1, "x"};
    //ThreadTiming t2 = {6, 13, 3, "y"};

    for ( int i = 0; threadTimingList[i] != 0; i++)
        for ( int j = 0; threadTimingList[j] != 0; j++) {
            if ( i==j ) continue;
            int  collTime =  checkOverlap( *threadTimingList[i], *threadTimingList[j] );
            if(collTime != 0) printf("%s and %s : %d\n", threadTimingList[i]->name, 
                 threadTimingList[j]->name, collTime);
        }
    return 0;
}
