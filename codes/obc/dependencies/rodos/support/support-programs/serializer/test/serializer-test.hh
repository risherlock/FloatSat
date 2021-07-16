
//#pragma once
#include "stdint.h"
#include "stdlib.h"
#include "time.h" 
/** testing the serializer application ***/

//using namespace Common;
//using namespace std;

struct SerializerTest {

    double   d;
    float    f;
    int64_t  i64;
    int32_t  i32;
    int16_t  i16;
    int8_t   i8;
    uint64_t ui64;
    uint32_t ui32;
    uint16_t ui16;
    uint8_t  ui8;
    bool     b;
    int32_t a[4];


    /****************************************/
    SerializerTest() { 
        d = 0.0;
        f = 0.0;
        i64 = 0;
        i32 = 0;
        i16 = 0;
        i8 = 0;
        ui64 = 0;
        ui32 = 0;
        ui16 = 0;
        ui8 = 0;
        b = false;
        a[0] = 0;
        a[1] = 1;
        a[2] = 2;
        a[3] = 3;
        srand(time(0));
    }
    void update(){
        d = (double)(rand()%100)/(rand()%10000);
        f = (double)(rand()%100)/(rand()%1000);
        i64 = rand()%4294967296 -4294967296/2;
        i32 = rand()%4294967296-4294967296/2;
        i16 = rand()%65536 - 65536/2;
        i8 = rand()%256 - 256/2;
        ui64 = rand()%4294967296;
        ui32 = rand()%4294967296;
        ui16 = rand()%65536;
        ui8 = rand()%256;
        b = rand()%2;
        a[0] = i32+1;
        a[1] = i32+2;
        a[2] = i32+3;
        a[3] = i32+4;
    }

    
    int serialize(char * const) const;
    int deserialize(char const * const);

};


