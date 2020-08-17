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
** File:  cfe_psp_support.c
**
** Purpose:
**   This file contains glue routines between the cFE and the OS Board Support Package ( BSP ).
**   The functions here allow the cFE to interface functions that are board and OS specific
**   and usually dont fit well in the OS abstraction layer.
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "common_types.h"
#include "osapi.h"

/*
** Types and prototypes for this module
*/
#include "cfe_psp.h"
#include "cfe_psp_config.h"
#include "cfe_psp_memory.h"


/*
** External Variables
*/
extern uint32  CFE_PSP_SpacecraftId;
extern uint32  CFE_PSP_CpuId;


/******************************************************************************
**  Function:  CFE_PSP_Restart()
**
**  Purpose:
**    Provides a common interface to the processor reset.
**
**  Arguments:
**    reset_type  : Type of reset.
**
**  Return:
**    (none)
*/

void CFE_PSP_Restart(uint32 reset_type)
{
   if ( reset_type == CFE_PSP_RST_TYPE_POWERON )
   {
       OS_printf("CFE_PSP: Exiting cFE with POWERON Reset status.\n");

       /* Also delete the SHM segments, so they will be recreated on next boot */
       /* Deleting these memories will unlink them, but active references should still work */
       CFE_PSP_DeleteProcessorReservedMemory();
   }
   else
   {
       OS_printf("CFE_PSP: Exiting cFE with PROCESSOR Reset status.\n");
   }

   /*
    * Record the reset type for the next boot.
    */
   CFE_PSP_ReservedMemoryMap.BootPtr->NextResetType = reset_type;
   CFE_PSP_ReservedMemoryMap.BootPtr->ValidityFlag = CFE_PSP_BOOTRECORD_VALID;

   /*
    * Begin process of orderly shutdown.
    *
    * This should cause the original thread (main task) to wake up
    * and start the shutdown procedure.
    */
   CFE_PSP_IdleTaskState.ShutdownReq = true;
   pthread_kill(CFE_PSP_IdleTaskState.ThreadID, CFE_PSP_EXCEPTION_EVENT_SIGNAL);

   /*
    * Give time for the orderly shutdown to occur.
    *
    * Normally during shutdown this task will be deleted, and therefore
    * this does not return.
    *
    * However, if problems occur (e.g. deadlock) eventually this timeout
    * will expire and return.
    */
   OS_TaskDelay(CFE_PSP_RESTART_DELAY);

   /*
    * Timeout expired without this task being deleted, so abort().
    *
    * This should generate a core file to reveal what went wrong
    * with normal shutdown.
    */
   abort();

}

/******************************************************************************
**  Function:  CFE_PSP_Panic()
**
**  Purpose:
**    Provides a common interface to abort the cFE startup process and return
**    back to the OS.
**
**  Arguments:
**    ErrorCode  : Reason for Exiting.
**
**  Return:
**    (none)
*/

void CFE_PSP_Panic(int32 ErrorCode)
{
   OS_printf("CFE_PSP_Panic Called with error code = 0x%08X. Exiting.\n",(unsigned int)ErrorCode);
   OS_printf("The cFE could not start.\n");
   abort(); /* abort() is preferable to exit(-1), as it may create a core file for debug */
}


/******************************************************************************
**  Function:  CFE_PSP_FlushCaches)
**
**  Purpose:
**    Provides a common interface to flush the processor caches. This routine
**    is in the BSP because it is sometimes implemented in hardware and
**    sometimes taken care of by the RTOS.
**
**  Arguments:
**
**  Return:
**    (none)
*/
void CFE_PSP_FlushCaches(uint32 type, void* address, uint32 size)
{
   printf("CFE_PSP_FlushCaches called -- Currently no Linux/OSX/Cygwin implementation\n");
}

/*
** Name: CFE_PSP_GetProcessorId
**
** Purpose:
**         return the processor ID.
**
**
** Parameters:
**
** Global Inputs: None
**
** Global Outputs: None
**
**
**
** Return Values: Processor ID
*/
uint32 CFE_PSP_GetProcessorId    (void)
{
    return(CFE_PSP_CpuId);
}


/*
** Name: CFE_PSP_GetSpacecraftId
**
** Purpose:
**         return the spacecraft ID.
**
** Parameters:
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values: Spacecraft ID
*/
uint32 CFE_PSP_GetSpacecraftId   (void)
{
   return(CFE_PSP_SpacecraftId);
}
