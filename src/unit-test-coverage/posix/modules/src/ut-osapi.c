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

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi.c"

/* Stub objects to satisfy linking requirements -
 */
OS_common_record_t OS_stub_task_table[OS_MAX_TASKS];
OS_common_record_t * const OS_global_task_table   = OS_stub_task_table;
OS_common_record_t OS_stub_queue_table[OS_MAX_QUEUES];
OS_common_record_t * const OS_global_queue_table   = OS_stub_queue_table;

OS_queue_internal_record_t OS_queue_table[OS_MAX_QUEUES];
OS_task_internal_record_t OS_task_table[OS_MAX_TASKS];
OS_console_internal_record_t OS_console_table[OS_MAX_CONSOLES];

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };

