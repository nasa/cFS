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
** Purpose:
**   This file contains glue routines between the app and the OS Board Support Package ( BSP ).
**   The functions here allow the app to interface functions that are board and OS specific
**   and usually dont fit well in the OS abstraction layer.
**
** History:
**
*************************************************************************************************/

/*
**  Include Files
*/


/*
** OSAL includes
*/
#include "common_types.h"
#include "osapi.h"

/*
**  System Include Files
*/
#include <stdio.h>
#include <stdlib.h>

/*
** Types and prototypes for this module
*/


/******************* Macro Definitions ***********************/

#define OS_BSP_TIMER_TICKS_PER_SECOND       1000000     /* Resolution of the least significant 32 bits of the 64 bit
                                                           time stamp returned by OS_BSPGet_Timebase in timer ticks per second.
                                                           The timer resolution for accuracy should not be any slower than 1000000
                                                           ticks per second or 1 us per tick */
#define OS_BSP_TIMER_LOW32_ROLLOVER         1000000     /* The number that the least significant 32 bits of the 64 bit
                                                           time stamp returned by OS_BSPGet_Timebase rolls over.  If the lower 32
                                                           bits rolls at 1 second, then the OS_BSP_TIMER_LOW32_ROLLOVER will be 1000000.
                                                           if the lower 32 bits rolls at its maximum value (2^32) then
                                                           OS_BSP_TIMER_LOW32_ROLLOVER will be 0. */

/******************************************************************************
**  Function:  OS_BSPGetTime()
**
**  Purpose: Gets the value of the time from the hardware
**
**  Arguments: LocalTime - where the time is returned through
******************************************************************************/

void OS_BSPGetTime( OS_time_t *LocalTime)
{

    /* since we don't have a hardware register to access like the mcp750,
     * we use a call to the OS to get the time */

    OS_GetLocalTime(LocalTime);

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
   /* SUB -add function call code*/
   return (0);
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
   OS_time_t        time;

   OS_GetLocalTime(&time);
   *Tbu = time.seconds;
   *Tbl = time.microsecs;
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
   /* SUB -add function call code*/
   return(0);
}

/**  Function:  OS_BSPWatchdogInit()
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
**  Notes:
**
*/
void OS_BSPWatchdogService(uint32 ExpireTime)
{


}
