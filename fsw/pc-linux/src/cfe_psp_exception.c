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
S
** File:  cfe_psp_exception.c
**
**      POSIX ( Mac OS X, Linux, Cygwin ) version
**
** Purpose:
**   cFE PSP Exception handling functions
**
** History:
**   2007/05/29  A. Cudmore      | POSIX Version
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

#include <target_config.h>

#define CFE_PSP_ES_EXCEPTION_FUNCTION   (*GLOBAL_CONFIGDATA.CfeConfig->SystemExceptionISR)

/*
** Types and prototypes for this module
*/

/*
** PSP Specific defines
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
** Local Function Prototypes
**
*/

void CFE_PSP_ExceptionHook ( int task_id, int vector, uint8 *pEsf );


/***************************************************************************
 **                        FUNCTION DEFINITIONS
 ***************************************************************************/

/*
**
**   Name: CFE_PSP_AttachExceptions
**
**   Purpose: This function Initializes the task execptions and adds a hook
**              into the system exception handling.
**
*/

void CFE_PSP_AttachExceptions(void)
{
   OS_printf("CFE_PSP: CFE_PSP_AttachExceptions Called\n");

}


/*
** Name: CFE_PSP_ExceptionHook
**
** Purpose: Make the proper call to CFE_PSP_ES_EXCEPTION_FUNCTION (defined in
**          cfe_es_platform.cfg)
**
*/
void CFE_PSP_ExceptionHook (int task_id, int vector, uint8 *pEsf )
{

    sprintf(CFE_PSP_ExceptionReasonString, "Processor Exception %d, task ID %d", vector, task_id);

    /*
    ** Save Exception Stack frame
    */
    memcpy(&(CFE_PSP_ExceptionContext.regs), pEsf, (32*4));

    /*
    ** Call the Generic cFE routine to finish processing the exception and
    ** restart the cFE
    */
    CFE_PSP_ES_EXCEPTION_FUNCTION((uint32)task_id, CFE_PSP_ExceptionReasonString,
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
