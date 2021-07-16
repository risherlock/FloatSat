
#include "stdio.h"
#include "stdint.h"
#include "threadtimings.h"

#define NANOSECONDS 1000000000.0

namespace Common {
extern ThreadTiming* threadTimingList[];
}
using namespace Common;

int64_t gcd( int64_t a, int64_t b) {
    if ( a == b ) return a;
    if ( a < b ) return gcd ( a, b - a );
    return gcd ( a - b, b);
}

int64_t lcm( int64_t a, int64_t b) {
    return a * b / gcd( a, b );
}

int64_t checkOverlap ( ThreadTiming t1, ThreadTiming t2 ) {

    int64_t time = t1.startTime;
    while ( time < t2.startTime ) time += t1.period;
    int64_t timeLimit = time + lcm ( t1.period, t2.period);
    for (; time < timeLimit; time+= t1.period )
        if( t2.runningAt( time )) return time;
    return 0;

}

int main() {
    ThreadTiming* tt;
    printf("\n\n");
    for(int i = 0; threadTimingList[i] != 0; i++) { 
        tt = threadTimingList[i];
	int color = 0;
//check for collisions
        for ( int j = 0; threadTimingList[j] != 0; j++) {
            if ( i==j ) continue;
           if(checkOverlap( *threadTimingList[i], *threadTimingList[j] ) != 0 || checkOverlap( *threadTimingList[j], *threadTimingList[i] ) != 0) {
		color = 1;
	    }
        }

//print
        int seconds = 0; // in seconds, to print name once per second
	for( double time = (double)tt->startTime/NANOSECONDS; time < 20; time+=(double)tt->period/NANOSECONDS) {
           printf("rectangle('Position', [%f, %f, %f, %f], 'FaceColor', [%d, %d, 0]);\n",
           	time,
           	(double)i,
           	(double)tt->length/NANOSECONDS,
           	0.5,
		color,
		1-color);
           if(seconds != (int)time) printf("text(%f, %f, '%s');\n", time, double(i), tt->name);
           seconds = (int)time;
	}
    }

    printf("input \"enter to terminate\"\n\n");
    return 0;
}
