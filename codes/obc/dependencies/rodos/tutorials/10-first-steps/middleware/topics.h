
/** Example topics for the tutorials **/

#pragma once

struct Pos {
    double x;
    double y;
    double z;
};

extern Topic<Pos>     position;
extern Topic<double>  speed;


