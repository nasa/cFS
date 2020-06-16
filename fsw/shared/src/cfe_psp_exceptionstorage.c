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

/******************************************************************************
**
** File:  cfe_psp_exception.c
**
**      MCP750 vxWorks 6.2 Version
**
** Purpose:
**   cFE PSP Exception related functions.
**
** History:
**   2007/05/29  A. Cudmore      | vxWorks 6.2 MCP750 version
**   2016/04/07  M.Grubb         | Updated for PSP version 1.3
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <string.h>

/*
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"

#include "cfe_psp.h"
#include "cfe_psp_config.h"
#include "cfe_psp_exceptionstorage_types.h"
#include "cfe_psp_exceptionstorage_api.h"
#include "cfe_psp_memory.h"

#include <target_config.h>

/*
**  Constants
*/
#define CFE_PSP_MAX_EXCEPTION_ENTRY_MASK    (CFE_PSP_MAX_EXCEPTION_ENTRIES-1)
#define CFE_PSP_EXCEPTION_ID_BASE           ((OS_OBJECT_TYPE_USER + 0x101) << OS_OBJECT_TYPE_SHIFT)


/***************************************************************************
 **                    INTERNAL FUNCTION DEFINITIONS
 **                 (Functions used only within the PSP itself)
 ***************************************************************************/

/*---------------------------------------------------------------------------
 * CFE_PSP_Exception_Reset
 * Internal function - see description in prototype
 *---------------------------------------------------------------------------*/
void CFE_PSP_Exception_Reset(void)
{
    /* just reset the counter */
    CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumRead =
            CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumWritten;
}

/*---------------------------------------------------------------------------
 * CFE_PSP_Exception_GetBuffer
 * Internal function - see description in prototype
 *---------------------------------------------------------------------------*/
CFE_PSP_Exception_LogData_t* CFE_PSP_Exception_GetBuffer(uint32 seq)
{
    return &CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->Entries[seq & CFE_PSP_MAX_EXCEPTION_ENTRY_MASK];
}

/*---------------------------------------------------------------------------
 * CFE_PSP_Exception_GetNextContextBuffer
 * Internal function - see description in prototype
 *---------------------------------------------------------------------------*/
CFE_PSP_Exception_LogData_t* CFE_PSP_Exception_GetNextContextBuffer(void)
{
    CFE_PSP_Exception_LogData_t* Buffer;
    uint32 NextWrite;

    NextWrite = CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumWritten;
    if ((NextWrite - CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumRead) >= CFE_PSP_MAX_EXCEPTION_ENTRIES)
    {
        /* no space to store another context */
        return NULL;
    }

    Buffer = CFE_PSP_Exception_GetBuffer(NextWrite);

    memset(Buffer, 0, sizeof(*Buffer));
    Buffer->context_id = CFE_PSP_EXCEPTION_ID_BASE + (NextWrite & OS_OBJECT_INDEX_MASK);

    return Buffer;
}

/*---------------------------------------------------------------------------
 * CFE_PSP_Exception_WriteComplete
 * Internal function - see description in prototype
 *---------------------------------------------------------------------------*/
void CFE_PSP_Exception_WriteComplete(void)
{
    CFE_PSP_Exception_LogData_t* Buffer;

    /*
     * Incrementing the "NumWritten" field allows the application to receive this data
     */
    ++CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumWritten;

    /*
     * preemptively zero-out the "id" field of the _next_ entry -
     * this expires the buffer, which prevents it from being read.
     *
     * This is just in case another exception occurs while a log
     * file write is in progress.  It's not 100% foolproof, but
     * it does reduce the chance of data corruption (because it
     * is not possible to "lock out" exceptions, they can occur at
     * any time code is running)
     */
    Buffer = CFE_PSP_Exception_GetBuffer(CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumWritten);
    Buffer->context_id = 0;
}


/***************************************************************************
 **                    EXTERNAL FUNCTION DEFINITIONS
 **                   (Functions used by CFE or PSP)
 ***************************************************************************/

/*---------------------------------------------------------------------------
 * CFE_PSP_Exception_GetCount
 * See description in PSP API
 *---------------------------------------------------------------------------*/
uint32 CFE_PSP_Exception_GetCount(void)
{
    return (CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumWritten - CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumRead);
}

/*---------------------------------------------------------------------------
 * CFE_PSP_Exception_GetSummary
 * See description in PSP API
 *---------------------------------------------------------------------------*/
int32 CFE_PSP_Exception_GetSummary(uint32 *ContextLogId, uint32 *TaskId, char *ReasonBuf, uint32 ReasonSize)
{
    const CFE_PSP_Exception_LogData_t* Buffer;
    uint32 NumStored;
    int32 Status;

    NumStored = CFE_PSP_Exception_GetCount();
    if (NumStored == 0)
    {
        /* no context available for reading */
        return CFE_PSP_NO_EXCEPTION_DATA;
    }

    Buffer = CFE_PSP_Exception_GetBuffer(CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumRead);

    /*
     * Store the abstract entry ID for future retrieval (e.g. log to file)
     */
    if (ContextLogId != NULL)
    {
        *ContextLogId = Buffer->context_id;
    }

    /*
     * If caller supplied a TaskID buffer, then call OSAL to reverse-lookup
     * the abstract ID from the underlying system thread ID.
     */
    if (TaskId != NULL)
    {
        Status = OS_TaskFindIdBySystemData(TaskId, &Buffer->sys_task_id, sizeof(Buffer->sys_task_id));
        if (Status != OS_SUCCESS)
        {
            *TaskId = 0; /* failed to find a corresponding OSAL ID, so set to zero. */
        }
    }

    /*
     * If caller supplied a reason buffer, then call the implementation to fill it.
     */
    if (ReasonBuf != NULL && ReasonSize > 0)
    {
        Status = CFE_PSP_ExceptionGetSummary_Impl(Buffer, ReasonBuf, ReasonSize);
        if (Status != CFE_PSP_SUCCESS)
        {
            ReasonBuf[0] = 0; /* failed to get a reason, so return empty string */
        }
    }

    ++CFE_PSP_ReservedMemoryMap.ExceptionStoragePtr->NumRead;

    /*
     * returning SUCCESS to indicate an entry was popped from the queue
     *
     * this doesn't necessarily mean that the output fields have valid data,
     * but it does mean they are initialized to something.
     */
    return CFE_PSP_SUCCESS;
}

/*---------------------------------------------------------------------------
 * CFE_PSP_Exception_CopyContext
 * See description in PSP API
 *---------------------------------------------------------------------------*/
int32 CFE_PSP_Exception_CopyContext(uint32 ContextLogId, void *ContextBuf, uint32 ContextSize)
{
    const CFE_PSP_Exception_LogData_t* Buffer;
    uint32 SeqId;
    uint32 ActualSize;

    SeqId = ContextLogId - CFE_PSP_EXCEPTION_ID_BASE;
    if (SeqId > OS_OBJECT_INDEX_MASK)
    {
        /* supplied ID is not valid at all */
        return CFE_PSP_NO_EXCEPTION_DATA;
    }

    Buffer = CFE_PSP_Exception_GetBuffer(SeqId);
    if (Buffer->context_id != ContextLogId)
    {
        /* data has expired from the memory log */
        return CFE_PSP_NO_EXCEPTION_DATA;
    }

    /* Copy the "context info" out to the buffer.
     * But do not copy more than the output buffer. */
    if (ContextSize >= Buffer->context_size)
    {
        ActualSize = Buffer->context_size;
    }
    else
    {
        /* this will truncate, not ideal, but no alternative.
         * If this happens it generally indicates a misconfiguration between CFE and PSP,
         * where the CFE platform configuration has not allocated enough space for context logs.
         * Generate a warning message to raise awareness. */
        OS_printf("CFE_PSP: Insufficient buffer for exception context, total=%lu bytes, saved=%lu\n",
                (unsigned long)Buffer->context_size, (unsigned long)ContextSize);
        ActualSize = ContextSize;
    }


    memcpy(ContextBuf, &Buffer->context_info, ActualSize);

    /*
     * The return value is the actual size of copied data.
     */
    return (int32)ActualSize;
}

