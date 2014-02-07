/*
** File   : ostimer.c
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Alan Cudmore
**
** Purpose: This file contains the OSAL Timer API for RTEMS
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#define _USING_RTEMS_INCLUDES_

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

#include <rtems.h>

#include "common_types.h"
#include "osapi.h"
/****************************************************************************************
                                EXTERNAL FUNCTION PROTOTYPES
****************************************************************************************/

uint32 OS_FindCreator(void);

/****************************************************************************************
                                INTERNAL FUNCTION PROTOTYPES
****************************************************************************************/

void  OS_TicksToUsecs(rtems_interval ticks, uint32 *usecs);
void  OS_UsecsToTicks(uint32 usecs, rtems_interval *ticks);

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

#define UNINITIALIZED 0

#define OSAL_TABLE_MUTEX_ATTRIBS \
 (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | \
  RTEMS_INHERIT_PRIORITY | RTEMS_NO_PRIORITY_CEILING | RTEMS_LOCAL)

/****************************************************************************************
                                    LOCAL TYPEDEFS 
****************************************************************************************/

typedef struct 
{
   uint32              free;
   char                name[OS_MAX_API_NAME];
   uint32              creator;
   uint32              start_time;
   uint32              interval_time;
   uint32              accuracy;
   OS_TimerCallback_t  callback_ptr;
   rtems_id            host_timerid;

} OS_timer_record_t;

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

OS_timer_record_t OS_timer_table[OS_MAX_TIMERS];
uint32            os_clock_accuracy;

/*
** The Mutex for protecting the above table
*/
rtems_id          OS_timer_table_sem;

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/
int32  OS_TimerAPIInit ( void )
{
   int               i;
   int32             return_code = OS_SUCCESS;
   rtems_status_code rtems_sc;
   
   /*
   ** Mark all timers as available
   */
   for ( i = 0; i < OS_MAX_TIMERS; i++ )
   {
      OS_timer_table[i].free      = TRUE;
      OS_timer_table[i].creator   = UNINITIALIZED;
      strcpy(OS_timer_table[i].name,"");
   }
	
   /*
   ** Store the clock accuracy for 1 tick.
   */
   OS_TicksToUsecs(1, &os_clock_accuracy);

   /*
   ** Create the Timer Table semaphore
   */
   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', '6'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_timer_table_sem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return_code = OS_ERROR;
   }
   
   return(return_code);
   
}

/****************************************************************************************
                                INTERNAL FUNCTIONS
****************************************************************************************/

/*
** Timer Signal Handler.
** The purpose of this function is to convert the POSIX signal number to the 
** OSAL signal number and pass it to the User defined signal handler.
*/
rtems_timer_service_routine OS_TimerSignalHandler(rtems_id rtems_timer_id, void *user_data)
{
   rtems_status_code  status;
   uint32             osal_timer_id;
   rtems_interval     timeout;
      
   osal_timer_id = ( uint32 )user_data;
   
   if ( osal_timer_id  < OS_MAX_TIMERS )
   {
      if ( OS_timer_table[osal_timer_id].free == FALSE )
      {
         /*
         ** Call the user function
         */
         (OS_timer_table[osal_timer_id].callback_ptr)(osal_timer_id);
        
         /*
         ** Only re-arm the timer if the interval time is greater than zero.
         */ 
         if ( OS_timer_table[osal_timer_id].interval_time > 0 )
         {     
            /*
            ** Reprogram the timer with the interval time
            */
            OS_UsecsToTicks(OS_timer_table[osal_timer_id].interval_time, &timeout);
   
            status = rtems_timer_fire_after(OS_timer_table[osal_timer_id].host_timerid, 
                                   timeout, 
                                   OS_TimerSignalHandler, (void *)osal_timer_id );
         }
      }
   }
}
 
/******************************************************************************
 **  Function:  OS_UsecToTicks
 **
 **  Purpose:  Convert Microseconds to a number of ticks.
 **
 */
void  OS_UsecsToTicks(uint32 usecs, rtems_interval *ticks)
{
   rtems_interval ticks_per_sec = rtems_clock_get_ticks_per_second(); 
   uint32         usecs_per_tick;
   
   usecs_per_tick = (1000000)/ticks_per_sec;

   if ( usecs < usecs_per_tick )
    {
       *ticks = 1;
    }
    else
    {
       *ticks = usecs / usecs_per_tick;
       /* Need to round up?? */ 
    }
	
}

/******************************************************************************
 **  Function:  OS_TicksToUsec
 **
 **  Purpose:  Convert a number of Ticks to microseconds
 **
 */
void  OS_TicksToUsecs(rtems_interval ticks, uint32 *usecs)
{
   rtems_interval ticks_per_sec = rtems_clock_get_ticks_per_second(); 
   uint32         usecs_per_tick;
   
   usecs_per_tick = (1000000)/ticks_per_sec;

   *usecs = ticks * usecs_per_tick;
}



/****************************************************************************************
                                   Timer API
****************************************************************************************/

/******************************************************************************
**  Function:  OS_TimerCreate
**
**  Purpose:  Create a new OSAL Timer
**
**  Arguments:
**
**  Return:
*/
int32 OS_TimerCreate(uint32 *timer_id,       const char         *timer_name, 
                     uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr)
{
   rtems_status_code status;
   rtems_name        RtemsTimerName;
   uint32            possible_tid;
   int32             i;

   if ( timer_id == NULL || timer_name == NULL || clock_accuracy == NULL )
   {
        return OS_INVALID_POINTER;
   }

   /* 
   ** we don't want to allow names too long
   ** if truncated, two names might be the same 
   */
   if (strlen(timer_name) > OS_MAX_API_NAME)
   {
      return OS_ERR_NAME_TOO_LONG;
   }

   /* 
   ** Check Parameters 
   */
   status = rtems_semaphore_obtain (OS_timer_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

   for(possible_tid = 0; possible_tid < OS_MAX_TIMERS; possible_tid++)
   {
      if (OS_timer_table[possible_tid].free == TRUE)
         break;
   }

   if( possible_tid >= OS_MAX_TIMERS || OS_timer_table[possible_tid].free != TRUE)
   {
        status = rtems_semaphore_release (OS_timer_table_sem);
        return OS_ERR_NO_FREE_IDS;
   }

   /* 
   ** Check to see if the name is already taken 
   */
   for (i = 0; i < OS_MAX_TIMERS; i++)
   {
       if ((OS_timer_table[i].free == FALSE) &&
            strcmp ((char*) timer_name, OS_timer_table[i].name) == 0)
       {
            status = rtems_semaphore_release (OS_timer_table_sem);
            return OS_ERR_NAME_TAKEN;
       }
   }

   /*
   ** Verify callback parameter
   */
   if (callback_ptr == NULL ) 
   {
      status = rtems_semaphore_release (OS_timer_table_sem);
      return OS_TIMER_ERR_INVALID_ARGS;
   }    

   /* 
   ** Set the possible timer Id to not free so that
   ** no other task can try to use it 
   */
   OS_timer_table[possible_tid].free = FALSE;
   status = rtems_semaphore_release (OS_timer_table_sem);

   OS_timer_table[possible_tid].creator = OS_FindCreator();
   strncpy(OS_timer_table[possible_tid].name, timer_name, OS_MAX_API_NAME);
   OS_timer_table[possible_tid].start_time = 0;
   OS_timer_table[possible_tid].interval_time = 0;
   OS_timer_table[possible_tid].callback_ptr = callback_ptr;
  
   /* 
   ** Create an interval timer
   */
   RtemsTimerName = rtems_build_name(timer_name[0],timer_name[1],timer_name[2],timer_name[3]);
   status = rtems_timer_create(RtemsTimerName, &(OS_timer_table[possible_tid].host_timerid));
   if ( status != RTEMS_SUCCESSFUL )
   {
      OS_timer_table[possible_tid].free = TRUE;
      return(OS_TIMER_ERR_UNAVAILABLE);
   }

   /*
   ** Return the clock accuracy to the user
   */
   *clock_accuracy = os_clock_accuracy;

   /*
   ** Return timer ID 
   */
   *timer_id = possible_tid;

   return OS_SUCCESS;
}

/******************************************************************************
**  Function:  OS_TimerSet
**
**  Purpose:  
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
   rtems_interval    timeout;
   rtems_status_code status;
   
   /* 
   ** Check to see if the timer_id given is valid 
   */
   if (timer_id >= OS_MAX_TIMERS || OS_timer_table[timer_id].free == TRUE)
   {
      return OS_ERR_INVALID_ID;
   }

   /*
   ** Round up the accuracy of the start time and interval times.
   ** Still want to preserve zero, since that has a special meaning. 
   */
   if (( start_time > 0 ) && (start_time < os_clock_accuracy))
   {
      start_time = os_clock_accuracy;
   }
 
   if ((interval_time > 0) && (interval_time < os_clock_accuracy ))
   {
      interval_time = os_clock_accuracy;
   }

   /*
   ** Save the start and interval times 
   */
   OS_timer_table[timer_id].start_time = start_time;
   OS_timer_table[timer_id].interval_time = interval_time;

   /*
   ** The defined behavior is to not arm the timer if the start time is zero
   ** If the interval time is zero, then the timer will not be re-armed.
   */
   if ( start_time > 0 )
   {
      /*
      ** Convert from Microseconds to the timeout
      */
      OS_UsecsToTicks(start_time, &timeout);

      status = rtems_timer_fire_after(OS_timer_table[timer_id].host_timerid, 
                                        timeout, 
                                        OS_TimerSignalHandler, (void *)timer_id );
      if ( status != RTEMS_SUCCESSFUL )
      {
         return ( OS_TIMER_ERR_INTERNAL);
      }
   }
   return OS_SUCCESS;
}

/******************************************************************************
**  Function:  OS_TimerDelete
**
**  Purpose: 
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 OS_TimerDelete(uint32 timer_id)
{
   rtems_status_code status;

   /* 
   ** Check to see if the timer_id given is valid 
   */
   if (timer_id >= OS_MAX_TIMERS || OS_timer_table[timer_id].free == TRUE)
   {
      return OS_ERR_INVALID_ID;
   }

   status = rtems_semaphore_obtain (OS_timer_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
   OS_timer_table[timer_id].free = TRUE;
   status = rtems_semaphore_release (OS_timer_table_sem);

   /*
   ** Cancel the timer
   */
   status = rtems_timer_cancel(OS_timer_table[timer_id].host_timerid);
   if ( status != RTEMS_SUCCESSFUL )
   {
      return ( OS_TIMER_ERR_INTERNAL);
   }

   /*
   ** Delete the timer 
   */
   status = rtems_timer_delete(OS_timer_table[timer_id].host_timerid);
   if ( status != RTEMS_SUCCESSFUL )
   {
      return ( OS_TIMER_ERR_INTERNAL);
   }
	
   return OS_SUCCESS;
}

/***********************************************************************************
**
**    Name: OS_TimerGetIdByName
**
**    Purpose: This function tries to find a Timer Id given the name 
**             The id is returned through timer_id
**
**    Returns: OS_INVALID_POINTER if timer_id or timer_name are NULL pointers
**             OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
**             OS_ERR_NAME_NOT_FOUND if the name was not found in the table
**             OS_SUCCESS if success
**             
*/
int32 OS_TimerGetIdByName (uint32 *timer_id, const char *timer_name)
{
    uint32 i;

    if (timer_id == NULL || timer_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* 
    ** a name too long wouldn't have been allowed in the first place
    ** so we definitely won't find a name too long
    */
    if (strlen(timer_name) > OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    for (i = 0; i < OS_MAX_TIMERS; i++)
    {
        if (OS_timer_table[i].free != TRUE &&
                (strcmp (OS_timer_table[i].name , (char*) timer_name) == 0))
        {
            *timer_id = i;
            return OS_SUCCESS;
        }
    }
   
    /* 
    ** The name was not found in the table,
    **  or it was, and the sem_id isn't valid anymore 
    */
    return OS_ERR_NAME_NOT_FOUND;
    
}/* end OS_TimerGetIdByName */

/***************************************************************************************
**    Name: OS_TimerGetInfo
**
**    Purpose: This function will pass back a pointer to structure that contains 
**             all of the relevant info( name and creator) about the specified timer.
**             
**    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid timer 
**             OS_INVALID_POINTER if the timer_prop pointer is null
**             OS_SUCCESS if success
*/
int32 OS_TimerGetInfo (uint32 timer_id, OS_timer_prop_t *timer_prop)  
{
   
    rtems_status_code status;

    /* 
    ** Check to see that the id given is valid 
    */
    if (timer_id >= OS_MAX_TIMERS || OS_timer_table[timer_id].free == TRUE)
    {
       return OS_ERR_INVALID_ID;
    }

    if (timer_prop == NULL)
    {
       return OS_INVALID_POINTER;
    }

    /* 
    ** put the info into the stucture 
    */
    status = rtems_semaphore_obtain (OS_timer_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    timer_prop ->creator       = OS_timer_table[timer_id].creator;
    strcpy(timer_prop-> name, OS_timer_table[timer_id].name);
    timer_prop ->start_time    = OS_timer_table[timer_id].start_time;
    timer_prop ->interval_time = OS_timer_table[timer_id].interval_time;
    timer_prop ->accuracy      = OS_timer_table[timer_id].accuracy;
    
    status = rtems_semaphore_release (OS_timer_table_sem);

    return OS_SUCCESS;
    
} /* end OS_TimerGetInfo */
