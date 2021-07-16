#pragma once
/**
 * @date 2019/08/03
 * @author Sergio Montenegro, Uni Wuerzburg
 *
 * @brief class to combine a return value with a possible error code
 *
 */

#include "stdint.h"
#include "rodos-assert.h"
#include "rodos-errorcodes.h"

namespace RODOS {

template <typename Type>
struct Result {
    ErrorCode err;
    Type      val;

    Result()                                   : err(ErrorCode::OK) {}
    Result(const Type& v)                      : err(ErrorCode::OK), val(v) {}
    Result(const Type&& v)                     : err(ErrorCode::OK), val(v) {} //  add support for rvalues for v
    Result(const ErrorCode& e)                 : err(e) {}
    Result(const ErrorCode& e, const Type& v)  : err(e), val(v) {}
    Result(const ErrorCode& e, const Type&& v) : err(e), val(v) {} //  add support for rvalues for v

    bool    isOk() const                 { return err == ErrorCode::OK; }
    int32_t getErr() const               { return (int32_t)err; }
    void    setErr(int32_t iErr)         { err = (ErrorCode)iErr; }

    operator Type() { // Dangerous! But for compatibility with older code
        RODOS_ASSERT(this->isOk()); // The calling code will use wrong data (TODO: but you may remove this assert)
        return val;
    }
};

} // Namespace RODOS
