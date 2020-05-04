/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-impl-console.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-impl-console.h"

#include "os-shared-printf.h"

/*
 * By default the console output is always asynchronous
 * (equivalent to "OS_UTILITY_TASK_ON" being set)
 *
 * This option was removed from osconfig.h and now is
 * assumed to always be on.
 */
#define OS_CONSOLE_ASYNC                true
#define OS_CONSOLE_TASK_PRIORITY        OS_UTILITYTASK_PRIORITY


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
        sem_post(&local->data_sem);
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
 *  Purpose: Local Helper function
 *           Implements the console output task
 *
 *-----------------------------------------------------------------*/
static void*  OS_ConsoleTask_Entry(void* arg)
{
    OS_U32ValueWrapper_t local_arg;
    OS_impl_console_internal_record_t *local;

    local_arg.opaque_arg = arg;
    local = &OS_impl_console_table[local_arg.value];
    while (true)
    {
        OS_ConsoleOutput_Impl(local_arg.value);
        sem_wait(&local->data_sem);
    }
    return NULL;
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
    pthread_t consoletask;
    int32 return_code;
    OS_U32ValueWrapper_t local_arg = { 0 };

    if (local_id == 0)
    {
        return_code = OS_SUCCESS;
        local->is_async = OS_CONSOLE_ASYNC;

        if (local->is_async)
        {
            if (sem_init(&OS_impl_console_table[local_id].data_sem, 0, 0) < 0)
            {
                return_code = OS_SEM_FAILURE;
            }
            else
            {
                local_arg.value = local_id;
                return_code = OS_Posix_InternalTaskCreate_Impl(&consoletask, OS_CONSOLE_TASK_PRIORITY, 0,
                    OS_ConsoleTask_Entry, local_arg.opaque_arg);

                if (return_code != OS_SUCCESS)
                {
                    sem_destroy(&OS_impl_console_table[local_id].data_sem);
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

