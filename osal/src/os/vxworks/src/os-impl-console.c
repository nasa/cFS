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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-console.h"

#include "os-shared-printf.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/
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
****************************************************************************************/

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
        if(semGive(local->datasem) == ERROR)
        {
            OS_DEBUG("semGive() - vxWorks errno %d\n",errno);
        }
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
int OS_VxWorks_ConsoleTask_Entry(int arg)
{
    uint32 local_id = arg;
    OS_impl_console_internal_record_t *local;

    local = &OS_impl_console_table[local_id];
    while (true)
    {
        OS_ConsoleOutput_Impl(local_id);
        if(semTake(local->datasem, WAIT_FOREVER) == ERROR)
        {
            OS_DEBUG("semTake() - vxWorks errno %d\n",errno);
            break;
        }
    }

    return OK;
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

    if (local_id == 0)
    {
        return_code = OS_SUCCESS;
        local->is_async = OS_CONSOLE_ASYNC;

        if (local->is_async)
        {
            OS_DEBUG("%s(): Starting Async Console Handler\n", __func__);

            /* Initialize VxWorks Semaphore.
             * The memory for this sem is statically allocated. */
            local->datasem = semCInitialize(local->cmem, SEM_Q_PRIORITY, 0);

            /* check if semCInitialize failed */
            if(local->datasem == (SEM_ID)0)
            {
                OS_DEBUG("semCInitialize() - vxWorks errno %d\n",errno);
                return OS_SEM_FAILURE;
            }

            /* spawn the async output helper task */
            local->taskid = taskSpawn(OS_console_table[local_id].device_name,
                    OS_CONSOLE_TASK_PRIORITY,
                    0,
                    OS_CONSOLE_TASK_STACKSIZE ,
                    (FUNCPTR)OS_VxWorks_ConsoleTask_Entry,
                    local_id,0,0,0,0,0,0,0,0,0);

            if (local->taskid == (TASK_ID)ERROR)
            {
                OS_DEBUG("taskSpawn() - vxWorks errno %d\n",errno);
                return_code = OS_ERROR;
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



