#pragma once
/**
 * @date 2019/08/03
 * @author Sergio Montenegro, Uni Wuerzburg
 *
 * @brief Error codes definitions 
 */

#include <stdint.h>

namespace RODOS {

enum class ErrorCode : uint32_t {
    OK       = 0,
    NO_ERROR = 0,

    //_______Generic and undefined  errors
    UNKNOWN = 1,
    // .... to be extended  (by RODOS Developers)

    //_______Memory errors
    MEMORY = 100,
    //..... to be extended (by RODOS Developers)

    //_______Parameter out or range
    WRONG_PARAM = 200,
    NO_RESULT = 201,
    OUT_OF_RANGE,
    BAD_INDEX,
    BAD_POINTER,
    // .... to be extended (by RODOS Developers)
    //_______Communication errors
    // .... = 400 to be extended (by RODOS Developers)
    //_______Time and timing errors
    // .... = 500 to be extended (by RODOS Developers)

    //_______ Numerical and matlib errors
    NUMERICAL      = 600,
    VECTOR_LEN,
    NO_NORMALIZED,
    VECTOR_LEN_0,
    COLINEAR_VECTORS,
    //..... to be extended (by RODOS Developers)

    //_______IO errors
    NO_DATA_AVAILABLE, 
    TIMING_ERR,
    WAITING,
    // .... = 700, (by RODOS Developers)

    //_______File errors
    // .... = 800, (by RODOS Developers)

    //_______ undefined Application errors
    UNDEF_APPLICATION = 1000,
    // .... to be extended by users, see commets how to define your error codes

    //_______ defined Application errors (Anomalies) see commets how to define your error codes
    ANOMALY            = 10000 // from 10K, you shall define errors for your application
    // do not add here, create another enum class and just cast (ErrorCode)AnomalyId::ANOMALY_XXX

};

/** How to define your own error codes (normally application error codes are called "anomalies")
 * unfortunately c++ do not allow to inherit from enum class
 * do:
 *   enum class AnomalyId : uint32_t {
 *      ANOMALY_1 = 10001, // for application errors begin at 10000
 *      ANOMALY_2 = 10002,
 *      ANOMALY_3 = 10003
 *  };
 * ...
 * in your function:
 *    return (ErrorCode)AnomalyId::ANOMALY_1;
 */


} // Namespace RODOS
