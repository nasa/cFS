/*
** File: osapi-os-core.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Ezra Yeheksli -Code 582/Raytheon
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Core OS module
**
*/

#ifndef _osapi_core_
#define _osapi_core_

#include <stdarg.h>   /* for va_list */

/* Defines constants for making object ID's unique */
#define OS_OBJECT_INDEX_MASK        0xFFFF
#define OS_OBJECT_TYPE_SHIFT        16

#define OS_OBJECT_TYPE_UNDEFINED    0x00
#define OS_OBJECT_TYPE_OS_TASK      0x01
#define OS_OBJECT_TYPE_OS_QUEUE     0x02
#define OS_OBJECT_TYPE_OS_COUNTSEM  0x03
#define OS_OBJECT_TYPE_OS_BINSEM    0x04
#define OS_OBJECT_TYPE_OS_MUTEX     0x05
#define OS_OBJECT_TYPE_OS_STREAM    0x06
#define OS_OBJECT_TYPE_OS_DIR       0x07
#define OS_OBJECT_TYPE_OS_TIMEBASE  0x08
#define OS_OBJECT_TYPE_OS_TIMECB    0x09
#define OS_OBJECT_TYPE_OS_MODULE    0x0A
#define OS_OBJECT_TYPE_OS_FILESYS   0x0B
#define OS_OBJECT_TYPE_OS_CONSOLE   0x0C
#define OS_OBJECT_TYPE_USER         0x10

/* Upper limit for OSAL task priorities */
#define OS_MAX_TASK_PRIORITY        255

/*difines constants for OS_BinSemCreate for state of semaphore  */
#define OS_SEM_FULL     1
#define OS_SEM_EMPTY    0

/* #define for enabling floating point operations on a task*/
#define OS_FP_ENABLED 1

/*  tables for the properties of objects */

/*tasks */
typedef struct
{
    char name [OS_MAX_API_NAME];
    uint32 creator;
    uint32 stack_size;
    uint32 priority;
    uint32 OStask_id;
}OS_task_prop_t;
    
/* queues */
typedef struct
{
    char name [OS_MAX_API_NAME];
    uint32 creator;
}OS_queue_prop_t;

/* Binary Semaphores */
typedef struct
{                     
    char name [OS_MAX_API_NAME];
    uint32 creator;
    int32  value;
}OS_bin_sem_prop_t;

/* Counting Semaphores */
typedef struct
{                     
    char name [OS_MAX_API_NAME];
    uint32 creator;
    int32 value;
}OS_count_sem_prop_t;

/* Mutexes */
typedef struct
{
    char name [OS_MAX_API_NAME];
    uint32 creator;
}OS_mut_sem_prop_t;


/* struct for OS_GetLocalTime() */

typedef struct 
{ 
    uint32 seconds; 
    uint32 microsecs;
}OS_time_t; 

/* heap info */
typedef struct
{
    uint32 free_bytes;
    uint32 free_blocks;
    uint32 largest_free_block;
}OS_heap_prop_t;


/* This typedef is for the OS_GetErrorName function, to ensure
 * everyone is making an array of the same length.
 *
 * Implementation note for developers:
 *
 * The sizes of strings in OSAL functions are built with this
 * OS_ERROR_NAME_LENGTH limit in mind.  Always check the uses of os_err_name_t
 * when changing this value.
 */

#define OS_ERROR_NAME_LENGTH     35
typedef char os_err_name_t[OS_ERROR_NAME_LENGTH];

/*
** These typedefs are for the task entry point
*/
typedef void osal_task;
typedef osal_task ((*osal_task_entry)(void));

/*
** Typedef for general purpose OSAL callback functions
** This may be used by multiple APIS
*/
typedef void (*OS_ArgCallback_t)(uint32 object_id, void *arg);


/*
**  External Declarations
*/

/*
** Prototype for application startup function.
** This is implemented by the user application
*/
void OS_Application_Startup(void);


/*
** Exported Functions
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initialization of API
 *
 * Initialize the tables that the OS API uses to keep track of information
 * about objects
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_API_Init (void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Background thread implementation - waits forever for events to occur.
 *
 * This should be called from the BSP main routine / initial thread after all other
 * board / application initialization has taken place and all other tasks are running.
 *
 * Typically just waits forever until "OS_shutdown" flag becomes true.
 */
void OS_IdleLoop (void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief delete all resources created in OSAL.
 *
 * provides a means to clean up all resources allocated by this
 * instance of OSAL.  It would typically be used during an orderly
 * shutdown but may also be helpful for testing purposes.
 */
void OS_DeleteAllObjects       (void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initiate orderly shutdown
 *
 * Indicates that the OSAL application should perform an orderly shutdown
 * of ALL tasks, clean up all resources, and exit the application.
 *
 * This allows the task currently blocked in OS_IdleLoop() to wake up, and
 * for that function to return to its caller.
 *
 * This is preferred over e.g. ApplicationExit() which exits immediately and
 * does not provide for any means to clean up first.
 *
 * @param flag set to true to initiate shutdown, false to cancel
 */
void OS_ApplicationShutdown         (uint8 flag);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the type of an object given an arbitrary object ID
 *
 * Given an arbitrary object ID, get the type of the object
 *
 * @param[in] object_id The object ID to operate on
 *
 * @returns The type of object that the ID represents
 *          #OS_OBJECT_TYPE_OS_TASK for tasks
 *          #OS_OBJECT_TYPE_OS_QUEUE for queues, etc.
 */
uint32 OS_IdentifyObject       (uint32 object_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Converts an abstract ID into a number suitable for use as an array index.
 *
 * This will return a unique zero-based integer number in the range of [0,MAX) for
 * any valid object ID.  This may be used by application code as an array index
 * for indexing into local tables.
 *
 * @note This does NOT verify the validity of the ID, that is left to the caller.
 * This is only the conversion logic.
 *
 * @param[in]  object_id    The object ID to operate on
 *
 * @param[out] *ArrayIndex  The Index to return 
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_ConvertToArrayIndex   (uint32 object_id, uint32 *ArrayIndex);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief call the supplied callback function for all valid object IDs
 *
 * Loops through all defined OSAL objects and calls callback_ptr on each one
 * If creator_id is nonzero then only objects with matching creator id are processed.
 */
void OS_ForEachObject           (uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);


/*
** Task API
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a task and starts running it.
 *
 * @param[out]  task_id will be set to the ID of the newly-created resource
 * @param[in]   task_name the name of the new resource to create
 * @param[in]   function_pointer the entry point of the new task
 * @param[in]   stack_pointer pointer to the stack for the task, or NULL
 *              to allocate a stack from the system memory heap
 * @param[in]   stack_size the size of the stack, or 0 to use a default stack size.
 * @param[in]   priority initial priority of the new task
 * @param[in]   flags initial options for the new task
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if any of the necessary pointers are NULL
 * OS_ERR_NAME_TOO_LONG if the name of the task is too long to be copied
 * OS_ERR_INVALID_PRIORITY if the priority is bad
 * OS_ERR_NO_FREE_IDS if there can be no more tasks created
 * OS_ERR_NAME_TAKEN if the name specified is already used by a task
 * OS_ERROR if an unspecified/other error occurs
 */
int32 OS_TaskCreate            (uint32 *task_id, const char *task_name, 
                                osal_task_entry function_pointer,
                                uint32 *stack_pointer,
                                uint32 stack_size,
                                uint32 priority, uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Task
 *
 * The task will be removed from the local tables. and the OS will
 * be configured to stop executing the task at the next opportunity.
 *
 * @param[in] task_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the ID given to it is invalid
 * OS_ERROR if the OS delete call fails
 */
int32 OS_TaskDelete            (uint32 task_id); 

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Exits the calling task
 *
 * The calling thread is terminated.  This function does not return.
 */
void OS_TaskExit               (void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Installs a handler for when the task is deleted.
 *
 * @param[in]   function_pointer function to be called when task exits
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Delay a task for specified amount of milliseconds
 *
 * @param[in]   millisecond    Amount of time to delay
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if sleep fails or millisecond = 0
 * OS_SUCCESS if success
 */
int32 OS_TaskDelay             (uint32 millisecond);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Sets the given task to a new priority
 *
 * @param[in] task_id        The object ID to operate on
 *
 * @param[in] new_priority   Set the new priority
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the ID passed to it is invalid
 * OS_ERR_INVALID_PRIORITY if the priority is greater than the max allowed
 * OS_ERROR if the OS call to change the priority fails
 */
int32 OS_TaskSetPriority       (uint32 task_id, uint32 new_priority);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Registration to be called by new tasks after creation
 *
 * Obsolete function retained for compatibility purposes.
 * Does Nothing in the current implementation.
 *
 * @returns OS_SUCCESS (always)
 */
int32 OS_TaskRegister          (void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the task id of the calling task
 *
 * This function returns the task id of the calling task
 *
 * @returns Task ID, or zero if the operation failed (zero is never a valid task ID)
 */
uint32 OS_TaskGetId            (void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing task ID by name
 *
 * This function tries to find a task Id given the name of a task
 *
 * @param[out]  task_id will be set to the ID of the existing resource
 * @param[in]   task_name the name of the existing resource to find
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if the pointers passed in are NULL
 * OS_ERR_NAME_TOO_LONG if th ename to found is too long to begin with
 * OS_ERR_NAME_NOT_FOUND if the name wasn't found in the table
 */
int32 OS_TaskGetIdByName       (uint32 *task_id, const char *task_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info (creator, stack size, priority, name) about the
 * specified task.
 *
 * @param[in]   task_id The object ID to operate on
 * @param[out]  task_prop The property object buffer to fill
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the ID passed to it is invalid
 * OS_INVALID_POINTER if the task_prop pointer is NULL
 */
int32 OS_TaskGetInfo           (uint32 task_id, OS_task_prop_t *task_prop);          

/*
** Message Queue API
*/

/**
 * @brief Create a message queue
 *
 * @param[out]  queue_id will be set to the ID of the newly-created resource
 * @param[in]   queue_name the name of the new resource to create
 * @param[in]   queue_depth the maximum depth of the queue
 * @param[in]   data_size the size of each entry in the queue
 * @param[in]   flags options for the queue (reserved for future use, pass as 0)
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if a pointer passed in is NULL
 * OS_ERR_NAME_TOO_LONG if the name passed in is too long
 * OS_ERR_NO_FREE_IDS if there are already the max queues created
 * OS_ERR_NAME_TAKEN if the name is already being used on another queue
 * OS_ERROR if the OS create call fails
 */
int32 OS_QueueCreate           (uint32 *queue_id, const char *queue_name,
                                uint32 queue_depth, uint32 data_size, uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified message queue.
 *
 * @note If There are messages on the queue, they will be lost and any subsequent
 * calls to QueueGet or QueuePut to this queue will result in errors
 *
 * @param[in] queue_id The object ID to delete
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in does not exist
 * OS_ERROR if the OS call to delete the queue fails
 */
int32 OS_QueueDelete           (uint32 queue_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Receive a message on a message queue
 *
 * If a message is pending, it is returned immediately.  Otherwise the calling task
 * will block until a message arrives or the timeout expires.
 *
 * @param[in]   queue_id The object ID to operate on
 * @param[out]  data The buffer to store the received message
 * @param[in]   size The size of the data buffer
 * @param[out]  size_copied Set to the actual size of the message
 * @param[in]   timeout The maximum amount of time to block, or OS_PEND to wait forever
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the given ID does not exist
 * OS_ERR_INVALID_POINTER if a pointer passed in is NULL
 * OS_QUEUE_EMPTY if the Queue has no messages on it to be recieved
 * OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
 * OS_QUEUE_INVALID_SIZE if the size copied from the queue was not correct
 */
int32 OS_QueueGet              (uint32 queue_id, void *data, uint32 size, 
                                uint32 *size_copied, int32 timeout);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Put a message on a message queue.
 *
 * @param[in]  queue_id The object ID to operate on
 * @param[in]  data The buffer containing the message to put
 * @param[in]  size The size of the data buffer
 * @param[in]  flags Currently reserved/unused, should be passed as 0
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the queue id passed in is not a valid queue
 * OS_INVALID_POINTER if the data pointer is NULL
 * OS_QUEUE_FULL if the queue cannot accept another message
 * OS_ERROR if the OS call returns an error
 */
int32 OS_QueuePut              (uint32 queue_id, const void *data, uint32 size,
                                uint32 flags);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing queue ID by name
 *
 * This function tries to find a queue Id given the name of the queue. The
 * id of the queue is passed back in queue_id.
 *
 * @param[out]  queue_id will be set to the ID of the existing resource
 * @param[in]   queue_name the name of the existing resource to find
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if the name or id pointers are NULL
 * OS_ERR_NAME_TOO_LONG the name passed in is too long
 * OS_ERR_NAME_NOT_FOUND the name was not found in the table
 */
int32 OS_QueueGetIdByName      (uint32 *queue_id, const char *queue_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info (name and creator) about the specified queue.
 *
 * @param[in]   queue_id The object ID to operate on
 * @param[out]  queue_prop The property object buffer to fill
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if queue_prop is NULL
 * OS_ERR_INVALID_ID if the ID given is not  a valid queue
 * OS_SUCCESS if the info was copied over correctly
 */
int32 OS_QueueGetInfo          (uint32 queue_id, OS_queue_prop_t *queue_prop);

/*
** Semaphore API
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a binary semaphore
 *
 * Creates a binary semaphore with initial value specified by
 * sem_initial_value and name specified by sem_name. sem_id will be
 * returned to the caller
 *
 * @param[out]  sem_id will be set to the ID of the newly-created resource
 * @param[in]   sem_name the name of the new resource to create
 * @param[in]   sem_initial_value the initial value of the binary semaphore
 * @param[in]   options Reserved for future use, should be passed as 0.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if sen name or sem_id are NULL
 * OS_ERR_NAME_TOO_LONG if the name given is too long
 * OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
 * OS_ERR_NAME_TAKEN if this is already the name of a binary semaphore
 * OS_SEM_FAILURE if the OS call failed
 */
int32 OS_BinSemCreate          (uint32 *sem_id, const char *sem_name, 
                                uint32 sem_initial_value, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unblock all tasks pending on the specified semaphore
 *
 * The function unblocks all tasks pending on the specified semaphore. However,
 * this function does not change the state of the semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
 * OS_SEM_FAILURE if an unspecified failure occurs
 */
int32 OS_BinSemFlush            (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Increment the semaphore value
 *
 * The function  unlocks the semaphore referenced by sem_id by performing
 * a semaphore unlock operation on that semaphore.  If the semaphore value
 * resulting from this operation is positive, then no threads were blocked
 * waiting for the semaphore to become unlocked; the semaphore value is
 * simply incremented for this semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_SEM_FAILURE the semaphore was not previously initialized or is not
 * in the array of semaphores defined by the system
 * OS_ERR_INVALID_ID if the id passed in is not a binary semaphore
 */
int32 OS_BinSemGive            (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value
 *
 * The locks the semaphore referenced by sem_id by performing a
 * semaphore lock operation on that semaphore.  If the semaphore value
 * is currently zero, then the calling thread shall not return from
 * the call until it either locks the semaphore or the call is
 * interrupted.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID the Id passed in is not a valid binary semaphore
 * OS_SEM_FAILURE if the OS call failed
 */
int32 OS_BinSemTake            (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value with a timeout
 *
 * The function locks the semaphore referenced by sem_id. However,
 * if the semaphore cannot be locked without waiting for another process
 * or thread to unlock the semaphore, this wait shall be terminated when
 * the specified timeout, msecs, expires.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[in] msecs  The maximum amount of time to block, in milliseconds
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_SEM_TIMEOUT if semaphore was not relinquished in time
 * OS_SEM_FAILURE the semaphore was not previously initialized or is not
 * in the array of semaphores defined by the system
 * OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
 */
int32 OS_BinSemTimedWait       (uint32 sem_id, uint32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Binary Semaphore
 *
 * @param[in] sem_id The object ID to delete
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore
 * OS_SEM_FAILURE the OS call failed
 */
int32 OS_BinSemDelete          (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing semaphore ID by name
 *
 * This function tries to find a binary sem Id given the name of a bin_sem
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]   sem_name the name of the existing resource to find
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
 * OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_BinSemGetIdByName     (uint32 *sem_id, const char *sem_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified binary
 * semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[out]  bin_prop The property object buffer to fill
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * OS_INVALID_POINTER if the bin_prop pointer is null
 * OS_SUCCESS if success
 */
int32 OS_BinSemGetInfo         (uint32 sem_id, OS_bin_sem_prop_t *bin_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a counting semaphore
 *
 * Creates a counting semaphore with initial value specified by
 * sem_initial_value and name specified by sem_name. sem_id will be
 * returned to the caller
 *
 * @param[out]  sem_id will be set to the ID of the newly-created resource
 * @param[in]   sem_name the name of the new resource to create
 * @param[in]   sem_initial_value the initial value of the counting semaphore
 * @param[in]   options Reserved for future use, should be passed as 0.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if sen name or sem_id are NULL
 * OS_ERR_NAME_TOO_LONG if the name given is too long
 * OS_ERR_NO_FREE_IDS if all of the semaphore ids are taken
 * OS_ERR_NAME_TAKEN if this is already the name of a counting semaphore
 * OS_SEM_FAILURE if the OS call failed
 * OS_INVALID_SEM_VALUE if the semaphore value is too high
 */
int32 OS_CountSemCreate          (uint32 *sem_id, const char *sem_name, 
                                uint32 sem_initial_value, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Increment the semaphore value
 *
 * The function  unlocks the semaphore referenced by sem_id by performing
 * a semaphore unlock operation on that semaphore.  If the semaphore value
 * resulting from this operation is positive, then no threads were blocked
 * waiting for the semaphore to become unlocked; the semaphore value is
 * simply incremented for this semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_SEM_FAILURE the semaphore was not previously  initialized or is not
 * in the array of semaphores defined by the system
 * OS_ERR_INVALID_ID if the id passed in is not a counting semaphore
 */
int32 OS_CountSemGive            (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value
 *
 * The locks the semaphore referenced by sem_id by performing a
 * semaphore lock operation on that semaphore.  If the semaphore value
 * is currently zero, then the calling thread shall not return from
 * the call until it either locks the semaphore or the call is
 * interrupted.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID the Id passed in is not a valid counting semaphore
 * OS_SEM_FAILURE if the OS call failed
 */
int32 OS_CountSemTake            (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Decrement the semaphore value with timeout
 *
 * The function locks the semaphore referenced by sem_id. However,
 * if the semaphore cannot be locked without waiting for another process
 * or thread to unlock the semaphore, this wait shall be terminated when
 * the specified timeout, msecs, expires.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[in] msecs  The maximum amount of time to block, in milliseconds
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_SEM_TIMEOUT if semaphore was not relinquished in time
 * OS_SEM_FAILURE the semaphore was not previously initialized or is not
 * in the array of semaphores defined by the system
 * OS_ERR_INVALID_ID if the ID passed in is not a valid semaphore ID
 */
int32 OS_CountSemTimedWait       (uint32 sem_id, uint32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified counting Semaphore.
 *
 * @param[in] sem_id The object ID to delete
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore
 * OS_SEM_FAILURE the OS call failed
 */
int32 OS_CountSemDelete          (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing semaphore ID by name
 *
 * This function tries to find a counting sem Id given the name of a count_sem
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]   sem_name the name of the existing resource to find
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
 * OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_CountSemGetIdByName     (uint32 *sem_id, const char *sem_name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified counting
 * semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[out]  count_prop The property object buffer to fill
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * OS_INVALID_POINTER if the count_prop pointer is null
 */
int32 OS_CountSemGetInfo         (uint32 sem_id, OS_count_sem_prop_t *count_prop);

/*
** Mutex API
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a mutex semaphore
 *
 * Mutex semaphores are always created in the unlocked (full) state.
 *
 * @param[out]  sem_id will be set to the ID of the newly-created resource
 * @param[in]   sem_name the name of the new resource to create
 * @param[in]   options reserved for future use.  Should be passed as 0.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if sem_id or sem_name are NULL
 * OS_ERR_NAME_TOO_LONG if the sem_name is too long to be stored
 * OS_ERR_NO_FREE_IDS if there are no more free mutex Ids
 * OS_ERR_NAME_TAKEN if there is already a mutex with the same name
 * OS_SEM_FAILURE if the OS call failed
 */
int32 OS_MutSemCreate           (uint32 *sem_id, const char *sem_name, uint32 options);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Releases the mutex object referenced by sem_id.
 *
 * If there are threads blocked on the mutex object referenced by
 * mutex when this function is called, resulting in the mutex becoming
 * available, the scheduling policy shall determine which thread shall
 * acquire the mutex.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid mutex
 * OS_SEM_FAILURE if an unspecified error occurs
 */
int32 OS_MutSemGive             (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Acquire the mutex object referenced by sem_id.
 *
 * If the mutex is already locked, the calling thread shall
 * block until the mutex becomes available. This operation shall
 * return with the mutex object referenced by mutex in the locked state
 * with the calling thread as its owner.
 *
 * @param[in] sem_id The object ID to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_SEM_FAILURE if the semaphore was not previously initialized or is
 * not in the array of semaphores defined by the system
 * OS_ERR_INVALID_ID the id passed in is not a valid mutex
 */
int32 OS_MutSemTake             (uint32 sem_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Deletes the specified Mutex Semaphore.
 *
 * @param[in] sem_id The object ID to delete
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid mutex
 * OS_SEM_FAILURE if the OS call failed
 */
int32 OS_MutSemDelete           (uint32 sem_id);  

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Find an existing mutex ID by name
 *
 * This function tries to find a mutex sem Id given the name of a mut_sem
 * The id is returned through sem_id
 *
 * @param[out] sem_id will be set to the ID of the existing resource
 * @param[in]   sem_name the name of the existing resource to find
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER is semid or sem_name are NULL pointers
 * OS_ERR_NAME_TOO_LONG if the name given is to long to have been stored
 * OS_ERR_NAME_NOT_FOUND if the name was not found in the table
 */
int32 OS_MutSemGetIdByName      (uint32 *sem_id, const char *sem_name); 

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Fill a property object buffer with details regarding the resource
 *
 * This function will pass back a pointer to structure that contains
 * all of the relevant info( name and creator) about the specified mutex
 * semaphore.
 *
 * @param[in] sem_id The object ID to operate on
 * @param[out]  mut_prop The property object buffer to fill
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the id passed in is not a valid semaphore
 * OS_INVALID_POINTER if the mut_prop pointer is null
 */
int32 OS_MutSemGetInfo          (uint32 sem_id, OS_mut_sem_prop_t *mut_prop);

/*
** OS Time/Tick related API
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Convert time units from milliseconds to system ticks
 *
 * This function accepts a time interval in milliseconds and
 * returns the tick equivalent.  If the result is not an exact
 * number of system ticks, the result will be rounded up to
 * the nearest tick.
 *
 * @param[in]   milli_seconds the number of milliseconds
 *
 * @returns the number of ticks
 */
int32 OS_Milli2Ticks           (uint32 milli_seconds);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get the system tick size, in microseconds
 *
 * This function returns the duration of a system tick in micro seconds
 *
 * @note care is taken to ensure this does not return "0" since it is often used
 *  as the divisor in mathematical operations
 *
 * @returns duration of a system tick in microseconds
 */

int32 OS_Tick2Micros           (void);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get the local time
 *
 * This function gets the local time of the machine its on
 *
 * @param[out]  time_struct An OS_time_t that will be set to the current time
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32  OS_GetLocalTime         (OS_time_t *time_struct);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the local time
 *
 * This function sets the local time of the machine its on
 *
 * @param[in]  time_struct An OS_time_t containing the current time
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32  OS_SetLocalTime         (OS_time_t *time_struct);  

/*
** Exception API
** NOTE: Not implemented in current OSAL version
*/

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ExcAttachHandler      (uint32 ExceptionNumber, 
                                void (*ExceptionHandler)(uint32, const void *,uint32),
                                int32 parameter);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ExcEnable             (int32 ExceptionNumber);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ExcDisable            (int32 ExceptionNumber);

/*
** Floating Point Unit API
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set an FPU exception handler function
 *
 * @param[in] ExceptionNumber The exception number to attach to
 * @param[in] ExceptionHandler Pointer to handler function
 * @param[in] parameter Argument to pass to handler
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, void * ExceptionHandler ,
                                 int32 parameter);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Enable FPU exceptions
 *
 * @param[in] ExceptionNumber The exception number to enable
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_FPUExcEnable          (int32 ExceptionNumber);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Disable FPU exceptions
 *
 * @param[in] ExceptionNumber The exception number to disable
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_FPUExcDisable         (int32 ExceptionNumber);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Sets the FPU exception mask
 *
 * This function sets the FPU exception mask
 *
 * @note The exception environment is local to each task Therefore this must be
 *          called for each task that that wants to do floating point and catch exceptions.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_FPUExcSetMask         (uint32 mask);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Gets the FPU exception mask
 *
 * This function gets the FPU exception mask
 *
 * @note The exception environment is local to each task Therefore this must be
 *          called for each task that that wants to do floating point and catch exceptions.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_FPUExcGetMask         (uint32 *mask);

/*
** Interrupt API
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Associate an interrupt number to a specified handler routine
 *
 * The call associates a specified C routine to a specified interrupt
 * number. Upon occurring of the InterruptNumber, the InerruptHandler
 * routine will be called and passed the parameter.
 *
 * @param[in] InterruptNumber The Interrupt Number that will cause the start of the ISR
 * @param[in] InterruptHandler The ISR associated with this interrupt
 * @param[in] parameter Argument that is passed to the ISR
 *
 * @returns OS_SUCCESS on success or appropriate error code
 * OS_INVALID_POINTER if the Interrupt handler pointer is NULL
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Enable interrupts
 *
 * @param[in] IntLevel value from previous call to OS_IntLock()
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_IntUnlock         (int32 IntLevel);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Disable interrupts
 *
 * @returns An key value to be passed to OS_IntUnlock() to restore interrupts
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_IntLock           (void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Enables interrupts through Level
 *
 * @param[in] Level  the interrupts to enable
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_IntEnable         (int32 Level);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Disable interrupts through Level
 *
 * @param[in] Level  the interrupts to disable
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_IntDisable        (int32 Level);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the CPU interrupt mask register
 *
 * @note The interrupt bits are architecture-specific.
 *
 * @param[in] mask The value to set in the register
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_IntSetMask        (uint32 mask);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get the CPU interrupt mask register
 *
 * @note The interrupt bits are architecture-specific.
 *
 * @param[out] mask The register value will be stored to this location
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_NOT_IMPLEMENTED on platforms that do not support this function
 */
int32 OS_IntGetMask        (uint32 *mask);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_IntAck             (int32 InterruptNumber);

/*
** Shared memory API
** NOTE: Not implemented in current OSAL version
*/

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ShMemInit          (void);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ShMemCreate        (uint32 *Id, uint32 NBytes, const char* SegName);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ShMemSemTake       (uint32 Id);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ShMemSemGive       (uint32 Id);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ShMemAttach        (cpuaddr * Address, uint32 Id);

/*-------------------------------------------------------------------------------------*/
/* placeholder; not currently implemented */
int32 OS_ShMemGetIdByName   (uint32 *ShMemId, const char *SegName );

/*
** Heap API
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Return current info on the heap
 *
 * @param[out] heap_prop Storage buffer for heap info
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_HeapGetInfo       (OS_heap_prop_t *heap_prop);

/*
** API for useful debugging function
*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Convert an error number to a string
 *
 * @param[in] error_num Error number to convert
 * @param[out] err_name Buffer to store error string
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_GetErrorName      (int32 error_num, os_err_name_t* err_name);

/**
 * @brief An abstract structure capable of holding several OSAL IDs
 *
 * This is part of the select API and is manipulated using the
 * related API calls.  It should not be modified directly by applications.
 *
 * @sa OS_SelectFdZero(), OS_SelectFdAdd(), OS_SelectFdClear(), OS_SelectFdIsSet()
 */
typedef struct
{
   uint8 object_ids[(OS_MAX_NUM_OPEN_FILES + 7) / 8];
} OS_FdSet;

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Wait for events across multiple file handles
 *
 * Wait for any of the given sets of IDs to be become readable or writable
 *
 * This function will block until any of the following occurs:
 *  - At least one OSAL ID in the ReadSet is readable
 *  - At least one OSAL ID in the WriteSet is writable
 *  - The timeout has elapsed
 *
 * The sets are input/output parameters.  On entry, these indicate the
 * file handle(s) to wait for.  On exit, these are set to the actual
 * file handle(s) that have activity.
 *
 * If the timeout occurs this returns an error code and all output sets
 * should be empty.
 *
 * @note This does not lock or otherwise protect the file handles in the
 * given sets.  If a filehandle supplied via one of the FdSet arguments
 * is closed or modified by another while this function is in progress,
 * the results are undefined.  Because of this limitation, it is recommended
 * to use OS_SelectSingle() whenever possible.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Wait for events on a single file handle
 *
 * Wait for a single OSAL filehandle to change state
 *
 * This function can be used to wait for a single OSAL stream ID
 * to become readable or writable.   On entry, the "StateFlags"
 * parameter should be set to the desired state (readble or writable)
 * and upon return the flags will be set to the state actually
 * detected.
 *
 * As this operates on a single ID, the filehandle is protected
 * during this call, such that another thread accessing the same
 * handle will return an error.  However, it is important to note that
 * once the call returns then other threads may then also read/write
 * and affect the state before the current thread can service it.
 *
 * To mitigate this risk the application may prefer to use
 * the OS_TimedRead/OS_TimedWrite calls.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear a FdSet structure
 *
 * After this call the set will contain no OSAL IDs
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_SelectFdZero(OS_FdSet *Set);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Add an ID to an FdSet structure
 *
 * After this call the set will contain the given OSAL ID
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_SelectFdAdd(OS_FdSet *Set, uint32 objid);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear an ID from an FdSet structure
 *
 * After this call the set will no longer contain the given OSAL ID
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32 OS_SelectFdClear(OS_FdSet *Set, uint32 objid);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check if an FdSet structure contains a given ID
 */
bool OS_SelectFdIsSet(OS_FdSet *Set, uint32 objid);

/* 
** Abstraction for printf statements 
*/
/*-------------------------------------------------------------------------------------*/
/**
 * @brief Abstraction for the system printf() call
 *
 * This function abstracts out the printf type statements. This is
 * useful for using OS- specific thats that will allow non-polled
 * print statements for the real time systems.
 *
 * Operates in a manner similar to the printf() call defined by the standard C
 * library.  This abstraction may implement additional buffering, if necessary,
 * to improve the real-time performance of the call.
 *
 * The output of this routine also may be dynamically enabled or disabled by
 * the OS_printf_enable() and OS_printf_disable() calls, respectively.
 *
 * @param[in] string Format string, followed by additional arguments
 */
void OS_printf( const char *string, ...) OS_PRINTF(1,2);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function disables the output to the console from OS_printf.
 */
void OS_printf_disable(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function enables the output to the console through OS_printf.
 *
 */
void OS_printf_enable(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Exit/Abort the application
 *
 * Indicates that the OSAL application should exit and return control to the OS
 * This is intended for e.g. scripted unit testing where the test needs to end
 * without user intervention.
 *
 * This function does not return.  Production code typically should not ever call this.
 *
 * @note This exits the entire process including tasks that have been created.
 */
void OS_ApplicationExit(int32 Status);

#endif
