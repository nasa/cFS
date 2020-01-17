/*================================================================================*
** File:  ut_osfile_test.c
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osfile_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

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

char  g_longPathName[UT_OS_PATH_BUFF_SIZE];
char  g_longFileName[UT_OS_PATH_BUFF_SIZE];
char  g_invalidPath[UT_OS_PATH_BUFF_SIZE];

const char* g_devName = "/ramdev3";
const char* g_mntName = "/drive3";

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

int32 UT_os_setup_fs(void);
void UT_os_teardown_fs(void);

void UT_os_init_file_misc(void);

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

int32 UT_os_setup_fs()
{
    int32 res;

    res = OS_mkfs(g_fsAddrPtr, g_devName, " ", 512, 20);
    if (res != OS_FS_SUCCESS)
    {
        UT_OS_LOG("OS_mkfs() returns %d\n", (int)res);;
        goto UT_os_setup_fs_exit_tag;
    }

    res = OS_mount(g_devName, g_mntName);
    if (res != OS_FS_SUCCESS)
    {
        UT_OS_LOG("OS_mount() returns %d\n", (int)res);;
        OS_rmfs(g_devName);
        goto UT_os_setup_fs_exit_tag;
    }

UT_os_setup_fs_exit_tag:
    return (res);
}

/*--------------------------------------------------------------------------------*/

void UT_os_teardown_fs()
{
    OS_unmount(g_mntName);
    OS_rmfs(g_devName);
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_file_misc()
{
    memset(g_longPathName, 'Y', sizeof(g_longPathName));
    g_longPathName[0]  = '/';
    g_longPathName[sizeof(g_longPathName)-1] = '\0';

    /*
     * The "g_longFileName" should be within the OS_MAX_PATH_LEN limit,
     * but the filename portion of it exceeds the OS_MAX_FILE_NAME limit.
     */
    memset(g_longFileName, 'Z', sizeof(g_longFileName));
    g_longFileName[0] = '/';
    g_longFileName[4] = '/';
    g_longFileName[OS_MAX_FILE_NAME+9] = '\0';

    memset(g_invalidPath, '\0', sizeof(g_invalidPath));
    strcpy(g_invalidPath, "/InvalidMnt/log1.txt");
}

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/

void UtTest_Setup(void)
{
    UT_os_initfs_test();

    if (UT_os_setup_fs() == OS_FS_SUCCESS)
    {
        UT_os_init_file_misc();

        /* Directory I/O APIs */
        UtTest_Add(UT_os_makedir_test, NULL, NULL, "OS_mkdir");
        UtTest_Add(UT_os_opendir_test, NULL, NULL, "OS_opendir");
        UtTest_Add(UT_os_closedir_test, NULL, NULL, "OS_closedir");

        UtTest_Add(UT_os_readdir_test, NULL, NULL, "OS_readdir");
        UtTest_Add(UT_os_rewinddir_test, NULL, NULL, "OS_rewinddir");
        UtTest_Add(UT_os_removedir_test, NULL, NULL, "OS_rmdir");

        /* File I/O APIs */
        UtTest_Add(UT_os_createfile_test, NULL, NULL, "OS_creat");
        UtTest_Add(UT_os_openfile_test, NULL, NULL, "OS_open");
        UtTest_Add(UT_os_closefile_test, NULL, NULL, "OS_close");

        UtTest_Add(UT_os_readfile_test, NULL, NULL, "OS_read");
        UtTest_Add(UT_os_writefile_test, NULL, NULL, "OS_write");
        UtTest_Add(UT_os_lseekfile_test, NULL, NULL, "OS_lseek");

        UtTest_Add(UT_os_chmodfile_test, NULL, NULL, "OS_chmod");
        UtTest_Add(UT_os_statfile_test, NULL, NULL, "OS_stat");

        UtTest_Add(UT_os_removefile_test, NULL, NULL, "OS_remove");
        UtTest_Add(UT_os_renamefile_test, NULL, NULL, "OS_rename");
        UtTest_Add(UT_os_copyfile_test, NULL, NULL, "OS_cp");
        UtTest_Add(UT_os_movefile_test, NULL, NULL, "OS_mv");

        UtTest_Add(UT_os_outputtofile_test, NULL, NULL, "OS_ShellOutputToFile");

        UtTest_Add(UT_os_getfdinfo_test, NULL, NULL, "OS_FDGetInfo");
        UtTest_Add(UT_os_checkfileopen_test, NULL, NULL, "OS_FileOpenCheck");

        UtTest_Add(UT_os_closeallfiles_test, NULL, NULL, "OS_CloseAllFiles");
        UtTest_Add(UT_os_closefilebyname_test, NULL, NULL, "OS_CloseFileByName");

        UtTest_Add(NULL, NULL, UT_os_teardown_fs, "TEARDOWN");
    }
}

/*================================================================================*
** End of File: ut_osfile_test.c
**================================================================================*/
