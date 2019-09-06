/*
**  GSC-18128-1, "Core Flight Executive Version 6.6"
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
**      m5235bcc Coldfire RTEMS Version
**
** Purpose:
**   cFE BSP Exception related functions.  
**
** History:
**   2007/09/23  A. Cudmore      | RTEMS Coldfire m5235bcc version
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <string.h>

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"

#include "cfe_psp.h" 
#include "cfe_psp_config.h"

#include "cfe_psp_memory.h"           

/*
** Types and prototypes for this module
*/


/*
** BSP Specific defines
*/

/*
**  External Declarations
*/
                                                                                
/*
** Global variables
*/

CFE_PSP_ExceptionContext_t CFE_PSP_ExceptionContext;
char                       CFE_PSP_ExceptionReasonString[256];

/*
**
** IMPORTED FUNCTIONS
**
*/

void CFE_ES_ProcessCoreException(uint32  HostTaskId,     uint8 *ReasonString, 
                                 uint32 *ContextPointer, uint32 ContextSize);                                   
                                   
/*
**
** LOCAL FUNCTION PROTOTYPES
**
*/

void CFE_PSP_ExceptionHook (int task_id, int vector, int32 *pEsf );

/***************************************************************************
 **                        FUNCTIONS DEFINITIONS
 ***************************************************************************/

/*
**
**   Name: OS_BSPAttachExceptions
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
  OS_printf("CFE PSP: Attached cFE Exception Handler. Context Size = %d bytes.\n",(int)CFE_PSP_CPU_CONTEXT_SIZE);
}


/*
** Name: CFE_PSP_ExceptionHook
**
** Purpose: Make the proper call to CFE_ES_ProcessCoreException 
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
void CFE_PSP_ExceptionHook (int task_id, int vector, int32 *pEsf )
{

    char TaskName[16];
    
    /*
    ** Get the task name
    */
    strncpy(TaskName, "TBD", 16);
    
    if ( TaskName == NULL )
    {
       sprintf(CFE_PSP_ExceptionReasonString, "Exception: Vector=0x%06X, vxWorks Task Name=NULL, Task ID=0x%08X", 
               vector,task_id);
    }
    else
    {
       sprintf(CFE_PSP_ExceptionReasonString, "Exception: Vector=0x%06X, vxWorks Task Name=%s, Task ID=0x%08X", 
                vector, TaskName, task_id);
    }
    
    /* 
    ** Save Exception Stack frame 
    */
    memcpy(&(CFE_PSP_ExceptionContext.registers), pEsf, sizeof(CFE_PSP_ExceptionContext_t));

    /*
    ** Save floating point registers
    */
   /* fppSave(&OS_ExceptionContext.fp); */

    /*
    ** Call the Generic cFE routine to finish processing the exception and 
    ** restart the cFE
    */
    CFE_ES_ProcessCoreException((uint32)task_id, (uint8 *)CFE_PSP_ExceptionReasonString, 
                                (uint32 *)&CFE_PSP_ExceptionContext, sizeof(CFE_PSP_ExceptionContext_t));

    /*
    ** No return to here 
    */
    
} /* end function */


/*
**
**   Name: CFE_PSP_SetDefaultExceptionEnvironment
**
**   Purpose: This function sets a default exception environment that can be used
**
**   Notes: The exception environment is local to each task Therefore this must be
**          called for each task that that wants to do floating point and catch exceptions
**          Currently, this is automaticall called from OS_TaskRegister for every task
*/
void CFE_PSP_SetDefaultExceptionEnvironment(void)
{
	 return;
}

