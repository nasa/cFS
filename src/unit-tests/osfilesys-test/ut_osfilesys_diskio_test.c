/*================================================================================*
** File:  ut_osfilesys_diskio_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osfilesys_diskio_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t g_logInfo;

extern char* g_fsAddrPtr;

extern int32 g_blkSize;
extern int32 g_blkCnt;

extern int32  g_skipTestCase;
extern char*  g_skipTestCaseResult;

extern char  g_fsLongName[OS_MAX_PATH_LEN+5];
extern char  g_physDriveName[OS_MAX_PATH_LEN];

extern char  g_volNames[UT_OS_FILESYS_LIST_LEN][UT_OS_XS_TEXT_LEN];
extern char  g_devNames[UT_OS_FILESYS_LIST_LEN][UT_OS_XS_TEXT_LEN];
extern char  g_mntNames[UT_OS_FILESYS_LIST_LEN][UT_OS_XS_TEXT_LEN];

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/* Test code template for testing a single OSAL API with multiple test cases */

#if 0
void UT_os_sample_test()
{
    /* Must declare these variables for each function. They can be renamed.
     * They're referenced in the macros used to track test cases and their results. */
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;

    /* Call this once at the beginning of the function to initialize the test variables. */
    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*
     * For each test case,
     *   1. Assign testDesc a brief description of the test
     *   2. Setup the test environment, if necessary
     *   3. Run the test
     *   4. Log result by calling UT_OS_SET_TEST_RESULT_MACRO
     *   4. Reset the test environment, if neccessary
     *
     * NOTE: "Not implemented" is always checked first but not
     *       being included as a test case.
     *       "Nominal" test case is always the last test case.
     *-----------------------------------------------------*/

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    /* TODO: Setup the test environment, if necessary */

    if (OS_xxx() == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_sample_test_exit_tag;
    }

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(NULL,...) == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(aVeryLoooooongName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(...) != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)

    /* TODO: Reset the test environment here, if necessary */

UT_os_sample_test_exit_tag:
    /* Call these macros at the very end of the function to close out the test variables
     * and get it added to the global list being tracked. */
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_xxx", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}
#endif

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_initfs(char *address, char *devname, char *volname, uint32 blocksize, uint32 numblocks)
** Purpose: Initializes (without re-formatting) a drive on the target without
**          erasing the existing file system
** Parameters: *address - a pointer to the address at which to start the new disk
**             *devname - a pointer to the name of the "generic" device
**             *volname - a pointer to the name of the volume (only used in vxWorks)
**             blocksize - size of a single block on the drive
**             numblocks - the number of blocks to be allocated for the drive
** Returns: OS_FS_ERR_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the device name or volume name passed in is too long
**          OS_FS_ERR_DEVICE_NOT_FREE if the Volume table is full
**          OS_FS_ERR_DRIVE_NOT_CREATED if the volume is not FS-BASED
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some device name or volume name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup test to cause the OS call to fail inside this routine
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_DRIVE_NOT_CREATED
** -----------------------------------------------------
** Test #4: Disk-full condition
**   1) Call this routine (NUM_TABLE_ENTRIES+1) of times
**   2) Expect the returned value to be (except the last call)
**        (a) OS_FS_SUCCESS
**   3) Expect the returned value of the last call to be
**        (a) OS_FS_ERR_DEVICE_NOT_FREE
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Make sure no file system has been created previously
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   4) Call OS_rmfs with device name used in #1 as argument
**   5) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_initfs_test()
{
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    int32 res=0, idx=0, i=0, j=0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_initfs(NULL, NULL, NULL, g_blkSize, g_blkCnt);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_initfs_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_initfs(g_fsAddrPtr, NULL, g_volNames[1], 0, 0) ==
         OS_FS_ERR_INVALID_POINTER) &&
        (OS_initfs(g_fsAddrPtr, g_devNames[1], NULL, 0, 0) ==
         OS_FS_ERR_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if ((OS_initfs(g_fsAddrPtr, g_fsLongName, g_volNames[2], g_blkSize, g_blkCnt) ==
         OS_FS_ERR_PATH_TOO_LONG) &&
        (OS_initfs(g_fsAddrPtr, g_devNames[2], g_fsLongName, g_blkSize, g_blkCnt) ==
         OS_FS_ERR_PATH_TOO_LONG))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#4 Disk-full";

    UT_os_print_volumetable("VOLUME TABLE _BEFORE_ DISK-FULL TEST SETUP FOR OS_initfs()");

    for (i=0; i <= NUM_TABLE_ENTRIES; i++)
    {
        memset(g_devNames[i], '\0', sizeof(g_devNames[i]));
        UT_os_sprintf(g_devNames[i], "/ramdev%d", (int)i);
        memset(g_volNames[i], '\0', sizeof(g_volNames[i]));
        UT_os_sprintf(g_volNames[i], "RAM%d", (int)i);
        res = OS_initfs(g_fsAddrPtr, g_devNames[i], g_volNames[i], g_blkSize, g_blkCnt);
        if (res != OS_FS_SUCCESS)
            break;
    }

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ DISK-FULL TEST SETUP FOR OS_initfs()");

    /* Only need to check the last call to OS_initfs() */
    if (res == OS_FS_ERR_DEVICE_NOT_FREE)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    for (j=0; j < i; j++)
        OS_rmfs(g_devNames[j]);

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ DISK-FULL TEST TEARDOWN FOR OS_initfs()");

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    UT_os_print_volumetable("VOLUME TABLE _BEFORE_ NOMINAL TEST SETUP FOR OS_initfs()");

    if (OS_initfs(g_fsAddrPtr, g_devNames[5], g_volNames[5], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        goto UT_os_initfs_test_exit_tag;
    }

    if (OS_rmfs(g_devNames[5]) == OS_FS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ NOMINAL TEST FOR OS_initfs()");

UT_os_initfs_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_initfs", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mkfs(char *address, char *devname, char *volname, uint32 blocksize, uint32 numblocks)
** Purpose: Formats a volume with a file system
** Parameters: *address - a pointer to the address at which to start the new disk
**             *devname - a pointer to the name of the "generic" device
**             *volname - a pointer to the name of the volume (only used in vxWorks)
**             blocksize - the size of a single block on the drive
**             numblocks - the number of blocks to be allocated for the drive
** Returns: OS_FS_ERR_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the name passed in is too long
**          OS_FS_ERR_DRIVE_NOT_CREATED if the OS call failed
**          OS_FS_ERR_DEVICE_NOT_FREE if the Volume table is full
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_mkfs() is implemented, then OS_rmfs() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some device name or volume name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup test to cause the OS call to fail inside this routine
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_DRIVE_NOT_CREATED
** -----------------------------------------------------
** Test #4: Disk-full condition
**   1) Call this routine (NUM_TABLE_ENTRIES+1) of times
**   2) Expect the returned value to be (except the last call)
**        (a) OS_FS_SUCCESS
**   3) Expect the returned value of the last call to be
**        (a) OS_FS_ERR_DEVICE_NOT_FREE
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Make sure no file system has been created previously
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   4) Call OS_rmfs with device name used in #1 as argument
**   5) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_makefs_test()
{
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    int32 res=0, idx=0, i=0, j=0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_mkfs(g_fsAddrPtr, NULL, NULL, g_blkSize, g_blkCnt);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_makefs_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_mkfs(g_fsAddrPtr, NULL, g_volNames[1], 0, 0) == OS_FS_ERR_INVALID_POINTER) &&
        (OS_mkfs(g_fsAddrPtr, g_devNames[1], NULL, 0, 0) == OS_FS_ERR_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if ((OS_mkfs(g_fsAddrPtr, g_fsLongName, g_volNames[2], g_blkSize, g_blkCnt) ==
         OS_FS_ERR_PATH_TOO_LONG) &&
        (OS_mkfs(g_fsAddrPtr, g_devNames[2], g_fsLongName, g_blkSize, g_blkCnt) ==
         OS_FS_ERR_PATH_TOO_LONG))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#4 Disk-full";
    UT_os_print_volumetable("VOLUME TABLE _BEFORE_ DISK-FULL TEST SETUP FOR OS_mkfs()");

    for (i=0; i <= NUM_TABLE_ENTRIES; i++)
    {
        memset(g_devNames[i], '\0', sizeof(g_devNames[i]));
        UT_os_sprintf(g_devNames[i], "/ramdev%d", (int)i);
        memset(g_volNames[i], '\0', sizeof(g_volNames[i]));
        UT_os_sprintf(g_volNames[i], "RAM%d", (int)i);
        res = OS_mkfs(g_fsAddrPtr, g_devNames[i], g_volNames[i], g_blkSize, g_blkCnt);
        if (res != OS_FS_SUCCESS)
            break;
    }

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ DISK-FULL TEST SETUP FOR OS_mkfs()");

    /* Only need to check the last call to OS_mkfs() */
    if (res == OS_FS_ERR_DEVICE_NOT_FREE)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    for (j=0; j < i; j++)
        OS_rmfs(g_devNames[j]);

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ DISK-FULL TEST TEARDOWN FOR OS_mkfs()");

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    UT_os_print_volumetable("VOLUME TABLE _BEFORE_ NOMINAL TEST SETUP FOR OS_mkfs()");

    if (OS_mkfs(g_fsAddrPtr, g_devNames[5], g_volNames[5], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        goto UT_os_makefs_test_exit_tag;
    }

    if (OS_rmfs(g_devNames[5]) == OS_FS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ NOMINAL TEST FOR OS_mkfs()");

UT_os_makefs_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_mkfs", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_rmfs(char *devname)
** Purpose: Removes or un-maps the target file system
** Parameters: *devname - a pointer to the name of the "generic" drive
** Returns: OS_FS_ERR_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERROR if the given device is not found in the Volume table
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_rmfs() is implemented, then OS_mkfs() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-device-arg condition
**   1) Make sure no file system has been created previously
**   2) Call this routine with some device name as argument
**   3) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_mkfs to create a file system
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call this routine with the device name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   5) Call OS_mkfs to create a file system again exactly as in #1
**   6) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_removefs_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_rmfs(NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_removefs_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_rmfs(NULL) == OS_FS_ERR_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-device-arg";

    if (OS_rmfs(g_devNames[2]) == OS_FS_ERROR)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    if (OS_mkfs(g_fsAddrPtr, g_devNames[3], g_volNames[3], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        testDesc = "#3 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_removefs_test_exit_tag;
    }

    if ((OS_rmfs(g_devNames[3]) == OS_FS_SUCCESS) &&
        (OS_mkfs(g_fsAddrPtr, g_devNames[3], g_volNames[3], g_blkSize, g_blkCnt) == OS_FS_SUCCESS))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_rmfs(g_devNames[3]);

UT_os_removefs_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_rmfs", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mount(const char *devname, char *mountpoint)
** Purpose: Mounts a disk volume to the file system tree
** Parameters: *devname - a pointer to the name of the drive to mount
**             *mountpoint - a pointer to the name to call this disk from now on
** Returns: OS_FS_ERR_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERROR if the given device is not found in the Volume table
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_mount() is implemented, then
**            OS_mkfs(), OS_rmfs and OS_unmount should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-device-arg condition
**   1) Make sure no file system has been created previously
**   2) Call this routine with some device name as argument
**   3) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_mkfs to create a file system
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call this routine with the device name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   5) Call this routine again exactly as in #3
**   6) Expect the returned value to be
**        (a) OS_FS_ERROR
**   7) Call OS_unmount with the mount-point used in #3 as argument
**   8) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_mount_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_mount(NULL, NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_mount_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_mount(NULL, g_mntNames[1]) == OS_FS_ERR_INVALID_POINTER) &&
        (OS_mount(g_devNames[1], NULL) == OS_FS_ERR_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-device-arg";

    if (OS_mount("ramdev0", g_mntNames[2]) == OS_FS_ERROR)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    UT_os_print_volumetable("VOLUME TABLE _BEFORE_ NOMINAL TEST SETUP FOR OS_mount()");

    if (OS_mkfs(g_fsAddrPtr, g_devNames[3], g_volNames[3], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        testDesc = "#3 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_mount_test_exit_tag;
    }

    if ((OS_mount(g_devNames[3], g_mntNames[3]) == OS_FS_SUCCESS) &&
        (OS_mount(g_devNames[3], g_mntNames[3]) == OS_FS_ERROR))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ NOMINAL TEST FOR OS_mount()");

    /* Reset test environment */
    OS_unmount(g_mntNames[3]);
    OS_rmfs(g_devNames[3]);

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ NOMINAL TEST TEARDOWN FOR OS_mount()");

UT_os_mount_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_mount", idx)
    UT_OS_LOG_API_MACRO(apiInfo)}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_unmount(const char *mountpoint)
** Purpose: Un-mounts a drive from the file system and makes all open file descriptors
**          obsolete
** Parameters: *mountpoint - a pointer to the name of the drive to unmount
** Returns: OS_FS_ERR_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the absolute path passed in is too long
**          OS_FS_ERROR if the mount-point passed in is not found in the Volume table
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_unmount() is implemented, then
**            OS_mkfs(), OS_rmfs() and OS_mount() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some mount-point name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-mount-point-arg condition
**   1) Make sure no file system has been created previously
**   2) Call this routine with some mount-point name as argument
**   3) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_mkfs to create a file system
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call OS_mount to mount the device to a mount-point
**   4) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   5) Call this routine with the mount-point used in #3 as argument
**   6) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   7) Call this routine again exactly as in #5
**   8) Expect the returned value to be
**        (a) OS_FS_ERROR
**--------------------------------------------------------------------------------*/
void UT_os_unmount_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_unmount(NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_unmount_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_unmount(NULL) == OS_FS_ERR_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_unmount(g_fsLongName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-mount-point-arg";

    if (OS_unmount(g_mntNames[3]) == OS_FS_ERROR)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    if (OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        testDesc = "#3 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_unmount_test_exit_tag;
    }

    if ((OS_mount(g_devNames[4], g_mntNames[4]) == OS_FS_SUCCESS) &&
        (OS_unmount(g_mntNames[4]) == OS_FS_SUCCESS) &&
        (OS_unmount(g_mntNames[4]) == OS_FS_ERROR))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_rmfs(g_devNames[4]);

UT_os_unmount_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_unmount", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_FS_GetPhysDriveName(char *PhysDriveName, char *MountPoint)
** Purpose: Returns the name of the physical drive of a given mount-point
** Parameters: *PhysDriveName - a pointer that will hold the name of the physical drive
**             *MountPoint - a pointer to the name of the mount-point
** Returns: OS_FS_ERR_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the mount-point passed in is too long
**          OS_FS_ERROR if the mount-point passed in is not found in the Volume table
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_unmount() is implemented, then
**            OS_mkfs(), OS_rmfs() and OS_mount() should also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some mount-point name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-mount-point-arg condition
**   1) Call OS_mkfs
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call this routine
**   4) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_mkfs
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call OS_mount with device name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   5) Call this routine with the device name used in #1 as argument
**   6) Expect the returned value to be
**        (a) OS_FS_SUCCESS
** --------------------------------------------------------------------------------*/
void UT_os_getphysdrivename_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    char physDevName[UT_OS_LG_TEXT_LEN];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_FS_GetPhysDriveName(NULL, NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_getphysicaldrivename_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_FS_GetPhysDriveName(NULL, g_mntNames[1]) == OS_FS_ERR_INVALID_POINTER) &&
        (OS_FS_GetPhysDriveName(physDevName, NULL) == OS_FS_ERR_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_FS_GetPhysDriveName(physDevName, g_fsLongName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-mount-point-arg";

    if (OS_FS_GetPhysDriveName(physDevName, g_mntNames[3]) == OS_FS_ERROR)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    if (OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_getphysicaldrivename_test_exit_tag;
    }

    if (OS_mount(g_devNames[4], g_mntNames[4]) != OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-mount failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_getphysicaldrivename_test_exit_tag;
    }

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ NOMINAL TEST SETUP for OS_FS_GetPhysDriveName()");

    memset(physDevName, '\0', sizeof(physDevName));
    if ((OS_FS_GetPhysDriveName(physDevName, g_mntNames[4]) == OS_FS_SUCCESS) &&
        (strncmp(physDevName, g_physDriveName, strlen(g_physDriveName)) == 0))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_unmount(g_mntNames[4]);
    OS_rmfs(g_devNames[4]);

UT_os_getphysicaldrivename_test_exit_tag:
        UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_FS_GetPhysDriveName", idx)
        UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
** Purpose: Translates the given virtual path to the real path
** Parameters: *VirtualPath - pointer to the name of the virtual path or mount point
**             *LocalPath - pointer that will hold the name of the local path or
**                          physical device name
** Returns: OS_FS_ERR_INVALID_POINTER if any of the pointers passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the device name or volume name passed in is too long
**          OS_FS_ERR_PATH_INVALID if the virtual path passed in is not in correct format, or
**                                 virtual path name not found in the Volume table
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with some device name or volume name of length greater than
**      Volume table's name
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-virtual-path-arg condition
**   1) Call this routine with a incorrectly formatted virtual path name as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Make sure no file system has been created previously
**   2) Call OS_mkfs
**   3) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   4) Call OS_mount with device name used in #2 as argument
**   5) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   6) Call this routine with the mount-point used in #4 as argument
**   7) Expect the returned value to be
**        (a) OS_FS_SUCCESS __and__
**        (b) the returned local path to be ?
** --------------------------------------------------------------------------------*/
void UT_os_translatepath_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    char localPath[UT_OS_LG_TEXT_LEN];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TranslatePath(NULL, NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_translatepath_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_TranslatePath(NULL, localPath) == OS_FS_ERR_INVALID_POINTER) &&
        (OS_TranslatePath(g_mntNames[1], NULL) == OS_FS_ERR_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_TranslatePath(g_fsLongName, localPath) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-virtual-path-arg";

    if ((OS_TranslatePath("cf", localPath) == OS_FS_ERR_PATH_INVALID) &&
        (OS_TranslatePath("/foobar", localPath) == OS_FS_ERR_PATH_INVALID))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    if (OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt) !=
        OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_translatepath_test_exit_tag;
    }

    if (OS_mount(g_devNames[4], g_mntNames[4]) != OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-mount failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        /* Reset test environment */
        OS_rmfs(g_devNames[4]);

        goto UT_os_translatepath_test_exit_tag;
    }

    if ((OS_TranslatePath(g_mntNames[4], localPath) == OS_FS_SUCCESS) &&
        (strncmp(localPath, g_physDriveName, strlen(g_physDriveName)) == 0))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_unmount(g_mntNames[4]);
    OS_rmfs(g_devNames[4]);

UT_os_translatepath_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TranslatePath (internal)", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: os_fshealth_t OS_chkfs(const char *name, boolean repair)
** Purpose: Checks the integrity of the file system, and may or may not repair it,
**          depending on repair
** Parameters: *name - the name of the drive to check
**             repair - boolean flag to repair or not to repair
** Returns: OS_FS_ERR_INVALID_POINTER if the pointer passed in is NULL
**          OS_FS_ERROR if the OS call failed
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup the test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test#3: Nominal condition
**   1) TBD - posix implementation is not yet implemented
** --------------------------------------------------------------------------------*/
void UT_os_checkfs_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_chkfs(NULL, 0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_checkfs_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_chkfs(NULL, 0) == OS_FS_ERR_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    if (g_skipTestCase == 3)
    {
        testDesc = "#3 Nominal - Not implemented in API";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, g_skipTestCaseResult)
    }
    else
    {
        /* TBD */
    }

UT_os_checkfs_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_chkfs", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_fsBlocksFree(const char *name)
** Purpose: Returns the number of blocks free in a the file system
** Parameters: *name - a pointer to the name of the drive to check for free blocks
** Returns: OS_FS_ERR_INVALID_POINTER if the pointer passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if the path passed in is too long
**          OS_FS_ERROR if the OS call failed
**          Number of blocks free in a volume if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with a path name of length greater than Volume table's
**      name as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test#4: Nominal condition
**   1) Make sure no file system has been previously created
**   2) Call OS_mkfs
**   3) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   4) Call OS_mount with device name used in #2
**   5) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   6) Call this routine with mount-point used in #4
**   7) Expect the returned value to be
**        (a) greater than or equal to 0
** --------------------------------------------------------------------------------*/
void UT_os_fsblocksfree_test()
{
    int32 idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_fsBlocksFree(NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_fsblocksfree_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_fsBlocksFree(NULL) == OS_FS_ERR_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_fsBlocksFree(g_fsLongName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    if (OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_fsblocksfree_test_exit_tag;
    }

    if (OS_mount(g_devNames[4], g_mntNames[4]) != OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-mount failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_fsblocksfree_test_exit_tag;
    }

    if (OS_fsBlocksFree(g_mntNames[4]) >= 0)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_unmount(g_mntNames[4]);
    OS_rmfs(g_devNames[4]);

UT_os_fsblocksfree_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_fsBlocksFree", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_fsBytesFree(const char *name, uint64 *bytes_free)
** Purpose: Returns the number of bytes free in a the file system
** Parameters: *name - a pointer to the name of the drive to check for free bytes
**             *bytes_free - a pointer that will hold the number of bytes free
** Returns: OS_FS_ERR_INVALID_POINTER if the pointer passed in is NULL
**          OS_FS_ERROR if the OS call failed
**          OS_FS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as one of the arguments
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with a path name of length greater than Volume table's
**      name as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test#4: Nominal condition
**   1) Make sure no file system has been previously created
**   2) Call OS_mkfs
**   3) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   4) Call OS_mount with device name used in #2
**   5) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   6) Call this routine with mount-point used in #4
**   7) Expect the returned value to be
**        (a) greater than or equal to 0
** --------------------------------------------------------------------------------*/
void UT_os_fsbytesfree_test()
{
    int32 idx=0;
    uint64 retBytes=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_fsBytesFree(NULL, NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_fsbytesfree_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_fsBytesFree(NULL, &retBytes) == OS_FS_ERR_INVALID_POINTER) &&
        (OS_fsBytesFree(g_mntNames[1], NULL) == OS_FS_ERR_INVALID_POINTER))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_fsBytesFree(g_fsLongName, &retBytes) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    if (OS_mkfs(g_fsAddrPtr, g_devNames[4], g_volNames[4], g_blkSize, g_blkCnt) != OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_fsbytesfree_test_exit_tag;
    }

    if (OS_mount(g_devNames[4], g_mntNames[4]) != OS_FS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-system-mount failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_fsbytesfree_test_exit_tag;
    }

    if (OS_fsBytesFree(g_mntNames[4], &retBytes) == OS_FS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_unmount(g_mntNames[4]);
    OS_rmfs(g_devNames[4]);

UT_os_fsbytesfree_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_fsBytesFree", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_osfilesys_diskio_test.c
**================================================================================*/
