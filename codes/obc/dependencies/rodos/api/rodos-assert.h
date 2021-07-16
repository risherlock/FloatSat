#pragma once

#ifdef DANGEROUS_ASSERT_ENABLED
#  include <cassert>
#  define RODOS_ASSERT(_condition)                    assert(_condition)
#  define RODOS_ASSERT_IFNOT_RETURN_VOID(_condition)  assert(_condition)
#  define RODOS_ASSERT_IFNOT_RETURN(_condition, _val) assert(_condition)
#else
#  include "rodos-debug.h"
#  define RODOS_ASSERT(_condition)                    if(!(_condition))   RODOS::RODOS_ERROR(LOCATION); 
#  define RODOS_ASSERT_IFNOT_RETURN_VOID(_condition)  if(!(_condition)) { RODOS::RODOS_ERROR(LOCATION); return     ; }
#  define RODOS_ASSERT_IFNOT_RETURN(_condition, _val) if(!(_condition)) { RODOS::RODOS_ERROR(LOCATION); return _val; }
#endif

