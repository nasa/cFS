/*================================================================================*
** File:  ut_ostimer_timerio_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_ostimer_timerio_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern char*  g_timerNames[UT_OS_TIMER_LIST_LEN];
extern char   g_longTimerName[UT_OS_NAME_BUFF_SIZE];

extern uint32  g_cbLoopCntMax;
extern uint32  g_toleranceVal;
extern uint32  g_timerFirst;
extern int32   g_status;
extern uint32  g_timerId;

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
    int32 res=0;
    const char* testDesc;

    /*-----------------------------------------------------*/
    testDesc = "#1 Init-not-call-first";

    res = OS_TimerCreate(&g_timerIds[0], "Timer #0", &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    }
    else
    {
        /* Reset test environment */
        OS_TimerDelete(g_timerIds[0]);

        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_API_Init();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        testDesc = "API not implemented";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
    }
    else if (res == OS_SUCCESS)
    {
        res = OS_TimerCreate(&g_timerIds[0], "Timer #0", &g_clkAccuracy, &UT_os_timercallback);
        if (res == OS_SUCCESS)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        OS_TimerDelete(g_timerIds[0]);
    }
    else
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    return;
    
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
** Test #3: Name equal to OS_MAX_API_NAME characters test
**   1) Call this routine with a timer name equal to OS_MAX_API_NAME + 1 as argument
**   2) Expect the returned value to be
**        (a) OS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-taken-argument condition
**   1) Call this routine with a valid timer name as argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine the second time with the timer name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_ERR_NAME_TAKEN
** -----------------------------------------------------
** Test #5: No-free-ids condition
**   1) Call this routine N number of times, where N = OS_MAX_TIMERS+1
**   2) Expect the returned value of the last call to be
**        (a) OS_ERR_NO_FREE_IDS
** -----------------------------------------------------
** Test #6: Invalid-argument condition
**   1) Call this routine with a null pointer for callback as argument
**   2) Expect the returned value to be
**        (a) OS_TIMER_ERR_INVALID_ARGS
** -----------------------------------------------------
** Test #7: Timer-unavailable condition
**   1) Set up test to cause the OS call inside this routine to fail
**   2) Call this routine with valid arguments
**   3) Expect the returned value to be
**        (a) OS_TIMER_ERR_UNAVAILABLE
** -----------------------------------------------------
** Test #8: Nominal condition
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
    const char* testDesc;
    int32 res=0, i=0, j=0;
    char  tmpStr[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerCreate(&g_timerIds[0], g_timerNames[0], &g_clkAccuracy, &UT_os_timercallback);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
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
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long";

    if (OS_TimerCreate(&g_timerIds[2], g_longTimerName, &g_clkAccuracy, &UT_os_timercallback) ==
        OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name equal to OS_MAX_API_NAME characters test";

    /* Test Load library returning an error on a too long library name */
    memset(&tmpStr[0], 'a', OS_MAX_API_NAME);
    tmpStr[OS_MAX_API_NAME] = '\0';

    if (OS_TimerCreate(&g_timerIds[2], tmpStr, &g_clkAccuracy, &UT_os_timercallback) ==
        OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-taken";

    if ((OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback) ==
         OS_SUCCESS) &&
        (OS_TimerCreate(&g_timerIds[4], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback) ==
         OS_ERR_NAME_TAKEN))
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_TimerDelete(g_timerIds[3]);

    /*-----------------------------------------------------*/
    testDesc = "#5 No-free-IDs";

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
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else if (res == OS_ERR_NO_FREE_IDS)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    }
    else
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    /* Reset test environment */
    for (j=0; j < i; j++)
        OS_TimerDelete(g_timerIds[j]);

    /*-----------------------------------------------------*/
    testDesc = "#6 Invalid-arg";

    if (OS_TimerCreate(&g_timerIds[5], g_timerNames[5], &g_clkAccuracy, NULL) ==
        OS_TIMER_ERR_INVALID_ARGS)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#7 Timer-unavailable";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#8 Nominal";

    res = OS_TimerCreate(&g_timerIds[7], g_timerNames[7], &g_clkAccuracy, &UT_os_timercallback);
    if (res == OS_SUCCESS)
    {
        if ((OS_TimerGetIdByName(&g_timerIds[8], g_timerNames[7]) == OS_SUCCESS) &&
            (g_timerIds[7] == g_timerIds[8]) &&
            (OS_TimerDelete(g_timerIds[7]) == OS_SUCCESS))
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }
    else
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

UT_os_timercreate_test_exit_tag:
    return;
    
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
    int32 res=0;
    const char* testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerDelete(99999);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_timerdelete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-id-arg";

    if (OS_TimerDelete(99999) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Internal-error";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Timer-create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_timerdelete_test_exit_tag;
    }

    if (OS_TimerDelete(g_timerIds[3]) == OS_SUCCESS)
    {
        if ((OS_TimerGetIdByName(&g_timerIds[4], g_timerNames[3]) == OS_ERR_NAME_NOT_FOUND) &&
            (OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback) ==
             OS_SUCCESS))
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }
    else
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    /* Reset test environment */
    OS_TimerDelete(g_timerIds[3]);

UT_os_timerdelete_test_exit_tag:
    return;
    
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
    int32 res=0;
    const char* testDesc;
    uint32 startTime=0, intervalTime=0;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerSet(99999, startTime, intervalTime);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_timerset_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-id-arg";

    res = OS_TimerSet(99999, 10000, 10000);
    if (res == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Internal-error";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Interval-too-short";

    res = OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Interval-too-short - Timer-create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        g_status  = 0;
        g_timerId = g_timerIds[3];
        g_timerFirst   = 1;
        g_cbLoopCntMax = 10;
        startTime      = 1000;
        intervalTime   = 5;
        g_toleranceVal = 0;

        UT_OS_LOG("\nOS_TimerSet() - #3 Interval-too-short (clk_accuracy=%d)\n",
                            (int)g_clkAccuracy);
        res = OS_TimerSet(g_timerIds[3], startTime, intervalTime);
        if (res == OS_SUCCESS)
        {
            while (!g_status)
                OS_TaskDelay(1);
            if (g_status < 0)
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        else
        {
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        /* Reset test environment */
        OS_TimerDelete(g_timerIds[3]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    res = OS_TimerCreate(&g_timerIds[4], g_timerNames[4], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - Timer-create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_timerset_test_exit_tag;
    }
    else
    {
        g_status  = 0;
        g_timerId = g_timerIds[4];
        g_timerFirst   = 1;
        g_cbLoopCntMax = 10;
        startTime      = 1000;
        intervalTime   = 500000;
        g_toleranceVal = intervalTime / 20;  /* 5% */
        UT_OS_LOG("\nOS_TimerSet() - #1 Nominal condition (clk_accuracy=%d)\n",
                            (int)g_clkAccuracy);
        res = OS_TimerSet(g_timerIds[4], startTime, intervalTime);
        if (res == OS_SUCCESS)
        {
            while (!g_status)
                OS_TaskDelay(1);
            if (g_status > 0)
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
             else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        else
        {
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        /* Reset test environment */
         OS_TimerDelete(g_timerIds[4]);
    }

UT_os_timerset_test_exit_tag:
    return;
    
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
    int32 res=0;
    const char* testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerGetIdByName(&g_timerIds[0], g_timerNames[0]);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_timergetidbyname_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_TimerGetIdByName(NULL, g_timerNames[1]) == OS_INVALID_POINTER) &&
        (OS_TimerGetIdByName(&g_timerIds[1], NULL) == OS_INVALID_POINTER))
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long";

    if (OS_TimerGetIdByName(&g_timerIds[2], g_longTimerName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-not-found";

    if (OS_TimerGetIdByName(&g_timerIds[3], g_timerNames[3]) == OS_ERR_NAME_NOT_FOUND)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    res = OS_TimerCreate(&g_timerIds[4], g_timerNames[4], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - Timer-create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_timergetidbyname_test_exit_tag;
    }

    res = OS_TimerGetIdByName(&g_timerIds[5], g_timerNames[4]);
    if ((res == OS_SUCCESS) && (g_timerIds[4] == g_timerIds[5]))
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_TimerDelete(g_timerIds[4]);

UT_os_timergetidbyname_test_exit_tag:
    return;
    
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
    int32 res=0;
    const char* testDesc;
    OS_timer_prop_t timerProps;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TimerGetInfo(99999, &timerProps);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_timergetinfo_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_TimerCreate(&g_timerIds[1], g_timerNames[1], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#1 Null-pointer-arg - Timer-create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        if (OS_TimerGetInfo(g_timerIds[1], NULL) == OS_INVALID_POINTER)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        OS_TimerDelete(g_timerIds[1]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-id-arg";

    if (OS_TimerGetInfo(99999, &timerProps) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);


    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Timer-create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        memset(&timerProps, 0x00, sizeof(timerProps));
        res = OS_TimerGetInfo(g_timerIds[3], &timerProps);
        if (res == OS_SUCCESS)
        {
            if (strcmp(timerProps.name, g_timerNames[3]) == 0)
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        else
        {
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }

        /* Reset test environment */
        OS_TimerDelete(g_timerIds[3]);
    }

UT_os_timergetinfo_test_exit_tag:
    return;
    
}

/*================================================================================*
** End of File: ut_ostimer_timerio_test.c
**================================================================================*/
