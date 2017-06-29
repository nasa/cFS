/*
 *  Copyright (c) 2004-2015, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include <string.h>

#include "osapi.h"
#include "utstubs.h"
#include "utbsp.h"
#include "utassert.h"
#include "uttools.h"

/*
 * Instantiate the old-style return code holder objects
 * unless the "OMIT_DEPRECATED" flag is set.  This should
 * be removed at some point in the future.
 */
#ifndef OSAL_OMIT_DEPRECATED

/* Instantiate "UT_SetRtn_t" objects for compatibility */
UT_Compat_SetRtn_t FileWriteRtn;
UT_Compat_SetRtn_t OSReadRtn;
UT_Compat_SetRtn_t OSReadRtn2;
UT_Compat_SetRtn_t OSlseekRtn;
UT_Compat_SetRtn_t OSTimerGetInfoRtn;
UT_Compat_SetRtn_t OSCloseRtn;
UT_Compat_SetRtn_t UnmountRtn;
UT_Compat_SetRtn_t BlocksFreeRtn;
UT_Compat_SetRtn_t MutSemCreateRtn;
UT_Compat_SetRtn_t MutSemGiveRtn;
UT_Compat_SetRtn_t MutSemTakeRtn;
UT_Compat_SetRtn_t MutSemDelRtn;
UT_Compat_SetRtn_t QueueCreateRtn;
UT_Compat_SetRtn_t QueueDelRtn;
UT_Compat_SetRtn_t QueueGetRtn;
UT_Compat_SetRtn_t QueuePutRtn;
UT_Compat_SetRtn_t CountSemDelRtn;
UT_Compat_SetRtn_t OSBinSemTimedWaitRtn;
UT_Compat_SetRtn_t OSBinSemFlushRtn;
UT_Compat_SetRtn_t BinSemDelRtn;
UT_Compat_SetRtn_t OS_BinSemCreateRtn;
UT_Compat_SetRtn_t OSTaskExitRtn;
UT_Compat_SetRtn_t SymbolLookupRtn;
UT_Compat_SetRtn_t ModuleLoadRtn;
UT_Compat_SetRtn_t ModuleUnloadRtn;
UT_Compat_SetRtn_t ModuleInfoRtn;
UT_Compat_SetRtn_t HeapGetInfoRtn;

#endif

typedef enum
{
    UT_OBJTYPE_NONE = 0,
    UT_OBJTYPE_TASK,
    UT_OBJTYPE_QUEUE,
    UT_OBJTYPE_COUNTSEM,
    UT_OBJTYPE_BINSEM,
    UT_OBJTYPE_MUTEX,
    UT_OBJTYPE_TIMECB,
    UT_OBJTYPE_MODULE,
    UT_OBJTYPE_FILESTREAM,
    UT_OBJTYPE_TIMEBASE,
    UT_OBJTYPE_MAX
} UT_ObjType_t;

const uint32 UT_MAXOBJS[UT_OBJTYPE_MAX] =
{
        [UT_OBJTYPE_TASK] = OS_MAX_TASKS,
        [UT_OBJTYPE_QUEUE] = OS_MAX_QUEUES,
        [UT_OBJTYPE_COUNTSEM] = OS_MAX_COUNT_SEMAPHORES,
        [UT_OBJTYPE_BINSEM] = OS_MAX_BIN_SEMAPHORES,
        [UT_OBJTYPE_MUTEX] = OS_MAX_MUTEXES,
        [UT_OBJTYPE_TIMECB] = OS_MAX_TIMERS,
        [UT_OBJTYPE_MODULE] = OS_MAX_MODULES,
        [UT_OBJTYPE_FILESTREAM] = OS_MAX_NUM_OPEN_FILES,
        [UT_OBJTYPE_TIMEBASE] = OS_MAX_TIMEBASES,
};

/*
 * Macro to adjust any objid into the proper range
 */
#define UT_FIXUP_ID(val,objtype)        val %= UT_MAXOBJS[objtype]

/*
 * Size of the bitmask for the OSAL fake object ID validity table
 * Set this according to the highest anticipated number of OSAL objects
 * simultaneously created by any given test case (it is reset when the
 * test is reset).
 *
 * Keep as a power of two, and actual number of bits is 8x this value
 * (Default value of 16 allows for up to 128 objects to be created in
 * a single test case, far more than anything I've seen yet)
 */
#define OSAL_MAX_VALID_PER_TYPE     16

typedef struct
{
    uint32 LastIssueNumber;
    uint8 ValidBits[OSAL_MAX_VALID_PER_TYPE];
} UT_ObjTypeState_t;

UT_ObjTypeState_t UT_ObjState[UT_OBJTYPE_MAX];

/*
 * Helper function - "allocate" a fake object ID of the given type
 */
static uint32 UT_AllocStubObjId(UT_ObjType_t ObjType)
{
    UT_ObjTypeState_t *StatePtr;
    uint8 ObjMask;
    uint32 Result;

    if (ObjType != UT_OBJTYPE_NONE && ObjType >= UT_OBJTYPE_MAX)
    {
        /* Code is broken, abort the test
         * (This signifies an error in the stub code itself hence the abort)
         */
        UtAssert_Abort("ObjType out of range");
    }

    StatePtr = &UT_ObjState[ObjType];

    if (StatePtr->LastIssueNumber < ((8 * sizeof(StatePtr->ValidBits)) - 1))
    {
        ++StatePtr->LastIssueNumber;
    }

    Result = StatePtr->LastIssueNumber;

    ObjMask = 1 << (Result & 0x07);
    /*
     * Check for overlap/re-issue - this COULD happen when using
     * the original (non-opaque) object IDs if a UT creates too many
     * objects of a certain type.  There is really no way around this
     * and it means the test needs to be revised to not create so many
     * objects OR it needs to support opaque object IDs
     */
    if ((StatePtr->ValidBits[Result >> 3] & ObjMask) != 0)
    {
        UtAssert_Failed("OSAPI UT stub object overlap");
    }
    StatePtr->ValidBits[Result >> 3] |= ObjMask;

    /*
     * Finalize Object ID - put into proper range for type
     */
    UT_FIXUP_ID(Result, ObjType);

    return Result;
}

/*
 * Helper function - "deallocate" a fake object ID of the given type
 */
static void UT_DeleteStubObjId(UT_ObjType_t ObjType, uint32 ObjId)
{
    UT_ObjTypeState_t *StatePtr;
    uint8 ObjMask;

    ObjId &= 0xFFFFU;
    if (ObjId >= (8 * sizeof(StatePtr->ValidBits)))
    {
        /* Calling code is broken */
        UtAssert_Failed("ObjId out of range");
    }

    StatePtr = &UT_ObjState[ObjType];

    /* Clear out any bit it could have been */
    while (ObjId <= StatePtr->LastIssueNumber)
    {
        ObjMask = 1 << (ObjId & 0x07);
        StatePtr->ValidBits[ObjId >> 3] &= ~ObjMask;
        ObjId += UT_MAXOBJS[ObjType];
    }

}

/**
 * Initialization function
 */
int32 OS_API_Init (void)
{
   /*
    * Reset the fake ID numbers for create/delete operations
    */
   memset(UT_ObjState, 0, sizeof(UT_ObjState));

#ifndef OSAL_OMIT_DEPRECATED
   /*
    * Define a "Mapping Table" of UT_SetRtn_t objects to the stub function(s)
    * that they should affect.
    *
    * This is an ugly translation layer required due to existing CFE test code that
    * is calling UT_SetRtnCode() referencing these object names to set up OSAL stubs.
    *
    * Each of these calls to UT_SetRtnCode() could be replaced by UT_SetDeferredRetcode()
    * and all these extra objects can be removed once and for all.
    */
    UT_Compat_SetCodeMap(&FileWriteRtn, UT_KEY(OS_write));
    UT_Compat_SetCodeMap(&OSReadRtn, UT_KEY(OS_read));
    UT_Compat_SetCodeMap(&OSReadRtn2, UT_KEY(OS_read));
    UT_Compat_SetCodeMap(&OSlseekRtn, UT_KEY(OS_lseek));
    UT_Compat_SetCodeMap(&OSTimerGetInfoRtn, UT_KEY(OS_TimerGetInfo));
    UT_Compat_SetCodeMap(&OSCloseRtn, UT_KEY(OS_close));
    UT_Compat_SetCodeMap(&UnmountRtn, UT_KEY(OS_unmount));
    UT_Compat_SetCodeMap(&BlocksFreeRtn, UT_KEY(OS_fsBlocksFree));
    UT_Compat_SetCodeMap(&MutSemCreateRtn, UT_KEY(OS_MutSemCreate));
    UT_Compat_SetCodeMap(&MutSemGiveRtn, UT_KEY(OS_MutSemGive));
    UT_Compat_SetCodeMap(&MutSemTakeRtn, UT_KEY(OS_MutSemTake));
    UT_Compat_SetCodeMap(&MutSemDelRtn, UT_KEY(OS_MutSemDelete));
    UT_Compat_SetCodeMap(&QueueCreateRtn, UT_KEY(OS_QueueCreate));
    UT_Compat_SetCodeMap(&QueueDelRtn, UT_KEY(OS_QueueDelete));
    UT_Compat_SetCodeMap(&QueueGetRtn, UT_KEY(OS_QueueGet));
    UT_Compat_SetCodeMap(&QueuePutRtn, UT_KEY(OS_QueuePut));
    UT_Compat_SetCodeMap(&CountSemDelRtn, UT_KEY(OS_CountSemDelete));
    UT_Compat_SetCodeMap(&OSBinSemTimedWaitRtn, UT_KEY(OS_BinSemTimedWait));
    UT_Compat_SetCodeMap(&OSBinSemFlushRtn, UT_KEY(OS_BinSemFlush));
    UT_Compat_SetCodeMap(&BinSemDelRtn, UT_KEY(OS_BinSemDelete));
    UT_Compat_SetCodeMap(&OS_BinSemCreateRtn, UT_KEY(OS_BinSemCreate));
    UT_Compat_SetCodeMap(&OSTaskExitRtn, UT_KEY(OS_TaskExit));
    UT_Compat_SetCodeMap(&SymbolLookupRtn, UT_KEY(OS_SymbolLookup));
    UT_Compat_SetCodeMap(&ModuleLoadRtn, UT_KEY(OS_ModuleLoad));
    UT_Compat_SetCodeMap(&ModuleUnloadRtn, UT_KEY(OS_ModuleUnload));
    UT_Compat_SetCodeMap(&ModuleInfoRtn, UT_KEY(OS_ModuleInfo));
    UT_Compat_SetCodeMap(&HeapGetInfoRtn, UT_KEY(OS_HeapGetInfo));

#endif


   return OS_SUCCESS;
}

/*****************************************************************************/
/**
** \brief OS_BinSemTake stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemTake.  The user can adjust the response by setting
**        the value of UT_BinSemFail, causing it to return a failure
**        indication (-1) on the first or second call to the function.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either -1 or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemTake(uint32 sem_id)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_BinSemTake);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_close stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_close.  The user determines the response by setting the value of
**        UT_OS_Fail prior to this function being called.  The values in the
**        structure OSCloseRtn are updated and subsequently used by the unit
**        tests to determine if the proper response was made to the calling
**        conditions.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_FS_ERROR or OS_FS_SUCCESS.
**
******************************************************************************/
int32 OS_close(int32 filedes)
{
    int32 status = OS_FS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_close);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_FILESTREAM, filedes);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_creat stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_creat.  The user determines the response by setting the value of
**        UT_OS_Fail prior to this function being called.  UT_lognum is
**        incremented each time this function is called; if UT_OS_Fail has not
**        been set to OS_CREAT_FAIL then UT_lognum is returned.  Otherwise
**        OS_FS_ERROR is returned.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_FS_ERROR or the value of UT_lognum.
**
******************************************************************************/
/* Open local log file, increment reporting number on each call */
int32 OS_creat(const char *path, int32 access)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_creat);

    if (status == OS_SUCCESS)
    {
        status = UT_AllocStubObjId(UT_OBJTYPE_FILESTREAM);
    }

   return status;
}


/*****************************************************************************/
/**
** \brief OS_IntLock stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_IntLock.  It always returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_IntLock(void)
{
    return OS_SUCCESS;
}

/*****************************************************************************/
/**
** \brief OS_IntUnlock stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_IntUnlock.  It always returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_IntUnlock(int32 IntLevel)
{
    return OS_SUCCESS;
}

/*****************************************************************************/
/**
** \brief OS_MutSemGive stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemGive.  The user can adjust the response by setting
**        the values in the MutSemGiveRtn structure prior to this function
**        being called.  If the value MutSemGiveRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value MutSemGiveRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemGive(uint32 sem_id)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_MutSemGive);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemTake stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemTake.  The user can adjust the response by setting
**        the values in the MutSemTakeRtn structure prior to this function
**        being called.  If the value MutSemTakeRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value MutSemTakeRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemTake(uint32 sem_id)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_MutSemTake);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueCreate.  The user can adjust the response by setting
**        the values in the QueueCreateRtn structure prior to this function
**        being called.  If the value QueueCreateRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueCreateRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        creating the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Similar to the real call, doesn't care about creator or do any
**           mutex locking\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_INVALID_POINTER,
**        OS_ERR_NAME_TOO_LONG, OS_ERR_NO_FREE_IDS, OS_ERR_NAME_TAKEN,
**        or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueCreate(uint32 *queue_id,
                     const char *queue_name,
                     uint32 queue_depth,
                     uint32 data_size,
                     uint32 flags)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueueCreate);

    if (status == OS_SUCCESS)
    {
        *queue_id = UT_AllocStubObjId(UT_OBJTYPE_QUEUE);
    }
    else
    {
        *queue_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueDelete.  The user can adjust the response by setting
**        the values in the QueueDelRtn structure prior to this function
**        being called.  If the value QueueDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueDelRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        deleting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Similar to real code without mutex locking\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueDelete(uint32 queue_id)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueueDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_QUEUE, queue_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueGet stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueGet.  The user can adjust the response by setting
**        the values in the QueueGetRtn structure prior to this function
**        being called.  If the value QueueGetRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueGetRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        getting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Works similar to real function; note that pend on empty queue
**           doesn't block\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_INVALID_POINTER, OS_QUEUE_EMPTY, OS_QUEUE_INVALID_SIZE,
**        or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueGet(uint32 queue_id,
                  void *data,
                  uint32 size,
                  uint32 *size_copied,
                  int32 timeout)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueueGet);

    if (status == OS_SUCCESS)
    {
        *size_copied = UT_Stub_CopyToLocal((UT_EntryKey_t)&OS_QueueGet + queue_id, (uint8 *)data, size);
        if (*size_copied == 0)
        {
            status = OS_QUEUE_EMPTY;
        }
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueuePut stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueuePut.  The user can adjust the response by setting
**        the values in the QueuePutRtn structure prior to this function
**        being called.  If the value QueuePutRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueuePutRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        putting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Same as real function\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_INVALID_POINTER, OS_QUEUE_FULL, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueuePut(uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueuePut);

    if (status == OS_SUCCESS)
    {
        UT_SetDataBuffer((UT_EntryKey_t)&OS_QueueGet + queue_id, (void *)data, size, TRUE);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_write stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_write.  The user can adjust the response by setting the values
**        in the FileWriteRtn structure prior to this function being called.
**        If the value FileWriteRtn.count is greater than zero then the counter
**        is decremented; if it then equals zero the return value is set to the
**        user-defined value FileWriteRtn.value.  Alternately, the user can
**        cause the function to return a failure result, OS_FS_ERROR, by
**        setting the value of UT_OS_Fail to OS_WRITE_FAIL prior to this
**        function being called.  The input value, nbytes, is returned
**        otherwise.
**
** \par Assumptions, External Events, and Notes:
**        Only outputs SUCCESS or FAILURE since packet structure isn't known
**
** \returns
**        Returns either a user-defined status flag, OS_FS_ERROR, or the value
**        of the input variable, nbytes.
**
******************************************************************************/
int32 OS_write(int32 filedes, void *buffer, uint32 nbytes)
{
    int32   status;
    uint32  CopySize;

    status = UT_DEFAULT_IMPL_RC(OS_write, 0x7FFFFFFF);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyFromLocal(UT_KEY(OS_write), (const uint8 *)buffer, nbytes);

        /* If CopyFromLocal returns zero, this probably means no buffer was supplied,
         * in which case just throw out the data and pretend it was written.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            status = nbytes;
        }
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemFlush stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemFlush.  The variable OSBinSemFlushRtn.value is set to the
**        value passed to the function, reset_type, and the variable
**        OSBinSemFlushRtn.count is incremented each time this function is
**        called.  The unit tests compare these values to expected results to
**        verify proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemFlush(uint32 sem_id)
{
    int32   status;

    status = UT_DEFAULT_IMPL(OS_BinSemFlush);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskRegister stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskRegister.  The user can adjust the response by setting the value
**        of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskRegister(void)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_TaskRegister);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskCreate.  The user can adjust the response by setting the value
**        of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskCreate(uint32 *task_id, const char *task_name,
                    osal_task_entry function_pointer,
                    uint32 *stack_pointer,
                    uint32 stack_size, uint32 priority,
                    uint32 flags)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_TaskCreate);

    if (status == OS_SUCCESS)
    {
        *task_id = UT_AllocStubObjId(UT_OBJTYPE_TASK);
    }
    else
    {
        *task_id = 0xDEADBEEFU;
    }


    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskGetId stub function
**
** \par Description
**        This function is used as a placeholder for the OS API function
**        OS_TaskGetId.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns 1 unless an override value is configured.
**
******************************************************************************/
uint32 OS_TaskGetId(void)
{
    int32 TaskId;

    TaskId = 1;

    TaskId = UT_DEFAULT_IMPL_RC(OS_TaskGetId, TaskId);

    return TaskId;
}

/*****************************************************************************/
/**
** \brief OS_TaskGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskGetInfo.  If the input structure, task_prop, is null, it
**        returns OS_INVALID_POINTER.  Otherwise it sets the task structure
**        variables to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_INVALID_POINTER or OS_SUCCESS.
**
******************************************************************************/
int32 OS_TaskGetInfo(uint32 task_id, OS_task_prop_t *task_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TaskGetInfo);

    /* Fill out the info regardless of status (preserves old behavior) */
    task_prop->creator = 0;
    task_prop->OStask_id = task_id & 0xFFFF;
    task_prop->stack_size = 100;
    task_prop->priority = 150;
    strncpy(task_prop->name, "UnitTest", OS_MAX_API_NAME - 1);
    task_prop->name[OS_MAX_API_NAME - 1] = '\0';

    return status;
}

/*****************************************************************************/
/**
** \brief OS_read stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_read.  The user can adjust the response by setting the values
**        in the OSReadRtn structure prior to this function being called.  If
**        the value OSReadRtn.count is greater than zero then the counter is
**        decremented; if it then equals zero the return value is set to the
**        user-defined value OSReadRtn.value.  Alternately, the user can adjust
**        the response by setting the values in the OSReadRtn2 structure prior
**        to this function being called.  If the value OSReadRtn2.count is
**        greater than zero then the counter is decremented; if it then equals
**        zero the return value is set to the user-defined value
**        OSReadRtn2.value.  Finally, the user can cause the function to return
**        a failure result, nbytes - 1, by setting the value of UT_OS_Fail to
**        OS_READ_FAIL prior to this function being called.  The input value,
**        nbytes, is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either one of two user-defined status flags, the value
**        of the input variable, nbytes, - 1 (failure case), or the value of
**        the input variable, nbytes (success case).
**
******************************************************************************/
int32 OS_read(int32 filedes, void *buffer, uint32 nbytes)
{
    int32 status;
    uint32 CopySize;

    status = UT_DEFAULT_IMPL_RC(OS_read, 0x7FFFFFFF);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_read), (uint8 *)buffer, nbytes);

        /* If CopyToLocal returns zero, this probably means no buffer was supplied,
         * in which case just generate fill data and pretend it was read.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            memset(buffer, 0, nbytes);
            status = nbytes;
        }
    }
    else if (status > 0)
    {
        /* generate fill data for requested size */
        memset(buffer, 0, status);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemCreate.  The user can adjust the response by setting the
**        values in the OS_BinSemCreateRtn structure prior to this function
**        being called.  If the value OS_BinSemCreateRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value
**        OS_BinSemCreateRtn.value.  Alternately, the user can cause the
**        function to return a failure result, OS_ERROR, by setting the value
**        of UT_OS_Fail to OS_SEMCREATE_FAIL prior to this function being
**        called.  OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag, OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemCreate(uint32 *sem_id, const char *sem_name,
                      uint32 sem_initial_value, uint32 options)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemCreate);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(UT_OBJTYPE_BINSEM);
    }
    else
    {
        *sem_id = 0xDEADBEEFU;
    }

    return status;
}

int32 OS_CountSemCreate(uint32 *sem_id, const char *sem_name,
                      uint32 sem_initial_value, uint32 options)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemCreate);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(UT_OBJTYPE_COUNTSEM);
    }
    else
    {
        *sem_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemCreate.  The user can adjust the response by setting the
**        values in the MutSemCreateRtn structure prior to this function being
**        called.  If the value MutSemCreateRtn.count is greater than zero
**        then the counter is decremented; if it then equals zero the return
**        value is set to the user-defined value MutSemCreateRtn.value.
**        Alternately, the user can cause the function to return a failure
**        result, OS_ERROR, by setting the value of UT_OS_Fail to
**        OS_MUTCREATE_FAIL prior to this function being called.  OS_SUCCESS
**        is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag, OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemCreate(uint32 *sem_id, const char *sem_name, uint32 options)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemCreate);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(UT_OBJTYPE_MUTEX);
    }
    else
    {
        *sem_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_open stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_open.  The user can adjust the response by setting the value
**        of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_ERROR or a value that is incremented by 1 each
**        time the function is called.
**
******************************************************************************/
int32 OS_open(const char *path, int32 access, uint32 mode)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_open);

    if (status == OS_SUCCESS)
    {
        status = UT_AllocStubObjId(UT_OBJTYPE_FILESTREAM);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskDelay stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskDelay.  The user can adjust the response by setting the value
**        of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskDelay(uint32 millisecond)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TaskDelay);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemGive stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemGive.  The user can adjust the response by setting the value
**        of UT_BinSemFail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns the user-defined value UT_BinSemFail.
**
******************************************************************************/
int32 OS_BinSemGive(uint32 sem_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGive);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemGetInfo.  It sets the binary semaphore structure variables
**        to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemGetInfo(uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGetInfo);

    bin_prop->creator =  1;
    strncpy(bin_prop->name, "Name", OS_MAX_API_NAME - 1);
    bin_prop->name[OS_MAX_API_NAME - 1] = '\0';

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemGetInfo.  It sets the mutex semaphore structure variables
**        to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemGetInfo(uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemGetInfo);

    strncpy(mut_prop->name, "Name", OS_MAX_API_NAME - 1);
    mut_prop->name[OS_MAX_API_NAME - 1] = '\0';
    mut_prop->creator =  1;

    return status;
}

/*****************************************************************************/
/**
** \brief OS_CountSemGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_CountSemGetInfo.  It sets the counting semaphore structure
**        variables to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_CountSemGetInfo(uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemGetInfo);

    count_prop->creator = 1;
    strncpy(count_prop->name, "Name", OS_MAX_API_NAME - 1);
    count_prop->name[OS_MAX_API_NAME - 1] = '\0';

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueGetInfo.  It sets the queue structure variables to fixed
**        values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueGetInfo(uint32 sem_id, OS_queue_prop_t *queue_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueGetInfo);

    queue_prop->creator = 1;
    strncpy(queue_prop->name, "Name", OS_MAX_API_NAME - 1);
    queue_prop->name[OS_MAX_API_NAME - 1] = '\0';

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemDelete.  The user can adjust the response by setting
**        the values in the MutSemDelRtn structure prior to this function
**        being called.  If the value MutSemDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value MutSemDelRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemDelete(uint32 sem_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_MUTEX, sem_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemDelete.  The user can adjust the response by setting
**        the values in the BinSemDelRtn structure prior to this function
**        being called.  If the value BinSemDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value BinSemDelRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemDelete(uint32 sem_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_BINSEM, sem_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_CountSemDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_CountSemDelete.  The user can adjust the response by setting
**        the values in the CountSemDelRtn structure prior to this function
**        being called.  If the value CountSemDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value CountSemDelRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_CountSemDelete(uint32 sem_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_COUNTSEM, sem_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_FDGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_FDGetInfo.  It sets the table entry structure variables to fixed
**        values and returns OS_FS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_FS_SUCCESS.
**
******************************************************************************/
int32 OS_FDGetInfo(int32 filedes, OS_FDTableEntry *fd_prop)
{
    int32 status;
    uint32 CopySize;

    status = UT_DEFAULT_IMPL(OS_FDGetInfo);

    if (status == OS_SUCCESS)
    {
        /* The user may supply specific entries to return */
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_FDGetInfo), (uint8 *)fd_prop, sizeof(*fd_prop));
        if (CopySize < sizeof(*fd_prop))
        {
            memset(fd_prop, 0, sizeof(*fd_prop));
            fd_prop->IsValid = TRUE;
            fd_prop->User = 1;
        }
    }

    return status;
}

int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name,
        uint32 *clock_accuracy, OS_TimerCallback_t callback_ptr)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerCreate);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(UT_OBJTYPE_TIMECB);
    }
    else
    {
        *timer_id = 0xDEADBEEFU;
    }

    return status;
}

int32 OS_TimeBaseCreate         (uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseCreate);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(UT_OBJTYPE_TIMEBASE);
    }
    else
    {
        *timer_id = 0xDEADBEEFU;
    }

    return status;
}

int32 OS_TimeBaseSet            (uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseSet);

    return status;
}

int32 OS_TimeBaseDelete         (uint32 timer_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_TIMEBASE, timer_id);
    }

    return status;
}

int32 OS_TimeBaseGetIdByName    (uint32 *timer_id, const char *timebase_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseGetIdByName);

    return status;
}


int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_id, OS_ArgCallback_t  callback_ptr, void *callback_arg)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerAdd);

    return status;
}

int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerSet);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskDelete.  The user can adjust the response by
**        setting the value of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskDelete(uint32 task_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TaskDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_TASK, task_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskExit stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskExit.  The variable OSTaskExitRtn.value is set to 1 and the
**        variable OSTaskExitRtn.count is incremented each time this function
**        is called.  The unit tests compare these values to expected results
**        to verify proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void OS_TaskExit()
{
    /* Although this has no retcode, invoke the hooks provided in the default impl */
    UT_DEFAULT_IMPL_RC(OS_TaskExit, 1);

}

/*****************************************************************************/
/**
** \brief dummy_function stub function
**
** \par Description
**        This function is used by the OS API function, OS_SymbolLookup, which
**        requires a valid function for which to report the address.  The user
**        defines the function's return value in the variable UT_DummyFuncRtn.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns a user-defined status value, UT_DummyFuncRtn.
**
******************************************************************************/
int32 dummy_function(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL_RC(dummy_function, 0);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_SymbolLookup stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_SymbolLookup.  The user can adjust the response by setting
**        the values in the SymbolLookupRtn structure prior to this function
**        being called.  If the value SymbolLookupRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value SymbolLookupRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_SymbolLookup(cpuaddr *symbol_address, const char *symbol_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_SymbolLookup);

    if (status == OS_SUCCESS)
    {
        *symbol_address = (cpuaddr) &dummy_function;
    }
    else
    {
        *symbol_address = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_ModuleLoad stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_ModuleLoad.  The user can adjust the response by setting
**        the values in the ModuleLoadRtn structure prior to this function
**        being called.  If the value ModuleLoadRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value ModuleLoadRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_ModuleLoad(uint32 *module_id, const char *module_name, const char *filename)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_ModuleLoad);

    if (status == OS_SUCCESS)
    {
        *module_id = UT_AllocStubObjId(UT_OBJTYPE_MODULE);
    }
    else
    {
        *module_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_ModuleUnload stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_ModuleUnload.  The user can adjust the response by setting
**        the values in the ModuleUnloadRtn structure prior to this function
**        being called.  If the value ModuleUnloadRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value ModuleUnloadRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_ModuleUnload(uint32 module_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_ModuleUnload);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_MODULE, module_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_ModuleInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_ModuleInfo.  The user can adjust the response by setting
**        the values in the ModuleInfoRtn structure prior to this function
**        being called.  If the value ModuleInfoRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value ModuleInfoRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_ModuleInfo(uint32 module_id, OS_module_prop_t *module_info)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_ModuleInfo);

    memset(module_info, 0, sizeof(*module_info));

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemTimedWait stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemTimedWait.  The variable OSBinSemTimedWaitRtn.value is set
**        to the value passed to the function, reset_type, and the variable
**        OSBinSemTimedWaitRtn.count is incremented each time this function is
**        called.  The unit tests compare these values to expected results to
**        verify proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemTimedWait(uint32 sem_id, uint32 msecs)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemTimedWait);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_HeapGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_HeapGetInfo.  If the input structure, heap_prop, is null then
**        OS_INVALID_POINTER is returned.  If not null, the user can adjust
**        the response by setting the values in the HeapGetInfoRtn structure
**        prior to this function being called.  If the value
**        HeapGetInfoRtn.count is greater than zero then the counter is
**        decremented; if it then equals zero the return value is set to the
**        user-defined value HeapGetInfoRtn.value. OS_SUCCESS is returned
**        otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag, OS_INVALID_POINTER, or
**        OS_SUCCESS.
**
******************************************************************************/
int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_HeapGetInfo);

    memset(heap_prop, 0, sizeof(*heap_prop));
    if (status == OS_SUCCESS)
    {
        /* Return some random data */
        heap_prop->free_bytes = (uint32) 12345;
        heap_prop->free_blocks = (uint32) 6789;
        heap_prop->largest_free_block = (uint32) 100;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TimerGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TimerGetInfo.  The user can adjust the response by setting
**        the values in the OSTimerGetInfoRtn structure prior to this function
**        being called.  If the value OSTimerGetInfoRtn.count is greater than
**        zero then the counter is decremented and the timer creator value is
**        set to the user-defined value OSTimerGetInfoRtn.value.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_TimerGetInfo(uint32 timer_id, OS_timer_prop_t *timer_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerGetInfo);

    memset(timer_prop, 0, sizeof(*timer_prop));
    timer_prop->creator = 1;

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TimerDelete stub function
**
** \par Description
**        This function is used as a placeholder for the OS API function
**        OS_TimerDelete.  It always returns OS_ERR_INVALID_ID.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_ERR_INVALID_ID.
**
******************************************************************************/
int32 OS_TimerDelete(uint32 timer_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_TIMECB, timer_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_IntAttachHandler stub function
**
** \par Description
**        This function is used as a placeholder for the OS API function
**        OS_IntAttachHandler.  It always returns OS_ERR_NOT_IMPLEMENTED.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_ERR_NOT_IMPLEMENTED.
**
******************************************************************************/
int32 OS_IntAttachHandler(uint32 InterruptNumber,
                          osal_task_entry InterruptHandler,
                          int32 parameter)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*
** Report and close any sockets found open
** Moved here temporarily to ensure full compatibility with CFE implementation
**
** NOTE - this historically only checked for queues that were created but not
** cleaned up.  Although the current impl could check for anything, only queues
** are done for now.
*/
void UT_CheckForOpenSockets(void)
{
    UT_ObjTypeState_t *StatePtr;
    uint32 i;
    uint32 id;
    uint32 InUse = 0;

    StatePtr = &UT_ObjState[UT_OBJTYPE_QUEUE];
    for (i=0; i <= StatePtr->LastIssueNumber; ++i)
    {
        if ((StatePtr->ValidBits[i >> 3] & (1 << (i & 0x07))) != 0)
        {
            id = i;
            UT_FIXUP_ID(id, UT_OBJTYPE_QUEUE);
            UtAssert_Failed("UT_Queue %d left open. ID=%x\n", (int)i, (unsigned int)id);
            ++InUse;
        }
    }

}

