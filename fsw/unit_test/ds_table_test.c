 /*************************************************************************
 ** File:
 **   $Id: ds_table_test.c 1.3 2017/01/25 12:29:29EST sstrege Exp  $
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
 **   This file contains unit test cases for the functions contained in the file ds_table.c
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

#include "ds_table_test.h"
#include "ds_app.h"
#include "ds_appdefs.h"
#include "ds_table.h"
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

DS_DestFileTable_t  UT_DS_TABLE_TEST_DestFileTable;
int32 UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook1 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
    DS_AppData.DestFileTblPtr = &UT_DS_TABLE_TEST_DestFileTable;

    return CFE_TBL_INFO_UPDATED;
} /* end UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook1 */

int32 UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook2 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
    DS_AppData.DestFileTblPtr = &UT_DS_TABLE_TEST_DestFileTable;

    return CFE_TBL_ERR_NEVER_LOADED;
} /* end UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook2 */

DS_FilterTable_t  UT_DS_TABLE_TEST_FilterTable;
int32 UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook3 (void **TblPtr, CFE_TBL_Handle_t TblHandle)
{
    DS_AppData.FilterTblPtr = &UT_DS_TABLE_TEST_FilterTable;

    return CFE_TBL_INFO_UPDATED;
} /* end UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook3 */

int32 UT_DS_TABLE_TEST_CFE_TBL_GetStatusHook (CFE_TBL_Handle_t TblHandle)
{
    DS_AppData.DestFileTblPtr = &UT_DS_TABLE_TEST_DestFileTable;

    return CFE_TBL_INFO_UPDATE_PENDING;
} /* end UT_DS_TABLE_TEST_CFE_TBL_GetStatusHook */

int32 UT_DS_TABLE_TEST_CFE_ES_RestoreFromCDSHook (void *RestoreToMemory, CFE_ES_CDSHandle_t Handle)
{
    memset(RestoreToMemory, 0, DS_DEST_FILE_CNT*4);

    return CFE_SUCCESS;
} /* end UT_DS_TABLE_TEST_CFE_ES_RestoreFromCDSHook */

/*
 * Function Definitions
 */

void DS_TableInit_Test_Nominal(void)
{
    int32 Result;

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = DS_TableInit();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableInit_Test_Nominal */

void DS_TableInit_Test_TableInfoRecovered(void)
{
    int32 Result;

    /* Set to generate both of the two error messages DS_INIT_TBL_CDS_EID  */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, CFE_TBL_INFO_RECOVERED_TBL, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = DS_TableInit();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_TBL_CDS_EID, CFE_EVS_DEBUG, "Destination File Table data restored from CDS"),
        "Destination File Table data restored from CDS");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_TBL_CDS_EID, CFE_EVS_DEBUG, "Filter Table data restored from CDS"),
        "Filter Table data restored from CDS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
} /* end DS_TableInit_Test_TableInfoRecovered */

void DS_TableInit_Test_RegisterDestTableError(void)
{
    int32 Result;

    /* Set to generate first instance of error message DS_INIT_TBL_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, 0x99, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_REGISTER_INDEX);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = DS_TableInit();
    
    /* Verify results */
    UtAssert_True (Result == 0x99, "Result == 0x99");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_TBL_ERR_EID, CFE_EVS_ERROR, "Unable to register Destination File Table: Error = 0x00000099"),
        "Unable to register Destination File Table: Error = 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableInit_Test_RegisterDestTableError */

void DS_TableInit_Test_RegisterFilterTableError(void)
{
    int32 Result;

    /* Set to generate second instance of error message DS_INIT_TBL_ERR_EID */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_REGISTER_INDEX, 0x99, 2);

    /* Set to prevent unintended error messages */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);
    Ut_CFE_TBL_ContinueReturnCodeAfterCountZero(UT_CFE_TBL_LOAD_INDEX);

    /* Execute the function being tested */
    Result = DS_TableInit();
    
    /* Verify results */
    UtAssert_True (Result == 0x99, "Result == 0x99");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_TBL_ERR_EID, CFE_EVS_ERROR, "Unable to register Filter Table: Error = 0x00000099"),
        "Unable to register Filter Table: Error = 0x00000099");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableInit_Test_RegisterFilterTableError */

void DS_TableInit_Test_LoadDestTableError(void)
{
    int32 Result;

    /* Set to generate error message DS_INIT_TBL_ERR_EID on 1st call (but not 2nd) */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 2);

    /* Execute the function being tested */
    Result = DS_TableInit();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_TBL_ERR_EID, CFE_EVS_ERROR, "Unable to load default Destination File Table: Filename = '/cf/apps/ds_file_tbl.tbl', Error = 0xCC000013"),
        "Unable to load default Destination File Table: Filename = '/cf/apps/ds_file_tbl.tbl', Error = 0xCC000013");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableInit_Test_LoadDestTableError */

void DS_TableInit_Test_LoadFilterTableError(void)
{
    int32 Result;

    /* Set to generate error message DS_INIT_TBL_ERR_EID on 2nd call (but not 1st) */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_LOAD_INDEX, CFE_SUCCESS, 1);

    /* Execute the function being tested */
    Result = DS_TableInit();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_TBL_ERR_EID, CFE_EVS_ERROR, "Unable to load default Filter Table: Filename = '/cf/apps/ds_filter_tbl.tbl', Error = 0xCC000013"),
        "Unable to load default Filter Table: Filename = '/cf/apps/ds_filter_tbl.tbl', Error = 0xCC000013");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableInit_Test_LoadFilterTableError */

void DS_TableManageDestFile_Test_TableInfoUpdated(void)
{
    uint32   i;
    
    DS_AppData.DestFileTblPtr = &UT_DS_TABLE_TEST_DestFileTable;

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.DestFileTblPtr->File[i].EnableState = i;
        DS_AppData.DestFileTblPtr->File[i].SequenceCount = i;
    }
    
    DS_AppData.DestFileTblPtr = (DS_DestFileTable_t *) NULL;

    /* Returns CFE_TBL_INFO_UPDATED to satisfy condition "if (Result == CFE_TBL_INFO_UPDATED)", and sets
     * DS_AppData.DestFileTblPtr to the address of a local table defined globally in this file, to prevent segmentation fault */
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook1);

    /* Execute the function being tested */
    DS_TableManageDestFile();
    
    /* Verify results */
    UtAssert_True (DS_AppData.DestTblLoadCounter == 1, "DS_AppData.DestTblLoadCounter == 1");

    UtAssert_True (DS_AppData.FileStatus[0].FileState == 0, "DS_AppData.FileStatus[0].FileState == 0");
    UtAssert_True (DS_AppData.FileStatus[0].FileCount == 0, "DS_AppData.FileStatus[0].FileCount == 0");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileState == DS_DEST_FILE_CNT/2, "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileState == DS_DEST_FILE_CNT/2");
    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileCount == DS_DEST_FILE_CNT/2, "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileCount == DS_DEST_FILE_CNT/2");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileState == DS_DEST_FILE_CNT-1, "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileState == DS_DEST_FILE_CNT-1");
    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileCount == DS_DEST_FILE_CNT-1, "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileCount == DS_DEST_FILE_CNT-1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageDestFile_Test_TableInfoUpdated */

void DS_TableManageDestFile_Test_TableNeverLoaded(void)
{
    /* Returns CFE_TBL_INFO_UPDATED to satisfy condition "if (Result == CFE_TBL_ERR_NEVER_LOADED)", and sets
     * DS_AppData.DestFileTblPtr to the address of a local table defined globally in this file, to prevent segmentation fault */
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook2);

    /* Execute the function being tested */
    DS_TableManageDestFile();
    
    /* Verify results */
    UtAssert_True (DS_AppData.DestTblErrCounter == 1, "DS_AppData.DestTblErrCounter == 1");
    UtAssert_True (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL, "DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageDestFile_Test_TableNeverLoaded */

void DS_TableManageDestFile_Test_TableInfoDumpPending(void)
{
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Set to satisfy condition "if (Result == CFE_TBL_INFO_DUMP_PENDING)" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_DUMP_PENDING, 1);

    /* Execute the function being tested */
    DS_TableManageDestFile();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageDestFile_Test_TableInfoDumpPending */

void DS_TableManageDestFile_Test_TableInfoValidationPending(void)
{
    DS_DestFileTable_t  DestFileTable;

    DS_AppData.DestFileTblPtr = &DestFileTable;

    /* Set to satisfy condition "if (Result == CFE_TBL_INFO_VALIDATION_PENDING)" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);

    /* Execute the function being tested */
    DS_TableManageDestFile();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageDestFile_Test_TableInfoValidationPending */

void DS_TableManageDestFile_Test_TableInfoUpdatePending(void)
{
    uint32              i;

    DS_AppData.DestFileTblPtr = &UT_DS_TABLE_TEST_DestFileTable;

    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.DestFileTblPtr->File[i].EnableState = i;
        DS_AppData.DestFileTblPtr->File[i].SequenceCount = i;
    }

    /* Returns CFE_TBL_INFO_UPDATED to satisfy condition "if (Result == CFE_TBL_INFO_UPDATE_PENDING)", and sets
     * DS_AppData.DestFileTblPtr to the address of a local table defined globally in this file, to prevent segmentation fault */
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETSTATUS_INDEX, &UT_DS_TABLE_TEST_CFE_TBL_GetStatusHook);

    /* Prevents segmentation fault */
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook1);

    /* Execute the function being tested */
    DS_TableManageDestFile();
    
    /* Verify results */
    UtAssert_True (DS_AppData.DestTblLoadCounter == 1, "DS_AppData.DestTblLoadCounter == 1");

    UtAssert_True (DS_AppData.FileStatus[0].FileState == 0, "DS_AppData.FileStatus[0].FileState == 0");
    UtAssert_True (DS_AppData.FileStatus[0].FileCount == 0, "DS_AppData.FileStatus[0].FileCount == 0");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileState == DS_DEST_FILE_CNT/2, "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileState == DS_DEST_FILE_CNT/2");
    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileCount == DS_DEST_FILE_CNT/2, "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileCount == DS_DEST_FILE_CNT/2");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileState == DS_DEST_FILE_CNT-1, "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileState == DS_DEST_FILE_CNT-1");
    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileCount == DS_DEST_FILE_CNT-1, "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileCount == DS_DEST_FILE_CNT-1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageDestFile_Test_TableInfoUpdatePending */

void DS_TableManageFilter_Test_TableInfoUpdated(void)
{
    /* Returns CFE_TBL_INFO_UPDATED to satisfy condition "if (Result == CFE_TBL_INFO_UPDATED)", and sets
     * DS_AppData.FilterTblPtr to the address of a table defined globally in this file, to prevent segmentation fault */
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook3);

    /* Execute the function being tested */
    DS_TableManageFilter();
    
    /* Verify results */
    UtAssert_True (DS_AppData.FilterTblLoadCounter == 1, "DS_AppData.FilterTblLoadCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageFilter_Test_TableInfoUpdated */

void DS_TableManageFilter_Test_TableNeverLoaded(void)
{
    /* Set to satisfy condition "if (Result == CFE_TBL_ERR_NEVER_LOADED)" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_NEVER_LOADED, 1);

    /* Execute the function being tested */
    DS_TableManageFilter();
    
    /* Verify results */
    UtAssert_True (DS_AppData.FilterTblErrCounter == 1, "DS_AppData.FilterTblErrCounter == 1");
    UtAssert_True (DS_AppData.FilterTblPtr == (DS_FilterTable_t *) NULL, "DS_AppData.FilterTblPtr == (DS_FilterTable_t *) NULL");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageFilter_Test_TableNeverLoaded */

void DS_TableManageFilter_Test_TableInfoDumpPending(void)
{
    DS_FilterTable_t  FilterTable;

    DS_AppData.FilterTblPtr = &FilterTable;

    /* Set to satisfy condition "if (Result == CFE_TBL_INFO_DUMP_PENDING)" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_DUMP_PENDING, 1);

    /* Execute the function being tested */
    DS_TableManageFilter();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageFilter_Test_TableInfoDumpPending */

void DS_TableManageFilter_Test_TableInfoValidationPending(void)
{
    DS_FilterTable_t  FilterTable;

    DS_AppData.FilterTblPtr = &FilterTable;

    /* Set to satisfy condition "CFE_TBL_INFO_VALIDATION_PENDING" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETSTATUS_INDEX, CFE_TBL_INFO_VALIDATION_PENDING, 1);

    /* Execute the function being tested */
    DS_TableManageFilter();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageFilter_Test_TableInfoValidationPending */

void DS_TableManageFilter_Test_TableInfoUpdatePending(void)
{
    DS_AppData.FilterTblPtr = &UT_DS_TABLE_TEST_FilterTable;

    /* Returns CFE_TBL_INFO_UPDATED to satisfy condition "if (Result == CFE_TBL_INFO_UPDATE_PENDING)", and sets
     * DS_AppData.DestFileTblPtr to the address of a local table defined globally in this file, to prevent segmentation fault */
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETSTATUS_INDEX, &UT_DS_TABLE_TEST_CFE_TBL_GetStatusHook);

    /* Prevents segmentation fault */
    Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_GETADDRESS_INDEX, &UT_DS_TABLE_TEST_CFE_TBL_GetAddressHook1);

    /* Execute the function being tested */
    DS_TableManageFilter();
    
    /* Verify results */
    UtAssert_True (DS_AppData.FilterTblLoadCounter == 1, "DS_AppData.FilterTblLoadCounter == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableManageFilter_Test_TableInfoUpdatePending */

void DS_TableVerifyDestFile_Test_Nominal(void)
{
    int32               Result;
    DS_DestFileTable_t  DestFileTable;
    uint32              FileIndex = 0;
    uint32              i;

    DestFileTable.File[FileIndex].FileNameType = DS_BY_TIME;
    DestFileTable.File[FileIndex].EnableState = DS_ENABLED;
    DestFileTable.File[FileIndex].MaxFileSize = 2048;
    DestFileTable.File[FileIndex].MaxFileAge = 100;
    DestFileTable.File[FileIndex].SequenceCount = 1;

    strncpy (DestFileTable.File[FileIndex].Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileTable.File[FileIndex].Basename, "basename", DS_BASENAME_BUFSIZE);
    strncpy (DestFileTable.File[FileIndex].Extension, "ext", DS_EXTENSION_BUFSIZE);

    for (i = 1; i < DS_DEST_FILE_CNT; i++)
    {
        memset(&DestFileTable.File[i], DS_UNUSED, sizeof(DS_DestFileEntry_t));
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFile(&DestFileTable);
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_EID, CFE_EVS_INFORMATION, "Destination file table verify results: desc text = OK, good entries = 1, bad = 0, unused = 15"),
        "Destination file table verify results: desc text = OK, good entries = 1, bad = 0, unused = 15");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFile_Test_Nominal */

void DS_TableVerifyDestFile_Test_DestFileTableVerificationError(void)
{
    int32               Result;
    DS_DestFileTable_t  DestFileTable;
    uint32              FileIndex = 0;
    uint32              i;

    DestFileTable.File[FileIndex].FileNameType = DS_BY_TIME;
    DestFileTable.File[FileIndex].EnableState = DS_ENABLED;
    DestFileTable.File[FileIndex].MaxFileSize = 2048;
    DestFileTable.File[FileIndex].MaxFileAge = 100;
    DestFileTable.File[FileIndex].SequenceCount = 1;

    strncpy (DestFileTable.File[FileIndex].Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileTable.File[FileIndex].Basename, "basename", DS_BASENAME_BUFSIZE);
    strncpy (DestFileTable.File[FileIndex].Extension, "ext", DS_EXTENSION_BUFSIZE);

    for (i = 1; i < DS_DEST_FILE_CNT; i++)
    {
        memset(&DestFileTable.File[i], DS_UNUSED, sizeof(DS_DestFileEntry_t));
    }

    for (i = 0; i < DS_DESCRIPTOR_BUFSIZE; i++)
    {
        DestFileTable.Descriptor[i] = '*';
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFile(&DestFileTable);
    
    /* Verify results */
    UtAssert_True (Result == DS_TABLE_VERIFY_ERR, "Result == DS_TABLE_VERIFY_ERR");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: invalid descriptor text"),
        "Destination file table verify err: invalid descriptor text");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_EID, CFE_EVS_INFORMATION, "Destination file table verify results: desc text = bad, good entries = 1, bad = 0, unused = 15"),
        "Destination file table verify results: desc text = bad, good entries = 1, bad = 0, unused = 15");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
} /* end DS_TableVerifyDestFile_Test_DestFileTableVerificationError */

void DS_TableVerifyDestFile_Test_CountBad(void)
{
    int32               Result;
    DS_DestFileTable_t  DestFileTable;
    uint32              FileIndex = 0;
    uint32              i;

    strncpy (DestFileTable.Descriptor, "descriptor", DS_DESCRIPTOR_BUFSIZE);

    DestFileTable.File[FileIndex].FileNameType = DS_BY_TIME;
    DestFileTable.File[FileIndex].EnableState = DS_ENABLED;
    DestFileTable.File[FileIndex].MaxFileSize = 2048;
    DestFileTable.File[FileIndex].MaxFileAge = 100;
    DestFileTable.File[FileIndex].SequenceCount = 1;

    strncpy (DestFileTable.File[FileIndex].Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileTable.File[FileIndex].Basename, "basename", DS_BASENAME_BUFSIZE);
    strncpy (DestFileTable.File[FileIndex].Extension, "123456789", DS_EXTENSION_BUFSIZE);

    for (i = 1; i < DS_DEST_FILE_CNT; i++)
    {
        memset(&DestFileTable.File[i], DS_UNUSED, sizeof(DS_DestFileEntry_t));
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFile(&DestFileTable);
    
    /* Verify results */
    UtAssert_True (Result == DS_TABLE_VERIFY_ERR, "Result == DS_TABLE_VERIFY_ERR");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, invalid extension text"),
        "Destination file table verify err: index = 0, invalid extension text");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_EID, CFE_EVS_INFORMATION, "Destination file table verify results: desc text = OK, good entries = 0, bad = 1, unused = 15"),
        "Destination file table verify results: desc text = OK, good entries = 0, bad = 1, unused = 15");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
} /* end DS_TableVerifyDestFile_Test_CountBad */

void DS_TableVerifyDestFileEntry_Test_Nominal(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "basename", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyDestFileEntry_Test_Nominal */

void DS_TableVerifyDestFileEntry_Test_InvalidPathname(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "***", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "basename", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, invalid pathname text"),
        "Destination file table verify err: index = 0, invalid pathname text");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidPathname */

void DS_TableVerifyDestFileEntry_Test_InvalidBasename(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "***", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, invalid basename text"),
        "Destination file table verify err: index = 0, invalid basename text");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidBasename */

void DS_TableVerifyDestFileEntry_Test_InvalidExtension(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "pathname", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "123456789", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, invalid extension text"),
        "Destination file table verify err: index = 0, invalid extension text");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidExtension */

void DS_TableVerifyDestFileEntry_Test_InvalidFilenameType(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = 99;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "pathname", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, filename type = 99"),
        "Destination file table verify err: index = 0, filename type = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidFilenameType */

void DS_TableVerifyDestFileEntry_Test_InvalidFileEnableState(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = 99;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "pathname", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, file enable state = 99"),
        "Destination file table verify err: index = 0, file enable state = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidFileEnableState */

void DS_TableVerifyDestFileEntry_Test_InvalidSize(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = DS_FILE_MIN_SIZE_LIMIT - 1;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "pathname", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, max file size = 1023"),
        "Destination file table verify err: index = 0, max file size = 1023");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidSize */

void DS_TableVerifyDestFileEntry_Test_InvalidAge(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = DS_FILE_MIN_AGE_LIMIT - 1;
    DestFileEntry.SequenceCount = 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "pathname", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, max file age = 59"),
        "Destination file table verify err: index = 0, max file age = 59");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidAge */

void DS_TableVerifyDestFileEntry_Test_InvalidSequenceCount(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;

    DestFileEntry.FileNameType = DS_BY_TIME;
    DestFileEntry.EnableState = DS_ENABLED;
    DestFileEntry.MaxFileSize = 2048;
    DestFileEntry.MaxFileAge = 100;
    DestFileEntry.SequenceCount = DS_MAX_SEQUENCE_COUNT + 1;

    strncpy (DestFileEntry.Pathname, "path", DS_PATHNAME_BUFSIZE);
    strncpy (DestFileEntry.Basename, "pathname", DS_BASENAME_BUFSIZE);
    strncpy (DestFileEntry.Extension, "ext", DS_EXTENSION_BUFSIZE);

    /* Execute the function being tested */
    Result = DS_TableVerifyDestFileEntry(&DestFileEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FIL_TBL_ERR_EID, CFE_EVS_ERROR, "Destination file table verify err: index = 0, sequence count = 100000000"),
        "Destination file table verify err: index = 0, sequence count = 100000000");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyDestFileEntry_Test_InvalidSequenceCount */

void DS_TableVerifyFilter_Test_Nominal(void)
{
    int32               Result;
    DS_FilterTable_t    FilterTable;
    uint32              i;

    FilterTable.Packet[0].MessageID                = 0x18BB;
    FilterTable.Packet[0].Filter[0].FileTableIndex = 0;
    FilterTable.Packet[0].Filter[0].Algorithm_N    = 1;
    FilterTable.Packet[0].Filter[0].Algorithm_X    = 3;
    FilterTable.Packet[0].Filter[0].Algorithm_O    = 0;
    FilterTable.Packet[0].Filter[0].FilterType     = 1;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    strncpy (FilterTable.Descriptor, "descriptor", DS_DESCRIPTOR_BUFSIZE);

    for (i = 0; i < DS_FILTERS_PER_PACKET; i++)
    {
        memset(&FilterTable.Packet[0].Filter[i], DS_UNUSED, sizeof(DS_FilterParms_t));
    }

    for (i = 1; i < 256; i++)
    {
        FilterTable.Packet[i].MessageID = DS_UNUSED;
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyFilter(&FilterTable);
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_EID, CFE_EVS_INFORMATION, "Filter table verify results: desc text = OK, good entries = 1, bad = 0, unused = 255"),
        "Filter table verify results: desc text = OK, good entries = 1, bad = 0, unused = 255");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyFilter_Test_Nominal */

void DS_TableVerifyFilter_Test_FilterTableVerificationError(void)
{
    int32               Result;
    DS_FilterTable_t    FilterTable;
    uint32              i;

    FilterTable.Packet[0].MessageID                = 0x18BB;
    FilterTable.Packet[0].Filter[0].FileTableIndex = 0;
    FilterTable.Packet[0].Filter[0].Algorithm_N    = 1;
    FilterTable.Packet[0].Filter[0].Algorithm_X    = 3;
    FilterTable.Packet[0].Filter[0].Algorithm_O    = 0;
    FilterTable.Packet[0].Filter[0].FilterType     = 1;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    for (i = 0; i < DS_DESCRIPTOR_BUFSIZE; i++)
    {
        FilterTable.Descriptor[i] = '*';
    }

    for (i = 0; i < DS_FILTERS_PER_PACKET; i++)
    {
        memset(&FilterTable.Packet[0].Filter[i], DS_UNUSED, sizeof(DS_FilterParms_t));
    }

    for (i = 1; i < 256; i++)
    {
        FilterTable.Packet[i].MessageID = DS_UNUSED;
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyFilter(&FilterTable);
    
    /* Verify results */
    UtAssert_True (Result == DS_TABLE_VERIFY_ERR, "Result == DS_TABLE_VERIFY_ERR");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_ERR_EID, CFE_EVS_ERROR, "Filter table verify err: invalid descriptor text"),
        "Filter table verify err: invalid descriptor text");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_EID, CFE_EVS_INFORMATION, "Filter table verify results: desc text = bad, good entries = 1, bad = 0, unused = 255"),
        "Filter table verify results: desc text = bad, good entries = 1, bad = 0, unused = 255");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
} /* end DS_TableVerifyFilter_Test_FilterTableVerificationError */

void DS_TableVerifyFilter_Test_CountBad(void)
{
    int32               Result;
    DS_FilterTable_t    FilterTable;
    uint32              i;

    FilterTable.Packet[0].MessageID                = 0x18BB;
    FilterTable.Packet[0].Filter[0].FileTableIndex = 0;
    FilterTable.Packet[0].Filter[0].Algorithm_N    = 1;
    FilterTable.Packet[0].Filter[0].Algorithm_X    = 3;
    FilterTable.Packet[0].Filter[0].Algorithm_O    = 0;
    FilterTable.Packet[0].Filter[0].FilterType     = DS_BY_TIME * 2;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    strncpy (FilterTable.Descriptor, "descriptor", DS_DESCRIPTOR_BUFSIZE);

    for (i = 1; i < DS_FILTERS_PER_PACKET; i++)
    {
        memset(&FilterTable.Packet[0].Filter[i], DS_UNUSED, sizeof(DS_FilterParms_t));
    }

    for (i = 1; i < 256; i++)
    {
        FilterTable.Packet[i].MessageID = DS_UNUSED;
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyFilter(&FilterTable);
    
    /* Verify results */
    UtAssert_True (Result == DS_TABLE_VERIFY_ERR, "Result == DS_TABLE_VERIFY_ERR");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_ERR_EID, CFE_EVS_ERROR, "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, filter type = 4"),
        "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, filter type = 4");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_EID, CFE_EVS_INFORMATION, "Filter table verify results: desc text = OK, good entries = 0, bad = 1, unused = 255"),
        "Filter table verify results: desc text = OK, good entries = 0, bad = 1, unused = 255");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");
} /* end DS_TableVerifyFilter_Test_CountBad */

void DS_TableVerifyFilterEntry_Test_Nominal(void)
{
    int32               Result;
    DS_PacketEntry_t    PacketEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;
    uint32              i;

    PacketEntry.MessageID                = 0x18BB;
    PacketEntry.Filter[0].FileTableIndex = 0;
    PacketEntry.Filter[0].Algorithm_N    = 1;
    PacketEntry.Filter[0].Algorithm_X    = 3;
    PacketEntry.Filter[0].Algorithm_O    = 0;
    PacketEntry.Filter[0].FilterType     = 1;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    for (i = 0; i < DS_FILTERS_PER_PACKET; i++)
    {
        memset(&PacketEntry.Filter[i], DS_UNUSED, sizeof(DS_FilterParms_t));
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyFilterEntry(&PacketEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyFilterEntry_Test_Nominal */

void DS_TableVerifyFilterEntry_Test_InvalidFileTableIndex(void)
{
    int32               Result;
    DS_PacketEntry_t    PacketEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;
    uint32              i;

    PacketEntry.MessageID                = 0x18BB;
    PacketEntry.Filter[0].FileTableIndex = DS_DEST_FILE_CNT + 1;
    PacketEntry.Filter[0].Algorithm_N    = 1;
    PacketEntry.Filter[0].Algorithm_X    = 3;
    PacketEntry.Filter[0].Algorithm_O    = 0;
    PacketEntry.Filter[0].FilterType     = 1;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    for (i = 1; i < DS_FILTERS_PER_PACKET; i++)
    {
        memset(&PacketEntry.Filter[i], DS_UNUSED, sizeof(DS_FilterParms_t));
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyFilterEntry(&PacketEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_ERR_EID, CFE_EVS_ERROR, "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, file table index = 17"),
        "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, file table index = 17");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyFilterEntry_Test_InvalidFileTableIndex */

void DS_TableVerifyFilterEntry_Test_InvalidFilterType(void)
{
    int32               Result;
    DS_PacketEntry_t    PacketEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;
    uint32              i;

    PacketEntry.MessageID                = 0x18BB;
    PacketEntry.Filter[0].FileTableIndex = 0;
    PacketEntry.Filter[0].Algorithm_N    = 1;
    PacketEntry.Filter[0].Algorithm_X    = 3;
    PacketEntry.Filter[0].Algorithm_O    = 0;
    PacketEntry.Filter[0].FilterType     = 99;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    for (i = 1; i < DS_FILTERS_PER_PACKET; i++)
    {
        memset(&PacketEntry.Filter[i], DS_UNUSED, sizeof(DS_FilterParms_t));
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyFilterEntry(&PacketEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_ERR_EID, CFE_EVS_ERROR, "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, filter type = 99"),
        "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, filter type = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyFilterEntry_Test_InvalidFilterType */

void DS_TableVerifyFilterEntry_Test_InvalidFilterParms(void)
{
    int32               Result;
    DS_PacketEntry_t    PacketEntry;
    uint32              TableIndex = 0;
    uint32              ErrorCount = 0;
    uint32              i;

    PacketEntry.MessageID                = 0x18BB;
    PacketEntry.Filter[0].FileTableIndex = 0;
    PacketEntry.Filter[0].Algorithm_N    = 1;
    PacketEntry.Filter[0].Algorithm_X    = 3;
    PacketEntry.Filter[0].Algorithm_O    = 99;
    PacketEntry.Filter[0].FilterType     = 1;
    DS_AppData.FileStatus[0].FileState = DS_ENABLED;

    for (i = 1; i < DS_FILTERS_PER_PACKET; i++)
    {
        memset(&PacketEntry.Filter[i], DS_UNUSED, sizeof(DS_FilterParms_t));
    }

    /* Execute the function being tested */
    Result = DS_TableVerifyFilterEntry(&PacketEntry, TableIndex, ErrorCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_FLT_TBL_ERR_EID, CFE_EVS_ERROR, "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, filter parms N = 1, X = 3, O = 99"),
        "Filter table verify err: MID = 0x18BB, index = 0, filter = 0, filter parms N = 1, X = 3, O = 99");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableVerifyFilterEntry_Test_InvalidFilterParms */

void DS_TableEntryUnused_Test_Nominal(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;

    memset(&DestFileEntry, DS_UNUSED, sizeof(DS_DestFileEntry_t));

    /* Execute the function being tested */
    Result = DS_TableEntryUnused(&DestFileEntry, sizeof(DS_DestFileEntry_t));
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableEntryUnused_Test_Nominal */

void DS_TableEntryUnused_Test_Fail(void)
{
    int32               Result;
    DS_DestFileEntry_t  DestFileEntry;

    memset(&DestFileEntry, 99, sizeof(DS_DestFileEntry_t));

    /* Execute the function being tested */
    Result = DS_TableEntryUnused(&DestFileEntry, sizeof(DS_DestFileEntry_t));
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableEntryUnused_Test_Fail */

void DS_TableVerifyFileIndex_Test_Nominal(void)
{
    int32   Result;
    uint16  FileTableIndex = 0;

    /* Execute the function being tested */
    Result = DS_TableVerifyFileIndex(FileTableIndex);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyFileIndex_Test_Nominal */

void DS_TableVerifyFileIndex_Test_Fail(void)
{
    int32   Result;
    uint16  FileTableIndex = DS_DEST_FILE_CNT;

    /* Execute the function being tested */
    Result = DS_TableVerifyFileIndex(FileTableIndex);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyFileIndex_Test_Fail */

void DS_TableVerifyParms_Test_NominalOnlyXNonZero(void)
{
    int32   Result;
    uint16  Algorithm_N  = 0;
    uint16  Algorithm_X  = 1;
    uint16  Algorithm_O  = 0;

    /* Execute the function being tested */
    Result = DS_TableVerifyParms(Algorithm_N, Algorithm_X, Algorithm_O);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyParms_Test_NominalOnlyXNonZero */

void DS_TableVerifyParms_Test_NGreaterThanXOnlyNNonZero(void)
{
    int32   Result;
    uint16  Algorithm_N  = 1;
    uint16  Algorithm_X  = 0;
    uint16  Algorithm_O  = 0;

    /* Execute the function being tested */
    Result = DS_TableVerifyParms(Algorithm_N, Algorithm_X, Algorithm_O);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyParms_Test_NGreaterThanXOnlyNNonZero */

void DS_TableVerifyParms_Test_OGreaterThanXOnlyONonZero(void)
{
    int32   Result;
    uint16  Algorithm_N  = 0;
    uint16  Algorithm_X  = 0;
    uint16  Algorithm_O  = 1;

    /* Execute the function being tested */
    Result = DS_TableVerifyParms(Algorithm_N, Algorithm_X, Algorithm_O);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyParms_Test_OGreaterThanXOnlyONonZero */

void DS_TableVerifyType_Test_Nominal(void)
{
    int32   Result;
    uint16  TimeVsCount = DS_BY_TIME;

    /* Execute the function being tested */
    Result = DS_TableVerifyType(TimeVsCount);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyType_Test_Nominal */

void DS_TableVerifyType_Test_Fail(void)
{
    int32   Result;
    uint16  TimeVsCount = 99;

    /* Execute the function being tested */
    Result = DS_TableVerifyType(TimeVsCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyType_Test_Fail */

void DS_TableVerifyState_Test_Nominal(void)
{
    int32   Result;
    uint16  EnableState = DS_ENABLED;

    /* Execute the function being tested */
    Result = DS_TableVerifyState(EnableState);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyState_Test_Nominal */

void DS_TableVerifyState_Test_Fail(void)
{
    int32   Result;
    uint16  EnableState = 99;

    /* Execute the function being tested */
    Result = DS_TableVerifyState(EnableState);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyState_Test_Fail */

void DS_TableVerifySize_Test_Nominal(void)
{
    int32   Result;
    uint32  MaxFileSize = DS_FILE_MIN_SIZE_LIMIT;

    /* Execute the function being tested */
    Result = DS_TableVerifySize(MaxFileSize);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifySize_Test_Nominal */

void DS_TableVerifySize_Test_Fail(void)
{
    int32   Result;
    uint32  MaxFileSize = DS_FILE_MIN_SIZE_LIMIT - 1;

    /* Execute the function being tested */
    Result = DS_TableVerifySize(MaxFileSize);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifySize_Test_Fail */

void DS_TableVerifyAge_Test_Nominal(void)
{
    int32   Result;
    uint32  MaxFileAge = DS_FILE_MIN_AGE_LIMIT;

    /* Execute the function being tested */
    Result = DS_TableVerifyAge(MaxFileAge);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyAge_Test_Nominal */

void DS_TableVerifyAge_Test_Fail(void)
{
    int32   Result;
    uint32  MaxFileAge = DS_FILE_MIN_AGE_LIMIT - 1;

    /* Execute the function being tested */
    Result = DS_TableVerifyAge(MaxFileAge);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyAge_Test_Fail */

void DS_TableVerifyCount_Test_Nominal(void)
{
    int32   Result;
    uint32  SequenceCount = DS_MAX_SEQUENCE_COUNT;

    /* Execute the function being tested */
    Result = DS_TableVerifyCount(SequenceCount);
    
    /* Verify results */
    UtAssert_True (Result == TRUE, "Result == TRUE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyCount_Test_Nominal */

void DS_TableVerifyCount_Test_Fail(void)
{
    int32   Result;
    uint32  SequenceCount = DS_MAX_SEQUENCE_COUNT + 1;

    /* Execute the function being tested */
    Result = DS_TableVerifyCount(SequenceCount);
    
    /* Verify results */
    UtAssert_True (Result == FALSE, "Result == FALSE");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableVerifyCount_Test_Fail */

void DS_TableSubscribe_Test(void)
{
    DS_FilterTable_t  FilterTable;

    DS_AppData.FilterTblPtr = &FilterTable;

    FilterTable.Packet[0].MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_TableSubscribe();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableSubscribe_Test */

void DS_TableUnsubscribe_Test(void)
{
    DS_FilterTable_t  FilterTable;

    DS_AppData.FilterTblPtr = &FilterTable;

    FilterTable.Packet[0].MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_TableUnsubscribe();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableUnsubscribe_Test */

void DS_TableCreateCDS_Test_NewCDSArea(void)
{
    int32   Result;

    /* Execute the function being tested */
    Result = DS_TableCreateCDS();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableCreateCDS_Test_NewCDSArea */

void DS_TableCreateCDS_Test_PreExistingCDSArea(void)
{
    int32   Result;

    /* Set to satisfy condition "if (Result == CFE_ES_CDS_ALREADY_EXISTS)", which is the main thing we're testing here */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCDS_INDEX, CFE_ES_CDS_ALREADY_EXISTS, 1);

    /* Gives values to array DataStoreBuffer, which is used in the function under test 
     * to set values for DS_AppData.FileStatus[i].FileCount.  Also returns CFE_SUCCESS, in order
     * to reach line that sets DS_AppData.AppEnableState */
    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_RESTOREFROMCDS_INDEX, &UT_DS_TABLE_TEST_CFE_ES_RestoreFromCDSHook);

    /* Execute the function being tested */
    Result = DS_TableCreateCDS();
    
    /* Verify results */
    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True (DS_AppData.FileStatus[0].FileCount == 0, "DS_AppData.FileStatus[0].FileCount == 0");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileCount == 0, "DS_AppData.FileStatus[DS_DEST_FILE_CNT/2].FileCount == 0");

    UtAssert_True (DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileCount == 0, "DS_AppData.FileStatus[DS_DEST_FILE_CNT-1].FileCount == 0");
     
    UtAssert_True (DS_AppData.AppEnableState == 0, "DS_AppData.AppEnableState == 0");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableCreateCDS_Test_PreExistingCDSArea */

void DS_TableCreateCDS_Test_Error(void)
{
    int32   Result;

    /* Set to generate error message DS_INIT_CDS_ERR_EID */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_REGISTERCDS_INDEX, -1, 1);

    /* Execute the function being tested */
    Result = DS_TableCreateCDS();
    
    /* Verify results */
    UtAssert_True (DS_AppData.DataStoreHandle == 0, "DS_AppData.DataStoreHandle == 0");

    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_CDS_ERR_EID, CFE_EVS_ERROR, "Critical Data Store access error = 0xFFFFFFFF"),
        "Critical Data Store access error = 0xFFFFFFFF");

    UtAssert_True (Result == CFE_SUCCESS, "Result == CFE_SUCCESS"); /* CFE_SUCCESS because error is not fatal */

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableCreateCDS_Test_Error */

void DS_TableUpdateCDS_Test_Nominal(void)
{
    DS_AppData.DataStoreHandle = 1;

    /* Execute the function being tested */
    DS_TableUpdateCDS();
    
    /* Verify results */
    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableUpdateCDS_Test_Nominal */

void DS_TableUpdateCDS_Test_Error(void)
{
    DS_AppData.DataStoreHandle = 1;

    /* Set to generate error message DS_INIT_CDS_ERR_EID */
    Ut_CFE_ES_SetReturnCode(UT_CFE_ES_COPYTOCDS_INDEX, -1, 1);

    /* Execute the function being tested */
    DS_TableUpdateCDS();
    
    /* Verify results */
    UtAssert_True
        (Ut_CFE_EVS_EventSent(DS_INIT_CDS_ERR_EID, CFE_EVS_ERROR, "Critical Data Store access error = 0xFFFFFFFF"),
        "Critical Data Store access error = 0xFFFFFFFF");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");
} /* end DS_TableUpdateCDS_Test_Error */

void DS_TableHashFunction_Test(void)
{
    uint32         Result;
    CFE_SB_MsgId_t MessageID = 0x18BB;

    /* Execute the function being tested */
    Result = DS_TableHashFunction(MessageID);
    
    /* Verify results */
    UtAssert_True (Result == 187, "Result == 187");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableHashFunction_Test */

void DS_TableCreateHash_Test_Nominal(void)
{
    DS_HashLink_t     HashLink1;
    DS_HashLink_t     HashLink2;
    DS_FilterTable_t  FilterTable; 

    DS_AppData.FilterTblPtr = &FilterTable;

    FilterTable.Packet[0].MessageID = 0x18BB;

    DS_AppData.HashTable[187] = &HashLink1;

    DS_AppData.HashLinks[0].Index = 0;
    DS_AppData.HashLinks[0].MessageID = 0x18BB;
    DS_AppData.HashTable[187]->Next = &HashLink2;

    /* Execute the function being tested */
    DS_TableCreateHash();
    
    /* Verify results */
    UtAssert_True (DS_AppData.HashLinks[0].Index == 0, "DS_AppData.HashLinks[0].Index == 0");
    UtAssert_True (DS_AppData.HashLinks[0].MessageID == 0x18BB, "DS_AppData.HashLinks[0].MessageID == 0x18BB");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableCreateHash_Test_Nominal */

void DS_TableCreateHash_Test_NullTable(void)
{
    DS_FilterTable_t  FilterTable;

    DS_AppData.FilterTblPtr = &FilterTable;

    FilterTable.Packet[0].MessageID = 0x18BB;

    /* Execute the function being tested */
    DS_TableCreateHash();
    
    /* Verify results */
    UtAssert_True (DS_AppData.HashLinks[0].Index == 0, "DS_AppData.HashLinks[0].Index == 0");
    UtAssert_True (DS_AppData.HashLinks[0].MessageID == 0x18BB, "DS_AppData.HashLinks[0].MessageID == 0x18BB");
    UtAssert_True (DS_AppData.HashTable[187] == &DS_AppData.HashLinks[0], "DS_AppData.HashTable[187] == &DS_AppData.HashLinks[0]");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableCreateHash_Test_NullTable */

void DS_TableFindMsgID_Test(void)
{
    int32             Result;
    CFE_SB_MsgId_t    MessageID = 0x18BB;
    DS_HashLink_t     HashLink;
    DS_FilterTable_t  FilterTable;

    DS_AppData.FilterTblPtr = &FilterTable;

    DS_AppData.HashTable[187] = &HashLink;

    HashLink.Index = 1;

    DS_AppData.FilterTblPtr->Packet[HashLink.Index].MessageID = MessageID;

    /* Execute the function being tested */
    Result = DS_TableFindMsgID(MessageID);
    
    /* Verify results */
    UtAssert_True (Result == 1, "Result == 1");

    UtAssert_True (Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");
} /* end DS_TableFindMsgID_Test */

void DS_Table_Test_AddTestCases(void)
{
    UtTest_Add(DS_TableInit_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableInit_Test_Nominal");
    UtTest_Add(DS_TableInit_Test_TableInfoRecovered, DS_Test_Setup, DS_Test_TearDown, "DS_TableInit_Test_TableInfoRecovered");
    UtTest_Add(DS_TableInit_Test_RegisterDestTableError, DS_Test_Setup, DS_Test_TearDown, "DS_TableInit_Test_RegisterDestTableError");
    UtTest_Add(DS_TableInit_Test_RegisterFilterTableError, DS_Test_Setup, DS_Test_TearDown, "DS_TableInit_Test_RegisterFilterTableError");
    UtTest_Add(DS_TableInit_Test_LoadDestTableError, DS_Test_Setup, DS_Test_TearDown, "DS_TableInit_Test_LoadDestTableError");
    UtTest_Add(DS_TableInit_Test_LoadFilterTableError, DS_Test_Setup, DS_Test_TearDown, "DS_TableInit_Test_LoadFilterTableError");

    UtTest_Add(DS_TableManageDestFile_Test_TableInfoUpdated, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageDestFile_Test_TableInfoUpdated");
    UtTest_Add(DS_TableManageDestFile_Test_TableNeverLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageDestFile_Test_TableNeverLoaded");
    UtTest_Add(DS_TableManageDestFile_Test_TableInfoDumpPending, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageDestFile_Test_TableInfoDumpPending");
    UtTest_Add(DS_TableManageDestFile_Test_TableInfoValidationPending, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageDestFile_Test_TableInfoValidationPending");
    UtTest_Add(DS_TableManageDestFile_Test_TableInfoUpdatePending, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageDestFile_Test_TableInfoUpdatePending");

    UtTest_Add(DS_TableManageFilter_Test_TableInfoUpdated, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageFilter_Test_TableInfoUpdated");
    UtTest_Add(DS_TableManageFilter_Test_TableNeverLoaded, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageFilter_Test_TableNeverLoaded");
    UtTest_Add(DS_TableManageFilter_Test_TableInfoDumpPending, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageFilter_Test_TableInfoDumpPending");
    UtTest_Add(DS_TableManageFilter_Test_TableInfoValidationPending, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageFilter_Test_TableInfoValidationPending");
    UtTest_Add(DS_TableManageFilter_Test_TableInfoUpdatePending, DS_Test_Setup, DS_Test_TearDown, "DS_TableManageFilter_Test_TableInfoUpdatePending");

    UtTest_Add(DS_TableVerifyDestFile_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFile_Test_Nominal");
    UtTest_Add(DS_TableVerifyDestFile_Test_DestFileTableVerificationError, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFile_Test_DestFileTableVerificationError");
    UtTest_Add(DS_TableVerifyDestFile_Test_CountBad, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFile_Test_CountBad");

    UtTest_Add(DS_TableVerifyDestFileEntry_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_Nominal");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidPathname, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidPathname");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidBasename, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidBasename");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidExtension, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidExtension");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidFilenameType, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidFilenameType");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidFileEnableState, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidFileEnableState");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidSize, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidSize");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidAge, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidAge");
    UtTest_Add(DS_TableVerifyDestFileEntry_Test_InvalidSequenceCount, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyDestFileEntry_Test_InvalidSequenceCount");

    UtTest_Add(DS_TableVerifyFilter_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFilter_Test_Nominal");
    UtTest_Add(DS_TableVerifyFilter_Test_FilterTableVerificationError, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFilter_Test_FilterTableVerificationError");
    UtTest_Add(DS_TableVerifyFilter_Test_CountBad, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFilter_Test_CountBad");

    UtTest_Add(DS_TableVerifyFilterEntry_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFilterEntry_Test_Nominal");
    UtTest_Add(DS_TableVerifyFilterEntry_Test_InvalidFileTableIndex, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFilterEntry_Test_InvalidFileTableIndex");
    UtTest_Add(DS_TableVerifyFilterEntry_Test_InvalidFilterType, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFilterEntry_Test_InvalidFilterType");
    UtTest_Add(DS_TableVerifyFilterEntry_Test_InvalidFilterParms, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFilterEntry_Test_InvalidFilterParms");

    UtTest_Add(DS_TableEntryUnused_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableEntryUnused_Test_Nominal");
    UtTest_Add(DS_TableEntryUnused_Test_Fail, DS_Test_Setup, DS_Test_TearDown, "DS_TableEntryUnused_Test_Fail");

    UtTest_Add(DS_TableVerifyFileIndex_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFileIndex_Test_Nominal");
    UtTest_Add(DS_TableVerifyFileIndex_Test_Fail, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyFileIndex_Test_Fail");

    UtTest_Add(DS_TableVerifyParms_Test_NominalOnlyXNonZero, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyParms_Test_NominalOnlyXNonZero");
    UtTest_Add(DS_TableVerifyParms_Test_NGreaterThanXOnlyNNonZero, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyParms_Test_NGreaterThanXOnlyNNonZero");
    UtTest_Add(DS_TableVerifyParms_Test_OGreaterThanXOnlyONonZero, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyParms_Test_OGreaterThanXOnlyONonZero");

    UtTest_Add(DS_TableVerifyType_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyType_Test_Nominal");
    UtTest_Add(DS_TableVerifyType_Test_Fail, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyType_Test_Fail");

    UtTest_Add(DS_TableVerifyState_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyState_Test_Nominal");
    UtTest_Add(DS_TableVerifyState_Test_Fail, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyState_Test_Fail");

    UtTest_Add(DS_TableVerifySize_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifySize_Test_Nominal");
    UtTest_Add(DS_TableVerifySize_Test_Fail, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifySize_Test_Fail");

    UtTest_Add(DS_TableVerifyAge_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyAge_Test_Nominal");
    UtTest_Add(DS_TableVerifyAge_Test_Fail, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyAge_Test_Fail");

    UtTest_Add(DS_TableVerifyCount_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyCount_Test_Nominal");
    UtTest_Add(DS_TableVerifyCount_Test_Fail, DS_Test_Setup, DS_Test_TearDown, "DS_TableVerifyCount_Test_Fail");

    UtTest_Add(DS_TableSubscribe_Test, DS_Test_Setup, DS_Test_TearDown, "DS_TableSubscribe_Test");

    UtTest_Add(DS_TableUnsubscribe_Test, DS_Test_Setup, DS_Test_TearDown, "DS_TableUnsubscribe_Test");

    UtTest_Add(DS_TableCreateCDS_Test_NewCDSArea, DS_Test_Setup, DS_Test_TearDown, "DS_TableCreateCDS_Test_NewCDSArea");
    UtTest_Add(DS_TableCreateCDS_Test_PreExistingCDSArea, DS_Test_Setup, DS_Test_TearDown, "DS_TableCreateCDS_Test_PreExistingCDSArea");
    UtTest_Add(DS_TableCreateCDS_Test_Error, DS_Test_Setup, DS_Test_TearDown, "DS_TableCreateCDS_Test_Error");

    UtTest_Add(DS_TableUpdateCDS_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableUpdateCDS_Test_Nominal");
    UtTest_Add(DS_TableUpdateCDS_Test_Error, DS_Test_Setup, DS_Test_TearDown, "DS_TableUpdateCDS_Test_Error");

    UtTest_Add(DS_TableHashFunction_Test, DS_Test_Setup, DS_Test_TearDown, "DS_TableHashFunction_Test");

    UtTest_Add(DS_TableCreateHash_Test_Nominal, DS_Test_Setup, DS_Test_TearDown, "DS_TableCreateHash_Test_Nominal");
    UtTest_Add(DS_TableCreateHash_Test_NullTable, DS_Test_Setup, DS_Test_TearDown, "DS_TableCreateHash_Test_NullTable");

    UtTest_Add(DS_TableFindMsgID_Test, DS_Test_Setup, DS_Test_TearDown, "DS_TableFindMsgID_Test");
} /* end DS_Table_Test_AddTestCases */


/************************/
/*  End of File Comment */
/************************/
