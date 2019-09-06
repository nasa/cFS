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

/*
** Initialization of API
*/
int32 OS_API_Init (void);

/*
** OS-specific background thread implementation - waits forever for events to occur.
**
** This should be called from the BSP main routine / initial thread after all other 
** board / application initialization has taken place and all other tasks are running.
*/
void OS_IdleLoop (void);

/*
** OS_DeleteAllObjects() provides a means to clean up all resources allocated by this 
** instance of OSAL.  It would typically be used during an orderly shutdown but may also
** be helpful for testing purposes.
*/
void OS_DeleteAllObjects       (void);

/*
** OS_ApplicationShutdown() provides a means for a user-created thread to request the orderly
** shutdown of the whole system, such as part of a user-commanded reset command.
** This is preferred over e.g. ApplicationExit() which exits immediately and does not
** provide for any means to clean up first.
*/
void OS_ApplicationShutdown         (uint8 flag);


/*
** Some general purpose helper functions --
** These are only available when using enhanced object IDs (-ng variants)
*/

/*
** OS_IdentifyObject() will return the type of an object given an arbitrary object ID
*/
uint32 OS_IdentifyObject       (uint32 object_id);

/*
** OS_ConvertToArrayIndex() will return a unique integer number in the range of [0,MAX)
** for any valid object ID.  This may be used by application code as an array index.
*/
int32 OS_ConvertToArrayIndex   (uint32 object_id, uint32 *ArrayIndex);

/*
** OS_ForEachObject() will call the supplied callback function for all valid object IDs.
*/
void OS_ForEachObject           (uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);


/*
** Task API
*/

int32 OS_TaskCreate            (uint32 *task_id, const char *task_name, 
                                osal_task_entry function_pointer,
                                uint32 *stack_pointer,
                                uint32 stack_size,
                                uint32 priority, uint32 flags);

int32 OS_TaskDelete            (uint32 task_id); 
void OS_TaskExit               (void);
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer);
int32 OS_TaskDelay             (uint32 millisecond);
int32 OS_TaskSetPriority       (uint32 task_id, uint32 new_priority);
int32 OS_TaskRegister          (void);
uint32 OS_TaskGetId            (void);
int32 OS_TaskGetIdByName       (uint32 *task_id, const char *task_name);
int32 OS_TaskGetInfo           (uint32 task_id, OS_task_prop_t *task_prop);          

/*
** Message Queue API
*/

/*
** Queue Create now has the Queue ID returned to the caller.
*/
int32 OS_QueueCreate           (uint32 *queue_id, const char *queue_name,
                                uint32 queue_depth, uint32 data_size, uint32 flags);
int32 OS_QueueDelete           (uint32 queue_id);
int32 OS_QueueGet              (uint32 queue_id, void *data, uint32 size, 
                                uint32 *size_copied, int32 timeout);
int32 OS_QueuePut              (uint32 queue_id, const void *data, uint32 size,
                                uint32 flags);
int32 OS_QueueGetIdByName      (uint32 *queue_id, const char *queue_name);
int32 OS_QueueGetInfo          (uint32 queue_id, OS_queue_prop_t *queue_prop);

/*
** Semaphore API
*/

int32 OS_BinSemCreate          (uint32 *sem_id, const char *sem_name, 
                                uint32 sem_initial_value, uint32 options);
int32 OS_BinSemFlush            (uint32 sem_id);
int32 OS_BinSemGive            (uint32 sem_id);
int32 OS_BinSemTake            (uint32 sem_id);
int32 OS_BinSemTimedWait       (uint32 sem_id, uint32 msecs);
int32 OS_BinSemDelete          (uint32 sem_id);
int32 OS_BinSemGetIdByName     (uint32 *sem_id, const char *sem_name);
int32 OS_BinSemGetInfo         (uint32 sem_id, OS_bin_sem_prop_t *bin_prop);

int32 OS_CountSemCreate          (uint32 *sem_id, const char *sem_name, 
                                uint32 sem_initial_value, uint32 options);
int32 OS_CountSemGive            (uint32 sem_id);
int32 OS_CountSemTake            (uint32 sem_id);
int32 OS_CountSemTimedWait       (uint32 sem_id, uint32 msecs);
int32 OS_CountSemDelete          (uint32 sem_id);
int32 OS_CountSemGetIdByName     (uint32 *sem_id, const char *sem_name);
int32 OS_CountSemGetInfo         (uint32 sem_id, OS_count_sem_prop_t *count_prop);

/*
** Mutex API
*/

int32 OS_MutSemCreate           (uint32 *sem_id, const char *sem_name, uint32 options);
int32 OS_MutSemGive             (uint32 sem_id);
int32 OS_MutSemTake             (uint32 sem_id);
int32 OS_MutSemDelete           (uint32 sem_id);  
int32 OS_MutSemGetIdByName      (uint32 *sem_id, const char *sem_name); 
int32 OS_MutSemGetInfo          (uint32 sem_id, OS_mut_sem_prop_t *mut_prop);

/*
** OS Time/Tick related API
*/

int32 OS_Milli2Ticks           (uint32 milli_seconds);
int32 OS_Tick2Micros           (void);
int32  OS_GetLocalTime         (OS_time_t *time_struct);
int32  OS_SetLocalTime         (OS_time_t *time_struct);  

/*
** Exception API
*/

int32 OS_ExcAttachHandler      (uint32 ExceptionNumber, 
                                void (*ExceptionHandler)(uint32, const void *,uint32),
                                int32 parameter);
int32 OS_ExcEnable             (int32 ExceptionNumber);
int32 OS_ExcDisable            (int32 ExceptionNumber);

/*
** Floating Point Unit API
*/

int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, void * ExceptionHandler ,
                                 int32 parameter);
int32 OS_FPUExcEnable          (int32 ExceptionNumber);
int32 OS_FPUExcDisable         (int32 ExceptionNumber);
int32 OS_FPUExcSetMask         (uint32 mask);
int32 OS_FPUExcGetMask         (uint32 *mask);

/*
** Interrupt API
*/
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter);
int32 OS_IntUnlock         (int32 IntLevel);
int32 OS_IntLock           (void);

int32 OS_IntEnable         (int32 Level);
int32 OS_IntDisable        (int32 Level);

int32 OS_IntSetMask        (uint32 mask);
int32 OS_IntGetMask        (uint32 *mask);
int32 OS_IntAck             (int32 InterruptNumber);

/*
** Shared memory API 
*/
int32 OS_ShMemInit          (void);
int32 OS_ShMemCreate        (uint32 *Id, uint32 NBytes, const char* SegName);
int32 OS_ShMemSemTake       (uint32 Id);
int32 OS_ShMemSemGive       (uint32 Id);
int32 OS_ShMemAttach        (cpuaddr * Address, uint32 Id);
int32 OS_ShMemGetIdByName   (uint32 *ShMemId, const char *SegName );

/*
** Heap API
*/
int32 OS_HeapGetInfo       (OS_heap_prop_t *heap_prop);

/*
** API for useful debugging function
*/
int32 OS_GetErrorName      (int32 error_num, os_err_name_t* err_name);

/**
 * An abstract structure capable of holding several OSAL IDs
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

/**
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
 */
int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);

/**
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
 */
int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);

/**
 * Clear a FdSet structure
 *
 * After this call the set will contain no OSAL IDs
 */
int32 OS_SelectFdZero(OS_FdSet *Set);

/**
 * Add an ID to an FdSet structure
 *
 * After this call the set will contain the given OSAL ID
 */
int32 OS_SelectFdAdd(OS_FdSet *Set, uint32 objid);

/**
 * Clear an ID from an FdSet structure
 *
 * After this call the set will no longer contain the given OSAL ID
 */
int32 OS_SelectFdClear(OS_FdSet *Set, uint32 objid);

/**
 * Check if an FdSet structure contains a given ID
 */
bool OS_SelectFdIsSet(OS_FdSet *Set, uint32 objid);

/* 
** Abstraction for printf statements 
*/
void OS_printf( const char *string, ...) OS_PRINTF(1,2);
void OS_printf_disable(void);
void OS_printf_enable(void);

/*
** Call to exit the running application
** Normally embedded applications run forever, but for debugging purposes
** (unit testing for example) this is needed in order to end the test
*/
void OS_ApplicationExit(int32 Status);

#endif
