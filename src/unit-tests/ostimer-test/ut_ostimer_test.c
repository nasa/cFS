/*================================================================================*
** File:  ut_ostimer_test.c
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

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

uint32  g_cbLoopCntMax = 5;
uint32  g_toleranceVal = 0;
uint32  g_timerFirst   = 0;
int32   g_status  = 0;
uint32  g_timerId = 0;

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

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

void UT_os_timercallback(uint32 timerId)
{
    int deltaTime = 0;
    char text[UT_OS_LG_TEXT_LEN];
    static int32 loopCnt = 0, res = 0;
    static uint32 prevIntervalTime = 0;
    static uint32 currIntervalTime = 0;
    static OS_time_t currTime = {0,0}, endTime = {0,0};

    if (timerId == g_timerId)
    {
        if (g_timerFirst)
        {
            g_timerFirst = 0;
            g_status = 0;
            prevIntervalTime = 0;
            res = 0;
            loopCnt = 0;
            OS_GetLocalTime(&currTime);
        }

        OS_GetLocalTime(&endTime);

        if (endTime.seconds == currTime.seconds)
            currIntervalTime = endTime.microsecs - currTime.microsecs;
        else
            currIntervalTime = endTime.microsecs + (1000000 - currTime.microsecs);

        if (currIntervalTime >= prevIntervalTime)
            deltaTime = currIntervalTime - prevIntervalTime;
        else
            deltaTime = prevIntervalTime - currIntervalTime;

        if (g_logInfo.verboseLevel == UT_OS_LOG_EVERYTHING)
        {
        	UT_OS_LOG_MACRO("\n");
            memset(text, '\0', sizeof(text));
            UT_os_sprintf(text, "OS_TimerSet() - #4 Nominal [Output from timer callback func: ");
            UT_OS_LOG_MACRO(text)
            memset(text, '\0', sizeof(text));
            UT_os_sprintf(text, "current_time=%d.%d, delta_time=%d, tolVal=%d, loopCnt=%d]\n",
                          (int)currTime.seconds, (int)currTime.microsecs,
                          (int)deltaTime, (int)g_toleranceVal, (int)loopCnt);
            UT_OS_LOG_MACRO(text)
        }

        if ((deltaTime > g_toleranceVal) && (loopCnt > 1))
            res = -1;

        loopCnt++;

        if (loopCnt < g_cbLoopCntMax)
        {
            currTime = endTime;
            prevIntervalTime = currIntervalTime;
        }
        else
        {
            g_status = (res == 0) ? 1 : -1;
            /* slow the timer down so the main test thread can continue */
            res = OS_TimerSet(g_timerId, 1000, 500000);
        }
    }
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
    memset(g_longTimerName, 'Y', sizeof(g_longTimerName));
    g_longTimerName[sizeof(g_longTimerName)-1] = '\0';

    g_timerNames[0] = "Create_NotImpl";
    g_timerNames[1] = "Create_NullPtr";
    g_timerNames[2] = "Create_LongName";
    g_timerNames[3] = "Create_DuplName";
    g_timerNames[4] = "Create_Full";
    g_timerNames[5] = "Create_InvArg";
    g_timerNames[6] = "Create_UnAvail";
    g_timerNames[7] = "Create_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timerdelete_test()
{
    g_timerNames[0] = "Delete_NotImpl";
    g_timerNames[1] = "Delete_InvArg";
    g_timerNames[2] = "Delete_IntErr";
    g_timerNames[3] = "Delete_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timergetidbyname_test()
{
    g_timerNames[0] = "GetId_NotImpl";
    g_timerNames[1] = "GetId_NullPtr";
    g_timerNames[2] = "GetId_LongName";
    g_timerNames[3] = "GetId_NotFound";
    g_timerNames[4] = "GetId_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timergetinfo_test()
{
    g_timerNames[0] = "GetInfo_NotImpl";
    g_timerNames[1] = "GetInfo_NullPtr";
    g_timerNames[2] = "GetInfo_InvArg";
    g_timerNames[3] = "GetInfo_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_timerset_test()
{
    g_timerNames[0] = "Set_NotImpl";
    g_timerNames[1] = "Set_InvArg";
    g_timerNames[2] = "Set_IntErr";
    g_timerNames[3] = "Set_ShortIntv";
    g_timerNames[4] = "Set_Nominal";
}

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/
#ifdef _OSAL_UNIT_TEST_
   void OS_Application_Startup(void)
#else
   int main(int argc, char* argv[])
#endif
{
    UT_os_setup(UT_OS_LOG_FILENAME);

    /* UT_OS_LOG_OFF, UT_OS_LOG_MINIMAL, UT_OS_LOG_MODERATE, UT_OS_LOG_EVERYTHING */
    UT_os_set_log_verbose(UT_OS_LOG_EVERYTHING);

    UT_OS_LOG_MACRO("OSAL Unit Test Output File for ostimer APIs\n");

    UT_os_timerinit_test();

    OS_API_Init();

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

    return (0);
}

/*================================================================================*
** End of File: ut_ostimer_test.c
**================================================================================*/
