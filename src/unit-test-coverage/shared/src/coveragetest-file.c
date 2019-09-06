/*
 * Filename: osapi_testcase_file.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-file" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-shared-coveragetest.h"
#include "ut-osapi-file.h"

#include <overrides/string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_FileAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_FileAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_FileAPI_Init();

    UtAssert_True(actual == expected, "OS_FileAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_creat(void)
{
    /*
     * Test Case For:
     * int32 OS_creat  (const char *path, int32  access)
     */
    int32 actual = OS_creat("/cf/file", OS_READ_WRITE);

    UtAssert_True(actual >= 0, "OS_creat() (%ld) >= 0", (long)actual);

    actual = OS_creat("/cf/file", OS_READ_ONLY);
    UtAssert_True(actual == OS_FS_ERROR, "OS_creat() (%ld) == OS_FS_ERROR", (long)actual);

    actual = OS_creat(NULL, OS_WRITE_ONLY);
    UtAssert_True(actual == OS_FS_ERR_INVALID_POINTER, "OS_creat() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_PATH_LEN);
    actual = OS_creat("/file", OS_WRITE_ONLY);
    UtAssert_True(actual == OS_FS_ERR_PATH_TOO_LONG, "OS_creat() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    UT_SetForceFail(UT_KEY(OCS_strrchr), -1);
    actual = OS_creat("/file", OS_WRITE_ONLY);
    UtAssert_True(actual == OS_FS_ERROR, "OS_creat() (%ld) == OS_FS_ERROR", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strrchr));

    UT_SetDeferredRetcode(UT_KEY(OCS_strlen), 2, 2 + OS_MAX_FILE_NAME);
    actual = OS_creat("/file", OS_WRITE_ONLY);
    UtAssert_True(actual == OS_FS_ERR_NAME_TOO_LONG, "OS_creat() (%ld) == OS_FS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

}

void Test_OS_open(void)
{
    /*
     * Test Case For:
     * int32 OS_open   (const char *path,  int32 access,  uint32  mode)
     */
    int32 actual = OS_open("/cf/file", OS_READ_WRITE, 0);

    UtAssert_True(actual > 0, "OS_open() (%ld) > 0", (long)actual);


    actual = OS_open("/cf/file", -1, 0);
    UtAssert_True(actual < 0, "OS_open() (%ld) < 0", (long)actual);
}


void Test_OS_close(void)
{
    /*
     * Test Case For:
     * int32 OS_close (uint32 filedes)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_close(1);

    UtAssert_True(actual == expected, "OS_close() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_TimedRead(void)
{
    /*
     * Test Case For:
     * int32 OS_TimedRead(uint32  filedes, void *buffer, uint32 nbytes, int32 timeout)
     */
    char Buf[4] = "zzz";
    char SrcBuf[8] = "rrrrrrr";
    int32 expected = sizeof(Buf);
    int32 actual = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), SrcBuf, sizeof(SrcBuf), false);
    actual = OS_TimedRead(1, Buf, sizeof(Buf), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld",
            (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf,SrcBuf,actual) == 0, "buffer content match");

    expected = OS_INVALID_POINTER;
    actual = OS_TimedRead(1, NULL, sizeof(Buf), 10);
    UtAssert_True(actual == expected, "OS_TimedRead() (%ld) == %ld",
            (long)actual, (long)expected);
}


void Test_OS_TimedWrite(void)
{
    /*
     * Test Case For:
     * int32 OS_TimedWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout)
     */
    const char Buf[4] = "www";
    char DstBuf[8] = "zzz";
    int32 expected = sizeof(Buf);
    int32 actual = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericWrite_Impl), DstBuf, sizeof(DstBuf), false);
    actual = OS_TimedWrite(1, Buf, sizeof(Buf), 10);

    UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld",
            (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf,DstBuf,actual) == 0, "buffer content match");

    expected = OS_INVALID_POINTER;
    actual = OS_TimedWrite(1, NULL, sizeof(Buf), 10);
    UtAssert_True(actual == expected, "OS_TimedWrite() (%ld) == %ld",
            (long)actual, (long)expected);
}


void Test_OS_read(void)
{
    /*
     * Test Case For:
     * int32 OS_read  (uint32  filedes, void *buffer, uint32 nbytes)
     */
    char Buf[4] = "zzz";
    char SrcBuf[8] = "RRRRRRR";
    int32 expected = sizeof(Buf);
    int32 actual = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), SrcBuf, sizeof(SrcBuf), false);
    actual = OS_read(1, Buf, sizeof(Buf));
    UtAssert_True(actual == expected, "OS_read() (%ld) == %ld",
            (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf,SrcBuf,actual) == 0, "buffer content match");
}


void Test_OS_write(void)
{
    /*
     * Test Case For:
     * int32 OS_write (uint32  filedes, const void *buffer, uint32 nbytes)
     */
    const char Buf[4] = "WWW";
    char DstBuf[8] = "zzz";
    int32 expected = sizeof(Buf);
    int32 actual = 0;

    UT_SetDataBuffer(UT_KEY(OS_GenericWrite_Impl), DstBuf, sizeof(DstBuf), false);
    actual = OS_write(1, Buf, sizeof(Buf));

    UtAssert_True(actual == expected, "OS_write() (%ld) == %ld",
            (long)actual, (long)expected);
    UtAssert_True(memcmp(Buf,DstBuf,actual) == 0, "buffer content match");

}


void Test_OS_chmod(void)
{
    /*
     * Test Case For:
     * int32 OS_chmod  (const char *path, uint32 access)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_chmod("/cf/file", 0);

    UtAssert_True(actual == expected, "OS_chmod() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_stat(void)
{
    /*
     * Test Case For:
     * int32 OS_stat   (const char *path, OS_FileStat_t *filestats)
     */
    int32 expected = OS_SUCCESS;
    os_fstat_t statbuf;
    int32 actual = OS_stat("/cf/file", &statbuf);

    UtAssert_True(actual == expected, "OS_stat() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_stat("/cf/file", NULL);
    UtAssert_True(actual == expected, "OS_stat() (%ld) == OS_INVALID_POINTER", (long)actual);
}


void Test_OS_lseek(void)
{
    /*
     * Test Case For:
     * int32 OS_lseek  (uint32  filedes, int32 offset, uint32 whence)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_lseek(1, 0, 0);

    UtAssert_True(actual == expected, "OS_lseek() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_remove(void)
{
    /*
     * Test Case For:
     * int32 OS_remove (const char *path)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_remove("/cf/file");

    UtAssert_True(actual == expected, "OS_remove() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_rename(void)
{
    /*
     * Test Case For:
     * int32 OS_rename (const char *old, const char *new)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    OS_global_stream_table[1].active_id = 1;
    strncpy(OS_stream_table[1].stream_name, "/cf/file1",
            sizeof(OS_stream_table[1].stream_name));
    actual = OS_rename("/cf/file1", "/cf/file2");

    UtAssert_True(actual == expected, "OS_rename() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(OS_stream_table[1].stream_name, "/cf/file2") == 0,
            "OS_stream_table[1].stream_name (%s) == /cf/file2",
            OS_stream_table[1].stream_name);
}


void Test_OS_cp(void)
{
    /*
     * Test Case For:
     * int32 OS_cp (const char *src, const char *dest)
     */
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual = OS_cp(NULL,NULL);
    char ReadBuf[] = "cpcpcpcp";
    char WriteBuf[sizeof(ReadBuf)] = "";

    UtAssert_True(actual == expected, "OS_cp() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    /* setup to make internal copy loop execute at least once */
    expected = OS_SUCCESS;
    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), ReadBuf, sizeof(ReadBuf), false);
    UT_SetDataBuffer(UT_KEY(OS_GenericWrite_Impl), WriteBuf, sizeof(WriteBuf), false);
    actual = OS_cp("/cf/file1", "/cf/file2");

    UtAssert_True(actual == expected, "OS_cp() (%ld) == OS_SUCCESS", (long)actual);

    UT_SetForceFail(UT_KEY(OS_GenericRead_Impl), -444);
    expected = -444;
    actual = OS_cp("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_cp() (%ld) == -444", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_GenericRead_Impl));

    UT_SetDataBuffer(UT_KEY(OS_GenericRead_Impl), ReadBuf, sizeof(ReadBuf), false);
    UT_SetForceFail(UT_KEY(OS_GenericWrite_Impl), -555);
    expected = -555;
    actual = OS_cp("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_cp() (%ld) == -555", (long)actual);

    UT_SetDeferredRetcode(UT_KEY(OCS_strrchr), 1, -1);
    expected = OS_FS_ERROR;
    actual = OS_cp("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_cp() (%ld) == OS_INVALID_POINTER", (long)actual);
    UT_SetDeferredRetcode(UT_KEY(OCS_strrchr), 2, -1);
    actual = OS_cp("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_cp() (%ld) == OS_INVALID_POINTER", (long)actual);
}


void Test_OS_mv(void)
{
    /*
     * Test Case For:
     * int32 OS_mv (const char *src, const char *dest)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_mv("/cf/file1", "/cf/file2");

    UtAssert_True(actual == expected, "OS_mv() (%ld) == OS_SUCCESS", (long)actual);

    /* In the default case, the implementation tries to rename first.
     * Force rename to fail so it does a full copy and remove */
    UT_SetForceFail(UT_KEY(OS_FileRename_Impl), OS_ERROR);
    actual = OS_mv("/cf/file1", "/cf/file2");
    UtAssert_True(actual == expected, "OS_mv() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_FDGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_FDGetInfo (uint32 filedes, OS_file_prop_t *fd_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_file_prop_t file_prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_FDGetInfo(1, &file_prop);

    UtAssert_True(actual == expected, "OS_FDGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(file_prop.User == 111, "file_prop.User (%lu) == 111",
            (unsigned long)file_prop.User);
    UtAssert_True(strcmp(file_prop.Path, "ABC") == 0, "file_prop.Path (%s) == ABC",
            file_prop.Path);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_FDGetInfo(1, NULL);
    UtAssert_True(actual == expected, "OS_FDGetInfo() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

}

void Test_OS_FileOpenCheck(void)
{
    /*
     * Test Case For:
     * int32 OS_FileOpenCheck(const char *Filename)
     */
    int32 expected = OS_FS_ERROR;
    int32 actual = OS_FileOpenCheck("/cf/file");

    UtAssert_True(actual == expected, "OS_FileOpenCheck() (%ld) == OS_FS_ERROR", (long)actual);

    OS_global_stream_table[0].active_id = 1;
    UT_SetForceFail(UT_KEY(OCS_strcmp), 0);
    expected = OS_SUCCESS;
    actual = OS_FileOpenCheck("/cf/file");

    UtAssert_True(actual == expected, "OS_FileOpenCheck() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_FileOpenCheck(NULL);
    UtAssert_True(actual == expected, "OS_FDGetInfo() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

}


void Test_OS_CloseFileByName(void)
{
    /*
     * Test Case For:
     * int32 OS_CloseFileByName(const char *Filename)
     */
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual = OS_CloseFileByName("/cf/file");

    UtAssert_True(actual == expected, "OS_CloseFileByName() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    /* setup for success */
    expected = OS_SUCCESS;
    OS_global_stream_table[0].active_id = 1;
    UT_SetForceFail(UT_KEY(OCS_strcmp), 0);
    actual = OS_CloseFileByName("/cf/file");
    UtAssert_True(actual == expected, "OS_CloseFileByName() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_CloseFileByName(NULL);
    UtAssert_True(actual == expected, "OS_CloseFileByName() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

}


void Test_OS_CloseAllFiles(void)
{
    /*
     * Test Case For:
     * int32 OS_CloseAllFiles(void)
     */
    int32 expected = -222;
    int32 actual;

    OS_global_stream_table[0].active_id = 1;
    OS_global_stream_table[1].active_id = 2;
    UT_SetDeferredRetcode(UT_KEY(OS_GenericClose_Impl), 1, expected);
    actual = OS_CloseAllFiles();

    UtAssert_True(actual == expected, "OS_CloseAllFiles() (%ld) == -222", (long)actual);
}


void Test_OS_ShellOutputToFile(void)
{
    /*
     * Test Case For:
     * int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_ShellOutputToFile("Cmd", 1);

    UtAssert_True(actual == expected, "OS_ShellOutputToFile() (%ld) == OS_SUCCESS", (long)actual);


    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_ShellOutputToFile(NULL, 1);

    UtAssert_True(actual == expected, "OS_ShellOutputToFile() (%ld) == OS_SUCCESS", (long)actual);
}





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

/*
 * Register the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_FileAPI_Init);
    ADD_TEST(OS_creat);
    ADD_TEST(OS_open);
    ADD_TEST(OS_close);
    ADD_TEST(OS_TimedRead);
    ADD_TEST(OS_TimedWrite);
    ADD_TEST(OS_read);
    ADD_TEST(OS_write);
    ADD_TEST(OS_chmod);
    ADD_TEST(OS_stat);
    ADD_TEST(OS_lseek);
    ADD_TEST(OS_remove);
    ADD_TEST(OS_rename);
    ADD_TEST(OS_cp);
    ADD_TEST(OS_mv);
    ADD_TEST(OS_FDGetInfo);
    ADD_TEST(OS_FileOpenCheck);
    ADD_TEST(OS_CloseFileByName);
    ADD_TEST(OS_CloseAllFiles);
    ADD_TEST(OS_ShellOutputToFile);
}





