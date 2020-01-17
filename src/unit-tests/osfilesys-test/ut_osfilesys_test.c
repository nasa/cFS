/*================================================================================*
** File:  ut_osfilesys_test.c
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osfilesys_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_FS_BLOCK_SIZE  512
#define UT_OS_FS_MAX_BLOCKS   20

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

char* g_fsAddrPtr = NULL;

int32 g_blkSize = UT_OS_FS_BLOCK_SIZE;
int32 g_blkCnt  = UT_OS_FS_MAX_BLOCKS;

char  g_fsLongName[UT_OS_PATH_BUFF_SIZE];
char  g_physDriveName[UT_OS_PHYS_NAME_BUFF_SIZE];

char  g_volNames[UT_OS_FILESYS_LIST_LEN][UT_OS_NAME_BUFF_SIZE];
char  g_devNames[UT_OS_FILESYS_LIST_LEN][UT_OS_FILE_BUFF_SIZE];
char  g_mntNames[UT_OS_FILESYS_LIST_LEN][UT_OS_FILE_BUFF_SIZE];

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_init_fs_misc(void);

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

void UT_os_init_fs_misc()
{
    memset(g_fsLongName, 'X', sizeof(g_fsLongName));
    g_fsLongName[0] = '/';
    g_fsLongName[sizeof(g_fsLongName)-1] = '\0';

    strcpy(g_devNames[0], "/ramdev0");
    strcpy(g_volNames[0], " ");
    strcpy(g_mntNames[0], "/drive0");

    strcpy(g_devNames[1], "/ramdev1");
    strcpy(g_volNames[1], " ");
    strcpy(g_mntNames[1], "/drive1");

    strcpy(g_devNames[2], "/ramdev2");
    strcpy(g_volNames[2], " ");
    strcpy(g_mntNames[2], "/drive2");

    strcpy(g_devNames[3], "/ramdev3");
    strcpy(g_volNames[3], " ");
    strcpy(g_mntNames[3], "/drive3");

    strcpy(g_devNames[4], "/ramdev4");
    strcpy(g_volNames[4], " ");
    strcpy(g_mntNames[4], "/drive4");

    strcpy(g_devNames[5], "/ramdev5");
    strcpy(g_volNames[5], " ");
    strcpy(g_mntNames[5], "/drive5");

    strcpy(g_devNames[6], "/ramdev6");
    strcpy(g_volNames[6], " ");
    strcpy(g_mntNames[6], "/drive6");

    strcpy(g_devNames[7], "/ramdev7");
    strcpy(g_volNames[7], " ");
    strcpy(g_mntNames[7], "/drive7");
}

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    UT_os_init_fs_misc();

    UtTest_Add(UT_os_makefs_test, NULL, NULL, "OS_mkfs");
    UtTest_Add(UT_os_initfs_test, NULL, NULL, "OS_initfs");
    UtTest_Add(UT_os_removefs_test, NULL, NULL, "OS_rmfs");

    UtTest_Add(UT_os_mount_test, NULL, NULL, "OS_mount");
    UtTest_Add(UT_os_unmount_test, NULL, NULL, "OS_unmount");

    UtTest_Add(UT_os_getphysdrivename_test, NULL, NULL, "OS_FS_GetPhysDriveName");
    UtTest_Add(UT_os_getfsinfo_test, NULL, NULL, "OS_GetFsInfo");
    UtTest_Add(UT_os_translatepath_test, NULL, NULL, "OS_TranslatePath (internal)");

    UtTest_Add(UT_os_checkfs_test, NULL, NULL, "OS_chkfs");
    UtTest_Add(UT_os_fsblocksfree_test, NULL, NULL, "OS_fsBlocksFree");
    UtTest_Add(UT_os_fsbytesfree_test, NULL, NULL, "OS_fsBytesFree");
}

/*================================================================================*
** End of File: ut_osfilesys_test.c
**================================================================================*/
