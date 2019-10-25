/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "ostimer-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-vxworks-coveragetest.h"
#include "ut-ostimer.h"

#include <overrides/taskLib.h>
#include <overrides/signal.h>
#include <overrides/time.h>
#include <overrides/sysLib.h>
#include <overrides/semLib.h>


void Test_OS_VxWorks_TimeBaseAPI_Impl_Init(void)
{
    /* Test Case For:
     * int32 OS_VxWorks_TimeBaseAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_TimeBaseAPI_Impl_Init(), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_sysClkRateGet), -1);
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_TimeBaseAPI_Impl_Init(), OS_ERROR);
}


void Test_OS_TimeBaseLock_Impl(void)
{
    /* Test Case For:
     * void OS_TimeBaseLock_Impl(uint32 local_id)
     */
    OS_TimeBaseLock_Impl(0);
}

void Test_OS_TimeBaseUnlock_Impl(void)
{
    /* Test Case For:
     * void OS_TimeBaseUnlock_Impl(uint32 local_id)
     */
    OS_TimeBaseUnlock_Impl(0);
}

static int32 Osapi_Internal_TimeBaseRegHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    Osapi_Internal_SetTimeBaseRegState(0, true);
    return 0;
}

void Test_OS_Impl_UsecToTimespec(void)
{
    /* Test Case For:
     * static void OS_Impl_UsecToTimespec(uint32 usecs, struct timespec *time_spec)
     *
     * This is invoked through a wrapper, to get coverage on the function.
     *
     * Note: For some reason, the vxworks implementation does this differently for intervals
     * less than one second vs. one second or more, even though it does not need this
     * complexity/special case, because the "one-or-more" code works for all.  Nonetheless,
     * this needs to exist to test both ways.
     */

    struct OCS_timespec ts;

    memset(&ts, 255, sizeof(ts));
    Osapi_Internal_UsecToTimespec(1, &ts);
    UtAssert_True(ts.tv_sec == 0, "ts.tv_sec (%ld) == 0", (long)ts.tv_sec);
    UtAssert_True(ts.tv_nsec == 1000, "ts.tv_nsec (%ld) == 1000", (long)ts.tv_sec);


    memset(&ts, 255, sizeof(ts));
    Osapi_Internal_UsecToTimespec(1000000000, &ts);
    UtAssert_True(ts.tv_sec == 1000, "ts.tv_sec (%ld) == 1000", (long)ts.tv_sec);
    UtAssert_True(ts.tv_nsec == 0, "ts.tv_nsec (%ld) == 0", (long)ts.tv_sec);
}

void Test_OS_TimeBaseCreate_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseCreate_Impl(uint32 timer_id)
     */

    /*
     * Test paths though the signal number assignment.
     *
     * This should be done first as it will assign the "external_sync"
     * and therefore cause future calls to skip this block.
     */
    OS_global_timebase_table[1].active_id = 0x1;
    Osapi_Internal_Setup(1,OCS_SIGRTMIN, false);
    UT_SetForceFail(UT_KEY(OCS_sigismember), true);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_TIMER_ERR_UNAVAILABLE);
    UT_ResetState(UT_KEY(OCS_sigismember));

    /* fail to initialize the sem */
    UT_SetForceFail(UT_KEY(OCS_semMInitialize), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_TIMER_ERR_INTERNAL);
    UT_ClearForceFail(UT_KEY(OCS_semMInitialize));

    /* fail to spawn the task */
    UT_SetForceFail(UT_KEY(OCS_taskSpawn), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_TIMER_ERR_INTERNAL);
    UT_ClearForceFail(UT_KEY(OCS_taskSpawn));

    /*
     * this call to TimeBaseCreate_Impl should also fail, because
     * this mimics the situation where the reg global is never
     * set past OS_TimerRegState_INIT
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_TIMER_ERR_INTERNAL);

    /*
     * Do Nominal/success case now.
     * Using the hook function, this sets the global state to
     * mimic registration success
     */
    UT_SetHookFunction(UT_KEY(OCS_taskSpawn), Osapi_Internal_TimeBaseRegHook, NULL);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseCreate_Impl(0), OS_SUCCESS);

    /*
     * For coverage, call the  OS_VxWorks_TimeBaseTask() function.
     */
    Osapi_Internal_CallHelperTaskFunc(0);

    /*
     * Check outputs of OS_VxWorks_RegisterTimer() function.
     */
    Osapi_Internal_ClearTimeBaseRegState(0);
    Osapi_Internal_CallRegisterTimer(0);
    UtAssert_True(Osapi_Internal_CheckTimeBaseRegisteredState(0), "timer successfully registered");

    Osapi_Internal_ClearTimeBaseRegState(0);
    UT_SetForceFail(UT_KEY(OCS_timer_create), -1);
    Osapi_Internal_CallRegisterTimer(0);
    UtAssert_True(Osapi_Internal_CheckTimeBaseErrorState(0), "timer registration failure state");
}

void Test_OS_VxWorks_SigWait(void)
{
    /* Test Case For:
     * static uint32 OS_VxWorks_SigWait(uint32 local_id)
     * (invocation of static function through a wrapper)
     */
    int signo = OCS_SIGRTMIN;

    OS_global_timebase_table[0].active_id = 0x12345;
    OS_timebase_table[0].nominal_start_time = 8888;
    OS_timebase_table[0].nominal_interval_time = 5555;

    Osapi_Internal_Setup(0, signo, true);
    UT_SetDataBuffer(UT_KEY(OCS_sigwait),&signo,sizeof(signo),false);
    OSAPI_TEST_FUNCTION_RC(Osapi_Internal_CallSigWaitFunc(0), 8888);
    UT_SetDataBuffer(UT_KEY(OCS_sigwait),&signo,sizeof(signo),false);
    OSAPI_TEST_FUNCTION_RC(Osapi_Internal_CallSigWaitFunc(0), 5555);
    UT_SetDataBuffer(UT_KEY(OCS_sigwait),&signo,sizeof(signo),false);
    OSAPI_TEST_FUNCTION_RC(Osapi_Internal_CallSigWaitFunc(0), 5555);

    Osapi_Internal_Setup(0, 0, false);
    OS_global_timebase_table[0].active_id = 0;
    OS_timebase_table[0].nominal_interval_time = 0;
}

void Test_OS_TimeBaseSet_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseSet_Impl(uint32 timer_id, int32 start_time, int32 interval_time)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(0,1,1), OS_ERR_NOT_IMPLEMENTED);

    Osapi_Internal_Setup(0, OCS_SIGRTMIN, false);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(0,1,1), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_timer_settime), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseSet_Impl(0,1,1), OS_TIMER_ERR_INVALID_ARGS);
}

void Test_OS_TimeBaseDelete_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseDelete_Impl(uint32 timer_id)
     */
    Osapi_Internal_Setup(0, OCS_SIGRTMIN, false);
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseDelete_Impl(0), OS_SUCCESS);
}

void Test_OS_TimeBaseGetInfo_Impl(void)
{
    /* Test Case For:
     * int32 OS_TimeBaseGetInfo_Impl (uint32 timer_id, OS_timebase_prop_t *timer_prop)
     */
    OS_timebase_prop_t timer_prop;
    OSAPI_TEST_FUNCTION_RC(OS_TimeBaseGetInfo_Impl(0,&timer_prop), OS_SUCCESS);
}

/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
    Osapi_Internal_ResetState();
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


/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_VxWorks_TimeBaseAPI_Impl_Init);
    ADD_TEST(OS_TimeBaseLock_Impl);
    ADD_TEST(OS_TimeBaseUnlock_Impl);
    ADD_TEST(OS_TimeBaseCreate_Impl);
    ADD_TEST(OS_VxWorks_SigWait);
    ADD_TEST(OS_TimeBaseSet_Impl);
    ADD_TEST(OS_TimeBaseDelete_Impl);
    ADD_TEST(OS_TimeBaseGetInfo_Impl);
    ADD_TEST(OS_Impl_UsecToTimespec);
}

