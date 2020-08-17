/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     coveragetest-idmap.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

#include <OCS_string.h>


typedef struct
{
    uint32 TaskCount;
    uint32 QueueCount;
    uint32 MutexCount;
    uint32 OtherCount;
} Test_OS_ObjTypeCount_t;

/* a match function that always matches */
static bool TestAlwaysMatch(void *ref, uint32 local_id, const OS_common_record_t *obj)
{
    return true;
}

static void ObjTypeCounter(uint32 object_id, void *arg)
{
    Test_OS_ObjTypeCount_t *count = arg;

    switch(OS_IdentifyObject(object_id))
    {
    case OS_OBJECT_TYPE_OS_TASK:
        ++count->TaskCount;
        break;
    case OS_OBJECT_TYPE_OS_QUEUE:
        ++count->QueueCount;
        break;
    case OS_OBJECT_TYPE_OS_MUTEX:
        ++count->MutexCount;
        break;
    default:
        ++count->OtherCount;
        break;
    }
}


void Test_OS_ObjectIdInit(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdInit(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_ObjectIdInit();

    UtAssert_True(actual == expected, "OS_ObjectIdInit() (%ld) == %ld", (long)actual, (long)expected);
}


void Test_OS_LockUnlockGlobal(void)
{
    /*
     * Test Case For:
     * void OS_Lock_Global(uint32 idtype)
     * void OS_Unlock_Global(uint32 idtype)
     */

    /*
     * As these have no return codes, these tests
     * exist to get coverage of the paths.
     */
    OS_Lock_Global(OS_OBJECT_TYPE_OS_COUNTSEM);
    OS_Unlock_Global(OS_OBJECT_TYPE_OS_COUNTSEM);
    OS_Lock_Global(0);
    OS_Unlock_Global(0);
    OS_Lock_Global(55555);
    OS_Unlock_Global(55555);

    UT_SetForceFail(UT_KEY(OS_TaskGetId), 0);
    OS_Lock_Global(OS_OBJECT_TYPE_OS_BINSEM);
    OS_Unlock_Global(OS_OBJECT_TYPE_OS_BINSEM);
}

void Test_OS_ObjectIdConvertLock(void)
{
    /*
     * Test Case For:
     * static int32 OS_ObjectIdConvertLock(OS_lock_mode_t lock_mode,
     *      uint32 idtype, uint32 reference_id, OS_common_record_t *obj)
     *
     * NOTE: These test cases just focus on code paths that are not exercised
     * by the other test cases in this file.
     */
    int32 expected;
    int32 actual;
    uint32 array_index;
    OS_common_record_t *record;
    uint32 objid;

    /* get a valid (fake) OSAL ID to start with */
    OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "ut", &array_index, &record);
    objid = record->active_id;

    /*
     * Attempt to obtain a lock for the same record with a non-matching ID
     * This should return an error.
     */
    actual = OS_ObjectIdConvertLock(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TASK,
            objid + 123, record);
    expected = OS_ERR_INVALID_ID;

    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock() (%ld) == OS_ERR_INVALID_ID (%ld)", (long)actual, (long)expected);

    /*
     * Use mode OS_LOCK_MODE_NONE with matching ID
     * This should return success.
     */
    actual = OS_ObjectIdConvertLock(OS_LOCK_MODE_NONE,
            OS_OBJECT_TYPE_OS_TASK, objid, record);
    expected = OS_SUCCESS;

    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock() (%ld) == OS_ERR_INVALID_ID (%ld)", (long)actual, (long)expected);

    /*
     * Use mode OS_LOCK_MODE_EXCLUSIVE with matching ID and no other refs.
     * This should return success.
     */
    record->flags = 0;
    record->refcount = 0;
    actual = OS_ObjectIdConvertLock(OS_LOCK_MODE_EXCLUSIVE,
            OS_OBJECT_TYPE_OS_TASK, objid, record);
    expected = OS_SUCCESS;

    UtAssert_True(actual == expected, "OS_ObjectIdConvertLock() (%ld) == OS_SUCCESS (%ld)", (long)actual, (long)expected);

}

void Test_OS_ObjectIdGetBySearch(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, uint32 idtype,
     *      OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_common_record_t **record)
     *
     * NOTE: These test cases just focus on code paths that are not exercised
     * by the other test cases in this file.
     */
    int32 expected;
    int32 actual;
    OS_common_record_t *record;

    OS_global_task_table[0].active_id = 0x11111;
    actual = OS_ObjectIdGetBySearch(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TASK,
            TestAlwaysMatch, NULL, &record);
    expected = OS_SUCCESS;
    OS_global_task_table[0].active_id = 0;

    UtAssert_True(actual == expected, "OS_ObjectIdGetBySearch() (%ld) == OS_SUCCESS (%ld)", (long)actual, (long)expected);

}


void Test_OS_GetMaxForObjectType(void)
{
    /*
     * Test Case For:
     * uint32 OS_GetMaxForObjectType(uint32 idtype);
     */
    uint32 idtype = 0;
    uint32 expected = 0xFFFFFFFF;
    uint32 max = 0;

    for (idtype = 0; idtype < OS_OBJECT_TYPE_USER; ++idtype)
    {
        if (idtype == OS_OBJECT_TYPE_UNDEFINED)
        {
            expected = 0;
        }
        else
        {
            expected = 0xFFFFFFFF;
        }

        max = OS_GetMaxForObjectType(idtype);

        if (expected != 0xFFFFFFFF)
        {
            /* Verify Outputs */
            /* only the "0" is verifiable, since everything else depends on OS config */
            UtAssert_True(max == expected, "OS_GetMaxForObjectType(%lu) (%lu) == 0",
                    (unsigned long)idtype, (unsigned long)max);
        }
    }

}

void Test_OS_GetBaseForObjectType(void)
{
    /*
     * Test Case For:
     * uint32 OS_GetBaseForObjectType(uint32 idtype);
     */
    uint32 idtype = 0;
    uint32 expected = 0xFFFFFFFF;
    uint32 max = 0;

    for (idtype = 0; idtype < OS_OBJECT_TYPE_USER; ++idtype)
    {
        if (idtype == OS_OBJECT_TYPE_UNDEFINED)
        {
            expected = 0;
        }
        else
        {
            expected = 0xFFFFFFFF;
        }

        max = OS_GetBaseForObjectType(idtype);

        if (expected != 0xFFFFFFFF)
        {
            /* Verify Outputs */
            /* only the "0" is verifiable, since everything else depends on OS config */
            UtAssert_True(max == expected, "OS_GetBaseForObjectType(%lu) (%lu) == 0",
                    (unsigned long)idtype, (unsigned long)max);
        }
    }
}

void Test_OS_ObjectIdToArrayIndex(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdToArrayIndex(uint32 idtype, uint32 id, uint32 *ArrayIndex);
     *
     * Internally this uses OS_Id_UnMap for the nominal case, which is covered by a
     * different test case.  The only additional test here is to provide a value
     * which is out of range.
     */
    uint32 objid = 0xFFFFFFFF;
    uint32 local_idx = 0xFFFFFFFF;
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;

    /* need to get a "valid" objid for the nominal case */
    OS_ObjectIdCompose_Impl(OS_OBJECT_TYPE_OS_TASK, 1, &objid);
    actual = OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, objid, &local_idx);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdToArrayIndex() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(local_idx == 1, "local_idx (%lu) == 1", (unsigned long)local_idx);

    /* coverage for off-nominal case */
    expected = OS_ERR_INVALID_ID;
    actual   = OS_ObjectIdToArrayIndex(0xFFFFFFFF, 0xFFFFFFFF, &local_idx);
    UtAssert_True(actual == expected, "OS_ObjectIdToArrayIndex() (%ld) == OS_ERR_INVALID_ID", (long)actual);
}

void Test_OS_ObjectIdFindByName (void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectFindIdByName (uint32 idtype, const char *name, uint32 *object_id);
     *
     * Nominal case (with no additional setup) should return OS_ERR_NAME_NOT_FOUND
     * Setting up a special matching entry should yield OS_SUCCESS
     */
    char TaskName[] = "UT_find";
    uint32 objid = 0xFFFFFFFF;
    int32 expected = OS_ERR_NAME_NOT_FOUND;
    int32 actual   = OS_ObjectIdFindByName(OS_OBJECT_TYPE_UNDEFINED, NULL, NULL);

    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_ERR_NAME_NOT_FOUND", "NULL", (long)actual);

    /*
     * Pass in a name that is beyond OS_MAX_API_NAME
     */
    UT_SetForceFail(UT_KEY(OCS_strlen), OS_MAX_API_NAME + 10);
    expected = OS_ERR_NAME_TOO_LONG;
    actual   = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);
    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_ERR_NAME_TOO_LONG", TaskName, (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));


    /*
     * Pass in a name that is actually not found
     */
    OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);
    expected = OS_ERR_NAME_NOT_FOUND;
    actual   = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);

    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_ERR_NAME_NOT_FOUND", TaskName, (long)actual);


    /*
     * Set up for the ObjectIdSearch function to return success
     */
    OS_global_task_table[0].active_id = 0x11111;
    OS_global_task_table[0].name_entry = TaskName;
    actual = OS_ObjectIdFindByName(OS_OBJECT_TYPE_OS_TASK, TaskName, &objid);
    expected = OS_SUCCESS;
    OS_global_task_table[0].active_id = 0;
    OS_global_task_table[0].name_entry = NULL;

    UtAssert_True(actual == expected, "OS_ObjectFindIdByName(%s) (%ld) == OS_SUCCESS", TaskName, (long)actual);

}

void Test_OS_ObjectIdGetById(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdGetById(OS_lock_mode_t check_mode, uint32 idtype, uint32 id, uint32 *array_index, OS_common_record_t **record);
     *
     */
    int32 actual   = ~OS_SUCCESS;
    int32 expected = OS_SUCCESS;
    uint32 refobjid = 0xFFFFFFFF;
    uint32 local_idx = 0xFFFFFFFF;
    OS_common_record_t *rptr = NULL;

    /* verify that the call returns ERROR when not initialized */
    OS_SharedGlobalVars.Initialized = false;
    actual = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, 0, 0, &local_idx, &rptr);
    expected = OS_ERROR;
    UtAssert_True(actual == expected, "OS_ObjectIdGetById(uninitialized) (%ld) == OS_ERROR", (long)actual);

    /* set "true" for the remainder of tests */
    OS_SharedGlobalVars.Initialized = true;

    OS_ObjectIdCompose_Impl(OS_OBJECT_TYPE_OS_TASK, 1, &refobjid);
    OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TASK, refobjid, &local_idx);
    OS_global_task_table[local_idx].active_id = refobjid;
    expected = OS_SUCCESS;
    actual = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_TASK, refobjid, &local_idx, &rptr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(local_idx == 1, "local_idx (%lu) == 1", (unsigned long)local_idx);
    UtAssert_True(rptr != NULL, "rptr (%p) != NULL", (void*)rptr);
    UtAssert_True(rptr->refcount == 1, "refcount (%u) == 1",
            (unsigned int)rptr->refcount);

    /* attempting to get an exclusive lock should return IN_USE error */
    expected = OS_ERR_OBJECT_IN_USE;
    actual = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, OS_OBJECT_TYPE_OS_TASK, refobjid, &local_idx, &rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_ERR_OBJECT_IN_USE", (long)actual);

    /* attempt to get non-exclusive lock during shutdown should fail */
    OS_SharedGlobalVars.ShutdownFlag = OS_SHUTDOWN_MAGIC_NUMBER;
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_TASK, refobjid, &local_idx, &rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
    OS_SharedGlobalVars.ShutdownFlag = 0;

    /* attempt to get lock for invalid type object should fail */
    expected = OS_ERR_INVALID_ID;
    actual = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, 0xFFFFFFFF, refobjid, &local_idx, &rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdGetById() (%ld) == OS_ERR_INVALID_ID", (long)actual);
    OS_SharedGlobalVars.ShutdownFlag = 0;


    /* refcount decrement should work */
    expected = OS_SUCCESS;
    actual = OS_ObjectIdRefcountDecr(rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdRefcountDecr() (%ld) == OS_SUCCESS", (long)actual);

    /* decrement should fail if done a second time */
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_ObjectIdRefcountDecr(rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdRefcountDecr() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);

    /* clear out state entry */
    memset(&OS_global_task_table[local_idx], 0, sizeof(OS_global_task_table[local_idx]));

    expected = OS_ERR_INVALID_ID;
    actual = OS_ObjectIdRefcountDecr(rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdRefcountDecr() (%ld) == OS_ERR_INVALID_ID", (long)actual);

}

void Test_OS_ObjectIdFindNext(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdFindNext(uint32 idtype, uint32 *array_index, OS_common_record_t **record);
     * int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_common_record_t *record, uint32 *outid);
     *
     * Note This test case covers both functions because they are somewhat interlinked and
     * they share state between them - The output of FindNext() should be passed to Finalize()
     * before use.
     */

    int32 expected;
    int32 actual;
    OS_common_record_t *rec1;
    OS_common_record_t *rec2;
    uint32 id1;
    uint32 id2;
    uint32 saved_id;
    uint32 i;

    /* Need to first obtain a valid ID to finalize */
    expected = OS_SUCCESS;
    actual = OS_ObjectIdFindNext(OS_OBJECT_TYPE_OS_TASK, NULL, &rec1);
    UtAssert_True(actual == expected, "OS_ObjectIdFindNext() (%ld) == OS_SUCCESS", (long)actual);

    /* nominal case (success) */
    id1 = ~rec1->active_id;
    actual = OS_ObjectIdFinalizeNew(OS_SUCCESS, rec1, &id1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdFinalizeNew() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(id1 == rec1->active_id, "OS_ObjectIdFinalizeNew() id (%lx) == %lx",
            (unsigned long)id1, (unsigned long)rec1->active_id);

    /* Allocate another ID (should be different!) */
    actual = OS_ObjectIdFindNext(OS_OBJECT_TYPE_OS_TASK, NULL, &rec2);
    UtAssert_True(actual == expected, "OS_ObjectIdFindNext() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(rec2->active_id != rec1->active_id, "OS_ObjectIdFindNext() id (%lx) != %lx",
            (unsigned long)rec2->active_id, (unsigned long)rec1->active_id);

    /* Failure to initialize the second one.
     * Verify the error code passes thru */
    expected = -1234;
    saved_id = rec2->active_id;
    id2 = ~rec2->active_id;
    actual = OS_ObjectIdFinalizeNew(expected, rec2, &id2);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdFinalizeNew() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(id2 == 0, "OS_ObjectIdFinalizeNew() id (%lx) == 0", (unsigned long)id2);
    UtAssert_True(rec2->active_id == 0, "OS_ObjectIdFinalizeNew() active_id cleared");

    /* next call should re-issue the same id because init failed */
    expected = OS_SUCCESS;
    actual = OS_ObjectIdFindNext(OS_OBJECT_TYPE_OS_TASK, NULL, &rec2);
    UtAssert_True(actual == expected, "OS_ObjectIdFindNext() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(rec2->active_id == saved_id, "OS_ObjectIdFindNext() id (%lx) != %lx",
            (unsigned long)rec2->active_id, (unsigned long)saved_id);


    /* test invalid case*/
    rec2->active_id = 0;
    expected = OS_ERR_INVALID_ID;
    actual = OS_ObjectIdFinalizeNew(OS_SUCCESS, rec2, &id2);
    UtAssert_True(actual == expected, "OS_ObjectIdFinalizeNew() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(id2 == 0, "OS_ObjectIdFinalizeNew() id (%lx) == 0", (unsigned long)id2);
    UtAssert_True(rec2->active_id == 0, "OS_ObjectIdFinalizeNew() active_id cleared");

    /*
     * Finally - test the wrap-around function to verify that object IDs
     * will continue to allocate correctly after OS_OBJECT_INDEX_MASK
     */
    expected = OS_SUCCESS;
    saved_id = 0;
    for (i=0; i < (OS_OBJECT_INDEX_MASK+2); ++i)
    {
        actual = OS_ObjectIdFindNext(OS_OBJECT_TYPE_OS_TASK, NULL, &rec2);
        /* not usuing UtAssert_True here as it will create thousands  of duplicates. */
        if (expected != actual)
        {
            UtAssert_Failed("OS_ObjectIdFindNext() failure (%ld)",(long)actual);
            break;
        }

        actual = OS_ObjectIdFinalizeNew(OS_SUCCESS, rec2, NULL);
        if (expected != actual)
        {
            UtAssert_Failed("OS_ObjectIdFinalizeNew() failure (%ld)",(long)actual);
            break;
        }

        /* should always be different than the previous ID */
        if (saved_id == rec2->active_id)
        {
            UtAssert_Failed("OS_ObjectIdFindNext() re-issued ID (%lx)",(unsigned long)saved_id);
            break;
        }

        /* it also should never be id1, which was previously allocated */
        if (id1 == rec2->active_id)
        {
            UtAssert_Failed("OS_ObjectIdFindNext() duplicate ID (%lx)",(unsigned long)id1);
            break;
        }
        if (rec1 == rec2)
        {
            UtAssert_Failed("OS_ObjectIdFindNext() duplicate slot (%p)", (void*)rec1);
            break;
        }

        /* Find the wrap.  Once this occurs the test is successful. */
        if (saved_id > rec2->active_id)
        {
            /* Success */
            break;
        }

        /* clear the entry for re-use */
        saved_id = rec2->active_id;
        rec2->active_id = 0;
    }

    /* verify that the wrap occurred */
    UtAssert_True(i < (OS_OBJECT_INDEX_MASK+2), "OS_ObjectIdFindNext() wrap around occurred");

}

void Test_OS_ObjectIdAllocateNew(void)
{
    /*
     * Test Case For:
     * int32 OS_ObjectIdAllocateNew(uint32 idtype, const char *name, uint32 *array_index, OS_common_record_t **record);
     *
     * Most of the business logic is done by OS_ObjectIdFindNext() which is tested separately
     * This test case mainly focuses on additional error checking
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    OS_common_record_t *rptr = NULL;

    actual = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "UT_alloc", &objid, &rptr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(rptr != NULL, "rptr (%p) != NULL", (void*)rptr);

    /* Passing a NULL name also should work here (used for internal objects) */
    actual = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, NULL, &objid, &rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate(NULL) (%ld) == OS_SUCCESS", (long)actual);

    rptr->name_entry = "UT_alloc";
    expected = OS_ERR_NAME_TAKEN;
    actual = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "UT_alloc", &objid, &rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_ERR_NAME_TAKEN", (long)actual);

    OS_SharedGlobalVars.ShutdownFlag = OS_SHUTDOWN_MAGIC_NUMBER;
    expected = OS_ERROR;
    actual = OS_ObjectIdAllocateNew(OS_OBJECT_TYPE_OS_TASK, "UT_alloc", &objid, &rptr);
    OS_SharedGlobalVars.ShutdownFlag = 0;
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_ERR_NAME_TAKEN", (long)actual);

    expected = OS_ERR_INCORRECT_OBJ_TYPE;
    actual = OS_ObjectIdAllocateNew(0xFFFFFFFF, "UT_alloc", &objid, &rptr);
    UtAssert_True(actual == expected, "OS_ObjectIdAllocate() (%ld) == OS_ERR_INCORRECT_OBJ_TYPE", (long)actual);

}

void Test_OS_ConvertToArrayIndex(void)
{
    /*
     * Test Case For:
     * int32 OS_ConvertToArrayIndex(uint32 object_id, uint32 *ArrayIndex);
     *
     *
     */
    int32 expected = OS_SUCCESS;
    int32 actual   = ~OS_SUCCESS; //OS_ConvertToArrayIndex();
    uint32 local_idx1 = 0xFFFFFFF0;
    uint32 local_idx2 = 0xFFFFFFF1;
    OS_common_record_t *rptr = NULL;

    /* Need a valid ID to work with */
    OS_ObjectIdFindNext(OS_OBJECT_TYPE_OS_TASK, &local_idx1, &rptr);
    actual = OS_ConvertToArrayIndex(rptr->active_id, &local_idx2);
    UtAssert_True(actual == expected, "OS_ConvertToArrayIndex() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(local_idx1 == local_idx2, "local_idx1 (%lu) == local_idx2 (%lu)",
            (unsigned long)local_idx1, (unsigned long)local_idx2);

    expected = OS_ERR_INVALID_ID;
    actual = OS_ConvertToArrayIndex(0, &local_idx2);
    UtAssert_True(actual == expected, "OS_ConvertToArrayIndex() (%ld) == OS_ERR_INVALID_ID", (long)actual);
}

void Test_OS_ForEachObject(void)
{
    /*
     * Test Case For:
     * void OS_ForEachObject (uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);
     */
    uint32 objtype = OS_OBJECT_TYPE_UNDEFINED;
    OS_common_record_t *rptr = NULL;
    uint32 local_idx = 0xFFFFFFFF;
    uint32 self_id;
    Test_OS_ObjTypeCount_t Count;

    self_id = OS_TaskGetId();

    memset(&Count, 0, sizeof(Count));

    while (objtype < OS_OBJECT_TYPE_USER)
    {
        OS_ObjectIdFindNext(objtype, &local_idx, &rptr);
        ++objtype;
    }

    OS_ForEachObject (0, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 1, "OS_ForEachObject() TaskCount (%lu) == 1", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 1, "OS_ForEachObject() QueueCount (%lu) == 1", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.MutexCount == 1, "OS_ForEachObject() MutexCount (%lu) == 1", (unsigned long)Count.MutexCount);
    UtAssert_True(Count.OtherCount == 9, "OS_ForEachObject() OtherCount (%lu) == 9", (unsigned long)Count.OtherCount);

    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_QUEUE, self_id, ObjTypeCounter, &Count);
    UtAssert_True(Count.TaskCount == 1, "OS_ForEachObjectOfType(), creator %08lx TaskCount (%lu) == 1", (unsigned long)self_id, (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 2, "OS_ForEachObjectOfType() QueueCount (%lu) == 2", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.MutexCount == 1, "OS_ForEachObjectOfType() MutexCount (%lu) == 1", (unsigned long)Count.MutexCount);

    OS_ForEachObjectOfType(OS_OBJECT_TYPE_OS_QUEUE, self_id ^ 1, ObjTypeCounter, &Count);
    UtAssert_True(Count.TaskCount == 1, "OS_ForEachObjectOfType(), non-matching creator TaskCount (%lu) == 1", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 2, "OS_ForEachObjectOfType() QueueCount (%lu) == 2", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.MutexCount == 1, "OS_ForEachObjectOfType() MutexCount (%lu) == 1", (unsigned long)Count.MutexCount);
}

void Test_OS_GetResourceName(void)
{
    /*
     * Test Case For:
     * int32 OS_GetResourceName(uint32 id, char *buffer, uint32 buffer_size)
     */
    uint32 local_idx = 0xFFFFFFFF;
    OS_common_record_t *rptr = NULL;
    char NameBuffer[OS_MAX_API_NAME];
    int32 expected;
    int32 actual;

    /*
     * Set up for the OS_GetResourceName function to return success
     */
    /* Need a valid ID to work with */
    OS_ObjectIdFindNext(OS_OBJECT_TYPE_OS_TASK, &local_idx, &rptr);
    rptr->name_entry = "UTTask";
    expected = OS_SUCCESS;
    actual = OS_GetResourceName(rptr->active_id, NameBuffer, sizeof(NameBuffer));
    UtAssert_True(actual == expected, "OS_GetResourceName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(NameBuffer, "UTTask") == 0, "NameBuffer (%s) == UTTask", NameBuffer);

    expected = OS_ERR_NAME_TOO_LONG;
    actual = OS_GetResourceName(rptr->active_id, NameBuffer, 2);
    UtAssert_True(actual == expected, "OS_GetResourceName() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_GetResourceName(rptr->active_id, NULL, 0);
    UtAssert_True(actual == expected, "OS_GetResourceName() (%ld) == OS_INVALID_POINTER", (long)actual);
}

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);

    /* for sanity also clear out the task table, which is used by several test cases */
    memset(OS_global_task_table, 0, OS_MAX_TASKS * sizeof(OS_common_record_t));

    /*
     * The OS_SharedGlobalVars is also used here, but set the
     * "Initialized" field to true by default, as this is needed by most tests.
     */
    memset(&OS_SharedGlobalVars, 0, sizeof(OS_SharedGlobalVars));
    OS_SharedGlobalVars.Initialized = true;
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void)
{

}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_ObjectIdInit);
    ADD_TEST(OS_LockUnlockGlobal);
    ADD_TEST(OS_ObjectIdFindNext);
    ADD_TEST(OS_ObjectIdToArrayIndex);
    ADD_TEST(OS_ObjectIdFindByName);
    ADD_TEST(OS_ObjectIdGetById);
    ADD_TEST(OS_ObjectIdAllocateNew);
    ADD_TEST(OS_ObjectIdConvertLock);
    ADD_TEST(OS_ObjectIdGetBySearch);
    ADD_TEST(OS_ConvertToArrayIndex);
    ADD_TEST(OS_ForEachObject);
    ADD_TEST(OS_GetMaxForObjectType);
    ADD_TEST(OS_GetBaseForObjectType);
    ADD_TEST(OS_GetResourceName);
}



