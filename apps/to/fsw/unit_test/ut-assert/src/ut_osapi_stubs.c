/*
** File: ut_osapi_stubs.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_osapi_stubs.c 1.2 2011/05/16 16:25:35EDT rmcgraw Exp  $
**
** Purpose: Unit test stubs for OSAPI routines.
**
** $Log: ut_osapi_stubs.c  $
** Revision 1.2 2011/05/16 16:25:35EDT rmcgraw
** Added hook functionality to Count Semaphore APIs
** Revision 1.4 2011/05/16 14:42:42EDT rmcgraw
** Added SetRtnCode processing to Counting Semaphore APIs
** Revision 1.3 2011/04/01 16:02:41EDT sslegel
** Added (void) to unused parameters to avoid compiler warnings
** Revision 1.2 2011/03/08 15:42:09EST rmcgraw
** Added OS_CountSemGetIdByName
** Revision 1.1 2011/02/15 11:13:05EST sslegel
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
*/

/*
** Include section
*/
#include "cfe.h"
#include "ut_osapi_stubs.h"
#include <time.h>
#include <string.h>

Ut_OSAPI_HookTable_t        Ut_OSAPI_HookTable;
Ut_OSAPI_ReturnCodeTable_t  Ut_OSAPI_ReturnCodeTable[UT_OSAPI_MAX_INDEX];

void Ut_OSAPI_Reset(void)
{
    memset(&Ut_OSAPI_HookTable, 0, sizeof(Ut_OSAPI_HookTable));
    memset(&Ut_OSAPI_ReturnCodeTable, 0, sizeof(Ut_OSAPI_ReturnCodeTable));
}

void Ut_OSAPI_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_OSAPI_TASKDELAY_INDEX)       { Ut_OSAPI_HookTable.OS_TaskDelay = FunPtr; }
    else if (Index == UT_OSAPI_BINSEMTAKE_INDEX)      { Ut_OSAPI_HookTable.OS_BinSemTake = FunPtr; }
    else if (Index == UT_OSAPI_BINSEMTIMEDWAIT_INDEX) { Ut_OSAPI_HookTable.OS_BinSemTimedWait = FunPtr; }
    else if (Index == UT_OSAPI_MUTSEMTAKE_INDEX)      { Ut_OSAPI_HookTable.OS_MutSemTake = FunPtr; }
    else if (Index == UT_OSAPI_GETLOCALTIME_INDEX)    { Ut_OSAPI_HookTable.OS_GetLocalTime = FunPtr; }
    else if (Index == UT_OSAPI_QUEUEGET_INDEX)        { Ut_OSAPI_HookTable.OS_QueueGet = FunPtr; }
    else if (Index == UT_OSAPI_QUEUEPUT_INDEX)        { Ut_OSAPI_HookTable.OS_QueuePut = FunPtr; }
    else if (Index == UT_OSAPI_TASKDELETE_INDEX)      { Ut_OSAPI_HookTable.OS_TaskDelete = FunPtr; }
    else if (Index == UT_OSAPI_BINSEMGIVE_INDEX)      { Ut_OSAPI_HookTable.OS_BinSemGive = FunPtr; }
    else if (Index == UT_OSAPI_COUNTSEMCREATE_INDEX)  { Ut_OSAPI_HookTable.OS_CountSemCreate = FunPtr; }
    else if (Index == UT_OSAPI_COUNTSEMDELETE_INDEX)  { Ut_OSAPI_HookTable.OS_CountSemDelete = FunPtr; }
    else if (Index == UT_OSAPI_COUNTSEMGIVE_INDEX)    { Ut_OSAPI_HookTable.OS_CountSemGive = FunPtr; }
    else if (Index == UT_OSAPI_COUNTSEMTAKE_INDEX)    { Ut_OSAPI_HookTable.OS_CountSemTake = FunPtr; }
    else if (Index == UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX)   { Ut_OSAPI_HookTable.OS_CountSemTimedWait = FunPtr; }
    else if (Index == UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX) { Ut_OSAPI_HookTable.OS_CountSemGetIdByName = FunPtr; }
    else if (Index == UT_OSAPI_COUNTSEMGETINFO_INDEX) { Ut_OSAPI_HookTable.OS_CountSemGetInfo = FunPtr; }
    else                                              { printf("Unsupported OSAPI Index In SetFunctionHook Call %u\n", Index); }
}

void Ut_OSAPI_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_OSAPI_MAX_INDEX) {
        Ut_OSAPI_ReturnCodeTable[Index].Value = RtnVal;
        Ut_OSAPI_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported OSAPI Index In SetReturnCode Call %u\n", Index);
    }
}

boolean Ut_OSAPI_UseReturnCode(uint32 Index)
{
    if (Ut_OSAPI_ReturnCodeTable[Index].Count > 0) {
        Ut_OSAPI_ReturnCodeTable[Index].Count--;
        if (Ut_OSAPI_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

/**********************************************************************************
                                    TASK API
**********************************************************************************/

int32 OS_TaskCreate(uint32 *task_id, const char *task_name, osal_task_entry function_pointer, uint32 *stack_pointer,
                    uint32 stack_size, uint32 priority, uint32 flags)
{
    (void) task_id;
    (void) task_name;
    (void) function_pointer;
    (void) stack_pointer;
    (void) stack_size;
    (void) priority;
    (void) flags;

    return(OS_SUCCESS);
}

int32 OS_TaskDelete (uint32 task_id)
{
    if (Ut_OSAPI_HookTable.OS_TaskDelete)
        return(Ut_OSAPI_HookTable.OS_TaskDelete(task_id));

    return(OS_SUCCESS);
}

int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer)
{
    (void) function_pointer;

    return(OS_SUCCESS);
}

void OS_TaskExit(void)
{
    return;
}

int32 OS_TaskDelay(uint32 millisecond)
{
    (void) millisecond;

    if (Ut_OSAPI_HookTable.OS_TaskDelay)
        return(Ut_OSAPI_HookTable.OS_TaskDelay(millisecond));

    return(OS_SUCCESS);
}

int32 OS_TaskSetPriority (uint32 task_id, uint32 new_priority)
{
    (void) task_id;
    (void) new_priority;

    return (OS_SUCCESS);
}

int32 OS_TaskRegister (void)
{
    return (OS_SUCCESS);
}

uint32 OS_TaskGetId (void)
{
    return (uint32)(7);
}

int32 OS_TaskGetIdByName (uint32 *task_id, const char *task_name)
{
    (void) task_id;
    (void) task_name;

    return (uint32)(7);
}

int32 OS_TaskGetInfo (uint32 task_id, OS_task_prop_t *task_prop)
{
    (void) task_id;

    task_prop = NULL;        /* currently not implemented */
    return(OS_INVALID_POINTER);
}

/****************************************************************************************
                              MESSAGE QUEUE API
*****************************************************************************************/

int32 OS_QueueCreate (uint32 *queue_id, const char *queue_name, uint32 queue_depth,
                       uint32 data_size, uint32 flags)
{
    (void) queue_id;
    (void) queue_name;
    (void) queue_depth;
    (void) data_size;
    (void) flags;

    return (OS_SUCCESS);
}

int32 OS_QueueDelete (uint32 queue_id)
{
    (void) queue_id;

    return (OS_SUCCESS);
}

int32 OS_QueueGet (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
{
    if (Ut_OSAPI_HookTable.OS_QueueGet)
        return(Ut_OSAPI_HookTable.OS_QueueGet(queue_id, data, size, size_copied, timeout));

    return (OS_SUCCESS);
}

int32 OS_QueuePut (uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
    if (Ut_OSAPI_HookTable.OS_QueuePut)
        return(Ut_OSAPI_HookTable.OS_QueuePut(queue_id, data, size, flags));

    return (OS_SUCCESS);
}

int32 OS_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
{
    (void) queue_id;
    (void) queue_name;

    return (OS_SUCCESS);
}

/******************************************************************************************
                                  SEMAPHORE API
******************************************************************************************/

int32 OS_BinSemCreate (uint32 *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options)
{
    (void) sem_id;
    (void) sem_name;
    (void) sem_initial_value;
    (void) options;

    return (OS_SUCCESS);
}

int32 OS_BinSemDelete (uint32 sem_id)
{
    (void) sem_id;

    return (OS_SUCCESS);
}

int32 OS_BinSemGive (uint32 sem_id)
{
    if (Ut_OSAPI_HookTable.OS_BinSemGive)
        return(Ut_OSAPI_HookTable.OS_BinSemGive(sem_id));

    return (OS_SUCCESS);
}

int32 OS_BinSemTake (uint32 sem_id)
{
    if (Ut_OSAPI_HookTable.OS_BinSemTake)
        return(Ut_OSAPI_HookTable.OS_BinSemTake(sem_id));

    return (OS_SUCCESS);
}

int32 OS_BinSemTimedWait (uint32 sem_id, uint32 msecs)
{
    if (Ut_OSAPI_HookTable.OS_BinSemTimedWait)
        return(Ut_OSAPI_HookTable.OS_BinSemTimedWait(sem_id, msecs));

    return (OS_SUCCESS);
}

int32 OS_BinSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
    (void) sem_name;

    sem_id = NULL;             /* currently not implemented */
    return(OS_INVALID_POINTER);
}

int32 OS_BinSemGetInfo (uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
{
    (void) sem_id;

    bin_prop = NULL;        /* currently not implemented */
    return(OS_INVALID_POINTER);
}

int32 OS_CountSemCreate (uint32 *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options)
{
    if (Ut_OSAPI_UseReturnCode(UT_OSAPI_COUNTSEMCREATE_INDEX))
        return Ut_OSAPI_ReturnCodeTable[UT_OSAPI_COUNTSEMCREATE_INDEX].Value;

    if (Ut_OSAPI_HookTable.OS_CountSemCreate)
        return(Ut_OSAPI_HookTable.OS_CountSemCreate(sem_id, sem_name, sem_initial_value, options));

    return (OS_SUCCESS);
}

int32 OS_CountSemDelete (uint32 sem_id)
{
    (void) sem_id;

    if (Ut_OSAPI_HookTable.OS_CountSemDelete)
        return(Ut_OSAPI_HookTable.OS_CountSemDelete(sem_id));

    if (Ut_OSAPI_UseReturnCode(UT_OSAPI_COUNTSEMDELETE_INDEX))
        return Ut_OSAPI_ReturnCodeTable[UT_OSAPI_COUNTSEMDELETE_INDEX].Value;

    return (OS_SUCCESS);
}

int32 OS_CountSemGive (uint32 sem_id)
{
    if (Ut_OSAPI_HookTable.OS_CountSemGive)
        return(Ut_OSAPI_HookTable.OS_CountSemGive(sem_id));

    if (Ut_OSAPI_UseReturnCode(UT_OSAPI_COUNTSEMGIVE_INDEX))
        return Ut_OSAPI_ReturnCodeTable[UT_OSAPI_COUNTSEMGIVE_INDEX].Value;

    return (OS_SUCCESS);
}

int32 OS_CountSemTake (uint32 sem_id)
{
    if (Ut_OSAPI_HookTable.OS_CountSemTake)
        return(Ut_OSAPI_HookTable.OS_CountSemTake(sem_id));

    if (Ut_OSAPI_UseReturnCode(UT_OSAPI_COUNTSEMTAKE_INDEX))
        return Ut_OSAPI_ReturnCodeTable[UT_OSAPI_COUNTSEMTAKE_INDEX].Value;

    return OS_SUCCESS;
}

int32 OS_CountSemTimedWait (uint32 sem_id, uint32 msecs)
{
    (void) sem_id;
    (void) msecs;

    if (Ut_OSAPI_HookTable.OS_CountSemTimedWait)
        return(Ut_OSAPI_HookTable.OS_CountSemTimedWait(sem_id,msecs));

    if (Ut_OSAPI_UseReturnCode(UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX))
        return Ut_OSAPI_ReturnCodeTable[UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX].Value;

    return OS_SUCCESS;
}

int32 OS_CountSemGetIdByName (uint32 *sem_id, const char *sem_name)
{

    if (Ut_OSAPI_HookTable.OS_CountSemGetIdByName)
        return(Ut_OSAPI_HookTable.OS_CountSemGetIdByName(sem_id, sem_name));

    if (Ut_OSAPI_UseReturnCode(UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX))
        return Ut_OSAPI_ReturnCodeTable[UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX].Value;

    return OS_SUCCESS;
}

int32 OS_CountSemGetInfo (uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    (void) sem_id;
    (void) count_prop;

    if (Ut_OSAPI_HookTable.OS_CountSemGetInfo)
        return(Ut_OSAPI_HookTable.OS_CountSemGetInfo(sem_id, count_prop));

    if (Ut_OSAPI_UseReturnCode(UT_OSAPI_COUNTSEMGETINFO_INDEX))
        return Ut_OSAPI_ReturnCodeTable[UT_OSAPI_COUNTSEMGETINFO_INDEX].Value;

    return OS_SUCCESS;
}

/****************************************************************************************
                                  MUTEX API
****************************************************************************************/

int32 OS_MutSemCreate (uint32 *sem_id, const char *sem_name, uint32 options)
{
    (void) sem_id;
    (void) sem_name;
    (void) options;

    return (OS_SUCCESS);
}

int32 OS_MutSemDelete (uint32 sem_id)
{
    (void) sem_id;

    return (OS_SUCCESS);
}

int32 OS_MutSemGive (uint32 sem_id)
{
    (void) sem_id;

    return (OS_SUCCESS);
}

int32 OS_MutSemTake (uint32 sem_id)
{
    if (Ut_OSAPI_HookTable.OS_MutSemTake)
        return(Ut_OSAPI_HookTable.OS_MutSemTake(sem_id));

    return (OS_SUCCESS);
}

int32 OS_MutSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
    (void) sem_name;

    sem_id = NULL;             /* currently not implemented */
    return(OS_INVALID_POINTER);
}

int32 OS_MutSemGetInfo (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
{
    (void) sem_id;

    mut_prop = NULL;        /* currently not implemented */
    return(OS_INVALID_POINTER);
}

/****************************************************************************************
                                  Time/Tick Related API
****************************************************************************************/

int32 OS_Milli2Ticks(uint32 milli_seconds)
{
    uint32 num_of_ticks,tick_duration_usec ;

    tick_duration_usec = OS_Tick2Micros() ;

    num_of_ticks = ( (milli_seconds * 1000) + tick_duration_usec -1 ) / tick_duration_usec ;

    return(num_of_ticks) ;
}

int32 OS_Tick2Micros (void)
{
    return((1/(CLOCKS_PER_SEC))*1000);
}

int32 OS_GetLocalTime(OS_time_t *time_struct)
{
    if (Ut_OSAPI_HookTable.OS_GetLocalTime)
        return(Ut_OSAPI_HookTable.OS_GetLocalTime(time_struct));

    return (OS_SUCCESS);
}

/****************************************************************************************
                                  Exception API
****************************************************************************************/

int32 OS_ExcAttachHandler(uint32 ExceptionNumber, void (*ExceptionHandler)(uint32, const void *,uint32), int32 Parameter)
{
    (void) ExceptionNumber;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_ExcEnable(int32 ExceptionNumber)
{
    (void) ExceptionNumber;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_ExcDisable(int32 ExceptionNumber)
{
    (void) ExceptionNumber;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

/****************************************************************************************
                                  Floating Point Unit API
****************************************************************************************/

int32 OS_FPUExcAttachHandler(uint32 ExceptionNumber, void * ExceptionHandler, int32 Parameter)
{
    (void) ExceptionNumber;
    (void) ExceptionHandler;
    (void) Parameter;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_FPUExcEnable(int32 ExceptionNumber)
{
    (void) ExceptionNumber;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_FPUExcDisable(int32 ExceptionNumber)
{
    (void) ExceptionNumber;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_FPUExcSetMask(uint32 Mask)
{
    (void) Mask;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_FPUExcGetMask(uint32 *Mask)
{
    (void) Mask;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

/****************************************************************************************
                                  Interrupt API
****************************************************************************************/

int32 OS_IntAttachHandler(uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 Parameter)
{
    (void) InterruptNumber;
    (void) InterruptHandler;
    (void) Parameter;

    return(OS_SUCCESS);
}

int32 OS_IntEnable(int32 Level)
{
    (void) Level;

    return(OS_SUCCESS);
}

int32 OS_IntDisable(int32 Level)
{
    (void) Level;

    return(OS_SUCCESS);
}

int32 OS_IntUnlock (int32 IntLevel)
{
    (void) IntLevel;

    return(OS_SUCCESS);
}

int32 OS_IntLock (void)
{
    return(OS_SUCCESS);
}

int32 OS_IntSetMask(uint32 Mask)
{
    (void) Mask;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_IntGetMask(uint32 *Mask)
{
    (void) Mask;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

int32 OS_IntAck(int32 InterruptNumber)
{
    (void) InterruptNumber;

    return(CFE_OSAPI_NOT_IMPLEMENTED);
}

/****************************************************************************************
                                  Debug API
****************************************************************************************/

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

void OS_printf( const char *String, ...)
{
    va_list     ptr;
    char msg_buffer [OS_BUFFER_SIZE];

    va_start(ptr,String);
    vsprintf(&msg_buffer[0], String, ptr);
    va_end(ptr);

    msg_buffer[OS_BUFFER_SIZE-1] = '\0';
    printf("%s", &msg_buffer[0]); //FIXME??
}
