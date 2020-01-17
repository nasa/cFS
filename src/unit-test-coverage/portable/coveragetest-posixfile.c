/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osfileapi-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include <utassert.h>
#include <uttest.h>
#include <utstubs.h>

#include <osapi.h>
#include <os-impl.h>

#include "ut-osfileapi.h"

#include <OCS_stdio.h>
#include <OCS_stdlib.h>
#include <OCS_unistd.h>
#include <OCS_fcntl.h>
#include <OCS_stat.h>

#define OSAPI_TEST_FUNCTION_RC(func,exp)   \
{                                               \
    int32 rcexp = exp;                          \
    int32 rcact = func;                    \
    UtAssert_True(rcact == rcexp, "%s (%ld) == %s (%ld)", \
        #func, (long)rcact, #exp, (long)rcexp);      \
}



void Test_OS_FileOpen_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileOpen_Impl(uint32 local_id, const char *local_path, int32 flags, int32 access)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FileOpen_Impl(0,"local",OS_FILE_FLAG_TRUNCATE,OS_WRITE_ONLY), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileOpen_Impl(0,"local",0,OS_READ_ONLY), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileOpen_Impl(0,"local",OS_FILE_FLAG_CREATE,OS_READ_WRITE), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileOpen_Impl(0,"local",0,-1234), OS_FS_ERROR);


    /* failure mode */
    UT_SetForceFail(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileOpen_Impl(0,"local",0,OS_READ_ONLY), OS_ERROR);
}

void Test_OS_FileStat_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileStat_Impl(const char *local_path, os_fstat_t *FileStats)
     */
    os_fstat_t FileStats;
    struct OCS_stat RefStat;

    /* failure mode */
    UT_SetForceFail(UT_KEY(OCS_stat), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileStat_Impl("local",&FileStats), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_stat));

    /* nominal, no permission bits */
    memset(&FileStats,0,sizeof(FileStats));
    OSAPI_TEST_FUNCTION_RC(OS_FileStat_Impl("local",&FileStats), OS_SUCCESS);
    UtAssert_True(FileStats.FileModeBits == 0, "File Mode Bits unset");

    /* all permission bits with uid/gid match */
    RefStat.st_uid = Osapi_Internal_GetSelfEUID();
    RefStat.st_gid = Osapi_Internal_GetSelfEGID();
    RefStat.st_mode = ~0;
    RefStat.st_size = 1234;
    RefStat.st_mtime = 5678;
    UT_SetDataBuffer(UT_KEY(OCS_stat), &RefStat, sizeof(RefStat), false);
    OSAPI_TEST_FUNCTION_RC(OS_FileStat_Impl("local",&FileStats), OS_SUCCESS);

    /* Test that the result checking macros work */
    UtAssert_True(OS_FILESTAT_EXEC(FileStats), "File Exec Bit set");
    UtAssert_True(OS_FILESTAT_WRITE(FileStats), "File Write Bit set");
    UtAssert_True(OS_FILESTAT_READ(FileStats), "File Read Bit set");
    UtAssert_True(OS_FILESTAT_ISDIR(FileStats), "Directory Bit set");
    UtAssert_True(OS_FILESTAT_SIZE(FileStats) == 1234, "Size match");
    UtAssert_True(OS_FILESTAT_TIME(FileStats) == 5678, "Time match");

}

void Test_OS_FileChmod_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileChmod_Impl(const char *local_path, uint32 access)
     */
    struct OCS_stat RefStat;

    /* failure mode 1 (stat) */
    UT_SetForceFail(UT_KEY(OCS_stat), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileChmod_Impl("local",OS_READ_WRITE), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_stat));

    /* failure mode 2 (chmod) */
    UT_SetForceFail(UT_KEY(OCS_chmod), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileChmod_Impl("local",OS_READ_WRITE), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_chmod));

    /* all permission bits with uid/gid match */
    RefStat.st_uid = Osapi_Internal_GetSelfEUID();
    RefStat.st_gid = Osapi_Internal_GetSelfEGID();
    RefStat.st_mode = ~0;
    RefStat.st_size = 1234;
    RefStat.st_mtime = 5678;
    UT_SetDataBuffer(UT_KEY(OCS_stat), &RefStat, sizeof(RefStat), false);

    /* nominal 1 - full permissions with file owned by own uid/gid */
    OSAPI_TEST_FUNCTION_RC(OS_FileChmod_Impl("local",OS_READ_WRITE), OS_SUCCESS);

    /* nominal 2 - partial permissions */
    OSAPI_TEST_FUNCTION_RC(OS_FileChmod_Impl("local", OS_READ_ONLY), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileChmod_Impl("local", OS_WRITE_ONLY), OS_SUCCESS);

    /* nominal 3 - non-owned file */
    ++RefStat.st_uid;
    ++RefStat.st_gid;
    UT_SetDataBuffer(UT_KEY(OCS_stat), &RefStat, sizeof(RefStat), false);
    OSAPI_TEST_FUNCTION_RC(OS_FileChmod_Impl("local",OS_READ_WRITE), OS_SUCCESS);
}

void Test_OS_FileRemove_Impl (void)
{
    /*
     * Test Case For:
     * int32 OS_FileRemove_Impl(const char *local_path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FileRemove_Impl("local"), OS_SUCCESS);

    /* failure mode */
    UT_SetForceFail(UT_KEY(OCS_remove), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileRemove_Impl("local"), OS_ERROR);

}

void Test_OS_FileRename_Impl (void)
{
    /*
     * Test Case For:
     * int32 OS_FileRename_Impl(const char *old_path, const char *new_path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FileRename_Impl("old","new"), OS_SUCCESS);

    /* failure mode */
    UT_SetForceFail(UT_KEY(OCS_rename), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileRename_Impl("old","new"), OS_ERROR);
}



/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Osapi_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_TearDown(void)
{

}


#define ADD_TEST(test) UtTest_Add((Test_ ## test), Osapi_Task_Setup, Osapi_TearDown, #test)

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_FileOpen_Impl);
    ADD_TEST(OS_FileStat_Impl);
    ADD_TEST(OS_FileChmod_Impl);
    ADD_TEST(OS_FileRemove_Impl);
    ADD_TEST(OS_FileRename_Impl);
}


