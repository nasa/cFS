 /*************************************************************************
 ** File:
 **   $Id: ds_app_test.c 1.3.1.1 2017/02/06 01:43:33EST mdeschu Exp  $
 **
 **  Copyright (c) 2007-2014 United States Government as represented by the 
 **  Administrator of the National Aeronautics and Space Administration. 
 **  All Other Rights Reserved.  
 **
 **  This software was created at NASA's Goddard Space Flight Center.
 **  This software is governed by the NASA Open Source Agreement and may be 
 **  used, distributed and modified only pursuant to the terms of that 
 **  agreement.
 **
 ** Purpose: 
 **   This file contains unit test cases for the functions contained in the file ds_app.c
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 **
 ** Notes:
 **
 *************************************************************************/

/*
 * Includes
 */

#include "ds_app_test.h"
#include "ds_app.h"
#include "ds_appdefs.h"
#include "ds_msg.h"
#include "ds_msgdefs.h"
#include "ds_msgids.h"
#include "ds_events.h"
#include "ds_version.h"
#include "ds_test_utils.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define TLM_STRUCT_DATA_IS_32_ALIGNED(x) ((sizeof(x) - CFE_SB_TLM_HDR_SIZE) % 4) == 0

/*
 * Function Definitions
 */

void DS_AppMain_Test_Nominal(void)
{
    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set to prevent call to CFE_SB_RcvMsg from returning an error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    DS_AppMain();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 3, "Ut_CFE_EVS_GetEventQueueDepth() == 3");
    /* Generates 2 event messages we don't care about in this test */

} /* end DS_AppMain_Test_Nominal */

void DS_AppMain_Test_RegisterAppError(void)
{
    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set to prevent call to CFE_SB_RcvMsg from returning an error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);

    /* Set to satisfy condition "if (Result != CFE_SUCCESS)" immediately after call to CFE_ES_RegisterApp */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERAPP_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_AppMain();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXIT_ERR_EID, CFE_EVS_CRITICAL, "Application terminating, err = 0xFFFFFFFF"),
        "Application terminating, err = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("DS application terminating, err = 0xFFFFFFFF\n"),
        "DS application terminating, err = 0xFFFFFFFF\n");

    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 1, "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

} /* end DS_AppMain_Test_RegisterAppError */

void DS_AppMain_Test_AppInitializeError(void)
{
    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set to prevent call to CFE_SB_RcvMsg from returning an error */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, CFE_SUCCESS, 1);

    /* Set to satisfy condition "if (Result != CFE_SUCCESS)" immediately after call to DS_AppInitialize (which calls CFE_EVS_Register) */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_AppMain();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXIT_ERR_EID, CFE_EVS_CRITICAL, "Application terminating, err = 0xFFFFFFFF"),
        "Application terminating, err = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
    /* Generates 1 event message we don't care about in this test */

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("DS application terminating, err = 0xFFFFFFFF\n"),
        "DS application terminating, err = 0xFFFFFFFF\n");

    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 1, "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

} /* end DS_AppMain_Test_AppInitializeError */

void DS_AppMain_Test_SBError(void)
{
    /* Set to exit loop after first run */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_RUNLOOP_INDEX, FALSE, 2);

    /* Set to fail condition "if (Result != CFE_SUCCESS)" immediately after call to CFE_SB_RcvMsg */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_RCVMSG_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_AppMain();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXIT_ERR_EID, CFE_EVS_CRITICAL, "Application terminating, err = 0xFFFFFFFF"),
        "Application terminating, err = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 4, "Ut_CFE_EVS_GetEventQueueDepth() == 4");
    /* Generates 3 event messages we don't care about in this test */

    UtAssert_True
        (Ut_CFE_ES_SysLogWritten("DS application terminating, err = 0xFFFFFFFF\n"),
        "DS application terminating, err = 0xFFFFFFFF\n");

    UtAssert_True (Ut_CFE_ES_GetSysLogQueueDepth() == 1, "Ut_CFE_ES_GetSysLogQueueDepth() == 1");

} /* end DS_AppMain_Test_SBError */

void DS_AppInitialize_Test_Nominal(void)
{
    int32  Result;
    char   Message[125];

    CFE_PSP_MemSet(&DS_AppData, 1, sizeof(DS_AppData_t));

    /* Execute the function being tested */
    Result = DS_AppInitialize();
    
    /* Verify results */
    UtAssert_True 
        (DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE,
        "DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE");

    UtAssert_True 
        (DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE");

    /* Note: not verifying that CFE_PSP_MemSet set the rest of DS_AppData to 0, because some elements of DS_AppData 
     * are modified by subfunctions, which we're not testing here */

    sprintf(Message, "Application initialized, version %d.%d.%d.%d, data at 0x%08X", DS_MAJOR_VERSION, DS_MINOR_VERSION, DS_REVISION, DS_MISSION_REV, (unsigned int) &DS_AppData);
    UtAssert_True (Ut_CFE_EVS_EventSent(DS_INIT_EID, CFE_EVS_INFORMATION, Message), Message);

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 3, "Ut_CFE_EVS_GetEventQueueDepth() == 3");
    /* Generates 2 event messages we don't care about in this test */

} /* end DS_AppInitialize_Test_Nominal */

void DS_AppInitialize_Test_EVSRegisterError(void)
{
    int32 Result;

    /* Set to generate error message DS_INIT_ERR_EID for EVS services */
    Ut_CFE_EVS_SetReturnCode(UT_CFE_EVS_REGISTER_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = DS_AppInitialize();
    
    /* Verify results */
    UtAssert_True 
        (DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE,
        "DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE");

    UtAssert_True 
        (DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE");

    /* Note: not verifying that CFE_PSP_MemSet set the rest of DS_AppData to 0, because some elements of DS_AppData 
     * are modified by subfunctions, which we're not testing here */

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_ERR_EID, CFE_EVS_ERROR, "Unable to register for EVS services, err = 0xFFFFFFFF"),
        "Unable to register for EVS services, err = 0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppInitialize_Test_EVSRegisterError */

void DS_AppInitialize_Test_SBCreatePipeError(void)
{
    int32 Result;

    /* Set to generate error message DS_INIT_ERR_EID for input pipe */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_CREATEPIPE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = DS_AppInitialize();
    
    /* Verify results */
    UtAssert_True 
        (DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE,
        "DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE");

    UtAssert_True 
        (DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE");

    /* Note: not verifying that CFE_PSP_MemSet set the rest of DS_AppData to 0, because some elements of DS_AppData 
     * are modified by subfunctions, which we're not testing here */

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_ERR_EID, CFE_EVS_ERROR, "Unable to create input pipe, err = 0xFFFFFFFF"),
        "Unable to create input pipe, err = 0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppInitialize_Test_SBCreatePipeError */

void DS_AppInitialize_Test_SBSubscribeHKError(void)
{
    int32 Result;

    /* Set to generate error message DS_INIT_ERR_EID for HK request */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = DS_AppInitialize();
    
    /* Verify results */
    UtAssert_True 
        (DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE,
        "DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE");

    UtAssert_True 
        (DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE");

    /* Note: not verifying that CFE_PSP_MemSet set the rest of DS_AppData to 0, because some elements of DS_AppData 
     * are modified by subfunctions, which we're not testing here */

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_ERR_EID, CFE_EVS_ERROR, "Unable to subscribe to HK request, err = 0xFFFFFFFF"),
        "Unable to subscribe to HK request, err = 0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppInitialize_Test_SBSubscribeHKError */

void DS_AppInitialize_Test_SBSubscribeDSError(void)
{
    int32 Result;

    /* Set to generate error message DS_INIT_ERR_EID for DS commands */
    Ut_CFE_SB_SetReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX, -1, 2);

    /* Execute the function being tested */
    Result = DS_AppInitialize();
    
    /* Verify results */
    UtAssert_True 
        (DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE,
        "DS_AppData.AppEnableState == DS_DEF_ENABLE_STATE");

    UtAssert_True 
        (DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[0].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileHandle == DS_CLOSED_FILE_HANDLE");

    UtAssert_True 
        (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE,
        "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileHandle == DS_CLOSED_FILE_HANDLE");

    /* Note: not verifying that CFE_PSP_MemSet set the rest of DS_AppData to 0, because some elements of DS_AppData 
     * are modified by subfunctions, which we're not testing here */

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_ERR_EID, CFE_EVS_ERROR, "Unable to subscribe to DS commands, err = 0xFFFFFFFF"),
        "Unable to subscribe to DS commands, err = 0xFFFFFFFF");

    UtAssert_True (Result == -1, "Result == -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppInitialize_Test_SBSubscribeDSError */

void DS_AppProcessMsg_Test_Cmd(void)
{
    DS_NoopCmd_t      CmdPacket;
    DS_HashLink_t     HashLink;
    DS_FilterTable_t  FilterTable;
    char              Message[125];

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 6331;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    /* Execute the function being tested */
    DS_AppProcessMsg((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */

    /* This message is generated in the subfunction DS_AppProcessCmd, as a result of reaching the case being tested here */
    sprintf(Message, "NOOP command, Version %d.%d.%d.%d", DS_MAJOR_VERSION, DS_MINOR_VERSION, DS_REVISION, DS_MISSION_REV);
    UtAssert_True (Ut_CFE_EVS_EventSent(DS_NOOP_CMD_EID, CFE_EVS_INFORMATION, Message), Message);

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessMsg_Test_Cmd */

void DS_AppProcessMsg_Test_HK(void)
{
    DS_HkPacket_t     CmdPacket;
    DS_HashLink_t     HashLink;
    DS_FilterTable_t  FilterTable;

    DS_AppData.HashTable[188] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 6332;

    CFE_SB_InitMsg (&CmdPacket, DS_SEND_HK_MID, CFE_SB_CMD_HDR_SIZE, TRUE);

    /* Execute the function being tested */
    DS_AppProcessMsg((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_AppProcessMsg_Test_HK */

void DS_AppProcessMsg_Test_HKInvalidRequest(void)
{
    DS_HkPacket_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_SEND_HK_MID, 0, TRUE);

    /* Execute the function being tested */
    DS_AppProcessMsg((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_HK_REQUEST_ERR_EID, CFE_EVS_ERROR, "Invalid HK request length: expected = 8, actual = 0"),
        "Invalid HK request length: expected = 8, actual = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessMsg_Test_HKInvalidRequest */

void DS_AppProcessMsg_Test_UnknownMID(void)
{
    DS_HkPacket_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, 99, 0, TRUE);

    /* Execute the function being tested */
    DS_AppProcessMsg((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_AppProcessMsg_Test_UnknownMID */

void DS_AppProcessCmd_Test_Noop(void)
{
    DS_NoopCmd_t   CmdPacket;
    char           Message[125];

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    sprintf(Message, "NOOP command, Version %d.%d.%d.%d", DS_MAJOR_VERSION, DS_MINOR_VERSION, DS_REVISION, DS_MISSION_REV);
    UtAssert_True (Ut_CFE_EVS_EventSent(DS_NOOP_CMD_EID, CFE_EVS_INFORMATION, Message), Message);

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_Noop */

void DS_AppProcessCmd_Test_Reset(void)
{
    DS_ResetCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_ResetCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_RESET_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_RESET_CMD_EID, CFE_EVS_DEBUG, "Reset counters command"),
        "Reset counters command");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_Reset */

void DS_AppProcessCmd_Test_SetAppState(void)
{
    DS_AppStateCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AppStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_APP_STATE_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ENADIS_CMD_EID, CFE_EVS_DEBUG, "APP STATE command: state = 0"),
        "APP STATE command: state = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetAppState */

void DS_AppProcessCmd_Test_SetFilterFile(void)
{
    DS_FilterFileCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    CmdPacket.MessageID = DS_CMD_MID;

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER FILE command: packet filter table is not loaded"),
        "Invalid FILTER FILE command: packet filter table is not loaded");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetFilterFile */

void DS_AppProcessCmd_Test_SetFilterType(void)
{
    DS_FilterTypeCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    CmdPacket.MessageID = DS_CMD_MID;

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER TYPE command arg: filter type = 0"),
        "Invalid FILTER TYPE command arg: filter type = 0");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetFilterType */

void DS_AppProcessCmd_Test_SetFilterParms(void)
{
    DS_FilterParmsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterParmsCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER PARMS command arg: invalid messageID = 0x0000"),
        "Invalid FILTER PARMS command arg: invalid messageID = 0x0000");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetFilterParms */

void DS_AppProcessCmd_Test_SetDestType(void)
{
    DS_DestTypeCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_TYPE_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST TYPE command arg: filename type = 0"),
        "Invalid DEST TYPE command arg: filename type = 0");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestType */

void DS_AppProcessCmd_Test_SetDestState(void)
{
    DS_DestStateCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST STATE command: destination file table is not loaded"),
        "Invalid DEST STATE command: destination file table is not loaded");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestState */

void DS_AppProcessCmd_Test_SetDestPath(void)
{
    DS_DestPathCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestPathCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_PATH_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST PATH command arg: pathname"),
        "Invalid DEST PATH command arg: pathname");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestPath */

void DS_AppProcessCmd_Test_SetDestBase(void)
{
    DS_DestBaseCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestBaseCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_BASE_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST BASE command: destination file table is not loaded"),
        "Invalid DEST BASE command: destination file table is not loaded");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestBase */

void DS_AppProcessCmd_Test_SetDestExt(void)
{
    DS_DestExtCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestExtCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST EXT command: destination file table is not loaded"),
        "Invalid DEST EXT command: destination file table is not loaded");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestExt */

void DS_AppProcessCmd_Test_SetDestSize(void)
{
    DS_DestSizeCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestSizeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_SIZE_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST SIZE command arg: size limit = 0"),
        "Invalid DEST SIZE command arg: size limit = 0");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestSize */

void DS_AppProcessCmd_Test_SetDestAge(void)
{
    DS_DestAgeCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestAgeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_AGE_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST AGE command arg: age limit = 0"),
        "Invalid DEST AGE command arg: age limit = 0");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestAge */

void DS_AppProcessCmd_Test_SetDestCount(void)
{
    DS_DestCountCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestCountCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_COUNT_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST COUNT command: destination file table is not loaded"),
        "Invalid DEST COUNT command: destination file table is not loaded");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_SetDestCount */

void DS_AppProcessCmd_Test_CloseFile(void)
{
    DS_CloseFileCmd_t   CmdPacket;
    uint32              i;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_CLOSE_FILE_CC);

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileHandle = DS_CLOSED_FILE_HANDLE;
    }

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CLOSE_CMD_EID, CFE_EVS_DEBUG, "DEST CLOSE command: file table index = 0"),
        "DEST CLOSE command: file table index = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_CloseFile */

void DS_AppProcessCmd_Test_GetFileInfo(void)
{
    DS_GetFileInfoCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_GetFileInfoCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_GET_FILE_INFO_CC);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_GET_FILE_INFO_CMD_EID, CFE_EVS_DEBUG, "GET FILE INFO command"),
        "GET FILE INFO command");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_GetFileInfo */

void DS_AppProcessCmd_Test_AddMID(void)
{
    DS_AddMidCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AddMidCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_ADD_MID_CC);

    CmdPacket.MessageID = DS_CMD_MID;

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid ADD MID command: filter table is not loaded"),
        "Invalid ADD MID command: filter table is not loaded");
    /* For this test, we don't care whether or not the command is successful - only that it's been called */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_AddMID */

void DS_AppProcessCmd_Test_CloseAll(void)
{
    DS_CloseAllCmd_t    CmdPacket;
    uint32              i;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseAllCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_CLOSE_ALL_CC);

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileHandle = DS_CLOSED_FILE_HANDLE;
    }

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CLOSE_ALL_CMD_EID, CFE_EVS_DEBUG, "DEST CLOSE ALL command"),
        "DEST CLOSE ALL command");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_CloseAll */

void DS_AppProcessCmd_Test_InvalidCommandCode(void)
{
    DS_CloseAllCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseAllCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, 99);

    /* Execute the function being tested */
    DS_AppProcessCmd((CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CMD_CODE_ERR_EID, CFE_EVS_ERROR, "Invalid command code: MID = 0x18BB, CC = 99"),
        "Invalid command code: MID = 0x18BB, CC = 99");

    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_AppProcessCmd_Test_InvalidCommandCode */

void DS_AppProcessHK_Test(void)
{
    uint32 i;

    /* Most values in the HK packet can't be checked because they're stored in a local variable. */

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileGrowth = 99;
    }

    /* Execute the function being tested */
    DS_AppProcessHK();
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileStatus[0].FileRate == 99 / DS_SECS_PER_HK_CYCLE, "DS_AppData.FileStatus[0].FileRate == 99 / DS_SECS_PER_HK_CYCLE");
    UtAssert_True (DS_AppData.FileStatus[0].FileGrowth == 0, "DS_AppData.FileStatus[0].FileGrowth == 0");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileRate == 99 / DS_SECS_PER_HK_CYCLE, "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileRate == 99 / DS_SECS_PER_HK_CYCLE");
    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileGrowth == 0, "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileGrowth == 0");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT - 1].FileRate == 99 / DS_SECS_PER_HK_CYCLE, "DS_AppData.FileStatus[DS_DEST_FILE_CNT - 1].FileRate == 99 / DS_SECS_PER_HK_CYCLE");
    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT - 1].FileGrowth == 0, "DS_AppData.FileStatus[DS_DEST_FILE_CNT - 1].FileGrowth == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (TLM_STRUCT_DATA_IS_32_ALIGNED(DS_HkPacket_t), "DS_HkPacket_t is 32-bit aligned");
} /* end DS_AppProcessHK_Test */

void DS_AppStorePacket_Test_Nominal(void)
{
    DS_CloseAllCmd_t   CmdPacket;
    CFE_SB_MsgId_t     MessageID = 1;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseAllCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, 99);

    /* Execute the function being tested */
    DS_AppStorePacket(MessageID, (CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_AppStorePacket_Test_Nominal */

void DS_AppStorePacket_Test_DSDisabled(void)
{
    DS_CloseAllCmd_t   CmdPacket;
    CFE_SB_MsgId_t     MessageID = 1;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseAllCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, 99);

    DS_AppData.AppEnableState = DS_DISABLED;

    /* Execute the function being tested */
    DS_AppStorePacket(MessageID, (CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (DS_AppData.DisabledPktCounter == 1, "DS_AppData.DisabledPktCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_AppStorePacket_Test_DSDisabled */

void DS_AppStorePacket_Test_FilterTableNotLoaded(void)
{
    DS_CloseAllCmd_t   CmdPacket;
    CFE_SB_MsgId_t     MessageID = 1;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseAllCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, 99);

    DS_AppData.AppEnableState = DS_ENABLED;
    DS_AppData.FilterTblPtr = 0;

    /* Execute the function being tested */
    DS_AppStorePacket(MessageID, (CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (DS_AppData.IgnoredPktCounter == 1, "DS_AppData.IgnoredPktCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_AppStorePacket_Test_FilterTableNotLoaded */

void DS_AppStorePacket_Test_DestFileTableNotLoaded(void)
{
    DS_CloseAllCmd_t   CmdPacket;
    CFE_SB_MsgId_t     MessageID = 1;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseAllCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, 99);

    DS_AppData.AppEnableState = DS_ENABLED;
    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_AppStorePacket(MessageID, (CFE_SB_MsgPtr_t)(&CmdPacket));
    
    /* Verify results */
    UtAssert_True (DS_AppData.IgnoredPktCounter == 1, "DS_AppData.IgnoredPktCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_AppStorePacket_Test_DestFileTableNotLoaded */

void DS_App_Test_AddTestCases(void)
{
    UtTest_Add(DS_AppMain_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_AppMain_Test_Nominal");
    UtTest_Add(DS_AppMain_Test_RegisterAppError, DS_Test_Setup, DS_Test_TearDown, "DS_AppMain_Test_RegisterAppError");
    UtTest_Add(DS_AppMain_Test_AppInitializeError, DS_Test_Setup, DS_Test_TearDown, "DS_AppMain_Test_AppInitializeError");
    UtTest_Add(DS_AppMain_Test_SBError, DS_Test_Setup, DS_Test_TearDown, "DS_AppMain_Test_SBError");

    UtTest_Add(DS_AppInitialize_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_AppInitialize_Test_Nominal");
    UtTest_Add(DS_AppInitialize_Test_EVSRegisterError, DS_Test_Setup, DS_Test_TearDown, "DS_AppInitialize_Test_EVSRegisterError");
    UtTest_Add(DS_AppInitialize_Test_SBCreatePipeError, DS_Test_Setup, DS_Test_TearDown, "DS_AppInitialize_Test_SBCreatePipeError");
    UtTest_Add(DS_AppInitialize_Test_SBSubscribeHKError, DS_Test_Setup, DS_Test_TearDown, "DS_AppInitialize_Test_SBSubscribeHKError");
    UtTest_Add(DS_AppInitialize_Test_SBSubscribeDSError, DS_Test_Setup, DS_Test_TearDown, "DS_AppInitialize_Test_SBSubscribeDSError");

    UtTest_Add(DS_AppProcessMsg_Test_Cmd, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessMsg_Test_Cmd");
    UtTest_Add(DS_AppProcessMsg_Test_HK, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessMsg_Test_HK");
    UtTest_Add(DS_AppProcessMsg_Test_HKInvalidRequest, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessMsg_Test_HKInvalidRequest");
    UtTest_Add(DS_AppProcessMsg_Test_UnknownMID, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessMsg_Test_UnknownMID");

    UtTest_Add(DS_AppProcessCmd_Test_Noop, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_Noop");
    UtTest_Add(DS_AppProcessCmd_Test_Reset, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_Reset");
    UtTest_Add(DS_AppProcessCmd_Test_SetAppState, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetAppState");
    UtTest_Add(DS_AppProcessCmd_Test_SetFilterFile, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetFilterFile");
    UtTest_Add(DS_AppProcessCmd_Test_SetFilterType, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetFilterType");
    UtTest_Add(DS_AppProcessCmd_Test_SetFilterParms, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetFilterParms");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestType, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestType");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestState, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestState");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestPath, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestPath");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestBase, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestBase");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestExt, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestExt");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestSize, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestSize");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestAge, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestAge");
    UtTest_Add(DS_AppProcessCmd_Test_SetDestCount, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_SetDestCount");
    UtTest_Add(DS_AppProcessCmd_Test_CloseFile, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_CloseFile");
    UtTest_Add(DS_AppProcessCmd_Test_GetFileInfo, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_GetFileInfo");
    UtTest_Add(DS_AppProcessCmd_Test_AddMID, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_AddMID");
    UtTest_Add(DS_AppProcessCmd_Test_CloseAll, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_CloseAll");
    UtTest_Add(DS_AppProcessCmd_Test_InvalidCommandCode, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessCmd_Test_InvalidCommandCode");

    UtTest_Add(DS_AppProcessHK_Test, DS_Test_Setup, DS_Test_TearDown, "DS_AppProcessHK_Test");

    UtTest_Add(DS_AppStorePacket_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_AppStorePacket_Test_Nominal");
    UtTest_Add(DS_AppStorePacket_Test_DSDisabled, DS_Test_Setup, DS_Test_TearDown, "DS_AppStorePacket_Test_DSDisabled");
    UtTest_Add(DS_AppStorePacket_Test_FilterTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_AppStorePacket_Test_FilterTableNotLoaded");
    UtTest_Add(DS_AppStorePacket_Test_DestFileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_AppStorePacket_Test_DestFileTableNotLoaded");
} /* end DS_App_Test_AddTestCases */


/************************/
/*  End of File Comment */
/************************/
