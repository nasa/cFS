 /*************************************************************************
 ** File:
 **   $Id: ds_file_test.c 1.5 2017/01/25 12:29:16EST sstrege Exp  $
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
 **   This file contains unit test cases for the functions contained in the file ds_file.c
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

#include "ds_file_test.h"
#include "ds_app.h"
#include "ds_appdefs.h"
#include "ds_file.h"
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

/*
 * Function Definitions
 */

uint8 UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHookRunCount;
int32 UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHook (int32  filedes, void *buffer, uint32 nbytes)
{
    char Buf8[8];

    UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHookRunCount++;

    if (UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHookRunCount == 1)
    {
        return sizeof(uint32);
    }
    else
    {
        return sizeof(Buf8);
    }
} /* end UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHook */

void DS_FileStorePacket_Test_Nominal(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      MessageID = 0x18BB;
    DS_FilterTable_t    FilterTable;
    DS_DestFileTable_t  DestFileTable;
    DS_HashLink_t       HashLink;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[0].MaxFileSize = 10;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr->Packet[0].MessageID = 0x18BB;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].Algorithm_N = 1;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].Algorithm_X = 3;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].Algorithm_O = 0;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].FilterType  = 1;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    /* Execute the function being tested */
    DS_FileStorePacket(MessageID, (CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.PassedPktCounter == 1, "DS_AppData.PassedPktCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_FileStorePacket_Test_Nominal */

void DS_FileStorePacket_Test_PacketNotInTable(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      MessageID = 0x0001;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    /* Execute the function being tested */
    DS_FileStorePacket(MessageID, (CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.IgnoredPktCounter == 1, "DS_AppData.IgnoredPktCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_FileStorePacket_Test_PacketNotInTable */

void DS_FileStorePacket_Test_PassedFilterFalse(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      MessageID = 0x18BB;
    DS_FilterTable_t    FilterTable;
    DS_DestFileTable_t  DestFileTable;
    DS_HashLink_t       HashLink;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    DS_AppData.FilterTblPtr = &FilterTable;
    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[0].MaxFileSize = 10;

    DS_AppData.HashTable[187] = &HashLink;
    HashLink.Index = 0;
    DS_AppData.FilterTblPtr->Packet[0].MessageID = 0x18BB;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].Algorithm_N = 1;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].Algorithm_X = 3;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].Algorithm_O = 1;
    DS_AppData.FilterTblPtr->Packet[0].Filter[0].FilterType  = 1;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    /* Execute the function being tested */
    DS_FileStorePacket(MessageID, (CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FilteredPktCounter == 1, "DS_AppData.FilteredPktCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_FileStorePacket_Test_PassedFilterFalse */

void DS_FileSetupWrite_Test_Nominal(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    DS_AppData.FileStatus[FileIndex].FileHandle = 99;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].MaxFileSize = 100;
    DS_AppData.FileStatus[FileIndex].FileSize = 3;

    /* Execute the function being tested */
    DS_FileSetupWrite(FileIndex, (CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
#if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)
    UtAssert_True (DS_AppData.CurrentPktTime == 0, "DS_AppData.CurrentPktTime == 0");
#endif

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_FileSetupWrite_Test_Nominal */

void DS_FileSetupWrite_Test_FileHandleClosed(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    DS_AppData.FileStatus[FileIndex].FileHandle = DS_CLOSED_FILE_HANDLE;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].MaxFileSize = 100;
    DS_AppData.FileStatus[FileIndex].FileSize = 3;

    /* Execute the function being tested */
    DS_FileSetupWrite(FileIndex, (CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
#if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)
    UtAssert_True (DS_AppData.CurrentPktTime == 0, "DS_AppData.CurrentPktTime == 0");
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
#else
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */
#endif
} /* end DS_FileSetupWrite_Test_FileHandleClosed */

void DS_FileSetupWrite_Test_MaxFileSizeExceeded(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    DS_AppData.FileStatus[FileIndex].FileHandle = 99;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].MaxFileSize = 5;
    DS_AppData.FileStatus[FileIndex].FileSize = 10;

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "directory1/", DS_TOTAL_FNAME_BUFSIZE);

    /* Set to prevent an error message that we don't care about in this test */
    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX, sizeof(CFE_FS_Header_t), 1);

#if (DS_MOVE_FILES == TRUE)
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Movename, "directory2/movename/", DS_PATHNAME_BUFSIZE);
#endif

    /* Execute the function being tested */
    DS_FileSetupWrite(FileIndex, (CFE_SB_MsgPtr_t)&CmdPacket);
    
    /* Verify results */
#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
    UtAssert_True (DS_AppData.CurrentPktTime == 0, "DS_AppData.CurrentPktTime == 0");
#endif

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_FileSetupWrite_Test_MaxFileSizeExceeded */

void DS_FileWriteData_Test_Nominal(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      FileIndex = 0;
    uint32              DataLength = 10;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    /* Set to return DataLength to satisfy condition "if (Result == DataLength)" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, DataLength, 1);

    /* Execute the function being tested */
    DS_FileWriteData(FileIndex, (CFE_SB_MsgPtr_t)&CmdPacket, DataLength);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileWriteCounter == 1, "DS_AppData.FileWriteCounter == 1");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileSize == 10, "DS_AppData.FileStatus[FileIndex].FileSize == 10");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileGrowth == 10, "DS_AppData.FileStatus[FileIndex].FileGrowth == 10");

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
    UtAssert_True (DS_AppData.LastPktTime[FileIndex] == 0, "DS_AppData.LastPktTime[FileIndex] == 0");
#endif

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileWriteData_Test_Nominal */

void DS_FileWriteData_Test_Error(void)
{
    DS_NoopCmd_t        CmdPacket;
    CFE_SB_MsgId_t      FileIndex = 0;
    uint32              DataLength = 10;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    CFE_SB_InitMsg (&CmdPacket, DS_CMD_MID, sizeof(DS_NoopCmd_t), TRUE);
    CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&CmdPacket, DS_NOOP_CC);

    /* Set to reach error case being tested (DS_FileWriteError) */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, -1, 1);

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "directory1/", DS_TOTAL_FNAME_BUFSIZE);

    /* Execute the function being tested */
    DS_FileWriteData(FileIndex, (CFE_SB_MsgPtr_t)&CmdPacket, DataLength);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_WRITE_FILE_ERR_EID, CFE_EVS_ERROR, "FILE WRITE error: result = -1, length = 10, dest = 0, name = 'directory1/'"),
        "FILE WRITE error: result = -1, length = 10, dest = 0, name = 'directory1/'");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_FileWriteData_Test_Error */

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileWriteHeader_Test_PlatformConfigCFE_Nominal(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = 1;

    /* Set to satisfy condition "if (Result == sizeof(CFE_FS_Header_t))" */
    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX, sizeof(CFE_FS_Header_t), 1);

    /* Set to satisfy condition "if (Result == sizeof(DS_FileHeader_t))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, sizeof(DS_FileHeader_t), 1);

    /* Execute the function being tested */
    DS_FileWriteHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
    UtAssert_True (DS_AppData.FileWriteCounter == 2, "DS_AppData.FileWriteCounter == 2");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileSize == sizeof(CFE_FS_Header_t) + sizeof(DS_FileHeader_t), "sizeof(CFE_FS_Header_t) + sizeof(DS_FileHeader_t)");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileGrowth == sizeof(CFE_FS_Header_t) + sizeof(DS_FileHeader_t), "sizeof(CFE_FS_Header_t) + sizeof(DS_FileHeader_t)");

} /* end DS_FileWriteHeader_Test_PlatformConfigCFE_Nominal */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileWriteHeader_Test_PrimaryHeaderError(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = 1;

    /* Set to generate primary header error */
    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_FileWriteHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end DS_FileWriteHeader_Test_PrimaryHeaderError */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileWriteHeader_Test_SecondaryHeaderError(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = 1;

    /* Set to satisfy condition "if (Result == sizeof(CFE_FS_Header_t))" */
    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX, sizeof(CFE_FS_Header_t), 1);

    /* Set to generate secondary header error */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_FileWriteHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end DS_FileWriteHeader_Test_SecondaryHeaderError */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileWriteHeader_Test_PlatformConfigGPM_Nominal(void)
{
    CFE_SB_MsgId_t   FileIndex = 0;

    /* Set to satisfy condition "if (Result == sizeof(DS_FileHeaderGPM_t))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, sizeof(DS_FileHeaderGPM_t), 1);

    /* Execute the function being tested */
    DS_FileWriteHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileWriteCounter == 1, "DS_AppData.FileWriteCounter == 1");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileSize == sizeof(DS_FileHeaderGPM_t), "DS_AppData.FileStatus[FileIndex].FileSize == sizeof(DS_FileHeaderGPM_t)");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileGrowth == sizeof(DS_FileHeaderGPM_t), "DS_AppData.FileStatus[FileIndex].FileGrowth == sizeof(DS_FileHeaderGPM_t)");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileWriteHeader_Test_PlatformConfigGPM_Nominal */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileWriteHeader_Test_PlatformConfigGPM_HeaderError(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "directory1/filename", DS_TOTAL_FNAME_BUFSIZE);

#if (DS_MOVE_FILES == TRUE)
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Movename, "directory2/movename/", DS_PATHNAME_BUFSIZE);
#endif

    /* Set to generate error */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_FileWriteHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
    /* Generates 1 event message we don't care about in this test */

} /* end DS_FileWriteHeader_Test_PlatformConfigGPM_HeaderError */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileWriteError_Test(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    uint32              DataLength = 10;
    int32               WriteResult = -1;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = 1;

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "filename", OS_MAX_PATH_LEN);

    /* Execute the function being tested */
    DS_FileWriteError(FileIndex, DataLength, WriteResult);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileWriteErrCounter == 1, "DS_AppData.FileWriteErrCounter == 1");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_WRITE_FILE_ERR_EID, CFE_EVS_ERROR, "FILE WRITE error: result = -1, length = 10, dest = 0, name = 'filename'"),
        "FILE WRITE error: result = -1, length = 10, dest = 0, name = 'filename'");

    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileState == DS_DISABLED, "DS_AppData.FileStatus[FileIndex].FileState == DS_DISABLED");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_FileWriteError_Test */
#endif

void DS_FileCreateDest_Test_Nominal(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_COUNT;
    DS_AppData.DestFileTblPtr->File[FileIndex].SequenceCount = 5;

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "filename", OS_MAX_PATH_LEN);

    DS_AppData.FileStatus[FileIndex].FileHandle = 99;
    DS_AppData.FileStatus[FileIndex].FileCount = DS_MAX_SEQUENCE_COUNT + 1;

    /* Set to fail the condition "if (Result < 0)" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, 5, 1);

    /* Set to prevent error messages in subfunction DS_FileWriteHeader that we don't care about in this test */
    Ut_CFE_FS_SetReturnCode(UT_CFE_FS_WRITEHDR_INDEX, sizeof(CFE_FS_Header_t), 1);

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
    /* Set to prevent error messages in subfunction DS_FileWriteHeader that we don't care about in this test */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, sizeof(DS_FileHeader_t), 1);
#endif

    /* Execute the function being tested */
    DS_FileCreateDest(FileIndex);
    
    /* Verify results */
#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
    UtAssert_True (DS_AppData.FileWriteCounter == 3, "DS_AppData.FileWriteCounter == 3");
#elif DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
    UtAssert_True (DS_AppData.FileWriteCounter == 2, "DS_AppData.FileWriteCounter == 2");
#endif
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileHandle == 5, "DS_AppData.FileStatus[FileIndex].FileHandle == 5");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileCount == 5, "DS_AppData.FileStatus[FileIndex].FileCount == 5");


    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCreateDest_Test_Nominal */

void DS_FileCreateDest_Test_Error(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_COUNT;
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Pathname, "path", OS_MAX_PATH_LEN);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Basename, "basename", OS_MAX_PATH_LEN);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Extension, "extension", OS_MAX_PATH_LEN);

    DS_AppData.FileStatus[FileIndex].FileHandle = 99;
    DS_AppData.FileStatus[FileIndex].FileCount = DS_MAX_SEQUENCE_COUNT + 1;

    /* Set to generate error message DS_CREATE_FILE_ERR_EID */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_CREAT_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_FileCreateDest(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileWriteErrCounter == 1, "DS_AppData.FileWriteErrCounter == 1");

    /* For this test it doesn't matter what filename results from the call to DS_FileCreateName. */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_CREATE_FILE_ERR_EID, CFE_EVS_ERROR, "FILE CREATE error: result = -1, dest = 0, name = 'path/basename.extension'"),
        "FILE CREATE error: result = -1, dest = 0, name = 'path/basename.extension'");

    UtAssert_True
        (strncmp (DS_AppData.FileStatus[FileIndex].FileName, "", DS_TOTAL_FNAME_BUFSIZE) == 0,
        "strncmp (DS_AppData.FileStatus[FileIndex].FileName, "", DS_TOTAL_FNAME_BUFSIZE) == 0");

    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileState == DS_DISABLED, "DS_AppData.FileStatus[FileIndex].FileState == DS_DISABLED");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_FileCreateDest_Test_Error */

void DS_FileCreateName_Test_Nominal(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_COUNT;
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Pathname, "path", OS_MAX_PATH_LEN);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Basename, "basename", OS_MAX_PATH_LEN);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Extension, "extension", OS_MAX_PATH_LEN);

    DS_AppData.FileStatus[FileIndex].FileCount = 1;


    /* Execute the function being tested */
    DS_FileCreateName(FileIndex);
    
    /* Verify results */
    UtAssert_True
        (strncmp (DS_AppData.FileStatus[FileIndex].FileName, "path/basename.extension", DS_TOTAL_FNAME_BUFSIZE) == 0,
        "strncmp (DS_AppData.FileStatus[FileIndex].FileName, 'path/basename.extension', DS_TOTAL_FNAME_BUFSIZE) == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCreateName_Test_Nominal */

void DS_FileCreateName_Test_Error(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;
    int32               i;
    char                EventMessage[300];

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_COUNT;
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Pathname, "path", OS_MAX_PATH_LEN);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Extension, "extension", OS_MAX_PATH_LEN);

    for (i = 0; i < DS_TOTAL_FNAME_BUFSIZE - 2; i++)
    {
        DS_AppData.DestFileTblPtr->File[FileIndex].Basename[i] = 'a';
    }

    DS_AppData.DestFileTblPtr->File[FileIndex].Basename[DS_TOTAL_FNAME_BUFSIZE - 1] = DS_STRING_TERMINATOR;

    /* Execute the function being tested */
    DS_FileCreateName(FileIndex);
    
    /* Verify results */
    sprintf(EventMessage, "FILE NAME error: dest = 0, path = 'path', base = '%s', seq = ", DS_AppData.DestFileTblPtr->File[FileIndex].Basename);
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FILE_NAME_ERR_EID, CFE_EVS_ERROR, EventMessage),
        EventMessage);

    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileState == DS_DISABLED, "DS_AppData.FileStatus[FileIndex].FileState == DS_DISABLED");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_FileCreateName_Test_Error */

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileCreateSequence_Test_ByCount(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    char Sequence[DS_TOTAL_FNAME_BUFSIZE];

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_COUNT;

    DS_AppData.FileStatus[FileIndex].FileCount = 1;


    /* Execute the function being tested */
    DS_FileCreateSequence(Sequence, DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType, DS_AppData.FileStatus[FileIndex].FileCount);
    
    /* Verify results */
    UtAssert_True (strncmp(Sequence, "00000001", DS_TOTAL_FNAME_BUFSIZE) == 0, "strncmp(Sequence, '00000001', DS_TOTAL_FNAME_BUFSIZE) == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCreateSequence_Test_ByCount */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileCreateSequence_Test_ByTime(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    char Sequence[DS_TOTAL_FNAME_BUFSIZE];

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_TIME;

    DS_AppData.FileStatus[FileIndex].FileCount = 1;


    /* Execute the function being tested */
    DS_FileCreateSequence(Sequence, DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType, DS_AppData.FileStatus[FileIndex].FileCount);
    
    /* Verify results */
    UtAssert_True (strncmp(Sequence, "1980001000000", DS_TOTAL_FNAME_BUFSIZE) == 0, "strncmp(Sequence, '1980001000000', DS_TOTAL_FNAME_BUFSIZE) == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCreateSequence_Test_ByTime */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileCreateSequence_Test_BadFilenameType(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    char Sequence[DS_TOTAL_FNAME_BUFSIZE];

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_TIME;

    DS_AppData.FileStatus[FileIndex].FileCount = 1;

    /* Execute the function being tested */
    DS_FileCreateSequence(Sequence, 99, DS_AppData.FileStatus[FileIndex].FileCount);
    
    /* Verify results */
    UtAssert_True (strncmp(Sequence, "", DS_TOTAL_FNAME_BUFSIZE) == 0, "strncmp(Sequence, '', DS_TOTAL_FNAME_BUFSIZE) == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCreateSequence_Test_BadFilenameType */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileCreateSequence_Test_PlatformConfigGPM_ByTime(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    char Sequence[DS_TOTAL_FNAME_BUFSIZE];

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType = DS_BY_TIME;

    DS_AppData.FileStatus[FileIndex].FileCount = 1;

    DS_AppData.CurrentPktTime = 1;

    /* Execute the function being tested */
    DS_FileCreateSequence(Sequence, DS_AppData.DestFileTblPtr->File[FileIndex].FileNameType, DS_AppData.FileStatus[FileIndex].FileCount);
    
    /* Verify results */
    UtAssert_True (strncmp(Sequence, "1980001000001", DS_TOTAL_FNAME_BUFSIZE) == 0, "strncmp(Sequence, '1980001000000', DS_TOTAL_FNAME_BUFSIZE) == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCreateSequence_Test_PlatformConfigGPM_ByTime */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileUpdateHeader_Test_PlatformConfigCFE_Nominal(void)
{
    CFE_SB_MsgId_t     FileIndex = 0;

    /* Set to satisfy condition "if (Result == sizeof(CFE_FS_Header_t))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_LSEEK_INDEX, sizeof(CFE_FS_Header_t), 1);

    /* Set to satisfy condition "if (Result == sizeof(CFE_TIME_SysTime_t))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, sizeof(CFE_TIME_SysTime_t), 1);

    /* Execute the function being tested */
    DS_FileUpdateHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileUpdateCounter == 1, "DS_AppData.FileUpdateCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileUpdateHeader_Test_PlatformConfigCFE_Nominal */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileUpdateHeader_Test_WriteError(void)
{
    CFE_SB_MsgId_t     FileIndex = 0;

    /* Set to satisfy condition "if (Result == sizeof(CFE_FS_Header_t))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_LSEEK_INDEX, sizeof(CFE_FS_Header_t), 1);

    /* Set to fail condition "if (Result == sizeof(CFE_TIME_SysTime_t))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_FileUpdateHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileUpdateErrCounter == 1, "DS_AppData.FileUpdateErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileUpdateHeader_Test_WriteError */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
void DS_FileUpdateHeader_Test_PlatformConfigCFE_SeekError(void)
{
    CFE_SB_MsgId_t     FileIndex = 0;

    /* Set to fail condition "if (Result == sizeof(CFE_FS_Header_t))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_LSEEK_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_FileUpdateHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileUpdateErrCounter == 1, "DS_AppData.FileUpdateErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileUpdateHeader_Test_PlatformConfigCFE_SeekError */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileUpdateHeader_Test_PlatformConfigGPM_Nominal(void)
{
    CFE_SB_MsgId_t     FileIndex = 0;

    UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHookRunCount = 0;

    /* Set to satisfy condition  "if (Result == 16)" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_LSEEK_INDEX, 16, 1);

    /* Causes function to return sizeof(uint32) on first run and sizeof(Buf8) on second run, to satisfy both 
     * if-statements that check result of OS_write */
    Ut_OSFILEAPI_SetFunctionHook(UT_OSFILEAPI_WRITE_INDEX, &UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHook);

    /* Execute the function being tested */
    DS_FileUpdateHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileUpdateCounter == 1, "DS_AppData.FileUpdateCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileUpdateHeader_Test_PlatformConfigGPM_Nominal */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteBufError(void)
{
    CFE_SB_MsgId_t     FileIndex = 0;

    UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHookRunCount = 0;

    /* Set to satisfy condition  "if (Result == 16)" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_LSEEK_INDEX, 16, 1);

    /* Set to fail condition "if (Result == sizeof(Buf8))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, sizeof(uint32), 1);
    Ut_OSFILEAPI_ContinueReturnCodeAfterCountZero(UT_OSFILEAPI_WRITE_INDEX);

    /* Execute the function being tested */
    DS_FileUpdateHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileUpdateErrCounter == 1, "DS_AppData.FileUpdateErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteBufError */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteLastPktTimeError(void)
{
    CFE_SB_MsgId_t     FileIndex = 0;

    UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHookRunCount = 0;

    /* Set to satisfy condition  "if (Result == 16)" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_LSEEK_INDEX, 16, 1);

    /* Set to fail condition "if (Result == sizeof(uint32))" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_WRITE_INDEX, 0, 1);

    /* Execute the function being tested */
    DS_FileUpdateHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileUpdateErrCounter == 1, "DS_AppData.FileUpdateErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteLastPktTimeError */
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileUpdateHeader_Test_PlatformConfigGPM_SeekError(void)
{
    CFE_SB_MsgId_t     FileIndex = 0;

    UT_DS_FILE_TEST_CFE_OSFILEAPI_WriteHookRunCount = 0;

    /* Set to fail condition "if (Result == 16)" */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_LSEEK_INDEX, 0, 1);

    /* Execute the function being tested */
    DS_FileUpdateHeader(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileUpdateErrCounter == 1, "DS_AppData.FileUpdateErrCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileUpdateHeader_Test_PlatformConfigGPM_SeekError */
#endif

#if (DS_MOVE_FILES == TRUE)
void DS_FileCloseDest_Test_PlatformConfigMoveFiles_Nominal(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "directory1/filename", DS_TOTAL_FNAME_BUFSIZE);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Movename, "directory2/movename/", DS_PATHNAME_BUFSIZE);

    /* Execute the function being tested */
    DS_FileCloseDest(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE, "DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileAge == 0, "DS_AppData.FileStatus[FileIndex].FileAge == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileSize == 0, "DS_AppData.FileStatus[FileIndex].FileSize == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileName[0] == 0, "DS_AppData.FileStatus[FileIndex].FileName[0] == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCloseDest_Test_PlatformConfigMoveFiles_Nominal */
#endif

#if (DS_MOVE_FILES == TRUE)
void DS_FileCloseDest_Test_PlatformConfigMoveFiles_MoveError(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "directory1/filename", DS_TOTAL_FNAME_BUFSIZE);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Movename, "directory2/movename/", DS_PATHNAME_BUFSIZE);

    /* Set to generate error message DS_MOVE_FILE_ERR_EID */
    Ut_OSFILEAPI_SetReturnCode(UT_OSFILEAPI_MV_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_FileCloseDest(FileIndex);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_MOVE_FILE_ERR_EID, CFE_EVS_ERROR, "FILE MOVE error: src = 'directory1/filename', tgt = 'directory2/movename/filename', result = -1"),
        "FILE MOVE error: src = 'directory1/filename', tgt = 'directory2/movename/filename', result = -1");

    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE, "DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileAge == 0, "DS_AppData.FileStatus[FileIndex].FileAge == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileSize == 0, "DS_AppData.FileStatus[FileIndex].FileSize == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileName[0] == 0, "DS_AppData.FileStatus[FileIndex].FileName[0] == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_FileCloseDest_Test_PlatformConfigMoveFiles_MoveError */
#endif

#if (DS_MOVE_FILES == TRUE)
void DS_FileCloseDest_Test_PlatformConfigMoveFiles_FilenameTooLarge(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    strncpy (DS_AppData.FileStatus[FileIndex].FileName, "directory1/filenamefilenamefilenamefilenamefilenamefilename", DS_TOTAL_FNAME_BUFSIZE);
    strncpy (DS_AppData.DestFileTblPtr->File[FileIndex].Movename, "directory2/movename/", DS_PATHNAME_BUFSIZE);

    /* Execute the function being tested */
    DS_FileCloseDest(FileIndex);
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_MOVE_FILE_ERR_EID, CFE_EVS_ERROR, "FILE MOVE error: dir name = 'directory2/movename', filename = '/filenamefilenamefilenamefilenamefilenamefilename'"),
        "FILE MOVE error: dir name = 'directory2/movename', filename = '/filenamefilenamefilen'");

    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE, "DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileAge == 0, "DS_AppData.FileStatus[FileIndex].FileAge == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileSize == 0, "DS_AppData.FileStatus[FileIndex].FileSize == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileName[0] == 0, "DS_AppData.FileStatus[FileIndex].FileName[0] == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end DS_FileCloseDest_Test_PlatformConfigMoveFiles_FilenameTooLarge */
#endif

#if (DS_MOVE_FILES == FALSE)
void DS_FileCloseDest_Test_MoveFilesFalse(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;

    /* Execute the function being tested */
    DS_FileCloseDest(FileIndex);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE, "DS_AppData.FileStatus[FileIndex].FileHandle == DS_CLOSED_FILE_HANDLE");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileAge == 0, "DS_AppData.FileStatus[FileIndex].FileAge == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileSize == 0, "DS_AppData.FileStatus[FileIndex].FileSize == 0");
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileName[0] == 0, "DS_AppData.FileStatus[FileIndex].FileName[0] == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileCloseDest_Test_MoveFilesFalse */
#endif

void DS_FileTestAge_Test_Nominal(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    uint32              ElapsedSeconds = 2;
    DS_DestFileTable_t  DestFileTable;
    uint32              i;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        strncpy (DS_AppData.FileStatus[i].FileName, "directory1/filename", DS_TOTAL_FNAME_BUFSIZE);
#if (DS_MOVE_FILES == TRUE)
        strncpy (DS_AppData.DestFileTblPtr->File[i].Movename, "", DS_PATHNAME_BUFSIZE);
#endif
    }

    DS_AppData.FileStatus[FileIndex].FileHandle            = 99;
    DS_AppData.DestFileTblPtr->File[FileIndex].MaxFileAge  = 3;

    /* Execute the function being tested */
    DS_FileTestAge(ElapsedSeconds);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileAge == 2, "DS_AppData.FileStatus[FileIndex].FileAge == 2");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileTestAge_Test_Nominal */

void DS_FileTestAge_Test_ExceedMaxAge(void)
{
    CFE_SB_MsgId_t      FileIndex = 0;
    uint32              ElapsedSeconds = 2;
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    DS_AppData.FileStatus[FileIndex].FileHandle            = 99;
    DS_AppData.DestFileTblPtr->File[FileIndex].MaxFileAge  = 1;

    /* Execute the function being tested */
    DS_FileTestAge(ElapsedSeconds);
    
    /* Verify results */
    UtAssert_True (DS_AppData.FileStatus[FileIndex].FileAge == 0, "DS_AppData.FileStatus[FileIndex].FileAge == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileTestAge_Test_ExceedMaxAge */

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
void DS_FileConvertGPM_Test(void)
{
    char Buffer[8];

    /* Execute the function being tested */
    DS_FileConvertGPM(Buffer, 100000000);
    
    /* Verify results */
    UtAssert_True (strncmp(Buffer, "99999999", 8) == 0, "strncmp(Buffer, '99999999', 8) == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end DS_FileConvertGPM_Test */
#endif

void DS_File_Test_AddTestCases(void)
{
    UtTest_Add(DS_FileStorePacket_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileStorePacket_Test_Nominal");
    UtTest_Add(DS_FileStorePacket_Test_PacketNotInTable, DS_Test_Setup, DS_Test_TearDown, "DS_FileStorePacket_Test_PacketNotInTable");
    UtTest_Add(DS_FileStorePacket_Test_PassedFilterFalse, DS_Test_Setup, DS_Test_TearDown, "DS_FileStorePacket_Test_PassedFilterFalse");

    UtTest_Add(DS_FileSetupWrite_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileSetupWrite_Test_Nominal");
    UtTest_Add(DS_FileSetupWrite_Test_FileHandleClosed, DS_Test_Setup, DS_Test_TearDown, "DS_FileSetupWrite_Test_FileHandleClosed");
    UtTest_Add(DS_FileSetupWrite_Test_MaxFileSizeExceeded, DS_Test_Setup, DS_Test_TearDown, "DS_FileSetupWrite_Test_MaxFileSizeExceeded");

    UtTest_Add(DS_FileWriteData_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteData_Test_Nominal");
    UtTest_Add(DS_FileWriteData_Test_Error, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteData_Test_Error");

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
    UtTest_Add(DS_FileWriteHeader_Test_PlatformConfigCFE_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteHeader_Test_PlatformConfigCFE_Nominal");
    UtTest_Add(DS_FileWriteHeader_Test_PrimaryHeaderError, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteHeader_Test_PrimaryHeaderError");
    UtTest_Add(DS_FileWriteHeader_Test_SecondaryHeaderError, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteHeader_Test_SecondaryHeaderError");
#elif DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
    UtTest_Add(DS_FileWriteHeader_Test_PlatformConfigGPM_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteHeader_Test_PlatformConfigGPM_Nominal");
    UtTest_Add(DS_FileWriteHeader_Test_PlatformConfigGPM_HeaderError, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteHeader_Test_PlatformConfigGPM_HeaderError");
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
    UtTest_Add(DS_FileWriteError_Test, DS_Test_Setup, DS_Test_TearDown, "DS_FileWriteError_Test");
#endif

    UtTest_Add(DS_FileCreateDest_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateDest_Test_Nominal");
    UtTest_Add(DS_FileCreateDest_Test_Error, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateDest_Test_Error");

    UtTest_Add(DS_FileCreateName_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateName_Test_Nominal");
    UtTest_Add(DS_FileCreateName_Test_Error, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateName_Test_Error");

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
    UtTest_Add(DS_FileCreateSequence_Test_ByCount, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateSequence_Test_ByCount");
    UtTest_Add(DS_FileCreateSequence_Test_ByTime, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateSequence_Test_ByTime");
    UtTest_Add(DS_FileCreateSequence_Test_BadFilenameType, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateSequence_Test_BadFilenameType");
#elif DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
    UtTest_Add(DS_FileCreateSequence_Test_PlatformConfigGPM_ByTime, DS_Test_Setup, DS_Test_TearDown, "DS_FileCreateSequence_Test_PlatformConfigGPM_ByTime");
#endif

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE
    UtTest_Add(DS_FileUpdateHeader_Test_PlatformConfigCFE_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileUpdateHeader_Test_PlatformConfigCFE_Nominal");
    UtTest_Add(DS_FileUpdateHeader_Test_WriteError, DS_Test_Setup, DS_Test_TearDown, "DS_FileUpdateHeader_Test_WriteError");
    UtTest_Add(DS_FileUpdateHeader_Test_PlatformConfigCFE_SeekError, DS_Test_Setup, DS_Test_TearDown, "DS_FileUpdateHeader_Test_PlatformConfigCFE_SeekError");
#elif DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
    UtTest_Add(DS_FileUpdateHeader_Test_PlatformConfigGPM_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileUpdateHeader_Test_PlatformConfigGPM_Nominal");
    UtTest_Add(DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteBufError, DS_Test_Setup, DS_Test_TearDown, "DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteBufError");
    UtTest_Add(DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteLastPktTimeError, DS_Test_Setup, DS_Test_TearDown, "DS_FileUpdateHeader_Test_PlatformConfigGPM_WriteLastPktTimeError");
    UtTest_Add(DS_FileUpdateHeader_Test_PlatformConfigGPM_SeekError, DS_Test_Setup, DS_Test_TearDown, "DS_FileUpdateHeader_Test_PlatformConfigGPM_SeekError");
#endif

#if (DS_MOVE_FILES == TRUE)
    UtTest_Add(DS_FileCloseDest_Test_PlatformConfigMoveFiles_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileCloseDest_Test_PlatformConfigMoveFiles_Nominal");
    UtTest_Add(DS_FileCloseDest_Test_PlatformConfigMoveFiles_MoveError, DS_Test_Setup, DS_Test_TearDown, "DS_FileCloseDest_Test_PlatformConfigMoveFiles_MoveError");
    UtTest_Add(DS_FileCloseDest_Test_PlatformConfigMoveFiles_FilenameTooLarge, DS_Test_Setup, DS_Test_TearDown, "DS_FileCloseDest_Test_PlatformConfigMoveFiles_FilenameTooLarge");
#else
    UtTest_Add(DS_FileCloseDest_Test_MoveFilesFalse, DS_Test_Setup, DS_Test_TearDown, "DS_FileCloseDest_Test_MoveFilesFalse");
#endif

    UtTest_Add(DS_FileTestAge_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_FileTestAge_Test_Nominal");
    UtTest_Add(DS_FileTestAge_Test_ExceedMaxAge, DS_Test_Setup, DS_Test_TearDown, "DS_FileTestAge_Test_ExceedMaxAge");

#if DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM
    UtTest_Add(DS_FileConvertGPM_Test, DS_Test_Setup, DS_Test_TearDown, "DS_FileConvertGPM_Test");
#endif
} /* end DS_File_Test_AddTestCases */


/************************/
/*  End of File Comment */
/************************/
