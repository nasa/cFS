/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     os-impl-console.c
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 *
 *      NOTE: This uses only the "Classic" RTEMS API.  It is intended to
 *      work on RTEMS targets that do not provide the POSIX API, i.e.
 *      when "--disable-posix" is given during the configuration stage.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-printf.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OSAL_CONSOLE_STREAM         stdout
/*
 * By default use the stdout stream for the console (OS_printf)
 */
#define OSAL_CONSOLE_FILENO     STDOUT_FILENO

/*
 * By default the console output is always asynchronous
 * (equivalent to "OS_UTILITY_TASK_ON" being set)
 *
 * This option was removed from osconfig.h and now is
 * assumed to always be on.
 */
#define OS_CONSOLE_ASYNC                true
#define OS_CONSOLE_TASK_PRIORITY        OS_UTILITYTASK_PRIORITY
#define OS_CONSOLE_TASK_STACKSIZE       OS_UTILITYTASK_STACK_SIZE


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/
/* Console device */
typedef struct
{
    bool            is_async;
    rtems_id        data_sem;
    int             out_fd;
} OS_impl_console_internal_record_t;


/* Tables where the OS object information is stored */
OS_impl_console_internal_record_t   OS_impl_console_table       [OS_MAX_CONSOLES];


/********************************************************************/
/*                 CONSOLE OUTPUT                                   */
/********************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleWakeup_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void  OS_ConsoleWakeup_Impl(uint32 local_id)
{
    OS_impl_console_internal_record_t *local = &OS_impl_console_table[local_id];

    if (local->is_async)
    {
        /* post the sem for the utility task to run */
        rtems_semaphore_release(local->data_sem);
    }
    else
    {
        /* output directly */
        OS_ConsoleOutput_Impl(local_id);
    }
} /* end OS_ConsoleWakeup_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleTask_Entry
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
static void OS_ConsoleTask_Entry(rtems_task_argument arg)
{
    uint32 local_id = arg;
    OS_impl_console_internal_record_t *local;

    local = &OS_impl_console_table[local_id];
    while (true)
    {
        OS_ConsoleOutput_Impl(local_id);
        rtems_semaphore_obtain(local->data_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    }
} /* end OS_ConsoleTask_Entry */

/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleCreate_Impl(uint32 local_id)
{
    OS_impl_console_internal_record_t *local = &OS_impl_console_table[local_id];
    int32 return_code;
    rtems_name r_name;
    rtems_id r_task_id;
    rtems_status_code status;

    if (local_id == 0)
    {
        return_code = OS_SUCCESS;
        local->is_async = OS_CONSOLE_ASYNC;
        local->out_fd = OSAL_CONSOLE_FILENO;

        if (local->is_async)
        {
            OS_DEBUG("%s(): Starting Async Console Handler\n", __func__);
            /*
            ** RTEMS task names are 4 byte integers.
            ** It is convenient to use the OSAL ID in here, as we know it is already unique
            ** and trying to use the real name would be less than useful (only 4 chars)
            */
            r_name = OS_global_console_table[local_id].active_id;
            status = rtems_semaphore_create( r_name, 0,
                                             RTEMS_PRIORITY,
                                             0,
                                             &local->data_sem);
            if (status != RTEMS_SUCCESSFUL)
            {
                return_code = OS_SEM_FAILURE;
            }
            else
            {
                status = rtems_task_create(
                             r_name,
                             OS_CONSOLE_TASK_PRIORITY,
                             OS_CONSOLE_TASK_STACKSIZE,
                             RTEMS_PREEMPT | RTEMS_NO_ASR | RTEMS_NO_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0),
                             RTEMS_LOCAL,
                             &r_task_id);

                /* check if task_create failed */
                if (status != RTEMS_SUCCESSFUL )
                {
                    /* Provide some freedback as to why this failed */
                    OS_DEBUG("rtems_task_create failed: %s\n", rtems_status_text(status));
                    rtems_semaphore_delete(local->data_sem);
                    return_code = OS_ERROR;
                }
                else
                {
                    /* will place the task in 'ready for scheduling' state */
                    status = rtems_task_start (r_task_id, /*rtems task id*/
                            OS_ConsoleTask_Entry, /* task entry point */
                            (rtems_task_argument)local_id );  /* passed argument  */

                    if (status != RTEMS_SUCCESSFUL )
                    {
                        OS_printf("rtems_task_start failed: %s\n", rtems_status_text(status));
                        rtems_task_delete(r_task_id);
                        rtems_semaphore_delete(local->data_sem);
                        return_code = OS_ERROR;
                    }
                }
            }
        }
    }
    else
    {
        /* only one physical console device is implemented */
        return_code = OS_ERR_NOT_IMPLEMENTED;
    }

    return return_code;
} /* end OS_ConsoleCreate_Impl */


