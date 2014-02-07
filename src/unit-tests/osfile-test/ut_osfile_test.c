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

extern UT_OsLogInfo_t  g_logInfo;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

char* g_fsAddrPtr = NULL;

int32  g_skipTestCase = -1;
char*  g_skipTestCaseResult = " ";

char  g_longPathName[OS_MAX_PATH_LEN+5];
char  g_longFileName[OS_MAX_PATH_LEN];
char  g_invalidPath[OS_MAX_PATH_LEN];

char* g_devName = "/ramdev3";
char* g_mntName = "/drive3";

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

int32 UT_os_setup_fs(void);
int32 UT_os_teardown_fs(void);

void UT_os_init_file_misc(void);

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

int32 UT_os_setup_fs()
{
    int32 res=OS_FS_SUCCESS;
    char text[UT_OS_LG_TEXT_LEN];

    res = OS_mkfs(g_fsAddrPtr, g_devName, " ", 512, 20);
    if (res != OS_FS_SUCCESS)
        goto UT_os_setup_fs_exit_tag;

    res = OS_mount(g_devName, g_mntName);
    if (res != OS_FS_SUCCESS)
    {
        OS_rmfs(g_devName);
        goto UT_os_setup_fs_exit_tag;
    }

UT_os_setup_fs_exit_tag:
    memset(text, '\0', sizeof(text));
    UT_os_sprintf(text, "\nUT_os_setup_fs() returns %d\n", (int)res);
    UT_OS_LOG_MACRO(text);

    return (res);
}

/*--------------------------------------------------------------------------------*/

int32 UT_os_teardown_fs()
{
    OS_unmount(g_mntName);
    OS_rmfs(g_devName);

    return (OS_FS_SUCCESS);
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_file_misc()
{
    memset(g_longPathName, 'Y', sizeof(g_longPathName));
    g_longPathName[0]  = '/';
    g_longPathName[sizeof(g_longPathName)-1] = '\0';

    memset(g_longFileName, 'Z', sizeof(g_longFileName));
    g_longFileName[0] = '/';
    g_longFileName[4] = '/';
    g_longFileName[sizeof(g_longFileName)-1] = '\0';

    memset(g_invalidPath, '\0', sizeof(g_invalidPath));
    strcpy(g_invalidPath, "/InvalidMnt/log1.txt");
}

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/
#ifdef _OSAL_UNIT_TEST_
   void OS_Application_Startup(void)
#else
   int main(int argc, char* argv[])
#endif
{
    UT_os_setup(UT_OS_LOG_FILENAME);

    /* UT_OS_LOG_OFF, UT_OS_LOG_MINIMAL, UT_OS_LOG_MODERATE, UT_OS_LOG_EVERYTHING */
    UT_os_set_log_verbose(UT_OS_LOG_EVERYTHING);

    UT_OS_LOG_MACRO("OSAL Unit Test Output File for osfile APIs\n");

    UT_os_initfs_test();

    if (UT_os_setup_fs() == OS_FS_SUCCESS)
    {
        UT_os_init_file_misc();

        /* Directory I/O APIs */
        UT_os_makedir_test();
        UT_os_opendir_test();
        UT_os_closedir_test();

        UT_os_readdir_test();
        UT_os_rewinddir_test();
        UT_os_removedir_test();

        /* File I/O APIs */
        UT_os_createfile_test();
        UT_os_openfile_test();
        UT_os_closefile_test();

        UT_os_readfile_test();
        UT_os_writefile_test();
        UT_os_lseekfile_test();

        UT_os_chmodfile_test();
        UT_os_statfile_test();

        UT_os_removefile_test();
        UT_os_renamefile_test();
        UT_os_copyfile_test();
        UT_os_movefile_test();

        UT_os_outputtofile_test();

        UT_os_getfdinfo_test();
        UT_os_checkfileopen_test();

        UT_os_closeallfiles_test();
        UT_os_closefilebyname_test();

        UT_os_teardown_fs();
    }

    UT_os_teardown("ut_osfile");

    return (0);
}

/*================================================================================*
** End of File: ut_osfile_test.c
**================================================================================*/
