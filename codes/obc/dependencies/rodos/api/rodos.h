/**
 * @file rodos.h
 * @date 2008/06/06 , 2010, 2012, 2016, 2019
 * @author Sergio Montenero
 *
 * See the README.txt file
 */

#pragma once

#include "rodos-version.h"

#include <stdint.h>

#include "default-platform-parameter.h"

#include "rodos-errorcodes.h"

#include "string_pico.h"
//#include "stdlib_pico.h"

// #define DANGEROUS_ASSERT_ENABLED  // if you activate/deactivate it, please recompile the rodos libs
#include "rodos-debug.h"
#include "rodos-assert.h"

#include "hostinfos.h"  /* architecture dependent */

#include "misc-rodos-funcs.h"
#include "checksumes.h"
#include "stream-bytesex.h"

#include "listelement.h"

#include "initiator.h"

#include "timemodel.h"
#include "application.h"
#include "reserved_application_ids.h"
#include "thread.h"

#include "putter.h"
#include "fifo.h"
#include "commbuffer.h"

#include "rodos-semaphore.h"
#include "timeevent.h"
#include "timepoints.h"

#include "topic.h"
#include "reserved_topic_ids.h"
#include "subscriber.h"

#include "yprintf.h"

#include "gateway.h"

//___________________________ 
using namespace RODOS;

