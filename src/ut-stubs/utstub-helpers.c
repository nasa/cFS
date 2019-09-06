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

#include "utstub-helpers.h"


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
        [UT_OBJTYPE_FILESYS] = NUM_TABLE_ENTRIES,
        [UT_OBJTYPE_DIR] = OS_MAX_NUM_OPEN_DIRS
};


static UT_ObjTypeState_t UT_ObjState[UT_OBJTYPE_MAX];

/**
 * Initialization function
 */
void UT_ClearAllStubObjects (void)
{
   /*
    * Reset the fake ID numbers for create/delete operations
    */
   memset(UT_ObjState, 0, sizeof(UT_ObjState));
}



/*
 * Helper function - "allocate" a fake object ID of the given type
 */
uint32 UT_AllocStubObjId(UT_ObjType_t ObjType)
{
    UT_ObjTypeState_t *StatePtr;
    uint8 ObjMask;
    uint32 Result;

    UT_Stub_CallOnce(UT_ClearAllStubObjects);

    if (ObjType == UT_OBJTYPE_NONE || ObjType >= UT_OBJTYPE_MAX)
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
void UT_DeleteStubObjId(UT_ObjType_t ObjType, uint32 ObjId)
{
    UT_ObjTypeState_t *StatePtr;
    uint8 ObjMask;
    bool ObjWasValid;

    UT_Stub_CallOnce(UT_ClearAllStubObjects);

    /*
     * Verify the object type
     */
    if (ObjType != ((ObjId >> 16) ^ 0x4000U))
    {
        /* Calling code is broken, abort the test */
        UtAssert_Failed("Object type is not correct");
    }

    ObjId &= 0xFFFFU;
    if (ObjId >= (8 * sizeof(StatePtr->ValidBits)))
    {
        /* Calling code is broken */
        UtAssert_Failed("ObjId out of range");
    }

    StatePtr = &UT_ObjState[ObjType];

    /* Clear out any bit it could have been */
    ObjWasValid = false;
    ObjMask = 1 << (ObjId & 0x07);
    if ((StatePtr->ValidBits[ObjId >> 3] & ObjMask) != 0)
    {
        ObjWasValid = true;
        StatePtr->ValidBits[ObjId >> 3] &= ~ObjMask;
    }

    /* Unfortunately, some code has a habit of just blindly calling "Delete"
     * regardless of whether those entities were created or not.  This is
     * an issue on the non-opaque OSAL implementations and will produce
     * thousands of failed test cases, so this can only be verified
     * on the opaque objid versions.
     */
    if (!ObjWasValid)
    {
        /*
         * These stubs can be more strict about the usage patterns.
         *
         * In general any app calling delete on an ID they did not create likely
         * signifies an error in that application.
         */
        UtAssert_Failed("Deleted object that was never created");
    }
}


uint32 UT_ObjIdFixup(uint32 val, uint32 objtype)
{
    return (val | ((0x4000U | objtype) << 16));
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


