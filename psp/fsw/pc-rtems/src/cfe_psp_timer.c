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

/************************************************************************************************
** File:  cfe_psp_timer.c
**
** Purpose:
**   This file contains glue routines between the cFE and the OS Board Support Package ( BSP ).
**   The functions here allow the cFE to interface functions that are board and OS specific
**   and usually dont fit well in the OS abstraction layer.
**
** History:
**   2005/06/05  K.Audra    | Initial version,
**
*************************************************************************************************/

/*
**  Include Files
*/

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>

/*
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"

/*
** Types and prototypes for this module
*/
#include "cfe_psp.h"


/******************* Macro Definitions ***********************/
#define WATCHDOG_MIN_TIME   0
#define WATCHDOG_MAX_TIME   0xFFFFFFFF

#define CFE_PSP_TIMER_TICKS_PER_SECOND       100    /* Resolution of the least significant 32 bits of the 64 bit
                                                           time stamp returned by CFE_PSP_Get_Timebase in timer ticks per second.
                                                           The timer resolution for accuracy should not be any slower than 1000000
                                                           ticks per second or 1 us per tick */
#define CFE_PSP_TIMER_LOW32_ROLLOVER         0           /* The number that the least significant 32 bits of the 64 bit
                                                           time stamp returned by CFE_PSP_Get_Timebase rolls over.  If the lower 32
                                                           bits rolls at 1 second, then the CFE_PSP_TIMER_LOW32_ROLLOVER will be 1000000.
                                                           if the lower 32 bits rolls at its maximum value (2^32) then
                                                           CFE_PSP_TIMER_LOW32_ROLLOVER will be 0. */

/******************************************************************************
**  Function:  CFE_PSP_GetTime()
**
**  Purpose: Gets the value of the time from the hardware
**
**  Arguments: LocalTime - where the time is returned through
******************************************************************************/

void CFE_PSP_GetTime( OS_time_t *LocalTime)
{
    OS_GetLocalTime(LocalTime);
    
}/* end CFE_PSP_GetLocalTime */

/******************************************************************************
**  Function:  CFE_PSP_Get_Timer_Tick()
**
**  Purpose:
**    Provides a common interface to system clock tick. This routine
**    is in the BSP because it is sometimes implemented in hardware and
**    sometimes taken care of by the RTOS.
**
**  Arguments:
**
**  Return:
**  OS system clock ticks per second
*/
uint32 CFE_PSP_Get_Timer_Tick(void)
{
   return (0);
}

/******************************************************************************
**  Function:  CFE_PSP_GetTimerTicksPerSecond()
**
**  Purpose:
**    Provides the resolution of the least significant 32 bits of the 64 bit
**    time stamp returned by CFE_PSP_Get_Timebase in timer ticks per second.
**    The timer resolution for accuracy should not be any slower than 1000000
**    ticks per second or 1 us per tick
**
**  Arguments:
**
**  Return:
**    The number of timer ticks per second of the time stamp returned
**    by CFE_PSP_Get_Timebase
*/
uint32 CFE_PSP_GetTimerTicksPerSecond(void)
{
    return(CFE_PSP_TIMER_TICKS_PER_SECOND);
}

/******************************************************************************
**  Function:  CFE_PSP_GetTimerLow32Rollover()
**
**  Purpose:
**    Provides the number that the least significant 32 bits of the 64 bit
**    time stamp returned by CFE_PSP_Get_Timebase rolls over.  If the lower 32
**    bits rolls at 1 second, then the CFE_PSP_TIMER_LOW32_ROLLOVER will be 1000000.
**    if the lower 32 bits rolls at its maximum value (2^32) then
**    CFE_PSP_TIMER_LOW32_ROLLOVER will be 0.
**
**  Arguments:
**
**  Return:
**    The number that the least significant 32 bits of the 64 bit time stamp
**    returned by CFE_PSP_Get_Timebase rolls over.
*/
uint32 CFE_PSP_GetTimerLow32Rollover(void)
{
    return(CFE_PSP_TIMER_LOW32_ROLLOVER);
}

/******************************************************************************
**  Function:  CFE_PSP_Get_Timebase()
**
**  Purpose:
**    Provides a common interface to system timebase. This routine
**    is in the BSP because it is sometimes implemented in hardware and
**    sometimes taken care of by the RTOS.
**
**  Arguments:
**
**  Return:
**  Timebase register value
*/
void CFE_PSP_Get_Timebase(uint32 *Tbu, uint32* Tbl)
{
   OS_time_t        time;

   OS_GetLocalTime(&time);
   *Tbu = time.seconds;
   *Tbl = time.microsecs;
}

/******************************************************************************
**  Function:  CFE_PSP_Get_Dec()
**
**  Purpose:
**    Provides a common interface to decrementer counter. This routine
**    is in the BSP because it is sometimes implemented in hardware and
**    sometimes taken care of by the RTOS.
**
**  Arguments:
**
**  Return:
**  Timebase register value
*/

uint32 CFE_PSP_Get_Dec(void)
{
   return(0);
}

