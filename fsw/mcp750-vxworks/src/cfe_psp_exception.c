/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/******************************************************************************
**
** File:  cfe_psp_exception.c
**
**      MCP750 vxWorks 6.2 Version
**
** Purpose:
**   cFE PSP Exception related functions.
**
** History:
**   2007/05/29  A. Cudmore      | vxWorks 6.2 MCP750 version
**   2016/04/07  M.Grubb         | Updated for PSP version 1.3
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <string.h>
#include <vxWorks.h>
#include <sysLib.h>
#include "fppLib.h"
#include "excLib.h"
#include "taskLib.h"
#include "arch/ppc/vxPpcLib.h"
#include "arch/ppc/esfPpc.h"

/*
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"

#include "cfe_psp.h"
#include "cfe_psp_config.h"
#include "cfe_psp_exceptionstorage_types.h"
#include "cfe_psp_exceptionstorage_api.h"
#include "cfe_psp_memory.h"

#include <target_config.h>

/*
**
** LOCAL FUNCTION PROTOTYPES
**
*/

void CFE_PSP_ExceptionHook ( TASK_ID task_id, int vector, void* vpEsf );


/***************************************************************************
 **                        FUNCTIONS DEFINITIONS
 ***************************************************************************/

/*
**
**   Name: CFE_PSP_AttachExceptions
**
**   Purpose: This function Initializes the task execptions and adds a hook
**              into the VxWorks exception handling.  The below hook is called
**              for every exception that VxWorks catches.
**
**   Notes: if desired - to attach a custom handler put following code in
**          this function:  excConnect ((VOIDFUNCPTR*)VECTOR, ExceptionHandler);
**
*/

void CFE_PSP_AttachExceptions(void)
{
    excHookAdd( CFE_PSP_ExceptionHook );
    OS_printf("CFE_PSP: Attached cFE Exception Handler. Context Size = %lu bytes.\n",(unsigned long)sizeof(CFE_PSP_Exception_ContextDataEntry_t));
    CFE_PSP_Exception_Reset();
}


/*
** Name: CFE_PSP_ExceptionHook
**
** Purpose: Make the proper call to CFE_ES_EXCEPTION_FUNCTION (defined in
**          cfe_es_platform.cfg)
**
** Notes:   pEsf - pointer to exception stack frame.
**          fppSave - When it makes this call, it captures the last floating
**                      point context - which may not be valid.  If a floating
**                      point exception occurs you can be almost 100% sure
**                      that this will reflect the proper context.  But if another
**                      type of exception occurred then this has the possibility
**                      of not being valid.  Specifically if a task that is not
**                      enabled for floating point causes a non-floating point
**                      exception, then the meaning of the floating point context
**                      will not be valid.  If the task is enabled for floating point,
**                      then it will be valid.
**
*/
void CFE_PSP_ExceptionHook (TASK_ID task_id, int vector, void* vpEsf )
{
    CFE_PSP_Exception_LogData_t* Buffer;

    Buffer = CFE_PSP_Exception_GetNextContextBuffer();
    if (Buffer != NULL)
    {
        /*
         * Immediately get a snapshot of the timebase when exception occurred
         *
         * This is because the remainder of exception processing might be done
         * in a cleanup job as a low priority background task, and might be
         * considerably delayed from the time the actual exception occurred.
         */
        vxTimeBaseGet(&Buffer->context_info.timebase_upper, &Buffer->context_info.timebase_lower);

        Buffer->sys_task_id = task_id;
        Buffer->context_info.vector = vector;

        /*
         * Save Exception Stack frame
         */
        memcpy(&Buffer->context_info.esf, vpEsf, sizeof(Buffer->context_info.esf));

        /*
         * Save floating point registers
         */
        fppSave(&Buffer->context_info.fp);

        /*
         * Save total size of context info.
         * (This PSP always fills the entire structure)
         */
        Buffer->context_size = sizeof(Buffer->context_info);

        CFE_PSP_Exception_WriteComplete();
    }

    if (GLOBAL_CFE_CONFIGDATA.SystemNotify != NULL)
    {
        /* notify the CFE of the event */
        GLOBAL_CFE_CONFIGDATA.SystemNotify();
    }


} /* end function */


/*
**
**   Name: CFE_PSP_SetDefaultExceptionEnvironment
**
**   Purpose: This function sets a default exception environment that can be used
**
**   Notes: The exception environment is local to each task Therefore this must be
**          called for each task that that wants to do floating point and catch exceptions
**          Currently, this is automatically called from OS_TaskRegister for every task
*/
void CFE_PSP_SetDefaultExceptionEnvironment(void)
{
    vxMsrSet( vxMsrGet()                |
              _PPC_MSR_EE               |   /* enable the external interrupt */
              _PPC_MSR_FP               |   /* enable floating point */
              _PPC_MSR_ME               |   /* major hardware failures */
              _PPC_MSR_FE0              |   /* floating point exception 0 */
              _PPC_MSR_FE1              |   /* generate unrecoverable floating point exceptions */
              _PPC_MSR_DR               );  /* enable data address translation (dbats?) */

    vxFpscrSet(  vxFpscrGet()               |
                 _PPC_FPSCR_VE              |       /* enable exceptions for invalid operations */
                 _PPC_FPSCR_OE              |       /* enable overflow exceptions */
                 _PPC_FPSCR_NI              |       /* Non-IEEE mode for denormailized numbers */
                 _PPC_FPSCR_ZE              );  /* enable divide by zero exceptions */

    vxFpscrSet(  vxFpscrGet()               |
                 _PPC_FPSCR_XE              |  /* fp inexact exc enable */
                 _PPC_FPSCR_UE              ); /* fp underflow enable */
}

/*
 * Name: CFE_PSP_ExceptionGetSummary_Impl
 *
 * Purpose: Translate a stored exception log entry into a summary string
 */
int32 CFE_PSP_ExceptionGetSummary_Impl(const CFE_PSP_Exception_LogData_t* Buffer, char *ReasonBuf, uint32 ReasonSize)
{
    const char *TaskName;

    /*
    ** Get the vxWorks task name
    */
    TaskName = taskName(Buffer->sys_task_id);

    if ( TaskName == NULL )
    {
        TaskName = "NULL";
    }

    snprintf(ReasonBuf, ReasonSize, "Vector=0x%06X, vxWorks Task Name=%s, Task ID=0x%08X",
            Buffer->context_info.vector,TaskName,Buffer->sys_task_id);

    return CFE_PSP_SUCCESS;
}


