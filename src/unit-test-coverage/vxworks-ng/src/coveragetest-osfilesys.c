/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osfilesys-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-vxworks-coveragetest.h"
#include "ut-osfilesys.h"

#include <overrides/stdlib.h>
#include <overrides/sys/ioctl.h>
#include <overrides/sys/statvfs.h>
#include <overrides/fcntl.h>
#include <overrides/unistd.h>
#include <overrides/ramDrv.h>
#include <overrides/dosFsLib.h>
#include <overrides/xbdBlkDev.h>




void Test_OS_FileSysStartVolume_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysStartVolume_Impl (uint32 filesys_id)
     */
    int32 expected;

    /* Emulate an FS_BASED entry */
    OS_filesys_table[0].fstype = OS_FILESYS_TYPE_DEFAULT;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(0), OS_ERR_NOT_IMPLEMENTED);

    /* Emulate a VOLATILE_DISK entry (ramdisk) */
    OS_filesys_table[1].fstype = OS_FILESYS_TYPE_VOLATILE_DISK;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(1), OS_SUCCESS);

    /* Emulate a NORMAL_DISK entry (ATA) */
    OS_filesys_table[2].fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    /* the expected value depends on whether it is compiled with USE_VXWORKS_ATA_DRIVER */
#ifdef USE_VXWORKS_ATA_DRIVER
    expected = OS_SUCCESS;
#else
    expected = OS_ERR_NOT_IMPLEMENTED;
#endif
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(2), expected);

    /* Failure to create XBD layer */
    UT_SetForceFail(UT_KEY(OCS_xbdBlkDevCreateSync), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(1), OS_FS_ERR_DRIVE_NOT_CREATED);

    /* Failure to create low level block dev */
    UT_SetForceFail(UT_KEY(OCS_ramDevCreate), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStartVolume_Impl(1), OS_FS_ERR_DRIVE_NOT_CREATED);
}


;
void Test_OS_FileSysStopVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysStopVolume_Impl (uint32 filesys_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStopVolume_Impl(0), OS_SUCCESS);

    /* Failure to delete XBD layer */
    Osapi_Internal_SetupFileSysEntry(1, NULL, 1, 4);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStopVolume_Impl(1), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_xbdBlkDevDelete)) == 1, "xbdBlkDevDelete() called");
}

void Test_OS_FileSysFormatVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysFormatVolume_Impl (uint32 filesys_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FileSysFormatVolume_Impl(0), OS_SUCCESS);

    /* Failure of the dosFsVolFormat() call */
    UT_SetForceFail(UT_KEY(OCS_dosFsVolFormat), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysFormatVolume_Impl(0), OS_FS_ERR_DRIVE_NOT_CREATED);
}


void Test_OS_FileSysMountVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysMountVolume_Impl (uint32 filesys_id)
     */

    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysMountVolume_Impl(0), OS_FS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_open));
}


void Test_OS_FileSysUnmountVolume_Impl(void)
{
    /* Test Case For:
     * int32 OS_FileSysUnmountVolume_Impl (uint32 filesys_id)
     */

    OSAPI_TEST_FUNCTION_RC(OS_FileSysUnmountVolume_Impl(0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysUnmountVolume_Impl(0), OS_FS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_open));

    UT_SetForceFail(UT_KEY(OCS_ioctl), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysUnmountVolume_Impl(0), OS_FS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_ioctl));
}

void Test_OS_FileSysStatVolume_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysStatVolume_Impl (uint32 filesys_id, OS_statvfs_t *result)
     */
    OS_statvfs_t stat;
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStatVolume_Impl(0,&stat), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_statvfs), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysStatVolume_Impl(0,&stat), OS_FS_ERROR);
}

void Test_OS_FileSysCheckVolume_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysCheckVolume_Impl (uint32 filesys_id, bool repair)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FileSysCheckVolume_Impl(0,true), OS_SUCCESS);


    UT_SetForceFail(UT_KEY(OCS_open), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysCheckVolume_Impl(0,false), OS_FS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_open));

    UT_SetForceFail(UT_KEY(OCS_ioctl), -1);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysCheckVolume_Impl(0,false), OS_FS_ERROR);
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
    memset(OS_filesys_table, 0, sizeof(OS_filesys_table));
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
    ADD_TEST(OS_FileSysStartVolume_Impl);
    ADD_TEST(OS_FileSysStopVolume_Impl);
    ADD_TEST(OS_FileSysFormatVolume_Impl);
    ADD_TEST(OS_FileSysCheckVolume_Impl);
    ADD_TEST(OS_FileSysStatVolume_Impl);
    ADD_TEST(OS_FileSysMountVolume_Impl);
    ADD_TEST(OS_FileSysUnmountVolume_Impl);
}

