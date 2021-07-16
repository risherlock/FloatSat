#pragma once


class FilteredSensors {
    RawSensors rawSensors;
public:
    LowpassFilterObject<Vector3D> aFilter, gFilter, mFilter;

// Clibration and initial data
    Vector3D   nadirVectorInRepose;
    Vector3D   magnetVectorInRepose;
    Vector3D   gyroOffset;
    //YPR        gyroOffset;
    Quaternion magnetToNadirQ;

    void calibrateSensors(int numOfSteps);
    bool readAndFilter(); // false -> no more data
};

