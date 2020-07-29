 /*************************************************************************
 ** File:
 **   $Id: sch_app_test.c 1.2 2017/06/21 15:29:21EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains unit test cases for the functions contained in the file sch_app.c
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 *************************************************************************/

/*
 * Includes
 */

#include "sch_app_test.h"
#include "sch_app.h"
#include "sch_apipriv.h"
#include "sch_msg.h"
#include "sch_msgdefs.h"
#include "sch_msgids.h"
#include "sch_events.h"
#include "sch_version.h"
#include "sch_test_utils.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
// FIXME: these need to be implemented/updated in UT-Assert (by copying from sch_test_utils.c/.h):
//#include "ut_cfe_evs_stubs.h"
//#include "ut_cfe_time_stubs.h"
//#include "ut_ostimer_stubs.h"
//#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_psp_watchdog_stubs.h"
#include "ut_cfe_psp_timer_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Function Definitions
 */

int32 SCH_APP_TEST_CFE_TIME_RegisterSynchCallbackHook(CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr)
{
    SCH_AppData.IgnoreMajorFrame      = TRUE;
    SCH_AppData.MinorFramesSinceTone  = 1;

    return CFE_SUCCESS;
}

int32 SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook(CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr)
{
    SCH_AppData.AppID = 99;

    return CFE_SUCCESS;
}

int32 SCH_APP_TEST_OS_TimerCreateHook(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr)
{
    SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY + 1;

    return CFE_SUCCESS;
}

int32 SCH_APP_TEST_CFE_SB_RcvMsgHook(CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId, int32 TimeOut)
{
    return CFE_SB_NO_MESSAGE;
}

int32 SCH_APP_TEST_TimerSetHook(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    SCH_AppData.NextSlotNumber = 10;

    return CFE_SUCCESS;
}

void SCH_AppMain_Test_RegisterAppError(void)
{
    /* Set to fail condition "Status == CFE_SUCCESS" */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, -1, 1);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_APP_EXIT_EID, CFE_EVS_CRITICAL, "SCH App: terminating, err = 0xFFFFFFFF"),
        "SCH App: terminating, err = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("SCH App terminating, err = 0xFFFFFFFF\n"),
        "SCH App terminating, err = 0xFFFFFFFF");

    UtAssert_True (SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 1, "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

} /* end SCH_AppMain_Test_RegisterAppError */

void SCH_AppMain_Test_AppInitError(void)
{
    /* Set to make SCH_AppInit return -1, in order fail condition "Status == CFE_SUCCESS" */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETAPPID_INDEX, -1, 1);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_APP_EXIT_EID, CFE_EVS_CRITICAL, "SCH App: terminating, err = 0xFFFFFFFF"),
        "SCH App: terminating, err = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("SCH App terminating, err = 0xFFFFFFFF\n"),
        "SCH App terminating, err = 0xFFFFFFFF");

    UtAssert_True (SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 2, "Ut_CFE_ES_GetSysLogQueueDepth() == 2");
    /* Generates 1 system log message we don't care about in this test */

} /* end SCH_AppMain_Test_AppInitError */

void SCH_AppMain_Test_CustomLateInitError(void)
{
    /* Set to make SCH_TblInit return CFE_SUCCESS, in order to make SCH_AppInit return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Set to make CustomLateInit return -1, in order to generate error message SCH_MAJOR_FRAME_SUB_ERR_EID */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX, -1, 1);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MAJOR_FRAME_SUB_ERR_EID, CFE_EVS_ERROR, "Error initializing Timers (RC=0xFFFFFFFF)"),
        "Error initializing Timers (RC=0xFFFFFFFF)");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_APP_EXIT_EID, CFE_EVS_CRITICAL, "SCH App: terminating, err = 0xFFFFFFFF"),
        "SCH App: terminating, err = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 3, "Ut_CFE_EVS_GetEventQueueDepth() == 3");
    /* Generates 1 event message we don't care about in this test */

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("SCH App terminating, err = 0xFFFFFFFF\n"),
        "SCH App terminating, err = 0xFFFFFFFF");

    UtAssert_True (SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 1, "Ut_CFE_ES_GetSysLogQueueDepth() == 1");
    /* Generates 1 system log message we don't care about in this test */

} /* end SCH_AppMain_Test_CustomLateInitError */

void SCH_AppMain_Test_NoisyMajorFrameError(void)
{
    SCH_AppData.IgnoreMajorFrameMsgSent = FALSE;

    /* Causes SCH_CustomLateInit to set SCH_AppData.IgnoreMajorFrame = TRUE, which it would normally do if CFE_TIME_RegisterSynchCallback weren't stubbed out.
       Also sets SCH_AppData.MinorFramesSinceTone = 1. Setting these manually in the test setup doesn't work, because they get reset at a certain point */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_RegisterSynchCallbackHook);

    /* Set to make SCH_TblInit return CFE_SUCCESS, in order to make SCH_AppInit return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_NOISY_MAJOR_FRAME_ERR_EID, CFE_EVS_ERROR, "Major Frame Sync too noisy (Slot 1). Disabling synchronization."),
        "Major Frame Sync too noisy (Slot 1). Disabling synchronization.");

    UtAssert_True (SCH_AppData.IgnoreMajorFrameMsgSent == TRUE, "SCH_AppData.IgnoreMajorFrameMsgSent == TRUE");

    UtAssert_True (SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

#if SCH_LIB_PRESENCE == 1
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
    /* Generates 1 event message we don't care about in this test */
#else
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 3, "Ut_CFE_EVS_GetEventQueueDepth() == 3");
    /* Generates 2 event messages we don't care about in this test */
#endif

} /* end SCH_AppMain_Test_NoisyMajorFrameError */

void SCH_AppMain_Test_NominalIgnoreMajorFrameFalse(void)
{
    /* Set to make SCH_TblInit return CFE_SUCCESS, in order to make SCH_AppInit return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* SCH_AppData.IgnoreMajorFrameMsgSent = FALSE by default */

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.IgnoreMajorFrameMsgSent == FALSE, "SCH_AppData.IgnoreMajorFrameMsgSent == FALSE");

    UtAssert_True (SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppMain_Test_NominalIgnoreMajorFrameFalse */

void SCH_AppMain_Test_ProcessScheduleTableError(void)
{
    /* Set to make SCH_TblInit return CFE_SUCCESS, in order to make SCH_AppInit return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* SCH_AppData.IgnoreMajorFrameMsgSent = FALSE by default */

#if SCH_LIB_PRESENCE == 1
    /* Set to pass condition "SCH_GetProcessingState() == TRUE" */
    SCH_LibData.ProcessingDisabledCtr = 0;
#endif

    /* This section is to make SCH_ProcessScheduleTable return -1 */

    /* Set to make SCH_ProcessScheduleTable return not success, in order to satisfy subsequent condition "Status != SCH_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, -1, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_RCVMSG_INDEX);

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = 10;
    SCH_AppData.NextSlotNumber       = SCH_TIME_SYNC_SLOT;
    SCH_AppData.LastProcessCount     = 1;

    /* Sets SCH_AppData.NextSlotNumber = 10, in order to reach SCH_ProcessCommands */
    Ut_OSTIMER_SetFunctionHook(UT_OSTIMER_SET_INDEX, &SCH_APP_TEST_TimerSetHook);

    /* End section */

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 3, "Ut_CFE_EVS_GetEventQueueDepth() == 3");
    /* Generates 3 event messages we don't care about in this test */

} /* end SCH_AppMain_Test_ProcessScheduleTableError */

void SCH_AppMain_Test_Nominal(void)
{
    /* Set to make SCH_TblInit return CFE_SUCCESS, in order to make SCH_AppInit return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* SCH_AppData.IgnoreMajorFrameMsgSent = FALSE by default */

    /* Sets SCH_AppData.AppID to 99, to allow verification that the end of the function under test has been reached */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX, &SCH_APP_TEST_CFE_TIME_UnregisterSynchCallbackHook);

    /* Execute the function being tested */
    SCH_AppMain();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.AppID == 99, "SCH_AppData.AppID == 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppMain_Test_Nominal */

void SCH_AppInit_Test_GetAppIDError(void)
{
    int32   Result;

    SCH_AppData.SlotsProcessedCount = 1;
    SCH_AppData.SkippedSlotsCount   = 2;
    SCH_AppData.MultipleSlotsCount  = 3;
    SCH_AppData.SameSlotCount       = 4;
    SCH_AppData.ScheduleActivitySuccessCount = 5;
    SCH_AppData.ScheduleActivityFailureCount = 6;
    SCH_AppData.CmdCounter = 7;
    SCH_AppData.ErrCounter = 8;

    /* Set to fail subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_GETAPPID_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0, "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   == 0, "SCH_AppData.SkippedSlotsCount   == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  == 0, "SCH_AppData.MultipleSlotsCount  == 0");
    UtAssert_True(SCH_AppData.SameSlotCount       == 0, "SCH_AppData.SameSlotCount       == 0");
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0, "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0, "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True(SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("SCH App: Unable to obtain own AppID, RC=0xFFFFFFFF\n"),
        "SCH App: Unable to obtain own AppID, RC=0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 1, "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

} /* end SCH_AppInit_Test_GetAppIDError */

void SCH_AppInit_Test_EvsInitError(void)
{
    int32   Result;

    SCH_AppData.SlotsProcessedCount = 1;
    SCH_AppData.SkippedSlotsCount   = 2;
    SCH_AppData.MultipleSlotsCount  = 3;
    SCH_AppData.SameSlotCount       = 4;
    SCH_AppData.ScheduleActivitySuccessCount = 5;
    SCH_AppData.ScheduleActivityFailureCount = 6;
    SCH_AppData.CmdCounter = 7;
    SCH_AppData.ErrCounter = 8;

    /* Set to fail condition "Status != CFE_SUCCESS" after call to SCH_EvsInit */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0, "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   == 0, "SCH_AppData.SkippedSlotsCount   == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  == 0, "SCH_AppData.MultipleSlotsCount  == 0");
    UtAssert_True(SCH_AppData.SameSlotCount       == 0, "SCH_AppData.SameSlotCount       == 0");
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0, "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0, "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True(SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_AppInit_Test_EvsInitError */

void SCH_AppInit_Test_SbInitError(void)
{
    int32   Result;

    SCH_AppData.SlotsProcessedCount = 1;
    SCH_AppData.SkippedSlotsCount   = 2;
    SCH_AppData.MultipleSlotsCount  = 3;
    SCH_AppData.SameSlotCount       = 4;
    SCH_AppData.ScheduleActivitySuccessCount = 5;
    SCH_AppData.ScheduleActivityFailureCount = 6;
    SCH_AppData.CmdCounter = 7;
    SCH_AppData.ErrCounter = 8;

    /* Set to fail condition "Status != CFE_SUCCESS" after call to SCH_SbInit */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0, "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   == 0, "SCH_AppData.SkippedSlotsCount   == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  == 0, "SCH_AppData.MultipleSlotsCount  == 0");
    UtAssert_True(SCH_AppData.SameSlotCount       == 0, "SCH_AppData.SameSlotCount       == 0");
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0, "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0, "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True(SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppInit_Test_SbInitError */

void SCH_AppInit_Test_TblInitError(void)
{
    int32   Result;

    SCH_AppData.SlotsProcessedCount = 1;
    SCH_AppData.SkippedSlotsCount   = 2;
    SCH_AppData.MultipleSlotsCount  = 3;
    SCH_AppData.SameSlotCount       = 4;
    SCH_AppData.ScheduleActivitySuccessCount = 5;
    SCH_AppData.ScheduleActivityFailureCount = 6;
    SCH_AppData.CmdCounter = 7;
    SCH_AppData.ErrCounter = 8;

    /* Set to fail condition "Status != CFE_SUCCESS" after call to SCH_TblInit */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0, "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   == 0, "SCH_AppData.SkippedSlotsCount   == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  == 0, "SCH_AppData.MultipleSlotsCount  == 0");
    UtAssert_True(SCH_AppData.SameSlotCount       == 0, "SCH_AppData.SameSlotCount       == 0");
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0, "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0, "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True(SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppInit_Test_TblInitError */

void SCH_AppInit_Test_TimerInitError(void)
{
    int32   Result;

    SCH_AppData.SlotsProcessedCount = 1;
    SCH_AppData.SkippedSlotsCount   = 2;
    SCH_AppData.MultipleSlotsCount  = 3;
    SCH_AppData.SameSlotCount       = 4;
    SCH_AppData.ScheduleActivitySuccessCount = 5;
    SCH_AppData.ScheduleActivityFailureCount = 6;
    SCH_AppData.CmdCounter = 7;
    SCH_AppData.ErrCounter = 8;

    /* Set to make SCH_TblInit return CFE_SUCCESS, in order to make SCH_AppInit return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Set to fail condition "Status != CFE_SUCCESS" after call to SCH_TimerInit */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_BINSEMCREATE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_AppInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0, "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   == 0, "SCH_AppData.SkippedSlotsCount   == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  == 0, "SCH_AppData.MultipleSlotsCount  == 0");
    UtAssert_True(SCH_AppData.SameSlotCount       == 0, "SCH_AppData.SameSlotCount       == 0");
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0, "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0, "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True(SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppInit_Test_TimerInitError */

void SCH_AppInit_Test_Nominal(void)
{
    int32   Result;
    char    Message[125];

    SCH_AppData.SlotsProcessedCount = 1;
    SCH_AppData.SkippedSlotsCount   = 2;
    SCH_AppData.MultipleSlotsCount  = 3;
    SCH_AppData.SameSlotCount       = 4;
    SCH_AppData.ScheduleActivitySuccessCount = 5;
    SCH_AppData.ScheduleActivityFailureCount = 6;
    SCH_AppData.CmdCounter = 7;
    SCH_AppData.ErrCounter = 8;

    /* Set to make SCH_TblInit return CFE_SUCCESS, in order to make SCH_AppInit return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = SCH_AppInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.SlotsProcessedCount == 0, "SCH_AppData.SlotsProcessedCount == 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   == 0, "SCH_AppData.SkippedSlotsCount   == 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  == 0, "SCH_AppData.MultipleSlotsCount  == 0");
    UtAssert_True(SCH_AppData.SameSlotCount       == 0, "SCH_AppData.SameSlotCount       == 0");
    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount == 0, "SCH_AppData.ScheduleActivitySuccessCount == 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount == 0, "SCH_AppData.ScheduleActivityFailureCount == 0");
    UtAssert_True(SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True(SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    sprintf(Message, "SCH Initialized. Version %d.%d.%d.%d", SCH_MAJOR_VERSION, SCH_MINOR_VERSION, SCH_REVISION, SCH_MISSION_REV);
    UtAssert_True (Ut_CFE_EVS_EventSent(SCH_INITSTATS_INF_EID, CFE_EVS_INFORMATION, Message), Message);

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_AppInit_Test_Nominal */

void SCH_EvsInit_Test_RegisterError(void)
{
    int32   Result;

    SCH_AppData.EventFilters[0].EventID = 1;
    SCH_AppData.EventFilters[0].Mask    = 2;

    SCH_AppData.EventFilters[1].EventID = 3;
    SCH_AppData.EventFilters[1].Mask    = 4;

    SCH_AppData.EventFilters[2].EventID = 5;
    SCH_AppData.EventFilters[2].Mask    = 6;

    SCH_AppData.EventFilters[3].EventID = 7;
    SCH_AppData.EventFilters[3].Mask    = 8;

    SCH_AppData.EventFilters[4].EventID = 9;
    SCH_AppData.EventFilters[4].Mask    = 10;

    /* Set to fail condition "Status != CFE_SUCCESS" */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_EvsInit();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.EventFilters[0].EventID == SCH_SAME_SLOT_EID, "SCH_AppData.EventFilters[0].EventID == SCH_SAME_SLOT_EID");
    UtAssert_True (SCH_AppData.EventFilters[0].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[0].Mask    == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True (SCH_AppData.EventFilters[1].EventID == SCH_MULTI_SLOTS_EID, "SCH_AppData.EventFilters[1].EventID == SCH_MULTI_SLOTS_EID");
    UtAssert_True (SCH_AppData.EventFilters[1].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[1].Mask    == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True (SCH_AppData.EventFilters[2].EventID == SCH_SKIPPED_SLOTS_EID, "SCH_AppData.EventFilters[2].EventID == SCH_SKIPPED_SLOTS_EID");
    UtAssert_True (SCH_AppData.EventFilters[2].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[2].Mask    == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True (SCH_AppData.EventFilters[3].EventID == SCH_CORRUPTION_EID, "SCH_AppData.EventFilters[3].EventID == SCH_CORRUPTION_EID");
    UtAssert_True (SCH_AppData.EventFilters[3].Mask    == CFE_EVS_FIRST_TWO_STOP, "SCH_AppData.EventFilters[3].Mask    == CFE_EVS_FIRST_TWO_STOP");
    UtAssert_True (SCH_AppData.EventFilters[4].EventID == SCH_PACKET_SEND_EID, "SCH_AppData.EventFilters[4].EventID == SCH_PACKET_SEND_EID");
    UtAssert_True (SCH_AppData.EventFilters[4].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[4].Mask    == CFE_EVS_FIRST_ONE_STOP");

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("SCH App: Error Registering For Event Services, RC=0xFFFFFFFF\n"),
        "SCH App: Error Registering For Event Services, RC=0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 1, "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

} /* end SCH_EvsInit_Test_RegisterError */

void SCH_EvsInit_Test_Nominal(void)
{
    int32   Result;

    SCH_AppData.EventFilters[0].EventID = 1;
    SCH_AppData.EventFilters[0].Mask    = 2;

    SCH_AppData.EventFilters[1].EventID = 3;
    SCH_AppData.EventFilters[1].Mask    = 4;

    SCH_AppData.EventFilters[2].EventID = 5;
    SCH_AppData.EventFilters[2].Mask    = 6;

    SCH_AppData.EventFilters[3].EventID = 7;
    SCH_AppData.EventFilters[3].Mask    = 8;

    SCH_AppData.EventFilters[4].EventID = 9;
    SCH_AppData.EventFilters[4].Mask    = 10;

    /* Execute the function being tested */
    Result = SCH_EvsInit();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.EventFilters[0].EventID == SCH_SAME_SLOT_EID, "SCH_AppData.EventFilters[0].EventID == SCH_SAME_SLOT_EID");
    UtAssert_True (SCH_AppData.EventFilters[0].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[0].Mask    == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True (SCH_AppData.EventFilters[1].EventID == SCH_MULTI_SLOTS_EID, "SCH_AppData.EventFilters[1].EventID == SCH_MULTI_SLOTS_EID");
    UtAssert_True (SCH_AppData.EventFilters[1].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[1].Mask    == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True (SCH_AppData.EventFilters[2].EventID == SCH_SKIPPED_SLOTS_EID, "SCH_AppData.EventFilters[2].EventID == SCH_SKIPPED_SLOTS_EID");
    UtAssert_True (SCH_AppData.EventFilters[2].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[2].Mask    == CFE_EVS_FIRST_ONE_STOP");
    UtAssert_True (SCH_AppData.EventFilters[3].EventID == SCH_CORRUPTION_EID, "SCH_AppData.EventFilters[3].EventID == SCH_CORRUPTION_EID");
    UtAssert_True (SCH_AppData.EventFilters[3].Mask    == CFE_EVS_FIRST_TWO_STOP, "SCH_AppData.EventFilters[3].Mask    == CFE_EVS_FIRST_TWO_STOP");
    UtAssert_True (SCH_AppData.EventFilters[4].EventID == SCH_PACKET_SEND_EID, "SCH_AppData.EventFilters[4].EventID == SCH_PACKET_SEND_EID");
    UtAssert_True (SCH_AppData.EventFilters[4].Mask    == CFE_EVS_FIRST_ONE_STOP, "SCH_AppData.EventFilters[4].Mask    == CFE_EVS_FIRST_ONE_STOP");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 0, "Ut_CFE_ES_GetSysLogQueueDepth() == 0");

} /* end SCH_EvsInit_Test_Nominal */

void SCH_SbInit_Test_CreatePipeError(void)
{
    int32   Result;

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t) 1;
    SCH_AppData.CmdPipe = 1;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_SbInit();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0, "SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0");
    UtAssert_True (SCH_AppData.CmdPipe == 0, "SCH_AppData.CmdPipe == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CR_PIPE_ERR_EID, CFE_EVS_ERROR, "Error Creating SB Pipe, RC=0xFFFFFFFF"),
        "Error Creating SB Pipe, RC=0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SbInit_Test_CreatePipeError */

void SCH_SbInit_Test_SubscribeHKError(void)
{
    int32   Result;

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t) 1;
    SCH_AppData.CmdPipe = 1;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_SbInit();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0, "SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0");
    UtAssert_True (SCH_AppData.CmdPipe == 0, "SCH_AppData.CmdPipe == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SUB_HK_REQ_ERR_EID, CFE_EVS_ERROR, "Error Subscribing to HK Request(MID=0x1896), RC=0xFFFFFFFF"),
        "Error Subscribing to HK Request(MID=0x1896), RC=0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SbInit_Test_SubscribeHKError */

void SCH_SbInit_Test_SubscribeGNDError(void)
{
    int32   Result;

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t) 1;
    SCH_AppData.CmdPipe = 1;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, -1, 2);

    /* Execute the function being tested */
    Result = SCH_SbInit();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0, "SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0");
    UtAssert_True (SCH_AppData.CmdPipe == 0, "SCH_AppData.CmdPipe == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SUB_GND_CMD_ERR_EID, CFE_EVS_ERROR, "Error Subscribing to GND CMD(MID=0x1895), RC=0xFFFFFFFF"),
        "Error Subscribing to GND CMD(MID=0x1895), RC=0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SbInit_Test_SubscribeGNDError */

void SCH_SbInit_Test_Nominal(void)
{
    int32   Result;

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t) 1;
    SCH_AppData.CmdPipe = 1;

    /* Execute the function being tested */
    Result = SCH_SbInit();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0, "SCH_AppData.MsgPtr  == (CFE_SB_MsgPtr_t) 0");
    UtAssert_True (SCH_AppData.CmdPipe == 0, "SCH_AppData.CmdPipe == 0");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_SbInit_Test_Nominal */

void SCH_TblInit_Test_RegisterSdtError(void)
{
    int32   Result;

    SCH_AppData.ScheduleTable = (SCH_ScheduleEntry_t *) 1;
    SCH_AppData.MessageTable  = (SCH_MessageEntry_t  *) 2;
    SCH_AppData.ScheduleTableHandle = 3;
    SCH_AppData.MessageTableHandle  = 4;
    SCH_AppData.BadTableDataCount       = 5;
    SCH_AppData.TableVerifySuccessCount = 6;
    SCH_AppData.TableVerifyFailureCount = 7;
    SCH_AppData.TablePassCount          = 8;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_TblInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL, "SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL");
    UtAssert_True(SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL, "SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL");

    UtAssert_True(SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE, "SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE");
    UtAssert_True(SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE, "SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE");
    
    UtAssert_True(SCH_AppData.BadTableDataCount       == 0, "SCH_AppData.BadTableDataCount       == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.TablePassCount          == 0, "SCH_AppData.TablePassCount          == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SDT_REG_ERR_EID, CFE_EVS_ERROR, "Error Registering SDT, RC=0xFFFFFFFF"),
        "Error Registering SDT, RC=0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TblInit_Test_RegisterSdtError */

void SCH_TblInit_Test_RegisterMdtError(void)
{
    int32   Result;

    SCH_AppData.ScheduleTable = (SCH_ScheduleEntry_t *) 1;
    SCH_AppData.MessageTable  = (SCH_MessageEntry_t  *) 2;
    SCH_AppData.ScheduleTableHandle = 3;
    SCH_AppData.MessageTableHandle  = 4;
    SCH_AppData.BadTableDataCount       = 5;
    SCH_AppData.TableVerifySuccessCount = 6;
    SCH_AppData.TableVerifyFailureCount = 7;
    SCH_AppData.TablePassCount          = 8;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, -1, 2);

    /* Execute the function being tested */
    Result = SCH_TblInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL, "SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL");
    UtAssert_True(SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL, "SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL");

    /* Not verifying SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    UtAssert_True(SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE, "SCH_AppData.MessageTableHandle  == CFE_TBL_BAD_TABLE_HANDLE");
    
    UtAssert_True(SCH_AppData.BadTableDataCount       == 0, "SCH_AppData.BadTableDataCount       == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.TablePassCount          == 0, "SCH_AppData.TablePassCount          == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MDT_REG_ERR_EID, CFE_EVS_ERROR, "Error Registering MDT, RC=0xFFFFFFFF"),
        "Error Registering MDT, RC=0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TblInit_Test_RegisterMdtError */

void SCH_TblInit_Test_LoadSdtError(void)
{
    int32   Result;

    SCH_AppData.ScheduleTable = (SCH_ScheduleEntry_t *) 1;
    SCH_AppData.MessageTable  = (SCH_MessageEntry_t  *) 2;
    SCH_AppData.ScheduleTableHandle = 3;
    SCH_AppData.MessageTableHandle  = 4;
    SCH_AppData.BadTableDataCount       = 5;
    SCH_AppData.TableVerifySuccessCount = 6;
    SCH_AppData.TableVerifyFailureCount = 7;
    SCH_AppData.TablePassCount          = 8;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_TblInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL, "SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL");
    UtAssert_True(SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL, "SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL");

    /* Not verifying SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    /* Not verifying SCH_AppData.MessageTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    
    UtAssert_True(SCH_AppData.BadTableDataCount       == 0, "SCH_AppData.BadTableDataCount       == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.TablePassCount          == 0, "SCH_AppData.TablePassCount          == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SDT_LOAD_ERR_EID, CFE_EVS_ERROR, "Error (RC=0xFFFFFFFF) Loading SDT with /cf/apps/sch_def_schtbl.tbl"),
        "Error (RC=0xFFFFFFFF) Loading SDT with /cf/apps/sch_def_schtbl.tbl");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TblInit_Test_LoadSdtError */

void SCH_TblInit_Test_LoadMdtError(void)
{
    int32   Result;

    SCH_AppData.ScheduleTable = (SCH_ScheduleEntry_t *) 1;
    SCH_AppData.MessageTable  = (SCH_MessageEntry_t  *) 2;
    SCH_AppData.ScheduleTableHandle = 3;
    SCH_AppData.MessageTableHandle  = 4;
    SCH_AppData.BadTableDataCount       = 5;
    SCH_AppData.TableVerifySuccessCount = 6;
    SCH_AppData.TableVerifyFailureCount = 7;
    SCH_AppData.TablePassCount          = 8;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" (by setting only first call to CFE_TBL_Load to succeed) */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    Result = SCH_TblInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL, "SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL");
    UtAssert_True(SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL, "SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL");

    /* Not verifying SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    /* Not verifying SCH_AppData.MessageTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    
    UtAssert_True(SCH_AppData.BadTableDataCount       == 0, "SCH_AppData.BadTableDataCount       == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.TablePassCount          == 0, "SCH_AppData.TablePassCount          == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MDT_LOAD_ERR_EID, CFE_EVS_ERROR, "Error (RC=0xCC000013) Loading MDT with /cf/apps/sch_def_msgtbl.tbl"),
        "Error (RC=0xCC000013) Loading MDT with /cf/apps/sch_def_msgtbl.tbl");

    UtAssert_True (Result == 0xCC000013, "Result == 0xCC000013");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TblInit_Test_LoadMdtError */

void SCH_TblInit_Test_AcquirePointersError(void)
{
    int32   Result;

    SCH_AppData.ScheduleTable = (SCH_ScheduleEntry_t *) 1;
    SCH_AppData.MessageTable  = (SCH_MessageEntry_t  *) 2;
    SCH_AppData.ScheduleTableHandle = 3;
    SCH_AppData.MessageTableHandle  = 4;
    SCH_AppData.BadTableDataCount       = 5;
    SCH_AppData.TableVerifySuccessCount = 6;
    SCH_AppData.TableVerifyFailureCount = 7;
    SCH_AppData.TablePassCount          = 8;

    /* Set to prevent all calls to CFE_TBL_Load from failing */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Set to make "SCH_AcquirePointers" return not success, in order to generate error message SCH_ACQ_PTR_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_TblInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL, "SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL");
    UtAssert_True(SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL, "SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL");

    /* Not verifying SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    /* Not verifying SCH_AppData.MessageTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    
    UtAssert_True(SCH_AppData.BadTableDataCount       == 0, "SCH_AppData.BadTableDataCount       == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.TablePassCount          == 0, "SCH_AppData.TablePassCount          == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR, "Error Acquiring Tbl Ptrs (RC=0xFFFFFFFF)"),
        "Error Acquiring Tbl Ptrs (RC=0xFFFFFFFF)");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TblInit_Test_AcquirePointersError */

void SCH_TblInit_Test_Nominal(void)
{
    int32   Result;

    SCH_AppData.ScheduleTable = (SCH_ScheduleEntry_t *) 1;
    SCH_AppData.MessageTable  = (SCH_MessageEntry_t  *) 2;
    SCH_AppData.ScheduleTableHandle = 3;
    SCH_AppData.MessageTableHandle  = 4;
    SCH_AppData.BadTableDataCount       = 5;
    SCH_AppData.TableVerifySuccessCount = 6;
    SCH_AppData.TableVerifyFailureCount = 7;
    SCH_AppData.TablePassCount          = 8;

    /* Set to prevent all calls to CFE_TBL_Load from failing */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = SCH_TblInit();
    
    /* Verify results */
    /* Not verifying SCH_AppData.ScheduleTable == (SCH_ScheduleEntry_t *) NULL, because it's modified by SCH_AcquirePointers */
    /* Not verifying SCH_AppData.MessageTable  == (SCH_MessageEntry_t  *) NULL, because it's modified by SCH_AcquirePointers */

    /* Not verifying SCH_AppData.ScheduleTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    /* Not verifying SCH_AppData.MessageTableHandle == CFE_TBL_BAD_TABLE_HANDLE, because it's modified by CFE_TBL_Register */
    
    UtAssert_True(SCH_AppData.BadTableDataCount       == 0, "SCH_AppData.BadTableDataCount       == 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount == 0, "SCH_AppData.TableVerifySuccessCount == 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount == 0, "SCH_AppData.TableVerifyFailureCount == 0");
    UtAssert_True(SCH_AppData.TablePassCount          == 0, "SCH_AppData.TablePassCount          == 0");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_TblInit_Test_Nominal */

void SCH_TimerInit_Test_CustomEarlyInitError(void)
{
    int32   Result;

    SCH_AppData.IgnoreMajorFrame     = 1;
    SCH_AppData.IgnoreMajorFrameMsgSent = 2;
    SCH_AppData.UnexpectedMajorFrame = 3;
    SCH_AppData.SyncToMET            = 4;
    SCH_AppData.MajorFrameSource     = 5;
    SCH_AppData.NextSlotNumber       = 6;
    SCH_AppData.MinorFramesSinceTone = 7;
    SCH_AppData.LastSyncMETSlot      = 8;
    SCH_AppData.SyncAttemptsLeft     = 9;
    SCH_AppData.UnexpectedMajorFrameCount   = 10;
    SCH_AppData.MissedMajorFrameCount       = 11;
    SCH_AppData.ValidMajorFrameCount        = 12;
    SCH_AppData.WorstCaseSlotsPerMinorFrame = 13;
    SCH_AppData.ClockAccuracy = 14;

    /* Set to make SCH_CustomEarlyInit return -1, in order to generate error message SCH_MINOR_FRAME_TIMER_CREATE_ERR_EID */
    /* Note: CFE_PSP_TimerCreate is currently defined in sch_custom.c.  It calls OS_TimerCreate, which is what we're setting a return value for */
    Ut_OSTIMER_SetReturnCode(UT_OSTIMER_CREATE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_TimerInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.IgnoreMajorFrame     == FALSE, "SCH_AppData.IgnoreMajorFrame     == FALSE");
    UtAssert_True(SCH_AppData.IgnoreMajorFrameMsgSent == FALSE, "SCH_AppData.IgnoreMajorFrameMsgSent == FALSE");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrame == FALSE, "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.SyncToMET            == SCH_NOT_SYNCHRONIZED, "SCH_AppData.SyncToMET            == SCH_NOT_SYNCHRONIZED");
    UtAssert_True(SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE, "SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE");
    UtAssert_True(SCH_AppData.NextSlotNumber       == 0, "SCH_AppData.NextSlotNumber       == 0");
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT, "SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT");
    UtAssert_True(SCH_AppData.LastSyncMETSlot      == 0, "SCH_AppData.LastSyncMETSlot      == 0");
    UtAssert_True(SCH_AppData.SyncAttemptsLeft     == 0, "SCH_AppData.SyncAttemptsLeft     == 0");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrameCount   == 0, "SCH_AppData.UnexpectedMajorFrameCount   == 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount       == 0, "SCH_AppData.MissedMajorFrameCount       == 0");
    UtAssert_True(SCH_AppData.ValidMajorFrameCount        == 0, "SCH_AppData.ValidMajorFrameCount        == 0");
    UtAssert_True(SCH_AppData.WorstCaseSlotsPerMinorFrame == 1, "SCH_AppData.WorstCaseSlotsPerMinorFrame == 1");
    UtAssert_True(SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY, "SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MINOR_FRAME_TIMER_CREATE_ERR_EID, CFE_EVS_ERROR, "Error creating Timer (RC=0xFFFFFFFF)"),
        "Error creating Timer (RC=0xFFFFFFFF)");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TimerInit_Test_CustomEarlyInitError */

void SCH_TimerInit_Test_TimerAccuracyWarning(void)
{
    int32   Result;

    SCH_AppData.IgnoreMajorFrame     = 1;
    SCH_AppData.IgnoreMajorFrameMsgSent = 2;
    SCH_AppData.UnexpectedMajorFrame = 3;
    SCH_AppData.SyncToMET            = 4;
    SCH_AppData.MajorFrameSource     = 5;
    SCH_AppData.NextSlotNumber       = 6;
    SCH_AppData.MinorFramesSinceTone = 7;
    SCH_AppData.LastSyncMETSlot      = 8;
    SCH_AppData.SyncAttemptsLeft     = 9;
    SCH_AppData.UnexpectedMajorFrameCount   = 10;
    SCH_AppData.MissedMajorFrameCount       = 11;
    SCH_AppData.ValidMajorFrameCount        = 12;
    SCH_AppData.WorstCaseSlotsPerMinorFrame = 13;
    SCH_AppData.ClockAccuracy = 14;

    /* Satisfies condition "SCH_AppData.ClockAccuracy > SCH_WORST_CLOCK_ACCURACY" */
    Ut_OSTIMER_SetFunctionHook(UT_OSTIMER_CREATE_INDEX, &SCH_APP_TEST_OS_TimerCreateHook);

    /* Execute the function being tested */
    Result = SCH_TimerInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.IgnoreMajorFrame     == FALSE, "SCH_AppData.IgnoreMajorFrame     == FALSE");
    UtAssert_True(SCH_AppData.IgnoreMajorFrameMsgSent == FALSE, "SCH_AppData.IgnoreMajorFrameMsgSent == FALSE");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrame == FALSE, "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE, "SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE");
    UtAssert_True(SCH_AppData.NextSlotNumber       == 0, "SCH_AppData.NextSlotNumber       == 0");
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT, "SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT");
    UtAssert_True(SCH_AppData.LastSyncMETSlot      == 0, "SCH_AppData.LastSyncMETSlot      == 0");
    UtAssert_True(SCH_AppData.SyncAttemptsLeft     == 0, "SCH_AppData.SyncAttemptsLeft     == 0");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrameCount   == 0, "SCH_AppData.UnexpectedMajorFrameCount   == 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount       == 0, "SCH_AppData.MissedMajorFrameCount       == 0");
    UtAssert_True(SCH_AppData.ValidMajorFrameCount        == 0, "SCH_AppData.ValidMajorFrameCount        == 0");
    UtAssert_True(SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY, "SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MINOR_FRAME_TIMER_ACC_WARN_EID, CFE_EVS_INFORMATION, "OS Timer Accuracy (102 > reqd 101 usec) requires Minor Frame MET sync"),
        "OS Timer Accuracy (102 > reqd 101 usec) requires Minor Frame MET sync");

    UtAssert_True(SCH_AppData.SyncToMET == SCH_MINOR_SYNCHRONIZED, "SCH_AppData.SyncToMET == SCH_MINOR_SYNCHRONIZED");

    UtAssert_True
        (SCH_AppData.WorstCaseSlotsPerMinorFrame == ((SCH_AppData.ClockAccuracy * 2) / SCH_NORMAL_SLOT_PERIOD) + 1,
        "SCH_AppData.WorstCaseSlotsPerMinorFrame == ((SCH_AppData.ClockAccuracy * 2) / SCH_NORMAL_SLOT_PERIOD) + 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TimerInit_Test_TimerAccuracyWarning */

void SCH_TimerInit_Test_BinSemCreateError(void)
{
    int32   Result;

    SCH_AppData.IgnoreMajorFrame     = 1;
    SCH_AppData.IgnoreMajorFrameMsgSent = 2;
    SCH_AppData.UnexpectedMajorFrame = 3;
    SCH_AppData.SyncToMET            = 4;
    SCH_AppData.MajorFrameSource     = 5;
    SCH_AppData.NextSlotNumber       = 6;
    SCH_AppData.MinorFramesSinceTone = 7;
    SCH_AppData.LastSyncMETSlot      = 8;
    SCH_AppData.SyncAttemptsLeft     = 9;
    SCH_AppData.UnexpectedMajorFrameCount   = 10;
    SCH_AppData.MissedMajorFrameCount       = 11;
    SCH_AppData.ValidMajorFrameCount        = 12;
    SCH_AppData.WorstCaseSlotsPerMinorFrame = 13;
    SCH_AppData.ClockAccuracy = 14;

    /* Set to satisfy subsequent condition "Status != CFE_SUCCESS" */
    Ut_OSAPI_SetReturnCode(UT_OSAPI_BINSEMCREATE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_TimerInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.IgnoreMajorFrame     == FALSE, "SCH_AppData.IgnoreMajorFrame     == FALSE");
    UtAssert_True(SCH_AppData.IgnoreMajorFrameMsgSent == FALSE, "SCH_AppData.IgnoreMajorFrameMsgSent == FALSE");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrame == FALSE, "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.SyncToMET            == SCH_NOT_SYNCHRONIZED, "SCH_AppData.SyncToMET            == SCH_NOT_SYNCHRONIZED");
    UtAssert_True(SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE, "SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE");
    UtAssert_True(SCH_AppData.NextSlotNumber       == 0, "SCH_AppData.NextSlotNumber       == 0");
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT, "SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT");
    UtAssert_True(SCH_AppData.LastSyncMETSlot      == 0, "SCH_AppData.LastSyncMETSlot      == 0");
    UtAssert_True(SCH_AppData.SyncAttemptsLeft     == 0, "SCH_AppData.SyncAttemptsLeft     == 0");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrameCount   == 0, "SCH_AppData.UnexpectedMajorFrameCount   == 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount       == 0, "SCH_AppData.MissedMajorFrameCount       == 0");
    UtAssert_True(SCH_AppData.ValidMajorFrameCount        == 0, "SCH_AppData.ValidMajorFrameCount        == 0");
    UtAssert_True(SCH_AppData.WorstCaseSlotsPerMinorFrame == 1, "SCH_AppData.WorstCaseSlotsPerMinorFrame == 1");
    UtAssert_True(SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY, "SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SEM_CREATE_ERR_EID, CFE_EVS_ERROR, "Error creating Main Loop Timing Semaphore (RC=0xFFFFFFFF)"),
        "Error creating Main Loop Timing Semaphore (RC=0xFFFFFFFF)");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_TimerInit_Test_BinSemCreateError */

void SCH_TimerInit_Test_Nominal(void)
{
    int32   Result;

    SCH_AppData.IgnoreMajorFrame     = 1;
    SCH_AppData.IgnoreMajorFrameMsgSent = 2;
    SCH_AppData.UnexpectedMajorFrame = 3;
    SCH_AppData.SyncToMET            = 4;
    SCH_AppData.MajorFrameSource     = 5;
    SCH_AppData.NextSlotNumber       = 6;
    SCH_AppData.MinorFramesSinceTone = 7;
    SCH_AppData.LastSyncMETSlot      = 8;
    SCH_AppData.SyncAttemptsLeft     = 9;
    SCH_AppData.UnexpectedMajorFrameCount   = 10;
    SCH_AppData.MissedMajorFrameCount       = 11;
    SCH_AppData.ValidMajorFrameCount        = 12;
    SCH_AppData.WorstCaseSlotsPerMinorFrame = 13;
    SCH_AppData.ClockAccuracy = 14;

    /* Execute the function being tested */
    Result = SCH_TimerInit();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.IgnoreMajorFrame     == FALSE, "SCH_AppData.IgnoreMajorFrame     == FALSE");
    UtAssert_True(SCH_AppData.IgnoreMajorFrameMsgSent == FALSE, "SCH_AppData.IgnoreMajorFrameMsgSent == FALSE");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrame == FALSE, "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.SyncToMET            == SCH_NOT_SYNCHRONIZED, "SCH_AppData.SyncToMET            == SCH_NOT_SYNCHRONIZED");
    UtAssert_True(SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE, "SCH_AppData.MajorFrameSource     == SCH_MAJOR_FS_NONE");
    UtAssert_True(SCH_AppData.NextSlotNumber       == 0, "SCH_AppData.NextSlotNumber       == 0");
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT, "SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT");
    UtAssert_True(SCH_AppData.LastSyncMETSlot      == 0, "SCH_AppData.LastSyncMETSlot      == 0");
    UtAssert_True(SCH_AppData.SyncAttemptsLeft     == 0, "SCH_AppData.SyncAttemptsLeft     == 0");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrameCount   == 0, "SCH_AppData.UnexpectedMajorFrameCount   == 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount       == 0, "SCH_AppData.MissedMajorFrameCount       == 0");
    UtAssert_True(SCH_AppData.ValidMajorFrameCount        == 0, "SCH_AppData.ValidMajorFrameCount        == 0");
    UtAssert_True(SCH_AppData.WorstCaseSlotsPerMinorFrame == 1, "SCH_AppData.WorstCaseSlotsPerMinorFrame == 1");
    UtAssert_True(SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY, "SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_TimerInit_Test_Nominal */

void SCH_ProcessScheduleTable_Test_ProcessCount2LastProcessCount1(void)
{
    int32   Result;

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = 2;
    SCH_AppData.NextSlotNumber       = 1;
    SCH_AppData.LastProcessCount     = 1;

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == 2, "SCH_AppData.LastProcessCount == 2");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessScheduleTable_Test_ProcessCount2LastProcessCount1 */

void SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame(void)
{
    int32   Result;

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_TOTAL_SLOTS - 1;
    SCH_AppData.NextSlotNumber       = 0;
    SCH_AppData.LastProcessCount     = 1;

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS, "SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame */

void SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame(void)
{
    int32   Result;

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_TOTAL_SLOTS - 1;
    SCH_AppData.NextSlotNumber       = 0;
    SCH_AppData.LastProcessCount     = SCH_TOTAL_SLOTS;

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS, "SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS");

    UtAssert_True(SCH_AppData.SameSlotCount == 1, "SCH_AppData.SameSlotCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SAME_SLOT_EID, CFE_EVS_DEBUG, "Slot did not increment: slot = 99"),
        "Slot did not increment: slot = 99");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame */

void SCH_ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover(void)
{
    int32   Result;

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.ProcessCountOtherAndNoRollover */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = 0;
    SCH_AppData.NextSlotNumber       = SCH_TOTAL_SLOTS;
    SCH_AppData.LastProcessCount     = 1;

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == 1, "SCH_AppData.LastProcessCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover */

void SCH_ProcessScheduleTable_Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands(void)
{
    int32   Result;

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_MAX_LAG_COUNT + 1;
    SCH_AppData.NextSlotNumber       = SCH_MAX_LAG_COUNT + 3;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS - 1, "SCH_AppData.LastProcessCount == SCH_TOTAL_SLOTS - 1");

    UtAssert_True(SCH_AppData.SkippedSlotsCount == 1, "SCH_AppData.SkippedSlotsCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SKIPPED_SLOTS_EID, CFE_EVS_ERROR, "Slots skipped: slot = 53, count = 98"),
        "Slots skipped: slot = 53, count = 98");

    UtAssert_True(SCH_AppData.TablePassCount == 1, "SCH_AppData.TablePassCount == 1");

    /* Note: SCH_AppData.NextSlotNumber is incremented by 1 in SCH_ProcessNextSlot */
    UtAssert_True(SCH_AppData.NextSlotNumber == SCH_MAX_LAG_COUNT + 2, "SCH_AppData.NextSlotNumber == SCH_MAX_LAG_COUNT + 2");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ProcessScheduleTable_Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands */

void SCH_ProcessScheduleTable_Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup(void)
{
    int32   Result;

    SCH_AppData.SyncToMET = 99;

    SCH_AppData.NextSlotNumber = SCH_TOTAL_SLOTS - SCH_MAX_LAG_COUNT + 1;

    SCH_AppData.WorstCaseSlotsPerMinorFrame = 1;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT, "SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT");

    UtAssert_True(SCH_AppData.MultipleSlotsCount == 1, "SCH_AppData.MultipleSlotsCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MULTI_SLOTS_EID, CFE_EVS_INFORMATION, "Multiple slots processed: slot = 51, count = 5"),
        "Multiple slots processed: slot = 51, count = 5");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ProcessScheduleTable_Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup */

void SCH_ProcessScheduleTable_Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup(void)
{
    int32   Result;

    /* Cause SCH_CustomGetCurrentSlotNumber to return SCH_AppData.MinorFramesSinceTone */
    SCH_AppData.SyncToMET = SCH_NOT_SYNCHRONIZED;

    SCH_AppData.MinorFramesSinceTone = SCH_MAX_LAG_COUNT - 1;
    SCH_AppData.NextSlotNumber       = SCH_TOTAL_SLOTS;

    SCH_AppData.WorstCaseSlotsPerMinorFrame = 100;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessScheduleTable();
    
    /* Verify results */
    UtAssert_True(SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT, "SCH_AppData.LastProcessCount == SCH_MAX_LAG_COUNT");

    UtAssert_True(SCH_AppData.MultipleSlotsCount == 1, "SCH_AppData.MultipleSlotsCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MULTI_SLOTS_EID, CFE_EVS_INFORMATION, "Multiple slots processed: slot = 100, count = 5"),
        "Multiple slots processed: slot = 100, count = 5");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ProcessScheduleTable_Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup */

void SCH_ProcessNextSlot_Test_ProcessCommandsRollover(void)
{
    int32   Result;
    int32   SlotIndex;

    SCH_AppData.NextSlotNumber                       = SCH_TIME_SYNC_SLOT;
    SlotIndex                                        = SCH_AppData.NextSlotNumber * SCH_ENTRIES_PER_SLOT;
    SCH_AppData.ScheduleTable[SlotIndex].EnableState = SCH_ENABLED;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessNextSlot();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.NextSlotNumber == 0, "SCH_AppData.NextSlotNumber == 0");
    UtAssert_True (SCH_AppData.TablePassCount == 1, "SCH_AppData.TablePassCount == 1");
    UtAssert_True (SCH_AppData.SlotsProcessedCount == 1, "SCH_AppData.SlotsProcessedCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
    /* Generates 2 event message we don't care about in this test */

} /* end SCH_ProcessNextSlot_Test_ProcessCommandsRollover */

void SCH_ProcessNextSlot_Test_DoNotProcessCommandsNoRollover(void)
{
    int32   Result;
    int32   SlotIndex;

    SCH_AppData.NextSlotNumber                       = 1;
    SlotIndex                                        = SCH_AppData.NextSlotNumber * SCH_ENTRIES_PER_SLOT;
    SCH_AppData.ScheduleTable[SlotIndex].EnableState = SCH_ENABLED;

    /* Set to prevent failure in SCH_ProcessCommands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessNextSlot();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.NextSlotNumber == 2, "SCH_AppData.NextSlotNumber == 2");
    UtAssert_True (SCH_AppData.SlotsProcessedCount == 1, "SCH_AppData.SlotsProcessedCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
    /* Generates 2 event message we don't care about in this test */

} /* end SCH_ProcessNextSlot_Test_DoNotProcessCommandsNoRollover */

void SCH_ProcessNextEntry_Test_CorruptMessageIndex(void)
{
    int32 EntryNumber = 1;

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES;  /* Only this element should fail the first if-statement */
    SCH_AppData.ScheduleTable[EntryNumber].Frequency    = 99;
    SCH_AppData.ScheduleTable[EntryNumber].Type         = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder    = 1;

    SCH_AppData.NextSlotNumber = 2;

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber], EntryNumber);
    
    /* Verify results */
    UtAssert_True (SCH_AppData.BadTableDataCount == 1, "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (1): slot = 2, entry = 1"),
        "Corrupt data error (1): slot = 2, entry = 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (2): msg = 128, freq = 99, type = 1, rem = 1"),
        "Corrupt data error (2): msg = 128, freq = 99, type = 1, rem = 1");

    UtAssert_True (SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED, "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ProcessNextEntry_Test_CorruptMessageIndex */

void SCH_ProcessNextEntry_Test_CorruptFrequency(void)
{
    int32 EntryNumber = 1;

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency    = SCH_UNUSED;            /* Only this element should fail the first if-statement */
    SCH_AppData.ScheduleTable[EntryNumber].Type         = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder    = 1;

    SCH_AppData.NextSlotNumber = 2;

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber], EntryNumber);
    
    /* Verify results */
    UtAssert_True (SCH_AppData.BadTableDataCount == 1, "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (1): slot = 2, entry = 1"),
        "Corrupt data error (1): slot = 2, entry = 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (2): msg = 127, freq = 0, type = 1, rem = 1"),
        "Corrupt data error (2): msg = 127, freq = 0, type = 1, rem = 1");

    UtAssert_True (SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED, "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ProcessNextEntry_Test_CorruptFrequency */

void SCH_ProcessNextEntry_Test_CorruptType(void)
{
    int32 EntryNumber = 1;

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency    = 99;
    SCH_AppData.ScheduleTable[EntryNumber].Type         = 99;   /* Only this element should fail the first if-statement */
    SCH_AppData.ScheduleTable[EntryNumber].Remainder    = 1;

    SCH_AppData.NextSlotNumber = 2;

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber], EntryNumber);
    
    /* Verify results */
    UtAssert_True (SCH_AppData.BadTableDataCount == 1, "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (1): slot = 2, entry = 1"),
        "Corrupt data error (1): slot = 2, entry = 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (2): msg = 127, freq = 99, type = 99, rem = 1"),
        "Corrupt data error (2): msg = 127, freq = 99, type = 99, rem = 1");

    UtAssert_True (SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED, "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ProcessNextEntry_Test_CorruptType */

void SCH_ProcessNextEntry_Test_CorruptRemainder(void)
{
    int32 EntryNumber = 1;

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency    = 99;
    SCH_AppData.ScheduleTable[EntryNumber].Type         = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder    = 99;                      /* Only this element should fail the first if-statement */

    SCH_AppData.NextSlotNumber = 2;

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber], EntryNumber);
    
    /* Verify results */
    UtAssert_True (SCH_AppData.BadTableDataCount == 1, "SCH_AppData.BadTableDataCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (1): slot = 2, entry = 1"),
        "Corrupt data error (1): slot = 2, entry = 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CORRUPTION_EID, CFE_EVS_ERROR, "Corrupt data error (2): msg = 127, freq = 99, type = 1, rem = 99"),
        "Corrupt data error (2): msg = 127, freq = 99, type = 1, rem = 99");

    UtAssert_True (SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED, "SCH_AppData.ScheduleTable[EntryNumber].EnableState == SCH_DISABLED");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ProcessNextEntry_Test_CorruptRemainder */

void SCH_ProcessNextEntry_Test_Success(void)
{
    int32 EntryNumber = 1;

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency    = 2;
    SCH_AppData.ScheduleTable[EntryNumber].Type         = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder    = 1;

    SCH_AppData.NextSlotNumber = 2;
    SCH_AppData.TablePassCount = 3;

    /* Set to satisfy condition "Status == CFE_SUCCESS", and to prevent possible segmentation fault  */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SENDMSG_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber], EntryNumber);
    
    /* Verify results */
    UtAssert_True (SCH_AppData.ScheduleActivitySuccessCount == 1, "SCH_AppData.ScheduleActivitySuccessCount == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessNextEntry_Test_Success */

void SCH_ProcessNextEntry_Test_PacketSendError(void)
{
    int32 EntryNumber = 1;

    SCH_AppData.ScheduleTable[EntryNumber].MessageIndex = SCH_MAX_MESSAGES - 1;
    SCH_AppData.ScheduleTable[EntryNumber].Frequency    = 2;
    SCH_AppData.ScheduleTable[EntryNumber].Type         = SCH_ACTIVITY_SEND_MSG;
    SCH_AppData.ScheduleTable[EntryNumber].Remainder    = 1;

    SCH_AppData.NextSlotNumber = 2;
    SCH_AppData.TablePassCount = 3;

    /* Set to fail condition "Status == CFE_SUCCESS", and to prevent possible segmentation fault */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SENDMSG_INDEX, -1, 1);

    /* Execute the function being tested */
    SCH_ProcessNextEntry(&SCH_AppData.ScheduleTable[EntryNumber], EntryNumber);
    
    /* Verify results */
    UtAssert_True (SCH_AppData.ScheduleActivityFailureCount == 1, "SCH_AppData.ScheduleActivityFailureCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_PACKET_SEND_EID, CFE_EVS_ERROR, "Activity error: slot = 2, entry = 1, err = 0xFFFFFFFF"),
        "Activity error: slot = 2, entry = 1, err = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ProcessNextEntry_Test_PacketSendError */

void SCH_ProcessCommands_Test_OneMessage(void)
{
    int32   Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_SEND_HK_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t)(&CmdPacket);

    /* Set to satisfy condition "Status == CFE_SUCCESS" and to prevent unwanted error message on first run.  On second run, fail condition. */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_RCVMSG_INDEX, &SCH_APP_TEST_CFE_SB_RcvMsgHook);

    /* Set to make SCH_HousekeepingCmd make SCH_AppPipe return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);

    /* Execute the function being tested */
    Result = SCH_ProcessCommands();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessCommands_Test_OneMessage */

void SCH_ProcessCommands_Test_NoMessage(void)
{
    int32   Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_SEND_HK_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t)(&CmdPacket);

    /* Set to satisfy condition "Status == CFE_SUCCESS" and to prevent unwanted error message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SB_NO_MESSAGE, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessCommands();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessCommands_Test_NoMessage */

void SCH_ProcessCommands_Test_RcvMsgError(void)
{
    int32   Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_SEND_HK_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t)(&CmdPacket);

    /* Set to fail condition "Status == CFE_SUCCESS" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessCommands();
    
    /* Verify results */
    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessCommands_Test_RcvMsgError */

void SCH_ProcessCommands_Test_AppPipeError(void)
{
    int32   Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_SEND_HK_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t)(&CmdPacket);

    /* Set to satisfy condition "Status == CFE_SUCCESS" and to prevent unwanted error message */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);

    /* Set to make SCH_HousekeepingCmd make SCH_AppPipe return -1, in order to make function under test return -1 */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_ProcessCommands();
    
    /* Verify results */
    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_ProcessCommands_Test_AppPipeError */

void SCH_ValidateScheduleData_Test_GarbageFrequency(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;    /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[99] rem[0] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[99] rem[0] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_GARBAGE_ENTRY, "Result == SCH_SDT_GARBAGE_ENTRY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_GarbageFrequency */

void SCH_ValidateScheduleData_Test_GarbageRemainder(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 99;    /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[0] rem[99] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[0] rem[99] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_GARBAGE_ENTRY, "Result == SCH_SDT_GARBAGE_ENTRY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_GarbageRemainder */

void SCH_ValidateScheduleData_Test_GarbageGroupData(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = 0x0000000F;    /* Only this element should fail the second if-statement */
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[0] rem[0] msg[0] grp[0x0000000F]"),
        "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[0] rem[0] msg[0] grp[0x0000000F]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_GARBAGE_ENTRY, "Result == SCH_SDT_GARBAGE_ENTRY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_GarbageGroupData */

void SCH_ValidateScheduleData_Test_GarbageType(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = 99;    /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[0] typ[99] fre[0] rem[0] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[0] typ[99] fre[0] rem[0] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_GARBAGE_ENTRY, "Result == SCH_SDT_GARBAGE_ENTRY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_GarbageType */

void SCH_ValidateScheduleData_Test_GarbageMessageIndex(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 99;    /* Only this element should fail the second if-statement */
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[0] rem[0] msg[99] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[0] typ[0] fre[0] rem[0] msg[99] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_GARBAGE_ENTRY, "Result == SCH_SDT_GARBAGE_ENTRY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_GarbageMessageIndex */

void SCH_ValidateScheduleData_Test_EnableStateUnusedAllFieldsUnused(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[0] unused[500]"),
        "Schedule table verify results -- good[0] bad[0] unused[500]");

    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateScheduleData_Test_EnableStateUnusedAllFieldsUnused */

void SCH_ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_ENABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[1] typ[0] fre[0] rem[0] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[1] typ[0] fre[0] rem[0] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_NO_FREQUENCY, "Result == SCH_SDT_NO_FREQUENCY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused */

void SCH_ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[2] typ[0] fre[0] rem[0] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[2] typ[0] fre[0] rem[0] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_NO_FREQUENCY, "Result == SCH_SDT_NO_FREQUENCY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused */

void SCH_ValidateScheduleData_Test_BadRemainder(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 100;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[2] typ[0] fre[99] rem[100] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[2] typ[0] fre[99] rem[100] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_BAD_REMAINDER, "Result == SCH_SDT_BAD_REMAINDER");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_BadRemainder */

void SCH_ValidateScheduleData_Test_BadActivity(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = 99;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_UNUSED;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[2] typ[99] fre[99] rem[1] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[2] typ[99] fre[99] rem[1] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_BAD_ACTIVITY, "Result == SCH_SDT_BAD_ACTIVITY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_BadActivity */

void SCH_ValidateScheduleData_Test_MsgIndexZero(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 0;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[99] rem[1] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[99] rem[1] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_BAD_MSG_INDEX, "Result == SCH_SDT_BAD_MSG_INDEX");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_MsgIndexZero */

void SCH_ValidateScheduleData_Test_MsgIndexTooHigh(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_MAX_MESSAGES;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[99] rem[1] msg[128] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[2] typ[1] fre[99] rem[1] msg[128] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_BAD_MSG_INDEX, "Result == SCH_SDT_BAD_MSG_INDEX");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_MsgIndexTooHigh */

void SCH_ValidateScheduleData_Test_ValidEntryResult(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = SCH_MAX_MESSAGES - 1;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[500] bad[0] unused[0]"),
        "Schedule table verify results -- good[500] bad[0] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateScheduleData_Test_ValidEntryResult */

void SCH_ValidateScheduleData_Test_EnableStateOther(void)
{
    int32   Result;
    int32   TableIndex;

    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        SCH_AppData.ScheduleTable[TableIndex].EnableState = 99;
        SCH_AppData.ScheduleTable[TableIndex].Type = SCH_ACTIVITY_SEND_MSG;
        SCH_AppData.ScheduleTable[TableIndex].Frequency = 99;
        SCH_AppData.ScheduleTable[TableIndex].Remainder = 1;
        SCH_AppData.ScheduleTable[TableIndex].MessageIndex = 0;
        SCH_AppData.ScheduleTable[TableIndex].GroupData = SCH_UNUSED;
    }

    /* Execute the function being tested */
    Result = SCH_ValidateScheduleData(&SCH_AppData.ScheduleTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR, "Schedule tbl verify error - idx[0] ena[99] typ[1] fre[99] rem[1] msg[0] grp[0x00000000]"),
        "Schedule tbl verify error - idx[0] ena[99] typ[1] fre[99] rem[1] msg[0] grp[0x00000000]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG, "Schedule table verify results -- good[0] bad[500] unused[0]"),
        "Schedule table verify results -- good[0] bad[500] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_SDT_BAD_ENABLE_STATE, "Result == SCH_SDT_BAD_ENABLE_STATE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateScheduleData_Test_EnableStateOther */

void SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry(void)
{
    int32   Result;

    SCH_AppData.MessageTable[0].MessageBuffer[0] = 99;

    /* Set to satisfy condition "MessageID == SCH_UNUSED_MID" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SCH_UNUSED_MID, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set message length to a constant value */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Set to pass condition "*UserDataPtr != SCH_UNUSED" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETUSERDATA_INDEX, (int32)(&SCH_AppData.MessageTable[0].MessageBuffer[0]), 1);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x1897] len[10] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x1897] len[10] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[1] unused[127]"),
        "Message tbl verify results - good[0] bad[1] unused[127]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_GARBAGE_ENTRY, "Result == SCH_MDT_GARBAGE_ENTRY");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry */

void SCH_ValidateMessageData_Test_MessageIdUnusedValid(void)
{
    int32   Result;

    /* Set to satisfy condition "MessageID == SCH_UNUSED_MID" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, SCH_UNUSED_MID, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set message length to a constant value */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[0] unused[128]"),
        "Message tbl verify results - good[0] bad[0] unused[128]");

    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateMessageData_Test_MessageIdUnusedValid */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength > (SCH_MAX_MSG_WORDS * 2))" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MAX_MSG_WORDS * 2) + 2, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x1FFF] len[130] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x1FFF] len[130] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_INVALID_LENGTH, "Result == SCH_MDT_INVALID_LENGTH");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength < (SCH_MIN_MSG_WORDS * 2))" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) - 2, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x1FFF] len[6] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x1FFF] len[6] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_INVALID_LENGTH, "Result == SCH_MDT_INVALID_LENGTH");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength & 1) != 0)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) + 11, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x1FFF] len[19] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x1FFF] len[19] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_INVALID_LENGTH, "Result == SCH_MDT_INVALID_LENGTH");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd */

void SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid(void)
{
    int32   Result;

    /* Set to satisfy condition "(MessageID <= MaxValue) && (MessageID >= MinValue)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to fail all condtions in the if-statement for MessageLength */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) + 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[128] bad[0] unused[0]"),
        "Message tbl verify results - good[128] bad[0] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifySuccessCount == 1, "SCH_AppData.TableVerifySuccessCount == 1");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid */

void SCH_ValidateMessageData_Test_MessageIdOther(void)
{
    int32   Result;

    /* Set to fail all if-condtions on MessageID */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETMSGID_INDEX, (CFE_SB_MsgId_t)(SCH_MDT_MAX_MSG_ID + 1), 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETMSGID_INDEX);

    /* Set to pass condition "(MessageLength & 1) != 0)" */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, (SCH_MIN_MSG_WORDS * 2) + 10, 1);
    Ut_CFE_SB_ContinueReturnCodeAfterCountZero(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX);

    /* Execute the function being tested */
    Result = SCH_ValidateMessageData(&SCH_AppData.MessageTable[0]);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR, "Message tbl verify err - idx[0] mid[0x2000] len[18] buf[0]"),
        "Message tbl verify err - idx[0] mid[0x2000] len[18] buf[0]");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG, "Message tbl verify results - good[0] bad[128] unused[0]"),
        "Message tbl verify results - good[0] bad[128] unused[0]");

    UtAssert_True (SCH_AppData.TableVerifyFailureCount == 1, "SCH_AppData.TableVerifyFailureCount == 1");

    UtAssert_True (Result == SCH_MDT_BAD_MSG_ID, "Result == SCH_MDT_BAD_MSG_ID");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end SCH_ValidateMessageData_Test_MessageIdOther */

void SCH_App_Test_AddTestCases(void)
{
    UtTest_Add(SCH_AppMain_Test_RegisterAppError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_RegisterAppError");
    UtTest_Add(SCH_AppMain_Test_AppInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_AppInitError");
    UtTest_Add(SCH_AppMain_Test_CustomLateInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_CustomLateInitError");
    UtTest_Add(SCH_AppMain_Test_NoisyMajorFrameError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_NoisyMajorFrameError");
    UtTest_Add(SCH_AppMain_Test_NominalIgnoreMajorFrameFalse, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_NominalIgnoreMajorFrameFalse");
    UtTest_Add(SCH_AppMain_Test_ProcessScheduleTableError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_ProcessScheduleTableError");
    UtTest_Add(SCH_AppMain_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppMain_Test_Nominal");

    UtTest_Add(SCH_AppInit_Test_GetAppIDError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_GetAppIDError");
    UtTest_Add(SCH_AppInit_Test_EvsInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_EvsInitError");
    UtTest_Add(SCH_AppInit_Test_SbInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_SbInitError");
    UtTest_Add(SCH_AppInit_Test_TblInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_TblInitError");
    UtTest_Add(SCH_AppInit_Test_TimerInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_TimerInitError");
    UtTest_Add(SCH_AppInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppInit_Test_Nominal");

    UtTest_Add(SCH_EvsInit_Test_RegisterError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EvsInit_Test_RegisterError");
    UtTest_Add(SCH_EvsInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EvsInit_Test_Nominal");

    UtTest_Add(SCH_SbInit_Test_CreatePipeError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_CreatePipeError");
    UtTest_Add(SCH_SbInit_Test_SubscribeHKError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_SubscribeHKError");
    UtTest_Add(SCH_SbInit_Test_SubscribeGNDError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_SubscribeGNDError");
    UtTest_Add(SCH_SbInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SbInit_Test_Nominal");

    UtTest_Add(SCH_TblInit_Test_RegisterSdtError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TblInit_Test_RegisterSdtError");
    UtTest_Add(SCH_TblInit_Test_RegisterMdtError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TblInit_Test_RegisterMdtError");
    UtTest_Add(SCH_TblInit_Test_LoadSdtError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TblInit_Test_LoadSdtError");
    UtTest_Add(SCH_TblInit_Test_LoadMdtError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TblInit_Test_LoadMdtError");
    UtTest_Add(SCH_TblInit_Test_AcquirePointersError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TblInit_Test_AcquirePointersError");
    UtTest_Add(SCH_TblInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TblInit_Test_Nominal");

    UtTest_Add(SCH_TimerInit_Test_CustomEarlyInitError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_CustomEarlyInitError");
    UtTest_Add(SCH_TimerInit_Test_TimerAccuracyWarning, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_TimerAccuracyWarning");
    UtTest_Add(SCH_TimerInit_Test_BinSemCreateError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_BinSemCreateError");
    UtTest_Add(SCH_TimerInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_TimerInit_Test_Nominal");

    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCount2LastProcessCount1, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCount2LastProcessCount1");
    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountNotSame");
    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCountTotalSlotsLastProcessCountSame");
    UtTest_Add(SCH_ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessScheduleTable_Test_ProcessCountOtherAndNoRollover");
    UtTest_Add
        (SCH_ProcessScheduleTable_Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands, SCH_Test_Setup, SCH_Test_TearDown,
        "SCH_ProcessScheduleTable_Test_SkippedSlotsErrorIncrementTablePassCountAndCallProcessCommands");
    UtTest_Add
        (SCH_ProcessScheduleTable_Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup, SCH_Test_Setup, SCH_Test_TearDown,
        "SCH_ProcessScheduleTable_Test_MultiSlotsProcessCountTooLargeSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup");
    UtTest_Add
        (SCH_ProcessScheduleTable_Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup, SCH_Test_Setup, SCH_Test_TearDown,
        "SCH_ProcessScheduleTable_Test_MultiSlotsNotSynchronizedProcessCountGreaterThanMaxSlotsPerWakeup");

    UtTest_Add(SCH_ProcessNextSlot_Test_ProcessCommandsRollover, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextSlot_Test_ProcessCommandsRollover");
    UtTest_Add(SCH_ProcessNextSlot_Test_DoNotProcessCommandsNoRollover, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextSlot_Test_DoNotProcessCommandsNoRollover");

    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptMessageIndex, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptMessageIndex");
    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptFrequency, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptFrequency");
    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptType, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptType");
    UtTest_Add(SCH_ProcessNextEntry_Test_CorruptRemainder, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_CorruptRemainder");
    UtTest_Add(SCH_ProcessNextEntry_Test_Success, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_Success");
    UtTest_Add(SCH_ProcessNextEntry_Test_PacketSendError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessNextEntry_Test_PacketSendError");

    UtTest_Add(SCH_ProcessCommands_Test_OneMessage, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_OneMessage");
    UtTest_Add(SCH_ProcessCommands_Test_NoMessage, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_NoMessage");
    UtTest_Add(SCH_ProcessCommands_Test_RcvMsgError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_RcvMsgError");
    UtTest_Add(SCH_ProcessCommands_Test_AppPipeError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ProcessCommands_Test_AppPipeError");

    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageFrequency, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageFrequency");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageRemainder, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageRemainder");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageGroupData, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageGroupData");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageType, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageType");
    UtTest_Add(SCH_ValidateScheduleData_Test_GarbageMessageIndex, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_GarbageMessageIndex");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateUnusedAllFieldsUnused, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateUnusedAllFieldsUnused");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateEnabledFrequencyUnused");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateDisabledFrequencyUnused");
    UtTest_Add(SCH_ValidateScheduleData_Test_BadRemainder, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_BadRemainder");
    UtTest_Add(SCH_ValidateScheduleData_Test_BadActivity, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_BadActivity");
    UtTest_Add(SCH_ValidateScheduleData_Test_MsgIndexZero, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_MsgIndexZero");
    UtTest_Add(SCH_ValidateScheduleData_Test_MsgIndexTooHigh, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_MsgIndexTooHigh");
    UtTest_Add(SCH_ValidateScheduleData_Test_ValidEntryResult, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_ValidEntryResult");
    UtTest_Add(SCH_ValidateScheduleData_Test_EnableStateOther, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateScheduleData_Test_EnableStateOther");

    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdUnusedGarbageEntry");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdUnusedValid, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdUnusedValid");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooHigh");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthTooLow");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthOdd");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdValidRangeLengthValid");
    UtTest_Add(SCH_ValidateMessageData_Test_MessageIdOther, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ValidateMessageData_Test_MessageIdOther");

} /* end SCH_App_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
