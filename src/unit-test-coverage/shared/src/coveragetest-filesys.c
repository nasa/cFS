/*
 * Filename: osapi_testcase_filesys.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-filesys" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-shared-coveragetest.h"
#include "ut-osapi-filesys.h"

#include <overrides/string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/
void Test_OS_FileSysAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual;

    actual = OS_FileSysAPI_Init();
    UtAssert_True(actual == expected, "OS_FileSysAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_FileSysAddFixedMap(void)
{
    /*
     * Test Case For:
     * int32 OS_FileSysAddFixedMap(uint32 *filesys_id, const char *phys_path, const char *virt_path)
     */
    uint32 id;

    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, NULL, NULL), OS_INVALID_POINTER);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_PATH_LEN);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_ERR_NAME_TOO_LONG);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    UT_SetForceFail(UT_KEY(OCS_strrchr), -1);
    UT_SetDeferredRetcode(UT_KEY(OCS_strlen), 3, 2 + OS_MAX_API_NAME);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_ERR_NAME_TOO_LONG);
    UT_ResetState(UT_KEY(OCS_strlen));
    UT_ResetState(UT_KEY(OCS_strrchr));

    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_FileSysAddFixedMap(&id, "/phys", "/virt"), OS_SUCCESS);
}

void Test_OS_mkfs(void)
{
    /*
     * Test Case For:
     * int32 OS_mkfs (char *address, const char *devname, const char * volname,
     *          uint32 blocksize, uint32 numblocks)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_mkfs("adr","/ramdev0","vol",0,0);
    UtAssert_True(actual == expected, "OS_mkfs() (%ld) == OS_SUCCESS", (long)actual);

    /*
     * Test an entry NOT found in the OS_VolumeTable
     */
    actual = OS_mkfs("adr","/rd1","vol1",0,0);
    UtAssert_True(actual == expected, "OS_mkfs() (%ld) == OS_SUCCESS", (long)actual);


    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_mkfs(NULL,NULL,NULL,0,0);
    UtAssert_True(actual == expected, "OS_mkfs() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_FS_DEV_NAME_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_mkfs("adr","/ramdev0","vol",0,0);
    UtAssert_True(actual == expected, "OS_mkfs() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    /* set up for failure due to empty strings */
    expected = OS_FS_ERR_PATH_INVALID;
    actual = OS_mkfs("adr","","",0,0);
    UtAssert_True(actual == expected, "OS_mkfs() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    /* set up for failure due to formatting */
    UT_SetForceFail(UT_KEY(OS_FileSysFormatVolume_Impl), OS_FS_ERR_DRIVE_NOT_CREATED);
    expected = OS_FS_ERR_DRIVE_NOT_CREATED;
    actual = OS_mkfs("adr","/ramdev0","vol",0,0);
    UtAssert_True(actual == expected, "OS_mkfs() (%ld) == OS_FS_ERR_DRIVE_NOT_CREATED (format failure)", (long)actual);

    /* set up for failure due to no free slots */
    UT_SetForceFail(UT_KEY(OS_ObjectIdAllocateNew), OS_ERR_NO_FREE_IDS);
    expected = OS_FS_ERR_DEVICE_NOT_FREE;
    actual = OS_mkfs("adr","/ramdev0","vol",0,0);
    UtAssert_True(actual == expected, "OS_mkfs() (%ld) == OS_FS_ERR_DEVICE_NOT_FREE", (long)actual);
}

void Test_OS_rmfs(void)
{
    /*
     * Test Case For:
     * int32 OS_rmfs (const char *devname)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_rmfs("/ramdev5");
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_SUCCESS", (long)actual);

    /* check error paths */
    UT_SetForceFail(UT_KEY(OS_ObjectIdGetByName), OS_ERR_NAME_NOT_FOUND);
    expected = OS_FS_ERROR;
    actual = OS_rmfs("/ramdev4");
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_FS_ERROR", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdGetByName));

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_rmfs(NULL);
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_FS_DEV_NAME_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_rmfs("/ramdev4");
    UtAssert_True(actual == expected, "OS_rmfs() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
}


void Test_OS_initfs(void)
{
    /*
     * Test Case For:
     * int32 OS_initfs (char *address,const char *devname, const char *volname,
     *          uint32 blocksize, uint32 numblocks)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_initfs("adr","/ramdev0","vol",0,0);
    UtAssert_True(actual == expected, "OS_initfs() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_initfs(NULL,"/hda2","vol2",0,0);
    UtAssert_True(actual == expected, "OS_initfs() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_initfs(NULL,NULL,NULL,0,0);
    UtAssert_True(actual == expected, "OS_initfs() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_FS_DEV_NAME_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_initfs("adr","/ramdev0","vol",0,0);
    UtAssert_True(actual == expected, "OS_initfs() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    /* set up for failure */
    UT_SetForceFail(UT_KEY(OS_ObjectIdAllocateNew), OS_ERR_NO_FREE_IDS);
    expected = OS_FS_ERR_DEVICE_NOT_FREE;
    actual = OS_initfs("adr","/ramdev0","vol",0,0);
    UtAssert_True(actual == expected, "OS_initfs() (%ld) == OS_FS_ERR_DEVICE_NOT_FREE", (long)actual);
}

void Test_OS_mount(void)
{
    /*
     * Test Case For:
     * int32 OS_mount (const char *devname, const char* mountpoint)
     */
    int32 expected;
    int32 actual;

    /* supposed to be OS_ERR_INCORRECT_OBJ_STATE, but
     * actually OS_FS_ERROR for compatibility */
    expected = OS_FS_ERROR;
    actual = OS_mount("/ramdev5","/ram5");
    UtAssert_True(actual == expected, "OS_mount() (%ld) == OS_FS_ERROR", (long)actual);

    /* mount on a fixed disk historically returns OS_SUCCESS and is a no-op.
     * This is for backward compatibility (it should probably an error to do this) */
    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY |
            OS_FILESYS_FLAG_IS_FIXED;
    expected = OS_SUCCESS;
    actual = OS_mount("/ramdev5","/ram5");
    UtAssert_True(actual == expected, "OS_mount(fixed) (%ld) == OS_SUCCESS", (long)actual);


    /* set up so record is in the right state for mounting */
    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY;
    expected = OS_SUCCESS;
    actual = OS_mount("/ramdev5","/ram5");
    UtAssert_True(actual == expected, "OS_mount(nominal) (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_mount(NULL,NULL);
    UtAssert_True(actual == expected, "OS_mount() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_FS_DEV_NAME_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_mount("/ramdev0","/ram0");
    UtAssert_True(actual == expected, "OS_mount() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);

}


void Test_OS_unmount(void)
{
    /*
     * Test Case For:
     * int32 OS_unmount (const char *mountpoint)
     */
    int32 expected;
    int32 actual;

    /* supposed to be OS_ERR_INCORRECT_OBJ_STATE, but
     * actually OS_FS_ERROR for compatibility */
    expected = OS_FS_ERROR;
    actual = OS_unmount("/ram0");
    UtAssert_True(actual == expected, "OS_mount() (%ld) == OS_FS_ERROR", (long)actual);

    /* unmount on a fixed disk historically returns OS_SUCCESS and is a no-op.
     * This is for backward compatibility (it should probably an error to do this) */
    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY |
            OS_FILESYS_FLAG_IS_FIXED |
            OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM |
            OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
    expected = OS_SUCCESS;
    actual = OS_unmount("/ram0");
    UtAssert_True(actual == expected, "OS_unmount(fixed) (%ld) == OS_SUCCESS", (long)actual);

    /* set up so record is in the right state for mounting */
    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY |
            OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM |
            OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
    expected = OS_SUCCESS;
    actual = OS_unmount("/ram0");
    UtAssert_True(actual == expected, "OS_unmount(nominal) (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_unmount(NULL);
    UtAssert_True(actual == expected, "OS_unmount() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_PATH_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_unmount("/ram0");
    UtAssert_True(actual == expected, "OS_unmount() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);

}


void Test_OS_fsBlocksFree(void)
{
    /*
     * Test Case For:
     * int32 OS_fsBlocksFree (const char *name)
     */
    int32 expected = 1111;
    int32 actual = ~OS_SUCCESS;
    OS_statvfs_t statval;

    statval.block_size = 1024;
    statval.blocks_free = 1111;
    statval.total_blocks = 2222;
    UT_SetDataBuffer(UT_KEY(OS_FileSysStatVolume_Impl), &statval, sizeof(statval), false);
    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;

    actual = OS_fsBlocksFree("/cf");
    UtAssert_True(actual == expected, "OS_fsBlocksFree() (%ld) == 1111", (long)actual);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_fsBlocksFree(NULL);
    UtAssert_True(actual == expected, "OS_fsBlocksFree() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_PATH_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_fsBlocksFree("/cf");
    UtAssert_True(actual == expected, "OS_fsBlocksFree() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    UT_SetForceFail(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    expected = OS_FS_ERR_PATH_INVALID;
    actual = OS_fsBlocksFree("invalid");
    UtAssert_True(actual == expected, "OS_fsBlocksFree() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);


}


void Test_OS_fsBytesFree(void)
{
    /*
     * Test Case For:
     * int32 OS_fsBytesFree (const char *name, uint64 *bytes_free)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_statvfs_t statval;
    uint64 bytes_free = 0;

    statval.block_size = 1024;
    statval.blocks_free = 1111;
    statval.total_blocks = 2222;
    UT_SetDataBuffer(UT_KEY(OS_FileSysStatVolume_Impl), &statval, sizeof(statval), false);
    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;

    actual = OS_fsBytesFree("/cf", &bytes_free);

    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(bytes_free == (1024*1111), "bytes_free (%lu) == (1024*1111)",
            (unsigned long)bytes_free);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_fsBytesFree(NULL, NULL);
    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_PATH_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_fsBytesFree("/cf", &bytes_free);
    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    UT_SetForceFail(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    expected = OS_FS_ERR_PATH_INVALID;
    actual = OS_fsBytesFree("invalid",&bytes_free);
    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

}


void Test_OS_chkfs(void)
{
    /*
     * Test Case For:
     * int32 OS_chkfs (const char *name, bool repair)
     */
    int32 expected;
    int32 actual;

    /* Setup for success by having strings match */
    expected = OS_SUCCESS;
    actual = OS_chkfs("/cf",true);
    UtAssert_True(actual == expected, "OS_chkfs() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_chkfs(NULL,false);
    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_PATH_LEN);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_chkfs("/cf",false);
    UtAssert_True(actual == expected, "OS_fsBytesFree() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    /* Test Fail due to no matching VolTab entry */
    UT_SetForceFail(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_chkfs("none", true);
    UtAssert_True(actual == expected, "OS_chkfs() (%ld) == OS_ERR_NAME_NOT_FOUND", (long)actual);
}


void Test_OS_FS_GetPhysDriveName(void)
{
    /*
     * Test Case For:
     * int32 OS_FS_GetPhysDriveName(char * PhysDriveName, const char * MountPoint)
     */
    int32 expected = OS_FS_ERR_INVALID_POINTER;
    int32 actual = OS_FS_GetPhysDriveName(NULL, NULL);
    char NameBuf[OS_FS_PHYS_NAME_LEN];

    UtAssert_True(actual == expected, "OS_FS_GetPhysDriveName() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), OS_MAX_PATH_LEN + 10);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_FS_GetPhysDriveName(NameBuf,"none");
    UtAssert_True(actual == expected, "OS_FS_GetPhysDriveName() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_FS_GetPhysDriveName(NameBuf,"none");
    UtAssert_True(actual == expected, "OS_FS_GetPhysDriveName() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);

    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
    expected = OS_SUCCESS;
    actual = OS_FS_GetPhysDriveName(NameBuf,"none");
    UtAssert_True(actual == expected, "OS_FS_GetPhysDriveName() (%ld) == OS_FS_ERROR", (long)actual);

    /* Test Fail due to no matching VolTab entry */
    UT_SetForceFail(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    expected = OS_FS_ERROR;
    actual = OS_FS_GetPhysDriveName(NameBuf,"none");
    UtAssert_True(actual == expected, "OS_FS_GetPhysDriveName() (%ld) == OS_FS_ERROR", (long)actual);
}


void Test_OS_GetFsInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_GetFsInfo(OS_FsInfo_t  *filesys_info)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    os_fsinfo_t  filesys_info;

    actual = OS_GetFsInfo(&filesys_info);

    UtAssert_True(actual == expected, "OS_FileSysInfo() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(filesys_info.MaxFds == OS_MAX_NUM_OPEN_FILES,
            "filesys_info.MaxFds (%lu) == OS_MAX_NUM_OPEN_FILES",
            (unsigned long)filesys_info.MaxFds);
    UtAssert_True(filesys_info.MaxVolumes == OS_MAX_FILE_SYSTEMS,
            "filesys_info.MaxVolumes (%lu) == OS_MAX_FILE_SYSTEMS",
            (unsigned long)filesys_info.MaxVolumes);

    /* since there are no open files, the free fd count should match the max */
    UtAssert_True(filesys_info.FreeFds == OS_MAX_NUM_OPEN_FILES,
            "filesys_info.FreeFds (%lu) == OS_MAX_NUM_OPEN_FILES",
            (unsigned long)filesys_info.FreeFds);

    UtAssert_True(filesys_info.FreeVolumes == OS_MAX_FILE_SYSTEMS,
            "filesys_info.FreeVolumes (%lu) == OS_MAX_FILE_SYSTEMS",
            (unsigned long)filesys_info.FreeVolumes);

    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_GetFsInfo(NULL);
    UtAssert_True(actual == expected, "OS_GetFsInfo() (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

}

void Test_OS_TranslatePath(void)
{
    /*
     * Test Case For:
     * int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
     */
    char LocalBuffer[OS_MAX_PATH_LEN];
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    /* Set up the local record for success */
    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
    strcpy(OS_filesys_table[1].virtual_mountpt,"/cf");
    strcpy(OS_filesys_table[1].system_mountpt,"/mnt/cf");

    actual = OS_TranslatePath("/cf/test",LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(LocalBuffer,"/mnt/cf/test") == 0, "OS_TranslatePath(/cf/test) (%s)  == /mnt/cf/test", LocalBuffer);

    /* Check various error paths */
    expected = OS_FS_ERR_INVALID_POINTER;
    actual = OS_TranslatePath(NULL, NULL);
    UtAssert_True(actual == expected, "OS_TranslatePath(NULL,NULL) (%ld) == OS_FS_ERR_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), OS_MAX_PATH_LEN + 10);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_TranslatePath("/cf/test",LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    expected = OS_FS_ERR_PATH_INVALID;
    actual = OS_TranslatePath("invalid",LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    UT_SetForceFail(UT_KEY(OS_ObjectIdGetBySearch), OS_ERR_NAME_NOT_FOUND);
    actual = OS_TranslatePath("/cf/test",LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath() (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdGetBySearch));

    UT_SetDeferredRetcode(UT_KEY(OCS_strlen), 3, OS_MAX_PATH_LEN + 10);
    expected = OS_FS_ERR_PATH_INVALID;
    actual = OS_TranslatePath("/cf/test",LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_FS_ERR_PATH_INVALID", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_memcpy), OS_ERROR);
    UT_SetDeferredRetcode(UT_KEY(OCS_strlen), 1, OS_MAX_PATH_LEN - 1);
    UT_SetDeferredRetcode(UT_KEY(OCS_strlen), 1, OS_MAX_LOCAL_PATH_LEN - 1);
    UT_SetDeferredRetcode(UT_KEY(OCS_strlen), 1, 1);
    expected = OS_FS_ERR_PATH_TOO_LONG;
    actual = OS_TranslatePath("/cf/test",LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_FS_ERR_PATH_TOO_LONG", (long)actual);

    OS_filesys_table[1].flags = 0;
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TranslatePath("/cf/test",LocalBuffer);
    UtAssert_True(actual == expected, "OS_TranslatePath(/cf/test) (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_FileSys_FindVirtMountPoint(void)
{
    /*
     * Test Case For:
     * static bool OS_FileSys_FindVirtMountPoint(void *ref, uint32 local_id, const OS_common_record_t *obj)
     *
     * This is a static internal function and must be invoked through a UT-specific wrapper in
     * order to get coverage on it.
     */
    bool result;
    OS_common_record_t refobj;
    const char refstr[] = "/ut";

    memset(&refobj, 0, sizeof(refobj));
    OS_filesys_table[1].flags = 0;
    OS_filesys_table[1].virtual_mountpt[0] = 0;

    result = Osapi_Internal_FileSys_FindVirtMountPoint((void*)refstr, 1, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (unmounted) == false", refstr);

    OS_filesys_table[1].flags = OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;

    /* Verify cases where one is a substring of the other -
     * these should also return false */
    strncpy(OS_filesys_table[1].virtual_mountpt, "/ut11", sizeof(OS_filesys_table[1].virtual_mountpt));
    result = Osapi_Internal_FileSys_FindVirtMountPoint((void*)refstr, 1, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (mountpt=%s) == false", refstr, OS_filesys_table[1].virtual_mountpt);

    strncpy(OS_filesys_table[1].virtual_mountpt, "/u", sizeof(OS_filesys_table[1].virtual_mountpt));
    result = Osapi_Internal_FileSys_FindVirtMountPoint((void*)refstr, 1, &refobj);
    UtAssert_True(!result, "OS_FileSys_FindVirtMountPoint(%s) (mountpt=%s) == false", refstr, OS_filesys_table[1].virtual_mountpt);

    strncpy(OS_filesys_table[1].virtual_mountpt, "/ut", sizeof(OS_filesys_table[1].virtual_mountpt));
    result = Osapi_Internal_FileSys_FindVirtMountPoint((void*)refstr, 1, &refobj);
    UtAssert_True(result, "OS_FileSys_FindVirtMountPoint(%s) (nominal) == true", refstr);
}

/*
 * Specific volume table entries to exercise translation cases in OS_FileSys_InitLocalFromVolTable()
 */
static const OS_VolumeInfo_t UT_VOLTAB_TESTCASES[] =
{
        { .DeviceName = "/UT1", .VolumeType = ATA_DISK, .FreeFlag = false, .IsMounted = true },
        { .DeviceName = "/UT2", .VolumeType = EEPROM_DISK, .FreeFlag = true, .IsMounted = false },
        { .DeviceName = "/UT3", .VolumeType = RAM_DISK, .FreeFlag = true, .IsMounted = false }
};

void Test_OS_FileSys_InitLocalFromVolTable(void)
{
    /*
     * Test Case For:
     * static int32 OS_FileSys_InitLocalFromVolTable(OS_filesys_internal_record_t *local, const OS_VolumeInfo_t *Vol)
     *
     * This is a static internal function and must be invoked through a UT-specific wrapper in
     * order to get coverage on it.
     */
    OS_filesys_internal_record_t temprec;
    int32 actual;
    int32 expected;


    /* this should return OS_ERROR because the mount point was not valid */
    memset(&temprec,0,sizeof(temprec));
    expected = OS_ERROR;
    actual = Osapi_Internal_FileSys_InitLocalFromVolTable(&temprec, &UT_VOLTAB_TESTCASES[0]);
    UtAssert_True(actual == expected, "OS_FileSys_InitLocalFromVolTable(0) (%ld) == OS_ERROR", (long)actual);

    memset(&temprec,0,sizeof(temprec));
    expected = OS_SUCCESS;
    actual = Osapi_Internal_FileSys_InitLocalFromVolTable(&temprec, &UT_VOLTAB_TESTCASES[1]);
    UtAssert_True(actual == expected, "OS_FileSys_InitLocalFromVolTable(1) (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(temprec.fstype == OS_FILESYS_TYPE_MTD, "OS_FileSys_InitLocalFromVolTable(1) fstype(%u) == OS_FILESYS_TYPE_MTD",
            (unsigned int)temprec.fstype);

    memset(&temprec,0,sizeof(temprec));
    expected = OS_SUCCESS;
    actual = Osapi_Internal_FileSys_InitLocalFromVolTable(&temprec, &UT_VOLTAB_TESTCASES[2]);
    UtAssert_True(actual == expected, "OS_FileSys_InitLocalFromVolTable(1) (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(temprec.fstype == OS_FILESYS_TYPE_VOLATILE_DISK, "OS_FileSys_InitLocalFromVolTable(2) fstype(%u) == OS_FILESYS_TYPE_MTD",
            (unsigned int)temprec.fstype);
}

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

/*
 * Register the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_FileSysAPI_Init);
    ADD_TEST(OS_FileSysAddFixedMap);
    ADD_TEST(OS_mkfs);
    ADD_TEST(OS_rmfs);
    ADD_TEST(OS_initfs);
    ADD_TEST(OS_mount);
    ADD_TEST(OS_unmount);
    ADD_TEST(OS_fsBlocksFree);
    ADD_TEST(OS_fsBytesFree);
    ADD_TEST(OS_chkfs);
    ADD_TEST(OS_FS_GetPhysDriveName);
    ADD_TEST(OS_GetFsInfo);
    ADD_TEST(OS_TranslatePath);
    ADD_TEST(OS_FileSys_FindVirtMountPoint);
    ADD_TEST(OS_FileSys_InitLocalFromVolTable);
}





