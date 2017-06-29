/*
 * Filename: osfilesys_testcase.c
 *
 * Purpose: This file contains unit test cases
 *
 * Modification History:
 *   07/14/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include <fcntl.h>  /* for O_WRONLY etc. */

#include "osapi.h"              /* cfe.h not available from within osal. */

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "osfilesys_stubs.h"



/* Prototypes for non-exported functions */
int32 OS_FS_GetErrorName(int32 error_num, os_fs_err_name_t * err_name);
int32 OS_GetPhysDeviceName(char *PhysDevName, char *LocalVolname);


/* -------------------- Special Test Case Variables ------------------------- */
static char testName[] = "/testDev/testName";
static char testMaxVolName[OS_FS_VOL_NAME_LEN];
static char testMaxPhysName[OS_FS_PHYS_NAME_LEN];
static char testMaxPathName[OS_MAX_LOCAL_PATH_LEN];
static char *charPtr;


/* Utility functions */
/* -------------------------------------------------------------------------- */
int open_hook_RtnError (const char *filename, int flags, int mode)
{
    return VCS_ERROR;
}


/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_mkfs Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_NullDevName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_mkfs(charPtr, NULL, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_NullVolName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, NULL, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_DevNameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_FS_DEV_NAME_LEN, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_VolNameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_FS_DEV_NAME_LEN, 2);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_NoFreeDev(void)
{
    int32 expected = OS_FS_ERR_DEVICE_NOT_FREE;
    int32 actual   = 99;

    /* Setup Inputs */
    OS_VolumeTable[0].FreeFlag  = TRUE;  /* for full branch coverage */
    OS_VolumeTable[0].IsMounted = FALSE;
    OS_VolumeTable[1].FreeFlag  = TRUE;
    OS_VolumeTable[0].IsMounted = TRUE;

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DEVICE_NOT_FREE");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_Error(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = EEPROM_DISK;

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_RamDevCreateNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 1;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = RAM_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_RAMDEVCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_RamBlkCreateSyncNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 2;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = RAM_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_XBDBLKDEVCREATESYNC_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_RamVolFmtError(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 3;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = RAM_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_DOSFSVOLFORMAT_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_RamDiskSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;
    static char phyDevName[OS_FS_PHYS_NAME_LEN];

    /* Setup Inputs */
    uint32 idx = 0;
    uint32 blocksize = 30;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = RAM_DISK;

    strcpy(phyDevName, testMaxVolName);
    strcat(phyDevName, ":0");

    /* Execute Test */
    actual = OS_mkfs(testName, testName, testMaxVolName, blocksize, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == FALSE, "FreeFlag == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].DeviceName, testName, "DeviceName == expected");
    UtAssert_True(OS_VolumeTable[idx].BlockSize == blocksize, "blocksize == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].PhysDevName, phyDevName, "PhysDevName == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_FsBasedSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 0;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = FS_BASED;

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

#ifdef USE_VXWORKS_ATA_DRIVER
/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_AtaDevCreateNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 1;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_ATADEVCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_AtaBlkCreateSyncNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 2;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_XBDBLKDEVCREATESYNC_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_AtaPhysDevNameError(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 3;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    /* open used in OS_GetPhysDeviceName */
    OsFileSys_SetFunctionHook(OSFILESYS_OPEN_INDEX, open_hook_RtnError);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_AtaVolFmtError(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 4;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    /* open used in OS_GetPhysDeviceName */
    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_DOSFSVOLFORMAT_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mkfs_AtaSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;
    static char phyDevName[OS_FS_PHYS_NAME_LEN];

    /* Setup Inputs */
    uint32 idx = 5;
    uint32 blocksize = 30;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    strcpy(phyDevName, testName);
    strcat(phyDevName, ":0");

    /* open used in OS_GetPhysDeviceName */
    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_DOSFSVOLFORMAT_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_mkfs(charPtr, testName, charPtr, blocksize, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == FALSE, "FreeFlag == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].DeviceName, testName, "DeviceName == expected");
    UtAssert_True(OS_VolumeTable[idx].BlockSize == blocksize, "blocksize == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].PhysDevName, phyDevName, "PhysDevName == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

#endif

/*******************************************************************************
**
**  OS_rmfs Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_rmfs_NullDevName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_rmfs(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmfs_DevNameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_FS_DEV_NAME_LEN, 1);

    /* Execute Test */
    actual = OS_rmfs(charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmfs_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_rmfs(charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_rmfs_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);

    OS_VolumeTable[0].FreeFlag  = FALSE; /* for full test branch coverage */
    OS_VolumeTable[0].IsMounted = FALSE;
    OS_VolumeTable[1].FreeFlag  = FALSE;
    OS_VolumeTable[1].IsMounted = TRUE;
    strcpy(OS_VolumeTable[1].DeviceName, testName);
    OS_VolumeTable[2].FreeFlag  = TRUE;
    OS_VolumeTable[2].IsMounted = FALSE;
    strcpy(OS_VolumeTable[2].DeviceName, testName);


    /* Execute Test */
    actual = OS_rmfs(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_initfs Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_initfs_NullDevName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_initfs(charPtr, NULL, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_NullVolName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, NULL, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_DevNameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_FS_DEV_NAME_LEN, 1);

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_VolNameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_FS_DEV_NAME_LEN, 2);

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_NoFreeDev(void)
{
    int32 expected = OS_FS_ERR_DEVICE_NOT_FREE;
    int32 actual   = 99;

    /* Setup Inputs */
    OS_VolumeTable[0].FreeFlag  = TRUE;  /* for full branch coverage */
    OS_VolumeTable[0].IsMounted = FALSE;
    OS_VolumeTable[1].FreeFlag  = TRUE;
    OS_VolumeTable[0].IsMounted = TRUE;

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DEVICE_NOT_FREE");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_Error(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = EEPROM_DISK;

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_RamDevCreateNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 1;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = RAM_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_RAMDEVCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_RamBlkCreateSyncNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 2;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = RAM_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_XBDBLKDEVCREATESYNC_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_RamDiskSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;
    static char phyDevName[OS_FS_PHYS_NAME_LEN];

    /* Setup Inputs */
    uint32 idx = 0;
    uint32 blocksize = 30;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = RAM_DISK;

    strcpy(phyDevName, testMaxVolName);
    strcat(phyDevName, ":0");

    /* Execute Test */
    actual = OS_initfs(testName, testName, testMaxVolName, blocksize, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == FALSE, "FreeFlag == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].DeviceName, testName, "DeviceName == expected");
    UtAssert_True(OS_VolumeTable[idx].BlockSize == blocksize, "blocksize == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].PhysDevName, phyDevName, "PhysDevName == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_FsBasedSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 0;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = FS_BASED;

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

#ifdef USE_VXWORKS_ATA_DRIVER
/*----------------------------------------------------------------------------*/
void Test_OS_initfs_AtaDevCreateNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 1;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_ATADEVCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_AtaBlkCreateSyncNull(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 2;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    OsFileSys_SetReturnCode(OSFILESYS_XBDBLKDEVCREATESYNC_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_AtaPhysDevNameError(void)
{
    int32 expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 3;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    /* open used in OS_GetPhysDeviceName */
    OsFileSys_SetFunctionHook(OSFILESYS_OPEN_INDEX, open_hook_RtnError);

    /* Execute Test */
    actual = OS_initfs(charPtr, charPtr, charPtr, 1, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_DRIVE_NOT_CREATED");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == TRUE, "FreeFlag == TRUE on VCS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_initfs_AtaSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;
    static char phyDevName[OS_FS_PHYS_NAME_LEN];

    /* Setup Inputs */
    uint32 idx = 5;
    uint32 blocksize = 30;
    OS_VolumeTable[idx].FreeFlag  = TRUE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, testName);
    OS_VolumeTable[idx].VolumeType = ATA_DISK;

    strcpy(phyDevName, testName);
    strcat(phyDevName, ":0");

    /* open used in OS_GetPhysDeviceName */
    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_DOSFSVOLFORMAT_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_initfs(charPtr, testName, charPtr, blocksize, 1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(OS_VolumeTable[idx].FreeFlag == FALSE, "FreeFlag == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].DeviceName, testName, "DeviceName == expected");
    UtAssert_True(OS_VolumeTable[idx].BlockSize == blocksize, "blocksize == expected");
    UtAssert_StrCmp(OS_VolumeTable[idx].PhysDevName, phyDevName, "PhysDevName == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

#endif


/*******************************************************************************
**
**  OS_mount Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_mount_NullDevName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_mount(NULL, charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mount_NullMount(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_mount(charPtr, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mount_DevNameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_FS_DEV_NAME_LEN, 1);

    /* Execute Test */
    actual = OS_mount(charPtr, charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mount_MountTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_MAX_PATH_LEN, 2);

    /* Execute Test */
    actual = OS_mount(charPtr, charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mount_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_mount(charPtr, charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_mount_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 4;
    OS_VolumeTable[idx].FreeFlag = FALSE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].DeviceName, charPtr);

    /* for full branch coverage */
    OS_VolumeTable[0].FreeFlag  = FALSE;
    OS_VolumeTable[0].IsMounted = FALSE;
    OS_VolumeTable[1].FreeFlag  = FALSE;
    OS_VolumeTable[1].IsMounted = TRUE;
    OS_VolumeTable[2].FreeFlag  = TRUE;

    /* Execute Test */
    actual = OS_mount(charPtr, charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_StrCmp(OS_VolumeTable[idx].MountPoint, charPtr, "MountPoint == expected");
    UtAssert_True(OS_VolumeTable[idx].IsMounted == TRUE, "IsMounted == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_unmount Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_unmount_NullMount(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_unmount(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_unmount_MountTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_unmount(charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_unmount_MountNotFound(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_unmount(charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_unmount_OpenError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_unmount(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_unmount_IoctlError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_unmount(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_unmount_BadMountpoint(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;

    /* OS_TranslatePath() assumes the mountpoint starts with "/", so
     * let's clobber that and make it fail in OS_unmount() */
    const char tempName[] = "noSlashInPathName";

    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    /* for full branch coverage */
    OS_VolumeTable[0].FreeFlag  = FALSE;
    OS_VolumeTable[0].IsMounted = TRUE;
    OS_VolumeTable[1].FreeFlag  = FALSE;
    OS_VolumeTable[1].IsMounted = FALSE;
    OS_VolumeTable[3].FreeFlag  = TRUE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, VCS_ERROR, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_unmount(tempName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_unmount_AlreadyUnmounted(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = FALSE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    /* for full branch coverage */
    OS_VolumeTable[0].FreeFlag  = FALSE;
    OS_VolumeTable[0].IsMounted = TRUE;
    OS_VolumeTable[1].FreeFlag  = FALSE;
    OS_VolumeTable[1].IsMounted = FALSE;
    OS_VolumeTable[3].FreeFlag  = TRUE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, VCS_ERROR, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_unmount(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_unmount_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    /* for full branch coverage */
    OS_VolumeTable[0].FreeFlag  = FALSE;
    OS_VolumeTable[0].IsMounted = TRUE;
    OS_VolumeTable[1].FreeFlag  = FALSE;
    OS_VolumeTable[1].IsMounted = FALSE;
    OS_VolumeTable[3].FreeFlag  = TRUE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, VCS_ERROR, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_unmount(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_fsBlocksFree Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_fsBlocksFree_NullName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_fsBlocksFree(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBlocksFree_NameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_fsBlocksFree(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBlocksFree_NameNotFound(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_fsBlocksFree(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBlocksFree_NameError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    const char tempName[] = "noSlashInPathName";
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, tempName);

    /* Execute Test */
    actual = OS_fsBlocksFree(tempName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBlocksFree_StatfsError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 7;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    OsFileSys_SetReturnCode(OSFILESYS_STATFS_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_fsBlocksFree(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBlocksFree_StatfsSuccess(void)
{
    int32 expected = 50; /* in stub, f_bfree set to 50, f_bsize set to 4 */
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 9;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    /* for full branch coverage */
    OS_VolumeTable[0].FreeFlag  = FALSE;
    OS_VolumeTable[0].IsMounted = TRUE;
    OS_VolumeTable[1].FreeFlag  = FALSE;
    OS_VolumeTable[1].IsMounted = FALSE;
    OS_VolumeTable[1].FreeFlag  = TRUE;

    OsFileSys_SetReturnCode(OSFILESYS_STATFS_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_fsBlocksFree(testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_fsBytesFree Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_fsBytesFree_NullName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint64 bytes_free = 88;

    /* Execute Test */
    actual = OS_fsBytesFree(NULL, &bytes_free);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBytesFree_NullBytePtr(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_fsBytesFree(testName, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBytesFree_NameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    uint64 bytes_free = 88;

    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_fsBytesFree(testName, &bytes_free);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBytesFree_NameNotFound(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint64 bytes_free = 88;

    /* Execute Test */
    actual = OS_fsBytesFree(testName, &bytes_free);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBytesFree_NameError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint64 bytes_free = 88;
    const char tempName[] = "noSlashInPathName";
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, tempName);

    /* Execute Test */
    actual = OS_fsBytesFree(tempName, &bytes_free);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBytesFree_StatfsError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint64 bytes_free = 88;
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    OsFileSys_SetReturnCode(OSFILESYS_STATFS_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_fsBytesFree(testName, &bytes_free);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_fsBytesFree_StatfsSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint64 bytes_free = 88;
    uint32 idx = 5;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);

    /* for full branch coverage */
    OS_VolumeTable[0].FreeFlag  = FALSE;
    OS_VolumeTable[0].IsMounted = TRUE;
    OS_VolumeTable[1].FreeFlag  = FALSE;
    OS_VolumeTable[1].IsMounted = FALSE;
    OS_VolumeTable[2].FreeFlag  = TRUE;

    OsFileSys_SetReturnCode(OSFILESYS_STATFS_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_fsBytesFree(testName, &bytes_free);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_True(bytes_free == 50*4, "bytes_free == expected"); /* stub values 50 and 4 */
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_chkfs Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_NullName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    boolean repair = FALSE;

    /* Execute Test */
    actual = OS_chkfs(NULL, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_NameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    boolean repair = FALSE;

    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_chkfs(testName, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_OpenError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);
    boolean repair = FALSE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_chkfs(testName, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_RepairError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);
    boolean repair = TRUE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_chkfs(testName, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNOpenCalls() == getNCloseCalls(), "Calls to open == Calls to close");
}

/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_RepairSuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);
    boolean repair = TRUE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_chkfs(testName, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNOpenCalls() == getNCloseCalls(), "Calls to open == Calls to close");
}

/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_CheckOnlyError(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);
    boolean repair = FALSE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_chkfs(testName, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNOpenCalls() == getNCloseCalls(), "Calls to open == Calls to close");
}

/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_CheckOnlyBadName(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    boolean repair = FALSE;

    /* OS_TranslatePath() assumes the mountpoint starts with "/", so
     * let's clobber that and make it fail in OS_unmount() */
    const char tempName[] = "noSlashInPathName";

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_chkfs(tempName, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNOpenCalls() == getNCloseCalls(), "Calls to open == Calls to close");
}

/*----------------------------------------------------------------------------*/
void Test_OS_chkfs_CheckOnlySuccess(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag  = FALSE;
    OS_VolumeTable[idx].IsMounted = TRUE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);
    boolean repair = FALSE;

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_IOCTL_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_chkfs(testName, repair);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(getNOpenCalls() == getNCloseCalls(), "Calls to open == Calls to close");
}


/*******************************************************************************
**
**  OS_FS_GetPhysDriveName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetPhysDriveName_NullName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_FS_GetPhysDriveName(NULL, testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetPhysDriveName_NullMount(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_FS_GetPhysDriveName(testName, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetPhysDriveName_NameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_FS_DEV_NAME_LEN, 1);

    /* Execute Test */
    actual = OS_FS_GetPhysDriveName(testName, testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetPhysDriveName_MountTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_MAX_PATH_LEN, 2);

    /* Execute Test */
    actual = OS_FS_GetPhysDriveName(testName, testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetPhysDriveName_MountNotFound(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_FS_GetPhysDriveName(testName, testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetPhysDriveName_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    char physDevName[] = "testPhysDevName";
    strcpy(OS_VolumeTable[idx].PhysDevName, physDevName);
    OS_VolumeTable[idx].FreeFlag = FALSE;
    strcpy(OS_VolumeTable[idx].MountPoint, testName);
    memset(testMaxPhysName, 0, sizeof(testMaxPhysName));

    /* for full branch coverage */
    OS_VolumeTable[0].FreeFlag = FALSE;
    OS_VolumeTable[1].FreeFlag = TRUE;

    /* Execute Test */   /* Note: 1st arg must point to a large enough char array */
    actual = OS_FS_GetPhysDriveName(testMaxPhysName, testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(testMaxPhysName, OS_VolumeTable[idx].PhysDevName, "PhysDriveName == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TranslatePath Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TranslatePath_NullVirtualName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_TranslatePath(NULL, testMaxPathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TranslatePath_NullLocalName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_TranslatePath(testMaxPathName, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TranslatePath_VirtualNameTooLong(void)
{
    int32 expected = OS_FS_ERR_PATH_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetReturnCode(OSFILESYS_STRLEN_INDEX, OS_MAX_PATH_LEN, 1);

    /* Execute Test */
    actual = OS_TranslatePath(testName, testMaxPathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TranslatePath_MissingFirstDelim(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    static char missingStartChar[] = "missingStartChar";

    /* Execute Test */
    actual = OS_TranslatePath(missingStartChar, testMaxPathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TranslatePath_InvalidPath(void)
{
    int32 expected = OS_FS_ERR_PATH_INVALID;
    int32 actual   = 99;

    /* Setup Inputs */
    static char noName[OS_MAX_PATH_LEN];
    noName[0] = '/';

    /* Execute Test */
    actual = OS_TranslatePath(noName, testMaxPathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_PATH_INVALID");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TranslatePath_MaxPath(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 0;
    char mountPoint[] = "/";
    char devName[OS_MAX_PATH_LEN];
    char validName[OS_MAX_PATH_LEN];

    memset(devName, 'a', OS_MAX_PATH_LEN);
    devName[OS_MAX_PATH_LEN - 1] = '\0';
    memset(validName, 'b', OS_MAX_PATH_LEN);
    validName[OS_MAX_PATH_LEN - 1] = '\0';
    strcpy(validName, mountPoint);
    validName[sizeof(mountPoint) - 1] = '/'; /* filename will now be /bbb... */

    OS_VolumeTable[idx].FreeFlag = FALSE;
    strcpy(OS_VolumeTable[idx].MountPoint, mountPoint);
    strcpy(OS_VolumeTable[idx].PhysDevName, devName);

    /* Execute Test */
    actual = OS_TranslatePath(validName, testMaxPathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(strlen(testMaxPathName) <= OS_MAX_LOCAL_PATH_LEN, "path length <= max");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*----------------------------------------------------------------------------*/
void Test_OS_TranslatePath_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    char validName[] = "/mountPoint/arbitrary/File/Name/";
    uint32 idx = NUM_TABLE_ENTRIES - 1;
    OS_VolumeTable[idx].FreeFlag = FALSE;
    strcpy(OS_VolumeTable[idx].MountPoint, "/mountPoint");
    strcpy(OS_VolumeTable[idx].PhysDevName, "physDevName");

    /* Execute Test */
    actual = OS_TranslatePath(validName, testMaxPathName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_StrCmp("physDevName/arbitrary/File/Name/", testMaxPathName,
                    "translated path == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_FS_GetErrorName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_NullName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_SUCCESS;

    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_SUCCESS;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_SUCCESS", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_Error(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERROR;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERROR", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_InvalidPtr(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_INVALID_POINTER;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_INVALID_POINTER", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_PathTooLong(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_PATH_TOO_LONG;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_PATH_TOO_LONG", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_NameTooLong(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_NAME_TOO_LONG;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_NAME_TOO_LONG", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_Unimplemented(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_UNIMPLEMENTED;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_UNIMPLEMENTED", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_DriveNotCreated(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_DRIVE_NOT_CREATED;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_DRIVE_NOT_CREATED", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_DeviceNotFree(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_DEVICE_NOT_FREE;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_DEVICE_NOT_FREE", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_PathInvalid(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_PATH_INVALID;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_PATH_INVALID", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_NoFreeFds(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_NO_FREE_FDS;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_NO_FREE_FDS", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_InvalidFd(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = OS_FS_ERR_INVALID_FD;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "OS_FS_ERR_INVALID_FD", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FS_GetErrorName_Unknown(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 error_num = 111;
    static os_fs_err_name_t err_name;


    /* Execute Test */
    actual = OS_FS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_StrCmp(err_name, "ERROR_UNKNOWN", "err_name == expected");
}


/*******************************************************************************
**
**  OS_GetPhysDeviceName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_GetPhysDeviceName_NullDevName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_GetPhysDeviceName(NULL, testName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetPhysDeviceName_NullVolName(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_GetPhysDeviceName(testName, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetPhysDeviceName_VolNameTooLong(void)
{
    int32 expected = OS_FS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    static char maxVolName[OS_FS_VOL_NAME_LEN + 1];
    memset(maxVolName, 'a', OS_FS_VOL_NAME_LEN);

    /* Execute Test */
    actual = OS_GetPhysDeviceName(testName, maxVolName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_NAME_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetPhysDeviceName_MaxVolName(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    static char expectedName[OS_FS_PHYS_NAME_LEN];
    static char maxVolName[OS_FS_VOL_NAME_LEN];
    memset(maxVolName, 'a', sizeof(maxVolName) - 1);
    strncpy(expectedName, maxVolName, sizeof(maxVolName) - 1);
    strcat(expectedName, ":0");                          /* add device number */
    memset(testMaxPhysName, 0, sizeof(testMaxPhysName));
    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);

    /* Execute Test */
    actual = OS_GetPhysDeviceName(testMaxPhysName, maxVolName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_StrCmp(testMaxPhysName, expectedName, "PhysDevName == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetPhysDeviceName_Error(void)
{
    int32 expected = OS_FS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsFileSys_SetFunctionHook(OSFILESYS_OPEN_INDEX, open_hook_RtnError);

    /* Execute Test */
    actual = OS_GetPhysDeviceName(testMaxPhysName, charPtr);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetPhysDeviceName_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    char expectedName[OS_FS_PHYS_NAME_LEN];
    strcpy(testMaxVolName, "/successTestName");
    memset(testMaxPhysName, 0, sizeof(testMaxPhysName));
    strcpy(expectedName, testMaxVolName);
    strcat(expectedName, ":0");

    OsFileSys_SetReturnCode(OSFILESYS_OPEN_INDEX, 1, 1);
    OsFileSys_SetReturnCode(OSFILESYS_CLOSE_INDEX, 1, 1);

    /* Execute Test */
    actual = OS_GetPhysDeviceName(testMaxPhysName, testMaxVolName);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_StrCmp(testMaxPhysName, expectedName, "PhysDevName == expected");
}


/*******************************************************************************
**
**  OS_GetFsInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_GetFsInfo_NullPtr(void)
{
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_GetFsInfo(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_ERR_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetFsInfo_Success(void)
{
    int32 expected = OS_FS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    static os_fsinfo_t filesys_info;
    OS_FDTable[0].IsValid = TRUE; /* verify the full range is checked */
    OS_VolumeTable[NUM_TABLE_ENTRIES - 1].FreeFlag = TRUE;

    /* Execute Test */
    actual = OS_GetFsInfo(&filesys_info);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_FS_SUCCESS");
    UtAssert_True(filesys_info.MaxFds == OS_MAX_NUM_OPEN_FILES, "MaxFds == expected");
    UtAssert_True(filesys_info.MaxVolumes == NUM_TABLE_ENTRIES, "MaxVolumes == expected");
    UtAssert_True(filesys_info.FreeFds == OS_MAX_NUM_OPEN_FILES - 1, "FreeFds == expected");
    UtAssert_True(filesys_info.FreeVolumes == 1, "FreeVolumes == expected");
    UtAssert_True(getNSemTake() > 0, "semTake called");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
}


/* ------------------- End of test cases --------------------------------------*/





/*
 * OsFileSys_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void OsFileSys_Setup(void)
{

    OsFileSys_Reset();
    memset(OS_VolumeTable, 0, sizeof(OS_VolumeTable));
    memset(testMaxVolName, 'a', sizeof(testMaxVolName));
    testMaxVolName[OS_FS_VOL_NAME_LEN - 1] = '\0';

    charPtr = testName;
}


/*
 * OsFileSys_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void OsFileSys_TearDown(void)
{

}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* OsFileSys_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    /* OS_mkfs Tests */
    ADD_TEST(Test_OS_mkfs_NullDevName,         OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_NullVolName,         OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_DevNameTooLong,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_VolNameTooLong,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_NoFreeDev,           OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_Error,               OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_RamDevCreateNull,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_RamBlkCreateSyncNull,OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_RamVolFmtError,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_RamDiskSuccess,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_FsBasedSuccess,      OsFileSys_Setup, OsFileSys_TearDown);
#ifdef USE_VXWORKS_ATA_DRIVER
    ADD_TEST(Test_OS_mkfs_AtaDevCreateNull,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_AtaBlkCreateSyncNull,OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_AtaPhysDevNameError, OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_AtaVolFmtError,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mkfs_AtaSuccess,          OsFileSys_Setup, OsFileSys_TearDown);
#endif

    /* OS_rmfs Tests */
    ADD_TEST(Test_OS_rmfs_NullDevName,         OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_rmfs_DevNameTooLong,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_rmfs_Error,               OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_rmfs_Success,             OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_initfs Tests */
    ADD_TEST(Test_OS_initfs_NullDevName,         OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_NullVolName,         OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_DevNameTooLong,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_VolNameTooLong,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_NoFreeDev,           OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_Error,               OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_RamDevCreateNull,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_RamBlkCreateSyncNull,OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_RamDiskSuccess,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_FsBasedSuccess,      OsFileSys_Setup, OsFileSys_TearDown);
#ifdef USE_VXWORKS_ATA_DRIVER
    ADD_TEST(Test_OS_initfs_AtaDevCreateNull,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_AtaBlkCreateSyncNull,OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_AtaPhysDevNameError, OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_initfs_AtaSuccess,          OsFileSys_Setup, OsFileSys_TearDown);
#endif

    /* OS_mount Tests */
    ADD_TEST(Test_OS_mount_NullDevName,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mount_NullMount,          OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mount_DevNameTooLong,     OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mount_MountTooLong,       OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mount_Error,              OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_mount_Success,            OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_unmount Tests */
    ADD_TEST(Test_OS_unmount_NullMount,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_unmount_MountTooLong,     OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_unmount_MountNotFound,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_unmount_OpenError,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_unmount_IoctlError,       OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_unmount_BadMountpoint,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_unmount_AlreadyUnmounted, OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_unmount_Success,          OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_fsBlocksFree Tests */
    ADD_TEST(Test_OS_fsBlocksFree_NullName,     OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBlocksFree_NameTooLong,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBlocksFree_NameNotFound, OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBlocksFree_NameError,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBlocksFree_StatfsError,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBlocksFree_StatfsSuccess,OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_fsBytesFree Tests */
    ADD_TEST(Test_OS_fsBytesFree_NullName,     OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBytesFree_NullBytePtr,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBytesFree_NameTooLong,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBytesFree_NameNotFound, OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBytesFree_NameError,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBytesFree_StatfsError,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_fsBytesFree_StatfsSuccess,OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_chkfs Tests */
    ADD_TEST(Test_OS_chkfs_NullName,           OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_chkfs_NameTooLong,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_chkfs_OpenError,          OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_chkfs_RepairError,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_chkfs_RepairSuccess,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_chkfs_CheckOnlyError,     OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_chkfs_CheckOnlyBadName,   OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_chkfs_CheckOnlySuccess,   OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_FS_GetPhysDriveName Tests */
    ADD_TEST(Test_OS_FS_GetPhysDriveName_NullName,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetPhysDriveName_NullMount,     OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetPhysDriveName_NameTooLong,   OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetPhysDriveName_MountTooLong,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetPhysDriveName_MountNotFound, OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetPhysDriveName_Success,       OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_TranslatePath Tests */
    ADD_TEST(Test_OS_TranslatePath_NullVirtualName,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_TranslatePath_NullLocalName,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_TranslatePath_VirtualNameTooLong, OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_TranslatePath_MissingFirstDelim,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_TranslatePath_InvalidPath,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_TranslatePath_MaxPath,            OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_TranslatePath_Success,            OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_FS_GetErrorName Tests */
    ADD_TEST(Test_OS_FS_GetErrorName_NullName,       OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_Success,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_Error,          OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_InvalidPtr,     OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_PathTooLong,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_NameTooLong,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_Unimplemented,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_DriveNotCreated,OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_DeviceNotFree,  OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_PathInvalid,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_NoFreeFds,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_InvalidFd,      OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_FS_GetErrorName_Unknown,        OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_GetPhysDeviceName Tests */
    ADD_TEST(Test_OS_GetPhysDeviceName_NullDevName,   OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_GetPhysDeviceName_NullVolName,   OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_GetPhysDeviceName_VolNameTooLong,OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_GetPhysDeviceName_MaxVolName,    OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_GetPhysDeviceName_Error,         OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_GetPhysDeviceName_Success,       OsFileSys_Setup, OsFileSys_TearDown);

    /* OS_GetFsInfo Tests */
    ADD_TEST(Test_OS_GetFsInfo_NullPtr,        OsFileSys_Setup, OsFileSys_TearDown);
    ADD_TEST(Test_OS_GetFsInfo_Success,        OsFileSys_Setup, OsFileSys_TearDown);
}



