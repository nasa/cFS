 /*************************************************************************
 ** File:
 **   $Id: ds_cmds_test.c 1.6.1.2 2017/02/06 01:43:33EST mdeschu Exp  $
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
 **   This file contains unit test cases for the functions contained in the file ds_cmds.c
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

#include "ds_cmds_test.h"
#include "ds_app.h"
#include "ds_appdefs.h"
#include "ds_cmds.h"
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


#define CMD_STRUCT_DATA_IS_32_ALIGNED(x) ((sizeof(x) - CFE_SB_CMD_HDR_SIZE) % 4) == 0
#define TLM_STRUCT_DATA_IS_32_ALIGNED(x) ((sizeof(x) - CFE_SB_TLM_HDR_SIZE) % 4) == 0

/*
 * Function Definitions
 */

void DS_CmdNoop_Test_Nominal(void)
{
    DS_NoopCmd_t      CmdPacket;
    char              Message[125];

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    /* Execute the function being tested */
    DS_CmdNoop((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    sprintf(Message, "NOOP command, Version %d.%d.%d.%d", DS_MAJOR_VERSION, DS_MINOR_VERSION, DS_REVISION, DS_MISSION_REV);
    UtAssert_True (Ut_CFE_EVS_EventSent(DS_NOOP_CMD_EID, CFE_EVS_INFORMATION, Message), Message);

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_NoopCmd_t), "DS_NoopCmd_t is 32-bit aligned");

} /* end DS_CmdNoop_Test_Nominal */

void DS_CmdNoop_Test_InvalidCommandLength(void)
{
    DS_NoopCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    /* Execute the function being tested */
    DS_CmdNoop((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_NOOP_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid NOOP command length: expected = 8, actual = 1"),
        "Invalid NOOP command length: expected = 8, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_CmdNoop_Test_InvalidCommandLength */

void DS_CmdReset_Test_Nominal(void)
{
    DS_ResetCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_ResetCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_RESET_CC);

    /* Execute the function being tested */
    DS_CmdReset((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 0, "DS_AppData.CmdAcceptedCounter == 0");
    UtAssert_True (DS_AppData.CmdRejectedCounter == 0, "DS_AppData.CmdRejectedCounter == 0");
    UtAssert_True (DS_AppData.DisabledPktCounter == 0, "DS_AppData.DisabledPktCounter == 0");
    UtAssert_True (DS_AppData.IgnoredPktCounter == 0, "DS_AppData.IgnoredPktCounter == 0");
    UtAssert_True (DS_AppData.FilteredPktCounter == 0, "DS_AppData.FilteredPktCounter == 0");
    UtAssert_True (DS_AppData.PassedPktCounter == 0, "DS_AppData.PassedPktCounter == 0");
    UtAssert_True (DS_AppData.FileWriteCounter == 0, "DS_AppData.FileWriteCounter == 0");
    UtAssert_True (DS_AppData.FileWriteErrCounter == 0, "DS_AppData.FileWriteErrCounter == 0");
    UtAssert_True (DS_AppData.FileUpdateCounter == 0, "DS_AppData.FileUpdateCounter == 0");
    UtAssert_True (DS_AppData.FileUpdateErrCounter == 0, "DS_AppData.FileUpdateErrCounter == 0");
    UtAssert_True (DS_AppData.DestTblLoadCounter == 0, "DS_AppData.DestTblLoadCounter == 0");
    UtAssert_True (DS_AppData.DestTblErrCounter == 0, "DS_AppData.DestTblErrCounter == 0");
    UtAssert_True (DS_AppData.FilterTblLoadCounter == 0, "DS_AppData.FilterTblLoadCounter == 0");
    UtAssert_True (DS_AppData.FilterTblErrCounter == 0, "DS_AppData.FilterTblErrCounter == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_RESET_CMD_EID, CFE_EVS_DEBUG, "Reset counters command"),
        "Reset counters command");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_ResetCmd_t), "DS_ResetCmd_t is 32-bit aligned");


} /* end DS_CmdReset_Test_Nominal */

void DS_CmdReset_Test_InvalidCommandLength(void)
{
    DS_ResetCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_RESET_CC);

    /* Execute the function being tested */
    DS_CmdReset((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_RESET_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid RESET command length: expected = 8, actual = 1"),
        "Invalid RESET command length: expected = 8, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdReset_Test_InvalidCommandLength */

void DS_CmdSetAppState_Test_Nominal(void)
{
    DS_AppStateCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AppStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_APP_STATE_CC);

    CmdPacket.EnableState = TRUE;

    /* Execute the function being tested */
    DS_CmdSetAppState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");
    UtAssert_True (DS_AppData.AppEnableState == TRUE, "DS_AppData.AppEnableState == TRUE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ENADIS_CMD_EID, CFE_EVS_DEBUG, "APP STATE command: state = 1"),
        "APP STATE command: state = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_AppStateCmd_t), "DS_AppStateCmd_t is 32-bit aligned");

} /* end DS_CmdSetAppState_Test_Nominal */

void DS_CmdSetAppState_Test_InvalidCommandLength(void)
{
    DS_AppStateCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_APP_STATE_CC);

    CmdPacket.EnableState = TRUE;

    /* Execute the function being tested */
    DS_CmdSetAppState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ENADIS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid APP STATE command length: expected = 12, actual = 1"),
        "Invalid APP STATE command length: expected = 12, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetAppState_Test_InvalidCommandLength */

void DS_CmdSetAppState_Test_InvalidAppState(void)
{
    DS_AppStateCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AppStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_APP_STATE_CC);

    CmdPacket.EnableState = 99;

    /* Execute the function being tested */
    DS_CmdSetAppState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ENADIS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid APP STATE command arg: app state = 99"),
        "Invalid APP STATE command arg: app state = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetAppState_Test_InvalidAppState */

void DS_CmdSetFilterFile_Test_Nominal(void)
{
    DS_FilterFileCmd_t   CmdPacket;
    DS_HashLink_t        HashLink;
    DS_FilterTable_t     FilterTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    CmdPacket.FilterParmsIndex = 2;
    CmdPacket.MessageID = 0x18BB;
    CmdPacket.FileTableIndex = 4;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdSetFilterFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[CmdPacket.FileTableIndex].Filter[CmdPacket.FilterParmsIndex].FileTableIndex == 0,
        "DS_AppData.FilterTblPtr->Packet[CmdPacket.FileTableIndex].Filter[CmdPacket.FilterParmsIndex].FileTableIndex == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_EID, CFE_EVS_DEBUG, "FILTER FILE command: MID = 0x18BB, index = 0, filter = 2, file = 4"),
        "FILTER FILE command: MID = 0x18BB, index = 0, filter = 2, file = 4");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_FilterFileCmd_t), "DS_FilterFileCmd_t is 32-bit aligned");
} /* end DS_CmdSetFilterFile_Test_Nominal */

void DS_CmdSetFilterFile_Test_InvalidCommandLength(void)
{
    DS_FilterFileCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    /* Execute the function being tested */
    DS_CmdSetFilterFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER FILE command length: expected = 16, actual = 1"),
        "Invalid FILTER FILE command length: expected = 16, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterFile_Test_InvalidCommandLength */

void DS_CmdSetFilterFile_Test_InvalidMessageID(void)
{
    DS_FilterFileCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    CmdPacket.MessageID = DS_UNUSED;

    /* Execute the function being tested */
    DS_CmdSetFilterFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER FILE command arg: invalid messageID = 0x   0"),
        "Invalid FILTER FILE command arg: invalid messageID = 0x   0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterFile_Test_InvalidMessageID */

void DS_CmdSetFilterFile_Test_InvalidFilterParametersIndex(void)
{
    DS_FilterFileCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = DS_FILTERS_PER_PACKET;

    /* Execute the function being tested */
    DS_CmdSetFilterFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER FILE command arg: filter parameters index = 4"),
        "Invalid FILTER FILE command arg: filter parameters index = 4");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterFile_Test_InvalidFilterParametersIndex */

void DS_CmdSetFilterFile_Test_InvalidFileTableIndex(void)
{
    DS_FilterFileCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = 1;
    CmdPacket.FileTableIndex   = 99;

    /* Execute the function being tested */
    DS_CmdSetFilterFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER FILE command arg: file table index = 99"),
        "Invalid FILTER FILE command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterFile_Test_InvalidFileTableIndex */

void DS_CmdSetFilterFile_Test_FilterTableNotLoaded(void)
{
    DS_FilterFileCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = 1;
    CmdPacket.FileTableIndex   = 1;

    /* Execute the function being tested */
    DS_CmdSetFilterFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER FILE command: packet filter table is not loaded"),
        "Invalid FILTER FILE command: packet filter table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterFile_Test_FilterTableNotLoaded */

void DS_CmdSetFilterFile_Test_MessageIDNotInFilterTable(void)
{
    DS_FilterFileCmd_t   CmdPacket;
    DS_HashLink_t        HashLink;
    DS_FilterTable_t     FilterTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_FILE_CC);

    CmdPacket.FilterParmsIndex = 2;
    CmdPacket.MessageID = 0x9999;
    CmdPacket.FileTableIndex = 4;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdSetFilterFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER FILE command: Message ID 0x9999 is not in filter table"),
        "Invalid FILTER FILE command: Message ID 0x9999 is not in filter table");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterFile_Test_MessageIDNotInFilterTable */

void DS_CmdSetFilterType_Test_Nominal(void)
{
    DS_FilterTypeCmd_t   CmdPacket;
    DS_HashLink_t        HashLink;
    DS_FilterTable_t     FilterTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    CmdPacket.FilterParmsIndex = 2;
    CmdPacket.MessageID = 0x18BB;
    CmdPacket.FilterType = 1;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdSetFilterType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].FilterType == 1,
        "DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].FilterType == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_EID, CFE_EVS_DEBUG, "FILTER TYPE command: MID = 0x18BB, index = 0, filter = 2, type = 1"),
        "FILTER TYPE command: MID = 0x18BB, index = 0, filter = 2, type = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_FilterTypeCmd_t), "DS_FilterTypeCmd_t is 32-bit aligned");
} /* end DS_CmdSetFilterType_Test_Nominal */

void DS_CmdSetFilterType_Test_InvalidCommandLength(void)
{
    DS_FilterTypeCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    /* Execute the function being tested */
    DS_CmdSetFilterType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER TYPE command length: expected = 16, actual = 1"),
        "Invalid FILTER TYPE command length: expected = 16, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterType_Test_InvalidCommandLength */

void DS_CmdSetFilterType_Test_InvalidMessageID(void)
{
    DS_FilterTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    CmdPacket.MessageID = DS_UNUSED;

    /* Execute the function being tested */
    DS_CmdSetFilterType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER TYPE command arg: invalid messageID = 0x0000"),
        "Invalid FILTER TYPE command arg: invalid messageID = 0x0000");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterType_Test_InvalidMessageID */

void DS_CmdSetFilterType_Test_InvalidFilterParametersIndex(void)
{
    DS_FilterTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = DS_FILTERS_PER_PACKET;

    /* Execute the function being tested */
    DS_CmdSetFilterType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER TYPE command arg: filter parameters index = 4"),
        "Invalid FILTER TYPE command arg: filter parameters index = 4");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterType_Test_InvalidFilterParametersIndex */

void DS_CmdSetFilterType_Test_InvalidFilterType(void)
{
    DS_FilterTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = 1;
    CmdPacket.FilterType       = FALSE;

    /* Execute the function being tested */
    DS_CmdSetFilterType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER TYPE command arg: filter type = 0"),
        "Invalid FILTER TYPE command arg: filter type = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterType_Test_InvalidFilterType */

void DS_CmdSetFilterType_Test_FilterTableNotLoaded(void)
{
    DS_FilterTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = 1;
    CmdPacket.FilterType       = 1;

    /* Execute the function being tested */
    DS_CmdSetFilterType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER TYPE command: packet filter table is not loaded"),
        "Invalid FILTER TYPE command: packet filter table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterType_Test_FilterTableNotLoaded */

void DS_CmdSetFilterType_Test_MessageIDNotInFilterTable(void)
{
    DS_FilterTypeCmd_t   CmdPacket;
    DS_HashLink_t        HashLink;
    DS_FilterTable_t     FilterTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_TYPE_CC);

    CmdPacket.MessageID        = 0x9999;
    CmdPacket.FilterParmsIndex = 1;
    CmdPacket.FilterType       = 1;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdSetFilterType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER TYPE command: Message ID 0x9999 is not in filter table"),
        "Invalid FILTER TYPE command: Message ID 0x9999 is not in filter table");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterType_Test_MessageIDNotInFilterTable */

void DS_CmdSetFilterParms_Test_Nominal(void)
{
    DS_FilterParmsCmd_t   CmdPacket;
    DS_HashLink_t        HashLink;
    DS_FilterTable_t     FilterTable;


    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterParmsCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    CmdPacket.FilterParmsIndex = 2;
    CmdPacket.MessageID = 0x18BB;
    CmdPacket.Algorithm_N = 0;
    CmdPacket.Algorithm_X = 0;
    CmdPacket.Algorithm_O = 0;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdSetFilterParms((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].Algorithm_N == 0,
        "DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].Algorithm_N == 0");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].Algorithm_X == 0,
        "DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].Algorithm_X == 0");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].Algorithm_O == 0,
        "DS_AppData.FilterTblPtr->Packet[0].Filter[CmdPacket.FilterParmsIndex].Algorithm_O == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_EID, CFE_EVS_DEBUG, "FILTER PARMS command: MID = 0x18BB, index = 0, filter = 2, N = 0, X = 0, O = 0"),
        "FILTER PARMS command: MID = 0x18BB, index = 0, filter = 2, N = 0, X = 0, O = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_FilterParmsCmd_t), "DS_FilterParmsCmd_t is 32-bit aligned");

} /* end DS_CmdSetFilterParms_Test_Nominal */

void DS_CmdSetFilterParms_Test_InvalidCommandLength(void)
{
    DS_FilterParmsCmd_t   CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    /* Execute the function being tested */
    DS_CmdSetFilterParms((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER PARMS command length: expected = 20, actual = 1"),
        "Invalid FILTER PARMS command length: expected = 20, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterParms_Test_InvalidCommandLength */

void DS_CmdSetFilterParms_Test_InvalidMessageID(void)
{
    DS_FilterParmsCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterParmsCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    CmdPacket.MessageID = DS_UNUSED;

    /* Execute the function being tested */
    DS_CmdSetFilterParms((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER PARMS command arg: invalid messageID = 0x0000"),
        "Invalid FILTER PARMS command arg: invalid messageID = 0x0000");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterParms_Test_InvalidMessageID */

void DS_CmdSetFilterParms_Test_InvalidFilterParametersIndex(void)
{
    DS_FilterParmsCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterParmsCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = DS_FILTERS_PER_PACKET;

    /* Execute the function being tested */
    DS_CmdSetFilterParms((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER PARMS command arg: filter parameters index = 4"),
        "Invalid FILTER PARMS command arg: filter parameters index = 4");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterParms_Test_InvalidFilterParametersIndex */

void DS_CmdSetFilterParms_Test_InvalidFilterAlgorithm(void)
{
    DS_FilterParmsCmd_t   CmdPacket;
    DS_HashLink_t        HashLink;
    DS_FilterTable_t     FilterTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterParmsCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    CmdPacket.FilterParmsIndex = 2;
    CmdPacket.MessageID = 0x18BB;
    CmdPacket.Algorithm_N = 1;
    CmdPacket.Algorithm_X = 1;
    CmdPacket.Algorithm_O = 1;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdSetFilterParms((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER PARMS command arg: N = 1, X = 1, O = 1"),
        "Invalid FILTER PARMS command arg: N = 1, X = 1, O = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterParms_Test_InvalidFilterAlgorithm */

void DS_CmdSetFilterParms_Test_FilterTableNotLoaded(void)
{
    DS_FilterParmsCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterParmsCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    CmdPacket.MessageID        = 0x18BB;
    CmdPacket.FilterParmsIndex = 1;

    /* Execute the function being tested */
    DS_CmdSetFilterParms((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER PARMS command: packet filter table is not loaded"),
        "Invalid FILTER PARMS command: packet filter table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterParms_Test_FilterTableNotLoaded */

void DS_CmdSetFilterParms_Test_MessageIDNotInFilterTable(void)
{
    DS_FilterParmsCmd_t   CmdPacket;
    DS_HashLink_t         HashLink;
    DS_FilterTable_t      FilterTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_FilterParmsCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_FILTER_PARMS_CC);

    CmdPacket.FilterParmsIndex = 2;
    CmdPacket.MessageID = 0x9999;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdSetFilterParms((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid FILTER PARMS command: Message ID 0x9999 is not in filter table"),
        "Invalid FILTER PARMS command: Message ID 0x9999 is not in filter table");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetFilterParms_Test_MessageIDNotInFilterTable */

void DS_CmdSetDestType_Test_Nominal(void)
{
    DS_DestTypeCmd_t      CmdPacket;
    DS_DestFileTable_t    DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_TYPE_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.FileNameType   = 2;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].FileNameType == 2,
        "DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].FileNameType == 2");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_NTYPE_CMD_EID, CFE_EVS_DEBUG, "DEST TYPE command: file table index = 1, filename type = 2"),
        "DEST TYPE command: file table index = 1, filename type = 2");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestTypeCmd_t), "DS_DestTypeCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestType_Test_Nominal */

void DS_CmdSetDestType_Test_InvalidCommandLength(void)
{
    DS_DestTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_TYPE_CC);

    /* Execute the function being tested */
    DS_CmdSetDestType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST TYPE command length: expected = 12, actual = 1"),
        "Invalid DEST TYPE command length: expected = 12, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestType_Test_InvalidCommandLength */

void DS_CmdSetDestType_Test_InvalidFileTableIndex(void)
{
    DS_DestTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_TYPE_CC);

    CmdPacket.FileTableIndex   = 99;

    /* Execute the function being tested */
    DS_CmdSetDestType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST TYPE command arg: file table index = 99"),
        "Invalid DEST TYPE command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestType_Test_InvalidFileTableIndex */

void DS_CmdSetDestType_Test_InvalidFilenameType(void)
{
    DS_DestTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_TYPE_CC);

    CmdPacket.FileTableIndex   = 1;
    CmdPacket.FileNameType     = 99;

    /* Execute the function being tested */
    DS_CmdSetDestType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST TYPE command arg: filename type = 99"),
        "Invalid DEST TYPE command arg: filename type = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestType_Test_InvalidFilenameType */

void DS_CmdSetDestType_Test_FileTableNotLoaded(void)
{
    DS_DestTypeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestTypeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_TYPE_CC);

    CmdPacket.FileTableIndex   = 1;
    CmdPacket.FileNameType     = 2;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestType((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST TYPE command: destination file table is not loaded"),
        "Invalid DEST TYPE command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestType_Test_FileTableNotLoaded */

void DS_CmdSetDestState_Test_Nominal(void)
{
    DS_DestStateCmd_t      CmdPacket;
    DS_DestFileTable_t     DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.EnableState    = 1;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].EnableState == CmdPacket.EnableState,
        "DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].EnableState == CmdPacket.EnableState");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_STATE_CMD_EID, CFE_EVS_DEBUG, "DEST STATE command: file table index = 1, file state = 1"),
        "DEST STATE command: file table index = 1, file state = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestStateCmd_t), "DS_DestStateCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestState_Test_Nominal */

void DS_CmdSetDestState_Test_InvalidCommandLength(void)
{
    DS_DestStateCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    /* Execute the function being tested */
    DS_CmdSetDestState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST STATE command length: expected = 12, actual = 1"),
        "Invalid DEST STATE command length: expected = 12, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestState_Test_InvalidCommandLength */

void DS_CmdSetDestState_Test_InvalidFileTableIndex(void)
{
    DS_DestStateCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex   = 99;

    /* Execute the function being tested */
    DS_CmdSetDestState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST STATE command arg: file table index = 99"),
        "Invalid DEST STATE command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestState_Test_InvalidFileTableIndex */

void DS_CmdSetDestState_Test_InvalidFileState(void)
{
    DS_DestStateCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex   = 1;
    CmdPacket.EnableState      = 99;

    /* Execute the function being tested */
    DS_CmdSetDestState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST STATE command arg: file state = 99"),
        "Invalid DEST STATE command arg: file state = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestState_Test_InvalidFileState */

void DS_CmdSetDestState_Test_FileTableNotLoaded(void)
{
    DS_DestStateCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestStateCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex = 1;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestState((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST STATE command: destination file table is not loaded"),
        "Invalid DEST STATE command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestState_Test_FileTableNotLoaded */

void DS_CmdSetDestPath_Test_Nominal(void)
{
    DS_DestPathCmd_t      CmdPacket;
    DS_DestFileTable_t     DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestPathCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_PATH_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Pathname, "pathname", OS_MAX_PATH_LEN);

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestPath((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (strncmp (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].Pathname, "pathname", OS_MAX_PATH_LEN) == 0,
        "strncmp (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].Pathname, 'pathname', OS_MAX_PATH_LEN) == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PATH_CMD_EID, CFE_EVS_DEBUG, "DEST PATH command: file table index = 1, pathname = 'pathname'"),
        "DEST PATH command: file table index = 1, pathname = 'pathname'");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestPathCmd_t), "DS_DestPathCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestPath_Test_Nominal */

void DS_CmdSetDestPath_Test_InvalidCommandLength(void)
{
    DS_DestPathCmd_t      CmdPacket;
    char ErrorMessage[150];

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    /* Execute the function being tested */
    DS_CmdSetDestPath((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    sprintf(ErrorMessage, "Invalid DEST PATH command length: expected = %d, actual = 1", sizeof(DS_DestPathCmd_t));
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR, ErrorMessage),
        ErrorMessage);

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestPath_Test_InvalidCommandLength */

void DS_CmdSetDestPath_Test_InvalidFileTableIndex(void)
{
    DS_DestPathCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestPathCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex   = 99;

    /* Execute the function being tested */
    DS_CmdSetDestPath((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST PATH command arg: file table index = 99"),
        "Invalid DEST PATH command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestPath_Test_InvalidFileTableIndex */

void DS_CmdSetDestPath_Test_InvalidPathname(void)
{
    DS_DestPathCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestPathCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Pathname, "***", OS_MAX_PATH_LEN);

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestPath((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST PATH command arg: pathname"),
        "Invalid DEST PATH command arg: pathname");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestPath_Test_InvalidPathname */

void DS_CmdSetDestPath_Test_FileTableNotLoaded(void)
{
    DS_DestPathCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestPathCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Pathname, "pathname", OS_MAX_PATH_LEN);

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestPath((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST PATH command: destination file table is not loaded"),
        "Invalid DEST PATH command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestPath_Test_FileTableNotLoaded */

void DS_CmdSetDestBase_Test_Nominal(void)
{
    DS_DestBaseCmd_t       CmdPacket;
    DS_DestFileTable_t     DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestBaseCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_PATH_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Basename, "base", OS_MAX_PATH_LEN);

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestBase((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (strncmp (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].Basename, "base", OS_MAX_PATH_LEN) == 0,
        "strncmp (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].Basename, 'base', OS_MAX_PATH_LEN) == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_BASE_CMD_EID, CFE_EVS_DEBUG, "DEST BASE command: file table index = 1, base filename = 'base'"),
        "DEST BASE command: file table index = 1, base filename = 'base'");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestBaseCmd_t), "DS_DestBaseCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestBase_Test_Nominal */

void DS_CmdSetDestBase_Test_InvalidCommandLength(void)
{
    DS_DestBaseCmd_t      CmdPacket;
    char ErrorMessage[150];

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    /* Execute the function being tested */
    DS_CmdSetDestBase((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    sprintf(ErrorMessage, "Invalid DEST BASE command length: expected = %d, actual = 1", sizeof(DS_DestPathCmd_t));
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR, ErrorMessage),
        ErrorMessage);

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestBase_Test_InvalidCommandLength */

void DS_CmdSetDestBase_Test_InvalidFileTableIndex(void)
{
    DS_DestBaseCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestBaseCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex   = 99;

    /* Execute the function being tested */
    DS_CmdSetDestBase((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST BASE command arg: file table index = 99"),
        "Invalid DEST BASE command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestBase_Test_InvalidFileTableIndex */

void DS_CmdSetDestBase_Test_InvalidBaseFilename(void)
{
    DS_DestBaseCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestBaseCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Basename, "***", OS_MAX_PATH_LEN);

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestBase((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST BASE command arg: base filename"),
        "Invalid DEST BASE command arg: base filename");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestBase_Test_InvalidBaseFilename */

void DS_CmdSetDestBase_Test_FileTableNotLoaded(void)
{
    DS_DestPathCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestBaseCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_STATE_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Pathname, "pathname", OS_MAX_PATH_LEN);

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestBase((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST BASE command: destination file table is not loaded"),
        "Invalid DEST BASE command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestBase_Test_FileTableNotLoaded */

void DS_CmdSetDestExt_Test_Nominal(void)
{
    DS_DestExtCmd_t       CmdPacket;
    DS_DestFileTable_t    DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestExtCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Extension, "txt", DS_EXTENSION_BUFSIZE);

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestExt((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (strncmp (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].Extension, "txt", DS_EXTENSION_BUFSIZE) == 0,
        "strncmp (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].Extension, 'txt', DS_EXTENSION_BUFSIZE) == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXT_CMD_EID, CFE_EVS_DEBUG, "DEST EXT command: file table index = 1, extension = 'txt'"),
        "DEST EXT command: file table index = 1, extension = 'txt'");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestExtCmd_t), "DS_DestExtCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestExt_Test_Nominal */

void DS_CmdSetDestExt_Test_InvalidCommandLength(void)
{
    DS_DestExtCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    /* Execute the function being tested */
    DS_CmdSetDestExt((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST EXT command length: expected = 20, actual = 1"),
        "Invalid DEST EXT command length: expected = 20, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestExt_Test_InvalidCommandLength */

void DS_CmdSetDestExt_Test_InvalidFileTableIndex(void)
{
    DS_DestExtCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestExtCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex   = 99;

    /* Execute the function being tested */
    DS_CmdSetDestExt((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST EXT command arg: file table index = 99"),
        "Invalid DEST EXT command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestExt_Test_InvalidFileTableIndex */

void DS_CmdSetDestExt_Test_InvalidFilenameExtension(void)
{
    DS_DestExtCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestExtCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Extension, "***", DS_EXTENSION_BUFSIZE);

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestExt((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST EXT command arg: extension"),
        "Invalid DEST EXT command arg: extension");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestExt_Test_InvalidFilenameExtension */

void DS_CmdSetDestExt_Test_FileTableNotLoaded(void)
{
    DS_DestExtCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestExtCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex = 1;
    strncpy (CmdPacket.Extension, "txt", DS_SET_DEST_EXT_CC);

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestExt((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST EXT command: destination file table is not loaded"),
        "Invalid DEST EXT command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestExt_Test_FileTableNotLoaded */

void DS_CmdSetDestSize_Test_Nominal(void)
{
    DS_DestSizeCmd_t      CmdPacket;
    DS_DestFileTable_t    DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestSizeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.MaxFileSize = 100000000;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestSize((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].MaxFileSize == 100000000,
        "DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].MaxFileSize == 100000000");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SIZE_CMD_EID, CFE_EVS_DEBUG, "DEST SIZE command: file table index = 1, size limit = 100000000"),
        "DEST SIZE command: file table index = 1, size limit = 100000000");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestSizeCmd_t), "DS_DestSizeCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestSize_Test_Nominal */

void DS_CmdSetDestSize_Test_InvalidCommandLength(void)
{
    DS_DestSizeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.MaxFileSize = 100000000;

    /* Execute the function being tested */
    DS_CmdSetDestSize((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST SIZE command length: expected = 16, actual = 1"),
        "Invalid DEST SIZE command length: expected = 16, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestSize_Test_InvalidCommandLength */

void DS_CmdSetDestSize_Test_InvalidFileTableIndex(void)
{
    DS_DestSizeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestSizeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex  = 99;
    CmdPacket.MaxFileSize     = 100000000;

    /* Execute the function being tested */
    DS_CmdSetDestSize((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST SIZE command arg: file table index = 99"),
        "Invalid DEST SIZE command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestSize_Test_InvalidFileTableIndex */

void DS_CmdSetDestSize_Test_InvalidFileSizeLimit(void)
{
    DS_DestSizeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestSizeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex  = 1;
    CmdPacket.MaxFileSize     = 1;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestSize((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST SIZE command arg: size limit = 1"),
        "Invalid DEST SIZE command arg: size limit = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestSize_Test_InvalidFileSizeLimit */

void DS_CmdSetDestSize_Test_FileTableNotLoaded(void)
{
    DS_DestSizeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestSizeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_EXT_CC);

    CmdPacket.FileTableIndex  = 1;
    CmdPacket.MaxFileSize     = 100000000;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestSize((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST SIZE command: destination file table is not loaded"),
        "Invalid DEST SIZE command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestSize_Test_FileTableNotLoaded */

void DS_CmdSetDestAge_Test_Nominal(void)
{
    DS_DestAgeCmd_t       CmdPacket;
    DS_DestFileTable_t    DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestAgeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_AGE_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.MaxFileAge = 1000;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestAge((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].MaxFileAge == 1000,
        "DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].MaxFileAge == 1000");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_AGE_CMD_EID, CFE_EVS_DEBUG, "DEST AGE command: file table index = 1, age limit = 1000"),
        "DEST AGE command: file table index = 1, age limit = 1000");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestAgeCmd_t), "DS_DestAgeCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestAge_Test_Nominal */

void DS_CmdSetDestAge_Test_InvalidCommandLength(void)
{
    DS_DestAgeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_AGE_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.MaxFileAge = 1000;

    /* Execute the function being tested */
    DS_CmdSetDestAge((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST AGE command length: expected = 16, actual = 1"),
        "Invalid DEST AGE command length: expected = 16, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestAge_Test_InvalidCommandLength */

void DS_CmdSetDestAge_Test_InvalidFileTableIndex(void)
{
    DS_DestAgeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestAgeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_AGE_CC);

    CmdPacket.FileTableIndex  = 99;
    CmdPacket.MaxFileAge = 1000;

    /* Execute the function being tested */
    DS_CmdSetDestAge((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST AGE command arg: file table index = 99"),
        "Invalid DEST AGE command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestAge_Test_InvalidFileTableIndex */

void DS_CmdSetDestAge_Test_InvalidFileAgeLimit(void)
{
    DS_DestAgeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestAgeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_AGE_CC);

    CmdPacket.FileTableIndex  = 1;
    CmdPacket.MaxFileAge     = 1;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestAge((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST AGE command arg: age limit = 1"),
        "Invalid DEST AGE command arg: age limit = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestAge_Test_InvalidFileAgeLimit */

void DS_CmdSetDestAge_Test_FileTableNotLoaded(void)
{
    DS_DestAgeCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestAgeCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_AGE_CC);

    CmdPacket.FileTableIndex  = 1;
    CmdPacket.MaxFileAge = 1000;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestAge((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST AGE command: destination file table is not loaded"),
        "Invalid DEST AGE command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestAge_Test_FileTableNotLoaded */

void DS_CmdSetDestCount_Test_Nominal(void)
{
    DS_DestCountCmd_t       CmdPacket;
    DS_DestFileTable_t      DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestCountCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_COUNT_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.SequenceCount  = 1;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdSetDestCount((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].SequenceCount == 1,
        "DS_AppData.DestFileTblPtr->File[CmdPacket.FileTableIndex].SequenceCount == 1");

    UtAssert_True
        (DS_AppData.FileStatus[CmdPacket.FileTableIndex].FileCount == 1,
        "DS_AppData.FileStatus[CmdPacket.FileTableIndex].FileCount == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SEQ_CMD_EID, CFE_EVS_DEBUG, "DEST COUNT command: file table index = 1, sequence count = 1"),
        "DEST COUNT command: file table index = 1, sequence count = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_DestCountCmd_t), "DS_DestCountCmd_t is 32-bit aligned");
} /* end DS_CmdSetDestCount_Test_Nominal */

void DS_CmdSetDestCount_Test_InvalidCommandLength(void)
{
    DS_DestCountCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_COUNT_CC);

    CmdPacket.FileTableIndex = 1;
    CmdPacket.SequenceCount  = 1;

    /* Execute the function being tested */
    DS_CmdSetDestCount((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST COUNT command length: expected = 16, actual = 1"),
        "Invalid DEST COUNT command length: expected = 16, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestCount_Test_InvalidCommandLength */

void DS_CmdSetDestCount_Test_InvalidFileTableIndex(void)
{
    DS_DestCountCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestCountCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_COUNT_CC);

    CmdPacket.FileTableIndex  = 99;
    CmdPacket.SequenceCount  = 1;

    /* Execute the function being tested */
    DS_CmdSetDestCount((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST COUNT command arg: file table index = 99"),
        "Invalid DEST COUNT command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestCount_Test_InvalidFileTableIndex */

void DS_CmdSetDestCount_Test_InvalidFileSequenceCount(void)
{
    DS_DestCountCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestCountCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_COUNT_CC);

    CmdPacket.FileTableIndex  = 1;
    CmdPacket.SequenceCount   = -1;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestCount((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST COUNT command arg: sequence count = -1"),
        "Invalid DEST COUNT command arg: sequence count = -1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestCount_Test_InvalidFileSequenceCount */

void DS_CmdSetDestCount_Test_FileTableNotLoaded(void)
{
    DS_DestCountCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_DestCountCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_SET_DEST_COUNT_CC);

    CmdPacket.FileTableIndex  = 1;
    CmdPacket.SequenceCount  = 1;

    DS_AppData.DestFileTblPtr = 0;

    /* Execute the function being tested */
    DS_CmdSetDestCount((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST COUNT command: destination file table is not loaded"),
        "Invalid DEST COUNT command: destination file table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdSetDestCount_Test_FileTableNotLoaded */

#if (DS_MOVE_FILES == FALSE)
void DS_CmdCloseFile_Test_Nominal(void)
{
    DS_CloseFileCmd_t       CmdPacket;
    DS_DestFileTable_t      DestFileTable;
    uint32                  i;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_CLOSE_FILE_CC);

    CmdPacket.FileTableIndex = 0;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.FileStatus[CmdPacket.FileTableIndex].FileHandle = 99;

    for (i = 1; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileHandle = DS_CLOSED_FILE_HANDLE;
    }

    /* Execute the function being tested */
    DS_CmdCloseFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CLOSE_CMD_EID, CFE_EVS_DEBUG, "DEST CLOSE command: file table index = 0"),
        "DEST CLOSE command: file table index = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_CloseFileCmd_t), "DS_CloseFileCmd_t is 32-bit aligned");
} /* end DS_CmdCloseFile_Test_Nominal */
#endif

void DS_CmdCloseFile_Test_InvalidCommandLength(void)
{
    DS_CloseFileCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_CLOSE_FILE_CC);

    CmdPacket.FileTableIndex = 0;

    /* Execute the function being tested */
    DS_CmdCloseFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CLOSE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST CLOSE command length: expected = 12, actual = 1"),
        "Invalid DEST CLOSE command length: expected = 12, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdCloseFile_Test_InvalidCommandLength */

void DS_CmdCloseFile_Test_InvalidFileTableIndex(void)
{
    DS_CloseFileCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseFileCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_CLOSE_FILE_CC);

    CmdPacket.FileTableIndex = 99;

    /* Execute the function being tested */
    DS_CmdCloseFile((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CLOSE_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST CLOSE command arg: file table index = 99"),
        "Invalid DEST CLOSE command arg: file table index = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdCloseFile_Test_InvalidFileTableIndex */

void DS_CmdCloseAll_Test_Nominal(void)
{
    DS_CloseAllCmd_t       CmdPacket;
    DS_DestFileTable_t     DestFileTable;
    uint32                 i;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_CloseAllCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_CLOSE_ALL_CC);

    for (i = 1; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileHandle = DS_CLOSED_FILE_HANDLE;
    }

#if (DS_MOVE_FILES == TRUE)
    strncpy (DS_AppData.DestFileTblPtr->File[0].Movename, "", DS_PATHNAME_BUFSIZE);
#endif

    /* Execute the function being tested */
    DS_CmdCloseAll((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CLOSE_ALL_CMD_EID, CFE_EVS_DEBUG, "DEST CLOSE ALL command"),
        "DEST CLOSE ALL command");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_CloseAllCmd_t), "DS_CloseAllCmd_t is 32-bit aligned");
} /* end DS_CmdCloseAll_Test_Nominal */

void DS_CmdCloseAll_Test_InvalidCommandLength(void)
{
    DS_CloseAllCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_CLOSE_ALL_CC);

    /* Execute the function being tested */
    DS_CmdCloseAll((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CLOSE_ALL_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid DEST CLOSE ALL command length: expected = 8, actual = 1"),
        "Invalid DEST CLOSE ALL command length: expected = 8, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdCloseAll_Test_InvalidCommandLength */

void DS_CmdGetFileInfo_Test_EnabledOpen(void)
{
    DS_GetFileInfoCmd_t    CmdPacket;
    DS_DestFileTable_t     DestFileTable;
    uint32                 i;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_GetFileInfoCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_GET_FILE_INFO_CC);

    DS_AppData.DestFileTblPtr = &DestFileTable;

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileAge    = 1;
        DS_AppData.FileStatus[i].FileSize   = 2;
        DS_AppData.FileStatus[i].FileRate   = 3;
        DS_AppData.FileStatus[i].FileCount  = 4;
        DS_AppData.FileStatus[i].FileState  = 5;
        DS_AppData.FileStatus[i].FileHandle = 6;
        strncpy (DS_AppData.FileStatus[i].FileName, "filename", OS_MAX_PATH_LEN);
    }

    /* Execute the function being tested */
    DS_CmdGetFileInfo((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");
    /* Changes to DS_FileInfoPkt cannot easily be verified because DS_FileInfoPkt is a local variable */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (TLM_STRUCT_DATA_IS_32_ALIGNED(DS_FileInfoPkt_t), "DS_FileInfoPkt_t is 32-bit aligned");

} /* end DS_CmdGetFileInfo_Test_EnabledOpen */

void DS_CmdGetFileInfo_Test_DisabledClosed(void)
{
    DS_GetFileInfoCmd_t    CmdPacket;
    uint32                 i;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_GetFileInfoCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_GET_FILE_INFO_CC);

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileAge    = 1;
        DS_AppData.FileStatus[i].FileSize   = 2;
        DS_AppData.FileStatus[i].FileRate   = 3;
        DS_AppData.FileStatus[i].FileCount  = 4;
        DS_AppData.FileStatus[i].FileState  = 5;
        DS_AppData.FileStatus[i].FileHandle = DS_CLOSED_FILE_HANDLE;
        strncpy (DS_AppData.FileStatus[i].FileName, "filename", OS_MAX_PATH_LEN);
    }

    /* Execute the function being tested */
    DS_CmdGetFileInfo((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");
    /* Changes to DS_FileInfoPkt cannot easily be verified because DS_FileInfoPkt is a local variable */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end DS_CmdGetFileInfo_Test_DisabledClosed */

void DS_CmdGetFileInfo_Test_InvalidCommandLength(void)
{
    DS_GetFileInfoCmd_t   CmdPacket;
    DS_DestFileTable_t    DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_GET_FILE_INFO_CC);

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Execute the function being tested */
    DS_CmdGetFileInfo((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_GET_FILE_INFO_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid GET FILE INFO command length: expected = 8, actual = 1"),
        "Invalid GET FILE INFO command length: expected = 8, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdGetFileInfo_Test_InvalidCommandLength */

void DS_CmdAddMID_Test_Nominal(void)
{
    DS_AddMidCmd_t        CmdPacket;
    DS_FilterTable_t      FilterTable;
    int32                 FilterTableIndex;
    DS_HashLink_t         HashLink;

    /* Verify command struct size minus header is at least explicitly padded to 32-bit boundaries */
    UtAssert_True (CMD_STRUCT_DATA_IS_32_ALIGNED(DS_AddMidCmd_t), "DS_AddMidCmd_t is 32-bit aligned");

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AddMidCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_ADD_MID_CC);

    CmdPacket.MessageID = 0x18BB;

    DS_AppData.FilterTblPtr = &FilterTable;

    DS_AppData.HashTable[0] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr->Packet[HashLink.Index].MessageID = DS_UNUSED;
    HashLink.Next = (struct DS_HashTag *)&DS_AppData.HashTable[1];

    DS_AppData.HashTable[1] = &HashLink;
    HashLink.Index = 1;
    HashLink.Next = (struct DS_HashTag *)&DS_AppData.HashTable[2];

    DS_AppData.FilterTblPtr->Packet[HashLink.Index].MessageID = 0x0005;

    FilterTableIndex = 0;

    /* Execute the function being tested */
    DS_CmdAddMID((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdAcceptedCounter == 1, "DS_AppData.CmdAcceptedCounter == 1");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].MessageID == 0x18BB,
        "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].MessageID == 0x18BB");

    /* Check first elements */
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].FileTableIndex == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].FileTableIndex == 0");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].FilterType == DS_BY_COUNT, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].FilterType == DS_BY_COUNT");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].Algorithm_N == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].Algorithm_N == 0");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].Algorithm_X == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].Algorithm_X == 0");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].Algorithm_O == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[0].Algorithm_O == 0");

    /* Check middle elements */
    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].FileTableIndex == 0,
        "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].FileTableIndex == 0");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].FilterType == DS_BY_COUNT,
        "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].FilterType == DS_BY_COUNT");

    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].Algorithm_N == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].Algorithm_N == 0");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].Algorithm_X == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].Algorithm_X == 0");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].Algorithm_O == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET/2].Algorithm_O == 0");

    /* Check last elements */
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].FileTableIndex == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].FileTableIndex == 0");

    UtAssert_True
        (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].FilterType == DS_BY_COUNT,
        "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].FilterType == DS_BY_COUNT");

    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].Algorithm_N == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].Algorithm_N == 0");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].Algorithm_X == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].Algorithm_X == 0");
    UtAssert_True (DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].Algorithm_O == 0, "DS_AppData.FilterTblPtr->Packet[FilterTableIndex].Filter[DS_FILTERS_PER_PACKET-1].Algorithm_O == 0");

    UtAssert_True (DS_AppData.HashLinks[0].Index == 0, "DS_AppData.HashLinks[0].Index == 0");
    UtAssert_True (DS_AppData.HashLinks[0].MessageID == 0x18BB, "DS_AppData.HashLinks[0].MessageID == 0x18BB");
    
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ADD_MID_CMD_EID, CFE_EVS_DEBUG, "ADD MID command: MID = 0x18BB, index = 0"),
        "ADD MID command: MID = 0x18BB, index = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdAddMID_Test_Nominal */

void DS_CmdAddMID_Test_InvalidCommandLength(void)
{
    DS_AddMidCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, 1, TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_ADD_MID_CC);

    CmdPacket.MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdAddMID((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid ADD MID command length: expected = 12, actual = 1"),
        "Invalid ADD MID command length: expected = 12, actual = 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdAddMID_Test_InvalidCommandLength */

void DS_CmdAddMID_Test_InvalidMessageID(void)
{
    DS_AddMidCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AddMidCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_ADD_MID_CC);

    CmdPacket.MessageID = DS_UNUSED;

    /* Execute the function being tested */
    DS_CmdAddMID((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid ADD MID command arg: invalid MID = 0x   0"),
        "Invalid ADD MID command arg: invalid MID = 0x   0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdAddMID_Test_InvalidMessageID */

void DS_CmdAddMID_Test_FilterTableNotLoaded(void)
{
    DS_AddMidCmd_t      CmdPacket;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AddMidCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_ADD_MID_CC);

    CmdPacket.MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_CmdAddMID((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid ADD MID command: filter table is not loaded"),
        "Invalid ADD MID command: filter table is not loaded");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdAddMID_Test_FilterTableNotLoaded */

void DS_CmdAddMID_Test_MIDAlreadyInFilterTable(void)
{
    DS_AddMidCmd_t      CmdPacket;
    DS_FilterTable_t    FilterTable;
    DS_HashLink_t       HashLink;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AddMidCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_ADD_MID_CC);

    CmdPacket.MessageID = 0x18BB;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.FilterTblPtr->Packet->MessageID = 0x18BB;
    
    /* Execute the function being tested */
    DS_CmdAddMID((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid ADD MID command: MID = 0x18BB is already in filter table at index = 0"),
        "Invalid ADD MID command: MID = 0x18BB is already in filter table at index = 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdAddMID_Test_MIDAlreadyInFilterTable */

void DS_CmdAddMID_Test_FilterTableFull(void)
{
    DS_AddMidCmd_t      CmdPacket;
    DS_FilterTable_t    FilterTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_AddMidCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_ADD_MID_CC);

    CmdPacket.MessageID = 0x18BB;

    DS_AppData.FilterTblPtr = &FilterTable;

    /* Execute the function being tested */
    DS_CmdAddMID((CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.CmdRejectedCounter == 1, "DS_AppData.CmdRejectedCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR, "Invalid ADD MID command: filter table is full"),
        "Invalid ADD MID command: filter table is full");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_CmdAddMID_Test_FilterTableFull */

void DS_Cmds_Test_AddTestCases(void)
{
    UtTest_Add(DS_CmdNoop_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdNoop_Test_Nominal");
    UtTest_Add(DS_CmdNoop_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdNoop_Test_InvalidCommandLength");

    UtTest_Add(DS_CmdReset_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdReset_Test_Nominal");
    UtTest_Add(DS_CmdReset_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdReset_Test_InvalidCommandLength");

    UtTest_Add(DS_CmdSetAppState_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetAppState_Test_Nominal");
    UtTest_Add(DS_CmdSetAppState_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetAppState_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetAppState_Test_InvalidAppState, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetAppState_Test_InvalidAppState");

    UtTest_Add(DS_CmdSetFilterFile_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterFile_Test_Nominal");
    UtTest_Add(DS_CmdSetFilterFile_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterFile_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetFilterFile_Test_InvalidMessageID, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterFile_Test_InvalidMessageID");
    UtTest_Add(DS_CmdSetFilterFile_Test_InvalidFilterParametersIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterFile_Test_InvalidFilterParametersIndex");
    UtTest_Add(DS_CmdSetFilterFile_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterFile_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetFilterFile_Test_FilterTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterFile_Test_FilterTableNotLoaded");
    UtTest_Add(DS_CmdSetFilterFile_Test_MessageIDNotInFilterTable, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterFile_Test_MessageIDNotInFilterTable");

    UtTest_Add(DS_CmdSetFilterType_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterType_Test_Nominal");
    UtTest_Add(DS_CmdSetFilterType_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterType_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetFilterType_Test_InvalidMessageID, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterType_Test_InvalidMessageID");
    UtTest_Add(DS_CmdSetFilterType_Test_InvalidFilterParametersIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterType_Test_InvalidFilterParametersIndex");
    UtTest_Add(DS_CmdSetFilterType_Test_InvalidFilterType, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterType_Test_InvalidFilterType");
    UtTest_Add(DS_CmdSetFilterType_Test_FilterTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterType_Test_FilterTableNotLoaded");
    UtTest_Add(DS_CmdSetFilterType_Test_MessageIDNotInFilterTable, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterType_Test_MessageIDNotInFilterTable");

    UtTest_Add(DS_CmdSetFilterParms_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterParms_Test_Nominal");
    UtTest_Add(DS_CmdSetFilterParms_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterParms_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetFilterParms_Test_InvalidMessageID, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterParms_Test_InvalidMessageID");
    UtTest_Add(DS_CmdSetFilterParms_Test_InvalidFilterParametersIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterParms_Test_InvalidFilterParametersIndex");
    UtTest_Add(DS_CmdSetFilterParms_Test_InvalidFilterAlgorithm, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterParms_Test_InvalidFilterAlgorithm");
    UtTest_Add(DS_CmdSetFilterParms_Test_FilterTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterParms_Test_FilterTableNotLoaded");
    UtTest_Add(DS_CmdSetFilterParms_Test_MessageIDNotInFilterTable, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetFilterParms_Test_MessageIDNotInFilterTable");

    UtTest_Add(DS_CmdSetDestType_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestType_Test_Nominal");
    UtTest_Add(DS_CmdSetDestType_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestType_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestType_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestType_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestType_Test_InvalidFilenameType, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestType_Test_InvalidFilenameType");
    UtTest_Add(DS_CmdSetDestType_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestType_Test_FileTableNotLoaded");

    UtTest_Add(DS_CmdSetDestState_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestState_Test_Nominal");
    UtTest_Add(DS_CmdSetDestState_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestState_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestState_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestState_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestState_Test_InvalidFileState, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestState_Test_InvalidFileState");
    UtTest_Add(DS_CmdSetDestState_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestState_Test_FileTableNotLoaded");

    UtTest_Add(DS_CmdSetDestPath_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestPath_Test_Nominal");
    UtTest_Add(DS_CmdSetDestPath_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestPath_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestPath_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestPath_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestPath_Test_InvalidPathname, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestPath_Test_InvalidPathname");
    UtTest_Add(DS_CmdSetDestPath_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestPath_Test_FileTableNotLoaded");

    UtTest_Add(DS_CmdSetDestBase_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestBase_Test_Nominal");
    UtTest_Add(DS_CmdSetDestBase_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestBase_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestBase_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestBase_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestBase_Test_InvalidBaseFilename, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestBase_Test_InvalidBaseFilename");
    UtTest_Add(DS_CmdSetDestBase_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestBase_Test_FileTableNotLoaded");

    UtTest_Add(DS_CmdSetDestExt_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestExt_Test_Nominal");
    UtTest_Add(DS_CmdSetDestExt_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestExt_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestExt_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestExt_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestExt_Test_InvalidFilenameExtension, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestExt_Test_InvalidFilenameExtension");
    UtTest_Add(DS_CmdSetDestExt_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestExt_Test_FileTableNotLoaded");

    UtTest_Add(DS_CmdSetDestSize_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestSize_Test_Nominal");
    UtTest_Add(DS_CmdSetDestSize_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestSize_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestSize_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestSize_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestSize_Test_InvalidFileSizeLimit, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestSize_Test_InvalidFileSizeLimit");
    UtTest_Add(DS_CmdSetDestSize_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestSize_Test_FileTableNotLoaded");

    UtTest_Add(DS_CmdSetDestAge_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestAge_Test_Nominal");
    UtTest_Add(DS_CmdSetDestAge_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestAge_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestAge_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestAge_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestAge_Test_InvalidFileAgeLimit, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestAge_Test_InvalidFileAgeLimit");
    UtTest_Add(DS_CmdSetDestAge_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestAge_Test_FileTableNotLoaded");

    UtTest_Add(DS_CmdSetDestCount_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestCount_Test_Nominal");
    UtTest_Add(DS_CmdSetDestCount_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestCount_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdSetDestCount_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestCount_Test_InvalidFileTableIndex");
    UtTest_Add(DS_CmdSetDestCount_Test_InvalidFileSequenceCount, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestCount_Test_InvalidFileSequenceCount");
    UtTest_Add(DS_CmdSetDestCount_Test_FileTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdSetDestCount_Test_FileTableNotLoaded");

#if (DS_MOVE_FILES == FALSE)
    UtTest_Add(DS_CmdCloseFile_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdCloseFile_Test_Nominal");
#endif
    UtTest_Add(DS_CmdCloseFile_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdCloseFile_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdCloseFile_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_CmdCloseFile_Test_InvalidFileTableIndex");

    UtTest_Add(DS_CmdCloseAll_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdCloseAll_Test_Nominal");
    UtTest_Add(DS_CmdCloseAll_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdCloseAll_Test_InvalidCommandLength");

    UtTest_Add(DS_CmdGetFileInfo_Test_EnabledOpen, DS_Test_Setup, DS_Test_TearDown, "DS_CmdGetFileInfo_Test_EnabledOpen");
    UtTest_Add(DS_CmdGetFileInfo_Test_DisabledClosed, DS_Test_Setup, DS_Test_TearDown, "DS_CmdGetFileInfo_Test_DisabledClosed");
    UtTest_Add(DS_CmdGetFileInfo_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdGetFileInfo_Test_InvalidCommandLength");

    UtTest_Add(DS_CmdAddMID_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_CmdAddMID_Test_Nominal");
    UtTest_Add(DS_CmdAddMID_Test_InvalidCommandLength, DS_Test_Setup, DS_Test_TearDown, "DS_CmdAddMID_Test_InvalidCommandLength");
    UtTest_Add(DS_CmdAddMID_Test_InvalidMessageID, DS_Test_Setup, DS_Test_TearDown, "DS_CmdAddMID_Test_InvalidMessageID");
    UtTest_Add(DS_CmdAddMID_Test_FilterTableNotLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_CmdAddMID_Test_FilterTableNotLoaded");
    UtTest_Add(DS_CmdAddMID_Test_MIDAlreadyInFilterTable, DS_Test_Setup, DS_Test_TearDown, "DS_CmdAddMID_Test_MIDAlreadyInFilterTable");
    UtTest_Add(DS_CmdAddMID_Test_FilterTableFull, DS_Test_Setup, DS_Test_TearDown, "DS_CmdAddMID_Test_FilterTableFull");
} /* end DS_Cmds_Test_AddTestCases */


/************************/
/*  End of File Comment */
/************************/
