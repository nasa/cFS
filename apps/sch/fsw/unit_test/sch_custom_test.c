 /*************************************************************************
 ** File:
 **   $Id: sch_custom_test.c 1.3 2017/06/21 15:29:03EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains unit test cases for the functions contained in the file sch_custom.c
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

#include "sch_custom_test.h"
#include "sch_custom.h"
#include "sch_app.h"
#include "sch_msg.h"
#include "sch_msgdefs.h"
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
void SCH_CustomEarlyInit_Test(void)
{
    int32   Result;

    /* Execute the function being tested */
    Result = SCH_CustomEarlyInit();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomEarlyInit_Test */

void SCH_CustomLateInit_Test_Nominal(void)
{
    int32   Result;

    /* Execute the function being tested */
    Result = SCH_CustomLateInit();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomLateInit_Test_Nominal */

void SCH_CustomLateInit_Test_RegisterSynchCallbackError(void)
{
    int32   Result;

    /* Set to make function under test return -1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_CustomLateInit();
    
    /* Verify results */
    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomLateInit_Test_RegisterSynchCallbackError */

void SCH_CustomLateInit_Test_TimerSetError(void)
{
    int32   Result;

    /* Set to make function under test return -1 */
#ifndef CFE_PSP_MAX_TIMERS
    Ut_OSTIMER_SetReturnCode(UT_OSTIMER_SET_INDEX, -1, 1);
#else
    Ut_CFE_PSP_SetReturnCode(UT_CFE_PSP_TIMERSET_INDEX, -1, 1);
#endif

    /* Execute the function being tested */
    Result = SCH_CustomLateInit();
    
    /* Verify results */
    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomLateInit_Test_TimerSetError */

void SCH_CustomGetCurrentSlotNumber_Test_LowCurrentSlot(void)
{
    int32   Result;

    SCH_AppData.SyncToMET       = 99;
    SCH_AppData.LastSyncMETSlot = 10;

    /* Execute the function being tested */
    Result = SCH_CustomGetCurrentSlotNumber();
    
    /* Verify results */
    UtAssert_True (Result == SCH_TOTAL_SLOTS - 10, "Result == SCH_TOTAL_SLOTS - 10");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomGetCurrentSlotNumber_Test_LowCurrentSlot */

void SCH_CustomGetCurrentSlotNumber_Test_HighCurrentSlot(void)
{
    int32   Result;

    SCH_AppData.SyncToMET       = 99;
    SCH_AppData.LastSyncMETSlot = 0;

    /* Set to make SCH_GetMETSlotNumber return 1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD, 1);

    /* Execute the function being tested */
    Result = SCH_CustomGetCurrentSlotNumber();
    
    /* Verify results */
    UtAssert_True (Result == 1, "Result == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomGetCurrentSlotNumber_Test_HighCurrentSlot */

void SCH_CustomGetCurrentSlotNumber_Test_NotSynchronized(void)
{
    int32   Result;

    SCH_AppData.SyncToMET            = SCH_NOT_SYNCHRONIZED;
    SCH_AppData.MinorFramesSinceTone = 10;

    /* Execute the function being tested */
    Result = SCH_CustomGetCurrentSlotNumber();
    
    /* Verify results */
    UtAssert_True (Result == 10, "Result == 10");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomGetCurrentSlotNumber_Test_NotSynchronized */

void SCH_CustomCleanup_Test(void)
{
    /* Execute the function being tested */
    SCH_CustomCleanup();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_CustomCleanup_Test */

void SCH_GetMETSlotNumber_Test_Rollover(void)
{
    uint32   Result;

    /* Set to make function under test return 0 by rollover */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD * SCH_TOTAL_SLOTS, 1);

    /* Execute the function being tested */
    Result = SCH_GetMETSlotNumber();
    
    /* Verify results */
    UtAssert_True (Result == 0, "Result == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_GetMETSlotNumber_Test_Rollover */

void SCH_GetMETSlotNumber_Test_NoRollover(void)
{
    uint32   Result;

    /* Set to make function under test return 1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD, 1);

    /* Execute the function being tested */
    Result = SCH_GetMETSlotNumber();
    
    /* Verify results */
    UtAssert_True (Result == 1, "Result == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_GetMETSlotNumber_Test_NoRollover */

void SCH_MajorFrameCallback_Test_NoisyNotSynchronized(void)
{
    SCH_AppData.SyncToMET                    = SCH_NOT_SYNCHRONIZED;
    SCH_AppData.MinorFramesSinceTone         = 1;
    SCH_AppData.IgnoreMajorFrame             = FALSE;
    SCH_AppData.ConsecutiveNoisyFrameCounter = SCH_MAX_NOISY_MAJORF - 1;

    /* Set to make SCH_GetMETSlotNumber return 1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD, 1);

    /* Execute the function being tested */
    SCH_MajorFrameCallback();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.UnexpectedMajorFrame == TRUE, "SCH_AppData.UnexpectedMajorFrame == TRUE");
    UtAssert_True (SCH_AppData.UnexpectedMajorFrameCount == 1, "SCH_AppData.UnexpectedMajorFrameCount == 1");
    UtAssert_True (SCH_AppData.ConsecutiveNoisyFrameCounter == SCH_MAX_NOISY_MAJORF, "SCH_AppData.ConsecutiveNoisyFrameCounter == SCH_MAX_NOISY_MAJORF");
    UtAssert_True (SCH_AppData.IgnoreMajorFrame == TRUE, "SCH_AppData.IgnoreMajorFrame == TRUE");
    UtAssert_True (SCH_AppData.LastSyncMETSlot == 1, "SCH_AppData.LastSyncMETSlot == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MajorFrameCallback_Test_NoisyNotSynchronized */

void SCH_MajorFrameCallback_Test_NoisySynchronized(void)
{
    SCH_AppData.SyncToMET                    = SCH_MINOR_SYNCHRONIZED;
    SCH_AppData.NextSlotNumber               = 1;
    SCH_AppData.IgnoreMajorFrame             = FALSE;
    SCH_AppData.ConsecutiveNoisyFrameCounter = SCH_MAX_NOISY_MAJORF - 1;

    /* Set to make SCH_GetMETSlotNumber return 1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD, 1);

    /* Execute the function being tested */
    SCH_MajorFrameCallback();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.UnexpectedMajorFrame == TRUE, "SCH_AppData.UnexpectedMajorFrame == TRUE");
    UtAssert_True (SCH_AppData.UnexpectedMajorFrameCount == 1, "SCH_AppData.UnexpectedMajorFrameCount == 1");
    UtAssert_True (SCH_AppData.ConsecutiveNoisyFrameCounter == SCH_MAX_NOISY_MAJORF, "SCH_AppData.ConsecutiveNoisyFrameCounter == SCH_MAX_NOISY_MAJORF");
    UtAssert_True (SCH_AppData.IgnoreMajorFrame == TRUE, "SCH_AppData.IgnoreMajorFrame == TRUE");
    UtAssert_True (SCH_AppData.LastSyncMETSlot == 1, "SCH_AppData.LastSyncMETSlot == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MajorFrameCallback_Test_NoisySynchronized */

void SCH_MajorFrameCallback_Test_FrameOccurredWhenExpected(void)
{
    SCH_AppData.SyncToMET                    = 99;
    SCH_AppData.IgnoreMajorFrame             = FALSE;
    SCH_AppData.ConsecutiveNoisyFrameCounter = 1;

    /* Set to make SCH_GetMETSlotNumber return 1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD, 1);

    /* Execute the function being tested */
    SCH_MajorFrameCallback();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.UnexpectedMajorFrame == FALSE, "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True (SCH_AppData.ConsecutiveNoisyFrameCounter == 0, "SCH_AppData.ConsecutiveNoisyFrameCounter == 0");
    UtAssert_True (SCH_AppData.ValidMajorFrameCount == 1, "SCH_AppData.ValidMajorFrameCount == 1");
    UtAssert_True (SCH_AppData.MinorFramesSinceTone == 0, "SCH_AppData.MinorFramesSinceTone == 0");
    UtAssert_True (SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_CFE_TIME, "MajorFrameSource == SCH_MAJOR_FS_CFE_TIME");
    UtAssert_True (SCH_AppData.SyncToMET == (SCH_AppData.SyncToMET & SCH_MINOR_SYNCHRONIZED), "SCH_AppData.SyncToMET == (SCH_AppData.SyncToMET & SCH_MINOR_SYNCHRONIZED)");
    UtAssert_True (SCH_AppData.LastSyncMETSlot == 1, "SCH_AppData.LastSyncMETSlot == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MajorFrameCallback_Test_FrameOccurredWhenExpected */

#define CFE_TIME_FLAG_FLYING   1
void SCH_MajorFrameCallback_Test_FlywheelMode(void)
{
    /* Set to make fail condition "(StateFlags & CFE_TIME_FLAG_FLYING) == 0" */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_GETCLOCKINFO_INDEX, CFE_TIME_FLAG_FLYING, 1);

    /* Set to make SCH_GetMETSlotNumber return 1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD, 1);

    /* Execute the function being tested */
    SCH_MajorFrameCallback();
    
    /* Verify results */
    UtAssert_True (SCH_AppData.LastSyncMETSlot == 1, "SCH_AppData.LastSyncMETSlot == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MajorFrameCallback_Test_FlywheelMode */

void SCH_MinorFrameCallback_Test_SyncAttemptsLeft(void)
{
    uint32 TimerId = 1;

    SCH_AppData.MajorFrameSource = SCH_MAJOR_FS_NONE;

    /* Set to make SCH_GetMETSlotNumber return 1 */
    Ut_CFE_TIME_SetReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX, SCH_NORMAL_SLOT_PERIOD, 1);

    /* Execute the function being tested */
    SCH_MinorFrameCallback(TimerId);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_MINOR_FRAME_TIMER, "SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_MINOR_FRAME_TIMER");
    UtAssert_True(SCH_AppData.SyncToMET == SCH_PENDING_MAJOR_SYNCH, "SCH_AppData.SyncToMET == SCH_PENDING_MAJOR_SYNCH");
    UtAssert_True(SCH_AppData.SyncAttemptsLeft == SCH_MAX_SYNC_ATTEMPTS - 1, "SCH_AppData.SyncAttemptsLeft == SCH_MAX_SYNC_ATTEMPTS - 1");
    UtAssert_True(SCH_AppData.LastSyncMETSlot == 0, "SCH_AppData.LastSyncMETSlot == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MinorFrameCallback_Test_SyncAttemptsLeft */

void SCH_MinorFrameCallback_Test_SynchronizationAchievedNominal(void)
{
    uint32 TimerId = 1;

    SCH_AppData.MajorFrameSource = SCH_MAJOR_FS_NONE;

    /* Execute the function being tested */
    SCH_MinorFrameCallback(TimerId);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_MINOR_FRAME_TIMER, "SCH_AppData.MajorFrameSource == SCH_MAJOR_FS_MINOR_FRAME_TIMER");
    UtAssert_True(SCH_AppData.SyncAttemptsLeft == SCH_MAX_SYNC_ATTEMPTS - 1, "SCH_AppData.SyncAttemptsLeft == SCH_MAX_SYNC_ATTEMPTS - 1");
    UtAssert_True(SCH_AppData.SyncToMET == SCH_MAJOR_SYNCHRONIZED, "SCH_AppData.SyncToMET == SCH_MAJOR_SYNCHRONIZED");
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == 0, "SCH_AppData.MinorFramesSinceTone == 0");
    UtAssert_True(SCH_AppData.LastSyncMETSlot == 0, "SCH_AppData.LastSyncMETSlot == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MinorFrameCallback_Test_SynchronizationAchievedNominal */

void SCH_MinorFrameCallback_Test_AlreadySynchronizedNominal(void)
{
    uint32 TimerId = 1;

    SCH_AppData.MajorFrameSource = 99;

    /* Execute the function being tested */
    SCH_MinorFrameCallback(TimerId);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == 1, "SCH_AppData.MinorFramesSinceTone == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MinorFrameCallback_Test_AlreadySynchronizedNominal */

void SCH_MinorFrameCallback_Test_AlreadySynchronizedRollover(void)
{
    uint32 TimerId = 1;

    SCH_AppData.MajorFrameSource     = 99;
    SCH_AppData.MinorFramesSinceTone = SCH_TOTAL_SLOTS;

    /* Execute the function being tested */
    SCH_MinorFrameCallback(TimerId);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == 0, "SCH_AppData.MinorFramesSinceTone == 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount == 1, "SCH_AppData.MissedMajorFrameCount == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MinorFrameCallback_Test_AlreadySynchronizedRollover */

void SCH_MinorFrameCallback_Test_AlreadySynchronizedStartTimer(void)
{
    uint32 TimerId = 1;

    SCH_AppData.MajorFrameSource     = 99;
    SCH_AppData.MinorFramesSinceTone = SCH_TIME_SYNC_SLOT - 1;

    /* Execute the function being tested */
    SCH_MinorFrameCallback(TimerId);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT, "SCH_AppData.MinorFramesSinceTone == SCH_TIME_SYNC_SLOT");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_MinorFrameCallback_Test_AlreadySynchronizedStartTimer */

void SCH_Custom_Test_AddTestCases(void)
{
    UtTest_Add(SCH_CustomEarlyInit_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomEarlyInit_Test");

    UtTest_Add(SCH_CustomLateInit_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomLateInit_Test_Nominal");
    UtTest_Add(SCH_CustomLateInit_Test_RegisterSynchCallbackError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomLateInit_Test_RegisterSynchCallbackError");
    UtTest_Add(SCH_CustomLateInit_Test_TimerSetError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomLateInit_Test_TimerSetError");

    UtTest_Add(SCH_CustomGetCurrentSlotNumber_Test_LowCurrentSlot, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomGetCurrentSlotNumber_Test_LowCurrentSlot");
    UtTest_Add(SCH_CustomGetCurrentSlotNumber_Test_HighCurrentSlot, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomGetCurrentSlotNumber_Test_HighCurrentSlot");
    UtTest_Add(SCH_CustomGetCurrentSlotNumber_Test_NotSynchronized, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomGetCurrentSlotNumber_Test_NotSynchronized");

    UtTest_Add(SCH_CustomCleanup_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_CustomCleanup_Test");

    UtTest_Add(SCH_GetMETSlotNumber_Test_Rollover, SCH_Test_Setup, SCH_Test_TearDown, "SCH_GetMETSlotNumber_Test_Rollover");
    UtTest_Add(SCH_GetMETSlotNumber_Test_NoRollover, SCH_Test_Setup, SCH_Test_TearDown, "SCH_GetMETSlotNumber_Test_NoRollover");

    UtTest_Add(SCH_MajorFrameCallback_Test_NoisyNotSynchronized, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MajorFrameCallback_Test_NoisyNotSynchronized");
    UtTest_Add(SCH_MajorFrameCallback_Test_NoisySynchronized, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MajorFrameCallback_Test_NoisySynchronized");
    UtTest_Add(SCH_MajorFrameCallback_Test_FrameOccurredWhenExpected, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MajorFrameCallback_Test_FrameOccurredWhenExpected");
    UtTest_Add(SCH_MajorFrameCallback_Test_FlywheelMode, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MajorFrameCallback_Test_FlywheelMode");

    UtTest_Add(SCH_MinorFrameCallback_Test_SyncAttemptsLeft, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MinorFrameCallback_Test_SyncAttemptsLeft");
    UtTest_Add(SCH_MinorFrameCallback_Test_SynchronizationAchievedNominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MinorFrameCallback_Test_SynchronizationAchievedNominal");
    UtTest_Add(SCH_MinorFrameCallback_Test_AlreadySynchronizedNominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MinorFrameCallback_Test_AlreadySynchronizedNominal");
    UtTest_Add(SCH_MinorFrameCallback_Test_AlreadySynchronizedRollover, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MinorFrameCallback_Test_AlreadySynchronizedRollover");
    UtTest_Add(SCH_MinorFrameCallback_Test_AlreadySynchronizedStartTimer, SCH_Test_Setup, SCH_Test_TearDown, "SCH_MinorFrameCallback_Test_AlreadySynchronizedStartTimer");

} /* end SCH_Custom_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
