/*
** File   :	osapi.c
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author :	Ezra Yeheskeli
**
** Purpose: 
**	   This file  contains some of the OS APIs abstraction layer.It 
**     contains those APIs that call the  OS. In this case the OS is the Rtems OS.
**
**  $Date: 2013/12/11 15:46:47GMT-05:00 $
**  $Revision: 1.27 $
**  $Log: osapi.c  $
**  Revision 1.27 2013/12/11 15:46:47GMT-05:00 acudmore 
**  Updated OS_QueueGet to detect buffer overflow condition
**  Revision 1.26 2013/07/24 11:15:37GMT-05:00 acudmore 
**  Updated comments and formatting in Milli2Ticks and Tick2Micros
**  Revision 1.25 2013/01/15 16:41:26GMT-05:00 acudmore 
**  Removed fields from binary and counting sem tables that are not needed.
**  Fixed return code for binary semaphores when flushed.
**  Revision 1.24 2012/12/19 14:39:38GMT-05:00 acudmore 
**  Updated use of size_copied in OS_QueueGet
**  Revision 1.23 2012/12/06 14:54:37EST acudmore 
**  updated comments about task flags
**  Revision 1.22 2012/11/09 17:09:11EST acudmore 
**  Changed the way binary and counting sems work.
**    Fixed parameters to RTEMS call
**    Fixed Semaphore creation attributes
**    Removed use of OSAL maintained counters for sems
**  Revision 1.21 2012/06/14 11:32:50EDT acudmore 
**  Fixed a few bugs related to code walkthrough changes:
**    - Max count sem value is now max signed int
**    - typos in table initialization
**    - left an extra semaphore release in
**  Revision 1.20 2012/04/13 14:53:55EDT acudmore 
**  MMS/RTEMS Code walkthrough changes
**  Revision 1.19 2012/04/11 10:34:03EDT acudmore 
**  Made OS_printf ISR safe. It will return if called from an ISR.
**  Revision 1.18 2012/04/11 10:28:32EDT acudmore 
**  Added OS_printf_enable and OS_printf_disable
**  Revision 1.17 2011/12/13 11:57:16EST acudmore 
**  Removed Cast from interrupt disable call. It was failing to compile on rtems4.10-sparc
**  Revision 1.16 2011/12/05 15:26:05EST acudmore 
**  Added semaphore protection for counting semaphore give and take operations
**  Revision 1.15 2011/06/27 15:52:03EDT acudmore 
**  Went over APIs and Documentation for return code consistency.
**  Updated documentation, function comments, and return codes as needed.
**  Revision 1.14 2010/11/12 12:01:10EST acudmore 
**  replaced copyright character with (c) and added open source notice where needed.
**  Revision 1.13 2010/11/10 15:43:01EST acudmore 
**  Revision 1.1 2007/10/16 16:14:58EDT apcudmore 
**  Initial revision
*/
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h> /* checking ETIMEDOUT */
#include <rtems.h>
#include <rtems/malloc.h>

/*
** User defined include files
*/
#include "common_types.h"
#include "osapi.h"

/*
** Function Prototypes
*/
int     malloc_info ( Heap_Information_block *the_info); /* This is not in an RTEMS header */
uint32  OS_FindCreator(void);
/****************************************************************************************
                                     DEFINES
****************************************************************************************/

#define RTEMS_INT_LEVEL_ENABLE_ALL  0
#define RTEMS_INT_LEVEL_DISABLE_ALL 7

#define MAX_PRIORITY                255
#define MAX_SEM_VALUE               0x7FFFFFFF
#define UNINITIALIZED               0 


/*
** Define all of the RTEMS semaphore attributes
** 1. The TABLE_MUTEX attributes are for the internal OSAL tables.
**    In RTEMS, a MUTEX is defined as a binary semaphore 
**    It allows nested locks, priority wait order, and supports priority inheritance
**
** 2. OSAL Mutex attributes -- same as 1
**
** 3. OSAL Binary Semaphore attributes 
**    This is a simple binary semaphore used for syncronization. It does not 
**    allow nested calls ( nor should it ) It should not be used for mutual exclusion.
**
** 4. OSAL Counting Semaphore attributes
**     This is a counting semaphore with priority wait order.
**
*/
#define OSAL_TABLE_MUTEX_ATTRIBS (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY)

#define OSAL_MUTEX_ATTRIBS (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY)

#define OSAL_BINARY_SEM_ATTRIBS (RTEMS_SIMPLE_BINARY_SEMAPHORE | RTEMS_PRIORITY) 

#define OSAL_COUNT_SEM_ATTRIBS (RTEMS_PRIORITY) 


/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
/*  tables for the properties of objects */

/*tasks */
typedef struct
{
    int      free;
    rtems_id id;
    char     name [OS_MAX_API_NAME];
    int      creator;
    uint32   stack_size;
    uint32   priority;
    void    *delete_hook_pointer;
    
}OS_task_record_t;
    
/* queues */
typedef struct
{
    int      free;
    rtems_id id;
    uint32   max_size;
    char     name [OS_MAX_API_NAME];
    int      creator;
}OS_queue_record_t;

/* Binary Semaphores */
typedef struct
{
    int      free;
    rtems_id id;
    char     name [OS_MAX_API_NAME];
    int      creator;    
}OS_bin_sem_record_t;

/* Counting Semaphores */
typedef struct
{
    int      free;
    rtems_id id;
    char     name [OS_MAX_API_NAME];
    int      creator;
}OS_count_sem_record_t;

/* Mutexes */
typedef struct
{
    int             free;
    rtems_id        id;
    char            name [OS_MAX_API_NAME];
    int             creator;
}OS_mut_sem_record_t;

/* function pointer type */
typedef void (*FuncPtr_t)(void);

void    *OS_task_key = 0;         /* this is what rtems wants! */

/* Tables where the OS object information is stored */
OS_task_record_t    OS_task_table          [OS_MAX_TASKS];
OS_queue_record_t   OS_queue_table         [OS_MAX_QUEUES];
OS_bin_sem_record_t OS_bin_sem_table       [OS_MAX_BIN_SEMAPHORES];
OS_count_sem_record_t OS_count_sem_table   [OS_MAX_COUNT_SEMAPHORES];
OS_mut_sem_record_t OS_mut_sem_table       [OS_MAX_MUTEXES];

rtems_id            OS_task_table_sem;
rtems_id            OS_queue_table_sem;
rtems_id            OS_bin_sem_table_sem;
rtems_id            OS_mut_sem_table_sem;
rtems_id            OS_count_sem_table_sem;

uint32              OS_printf_enabled = TRUE;

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_API_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_API_Init(void)
{
    int               i;
    int32             return_code = OS_SUCCESS;
    rtems_status_code rtems_sc;

    /* Initialize Task Table */
    for(i = 0; i < OS_MAX_TASKS; i++)
    {
        OS_task_table[i].free                = TRUE;
        OS_task_table[i].id                  = UNINITIALIZED;
        OS_task_table[i].creator             = UNINITIALIZED;
        OS_task_table[i].delete_hook_pointer = NULL;
        OS_task_table[i].name[0] = '\0';    
    }

    /* Initialize Message Queue Table */
    for(i = 0; i < OS_MAX_QUEUES; i++)
    {
        OS_queue_table[i].free        = TRUE;
        OS_queue_table[i].id          = UNINITIALIZED;
        OS_queue_table[i].creator     = UNINITIALIZED;
        OS_queue_table[i].name[0] = '\0'; 
    }

    /* Initialize Binary Semaphore Table */
    for(i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        OS_bin_sem_table[i].free          = TRUE;
        OS_bin_sem_table[i].id            = UNINITIALIZED;
        OS_bin_sem_table[i].creator       = UNINITIALIZED;
        OS_bin_sem_table[i].name[0] = '\0';
    }

    /* Initialize Counting Semaphore Table */
    for(i = 0; i < OS_MAX_COUNT_SEMAPHORES; i++)
    {
        OS_count_sem_table[i].free        = TRUE;
        OS_count_sem_table[i].id          = UNINITIALIZED;
        OS_count_sem_table[i].creator     = UNINITIALIZED;
        OS_count_sem_table[i].name[0] = '\0';
    }

    /* Initialize Mutex Semaphore Table */
    for(i = 0; i < OS_MAX_MUTEXES; i++)
    {
        OS_mut_sem_table[i].free        = TRUE;
        OS_mut_sem_table[i].id          = UNINITIALIZED;
        OS_mut_sem_table[i].creator     = UNINITIALIZED;
        OS_mut_sem_table[i].name[0] = '\0';
    }
    
    /*
    ** Initialize the module loader
    */
    #ifdef OS_INCLUDE_MODULE_LOADER
      return_code = OS_ModuleTableInit();
      if ( return_code != OS_SUCCESS )
      {
         return(return_code);
      }
    #endif

   /*
   ** Initialize the Timer API
   */
   return_code = OS_TimerAPIInit();
   if ( return_code == OS_ERROR )
   {
      return(return_code);
   }

   /*
   ** Initialize the internal table Mutexes
   */
   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', '1'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_task_table_sem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return_code = OS_ERROR;
      return(return_code);
   }

   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', '2'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_queue_table_sem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return_code = OS_ERROR;
      return(return_code);
   }

   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', '3'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_bin_sem_table_sem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return_code = OS_ERROR;
      return(return_code);
   }

   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', '4'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_count_sem_table_sem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return_code = OS_ERROR;
      return(return_code);
   }
 
   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', '5'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_mut_sem_table_sem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return_code = OS_ERROR;
      return(return_code);
   }

   /*
   ** File system init
   */
   return_code = OS_FS_Init();

   
   return(return_code);
   
} /* end OS_API_Init */

/****************************************************************************************
                                    TASK API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_TaskCreate

   Purpose: Creates a task and starts running it.

   returns: OS_INVALID_POINTER if any of the necessary pointers are NULL
            OS_ERR_NAME_TOO_LONG if the name of the task is too long to be copied
            OS_ERR_INVALID_PRIORITY if the priority is bad
            OS_ERR_NO_FREE_IDS if there can be no more tasks created
            OS_ERR_NAME_TAKEN if the name specified is already used by a task
            OS_ERROR if the operating system calls fail
            OS_SUCCESS if success
            
    NOTES: task_id is passed back to the user as the ID. stack_pointer is usually null.


---------------------------------------------------------------------------------------*/

int32 OS_TaskCreate (uint32 *task_id, const char *task_name, osal_task_entry function_pointer,
                      const uint32 *stack_pointer, uint32 stack_size, uint32 priority, 
                      uint32 flags)
{
    uint32             possible_taskid;
    uint32             i;
    rtems_status_code  status;
    rtems_name         r_name;
    rtems_mode         r_mode;
    rtems_attribute    r_attributes;


    /* Check for NULL pointers */
    if( (task_name == NULL) || (function_pointer == NULL) || (task_id == NULL) )
    {
        return OS_INVALID_POINTER;
    }
    
    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */
    if (strlen(task_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    /* Check for bad priority */
    if (priority > MAX_PRIORITY)
    {
        return OS_ERR_INVALID_PRIORITY;
    }
    
    /* Check Parameters */
    status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for(possible_taskid = 0; possible_taskid < OS_MAX_TASKS; possible_taskid++)
    {
        if (OS_task_table[possible_taskid].free == TRUE)
        {
            break;
        }
    }

    /* Check to see if the id is out of bounds */
    if( possible_taskid >= OS_MAX_TASKS || OS_task_table[possible_taskid].free != TRUE)
    {    
        status = rtems_semaphore_release (OS_task_table_sem);
        return OS_ERR_NO_FREE_IDS;
    }

    /* Check to see if the name is already taken */
    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        if ((OS_task_table[i].free == FALSE) &&
           ( strcmp((char*)task_name, OS_task_table[i].name) == 0)) 
        {        
            status = rtems_semaphore_release (OS_task_table_sem);
            return OS_ERR_NAME_TAKEN;
        }
    }
    /* Set the possible task Id to not free so that
     * no other task can try to use it */

    OS_task_table[possible_taskid].free  = FALSE;
    status = rtems_semaphore_release (OS_task_table_sem);

    /*
    ** RTEMS task names are only 4 characters. 
    ** Use the passed in name so at least you can try to determine the name
    */    
    r_name = rtems_build_name(task_name[0],task_name[1],task_name[2],task_name[3]);
    r_mode = RTEMS_PREEMPT | RTEMS_NO_ASR | RTEMS_NO_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0);

    /* 
    ** see if the user wants floating point enabled. If 
    ** so, then se the correct option.
    */
    if (flags == OS_FP_ENABLED)
    {
        r_attributes = RTEMS_FLOATING_POINT | RTEMS_LOCAL;
    }
    else
    {
        r_attributes = RTEMS_LOCAL;
    }
	
    status = rtems_task_create(
                 r_name,
		 priority,
		 stack_size,
		 r_mode,
		 r_attributes,
		 &OS_task_table[possible_taskid].id); 
    
    /* check if task_create failed */
    if (status != RTEMS_SUCCESSFUL )
    {       
        status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        OS_task_table[possible_taskid].free  = TRUE;
        status = rtems_semaphore_release (OS_task_table_sem);
	return OS_ERROR;
    } 

    /* will place the task in 'ready for scheduling' state */
    status = rtems_task_start (OS_task_table[possible_taskid].id, /*rtems task id*/
			     (rtems_task_entry) function_pointer, /*task entry point */
				 0 );                             /* passed argument  */
	
    if (status != RTEMS_SUCCESSFUL )
    {		
        status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        OS_task_table[possible_taskid].free  = TRUE;
        status = rtems_semaphore_release (OS_task_table_sem);
	return OS_ERROR;		
    }
    
    /* Set the task_id to the id that was found available 
       Set the name of the task, the stack size, and priority */
    *task_id = possible_taskid;

    /* this Id no longer free */
    status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    strcpy(OS_task_table[*task_id].name, (char*) task_name);
    OS_task_table[*task_id].creator = OS_FindCreator();
    OS_task_table[*task_id].stack_size = stack_size;
    OS_task_table[*task_id].priority = priority;
    status = rtems_semaphore_release (OS_task_table_sem);
    return OS_SUCCESS;
    
} /* end OS_TaskCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_TaskDelete

    Purpose: Deletes the specified Task and removes it from the OS_task_table.

    returns: OS_ERR_INVALID_ID if the ID given to it is invalid
             OS_ERROR if the OS delete call fails
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_TaskDelete (uint32 task_id)
{    
    FuncPtr_t         FunctionPointer;
    rtems_status_code status;
    
    /* 
    ** Check to see if the task_id given is valid 
    */
    if (task_id >= OS_MAX_TASKS || OS_task_table[task_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    /*
    ** Call the task Delete hook if there is one.
    */
    if ( OS_task_table[task_id].delete_hook_pointer != NULL)
    {
       FunctionPointer = (FuncPtr_t)OS_task_table[task_id].delete_hook_pointer;
       (*FunctionPointer)();
    }

    /* Try to delete the task */
    if (rtems_task_delete(OS_task_table[task_id].id) != RTEMS_SUCCESSFUL)
    {
	return OS_ERROR;
    }
    
    /*
     * Now that the task is deleted, remove its 
     * "presence" in OS_task_table
    */
    status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    OS_task_table[task_id].free = TRUE;
    OS_task_table[task_id].id = UNINITIALIZED;
    OS_task_table[task_id].name[0] = '\0';
    OS_task_table[task_id].creator = UNINITIALIZED;
    OS_task_table[task_id].stack_size = UNINITIALIZED;
    OS_task_table[task_id].priority = UNINITIALIZED;
    OS_task_table[task_id].delete_hook_pointer = NULL;            
    status = rtems_semaphore_release (OS_task_table_sem);
    
    return OS_SUCCESS;
    
}/* end OS_TaskDelete */
/*--------------------------------------------------------------------------------------
     Name:    OS_TaskExit

     Purpose: Exits the calling task and removes it from the OS_task_table.

     returns: Nothing 
---------------------------------------------------------------------------------------*/
void OS_TaskExit()
{
    uint32            task_id;
    rtems_status_code status;

    task_id = OS_TaskGetId();

    status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    
    OS_task_table[task_id].free = TRUE;
    OS_task_table[task_id].id = UNINITIALIZED;
    OS_task_table[task_id].name[0] = '\0';
    OS_task_table[task_id].creator = UNINITIALIZED;
    OS_task_table[task_id].stack_size = UNINITIALIZED;
    OS_task_table[task_id].priority = UNINITIALIZED;
    OS_task_table[task_id].delete_hook_pointer = NULL;            
    status = rtems_semaphore_release (OS_task_table_sem); 

    rtems_task_delete(RTEMS_SELF);

}/*end OS_TaskExit */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskDelay

   Purpose: Delay a task for specified amount of milliseconds

   returns: OS_ERROR if sleep fails
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_TaskDelay (uint32 milli_second)
{
    rtems_interval     ticks;  
    rtems_status_code status;
	
    ticks = OS_Milli2Ticks(milli_second);
    status = rtems_task_wake_after(ticks);

    /*
    ** Always successful ( from RTEMS docs )
    */
    return(OS_SUCCESS) ;

}/* end OS_TaskDelay */
/*---------------------------------------------------------------------------------------
   Name: OS_TaskSetPriority

   Purpose: Sets the given task to a new priority

    returns: OS_ERR_INVALID_ID if the ID passed to it is invalid
             OS_ERR_INVALID_PRIORITY if the priority is greater than the max 
             allowed
             OS_ERROR if the OS call to change the priority fails
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_TaskSetPriority (uint32 task_id, uint32 new_priority)
{
    rtems_task_priority old_pri;
    
    /* Check Parameters */
    if(task_id >= OS_MAX_TASKS || OS_task_table[task_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if (new_priority > MAX_PRIORITY)
    {
        return OS_ERR_INVALID_PRIORITY;
    }
    
    /* Set RTEMS Task Priority */
    if (rtems_task_set_priority(OS_task_table[task_id].id, new_priority, &old_pri)
       != RTEMS_SUCCESSFUL )
    {
            return OS_ERROR;
    }
    OS_task_table[task_id].priority = new_priority;

    return OS_SUCCESS;

}/* end OS_TaskSetPriority */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskRegister
  
   Purpose: Registers the calling task id with the task by adding the var to the tcb
  			It searches the OS_task_table to find the task_id corresponding to the 
                        tcb_id
            
   Returns: OS_ERR_INVALID_ID if there the specified ID could not be found
            OS_ERROR if the OS call fails
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_TaskRegister (void)
{
   rtems_id          rtems_task_id;
   rtems_status_code rtems_status;
   int 	              i;
   uint32	      task_id;
	
   /* 
   ** Get RTEMS Task Id
   */
   rtems_status = rtems_task_ident(RTEMS_SELF, 0, &rtems_task_id);
   if ( rtems_status != RTEMS_SUCCESSFUL )
   {		
      return(OS_ERROR);
   }
	
    for(i = 0; i < OS_MAX_TASKS; i++)
    {
        if(OS_task_table[i].id == rtems_task_id)
            break;
    }

    task_id = i;

    if(task_id >= OS_MAX_TASKS)
    {
        return OS_ERR_INVALID_ID;
    }

    /* Add RTEMS Task Variable */
    rtems_status = rtems_task_variable_add(
		rtems_task_id,        /* rtems task id */
		(void *)&OS_task_key, /* the task variable of type: void *task_variable */
		NULL);                /* no function destructure is specified */
	
    if ( rtems_status != RTEMS_SUCCESSFUL )
    {
	return(OS_ERROR);
    }
    
    OS_task_key = (void *)task_id;
    
    return OS_SUCCESS;

}/* end OS_TaskRegister */

/*---------------------------------------------------------------------------------------
   Name: OS_TaskGetId

   Purpose: This function returns the #defined task id of the calling task

   Notes: The OS_task_key is initialized by the task switch if AND ONLY IF the 
          OS_task_key has been registered via OS_TaskRegister(..).  If this is not 
          called prior to this call, the value will be old and wrong.
---------------------------------------------------------------------------------------*/
uint32 OS_TaskGetId (void)
{
    return (uint32) OS_task_key;

}/* end OS_TaskGetId */

/*--------------------------------------------------------------------------------------
    Name: OS_TaskGetIdByName

    Purpose: This function tries to find a task Id given the name of a task

    Returns: OS_INVALID_POINTER if the pointers passed in are NULL
             OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with
             OS_ERR_NAME_NOT_FOUND if the name wasn't found in the table
             OS_SUCCESS if SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_TaskGetIdByName (uint32 *task_id, const char *task_name)
{
    uint32 i;

    if (task_id == NULL || task_name == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    /* we don't want to allow names too long because they won't be found at all */
    if (strlen(task_name) >= OS_MAX_API_NAME)
    {
       return OS_ERR_NAME_TOO_LONG;
    }

    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        if( (OS_task_table[i].free != TRUE) &&
          (strcmp(OS_task_table[i].name,(char*) task_name) == 0 ))
        {
            *task_id = i;
            return OS_SUCCESS;
        }
    }
    
    /* The name was not found in the table,
     *  or it was, and the task_id isn't valid anymore */
    return OS_ERR_NAME_NOT_FOUND;

}/* end OS_TaskGetIdByName */         

/*---------------------------------------------------------------------------------------
    Name: OS_TaskGetInfo

    Purpose: This function will pass back a pointer to structure that contains 
             all of the relevant info (creator, stack size, priority, name) about the 
             specified task. 

    Returns: OS_ERR_INVALID_ID if the ID passed to it is invalid
             OS_INVALID_POINTER if the task_prop pointer is NULL
             OS_SUCCESS if it copied all of the relevant info over
 
---------------------------------------------------------------------------------------*/
int32 OS_TaskGetInfo (uint32 task_id, OS_task_prop_t *task_prop)  
{
    rtems_status_code status;

    /* Check to see that the id given is valid */
    if (task_id >= OS_MAX_TASKS || OS_task_table[task_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if( task_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* put the info into the stucture */
    status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    task_prop -> creator =    OS_task_table[task_id].creator;
    task_prop -> stack_size = OS_task_table[task_id].stack_size;
    task_prop -> priority =   OS_task_table[task_id].priority;
    task_prop -> OStask_id =  (uint32) OS_task_table[task_id].id;
    status = rtems_semaphore_release (OS_task_table_sem);    
    
    strcpy(task_prop-> name, OS_task_table[task_id].name);
    
    return OS_SUCCESS;
    
} /* end OS_TaskGetInfo */

/*--------------------------------------------------------------------------------------
     Name: OS_TaskInstallDeleteHandler

    Purpose: Installs a handler for when the task is deleted.

    returns: status
---------------------------------------------------------------------------------------*/
int32 OS_TaskInstallDeleteHandler(void *function_pointer)
{
    uint32            task_id;
    rtems_status_code status;

    task_id = OS_TaskGetId();

    if ( task_id >= OS_MAX_TASKS )
    {
       return(OS_ERR_INVALID_ID);
    }

    status = rtems_semaphore_obtain (OS_task_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    if ( OS_task_table[task_id].free != FALSE )
    {
       /* 
       ** Somehow the calling task is not registered 
       */
       status = rtems_semaphore_release (OS_task_table_sem);
       return(OS_ERR_INVALID_ID);
    }

    /*
    ** Install the pointer
    */
    OS_task_table[task_id].delete_hook_pointer = function_pointer;    
    
    status = rtems_semaphore_release (OS_task_table_sem);

    return(OS_SUCCESS);
    
}/*end OS_TaskInstallDeleteHandler */

/****************************************************************************************
                                MESSAGE QUEUE API
****************************************************************************************/
/*---------------------------------------------------------------------------------------
   Name: OS_QueueCreate

   Purpose: Create a message queue which can be refered to by name or ID

   Returns: OS_INVALID_POINTER if a pointer passed in is NULL
            OS_ERR_NAME_TOO_LONG if the name passed in is too long
            OS_ERR_NO_FREE_IDS if there are already the max queues created
            OS_ERR_NAME_TAKEN if the name is already being used on another queue
            OS_ERROR if the OS create call fails
            OS_SUCCESS if success

   Notes: the flahs parameter is unused.
---------------------------------------------------------------------------------------*/

int32 OS_QueueCreate (uint32 *queue_id, const char *queue_name, uint32 queue_depth, 
                       uint32 data_size, uint32 flags)
{
    rtems_status_code  status;
    rtems_name         r_name;
    uint32             possible_qid;
    uint32             i;

    /* Check Parameters */
    if ( queue_id == NULL || queue_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */
    if (strlen(queue_name) >= OS_MAX_API_NAME)
    {
       return OS_ERR_NAME_TOO_LONG;
    }

    status = rtems_semaphore_obtain (OS_queue_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for(possible_qid = 0; possible_qid < OS_MAX_QUEUES; possible_qid++)
    {
        if (OS_queue_table[possible_qid].free == TRUE)
            break;
    }
    
    if( possible_qid >= OS_MAX_QUEUES || OS_queue_table[possible_qid].free != TRUE)
    {
        status = rtems_semaphore_release (OS_queue_table_sem);
        return OS_ERR_NO_FREE_IDS;
    }

    /* Check to see if the name is already taken */
    for (i = 0; i < OS_MAX_QUEUES; i++)
    {
        if ((OS_queue_table[i].free == FALSE) &&
                strcmp ((char*) queue_name, OS_queue_table[i].name) == 0)
        {
            status = rtems_semaphore_release (OS_queue_table_sem);
            return OS_ERR_NAME_TAKEN;
        }
    }

    /* set the ID free to false to prevent other tasks from grabbing it */
    OS_queue_table[possible_qid].free = FALSE;   
    status = rtems_semaphore_release (OS_queue_table_sem);

    /*
    ** Create the message queue.
    ** The queue attributes are set to default values; the waiting order
    ** (RTEMS_FIFO or RTEMS_PRIORITY) is irrelevant since only one task waits
    ** on each queue.
    */
    r_name = rtems_build_name(queue_name[0],queue_name[1],queue_name[2],queue_name[3]);
    status = rtems_message_queue_create(
    r_name,                        /* 32-bit RTEMS object name; not used */
    queue_depth,                   /* maximum number of messages in queue (queue depth) */
    data_size,                     /* maximum size in bytes of a message */
    RTEMS_FIFO|RTEMS_LOCAL,        /* attributes (default) */
    &(OS_queue_table[possible_qid].id)  /* object ID returned for queue */
    );
	
    /*
    ** If the operation failed, report the error 
    */
    if (status != RTEMS_SUCCESSFUL) 
    {    
       status = rtems_semaphore_obtain (OS_queue_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
       OS_queue_table[possible_qid].free = TRUE;   
       OS_queue_table[possible_qid].id = 0;
       status = rtems_semaphore_release (OS_queue_table_sem);
       return OS_ERROR;
    }
    
    /* Set the queue_id to the id that was found available*/
    /* Set the name of the queue, and the creator as well */
    *queue_id = possible_qid;
     
    status = rtems_semaphore_obtain (OS_queue_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    OS_queue_table[*queue_id].max_size = data_size; 
    strcpy( OS_queue_table[*queue_id].name, (char*) queue_name);
    OS_queue_table[*queue_id].creator = OS_FindCreator();
    status = rtems_semaphore_release (OS_queue_table_sem);

    return OS_SUCCESS;

} /* end OS_QueueCreate */

/*--------------------------------------------------------------------------------------
    Name: OS_QueueDelete

    Purpose: Deletes the specified message queue.

    Returns: OS_ERR_INVALID_ID if the id passed in does not exist
             OS_ERROR if the OS call to delete the queue fails 
             OS_SUCCESS if success

    Notes: If There are messages on the queue, they will be lost and any subsequent
           calls to QueueGet or QueuePut to this queue will result in errors
---------------------------------------------------------------------------------------*/
int32 OS_QueueDelete (uint32 queue_id)
{
    rtems_status_code status;

    /* Check to see if the queue_id given is valid */
    if (queue_id >= OS_MAX_QUEUES || OS_queue_table[queue_id].free == TRUE)
    {
       return OS_ERR_INVALID_ID;
    }

    /* Try to delete the queue */
    if (rtems_message_queue_delete(OS_queue_table[queue_id].id) != RTEMS_SUCCESSFUL)
    {
        return OS_ERROR;
    }
	    
    /* 
     * Now that the queue is deleted, remove its "presence"
     * in OS_queue_table
    */
    status = rtems_semaphore_obtain (OS_queue_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    OS_queue_table[queue_id].free = TRUE;
    OS_queue_table[queue_id].name[0] = '\0';
    OS_queue_table[queue_id].creator = UNINITIALIZED;
    OS_queue_table[queue_id].id = UNINITIALIZED;
    OS_queue_table[queue_id].max_size = 0;
    status = rtems_semaphore_release (OS_queue_table_sem);

    return OS_SUCCESS;

} /* end OS_QueueDelete */


/*---------------------------------------------------------------------------------------
   Name: OS_QueueGet

   Purpose: Receive a message on a message queue.  Will pend or timeout on the receive.
   Returns: OS_ERR_INVALID_ID if the given ID does not exist
            OS_INVALID_POINTER if a pointer passed in is NULL
            OS_QUEUE_EMPTY if the Queue has no messages on it to be recieved
            OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
            OS_QUEUE_INVALID_SIZE if the size passed in may be too small for the message 
            OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_QueueGet (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, 
                    int32 timeout)
{
    /* msecs rounded to the closest system tick count */
    rtems_status_code  status;
    rtems_interval     ticks;  
    rtems_id           rtems_queue_id;
    
    /* Check Parameters */
    if(queue_id >= OS_MAX_QUEUES || OS_queue_table[queue_id].free == TRUE)
    {
        return(OS_ERR_INVALID_ID);
    }
    else if( (data == NULL) || (size_copied == NULL) )
    {
        return (OS_INVALID_POINTER);
    }
    else if( size < OS_queue_table[queue_id].max_size )
    {
        /* 
        ** The buffer that the user is passing in is potentially too small
        ** RTEMS will just copy into a buffer that is too small
        */
        *size_copied = 0;
        return(OS_QUEUE_INVALID_SIZE);
    }

    rtems_queue_id = OS_queue_table[queue_id].id; 
    
    /* Get Message From Message Queue */
    if(timeout == OS_PEND)
    {
       /*
       ** Pend forever until a message arrives.
       */
       status = rtems_message_queue_receive(
		rtems_queue_id,            /* message queue descriptor */
		data,                      /* pointer to message buffer */
		(size_t *)size_copied,     /* returned size of message */
		RTEMS_WAIT,                /* wait option */
		RTEMS_NO_TIMEOUT           /* timeout */
		);
    }
    else if (timeout == OS_CHECK)
    {
	/*
	** Get a message without waiting.  If no message is present,
	** return with a failure indication.
	*/
	status = rtems_message_queue_receive(
   		   rtems_queue_id,            /* message queue descriptor */
		   data,                      /* pointer to message buffer */
		   (size_t *)size_copied,     /* returned size of message */
		   RTEMS_NO_WAIT,             /* wait option */
		   RTEMS_NO_TIMEOUT           /* timeout */
		);
		
	if (status == RTEMS_UNSATISFIED)
        {
            *size_copied = 0;
	    return OS_QUEUE_EMPTY;
        } 
        
    }
    else
    {
	/*
	** Wait for up to a specified amount of time for a message to arrive.
	** If no message arrives within the timeout interval, return with a
	** failure indication.
	*/
        ticks = OS_Milli2Ticks(timeout);
        status = rtems_message_queue_receive(
		rtems_queue_id,              /* message queue descriptor */
		data,                        /* pointer to message buffer */
		(size_t *)size_copied,       /* returned size of message */
		RTEMS_WAIT,                  /* wait option */
		ticks                        /* timeout */
		);
		
        if (status == RTEMS_TIMEOUT)
        {
            *size_copied = 0;
 	    return OS_QUEUE_TIMEOUT;       
        }
        
    }/* else */
   
    /*
    ** Check the status of the read operation.  If a valid message was
    ** obtained, indicate success.  
    */
    if (status == RTEMS_SUCCESSFUL)
    {
	/* Success. */
	return OS_SUCCESS;
    }
    else 
    {
       *size_copied = 0;
       return OS_ERROR;
    }
   
}/* end OS_QueueGet */

/*---------------------------------------------------------------------------------------
   Name: OS_QueuePut

   Purpose: Put a message on a message queue.

   Returns: OS_ERR_INVALID_ID if the queue id passed in is not a valid queue
            OS_INVALID_POINTER if the data pointer is NULL
            OS_QUEUE_FULL if the queue cannot accept another message
            OS_ERROR if the OS call returns an error
            OS_SUCCESS if SUCCESS            
   
   Notes: The flags parameter is not used.  The message put is always configured to
            immediately return an error if the receiving message queue is full.
---------------------------------------------------------------------------------------*/

int32 OS_QueuePut (uint32 queue_id, void *data, uint32 size, uint32 flags)
{
    rtems_status_code  status;
    rtems_id           rtems_queue_id;

    /* Check Parameters */
    if(queue_id >= OS_MAX_QUEUES || OS_queue_table[queue_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if (data == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    rtems_queue_id = OS_queue_table[queue_id].id; 

    /* Get Message From RTEMS Message Queue */

    /* Write the buffer pointer to the queue.  If an error occurred, report it
    ** with the corresponding SB status code.
    */
    status = rtems_message_queue_send(
		    rtems_queue_id,     /* message queue descriptor */
                    data,                             /* pointer to message */
                    size                              /* length of message */
        	);
   
    if (status == RTEMS_SUCCESSFUL) 
    {
	return OS_SUCCESS;
    }
    else if (status == RTEMS_TOO_MANY) 
    {
	/* 
	** Queue is full. 
	*/
	return OS_QUEUE_FULL;
    }
    else 
    {
	/* 
	** Unexpected error while writing to queue. 
	*/
	return OS_ERROR;
    }
    
}/* end OS_QueuePut */

/*--------------------------------------------------------------------------------------
    Name: OS_QueueGetIdByName

    Purpose: This function tries to find a queue Id given the name of the queue. The             
		id of the queue is passed back in queue_id

    Returns: OS_INVALID_POINTER if the name or id pointers are NULL
             OS_ERR_NAME_TOO_LONG the name passed in is too long
             OS_ERR_NAME_NOT_FOUND the name was not found in the table
             OS_SUCCESS if success
             
---------------------------------------------------------------------------------------*/

int32 OS_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
{
    uint32 i;

    if(queue_id == NULL || queue_name == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    /* a name too long wouldn't have been allowed in the first place
     * so we definitely won't find a name too long*/
    if (strlen(queue_name) >= OS_MAX_API_NAME)
    {
       return OS_ERR_NAME_TOO_LONG;
    }
    
    for (i = 0; i < OS_MAX_QUEUES; i++)
    {
        if (OS_queue_table[i].free != TRUE &&
           (strcmp(OS_queue_table[i].name, (char*) queue_name) == 0 ))
        {
            *queue_id = i;
            return OS_SUCCESS;
        }
    }

    /* 
    ** The name was not found in the table,
    ** or it was, and the queue_id isn't valid anymore 
    */
    return OS_ERR_NAME_NOT_FOUND;

}/* end OS_QueueGetIdByName */

/*---------------------------------------------------------------------------------------
    Name: OS_QueueGetInfo

    Purpose: This function will pass back a pointer to structure that contains 
             all of the relevant info (name and creator) about the specified queue. 

    Returns: OS_INVALID_POINTER if queue_prop is NULL
             OS_ERR_INVALID_ID if the ID given is not  a valid queue
             OS_SUCCESS if the info was copied over correctly
---------------------------------------------------------------------------------------*/

int32 OS_QueueGetInfo (uint32 queue_id, OS_queue_prop_t *queue_prop)  
{
    rtems_status_code status;

    /* Check to see that the id given is valid */
    if (queue_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    if (queue_id >= OS_MAX_QUEUES || OS_queue_table[queue_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    /* put the info into the stucture */
    status = rtems_semaphore_obtain (OS_queue_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    queue_prop -> creator =   OS_queue_table[queue_id].creator;
    strcpy(queue_prop -> name, OS_queue_table[queue_id].name);
    status = rtems_semaphore_release (OS_queue_table_sem);

    return OS_SUCCESS;
    
} /* end OS_QueueGetInfo */
/****************************************************************************************
                                  SEMAPHORE API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_BinSemCreate

   Purpose: Creates a binary semaphore with initial value specified by
            sem_initial_value and name specified by sem_name. sem_id will be 
            returned to the caller
            
   Returns: OS_INVALID_POINTER if sen name or sem_id are NULL
            OS_ERR_NAME_TOO_LONG if the name given is too long
            OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
            OS_ERR_NAME_TAKEN if this is already the name of a binary semaphore
            OS_SEM_FAILURE if the OS call failed
            OS_SUCCESS if success
            

   Notes: options is an unused parameter 
---------------------------------------------------------------------------------------*/

int32 OS_BinSemCreate (uint32 *sem_id, const char *sem_name, uint32 sem_initial_value, 
                        uint32 options)
{
    rtems_status_code status;
    uint32            possible_semid;
    uint32            i;
    rtems_name        r_name;

    if (sem_id == NULL || sem_name == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */
    if (strlen(sem_name) >= OS_MAX_API_NAME)
    {
            return OS_ERR_NAME_TOO_LONG;
    }
    
    /* Check Parameters */
    status = rtems_semaphore_obtain (OS_bin_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for (possible_semid = 0; possible_semid < OS_MAX_BIN_SEMAPHORES; possible_semid++)
    {
        if (OS_bin_sem_table[possible_semid].free == TRUE)    
            break;
    }
    
    if((possible_semid >= OS_MAX_BIN_SEMAPHORES) ||  
       (OS_bin_sem_table[possible_semid].free != TRUE))
    {
        status = rtems_semaphore_release (OS_bin_sem_table_sem);
        return OS_ERR_NO_FREE_IDS;
    }
    
    /* Check to see if the name is already taken */
    for (i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        if ((OS_bin_sem_table[i].free == FALSE) &&
                strcmp ((char*) sem_name, OS_bin_sem_table[i].name) == 0)
        {
            status = rtems_semaphore_release (OS_bin_sem_table_sem);
            return OS_ERR_NAME_TAKEN;
        }
    }
    OS_bin_sem_table[possible_semid].free = FALSE;
    status = rtems_semaphore_release (OS_bin_sem_table_sem);

    /* Check to make sure the sem value is going to be either 0 or 1 */
    if (sem_initial_value > 1)
    {
        sem_initial_value = 1;
    }
    
    /* Create RTEMS Semaphore */
    r_name = rtems_build_name(sem_name[0],sem_name[1],sem_name[2],sem_name[3]);
    status = rtems_semaphore_create( r_name, sem_initial_value, 
                                     OSAL_BINARY_SEM_ATTRIBS,
                                     0,
                                     &(OS_bin_sem_table[possible_semid].id));
    
    /* check if Create failed */
    if ( status != RTEMS_SUCCESSFUL )
    {
        status = rtems_semaphore_obtain (OS_bin_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        OS_bin_sem_table[possible_semid].free = TRUE;
        OS_bin_sem_table[possible_semid].id = 0;
        status = rtems_semaphore_release (OS_bin_sem_table_sem);
        return OS_SEM_FAILURE;
    }
    /* Set the sem_id to the one that we found available */
    /* Set the name of the semaphore,creator and free as well */

    *sem_id = possible_semid;
    
    status = rtems_semaphore_obtain (OS_bin_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    OS_bin_sem_table[*sem_id].free = FALSE;
    strcpy(OS_bin_sem_table[*sem_id].name , (char*) sem_name);
    OS_bin_sem_table[*sem_id].creator = OS_FindCreator();
    status = rtems_semaphore_release (OS_bin_sem_table_sem);
    
    return OS_SUCCESS;
    
}/* end OS_BinSemCreate */


/*--------------------------------------------------------------------------------------
     Name: OS_BinSemDelete

    Purpose: Deletes the specified Binary Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success
    
---------------------------------------------------------------------------------------*/

int32 OS_BinSemDelete (uint32 sem_id)
{
    rtems_status_code status;

    /* Check to see if this sem_id is valid */
    if (sem_id >= OS_MAX_BIN_SEMAPHORES || OS_bin_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    /* we must make sure the semaphore is given  to delete it */
    rtems_semaphore_release(OS_bin_sem_table[sem_id].id);
    
    if (rtems_semaphore_delete( OS_bin_sem_table[sem_id].id) != RTEMS_SUCCESSFUL) 
    {
	return OS_SEM_FAILURE;
    }
    
    /* Remove the Id from the table, and its name, so that it cannot be found again */
    status = rtems_semaphore_obtain (OS_bin_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    OS_bin_sem_table[sem_id].free = TRUE;
    OS_bin_sem_table[sem_id].name[0] = '\0';
    OS_bin_sem_table[sem_id].creator = UNINITIALIZED;
    OS_bin_sem_table[sem_id].id = UNINITIALIZED;
    status = rtems_semaphore_release (OS_bin_sem_table_sem);

    return OS_SUCCESS;

}/* end OS_BinSemDelete */

/*---------------------------------------------------------------------------------------
    Name: OS_BinSemGive

    Purpose: The function  unlocks the semaphore referenced by sem_id by performing
             a semaphore unlock operation on that semaphore.If the semaphore value 
             resulting from this operation is positive, then no threads were blocked
             waiting for the semaphore to become unlocked; the semaphore value is
             simply incremented for this semaphore.

    
    Returns: OS_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
             OS_SUCCESS if success
                
---------------------------------------------------------------------------------------*/

int32 OS_BinSemGive (uint32 sem_id)
{
    /* Check Parameters */
    if(sem_id >= OS_MAX_BIN_SEMAPHORES || OS_bin_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if( rtems_semaphore_release(OS_bin_sem_table[sem_id].id) != RTEMS_SUCCESSFUL)
    {
        return OS_SEM_FAILURE ;
    }
    else
    {
        return  OS_SUCCESS ;
    }

}/* end OS_BinSemGive */

/*---------------------------------------------------------------------------------------
    Name: OS_BinSemFlush

    Purpose: The function  releases all the tasks pending on this semaphore. Note
             that the state of the semaphore is not changed by this operation.
    
    Returns: OS_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
             OS_SUCCESS if success
                
---------------------------------------------------------------------------------------*/

int32 OS_BinSemFlush (uint32 sem_id)
{
    /* Check Parameters */
    if(sem_id >= OS_MAX_BIN_SEMAPHORES || OS_bin_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    /* Give Semaphore */
    if( rtems_semaphore_flush(OS_bin_sem_table[sem_id].id) != RTEMS_SUCCESSFUL)
    {
	return OS_SEM_FAILURE ;
    }
    else
    {
	return  OS_SUCCESS ;
    }

}/* end OS_BinSemFlush */

/*---------------------------------------------------------------------------------------
    Name:    OS_BinSemTake

    Purpose: The locks the semaphore referenced by sem_id by performing a 
             semaphore lock operation on that semaphore.If the semaphore value 
             is currently zero, then the calling thread shall not return from 
             the call until it either locks the semaphore or the call is 
             interrupted by a signal.

    Return:  OS_ERR_INVALID_ID : the semaphore was not previously initialized
             or is not in the array of semaphores defined by the system
             OS_SEM_FAILURE if the OS call failed and the semaphore is not obtained
             OS_SUCCESS if success
             
----------------------------------------------------------------------------------------*/

int32 OS_BinSemTake (uint32 sem_id)
{
    rtems_status_code status;

    /* Check Parameters */
    if(sem_id >= OS_MAX_BIN_SEMAPHORES  || OS_bin_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    status = rtems_semaphore_obtain(OS_bin_sem_table[sem_id].id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if ( status == RTEMS_SUCCESSFUL || status == RTEMS_UNSATISFIED ) 
    {
        /*
        ** If the semaphore is flushed, this function will return
        ** RTEMS_UNSATISFIED. If this happens, the OSAL does not want to return
        ** an error, it would be inconsistent with the other ports
        ** 
        ** I currently do not know of any other reasons this call would return 
        **  RTEMS_UNSATISFIED, so I think it is OK.
        */
        return OS_SUCCESS;
    }
    else
    {
        return OS_SEM_FAILURE;
    }

}/* end OS_BinSemTake */
/*---------------------------------------------------------------------------------------
    Name: OS_BinSemTimedWait
    
    Purpose: The function locks the semaphore referenced by sem_id . However,
             if the semaphore cannot be locked without waiting for another process
             or thread to unlock the semaphore , this wait shall be terminated when 
             the specified timeout ,msecs, expires.

    Returns: OS_SEM_TIMEOUT if semaphore was not relinquished in time
             OS_SUCCESS if success
             OS_SEM_FAILURE the semaphore was not previously initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
----------------------------------------------------------------------------------------*/

int32 OS_BinSemTimedWait (uint32 sem_id, uint32 msecs)
{
    rtems_status_code status;
    uint32            TimeInTicks;

    /* Check Parameters */
    if( (sem_id >= OS_MAX_BIN_SEMAPHORES) || (OS_bin_sem_table[sem_id].free == TRUE) )
    {
        return OS_ERR_INVALID_ID;	
    }
		
    TimeInTicks = OS_Milli2Ticks(msecs);
    
    status  = 	rtems_semaphore_obtain(OS_bin_sem_table[sem_id].id, RTEMS_WAIT,TimeInTicks ) ;
	
    switch (status)
    {
        case RTEMS_TIMEOUT :
            
            status = OS_SEM_TIMEOUT ;
            break ;
		
        case RTEMS_SUCCESSFUL :
            status = OS_SUCCESS ;
            break ;

         default :
            status = OS_SEM_FAILURE ;
            break ;
    }
    return status;

}/* end OS_BinSemTimedWait */

/*--------------------------------------------------------------------------------------
    Name: OS_BinSemGetIdByName

    Purpose: This function tries to find a binary sem Id given the name of a bin_sem
             The id is returned through sem_id

    Returns: OS_INVALID_POINTER is semid or sem_name are NULL pointers
             OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
             OS_ERR_NAME_NOT_FOUND if the name was not found in the table
             OS_SUCCESS if success
             
---------------------------------------------------------------------------------------*/
int32 OS_BinSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
    uint32 i;

    if (sem_id == NULL || sem_name == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    /* 
    ** a name too long wouldn't have been allowed in the first place
    ** so we definitely won't find a name too long
    */
    if (strlen(sem_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    for (i = 0; i < OS_MAX_BIN_SEMAPHORES; i++)
    {
        if (OS_bin_sem_table[i].free != TRUE &&
           (strcmp (OS_bin_sem_table[i].name , (char*) sem_name) == 0) )
        {
            *sem_id = i;
            return OS_SUCCESS;
        }
    }
    /* 
    ** The name was not found in the table,
    ** or it was, and the sem_id isn't valid anymore 
    */
    return OS_ERR_NAME_NOT_FOUND;
    
}/* end OS_BinSemGetIdByName */
/*---------------------------------------------------------------------------------------
    Name: OS_BinSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains 
             all of the relevant info( name and creator) about the specified binary
             semaphore.
             
    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid semaphore 
             OS_INVALID_POINTER if the bin_prop pointer is null
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_BinSemGetInfo (uint32 sem_id, OS_bin_sem_prop_t *bin_prop)  
{
    rtems_status_code status;

    /* Check to see that the id given is valid */
    if (sem_id >= OS_MAX_BIN_SEMAPHORES || OS_bin_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if (bin_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* put the info into the stucture */
    status = rtems_semaphore_obtain (OS_bin_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    bin_prop ->creator =    OS_bin_sem_table[sem_id].creator;
    strcpy(bin_prop-> name, OS_bin_sem_table[sem_id].name);
    bin_prop -> value = 0;
    
    status = rtems_semaphore_release (OS_bin_sem_table_sem);

    return OS_SUCCESS;
    
} /* end OS_BinSemGetInfo */
/*---------------------------------------------------------------------------------------
   Name: OS_CountSemCreate

   Purpose: Creates a countary semaphore with initial value specified by
            sem_initial_value and name specified by sem_name. sem_id will be 
            returned to the caller
            
   Returns: OS_INVALID_POINTER if sen name or sem_id are NULL
            OS_ERR_NAME_TOO_LONG if the name given is too long
            OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
            OS_ERR_NAME_TAKEN if this is already the name of a countary semaphore
            OS_SEM_FAILURE if the OS call failed
            OS_SUCCESS if success

   Notes: options is an unused parameter 
---------------------------------------------------------------------------------------*/

int32 OS_CountSemCreate (uint32 *sem_id, const char *sem_name, uint32 sem_initial_value, 
                        uint32 options)
{
    rtems_status_code status;
    uint32            possible_semid;
    rtems_name        r_name;
    uint32            i;

    /* 
    ** Check Parameters 
    */
    if (sem_id == NULL || sem_name == NULL)
    {
        return OS_INVALID_POINTER;
    }
   
    /*
    ** Verify that the semaphore maximum value is not too high
    */
    if ( sem_initial_value > MAX_SEM_VALUE )
    {
        return OS_INVALID_SEM_VALUE;
    }
 
    /*
    **  we don't want to allow names too long
    ** if truncated, two names might be the same 
    */
    if (strlen(sem_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }
   
    /*
    ** Lock
    */ 
    status = rtems_semaphore_obtain (OS_count_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for (possible_semid = 0; possible_semid < OS_MAX_COUNT_SEMAPHORES; possible_semid++)
    {
        if (OS_count_sem_table[possible_semid].free == TRUE)    
            break;
    }
    
    if((possible_semid >= OS_MAX_COUNT_SEMAPHORES) ||  
       (OS_count_sem_table[possible_semid].free != TRUE))
    {
        status = rtems_semaphore_release (OS_count_sem_table_sem);
        return OS_ERR_NO_FREE_IDS;
    }
    
    /* Check to see if the name is already taken */
    for (i = 0; i < OS_MAX_COUNT_SEMAPHORES; i++)
    {
        if ((OS_count_sem_table[i].free == FALSE) &&
             strcmp ((char*) sem_name, OS_count_sem_table[i].name) == 0)
        {
            status = rtems_semaphore_release (OS_count_sem_table_sem);
            return OS_ERR_NAME_TAKEN;
        }
    }
    OS_count_sem_table[possible_semid].free = FALSE;
    status = rtems_semaphore_release (OS_count_sem_table_sem);

    /* Create RTEMS Semaphore */
    r_name = rtems_build_name(sem_name[0],sem_name[1],sem_name[2],sem_name[3]);
    status = rtems_semaphore_create( r_name, sem_initial_value, 
                                     OSAL_COUNT_SEM_ATTRIBS,
                                     0,
                                     &(OS_count_sem_table[possible_semid].id));
    
    /* check if Create failed */
    if ( status != RTEMS_SUCCESSFUL )
    {        
        status = rtems_semaphore_obtain (OS_count_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        OS_count_sem_table[possible_semid].free = TRUE;
        status = rtems_semaphore_release (OS_count_sem_table_sem);

	return OS_SEM_FAILURE;
    }
    /* Set the sem_id to the one that we found available */
    /* Set the name of the semaphore,creator and free as well */

    *sem_id = possible_semid;
    
    status = rtems_semaphore_obtain (OS_count_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    OS_count_sem_table[*sem_id].free = FALSE;
    strcpy(OS_count_sem_table[*sem_id].name , (char*) sem_name);
    OS_count_sem_table[*sem_id].creator = OS_FindCreator();
   
    /*
    ** Unlock
    */ 
    status = rtems_semaphore_release (OS_count_sem_table_sem);
    
    return OS_SUCCESS;
    
}/* end OS_CountSemCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_CountSemDelete

    Purpose: Deletes the specified Counting Semaphore.

    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid countary semaphore
             OS_SEM_FAILURE the OS call failed
             OS_SUCCESS if success
    
---------------------------------------------------------------------------------------*/

int32 OS_CountSemDelete (uint32 sem_id)
{
    rtems_status_code status;

    /* 
    ** Check to see if this sem_id is valid 
    */
    if (sem_id >= OS_MAX_COUNT_SEMAPHORES || OS_count_sem_table[sem_id].free == TRUE)
    {
      return OS_ERR_INVALID_ID;
    }

    /* we must make sure the semaphore is given  to delete it */
    rtems_semaphore_release(OS_count_sem_table[sem_id].id);
    
    if (rtems_semaphore_delete( OS_count_sem_table[sem_id].id) != RTEMS_SUCCESSFUL) 
    {
	return OS_SEM_FAILURE;
    }
    
    /* Remove the Id from the table, and its name, so that it cannot be found again */
    status = rtems_semaphore_obtain (OS_count_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    OS_count_sem_table[sem_id].free = TRUE;
    OS_count_sem_table[sem_id].name[0] = '\0';
    OS_count_sem_table[sem_id].creator = UNINITIALIZED;
    OS_count_sem_table[sem_id].id = UNINITIALIZED;
    
    status = rtems_semaphore_release (OS_count_sem_table_sem);

    return OS_SUCCESS;

}/* end OS_CountSemDelete */

/*---------------------------------------------------------------------------------------
    Name: OS_CountSemGive

    Purpose: The function  unlocks the semaphore referenced by sem_id by performing
             a semaphore unlock operation on that semaphore.If the semaphore value 
             resulting from this operation is positive, then no threads were blocked             
             waiting for the semaphore to become unlocked; the semaphore value is
             simply incremented for this semaphore.

    
    Returns: OS_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the id passed in is not a countary semaphore
             OS_SUCCESS if success
                
---------------------------------------------------------------------------------------*/

int32 OS_CountSemGive (uint32 sem_id)
{
    int32             return_code = OS_SUCCESS;

    /* 
    ** Check Parameters 
    */
    if(sem_id >= OS_MAX_COUNT_SEMAPHORES || OS_count_sem_table[sem_id].free == TRUE)
    {
       return OS_ERR_INVALID_ID;
    }

    if( rtems_semaphore_release(OS_count_sem_table[sem_id].id) != RTEMS_SUCCESSFUL)
    {
        return_code = OS_SEM_FAILURE ;
    }
    else
    {
        return_code =  OS_SUCCESS ;
    }
    return(return_code);

}/* end OS_CountSemGive */

/*---------------------------------------------------------------------------------------
    Name:    OS_CountSemTake

    Purpose: The locks the semaphore referenced by sem_id by performing a 
             semaphore lock operation on that semaphore.If the semaphore value 
             is currently zero, then the calling thread shall not return from 
             the call until it either locks the semaphore or the call is 
             interrupted by a signal.

    Return:  OS_SEM_FAILURE : the semaphore was not previously initialized
             or is not in the array of semaphores defined by the system
             OS_ERR_INVALID_ID the Id passed in is not a valid countar semaphore
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success
             
----------------------------------------------------------------------------------------*/

int32 OS_CountSemTake (uint32 sem_id)
{
    int32  return_code = OS_SUCCESS;

    /* Check Parameters */
    if(sem_id >= OS_MAX_COUNT_SEMAPHORES  || OS_count_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if ( rtems_semaphore_obtain(OS_count_sem_table[sem_id].id, RTEMS_WAIT, 
                                RTEMS_NO_TIMEOUT)!= RTEMS_SUCCESSFUL)
    {
        return_code =  OS_SEM_FAILURE;
    }
    return(return_code);

}/* end OS_CountSemTake */


/*---------------------------------------------------------------------------------------
    Name: OS_CountSemTimedWait
    
    Purpose: The function locks the semaphore referenced by sem_id . However,
             if the semaphore cannot be locked without waiting for another process
             or thread to unlock the semaphore , this wait shall be terminated when 
             the specified timeout ,msecs, expires.

    Returns: OS_SEM_TIMEOUT if semaphore was not relinquished in time
             OS_SUCCESS if success
             OS_SEM_FAILURE the semaphore was not previously initialized or is not
             in the array of semaphores defined by the system
             OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
----------------------------------------------------------------------------------------*/

int32 OS_CountSemTimedWait (uint32 sem_id, uint32 msecs)
{
    rtems_status_code status;
    int32             return_code = OS_SUCCESS;
    uint32            TimeInTicks;

    /* Check Parameters */
    if( (sem_id >= OS_MAX_COUNT_SEMAPHORES) || (OS_count_sem_table[sem_id].free == TRUE) )
    {
        return OS_ERR_INVALID_ID;	
    }
		
    TimeInTicks = OS_Milli2Ticks(msecs);
    
    status = rtems_semaphore_obtain(OS_count_sem_table[sem_id].id, RTEMS_WAIT,TimeInTicks ) ;
    switch (status)
    {
        case RTEMS_TIMEOUT :
	   return_code = OS_SEM_TIMEOUT ;
           break ;
		
        case RTEMS_SUCCESSFUL :
           return_code = OS_SUCCESS ;
           break ;
		
        default :
           return_code = OS_SEM_FAILURE ;
           break ;
    }
    return(return_code);

}/* end OS_CountSemTimedWait */

/*--------------------------------------------------------------------------------------
    Name: OS_CountSemGetIdByName

    Purpose: This function tries to find a countary sem Id given the name of a count_sem
             The id is returned through sem_id

    Returns: OS_INVALID_POINTER is semid or sem_name are NULL pointers
             OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
             OS_ERR_NAME_NOT_FOUND if the name was not found in the table
             OS_SUCCESS if success
             
---------------------------------------------------------------------------------------*/
int32 OS_CountSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
    uint32 i;

    if (sem_id == NULL || sem_name == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    /* 
    ** a name too long wouldn't have been allowed in the first place
    ** so we definitely won't find a name too long
    */
    if (strlen(sem_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    for (i = 0; i < OS_MAX_COUNT_SEMAPHORES; i++)
    {
        if (OS_count_sem_table[i].free != TRUE &&
           (strcmp (OS_count_sem_table[i].name , (char*) sem_name) == 0) )
        {
            *sem_id = i;
            return OS_SUCCESS;
        }
    }

    /* 
    ** The name was not found in the table,
    **  or it was, and the sem_id isn't valid anymore 
    */
    return OS_ERR_NAME_NOT_FOUND;
    
}/* end OS_CountSemGetIdByName */
/*---------------------------------------------------------------------------------------
    Name: OS_CountSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains 
             all of the relevant info( name and creator) about the specified countary
             semaphore.
             
    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid semaphore 
             OS_INVALID_POINTER if the count_prop pointer is null
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_CountSemGetInfo (uint32 sem_id, OS_count_sem_prop_t *count_prop)  
{
    rtems_status_code status; 

    /* Check to see that the id given is valid */
    if (sem_id >= OS_MAX_COUNT_SEMAPHORES || OS_count_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if (count_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }
   
    /*
    ** Lock
    */ 
    status = rtems_semaphore_obtain (OS_count_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    
    /* 
    ** Populate the info stucture 
    */
    count_prop ->creator =    OS_count_sem_table[sem_id].creator;
    strcpy(count_prop-> name, OS_count_sem_table[sem_id].name);
    count_prop -> value = 0;
   
    /*
    ** Unlock 
    */ 
    status = rtems_semaphore_release (OS_count_sem_table_sem);

    return OS_SUCCESS;
    
} /* end OS_CountSemGetInfo */

/****************************************************************************************
                                  MUTEX API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
    Name: OS_MutSemCreate

    Purpose: Creates a mutex semaphore initially full.

    Returns: OS_INVALID_POINTER if sem_id or sem_name are NULL
             OS_ERR_NAME_TOO_LONG if the sem_name is too long to be stored
             OS_ERR_NO_FREE_IDS if there are no more free mutex Ids
             OS_ERR_NAME_TAKEN if there is already a mutex with the same name
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success
    
    Notes: the options parameter is not used in this implementation

---------------------------------------------------------------------------------------*/

int32 OS_MutSemCreate (uint32 *sem_id, const char *sem_name, uint32 options)
{
    uint32	        possible_semid;
    uint32	        i;	    
    rtems_status_code   status; 
    rtems_name          r_name;

    /* Check Parameters */
    if (sem_id == NULL || sem_name == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    /* 
    ** we don't want to allow names too long
    ** if truncated, two names might be the same 
    */
    if (strlen(sem_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    status = rtems_semaphore_obtain (OS_mut_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for (possible_semid = 0; possible_semid < OS_MAX_MUTEXES; possible_semid++)
    {
        if (OS_mut_sem_table[possible_semid].free == TRUE)    
            break;
    }
    
    if( (possible_semid >= OS_MAX_MUTEXES) ||
        (OS_mut_sem_table[possible_semid].free != TRUE) )
    {
        status = rtems_semaphore_release (OS_mut_sem_table_sem);
        return OS_ERR_NO_FREE_IDS;
    }
    
    /* Check to see if the name is already taken */
    for (i = 0; i < OS_MAX_MUTEXES; i++)
    {
        if ((OS_mut_sem_table[i].free == FALSE) &&
                strcmp ((char*)sem_name, OS_mut_sem_table[i].name) == 0)
        {
            status = rtems_semaphore_release (OS_mut_sem_table_sem);
            return OS_ERR_NAME_TAKEN;
        }
    }
    
    OS_mut_sem_table[possible_semid].free = FALSE;
    status = rtems_semaphore_release (OS_mut_sem_table_sem);

    /*
    ** Try to create the mutex
    */
    r_name = rtems_build_name(sem_name[0],sem_name[1],sem_name[2],sem_name[3]);
    status = rtems_semaphore_create ( r_name, 1,  
                                      OSAL_MUTEX_ATTRIBS ,
                                      0,
                                      &OS_mut_sem_table[possible_semid].id );
    if ( status != RTEMS_SUCCESSFUL )
    {
        status = rtems_semaphore_obtain (OS_mut_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        OS_mut_sem_table[possible_semid].free = TRUE;
        status = rtems_semaphore_release (OS_mut_sem_table_sem);
        return OS_SEM_FAILURE;
    } 

    *sem_id = possible_semid;

    status = rtems_semaphore_obtain (OS_mut_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    strcpy(OS_mut_sem_table[*sem_id].name, (char*)sem_name);
    OS_mut_sem_table[*sem_id].free = FALSE;
    OS_mut_sem_table[*sem_id].creator = OS_FindCreator();
    status = rtems_semaphore_release (OS_mut_sem_table_sem);
    
    return OS_SUCCESS;

}/* end OS_MutSemCreate */

/*--------------------------------------------------------------------------------------
     Name: OS_MutSemDelete

    Purpose: Deletes the specified Mutex Semaphore.
    
    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex
             OS_SEM_FAILURE if the OS call failed
             OS_SUCCESS if success

---------------------------------------------------------------------------------------*/

int32 OS_MutSemDelete (uint32 sem_id)
{
    rtems_status_code status; 

    /* Check to see if this sem_id is valid   */
    if (sem_id >= OS_MAX_MUTEXES || OS_mut_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if (rtems_semaphore_delete( OS_mut_sem_table[sem_id].id) != RTEMS_SUCCESSFUL)
    {
        /* clean up? */
        return OS_SEM_FAILURE;
    }

    /* Delete its presence in the table */
    status = rtems_semaphore_obtain (OS_mut_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    OS_mut_sem_table[sem_id].free = TRUE;
    OS_mut_sem_table[sem_id].id = UNINITIALIZED;
    OS_mut_sem_table[sem_id].name[0] = '\0';
    OS_mut_sem_table[sem_id].creator = UNINITIALIZED;
    status = rtems_semaphore_release (OS_mut_sem_table_sem);

    return OS_SUCCESS;

}/* end OS_MutSemDelete */


/*---------------------------------------------------------------------------------------
    Name: OS_MutSemGive

    Purpose: The function releases the mutex object referenced by sem_id.The 
             manner in which a mutex is released is dependent upon the mutex's type 
             attribute.  If there are threads blocked on the mutex object referenced by 
             mutex when this function is called, resulting in the mutex becoming 
             available, the scheduling policy shall determine which thread shall 
             acquire the mutex.

    Returns: OS_SUCCESS if success
             OS_SEM_FAILURE if the semaphore was not previously  initialized 
             OS_ERR_INVALID_ID if the id passed in is not a valid mutex

---------------------------------------------------------------------------------------*/

int32 OS_MutSemGive (uint32 sem_id)
{
    /* Check Parameters */
    if(sem_id >= OS_MAX_MUTEXES || OS_mut_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    /* Give the mutex */
    if( rtems_semaphore_release(OS_mut_sem_table[sem_id].id) != RTEMS_SUCCESSFUL)
    {
        return OS_SEM_FAILURE ;
    }
    else
    {
        return  OS_SUCCESS ;
    }

}/* end OS_MutSemGive */

/*---------------------------------------------------------------------------------------
    Name: OS_MutSemTake

    Purpose: The mutex object referenced by sem_id shall be locked by calling this
             function. If the mutex is already locked, the calling thread shall
             block until the mutex becomes available. This operation shall return
             with the mutex object referenced by mutex in the locked state with the
             calling thread as its owner.

    Returns: OS_SUCCESS if success
             OS_SEM_FAILURE if the semaphore was not previously initialized or is 
             not in the array of semaphores defined by the system
             OS_ERR_INVALID_ID the id passed in is not a valid mutex
---------------------------------------------------------------------------------------*/
int32 OS_MutSemTake (uint32 sem_id)
{
   /* 
   ** Check Parameters 
   */
   if(sem_id >= OS_MAX_MUTEXES || OS_mut_sem_table[sem_id].free == TRUE)
   {
      return OS_ERR_INVALID_ID;
   }

   if ( rtems_semaphore_obtain(OS_mut_sem_table[sem_id].id, RTEMS_WAIT,
                                RTEMS_NO_TIMEOUT)!= RTEMS_SUCCESSFUL)
   {
       return OS_SEM_FAILURE;
   }
   else
   {
       return OS_SUCCESS;
   }

}/* end OS_MutSemGive */

/*--------------------------------------------------------------------------------------
    Name: OS_MutSemGetIdByName

    Purpose: This function tries to find a mutex sem Id given the name of a bin_sem
             The id is returned through sem_id

    Returns: OS_INVALID_POINTER is semid or sem_name are NULL pointers
             OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
             OS_ERR_NAME_NOT_FOUND if the name was not found in the table
             OS_SUCCESS if success
             
---------------------------------------------------------------------------------------*/

int32 OS_MutSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
    uint32 i;

    if(sem_id == NULL || sem_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* a name too long wouldn't have been allowed in the first place
     * so we definitely won't find a name too long*/
    if (strlen(sem_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    for (i = 0; i < OS_MAX_MUTEXES; i++)
    {
        if ((OS_mut_sem_table[i].free != TRUE) &&
           (strcmp (OS_mut_sem_table[i].name, (char*) sem_name) == 0))
        {
            *sem_id = i;
            return OS_SUCCESS;
        }
    }
    
    /* The name was not found in the table,
     *  or it was, and the sem_id isn't valid anymore */
 
    return OS_ERR_NAME_NOT_FOUND;

}/* end OS_MutSemGetIdByName */
/*---------------------------------------------------------------------------------------
    Name: OS_MutSemGetInfo

    Purpose: This function will pass back a pointer to structure that contains 
             all of the relevant info( name and creator) about the specified mutex
             semaphore.
             
    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid semaphore 
             OS_INVALID_POINTER if the mut_prop pointer is null
             OS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_MutSemGetInfo (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)  
{
    rtems_status_code status; 
    
    /* Check to see that the id given is valid */
    if (sem_id >= OS_MAX_MUTEXES || OS_mut_sem_table[sem_id].free == TRUE)
    {
        return OS_ERR_INVALID_ID;
    }

    if (mut_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }
    
    /* put the info into the stucture */    
    status = rtems_semaphore_obtain (OS_mut_sem_table_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    mut_prop -> creator =   OS_mut_sem_table[sem_id].creator;
    strcpy(mut_prop-> name, OS_mut_sem_table[sem_id].name);
    status = rtems_semaphore_release (OS_mut_sem_table_sem);

    return OS_SUCCESS;
    
} /* end OS_MutSemGetInfo */

/****************************************************************************************
                                    TICK API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_Milli2Ticks

   Purpose: This function accepts a time interval in milliseconds as input an
            returns the tick equivalent is o.s. system clock ticks. The tick
            value is rounded up.  This algorthim should change to use a integer divide.
---------------------------------------------------------------------------------------*/

int32 OS_Milli2Ticks (uint32 milli_seconds)
{
    uint32 num_of_ticks;
    uint32 tick_duration_usec;
	
    tick_duration_usec = OS_Tick2Micros();
    num_of_ticks = ((milli_seconds * 1000) + tick_duration_usec - 1)/tick_duration_usec;

    return(num_of_ticks) ; 
}/* end OS_Milli2Ticks */

/*---------------------------------------------------------------------------------------
   Name: OS_Tick2Micros

   Purpose: This function returns the duration of a system tick in micro seconds.
---------------------------------------------------------------------------------------*/

int32 OS_Tick2Micros (void)
{
    /* 
    ** sysconf(_SC_CLK_TCK) returns ticks/second.
    */
    return(1000000/sysconf(_SC_CLK_TCK));
	    
}/* end OS_InfoGetTicks */

/*---------------------------------------------------------------------------------------
 * Name: OS_GetLocalTime
 * 
 * Purpose: This functions get the local time of the machine its on
 * ------------------------------------------------------------------------------------*/

int32 OS_GetLocalTime(OS_time_t *time_struct)
{
   int               status;
   struct  timespec  time;

   if (time_struct == NULL)
   {
      return OS_INVALID_POINTER;
   }
   
   status = clock_gettime(CLOCK_REALTIME, &time);
   if (status != 0)
   {
        return OS_ERROR;
   }

   time_struct -> seconds = time.tv_sec;
   time_struct -> microsecs = time.tv_nsec / 1000;

   return OS_SUCCESS;

} /* end OS_GetLocalTime */

/*---------------------------------------------------------------------------------------
 * Name: OS_SetLocalTime
 * 
 * Purpose: This function sets the local time of the machine its on
 * ------------------------------------------------------------------------------------*/

int32 OS_SetLocalTime(OS_time_t *time_struct)
{
   int               status;
   struct  timespec  time;

   if (time_struct == NULL)
   {
      return OS_INVALID_POINTER;
   }
   
   time.tv_sec = time_struct -> seconds;
   time.tv_nsec = (time_struct -> microsecs) * 1000;

   status = clock_settime(CLOCK_REALTIME, &time);
   if (status != 0)
   {
        return OS_ERROR;
   }

   return OS_SUCCESS;

} /* end OS_SetLocalTime */

/****************************************************************************************
                                 INT API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_IntAttachHandler

   Purpose: The call associates a specified C routine to a specified interrupt
            number.Upon occurring of the InterruptNumber the InerruptHandler
            routine will be called and passed the parameter.

   Parameters:
        InterruptNumber : The Interrupt Number that will cause the start of the ISR
        InerruptHandler : The ISR associatd with this interrupt
        parameter :The parameter that is passed to the ISR

---------------------------------------------------------------------------------------*/
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
   rtems_status_code ret_status;
   uint32 status ;
   rtems_isr_entry old_handler;
	
   ret_status = rtems_interrupt_catch( 
		(rtems_isr_entry)InterruptHandler,
		(rtems_vector_number)InterruptNumber,
		&old_handler);

   switch (ret_status) 
   {
       case RTEMS_SUCCESSFUL :
          status = OS_SUCCESS;
	  break ;
		
       case RTEMS_INVALID_NUMBER :
          status = OS_INVALID_INT_NUM;
          break ;
		
       case RTEMS_INVALID_ADDRESS :
          status = OS_INVALID_POINTER;
          break ;
		
    default :
          status = OS_ERROR;
          break ;
    }
    return(status) ;

}/* end OS_IntAttachHandler */
/*---------------------------------------------------------------------------------------
   Name: OS_IntUnlock

   Purpose: Enable previous state of interrupts

   Parameters:
        IntLevel : The Interrupt Level to be reinstated 
---------------------------------------------------------------------------------------*/

int32 OS_IntUnlock (int32 IntLevel)
{
    rtems_interrupt_enable ( (rtems_interrupt_level) IntLevel); 
    return (OS_SUCCESS);

}/* end OS_IntUnlock */

/*---------------------------------------------------------------------------------------
   Name: OS_IntLock

   Purpose: Disable interrupts.

   Parameters:
   
   Returns: Interrupt level    
---------------------------------------------------------------------------------------*/

int32 OS_IntLock (void)
{
   rtems_interrupt_level rtems_int_level;

   rtems_interrupt_disable(rtems_int_level) ; 
   return ( (int32) rtems_int_level) ;

}/* end OS_IntLock */


/*---------------------------------------------------------------------------------------
   Name: OS_IntEnable

   Purpose: Enable previous state of interrupts

   Parameters:
        IntLevel : The Interrupt Level to be reinstated 
---------------------------------------------------------------------------------------*/

int32 OS_IntEnable (int32 Level)
{
    rtems_interrupt_enable ( (rtems_interrupt_level) Level); 
    return(OS_SUCCESS);
}/* end OS_IntEnable */

/*---------------------------------------------------------------------------------------
   Name: OS_IntDisable

   Purpose: Disable the corresponding interrupt number.

   Parameters:
   
   Returns: Interrupt level before OS_IntDisable Call   
---------------------------------------------------------------------------------------*/

int32 OS_IntDisable (int32 Level)
{
   rtems_interrupt_level rtems_int_level;

   rtems_interrupt_disable(rtems_int_level) ; 
   return ( (int32) rtems_int_level) ;
}/* end OS_IntDisable */

/*---------------------------------------------------------------------------------------
   Name: OS_HeapGetInfo

   Purpose: Return current info on the heap

   Parameters:

---------------------------------------------------------------------------------------*/
int32 OS_HeapGetInfo       (OS_heap_prop_t *heap_prop)
{
    region_information_block info;
    int                      status;

    if (heap_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    status = malloc_info( &info );

    if ( status != 0 )
    {
       return(OS_ERROR);
    }

    heap_prop->free_bytes         = (uint32) info.Free.total;
    heap_prop->free_blocks        = (uint32) info.Free.number;
    heap_prop->largest_free_block = (uint32) info.Free.largest;
 
    return (OS_SUCCESS);
}

/*---------------------------------------------------------------------------------------
 *  Name: OS_GetErrorName()
 *  purpose: A handy function to copy the name of the error to a buffer.
---------------------------------------------------------------------------------------*/
int32 OS_GetErrorName(int32 error_num, os_err_name_t * err_name)
{
    os_err_name_t local_name;
    uint32 return_code;

    return_code = OS_SUCCESS;
    
    switch (error_num)
    {
        case OS_SUCCESS: 
            strcpy(local_name,"OS_SUCCESS"); break;
        case OS_ERROR: 
            strcpy(local_name,"OS_ERROR"); break;
        case OS_INVALID_POINTER: 
            strcpy(local_name,"OS_INVALID_POINTER"); break;
        case OS_ERROR_ADDRESS_MISALIGNED: 
            strcpy(local_name,"OS_ADDRESS_MISALIGNED"); break;
        case OS_ERROR_TIMEOUT: 
            strcpy(local_name,"OS_ERROR_TIMEOUT"); break;
        case OS_INVALID_INT_NUM: 
            strcpy(local_name,"OS_INVALID_INT_NUM"); break;
        case OS_SEM_FAILURE:
            strcpy(local_name,"OS_SEM_FAILURE"); break;
        case OS_SEM_TIMEOUT:
            strcpy(local_name,"OS_SEM_TIMEOUT"); break;
        case OS_QUEUE_EMPTY:
            strcpy(local_name,"OS_QUEUE_EMPTY"); break;
        case OS_QUEUE_FULL:
            strcpy(local_name,"OS_QUEUE_FULL"); break;
        case OS_QUEUE_TIMEOUT:
            strcpy(local_name,"OS_QUEUE_TIMEOUT"); break;
        case OS_QUEUE_INVALID_SIZE:
            strcpy(local_name,"OS_QUEUE_INVALID_SIZE"); break;
        case OS_QUEUE_ID_ERROR:
            strcpy(local_name,"OS_QUEUE_ID_ERROR"); break;
        case OS_ERR_NAME_TOO_LONG:
            strcpy(local_name,"OS_ERR_NAME_TOO_LONG"); break;
        case OS_ERR_NO_FREE_IDS:
            strcpy(local_name,"OS_ERR_NO_FREE_IDS"); break;
        case OS_ERR_NAME_TAKEN:
            strcpy(local_name,"OS_ERR_NAME_TAKEN"); break;
        case OS_ERR_INVALID_ID:
            strcpy(local_name,"OS_ERR_INVALID_ID"); break;
        case OS_ERR_NAME_NOT_FOUND:
            strcpy(local_name,"OS_ERR_NAME_NOT_FOUND"); break;
        case OS_ERR_SEM_NOT_FULL:
            strcpy(local_name,"OS_ERR_SEM_NOT_FULL"); break;
        case OS_ERR_INVALID_PRIORITY:
            strcpy(local_name,"OS_ERR_INVALID_PRIORITY"); break;

        default: strcpy(local_name,"ERROR_UNKNOWN");
                 return_code = OS_ERROR;
    }

    strcpy((char*) err_name, local_name);

    return return_code;
}
/*---------------------------------------------------------------------------------------
 * Name: OS_FindCreator
 * Purpose: Finds the creator of a the current task  to store in the table for lookup 
 *          later 
---------------------------------------------------------------------------------------*/

uint32 OS_FindCreator(void)
{
    rtems_id          rtems_task_id;
    int i; 
    /* find the calling task ID */
    rtems_task_ident(RTEMS_SELF, 0, &rtems_task_id);

    for (i = 0; i < OS_MAX_TASKS; i++)
    {
        if (rtems_task_id == OS_task_table[i].id)
            break;
    }
    return i;
}
/* ---------------------------------------------------------------------------
 * Name: OS_printf 
 * 
 * Purpose: This function abstracts out the printf type statements. This is 
 *          useful for using OS- specific thats that will allow non-polled
 *          print statements for the real time systems. 
 *
 ---------------------------------------------------------------------------*/
void OS_printf( const char *String, ...)
{
    va_list     ptr;
    char msg_buffer [OS_BUFFER_SIZE];

    /*
    ** First, check to see if this is being called from an ISR
    ** If it is, return immediately
    */
    if ( rtems_interrupt_is_in_progress())
    {
       return;
    }

    if ( OS_printf_enabled == TRUE )    
    {
       va_start(ptr,String);
       vsnprintf(&msg_buffer[0], (size_t)OS_BUFFER_SIZE, String, ptr);
       va_end(ptr);
    
       msg_buffer[OS_BUFFER_SIZE -1] = '\0';
       printf("%s", &msg_buffer[0]);
    }
    
}/* end OS_printf*/

/* ---------------------------------------------------------------------------
 * Name: OS_printf_disable
 * 
 * Purpose: This function disables the output to the UART from OS_printf.  
 *
 ---------------------------------------------------------------------------*/
void OS_printf_disable(void)
{
   OS_printf_enabled = FALSE;
}/* end OS_printf_disable*/

/* ---------------------------------------------------------------------------
 * Name: OS_printf_enable
 * 
 * Purpose: This function enables the output to the UART through OS_printf.  
 *
 ---------------------------------------------------------------------------*/
void OS_printf_enable(void)
{
   OS_printf_enabled = TRUE;
}/* end OS_printf_enable*/

/*
**
**   Name: OS_FPUExcSetMask
**
**   Purpose: This function sets the FPU exception mask
**
**   Notes: The exception environment is local to each task Therefore this must be
**          called for each task that that wants to do floating point and catch exceptions.
*/
int32 OS_FPUExcSetMask(uint32 mask)
{
    /*
    ** Not implemented in RTEMS.
    */
    return(OS_SUCCESS);
}

/*
**
**   Name: OS_FPUExcGetMask
**
**   Purpose: This function gets the FPU exception mask
**
**   Notes: The exception environment is local to each task Therefore this must be
**          called for each task that that wants to do floating point and catch exceptions.
*/
int32 OS_FPUExcGetMask(uint32 *mask)
{
    /*
    ** Not implemented in RTEMS.
    */
    return(OS_SUCCESS);
}
