

#pragma once

#include "math.h" // sin, sqrt, etc

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif




// Tutorials in matlib/tutorail/







/** All values -1 .. 1 or 0 ..1: scale transforms normilized data to your scale **/
class LowpassFilter : public Putter {
    double lastVal;
    double lastMean;
public:
    double scale;      ///< input: your scale for normilized lasVal, and mean
    int historyLen;    ///< input: 0 ... 10^6. 0: no filtering. 10^6: no reaction to new data, recomend ca 8
    double noiseIndex; ///< output: 0 : no noise, signal is a constant! 1: maximal noise, no info in signal
    double mean;       ///< output: Filtered value
    double noiseAfterFilter; ///< output: like noiseIndex
 
    LowpassFilter() { lastVal = noiseIndex = mean = lastMean = noiseAfterFilter = 0 ; historyLen = 8; scale = 1.0; }

    inline double getMean() { return mean*scale; }   

    inline double accumulateNormalizing(double val) { return accumulateAsIs(val/scale) * scale; }

    double accumulateAsIs(double val) {

        mean = (mean * historyLen + val) / (historyLen + 1);

        // variance = (variance * historyLen + (val - mean)*(val-mean)) /  (historyLen + 1); // it shall be  like this

        // Statistiks noiseIndex (like variance of the signal)
        double diff = lastVal - val;
        lastVal = val;
        if(diff < 0) diff = -diff;
        if(diff > 1) diff = 1;
        noiseIndex = (noiseIndex*historyLen + diff) / (historyLen + 1);
        if(noiseIndex > 1.0) noiseIndex = 1;

        diff = lastMean - mean;
        lastMean = mean;
        if(diff < 0) diff = -diff;
        if(diff > 1) diff = 1;
        noiseAfterFilter = (noiseAfterFilter*historyLen + diff) / (historyLen + 1);
        if(noiseAfterFilter > 1.0) noiseAfterFilter = 1;

        return mean;
    }

    /** implements the generic interface of putter, to allow receive from middleware */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        accumulateNormalizing(*static_cast<const double*>(msg));
        return true;
    }

};


template <typename Type> class LowpassFilterObject : public Putter {
    Type lastVal;
    Type lastMean;
public:
    double scale;      ///< input: your scale for normilized lasVal, and mean
    int historyLen;    ///< input: 0 ... 10^6. 0: no filtering. 10^6: no reaction to new data, recomend ca 8
    double noiseIndex; ///< output: 0 : no noise, signal is a constant! 1: maximal noise, no info in signal
    double noiseAfterFilter; ///< output: like noiseIndex
    Type mean;       ///< output: Filtered value
 
    LowpassFilterObject() { noiseIndex = noiseAfterFilter = 0 ; historyLen = 8; scale = 1.0; }

    inline Type getMean() { return mean*scale; }   

    inline Type accumulateNormalizing(const Type& val) { Type scaled = val / scale; return accumulateAsIs(scaled) * scale; }

    Type accumulateAsIs(Type& val) {

        mean = (mean * historyLen + val) / (historyLen + 1);

        // variance = (variance * historyLen + (val - mean)*(val-mean)) /  (historyLen + 1); // it shall be  like this

        // Statistiks noiseIndex (like variance of the signal)
        double diff = lastVal.distance(val);
        lastVal = val;
        if(diff < 0) diff = -diff;
        if(diff > 1) diff = 1;
        noiseIndex = (noiseIndex*historyLen + diff) / (historyLen + 1);
        if(noiseIndex > 1.0) noiseIndex = 1;

        diff = lastMean.distance(mean);
        lastMean = mean;
        if(diff < 0) diff = -diff;
        if(diff > 1) diff = 1;
        noiseAfterFilter = (noiseAfterFilter*historyLen + diff) / (historyLen + 1);
        if(noiseAfterFilter > 1.0) noiseAfterFilter = 1;

        return mean;
    }

    /** implements the generic interface of putter, to allow receive from middleware */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        accumulateNormalizing(*static_cast<const Type*>(msg));
        return true;
    }

};

#pragma once

/**
TrustedVal ald TrustedObject are (double) values or composed objects which
includes extra information about the certainty of the contained data. 
Certainty 1 means the contained values are certain and can be trusted.
Certainty 0 means the contained values are random (ony noise), do not use.

The certainty may be set by user. Operations between ThrustedValues
generate updated values and adjust certainty according to operations and values.
Certainty may increase (going closer to 1) or decrease (going closer to 0).

Every new coputation of val, shall update its certainity!

Shalls:
	values are normalized: (0 .. 1) or (1 .. 1)
	Angles are encodded (1- .. 1): corresponds to -180 .. 180 degres
	all vectors are normalized: len(vector) = 1
	sensor data (-1 .. 1) represents the max range of sensor

to use the val externaly multiply (to read) or devide (to set) by scale.

Author: Sergio Montenegro, January 2015

*/

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#define PLEASE_UNDEF_MIN
#endif


struct TrustedVal {
    double val;
    double certainty; ///< 0 (no infor, random data, do not use) ... 1(absoltle certain)
    double scale;     ///< scale of value when used externaly

    double certaintySensibility; ///< used to modify cerntainty: to 0 .. 1: 0: no certainty adaptions

    TrustedVal() { certaintySensibility = 0.5; val = certainty = 0; scale = 1.0;}
    TrustedVal(double v, double c = 1.0, double sc = 1.0) { certaintySensibility = 0.5; val = v; certainty = c; scale = sc; }

    inline double getVal()         { return val*scale; }
    inline void   setVal(double v) { val = v / scale;    }


    inline void decreaseCertainty() { 
        double newCertainty = certainty*certainty;
        certainty = certainty - (certainty - newCertainty)*certaintySensibility;
    }

    inline void mergeVal(const TrustedVal& other) {
        double oldVal = val;
        val = (val*certainty + other.val*other.certainty) / (certainty + other.certainty);
        if (!isfinite(val)) { val = oldVal; certainty = 0; }
    }

    inline void mergeValAndCertainty(const TrustedVal& other) {
        mergeVal(other);

	// increase certainity according to conformity of both values
        // easier to compute using uncertainty u = (1 - certainty). 
        // range of new unvertainty = u1*u2 ... min(u1,u2)
        // smallest uncertainty (u1*u2) if both values are identical
        // biggest uncertainty (mnin(u1,u2) if values diverge by a lot
        double u1 = 1 - certainty;  
        double u2 = 1 - other.certainty;
        double distance = (val - other.val);
        distance = distance*distance;
        if(distance > 1) distance = 1;
        double newCertainty = 1 - (u1*u2 + distance*(MIN(u1,u2) - u1*u2));
        if (!isfinite(newCertainty)) {  newCertainty = 0; }
        certainty = certainty - (certainty - newCertainty)*certaintySensibility;
    }
};

inline TrustedVal mergeVal(const TrustedVal& a, const TrustedVal& b) {
    TrustedVal result = a;
    result.mergeVal(b);
    return result;
}
inline TrustedVal mergeValAndCertainty(const TrustedVal& a, const TrustedVal& b) {
    TrustedVal result = a;
    result.mergeValAndCertainty(b);
    return result;
}


/** 
  The given type has to implement:
  operator*(Type, double), operator+(Type, Type), operator- (Type, Type), 
  double Tpye::distance(Type other) : a value from 0 to 1
**/


template <typename Type> struct TrustedObject {
    Type val;
    double certainty; ///< 0 (no infor, random data, do not use) ... 1(absoltle certain)

    double certaintySensibility; ///< used to modify cerntainty: to 0 .. 1: 0: no certainty adaptions

    TrustedObject(double c = 0) { certaintySensibility = 0.5; certainty = c;}
    TrustedObject(Type& t, double c = 1.0) { certaintySensibility = 0.5; val = t; certainty = c;}

    inline void decreaseCertainty() { 
        double newCertainty = certainty*certainty;
        certainty = certainty - (certainty - newCertainty)*certaintySensibility;
    }

    inline void mergeObject(const TrustedObject<Type>& other) {
        double conbinedInversCertainty = 1.0 / (certainty + other.certainty);
        if (isinf(conbinedInversCertainty)) { certainty = 0; }
        else val = (val*certainty + other.val*other.certainty) * conbinedInversCertainty;
    }

    inline void mergeObjectAndCertainty(const TrustedObject<Type>& other) {
        mergeObject(other);

	// increase certainity according to conformity of both values
        // easier to compute using uncertainty u = (1 - certainty). 
        // range of new unvertainty = u1*u2 ... min(u1,u2)
        // smallest uncertainty (u1*u2) if both values are identical
        // biggest uncertainty (mnin(u1,u2) if values diverge by a lot
        double u1 = 1 - certainty;  
        double u2 = 1 - other.certainty;
        double distance = val.distance(other.val);
        distance = distance*distance;
        double newCertainty = 1 - (u1*u2 + distance*(MIN(u1,u2) - u1*u2));
        if (!isfinite(newCertainty)) {  newCertainty = 0; }
        certainty = certainty - (certainty - newCertainty)*certaintySensibility;
    }
};



#ifdef PLEASE_UNDEF_MIN
#undef MIN
#endif

#ifndef NO_RODOS_NAMESPACE
}
#endif


