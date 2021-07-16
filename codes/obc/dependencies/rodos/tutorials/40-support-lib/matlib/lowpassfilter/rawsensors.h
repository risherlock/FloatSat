
#pragma once

#include "stdio.h"
#include "matlib.h"
 
class RawSensors {
    bool inArray;
    FILE *measurements;
    char inputLine[200];
public:
    Vector3D acc, gyro, mag;

    RawSensors() { measurements = 0; inArray = false; }  

    bool read() {
        if(!measurements) measurements = fopen("measurements.m", "r");
        if(measurements == 0) return false;
        while(!feof(measurements)) {
            if(fgets(inputLine, 200, measurements) == nullptr) continue;
            if(inArray && inputLine[0] != ' ') return false;
            if(inputLine[0] == ' ') inArray = true;
            if(!inArray) continue;

            sscanf(inputLine, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", 
                &acc.x,  &acc.y,  &acc.z, 
                &gyro.x, &gyro.y, &gyro.z, 
                &mag.x,  &mag.y,  &mag.z);

            return true;
       }
       return false;
    }

    void close() { 
        if(measurements) fclose(measurements);
        measurements = 0; 
        inArray = false;
    }
};

