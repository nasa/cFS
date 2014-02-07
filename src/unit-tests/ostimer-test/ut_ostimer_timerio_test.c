/*================================================================================*
** File:  ut_ostimer_timerio_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_ostimer_timerio_test.h"
#include "ut_ostimer_test_platforms.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t  g_logInfo;

extern char*  g_timerNames[UT_OS_TIMER_LIST_LEN];
extern char   g_longTimerName[OS_MAX_API_NAME+5];

extern int32  g_skipTestCase;
extern char*  g_skipTestCaseResult;

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

extern void UT_os_timercallback(uint32 timerId);

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

uint32  g_clkAccuracy = 0;
uint32  g_timerIds[UT_OS_TIMER_LIST_LEN];

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/* Test code template for testing a single OSAL API with multiple test cases */

#if 0
void UT_os_sample_test()
{
    /* Must declare these variables for each function. They can be renamed.
     * They're referenced in the macros used to track test cases and their results. */
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;

    /* Call this once at the beginning of the function to initialize the test variables. */
    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*
     * For each test case,
     *   1. Assign testDesc a brief description of the test
     *   2. Setup the test environment, if necessary
     *   3. Run the test
     *   4. Log result by calling UT_OS_SET_TEST_RESULT_MACRO
     *   4. Reset the test environment, if neccessary
     *
     * NOTE: "Not implemented" is always checked first but not
     *       being included as a test case.
     *       "Nominal" test case is always the last test case.
     *-----------------------------------------------------*/

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    /* TODO: Setup the test environment, if necessary */

    if (OS_xxx() == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_sample_test_exit_tag;
    }

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(NULL,...) == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(aVeryLoooooongName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(...) != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)

    /* TODO: Reset the test environment here, if necessary */

UT_os_sample_test_exit_tag:
    /* Call these macros at the very end of the function to close out the test variables
     * and get it added to the global list being tracked. */
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_xxx", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}
#endif

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerAPIInit(void)
** Purpose: Initializes the tables that the OS timer uses to keep track of information
**          about objects
** Parameters: None
** Returns: OS_ERROR on an unsuccessful inits
**          OS_SUCCESS on a successful inits
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Init-not-call-first condition
**   1) Don't call this routine first
**   2) Call TBD routine(s)
**   3) Expect the returned value from those routines to be
**       (a) __not__ OS_SUCCESS
*** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**       (a) OS_SUCCESS (although results are not directly observable)
**   3) Call TBD routine(s)
**   4) Expect the returned value from those routines to be
**       (a) OS_SUCCESS
*--------------------------------------------------------------------------------*/
void UT_os_timerinit_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "#1 Init-not-call-first";

    res = OS_TimerCreate(&g_timerIds[0], "Timer #0", &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    }
    else
    {
        /* Reset test environment */
        OS_TimerDelete(g_timerIds[0]);

        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_API_Init();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        testDesc = "API not implemented";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
    }
    else if (res == OS_SUCCESS)
    {
        res = OS_TimerCreate(&g_timerIds[0], "Timer #0", &g_clkAccuracy, &UT_os_timercallback);
        if (res == OS_SUCCESS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Reset test environment */
        OS_TimerDelete(g_timerIds[0]);
    }
    else
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TimerAPIInit", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t callback_ptr)
** Purpose: Creates a new timer and associates it with a callback routine
** Parameters: *timer_id - a pointer that will hold the timer id
**             *timer_name - a pointer that holds the name of the timer to be created
**             *clock_accuracy - a pointer that will hold the clock accuracy in microseconds
**             callback_ptr - a function pointer to the timer's callback
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if all of the timers has already been allocated
**          OS_TIMER_ERR_INVALID_ARGS if the callback pointer is null
**          OS_TIMER_ERR_UNAVAILABLE if the timer cannot be created
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_TimerCreate() is implemented, then
**            OS_TimerDelete() and OS_TimerGetIdByName() should be implemented
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null pointer as one of the arguments (exclude callback ptr)
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Name-too-long-argument condition
**   1) Call this routine with a timer name of length greater than OS_MAX_API_NAME as argument
**   2) Expect the returned value to be
**        (a) OS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #3: Name-taken-argument condition
**   1) Call this routine with a valid timer name as argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine the second time with the timer name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_ERR_NAME_TAKEN
** -----------------------------------------------------
** Test #4: No-free-ids condition
**   1) Call this routine N number of times, where N = OS_MAX_TIMERS+1
**   2) Expect the returned value of the last call to be
**        (a) OS_ERR_NO_FREE_IDS
** -----------------------------------------------------
** Test #5: Invalid-argument condition
**   1) Call this routine with a null pointer for callback as argument
**   2) Expect the returned value to be
**        (a) OS_TIMER_ERR_INVALID_ARGS
** -----------------------------------------------------
** Test #6: Timer-unavailable condition
**   1) Set up test to cause the OS call inside this routine to fail
**   2) Call this routine with valid arguments
**   3) Expect the returned value to be
**        (a) OS_TIMER_ERR_UNAVAILABLE
** -----------------------------------------------------
** Test #7: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_TimerGetIdByName with the timer name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS __and__
**        (b) the returned timer id is the same as the timer id returned in #1
**   5) Call OS_TimerDelete with the timer id returned in #1 as argument
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_timercreate_test()
{
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    int32 res=0, idx=0, i=0, j=0;
    char tmpStr[UT_OS_XS_TEXT_LEN];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerCreate(&g_timerIds[0], g_timerNames[0], &g_clkAccuracy, &UT_os_timercallback);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_timercreate_test_exit_tag;
    }

    /* Reset test environment */
    OS_TimerDelete(g_timerIds[0]);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_TimerCreate(NULL, g_timerNames[1], &g_clkAccuracy, &UT_os_timercallback) ==
         OS_INVALID_POINTER) &&
        (OS_TimerCreate(&g_timerIds[1], NULL, &g_clkAccuracy, &UT_os_timercallback) ==
         OS_INVALID_POINTER) &&
        (OS_TimerCreate(&g_timerIds[1], g_timerNames[1], NULL, &UT_os_timercallback) ==
         OS_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long";

    if (OS_TimerCreate(&g_timerIds[2], g_longTimerName, &g_clkAccuracy, &UT_os_timercallback) ==
        OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-taken";

    if ((OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback) ==
         OS_SUCCESS) &&
        (OS_TimerCreate(&g_timerIds[4], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback) ==
         OS_ERR_NAME_TAKEN))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_TimerDelete(g_timerIds[3]);

    /*-----------------------------------------------------*/
    testDesc = "#4 No-free-IDs";

    if (g_skipTestCase == 4)
    {
        testDesc = "#4 No-free-IDs - Test case not applicable on platform";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, g_skipTestCaseResult)
    }
    else
    {
        for (i=0; i <= OS_MAX_TIMERS; i++)
        {
            memset(tmpStr, '\0', sizeof(tmpStr));
            UT_os_sprintf(tmpStr, "Timer #%d", (int)i);
            res = OS_TimerCreate(&g_timerIds[i], tmpStr, &g_clkAccuracy, &UT_os_timercallback);
            if (res != OS_SUCCESS)
                break;
        }

        if (i < OS_MAX_TIMERS)
        {
            testDesc = "#4 No-free-IDs - Timer-created failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else if (i == OS_MAX_TIMERS)
        {
            if (res == OS_ERR_NO_FREE_IDS)
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }

        /* Reset test environment */
        for (j=0; j < i; j++)
            OS_TimerDelete(g_timerIds[j]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Invalid-arg";

    if (OS_TimerCreate(&g_timerIds[5], g_timerNames[5], &g_clkAccuracy, NULL) ==
        OS_TIMER_ERR_INVALID_ARGS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#6 Timer-unavailable";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#7 Nominal";

    res = OS_TimerCreate(&g_timerIds[7], g_timerNames[7], &g_clkAccuracy, &UT_os_timercallback);
    if (res == OS_SUCCESS)
    {
        if ((OS_TimerGetIdByName(&g_timerIds[8], g_timerNames[7]) == OS_SUCCESS) &&
            (g_timerIds[7] == g_timerIds[8]) &&
            (OS_TimerDelete(g_timerIds[7]) == OS_SUCCESS))
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }
    else
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_timercreate_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TimerCreate", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerDelete(uint32 timer_id)
** Purpose: Deletes a timer of a given timer id
** Parameters: timer_id - the id of the timer to be deleted
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid timer id
**          OS_TIMER_ERR_INTERNAL if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerDelete() is implemented, then
**            OS_TimerCreate() and OS_TimerGetIdByName() should be implemented.
** -----------------------------------------------------
** Test #1: Invalid-id-argument condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with some value for timer id as argument
**   3) Expect the returned value to be
**       (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #2: Internal-error condition
**   1) Set up test to cause the OS call inside this routine to fail
**   2) Call this routine with valid arguments
**   3) Expect the returned value to be
**        (a) OS_TIMER_ERR_INTERNAL
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the timer id returned in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_TimerGetIdByName with the timer name used in #1 as argument
**   6) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
**   7) Call OS_TimerCreate exactly as in #1
**   8) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_timerdelete_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerDelete(99999);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_timerdelete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-id-arg";

    if (OS_TimerDelete(99999) == OS_ERR_INVALID_ID)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Internal-error";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Timer-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_timerdelete_test_exit_tag;
    }

    if (OS_TimerDelete(g_timerIds[3]) == OS_SUCCESS)
    {
        if ((OS_TimerGetIdByName(&g_timerIds[4], g_timerNames[3]) == OS_ERR_NAME_NOT_FOUND) &&
            (OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback) ==
             OS_SUCCESS))
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }
    else
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /* Reset test environment */
    OS_TimerDelete(g_timerIds[3]);

UT_os_timerdelete_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TimerDelete", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerSet(uint32 timer_id, uint32 start_msec, uint32 interval_msec)
** Purpose: Programs a given timer with a start time of microseconds and an
**          interval time of microseconds
** Parameters: timer_id - id of the timer to be programmed
**             start_msec - start time in microseconds of when to first call the
**                          timer's callback function
**             interval_msec - interval in microseconds of what the periodic timer
**                             will be programmed for
** Returns: OS_ERR_INVALID_ID if the timer id passed in is not a valid timer id
**          OS_TIMER_ERR_INTERNAL if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerSet() is implemented, then
**            OS_TimerCreate(), OS_TimerDelete() and OS_TimerGetIdByName() should be implemented.
** -----------------------------------------------------
** Test #1: Invalid-id condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with some value for timer id as argument
**   3) Expect the returned value to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #2: Internal-error condition
**   1) Setup test to cause the OS call inside this routine to fail
**   2) Call this routine with valid arguments
**   3) Expect the returned value to be
**        (a) OS_TIMER_ERR_INTERNAL
** -----------------------------------------------------
** Test #3: Interval-too-short condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with a start time of 1,000 and an interval time of 5 microsecs
**   4) When the timer callback registered in #1 gets call, determine if the duration
**      is the set interval time
**   5) Exit test when the timer callback registered in #1 gets call 10 times
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with a start time of 1,000 and an interval time of 500,000 microsecs
**   4) When the timer callback registered in #1 gets call, determine if the duration
**      is within +/- 5% of the set interval time
**   5) Exit test when the timer callback registered in #1 gets call 10 times
**--------------------------------------------------------------------------------*/
void UT_os_timerset_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    char text[UT_OS_LG_TEXT_LEN];
    uint32 startTime=0, intervalTime=0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerSet(99999, startTime, intervalTime);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_timerset_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-id-arg";

    res = OS_TimerSet(99999, startTime, intervalTime);
    if (res == OS_ERR_INVALID_ID)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Internal-error";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Interval-too-short";

    if (g_skipTestCase == 3)
    {
        testDesc = "#3 Interval-too-short - Test case not applicable on platform";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, g_skipTestCaseResult)
    }
    else
    {
        res = OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback);
        if (res != OS_SUCCESS)
        {
            testDesc = "#3 Interval-too-short - Timer-create failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            UT_OS_TIMER_SET_SHORT_INTERVAL_ELSE_MACRO
        }
    }

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    res = OS_TimerCreate(&g_timerIds[4], g_timerNames[4], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - Timer-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_timerset_test_exit_tag;
    }
    else
    {
        UT_OS_TIMER_SET_NOMINAL_ELSE_MACRO
    }

UT_os_timerset_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TimerSet", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerGetIdByName(uint32 *timer_id, const char *timer_name)
** Purpose: Returns the timer id of a given timer name
** Parameters: *timer_id - a pointer that will hold the timer id
**             *timer_name - a pointer that holds the timer name
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in is not found in the timer table
**          OS_SUCCESS if succeeded
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerGetIdByName() is implemented, then
**            OS_TimerCreate() and OS_TimerDelete() should be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Name-too-long condition
**   1) Call this routine with a timer name of length greater than OS_MAX_API_NAME as argument
**   2) Expect the returned value to be
**        (a) OS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #3: Name-not-found condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with a valid timer name
**   3) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expected the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the timer name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS __and__
**        (b) the returned timer id is the same as the timer id returned in #1
**--------------------------------------------------------------------------------*/
void UT_os_timergetidbyname_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerGetIdByName(&g_timerIds[0], g_timerNames[0]);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_timergetidbyname_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_TimerGetIdByName(NULL, g_timerNames[1]) == OS_INVALID_POINTER) &&
        (OS_TimerGetIdByName(&g_timerIds[1], NULL) == OS_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long";

    if (OS_TimerGetIdByName(&g_timerIds[2], g_longTimerName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-not-found";

    if (OS_TimerGetIdByName(&g_timerIds[3], g_timerNames[3]) == OS_ERR_NAME_NOT_FOUND)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    res = OS_TimerCreate(&g_timerIds[4], g_timerNames[4], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - Timer-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_timergetidbyname_test_exit_tag;
    }

    res = OS_TimerGetIdByName(&g_timerIds[5], g_timerNames[4]);
    if ((res == OS_SUCCESS) && (g_timerIds[4] == g_timerIds[5]))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_TimerDelete(g_timerIds[4]);

UT_os_timergetidbyname_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TimerGetIdByName", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerGetInfo(uint32 timer_id, OS_timer_prop_t *timer_prop)
** Purpose: Returns timer information about a given timer
** Parameters: timer_id - the timer id to look up
**             *timer_prop - a pointer to an OS_timer_prop_t structure that will
**                           hold a timer's information
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid timer id
**          OS_SUCCESS if succeeded
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerGetInfo() is implemented, then
**            OS_TimerCreate() and OS_TimerDelete() should be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-id-argument condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with some value for timer id as argument
**   3) Expect the returned value to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the timer id returned in #1
**   4) Expect the returned value to be
**       (a) OS_SUCCESS __and__
**       (b) timer name returned for timer properties is the same as timer name used in #1
**--------------------------------------------------------------------------------*/
void UT_os_timergetinfo_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    OS_timer_prop_t timerProps;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerGetInfo(99999, &timerProps);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_timergetinfo_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_TimerCreate(&g_timerIds[1], g_timerNames[1], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#1 Null-pointer-arg - Timer-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        if (OS_TimerGetInfo(g_timerIds[1], NULL) == OS_INVALID_POINTER)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Reset test environment */
        OS_TimerDelete(g_timerIds[1]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-id-arg";

    if (OS_TimerGetInfo(99999, &timerProps) == OS_ERR_INVALID_ID)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)


    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Timer-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        memset(&timerProps, 0x00, sizeof(timerProps));
        res = OS_TimerGetInfo(g_timerIds[3], &timerProps);
        if (res == OS_SUCCESS)
        {
            if (strcmp(timerProps.name, g_timerNames[3]) == 0)
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }
        else
        {
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }

        /* Reset test environment */
        OS_TimerDelete(g_timerIds[3]);
    }

UT_os_timergetinfo_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TimerGetInfo", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_ostimer_timerio_test.c
**================================================================================*/
