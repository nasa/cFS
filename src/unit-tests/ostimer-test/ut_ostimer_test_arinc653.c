/*================================================================================*
** File:  ut_ostimer_test_arinc653.c
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include <apex_api.h>

#include "ut_ostimer_test.h"

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

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

int32  g_skipTestCase = -1;
char*  g_skipTestCaseResult = " ";

char*  g_timerNames[UT_OS_TIMER_LIST_LEN];
char   g_longTimerName[OS_MAX_API_NAME+5];

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_timercallback(uint32 timerId);

void UT_os_init_timer_misc(void);
void UT_os_setup_timercreate_test(void);
void UT_os_setup_timerdelete_test(void);
void UT_os_setup_timergetidbyname_test(void);
void UT_os_setup_timergetinfo_test(void);
void UT_os_setup_timerset_test(void);

void UT_os_timer_tests(void);

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

void UT_os_timercallback(uint32 timerId)
{
    int deltaTime = 0;
    char text[UT_OS_LG_TEXT_LEN];
    RETURN_CODE_TYPE arincRes = 0;
    static int32 loopCnt = 0, res = 0;
    static uint32 prevIntervalTime = 0;
    static uint32 currIntervalTime = 0;
    static OS_time_t beginTime = {0,0}, currTime = {0,0};

    OS_GetLocalTime(&currTime);

    if (currTime.seconds == beginTime.seconds)
        currIntervalTime = currTime.microsecs - beginTime.microsecs;
    else
        currIntervalTime = currTime.microsecs + (1000000 - beginTime.microsecs);

    if (currIntervalTime >= prevIntervalTime)
        deltaTime = currIntervalTime - prevIntervalTime;
    else
        deltaTime = prevIntervalTime - currIntervalTime;

    if (g_logInfo.verboseLevel == UT_OS_LOG_EVERYTHING)
    {
        memset(text, '\0', sizeof(text));
        UT_os_sprintf(text, "OS_TimerSet() - #4 Nominal [Output from timer callback func: ");
        UT_OS_LOG_MACRO(text)
        memset(text, '\0', sizeof(text));
        UT_os_sprintf(text, "current_time=%d.%d, delta_time=%d, loopCnt=%d]\n",
                      (int)currTime.seconds, (int)currTime.microsecs,
                      (int)deltaTime, (int)loopCnt);
        UT_OS_LOG_MACRO(text)
    }

    loopCnt++;
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_timer_misc()
{
    memset(g_longTimerName, 'Y', sizeof(g_longTimerName));
    g_longTimerName[sizeof(g_longTimerName)-1] = '\0';
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timercreate_test()
{
    g_timerNames[0] = "Timer #1";  /* Create_NotImpl */
    g_timerNames[1] = "Timer #1";  /* Create_NullPtr */
    g_timerNames[2] = "Timer #1";  /* Create_LongName */
    g_timerNames[3] = "Timer #1";  /* Create_DuplName */
    g_timerNames[4] = "Timer #1";  /* Create_Full */
    g_timerNames[5] = "Timer #1";  /* Create_InvArg */
    g_timerNames[6] = "Timer #1";  /* Create_UnAvail */
    g_timerNames[7] = "Timer #1";  /* Create_Nominal */

    g_skipTestCase = 4;
    g_skipTestCaseResult = UT_OS_NA;
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timerdelete_test()
{
    g_timerNames[0] = "Timer #1";  /* Delete_NotImpl */
    g_timerNames[1] = "Timer #1";  /* Delete_InvArg */
    g_timerNames[2] = "Timer #1";  /* Delete_IntErr */
    g_timerNames[3] = "Timer #1";  /* Delete_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timergetidbyname_test()
{
    g_timerNames[0] = "Timer #0";  /* GetId_NotImpl */
    g_timerNames[1] = "Timer #0";  /* GetId_NullPtr */
    g_timerNames[2] = "Timer #0";  /* GetId_LongName */
    g_timerNames[3] = "Timer #0";  /* GetId_NotFound */
    g_timerNames[4] = "Timer #0";  /* GetId_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timergetinfo_test()
{
    g_timerNames[0] = "Timer #1";  /* GetInfo_NotImpl */
    g_timerNames[1] = "Timer #1";  /* GetInfo_NullPtr */
    g_timerNames[2] = "Timer #1";  /* GetInfo_InvArg */
    g_timerNames[3] = "Timer #1";  /* GetInfo_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timerset_test()
{
    g_timerNames[0] = "Timer #0";  /* Set_NotImpl */
    g_timerNames[1] = "Timer #0";  /* Set_InvArg */
    g_timerNames[2] = "Timer #0";  /* Set_IntErr */
    g_timerNames[3] = "Timer #0";  /* Set_ShortIntv */
    g_timerNames[4] = "Timer #0";  /* Set_Nominal */

    g_skipTestCase = 3;
    g_skipTestCaseResult = UT_OS_NA;
}

/*--------------------------------------------------------------------------------*/

void UT_os_timer_tests()
{
    UT_os_init_timer_misc();

    UT_os_setup_timercreate_test();
    UT_os_timercreate_test();

    UT_os_setup_timerdelete_test();
    UT_os_timerdelete_test();

    UT_os_setup_timergetidbyname_test();
    UT_os_timergetidbyname_test();

    UT_os_setup_timergetinfo_test();
    UT_os_timergetinfo_test();

    UT_os_setup_timerset_test();
    UT_OS_LOG_MACRO("\n============================================\n")
    UT_os_timerset_test();
    UT_OS_LOG_MACRO("============================================\n")

    UT_os_teardown("ut_ostimer");
}

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/

void UT_main()
{
    int32 osalRet=0;
    uint32 tId1=0, tId2=0, stackPtr=0;
    RETURN_CODE_TYPE arincRet=NO_ERROR;

    UT_os_setup(UT_OS_LOG_FILENAME);

    /* UT_OS_LOG_OFF, UT_OS_LOG_MINIMAL, UT_OS_LOG_MODERATE, UT_OS_LOG_EVERYTHING */
    UT_os_set_log_verbose(UT_OS_LOG_EVERYTHING);

    UT_OS_LOG_MACRO("OSAL Unit Test Output File for ARINC653-implementation of ostimer APIs\n");

    UT_os_timerinit_test();

    osalRet = OS_TaskCreate(&tId1, "ut_ostimer",
                            (osal_task_entry)&UT_os_timer_tests,
                            &stackPtr, 0x2000, 110, 0);

    SET_PARTITION_MODE(NORMAL, &arincRet);
}

/*================================================================================*
** End of File: ut_ostimer_test_arinc653.c
**================================================================================*/
