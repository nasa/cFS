/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-osapi-time.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-time.c"


/* Stub objects to satisfy linking requirements -
 * the time module may look up entities in the timebase table
 */
OS_timebase_internal_record_t OS_timebase_table[OS_MAX_TIMEBASES];
OS_common_record_t OS_stub_timebase_table[OS_MAX_TIMEBASES];
OS_common_record_t OS_stub_timecb_table[OS_MAX_TIMERS];

OS_common_record_t * const OS_global_timebase_table   = OS_stub_timebase_table;
OS_common_record_t * const OS_global_timecb_table     = OS_stub_timecb_table;

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };

