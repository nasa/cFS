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

#include "os-vxworks-coveragetest.h"

#include <overrides/stdlib.h>
#include <overrides/taskLib.h>
#include <overrides/shellLib.h>
#include <overrides/dirent.h>
#include <overrides/unistd.h>
#include <overrides/sys/stat.h>

#include "ut-osfileapi.h"


void Test_OS_VxWorks_StreamAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_Works_StreamAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_StreamAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_VxWorks_DirAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_DirAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_DirAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_DirCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl("dir", 0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_mkdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl("dir", 0), OS_ERROR);
}

void Test_OS_DirOpen_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirOpen_Impl(uint32 local_id, const char *local_path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirOpen_Impl(0, "dir"), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_opendir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirOpen_Impl(0, "dir"), OS_ERROR);
}

void Test_OS_DirClose_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirClose_Impl(uint32 local_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirClose_Impl(0), OS_SUCCESS);
}

void Test_OS_DirRead_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRead_Impl(uint32 local_id, os_dirent_t *dirent)
     */
    os_dirent_t dirent_buff;

    OSAPI_TEST_FUNCTION_RC(OS_DirRead_Impl(0, &dirent_buff), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_readdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirRead_Impl(0, &dirent_buff), OS_ERROR);

}

void Test_OS_DirRewind_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRewind_Impl(uint32 local_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirRewind_Impl(0), OS_SUCCESS);
}

void Test_OS_DirRemove_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRemove_Impl(const char *local_path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirRemove_Impl("dir"), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_rmdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirRemove_Impl("dir"), OS_ERROR);
}

void Test_OS_ShellOutputToFile_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char *Cmd)
     */
    int32 expected = OS_SUCCESS;
    int32 actual;

    /*
     * The ShellOutputToFile will loop until the
     * taskNameToId() function returns ERROR, so this
     * must be set to avoid getting into an endless loop.
     */
    UT_SetDeferredRetcode(UT_KEY(OCS_taskNameToId), 2, -1);

    actual = OS_ShellOutputToFile_Impl(0, "TestCmd");

    UtAssert_True(actual == expected, "OS_ShellOutputToFile_Impl() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_shellGenericInit)) == 1, "shellGenericInit() called");

    /* failure to open the output file */
    UT_SetForceFail(UT_KEY(OS_creat), OS_ERROR);
    expected = OS_ERROR;
    actual = OS_ShellOutputToFile_Impl(0, "TestCmd");
    UtAssert_True(actual == expected, "OS_ShellOutputToFile_Impl() (%ld) == OS_ERROR", (long)actual);
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

/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_VxWorks_StreamAPI_Impl_Init);
    ADD_TEST(OS_VxWorks_DirAPI_Impl_Init);
    ADD_TEST(OS_DirCreate_Impl);
    ADD_TEST(OS_DirOpen_Impl);
    ADD_TEST(OS_DirClose_Impl);
    ADD_TEST(OS_DirRead_Impl);
    ADD_TEST(OS_DirRewind_Impl);
    ADD_TEST(OS_DirRemove_Impl);
    ADD_TEST(OS_ShellOutputToFile_Impl);
}


