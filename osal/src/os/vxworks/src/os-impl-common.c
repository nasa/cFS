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
 * \file     os-impl-common.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include "os-shared-common.h"

#include <errnoLib.h>
#include <objLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <taskLib.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/


static TASK_ID     OS_idle_task_id;


/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_API_Impl_Init
 *
 *  Purpose: Initialize the tables that the OS API uses to keep track of information
 *           about objects
 *
 *-----------------------------------------------------------------*/
int32 OS_API_Impl_Init(uint32 idtype)
{
    int32    return_code;

    return_code = OS_VxWorks_TableMutex_Init(idtype);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    switch(idtype)
    {
    case OS_OBJECT_TYPE_OS_TASK:
        return_code = OS_VxWorks_TaskAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_QUEUE:
        return_code = OS_VxWorks_QueueAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_BINSEM:
        return_code = OS_VxWorks_BinSemAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_COUNTSEM:
        return_code = OS_VxWorks_CountSemAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_MUTEX:
        return_code = OS_VxWorks_MutexAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_MODULE:
        return_code = OS_VxWorks_ModuleAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_TIMEBASE:
        return_code = OS_VxWorks_TimeBaseAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_STREAM:
        return_code = OS_VxWorks_StreamAPI_Impl_Init();
        break;
    case OS_OBJECT_TYPE_OS_DIR:
        return_code = OS_VxWorks_DirAPI_Impl_Init();
        break;
    default:
        break;
    }

    return(return_code);
} /* end OS_API_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_IdleLoop_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_IdleLoop_Impl(void)
{
    TASK_ID tid = taskIdSelf();
    OS_idle_task_id = tid;
    taskSuspend(tid);
} /* end OS_IdleLoop_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_ApplicationShutdown_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_ApplicationShutdown_Impl(void)
{
    taskResume(OS_idle_task_id);
} /* end OS_ApplicationShutdown_Impl */




/****************************************************************************************
                              GENERIC SEMAPHORE API
****************************************************************************************/

/*
 * ----------------------------------
 * generic semaphore give/take -
 * VxWorks uses the sem semTake()/semGive() API for all types of semaphores.
 * Only the initialization is different between them.
 * Therefore all semaphore actions can just invoke these generic actions
 * -----------------------------------
 */

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_GenericSemGive
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_GenericSemGive(SEM_ID vxid)
{
    /* Give VxWorks Semaphore */
    if(semGive(vxid) != OK)
    {
        OS_DEBUG("semGive() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }
    return OS_SUCCESS;
} /* end OS_VxWorks_GenericSemGive */


/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_GenericSemTake
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_GenericSemTake(SEM_ID vxid, int sys_ticks)
{
    int vx_status;

    /* Take VxWorks Semaphore */
    vx_status = semTake(vxid, sys_ticks);
    if (vx_status != OK)
    {
        /*
         * check for the timeout condition,
         * which has a different return code and
         * not necessarily an error of concern.
         */
        if (errno == S_objLib_OBJ_TIMEOUT)
        {
           return OS_SEM_TIMEOUT;
        }

        OS_DEBUG("semTake() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
} /* end OS_VxWorks_GenericSemTake */

