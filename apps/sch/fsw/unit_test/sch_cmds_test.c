 /*************************************************************************
 ** File:
 **   $Id: sch_cmds_test.c 1.2 2017/06/21 15:29:00EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains unit test cases for the functions contained in the file sch_cmds.c
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

#include "sch_cmds_test.h"
#include "sch_cmds.h"
#include "sch_app.h"
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

void SCH_AppPipe_Test_SendHK(void)
{
    int32             Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_SEND_HK_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    /* Set to make SCH_HousekeepingCmd return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_AppPipe_Test_SendHK */

void SCH_AppPipe_Test_Noop(void)
{
    int32             Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_NOOP_CC);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_Noop */

void SCH_AppPipe_Test_Reset(void)
{
    int32             Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_RESET_CC);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_Reset */

void SCH_AppPipe_Test_Enable(void)
{
    int32            Result;
    SCH_EntryCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_ENABLE_CC);

    /* Setting these 2 values just to prevent a segmentation fault */
    CmdPacket.SlotNumber = 99;
    CmdPacket.EntryNumber = 99;

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_Enable */

void SCH_AppPipe_Test_Disable(void)
{
    int32            Result;
    SCH_EntryCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_DISABLE_CC);

    /* Setting these 2 values just to prevent a segmentation fault */
    CmdPacket.SlotNumber = 99;
    CmdPacket.EntryNumber = 99;

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_Disable */

void SCH_AppPipe_Test_EnableGroupCmd(void)
{
    int32            Result;
    SCH_GroupCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_ENABLE_GROUP_CC);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_EnableGroupCmd */

void SCH_AppPipe_Test_DisableGroupCmd(void)
{
    int32            Result;
    SCH_GroupCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_DISABLE_GROUP_CC);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_DisableGroupCmd */

void SCH_AppPipe_Test_EnableSyncCmd(void)
{
    int32             Result;
    SCH_NoArgsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_ENABLE_SYNC_CC);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_EnableSyncCmd */

void SCH_AppPipe_Test_SendDiagTlmCmd(void)
{
    int32                 Result;
    SCH_ScheduleEntry_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_ScheduleEntry_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), SCH_SEND_DIAG_TLM_CC);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_AppPipe_Test_SendDiagTlmCmd */

void SCH_AppPipe_Test_InvalidCommandCode(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), 99);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CC_ERR_EID, CFE_EVS_ERROR, "Invalid command code: ID = 0x1895, CC = 99"),
        "Invalid command code: ID = 0x1895, CC = 99");

    UtAssert_True (SCH_AppData.ErrCounter == 1, "SCH_AppData.ErrCounter == 1");
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_AppPipe_Test_InvalidCommandCode */

void SCH_AppPipe_Test_InvalidMessageID(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, 0x0099, sizeof(SCH_NoArgsCmd_t), TRUE);

    /* Execute the function being tested */
    Result = SCH_AppPipe((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_MD_ERR_EID, CFE_EVS_ERROR, "Msg with Invalid message ID Rcvd -- ID = 0x0099"),
        "Msg with Invalid message ID Rcvd -- ID = 0x0099");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_AppPipe_Test_InvalidMessageID */

void SCH_HousekeepingCmd_Test(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_SEND_HK_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    /* Set to make SCH_AcquirePointers return CFE_SUCCESS */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);

    SCH_AppData.CmdCounter = 1;
    SCH_AppData.ErrCounter = 2;
    SCH_AppData.ScheduleActivitySuccessCount = 3;
    SCH_AppData.ScheduleActivityFailureCount = 4;
    SCH_AppData.SlotsProcessedCount = 5;
    SCH_AppData.SkippedSlotsCount = 6;
    SCH_AppData.MultipleSlotsCount = 7;
    SCH_AppData.SameSlotCount = 8;
    SCH_AppData.BadTableDataCount = 9;
    SCH_AppData.TableVerifySuccessCount = 10;
    SCH_AppData.TableVerifyFailureCount = 11;
    SCH_AppData.TablePassCount = 12;
    SCH_AppData.ValidMajorFrameCount = 13;
    SCH_AppData.MissedMajorFrameCount = 14;
    SCH_AppData.UnexpectedMajorFrameCount = 15;
    SCH_AppData.MinorFramesSinceTone = 16;
    SCH_AppData.NextSlotNumber = 17;
    SCH_AppData.LastSyncMETSlot = 18;
    SCH_AppData.IgnoreMajorFrame = 19;
    SCH_AppData.UnexpectedMajorFrame = 20;
    SCH_AppData.SyncToMET = 21;
    SCH_AppData.MajorFrameSource = 22;

    /* Execute the function being tested */
    Result = SCH_HousekeepingCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.HkPacket.CmdCounter == 1, "SCH_AppData.HkPacket.CmdCounter == 1");
    UtAssert_True(SCH_AppData.HkPacket.ErrCounter == 2, "SCH_AppData.HkPacket.ErrCounter == 2");
    UtAssert_True(SCH_AppData.HkPacket.ScheduleActivitySuccessCount == 3, "SCH_AppData.HkPacket.ScheduleActivitySuccessCount == 3");
    UtAssert_True(SCH_AppData.HkPacket.ScheduleActivityFailureCount == 4, "SCH_AppData.HkPacket.ScheduleActivityFailureCount == 4");
    UtAssert_True(SCH_AppData.HkPacket.SlotsProcessedCount == 5, "SCH_AppData.HkPacket.SlotsProcessedCount == 5");
    UtAssert_True(SCH_AppData.HkPacket.SkippedSlotsCount == 6, "SCH_AppData.HkPacket.SkippedSlotsCount == 6");
    UtAssert_True(SCH_AppData.HkPacket.MultipleSlotsCount == 7, "SCH_AppData.HkPacket.MultipleSlotsCount == 7");
    UtAssert_True(SCH_AppData.HkPacket.SameSlotCount == 8, "SCH_AppData.HkPacket.SameSlotCount == 8");
    UtAssert_True(SCH_AppData.HkPacket.BadTableDataCount == 9, "SCH_AppData.HkPacket.BadTableDataCount == 9");
    UtAssert_True(SCH_AppData.HkPacket.TableVerifySuccessCount == 10, "SCH_AppData.HkPacket.TableVerifySuccessCount == 10");
    UtAssert_True(SCH_AppData.HkPacket.TableVerifyFailureCount == 11, "SCH_AppData.HkPacket.TableVerifyFailureCount == 11");
    UtAssert_True(SCH_AppData.HkPacket.TablePassCount == 12, "SCH_AppData.HkPacket.TablePassCount == 12");
    UtAssert_True(SCH_AppData.HkPacket.ValidMajorFrameCount == 13, "SCH_AppData.HkPacket.ValidMajorFrameCount == 13");
    UtAssert_True(SCH_AppData.HkPacket.MissedMajorFrameCount == 14, "SCH_AppData.HkPacket.MissedMajorFrameCount == 14");
    UtAssert_True(SCH_AppData.HkPacket.UnexpectedMajorFrameCount == 15, "SCH_AppData.HkPacket.UnexpectedMajorFrameCount == 15");
    UtAssert_True(SCH_AppData.HkPacket.MinorFramesSinceTone == 16, "SCH_AppData.HkPacket.MinorFramesSinceTone == 16");
    UtAssert_True(SCH_AppData.HkPacket.NextSlotNumber == 17, "SCH_AppData.HkPacket.NextSlotNumber == 17");
    UtAssert_True(SCH_AppData.HkPacket.LastSyncMETSlot == 18, "SCH_AppData.HkPacket.LastSyncMETSlot == 18");
    UtAssert_True(SCH_AppData.HkPacket.IgnoreMajorFrame == 19, "SCH_AppData.HkPacket.IgnoreMajorFrame == 19");
    UtAssert_True(SCH_AppData.HkPacket.UnexpectedMajorFrame == 20, "SCH_AppData.HkPacket.UnexpectedMajorFrame == 20");
    UtAssert_True(SCH_AppData.HkPacket.SyncToMET == 21, "SCH_AppData.HkPacket.SyncToMET == 21");
    UtAssert_True(SCH_AppData.HkPacket.MajorFrameSource == 22, "SCH_AppData.HkPacket.MajorFrameSource == 22");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_HousekeepingCmd_Test */

void SCH_NoopCmd_Test_Error(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_NoopCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (SCH_AppData.ErrCounter == 1, "SCH_AppData.ErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_NoopCmd_Test_Error */

void SCH_NoopCmd_Test_Nominal(void)
{
    SCH_NoArgsCmd_t  CmdPacket;
    char             Message[125];

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    /* Execute the function being tested */
    SCH_NoopCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    sprintf(Message, "NO-op command. Version %d.%d.%d.%d", SCH_MAJOR_VERSION, SCH_MINOR_VERSION, SCH_REVISION, SCH_MISSION_REV);
    UtAssert_True (Ut_CFE_EVS_EventSent(SCH_NOOP_CMD_EID, CFE_EVS_INFORMATION, Message), Message);

    UtAssert_True (SCH_AppData.CmdCounter == 1, "SCH_AppData.CmdCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_NoopCmd_Test_Nominal */

void SCH_ResetCmd_Test_Error(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_ResetCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (SCH_AppData.ErrCounter == 1, "SCH_AppData.ErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_ResetCmd_Test_Error */

void SCH_ResetCmd_Test_Nominal(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_ResetCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (SCH_AppData.CmdCounter == 0, "SCH_AppData.CmdCounter == 0");
    UtAssert_True (SCH_AppData.ErrCounter == 0, "SCH_AppData.ErrCounter == 0");

    UtAssert_True(SCH_AppData.ScheduleActivitySuccessCount = 0, "SCH_AppData.ScheduleActivitySuccessCount = 0");
    UtAssert_True(SCH_AppData.ScheduleActivityFailureCount = 0, "SCH_AppData.ScheduleActivityFailureCount = 0");
    UtAssert_True(SCH_AppData.SlotsProcessedCount = 0, "SCH_AppData.SlotsProcessedCount = 0");
    UtAssert_True(SCH_AppData.SkippedSlotsCount   = 0, "SCH_AppData.SkippedSlotsCount   = 0");
    UtAssert_True(SCH_AppData.MultipleSlotsCount  = 0, "SCH_AppData.MultipleSlotsCount  = 0");
    UtAssert_True(SCH_AppData.SameSlotCount       = 0, "SCH_AppData.SameSlotCount       = 0");
    UtAssert_True(SCH_AppData.BadTableDataCount   = 0, "SCH_AppData.BadTableDataCount   = 0");
    UtAssert_True(SCH_AppData.TableVerifySuccessCount = 0, "SCH_AppData.TableVerifySuccessCount = 0");
    UtAssert_True(SCH_AppData.TableVerifyFailureCount = 0, "SCH_AppData.TableVerifyFailureCount = 0");
    UtAssert_True(SCH_AppData.ValidMajorFrameCount      = 0, "SCH_AppData.ValidMajorFrameCount      = 0");
    UtAssert_True(SCH_AppData.MissedMajorFrameCount     = 0, "SCH_AppData.MissedMajorFrameCount     = 0");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrameCount = 0, "SCH_AppData.UnexpectedMajorFrameCount = 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_RESET_CMD_EID, CFE_EVS_DEBUG, "RESET command"),
        "RESET command");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_ResetCmd_Test_Nominal */

void SCH_EnableCmd_Test_InvalidCmdLength(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_EnableCmd_Test_InvalidCmdLength */

void SCH_EnableCmd_Test_InvalidArgumentSlotNumber(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = SCH_TOTAL_SLOTS;
    CmdPacket.EntryNumber = 1;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, "ENABLE cmd: invalid argument, slot=100 (<100), entry=1 (<5)"),
        "ENABLE cmd: invalid argument, slot=100 (<100), entry=1 (<5)");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_InvalidArgumentSlotNumber */

void SCH_EnableCmd_Test_InvalidArgumentEntryNumber(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 1;
    CmdPacket.EntryNumber = SCH_ENTRIES_PER_SLOT;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, "ENABLE cmd: invalid argument, slot=1 (<100), entry=5 (<5)"),
        "ENABLE cmd: invalid argument, slot=1 (<100), entry=5 (<5)");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_InvalidArgumentEntryNumber */

void SCH_EnableCmd_Test_InvalidState(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = 99;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENABLE_CMD_ENTRY_ERR_EID, CFE_EVS_ERROR, "ENABLE command: invalid state = 99, slot = 0, entry = 0"),
        "ENABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_InvalidState */

void SCH_EnableCmd_Test_Nominal(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_EnableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED, "SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENABLE_CMD_EID, CFE_EVS_DEBUG, "ENABLE command: slot = 0, entry = 0"),
        "ENABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableCmd_Test_Nominal */

void SCH_DisableCmd_Test_InvalidCmdLength(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_DisableCmd_Test_InvalidCmdLength */

void SCH_DisableCmd_Test_InvalidArgumentSlotNumber(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = SCH_TOTAL_SLOTS;
    CmdPacket.EntryNumber = 1;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DISABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, "DISABLE cmd: invalid argument, slot=100 (<100), entry=1 (<5)"),
        "DISABLE cmd: invalid argument, slot=100 (<100), entry=1 (<5)");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_InvalidArgumentSlotNumber */

void SCH_DisableCmd_Test_InvalidArgumentEntryNumber(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 1;
    CmdPacket.EntryNumber = SCH_ENTRIES_PER_SLOT;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DISABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR, "DISABLE cmd: invalid argument, slot=1 (<100), entry=5 (<5)"),
        "DISABLE cmd: invalid argument, slot=1 (<100), entry=5 (<5)");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_InvalidArgumentEntryNumber */

void SCH_DisableCmd_Test_InvalidState(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = 99;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DISABLE_CMD_ENTRY_ERR_EID, CFE_EVS_ERROR, "DISABLE command: invalid state = 99, slot = 0, entry = 0"),
        "DISABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_InvalidState */

void SCH_DisableCmd_Test_Nominal(void)
{
    SCH_EntryCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_EntryCmd_t), TRUE);

    CmdPacket.SlotNumber  = 0;
    CmdPacket.EntryNumber = 0;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_DisableCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED, "SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DISABLE_CMD_EID, CFE_EVS_DEBUG, "DISABLE command: slot = 0, entry = 0"),
        "DISABLE command: invalid state = 99, slot = 0, entry = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableCmd_Test_Nominal */

void SCH_EnableGroupCmd_Test_InvalidCmdLength(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_EnableGroupCmd_Test_InvalidCmdLength */

void SCH_EnableGroupCmd_Test_InvalidArgument(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_CMD_ERR_EID, CFE_EVS_ERROR, "ENABLE GROUP command: invalid argument, no groups selected"),
        "ENABLE GROUP command: invalid argument, no groups selected");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_InvalidArgument */

void SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    CmdPacket.GroupData = SCH_GROUP_NUMBER_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_GROUP_NUMBER_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED, "SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_CMD_EID, CFE_EVS_DEBUG, "ENABLE GROUP command: match count = 1"),
        "ENABLE GROUP command: match count = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused */

void SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    CmdPacket.GroupData = SCH_MULTI_GROUP_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_MULTI_GROUP_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED, "SCH_AppData.ScheduleTable[0].EnableState == SCH_ENABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_CMD_EID, CFE_EVS_DEBUG, "ENABLE GROUP command: match count = 1"),
        "ENABLE GROUP command: match count = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused */

void SCH_EnableGroupCmd_Test_GroupNotFound(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    CmdPacket.GroupData = 0xFFFFFFFF;
    SCH_AppData.ScheduleTable[0].GroupData = 0x00000000;

    /* Execute the function being tested */
    SCH_EnableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_GRP_NOT_FOUND_ERR_EID, CFE_EVS_ERROR, "ENABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found"),
        "ENABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableGroupCmd_Test_GroupNotFound */

void SCH_DisableGroupCmd_Test_InvalidCmdLength(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_DisableGroupCmd_Test_InvalidCmdLength */

void SCH_DisableGroupCmd_Test_InvalidArgument(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_CMD_ERR_EID, CFE_EVS_ERROR, "DISABLE GROUP command: invalid argument, no groups selected"),
        "DISABLE GROUP command: invalid argument, no groups selected");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_InvalidArgument */

void SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    CmdPacket.GroupData = SCH_GROUP_NUMBER_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_GROUP_NUMBER_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED, "SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_CMD_EID, CFE_EVS_DEBUG, "DISABLE GROUP command: match count = 1"),
        "DISABLE GROUP command: match count = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused */

void SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    CmdPacket.GroupData = SCH_MULTI_GROUP_BIT_MASK;
    SCH_AppData.ScheduleTable[0].GroupData = SCH_MULTI_GROUP_BIT_MASK;

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED, "SCH_AppData.ScheduleTable[0].EnableState == SCH_DISABLED");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_CMD_EID, CFE_EVS_DEBUG, "DISABLE GROUP command: match count = 1"),
        "DISABLE GROUP command: match count = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused */

void SCH_DisableGroupCmd_Test_GroupNotFound(void)
{
    SCH_GroupCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_GroupCmd_t), TRUE);

    CmdPacket.GroupData = 0xFFFFFFFF;
    SCH_AppData.ScheduleTable[0].GroupData = 0x00000000;

    /* Execute the function being tested */
    SCH_DisableGroupCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_DIS_GRP_NOT_FOUND_ERR_EID, CFE_EVS_ERROR, "DISABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found"),
        "DISABLE GROUP command: Neither Group 255 nor Multi-Group 0xFFFFFF found");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_DisableGroupCmd_Test_GroupNotFound */

void SCH_EnableSyncCmd_Test_InvalidCmdLength(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_EnableSyncCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_EnableSyncCmd_Test_InvalidCmdLength */

void SCH_EnableSyncCmd_Test_Nominal(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    /* Execute the function being tested */
    SCH_EnableSyncCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.IgnoreMajorFrame == FALSE, "SCH_AppData.IgnoreMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.UnexpectedMajorFrame == FALSE, "SCH_AppData.UnexpectedMajorFrame == FALSE");
    UtAssert_True(SCH_AppData.ConsecutiveNoisyFrameCounter == 0, "SCH_AppData.ConsecutiveNoisyFrameCounter == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_ENA_SYNC_CMD_EID, CFE_EVS_DEBUG, "Major Frame Synchronization Enabled"),
        "Major Frame Synchronization Enabled");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_EnableSyncCmd_Test_Nominal */

void SCH_SendDiagTlmCmd_Test_InvalidCmdLength(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, 1, TRUE);

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end SCH_SendDiagTlmCmd_Test_InvalidCmdLength */

void SCH_SendDiagTlmCmd_Test_Enabled(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    SCH_AppData.ScheduleTable[0].EnableState = SCH_ENABLED;

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.DiagPacket.EntryStates[0] == (1 << 14), "SCH_AppData.DiagPacket.EntryStates[0] == (1 << 14)");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG, "Transmitting Diagnostic Message"),
        "Transmitting Diagnostic Message");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SendDiagTlmCmd_Test_Enabled */

void SCH_SendDiagTlmCmd_Test_Disabled(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    SCH_AppData.ScheduleTable[0].EnableState = SCH_DISABLED;

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.DiagPacket.EntryStates[0] == (2 << 14), "SCH_AppData.DiagPacket.EntryStates[0] == (2 << 14)");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG, "Transmitting Diagnostic Message"),
        "Transmitting Diagnostic Message");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SendDiagTlmCmd_Test_Disabled */

void SCH_SendDiagTlmCmd_Test_Other(void)
{
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);

    SCH_AppData.ScheduleTable[0].EnableState = 99;
    SCH_AppData.DiagPacket.MsgIDs[0]         = 0x1111;

    /* Execute the function being tested */
    SCH_SendDiagTlmCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True(SCH_AppData.DiagPacket.MsgIDs[0] == 0x0000, "SCH_AppData.DiagPacket.MsgIDs[0] == 0x0000");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG, "Transmitting Diagnostic Message"),
        "Transmitting Diagnostic Message");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_SendDiagTlmCmd_Test_Other */

void SCH_AcquirePointers_Test_AllResultsGreaterThanSuccess(void)
{
    int32   Result;

    /* Set to satisfy condition "Result > CFE_SUCCESS", both times */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_SUCCESS + 1, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);

    /* Execute the function being tested */
    Result = SCH_AcquirePointers();
    
    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_AcquirePointers_Test_AllResultsGreaterThanSuccess */

void SCH_AcquirePointers_Test_AllResultsSuccess(void)
{
    int32   Result;

    /* Set to fail condition "Result > CFE_SUCCESS", both times */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_GETADDRESS_INDEX);

    /* Execute the function being tested */
    Result = SCH_AcquirePointers();
    
    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_AcquirePointers_Test_AllResultsSuccess */

void SCH_AcquirePointers_Test_Result1LessThanSuccess(void)
{
    int32   Result;

    /* Set to fail first instance of condition "Result > CFE_SUCCESS" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = SCH_AcquirePointers();
    
    /* Verify results */
    UtAssert_True(Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_AcquirePointers_Test_Result1LessThanSuccess */

void SCH_VerifyCmdLength_Test_LengthError(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), 1);

    /* Execute the function being tested */
    Result = SCH_VerifyCmdLength((CFE_SB_MsgPtr_t)(&CmdPacket), 99);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(SCH_CMD_LEN_ERR_EID, CFE_EVS_ERROR, "Cmd Msg with Bad length Rcvd: ID = 0x1895, CC = 1, Exp Len = 99, Len = 8"),
        "Cmd Msg with Bad length Rcvd: ID = 0x1895, CC = 1, Exp Len = 99, Len = 8");

    UtAssert_True(Result == SCH_BAD_MSG_LENGTH_RC, "Result == SCH_BAD_MSG_LENGTH_RC");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end SCH_VerifyCmdLength_Test_LengthError */

void SCH_VerifyCmdLength_Test_Success(void)
{
    int32            Result;
    SCH_NoArgsCmd_t  CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, SCH_CMD_MID, sizeof(SCH_NoArgsCmd_t), TRUE);
    CFE_SB_SetCmdCode ((CFE_SB_MsgPtr_t)(&CmdPacket), 1);

    /* Execute the function being tested */
    Result = SCH_VerifyCmdLength((CFE_SB_MsgPtr_t)(&CmdPacket), sizeof(SCH_NoArgsCmd_t));
    
    /* Verify results */
    UtAssert_True(Result == SCH_SUCCESS, "Result == SCH_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_VerifyCmdLength_Test_Success */

void SCH_PostCommandResult_Test_GoodCommand(void)
{
    boolean   GoodCommand = TRUE;

    /* Execute the function being tested */
    SCH_PostCommandResult(GoodCommand);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.CmdCounter == 1, "SCH_AppData.CmdCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_PostCommandResult_Test_GoodCommand */

void SCH_PostCommandResult_Test_Error(void)
{
    boolean   GoodCommand = FALSE;

    /* Execute the function being tested */
    SCH_PostCommandResult(GoodCommand);
    
    /* Verify results */
    UtAssert_True(SCH_AppData.ErrCounter == 1, "SCH_AppData.ErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end SCH_PostCommandResult_Test_Error */

void SCH_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(SCH_AppPipe_Test_SendHK, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_SendHK");
    UtTest_Add(SCH_AppPipe_Test_Noop, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_Noop");
    UtTest_Add(SCH_AppPipe_Test_Reset, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_Reset");
    UtTest_Add(SCH_AppPipe_Test_Enable, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_Enable");
    UtTest_Add(SCH_AppPipe_Test_Disable, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_Disable");
    UtTest_Add(SCH_AppPipe_Test_EnableGroupCmd, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_EnableGroupCmd");
    UtTest_Add(SCH_AppPipe_Test_DisableGroupCmd, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_DisableGroupCmd");
    UtTest_Add(SCH_AppPipe_Test_EnableSyncCmd, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_EnableSyncCmd");
    UtTest_Add(SCH_AppPipe_Test_SendDiagTlmCmd, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_SendDiagTlmCmd");
    UtTest_Add(SCH_AppPipe_Test_InvalidCommandCode, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_InvalidCommandCode");
    UtTest_Add(SCH_AppPipe_Test_InvalidMessageID, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AppPipe_Test_InvalidMessageID");

    UtTest_Add(SCH_HousekeepingCmd_Test, SCH_Test_Setup, SCH_Test_TearDown, "SCH_HousekeepingCmd_Test");

    UtTest_Add(SCH_NoopCmd_Test_Error, SCH_Test_Setup, SCH_Test_TearDown, "SCH_NoopCmd_Test_Error");
    UtTest_Add(SCH_NoopCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_NoopCmd_Test_Nominal");

    UtTest_Add(SCH_ResetCmd_Test_Error, SCH_Test_Setup, SCH_Test_TearDown, "SCH_ResetCmd_Test_Error");
    UtTest_Add(SCH_NoopCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_NoopCmd_Test_Nominal");

    UtTest_Add(SCH_EnableCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_EnableCmd_Test_InvalidArgumentSlotNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidArgumentSlotNumber");
    UtTest_Add(SCH_EnableCmd_Test_InvalidArgumentEntryNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidArgumentEntryNumber");
    UtTest_Add(SCH_EnableCmd_Test_InvalidState, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_InvalidState");
    UtTest_Add(SCH_EnableCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableCmd_Test_Nominal");

    UtTest_Add(SCH_DisableCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_DisableCmd_Test_InvalidArgumentSlotNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidArgumentSlotNumber");
    UtTest_Add(SCH_DisableCmd_Test_InvalidArgumentEntryNumber, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidArgumentEntryNumber");
    UtTest_Add(SCH_DisableCmd_Test_InvalidState, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_InvalidState");
    UtTest_Add(SCH_DisableCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableCmd_Test_Nominal");

    UtTest_Add(SCH_EnableGroupCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_EnableGroupCmd_Test_InvalidArgument, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_InvalidArgument");
    UtTest_Add(SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_NominalCmdGroupNumberNotUnused");
    UtTest_Add(SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused");
    UtTest_Add(SCH_EnableGroupCmd_Test_GroupNotFound, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableGroupCmd_Test_GroupNotFound");

    UtTest_Add(SCH_DisableGroupCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_DisableGroupCmd_Test_InvalidArgument, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_InvalidArgument");
    UtTest_Add(SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_NominalCmdGroupNumberNotUnused");
    UtTest_Add(SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_NominalCmdMultiGroupBitwiseAndTblMultiGroupNotUnused");
    UtTest_Add(SCH_DisableGroupCmd_Test_GroupNotFound, SCH_Test_Setup, SCH_Test_TearDown, "SCH_DisableGroupCmd_Test_GroupNotFound");

    UtTest_Add(SCH_EnableSyncCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableSyncCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_EnableSyncCmd_Test_Nominal, SCH_Test_Setup, SCH_Test_TearDown, "SCH_EnableSyncCmd_Test_Nominal");

    UtTest_Add(SCH_SendDiagTlmCmd_Test_InvalidCmdLength, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_InvalidCmdLength");
    UtTest_Add(SCH_SendDiagTlmCmd_Test_Enabled, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_Enabled");
    UtTest_Add(SCH_SendDiagTlmCmd_Test_Disabled, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_Disabled");
    UtTest_Add(SCH_SendDiagTlmCmd_Test_Other, SCH_Test_Setup, SCH_Test_TearDown, "SCH_SendDiagTlmCmd_Test_Other");

    UtTest_Add(SCH_AcquirePointers_Test_AllResultsGreaterThanSuccess, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AcquirePointers_Test_AllResultsGreaterThanSuccess");
    UtTest_Add(SCH_AcquirePointers_Test_AllResultsSuccess, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AcquirePointers_Test_AllResultsSuccess");
    UtTest_Add(SCH_AcquirePointers_Test_Result1LessThanSuccess, SCH_Test_Setup, SCH_Test_TearDown, "SCH_AcquirePointers_Test_Result1LessThanSuccess");

    UtTest_Add(SCH_VerifyCmdLength_Test_LengthError, SCH_Test_Setup, SCH_Test_TearDown, "SCH_VerifyCmdLength_Test_LengthError");
    UtTest_Add(SCH_VerifyCmdLength_Test_Success, SCH_Test_Setup, SCH_Test_TearDown, "SCH_VerifyCmdLength_Test_Success");

    UtTest_Add(SCH_PostCommandResult_Test_GoodCommand, SCH_Test_Setup, SCH_Test_TearDown, "SCH_PostCommandResult_Test_GoodCommand");
    UtTest_Add(SCH_PostCommandResult_Test_Error, SCH_Test_Setup, SCH_Test_TearDown, "SCH_PostCommandResult_Test_Error");

} /* end SCH_Cmds_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
