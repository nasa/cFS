/*
 * Filename: osapi_testcase_dir.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-dir" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-shared-coveragetest.h"
#include "ut-osapi-dir.h"

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_DirAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_DirAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_DirAPI_Init();

    UtAssert_True(actual == expected, "OS_DirAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_mkdir(void)
{
    /*
     * Test Case For:
     * int32 OS_mkdir (const char *path, uint32 access)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_mkdir("Dir", 0);

    UtAssert_True(actual == expected, "OS_mkdir() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_DirectoryOpen(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryOpen(uint32 *dir_id, const char *path)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual = OS_DirectoryOpen(&objid, "Dir");

    UtAssert_True(actual == expected, "OS_DirectoryOpen() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    OSAPI_TEST_FUNCTION_RC(OS_DirectoryOpen(NULL, NULL), OS_INVALID_POINTER);
}


void Test_OS_DirectoryClose(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryClose(uint32 dir_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_DirectoryClose(1);

    UtAssert_True(actual == expected, "OS_DirectoryClose() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_DirectoryRead(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryRead(uint32 dir_id, OS_DirEntry_t *dirent)
     */
    int32 expected = OS_SUCCESS;
    os_dirent_t dirent;

    int32 actual = OS_DirectoryRead(1, &dirent);

    UtAssert_True(actual == expected, "OS_DirectoryRead() (%ld) == OS_SUCCESS", (long)actual);

    OSAPI_TEST_FUNCTION_RC(OS_DirectoryRead(1, NULL), OS_INVALID_POINTER);
}


void Test_OS_DirectoryRewind(void)
{
    /*
     * Test Case For:
     * int32 OS_DirectoryRewind(uint32 dir_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_DirectoryRewind(1);

    UtAssert_True(actual == expected, "OS_DirectoryRewind() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_rmdir(void)
{
    /*
     * Test Case For:
     * int32  OS_rmdir (const char *path)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_rmdir("Dir");

    UtAssert_True(actual == expected, "OS_rmdir() (%ld) == OS_SUCCESS", (long)actual);
}



/*
 * Compatibility layers for old-style API
 */
#ifndef OSAL_OMIT_DEPRECATED

void Test_OS_opendir(void)
{
    /*
     * Test Case For:
     * os_dirp_t OS_opendir (const char *path)
     */
    os_dirp_t dirp = OS_opendir("Dir");

    UtAssert_True(dirp != NULL, "OS_opendir() (%p) != NULL", dirp);
}


void Test_OS_closedir(void)
{
    /*
     * Test Case For:
     * int32 OS_closedir (os_dirp_t directory)
     */
    int32 expected = OS_SUCCESS;
    os_dirp_t dirp = OS_opendir("Dir");
    int32 actual = OS_closedir(dirp);

    UtAssert_True(actual == expected, "OS_closedir() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_readdir(void)
{
    /*
     * Test Case For:
     * os_dirent_t *OS_readdir (os_dirp_t directory)
     */
    os_dirp_t dirp = OS_opendir("Dir");
    os_dirent_t *dirent = OS_readdir(dirp);

    UtAssert_True(dirent != NULL, "OS_readdir() (%p) != NULL", dirent);
}


void Test_OS_rewinddir(void)
{
    /*
     * Test Case For:
     * void  OS_rewinddir (os_dirp_t directory )
     */
    os_dirp_t dirp = OS_opendir("Dir");
    OS_rewinddir(dirp);
}


#endif



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
    ADD_TEST(OS_DirAPI_Init);
    ADD_TEST(OS_mkdir);
    ADD_TEST(OS_DirectoryOpen);
    ADD_TEST(OS_DirectoryClose);
    ADD_TEST(OS_DirectoryRead);
    ADD_TEST(OS_DirectoryRewind);
    ADD_TEST(OS_rmdir);
#ifndef OSAL_OMIT_DEPRECATED
    ADD_TEST(OS_opendir);
    ADD_TEST(OS_closedir);
    ADD_TEST(OS_readdir);
    ADD_TEST(OS_rewinddir);
#endif
}





