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
#include "cfe_psp_exceptionstorage_types.h"
#include "cfe_psp_exceptionstorage_api.h"

/*
**
** LOCAL FUNCTION PROTOTYPES
**
*/


/***************************************************************************
 **                        FUNCTIONS DEFINITIONS
 ***************************************************************************/

/*
**
**   Name: CFE_PSP_AttachExceptions
** 
**   Purpose: No-op on this platform, implemented for API compatibility.
**
*/
void CFE_PSP_AttachExceptions(void)
{
    OS_printf("CFE_PSP: CFE_PSP_AttachExceptions Called\n");
    CFE_PSP_Exception_Reset();
}

/*
 * Name: CFE_PSP_ExceptionGetSummary_Impl
 *
 * Purpose: Translate a stored exception log entry into a summary string
 */
int32 CFE_PSP_ExceptionGetSummary_Impl(const CFE_PSP_Exception_LogData_t* Buffer, char *ReasonBuf, uint32 ReasonSize)
{
    /* exceptions are not yet implemented on this platform */
    return CFE_PSP_ERROR_NOT_IMPLEMENTED;
}



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

