/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-shared-coveragetest.h"
#include "ut-osapi-task.h"

#include <overrides/string.h>

static uint32 UT_TestHook_Count = 0;

static void UT_TestHook(void)
{
    ++UT_TestHook_Count;
}

/*
**********************************************************************************
**          INTERNAL API TEST CASES
**********************************************************************************
*/

void Test_OS_TaskEntryPoint(void)
{
    /*
     * Test Case For:
     * void OS_TaskEntryPoint(uint32 task_id)
     */

    /* by default with no additional setup,
     * the internal "prepare" call will return INVALID_ID,
     * which in turn will invoke OS_TaskExit()
     */
    uint32 CallCount = 0;

    UT_TestHook_Count = 0;
    OS_TaskEntryPoint(1);
    UtAssert_True(UT_TestHook_Count == 0, "UT_TestHook_Count (%lu) == 0",
            (unsigned long)UT_TestHook_Count);
    CallCount = UT_GetStubCount(UT_KEY(OS_TaskMatch_Impl));
    UtAssert_True(CallCount == 0, "OS_TaskMatch_Impl() count (%lu) == 0",
            (unsigned long)CallCount);

    CallCount = UT_GetStubCount(UT_KEY(OS_TaskRegister_Impl));
    UtAssert_True(CallCount == 0, "OS_TaskRegister_Impl() count (%lu) == 0",
            (unsigned long)CallCount);

    OS_global_task_table[1].active_id = 1;
    OS_task_table[1].entry_function_pointer = UT_TestHook;

    OS_TaskEntryPoint(1);
    UtAssert_True(UT_TestHook_Count == 1, "UT_TestHook_Count (%lu) == 1",
            (unsigned long)UT_TestHook_Count);
    CallCount = UT_GetStubCount(UT_KEY(OS_TaskMatch_Impl));
    UtAssert_True(CallCount == 1, "OS_TaskMatch_Impl() count (%lu) == 1",
            (unsigned long)CallCount);

    CallCount = UT_GetStubCount(UT_KEY(OS_TaskRegister_Impl));
    UtAssert_True(CallCount == 1, "OS_TaskRegister_Impl() count (%lu) == 1",
            (unsigned long)CallCount);

    OS_global_task_table[1].active_id = 0;
    OS_task_table[1].entry_function_pointer = NULL;

}

void Test_OS_TaskAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TaskAPI_Init();

    UtAssert_True(actual == expected, "OS_TaskAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

/*
**********************************************************************************
**          TASK API TEST CASES
**********************************************************************************
*/

void Test_OS_TaskCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskCreate (uint32 *task_id, const char *task_name, osal_task_entry function_pointer,
     *                uint32 *stack_pointer, uint32 stack_size, uint32 priority, uint32 flags)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual = OS_TaskCreate(&objid, "UT", UT_TestHook, NULL, 0, 0,0);

    UtAssert_True(actual == expected, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate(NULL, NULL, NULL, NULL, 0, 0,0), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate(&objid, "UT", UT_TestHook, NULL, 0, 10 + OS_MAX_TASK_PRIORITY,0), OS_ERR_INVALID_PRIORITY);
    UT_SetForceFail(UT_KEY(OCS_strlen), 10 + OS_MAX_API_NAME);
    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate(&objid, "UT", UT_TestHook, NULL, 0, 0,0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_TaskDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelete (uint32 task_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    UT_TestHook_Count = 0;
    OS_task_table[1].delete_hook_pointer = UT_TestHook;
    actual = OS_TaskDelete(1);

    UtAssert_True(actual == expected, "OS_TaskDelete() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(UT_TestHook_Count == 1, "UT_TestHook_Count (%lu) == 1",
            (unsigned long)UT_TestHook_Count);

    UT_SetForceFail(UT_KEY(OS_TaskDelete_Impl), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TaskDelete(1), OS_ERROR);
    UtAssert_True(UT_TestHook_Count == 1, "UT_TestHook_Count (%lu) == 1",
            (unsigned long)UT_TestHook_Count);

    OS_task_table[1].delete_hook_pointer = NULL;
}

void Test_OS_TaskExit(void)
{
    /*
     * Test Case For:
     * void OS_TaskExit()
     */
    uint32 local_index = 0;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.active_id = 1;
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);

    OS_TaskExit();

    /* TaskExit should have cleared the active_id */
    UtAssert_True(utrec.active_id == 0, "utrec.active_id (%lu) == 0",
            (unsigned long)utrec.active_id);
}
void Test_OS_TaskDelay(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelay(uint32 millisecond)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TaskDelay(1);

    UtAssert_True(actual == expected, "OS_TaskDelay() (%ld) == OS_SUCCESS", (long)actual);
}
void Test_OS_TaskSetPriority(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskSetPriority (uint32 task_id, uint32 new_priority)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TaskSetPriority(1, 1);

    UtAssert_True(actual == expected, "OS_TaskSetPriority() (%ld) == OS_SUCCESS", (long)actual);

    OSAPI_TEST_FUNCTION_RC(OS_TaskSetPriority(1, 10 + OS_MAX_TASK_PRIORITY), OS_ERR_INVALID_PRIORITY);
}
void Test_OS_TaskRegister(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskRegister (void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TaskRegister();

    UtAssert_True(actual == expected, "OS_TaskRegister() (%ld) == OS_SUCCESS", (long)actual);
}
void Test_OS_TaskGetId(void)
{
    /*
     * Test Case For:
     * uint32 OS_TaskGetId (void)
     */
    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 5555);
    OSAPI_TEST_FUNCTION_RC(OS_TaskGetId(), 5555);

    UT_SetForceFail(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TaskGetId(), 0);
}

void Test_OS_TaskGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskGetIdByName (uint32 *task_id, const char *task_name)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 objid = 0;

    UT_SetForceFail(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_TaskGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_TaskGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "OS_TaskGetIdByName() objid (%lu) != 0", (unsigned long)objid);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_TaskGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TaskGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_TaskGetIdByName(NULL, NULL), OS_INVALID_POINTER);
}

void Test_OS_TaskGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskGetInfo (uint32 task_id, OS_task_prop_t *task_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_task_prop_t task_prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    OS_task_table[1].stack_size = 222;
    OS_task_table[1].priority = 333;
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_TaskGetInfo(1, &task_prop);

    UtAssert_True(actual == expected, "OS_TaskGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(task_prop.creator == 111, "task_prop.creator (%lu) == 111",
            (unsigned long)task_prop.creator);
    UtAssert_True(strcmp(task_prop.name, "ABC") == 0, "task_prop.name (%s) == ABC",
            task_prop.name);
    UtAssert_True(task_prop.stack_size == 222, "task_prop.stack_size (%lu) == 222",
            (unsigned long)task_prop.stack_size);
    UtAssert_True(task_prop.priority == 333, "task_prop.priority (%lu) == 333",
            (unsigned long)task_prop.priority);

    OS_task_table[1].stack_size = 0;
    OS_task_table[1].priority = 0;

    OSAPI_TEST_FUNCTION_RC(OS_TaskGetInfo(0, NULL), OS_INVALID_POINTER);
}

void Test_OS_TaskInstallDeleteHandler(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1);
    actual = OS_TaskInstallDeleteHandler(UT_TestHook);
    UtAssert_True(actual == expected, "OS_TaskInstallDeleteHandler() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(OS_task_table[1].delete_hook_pointer == UT_TestHook, "OS_task_table[1].delete_hook_pointer (%p) == %p",
            OS_task_table[1].delete_hook_pointer, UT_TestHook);

    OS_task_table[1].delete_hook_pointer = NULL;
}


/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Osapi_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_TearDown(void)
{

}


/*
 * Register the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_TaskAPI_Init);
    ADD_TEST(OS_TaskEntryPoint);
    ADD_TEST(OS_TaskCreate);
    ADD_TEST(OS_TaskDelete);
    ADD_TEST(OS_TaskExit);
    ADD_TEST(OS_TaskDelay);
    ADD_TEST(OS_TaskSetPriority);
    ADD_TEST(OS_TaskRegister);
    ADD_TEST(OS_TaskGetId);
    ADD_TEST(OS_TaskGetIdByName);
    ADD_TEST(OS_TaskGetInfo);
    ADD_TEST(OS_TaskInstallDeleteHandler);
}





