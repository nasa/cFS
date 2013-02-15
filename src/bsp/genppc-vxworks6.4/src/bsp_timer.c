/************************************************************************************************
** File:  bsp_timer.c
**
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement. 
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. 
**
**
** Purpose:
**   This file contains glue routines between the OSAL and the OS Board Support Package ( BSP ).
**   The functions here allow the OSAL to interface functions that are board and OS specific
**   and usually dont fit well in the OS abstraction layer.
**
** History:
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
#include <unistd.h>
#include "vxWorks.h"
#include "sysLib.h"
#include "vxLib.h"
#include "taskLib.h"
/*
** OSAL includes
*/
#include "common_types.h"
#include "osapi.h"


/******************* Macro Definitions ***********************/
#define WATCHDOG_MIN_TIME   0
#define WATCHDOG_MAX_TIME   0xFFFFFFFF

#define OS_BSP_TIMER_TICKS_PER_SECOND       16666666    /* Resolution of the least significant 32 bits of the 64 bit
                                                           time stamp returned by OS_BSPGet_Timebase in timer ticks per second.
                                                           The timer resolution for accuracy should not be any slower than 1000000
                                                           ticks per second or 1 us per tick */
#define OS_BSP_TIMER_LOW32_ROLLOVER         0           /* The number that the least significant 32 bits of the 64 bit
                                                           time stamp returned by OS_BSPGet_Timebase rolls over.  If the lower 32
                                                           bits rolls at 1 second, then the OS_BSP_TIMER_LOW32_ROLLOVER will be 1000000.
                                                           if the lower 32 bits rolls at its maximum value (2^32) then
                                                           OS_BSP_TIMER_LOW32_ROLLOVER will be 0. */
/*
** MCP750/vxWorks timers
*/
/******************************************************************************
**  Function:  OS_BSPGetTime()
**
**  Purpose: Gets the value of the time from the hardware
**
**  Arguments: LocalTime - where the time is returned through
******************************************************************************/

void OS_BSPGetTime( OS_time_t *LocalTime)
{

    /* 
    ** Reads the time from the hardware register, then converts it
    ** into usable seconds and microseconds 
    */
    LocalTime->seconds = 0;
    LocalTime->microsecs = 0;
    
}/* end OS_BSPGetLocalTime */

/******************************************************************************
**  Function:  OS_BSPGet_Timer_Tick()
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
uint32 OS_BSPGet_Timer_Tick(void)
{
   return (sysClkRateGet());
}

/******************************************************************************
**  Function:  OS_BSPGetTimerTicksPerSecond()
**
**  Purpose:
**    Provides the resolution of the least significant 32 bits of the 64 bit
**    time stamp returned by OS_BSPGet_Timebase in timer ticks per second.
**    The timer resolution for accuracy should not be any slower than 1000000
**    ticks per second or 1 us per tick
**
**  Arguments:
**
**  Return:
**    The number of timer ticks per second of the time stamp returned
**    by OS_BSPGet_Timebase
*/
uint32 OS_BSPGetTimerTicksPerSecond(void)
{
    return(OS_BSP_TIMER_TICKS_PER_SECOND);
}

/******************************************************************************
**  Function:  OS_BSPGetTimerLow32Rollover()
**
**  Purpose:
**    Provides the number that the least significant 32 bits of the 64 bit
**    time stamp returned by OS_BSPGet_Timebase rolls over.  If the lower 32
**    bits rolls at 1 second, then the OS_BSP_TIMER_LOW32_ROLLOVER will be 1000000.
**    if the lower 32 bits rolls at its maximum value (2^32) then
**    OS_BSP_TIMER_LOW32_ROLLOVER will be 0.
**
**  Arguments:
**
**  Return:
**    The number that the least significant 32 bits of the 64 bit time stamp
**    returned by OS_BSPGet_Timebase rolls over.
*/
uint32 OS_BSPGetTimerLow32Rollover(void)
{
    return(OS_BSP_TIMER_LOW32_ROLLOVER);
}

/******************************************************************************
**  Function:  OS_BSPGet_Timebase()
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
void OS_BSPGet_Timebase(uint32 *Tbu, uint32* Tbl)
{
   vxTimeBaseGet((UINT32 *)Tbu, (UINT32 *)Tbl);
}

/******************************************************************************
**  Function:  OS_BSPGet_Dec()
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

uint32 OS_BSPGet_Dec(void)
{
   return(vxDecGet());
}


/******************************************************************************
**  Function:  OS_BSPWatchdogInit()
**
**  Purpose:
**    To setup the timer resolution and/or other settings custom to this platform.
**
**  Arguments:
**
**  Return:
*/
void OS_BSPWatchdogInit(void)
{

}

/******************************************************************************
**  Function:  OS_BSPWatchdogEnable()
**
**  Purpose:
**    Enable the watchdog timer
**
**  Arguments:
**
**  Return:
*/
void OS_BSPWatchdogEnable(void)
{

  return;
}

/******************************************************************************
**  Function:  OS_BSPWatchdogDisable()
**
**  Purpose:
**    Disable the watchdog timer
**
**  Arguments:
**
**  Return:
*/
void OS_BSPWatchdogDisable(void)
{

   return;
}

/******************************************************************************
**  Function:  OS_BSPWatchdogService()
**
**  Purpose:
**    Load the watchdog timer with a count that corresponds to the millisecond
**    time given in the parameter.
**
**  Arguments:
**    ExpireTime - The time in milliseconds that the counter will expire
**
**  Return:
**    None
**
**  Note:
**    Currently an ExpireTime value of zero will result in the minimum reset time
**    of 4.5 seconds. All other ExpireTime values will result in a reset time of
**    5.5 seconds. See comments below.
*/
void OS_BSPWatchdogService(uint32 ExpireTime)
{
    return;
    
}
