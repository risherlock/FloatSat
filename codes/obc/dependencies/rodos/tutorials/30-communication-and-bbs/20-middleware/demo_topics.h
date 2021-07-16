
/** Example topics for the tutorials **/

#ifndef __demotopics_h__
#define __demotopics_h__

struct Position {
	float x, y, z;
};


extern Topic<long>   	counter1;
extern Topic<long>      counter2;
extern Topic<Position>  position;


#endif

