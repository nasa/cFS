/*================================================================================*
** File:  ut_ostimer_test_platforms.h
** Owner: Tam Ngo
** Date:  September 2013
**================================================================================*/

#ifndef _UT_OSTIMER_TEST_PLATFORMS_H_
#define _UT_OSTIMER_TEST_PLATFORMS_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#ifdef _LINUX_OS_
#endif  /* _LINUX_OS_ */

#ifdef OSP_ARINC653
#include "apex_api.h"
#endif  /* OSP_ARINC653 */

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------*/
#ifdef _LINUX_OS_
/*--------------------------------------------*/

#define UT_OS_TIMER_SET_SHORT_INTERVAL_ELSE_MACRO                                      \
{                                                                                      \
    g_status  = 0;                                                                     \
    g_timerId = g_timerIds[3];                                                         \
    g_timerFirst   = 1;                                                                \
    g_cbLoopCntMax = 10;                                                               \
    startTime      = 1000;                                                             \
    intervalTime   = 5;                                                                \
    g_toleranceVal = 0;                                                                \
    memset(text, '\0', sizeof(text));                                                  \
    UT_os_sprintf(text, "\nOS_TimerSet() - #3 Interval-too-short (clk_accuracy=%d)\n", \
                        (int)g_clkAccuracy);                                           \
    UT_OS_LOG_MACRO(text)                                                              \
    res = OS_TimerSet(g_timerIds[3], startTime, intervalTime);                         \
    if (res == OS_SUCCESS)                                                             \
    {                                                                                  \
        while (!g_status)                                                              \
            OS_TaskDelay(1);                                                           \
        if (g_status < 0)                                                              \
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)          \
        else                                                                           \
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)          \
    }                                                                                  \
    else                                                                               \
    {                                                                                  \
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)              \
    }                                                                                  \
    /* Reset test environment */                                                       \
    OS_TimerDelete(g_timerIds[3]);                                                     \
}

/*--------------------------------------------------------------------------------*/

#define UT_OS_TIMER_SET_NOMINAL_ELSE_MACRO                                            \
{                                                                                     \
    g_status  = 0;                                                                    \
    g_timerId = g_timerIds[4];                                                        \
    g_timerFirst   = 1;                                                               \
    g_cbLoopCntMax = 10;                                                              \
    startTime      = 1000;                                                            \
    intervalTime   = 500000;                                                          \
    g_toleranceVal = intervalTime / 20;  /* 5% */                                     \
    memset(text, '\0', sizeof(text));                                                 \
    UT_os_sprintf(text, "\nOS_TimerSet() - #1 Nominal condition (clk_accuracy=%d)\n", \
                        (int)g_clkAccuracy);                                          \
    UT_OS_LOG_MACRO(text)                                                             \
    res = OS_TimerSet(g_timerIds[4], startTime, intervalTime);                        \
    if (res == OS_SUCCESS)                                                            \
    {                                                                                 \
        while (!g_status)                                                             \
            OS_TaskDelay(1);                                                          \
        if (g_status > 0)                                                             \
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)         \
         else                                                                         \
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)         \
    }                                                                                 \
    else                                                                              \
     {                                                                                \
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)             \
    }                                                                                 \
    /* Reset test environment */                                                      \
     OS_TimerDelete(g_timerIds[4]);                                                   \
}

/*--------------------------------------------*/
#endif  /* _LINUX_OS_ */
/*--------------------------------------------*/

/*--------------------------------------------*/
#ifdef OSP_ARINC653
/*--------------------------------------------*/

#define UT_OS_TIMER_SET_SHORT_INTERVAL_ELSE_MACRO \
{                                                 \
    /* Do nothing for now */                      \
}

/*--------------------------------------------------------------------------------*/

#define UT_OS_TIMER_SET_NOMINAL_ELSE_MACRO                                \
{                                                                         \
    RETURN_CODE_TYPE arincRes=0;                                          \
    startTime    = 1000;                                                  \
    intervalTime = 500000;                                                \
    res = OS_TimerSet(g_timerIds[4], startTime, intervalTime);            \
    if (res == OS_SUCCESS)                                                \
    {                                                                     \
    	testDesc = "#4 Nominal - Manual inspection required";             \
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_MIR)    \
    }                                                                     \
    else                                                                  \
    {                                                                     \
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED) \
    }                                                                     \
    /* Reset test environment */                                          \
    PERIODIC_WAIT(&arincRes);                                             \
    OS_TaskDelay(intervalTime * 500);                                     \
    OS_TimerDelete(g_timerIds[4]);                                        \
}

/*--------------------------------------------*/
#endif  /* OSP_ARINC653 */
/*--------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------*/
#ifdef _LINUX_OS_
/*--------------------------------------------*/

extern uint32  g_cbLoopCntMax;
extern uint32  g_toleranceVal;
extern uint32  g_timerFirst;
extern int32   g_status;
extern uint32  g_timerId;

/*--------------------------------------------*/
#endif  /* _LINUX_OS_ */
/*--------------------------------------------*/

/*--------------------------------------------*/
#ifdef OSP_ARINC653
/*--------------------------------------------*/
/*--------------------------------------------*/
#endif  /* OSP_ARINC653 */
/*--------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSTIMER_TEST_PLATFORMS_H_ */

/*================================================================================*
** End of File: ut_ostimer_test_platforms.h
**================================================================================*/
