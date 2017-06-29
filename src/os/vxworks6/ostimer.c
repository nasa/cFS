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
** Purpose: This file contains the OSAL Timer API for vxWorks6
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "common_types.h"
#include "osapi.h"

#include "vxWorks.h"
#include "version.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "semLib.h"
#include "sysLib.h"
#include "time.h"
#include "timers.h"
#include "signal.h"

#define VXWORKS_OSTIMER_UNINITIALIZED 0

/******************************************************************************
 * Design Notes on table locking and semaphore use in this file.
 *
 * With the current design, the OS_timer_table has a matching
 * OS_timer_table_sem semaphore that is used to lock the table for access.
 * However, the table data is not accessed in a thread-safe manner in all
 * of the methods below.  Locking an entire table to read/access a single
 * entry in the table results in performance problems and deadlock
 * conditions.  Therefore, in general, an entire table is locked primarily
 * when a table entry is created or deleted.  Locking the table for most
 * other access is not performed because the standard usage pattern is for
 * a task to only modify its own entries in the table.  WARNING: other
 * patterns of multi-threaded table access via the OSAL API are not
 * safely supported with the current implementation.
 *
 *****************************************************************************/

/****************************************************************************************
                                EXTERNAL FUNCTION PROTOTYPES
****************************************************************************************/

extern uint32 OS_FindCreator(void);

/****************************************************************************************
                                INTERNAL FUNCTION PROTOTYPES
****************************************************************************************/

void  OS_TimespecToUsec(struct timespec time_spec, uint32 *usecs);

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/*
** Since the API is storing the timer values in a 32 bit integer as Microseconds, 
** there is a limit to the number of seconds that can be represented.
*/
#define MAX_SECS_IN_USEC 4293


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
   timer_t              host_timerid;

} OS_timer_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

static OS_timer_internal_record_t OS_timer_table[OS_MAX_TIMERS];
static uint32           os_clock_accuracy;

/*
** The Semaphore for protecting the above table
*/
static SEM_ID OS_timer_table_sem;

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/
int32  OS_TimerAPIInit ( void )
{
   int    status;
   uint32 i;
   int32  return_code = OS_SUCCESS;
   struct timespec clock_resolution;

   /*
   ** Mark all timers as available
   */
   for ( i = 0; i < OS_MAX_TIMERS; i++ )
   {
      OS_timer_table[i].free      = TRUE;
      OS_timer_table[i].creator   = VXWORKS_OSTIMER_UNINITIALIZED;
      OS_timer_table[i].name[0] = '\0';
   }
	
   /*
   ** get the resolution of the realtime clock
   */
   status = clock_getres(CLOCK_REALTIME, &clock_resolution);
   if ( status < 0 )
   {
      return_code = OS_ERROR;
   }  
   else
   { 
      /*
      ** Convert to microseconds
      */
      OS_TimespecToUsec(clock_resolution, &os_clock_accuracy);

      /*
      ** Create the Timer Table mutex
      */
      OS_timer_table_sem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
      if ( OS_timer_table_sem == NULL )
      {
         return_code = OS_ERROR;
      }
   }
   
   return(return_code);
}

/****************************************************************************************
                                INTERNAL FUNCTIONS
****************************************************************************************/
/*
** Timer Signal Handler.
** The purpose of this function is to convert the host os timer number to the 
** OSAL timer number and pass it to the User defined signal handler.
** The Host (vxworks ) timer_id gets passed to the timer callback, so if we
** want the OSAL timer ID to be passed to the user function, this wrapper
** function must be used. 
*/
static void OS_TimerSignalHandler(int host_timer_id)
{
   int     i;

   for (i = 0; i < OS_MAX_TIMERS; i++ )   
   {
      if  (( OS_timer_table[i].free == FALSE ) && ( host_timer_id == (int)OS_timer_table[i].host_timerid))
      {
         (OS_timer_table[i].callback_ptr)(i);
         break;
      }
   } 

}

 
/******************************************************************************
 **  Function:  OS_UsecToTimespec
 **
 **  Purpose:  Convert Microseconds to a POSIX timespec structure.
 **
 */
void OS_UsecToTimespec(uint32 usecs, struct timespec *time_spec)
{
   if (time_spec == 0) {
      return;
   }
   if ( usecs < 1000000 )
   {
      time_spec->tv_nsec = (usecs * 1000);
      time_spec->tv_sec = 0;
   }
   else
   {
      time_spec->tv_sec = usecs / 1000000;
      time_spec->tv_nsec = (usecs % 1000000) * 1000;
   }
	
}

/******************************************************************************
 **  Function:  OS_TimespecToUsec
 **
 **  Purpose:  Convert a POSIX timespec structure to microseconds
 **
 */
void OS_TimespecToUsec(struct timespec time_spec, uint32 *usecs)
{
   if (usecs == 0) {
       return;
   }
   /*
   ** Need to account for maximum number of seconds that will fit in the 
   ** 32 bit usec integer
   */
   if ( time_spec.tv_sec > MAX_SECS_IN_USEC )
   {
      time_spec.tv_sec = MAX_SECS_IN_USEC;
   }
   *usecs = (time_spec.tv_sec * 1000000 ) + (time_spec.tv_nsec / 1000 );
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
int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr)
{
   uint32   possible_tid;
   int32    i;
   int      status;

   /*
   ** Check Parameters
   */
   if ( timer_id == NULL || timer_name == NULL || clock_accuracy == NULL )
   {
        return OS_INVALID_POINTER;
   }

   /* 
   ** Verify callback parameter
   */
   if (callback_ptr == NULL )
   {
      return OS_TIMER_ERR_INVALID_ARGS;
   }

   /*
   ** we don't want to allow names too long
   ** if truncated, two names might be the same 
   */
   if (strlen(timer_name) >= OS_MAX_API_NAME)
   {
      return OS_ERR_NAME_TOO_LONG;
   }

   semTake(OS_timer_table_sem,WAIT_FOREVER);
    
   for(possible_tid = 0; possible_tid < OS_MAX_TIMERS; possible_tid++)
   {
      if (OS_timer_table[possible_tid].free == TRUE)
      {
         break;
      }
   }


   if( possible_tid >= OS_MAX_TIMERS || OS_timer_table[possible_tid].free != TRUE)
   {
        semGive(OS_timer_table_sem);
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
            semGive(OS_timer_table_sem);
            return OS_ERR_NAME_TAKEN;
       }
   }

   /*
   ** Set the possible timer Id to not free so that
   ** no other task can try to use it 
   */
   OS_timer_table[possible_tid].free = FALSE;
   semGive(OS_timer_table_sem);

   OS_timer_table[possible_tid].creator = OS_FindCreator();
   strncpy(OS_timer_table[possible_tid].name, timer_name, OS_MAX_API_NAME);
   OS_timer_table[possible_tid].start_time = 0;
   OS_timer_table[possible_tid].interval_time = 0;
   OS_timer_table[possible_tid].callback_ptr = callback_ptr;
     
   /*
   ** Create the timer
   */
   status = timer_create(CLOCK_REALTIME, NULL, (timer_t *)&(OS_timer_table[possible_tid].host_timerid));
   if (status < 0) 
   {
      OS_timer_table[possible_tid].free = TRUE;
      return ( OS_TIMER_ERR_UNAVAILABLE);
   }

   /*
   ** Connect the timer to the callback function. This is non-posix, but it make the timer
   ** setup easier.
   */
   status = timer_connect((timer_t)(OS_timer_table[possible_tid].host_timerid), (VOIDFUNCPTR)OS_TimerSignalHandler, possible_tid );
   if (status < 0 )
   {
      (void) timer_delete((timer_t)(OS_timer_table[possible_tid].host_timerid));
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
   int    status;
   struct itimerspec timeout;
   
   /* 
   ** Check to see if the timer_id given is valid 
   */
   if (timer_id >= OS_MAX_TIMERS)
   {
      return OS_ERR_INVALID_ID;
   }

   semTake(OS_timer_table_sem,WAIT_FOREVER);

   if ( OS_timer_table[timer_id].free == TRUE)
   {
      semGive(OS_timer_table_sem);
      return OS_ERR_INVALID_ID;
   }
   /*
   ** Round up the accuracy of the start time and interval times 
   */
   if ((start_time > 0) && ( start_time < os_clock_accuracy ))
   {
      start_time = os_clock_accuracy;
   }

   if ((interval_time > 0) && ( interval_time < os_clock_accuracy ))
   {
      interval_time = os_clock_accuracy;
   }

   /*
   ** Save the start and interval times 
   */
   OS_timer_table[timer_id].start_time = start_time;
   OS_timer_table[timer_id].interval_time = interval_time;

   /*
   ** Convert from Microseconds to timespec structures
   */
   OS_UsecToTimespec(start_time, &(timeout.it_value));
   OS_UsecToTimespec(interval_time, &(timeout.it_interval));
	
   /*
   ** Program the real timer
   */
   status = timer_settime((timer_t)(OS_timer_table[timer_id].host_timerid), 
                           0,              /* Flags field can be zero */
                           &timeout,       /* struct itimerspec */
		                     NULL);         /* Oldvalue */
   semGive(OS_timer_table_sem);
   if (status < 0) 
   {
      return ( OS_TIMER_ERR_INTERNAL);
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
   int status;

   /* 
   ** Check to see if the timer_id given is valid 
   */
   if (timer_id >= OS_MAX_TIMERS)
   {
      return OS_ERR_INVALID_ID;
   }

   semTake(OS_timer_table_sem,WAIT_FOREVER);

   if (OS_timer_table[timer_id].free == TRUE)
   {
      semGive(OS_timer_table_sem);
      return OS_ERR_INVALID_ID;
   }

   OS_timer_table[timer_id].free = TRUE;
   semGive(OS_timer_table_sem);
   /*
   ** Delete the timer 
   */
   status = timer_delete((timer_t)(OS_timer_table[timer_id].host_timerid));
   if (status < 0)
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
    if (strlen(timer_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    semTake(OS_timer_table_sem,WAIT_FOREVER);
    for (i = 0; i < OS_MAX_TIMERS; i++)
    {
        if (OS_timer_table[i].free != TRUE &&
                (strcmp (OS_timer_table[i].name , (char*) timer_name) == 0))
        {
            *timer_id = i;
            semGive(OS_timer_table_sem);
            return OS_SUCCESS;
        }
    }
   
    semGive(OS_timer_table_sem);
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
    if (timer_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }
    /* 
    ** Check to see that the id given is valid 
    */
    if (timer_id >= OS_MAX_TIMERS)
    {
       return OS_ERR_INVALID_ID;
    }

    semTake(OS_timer_table_sem,WAIT_FOREVER);

    if (OS_timer_table[timer_id].free == TRUE)
    {
       semGive(OS_timer_table_sem);
       return OS_ERR_INVALID_ID;
    }

    /* 
    ** put the info into the structure
    */

    timer_prop ->creator       = OS_timer_table[timer_id].creator;
    strcpy(timer_prop-> name, OS_timer_table[timer_id].name);
    timer_prop ->start_time    = OS_timer_table[timer_id].start_time;
    timer_prop ->interval_time = OS_timer_table[timer_id].interval_time;
    timer_prop ->accuracy      = OS_timer_table[timer_id].accuracy;
    
    semGive(OS_timer_table_sem);

    return OS_SUCCESS;
    
} /* end OS_TimerGetInfo */

/****************************************************************
 * TIME BASE API
 *
 * This is not implemented by this OSAL, so return "OS_ERR_NOT_IMPLEMENTED"
 * for all calls defined by this API.  This is necessary for forward
 * compatibility (runtime code can check for this return code and use
 * an alternative API where appropriate).
 */

int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseDelete(uint32 timer_id)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_id, OS_ArgCallback_t  callback_ptr, void *callback_arg)
{
    return OS_ERR_NOT_IMPLEMENTED;
}


