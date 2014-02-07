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

extern UT_OsLogInfo_t  g_logInfo;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

char* g_fsAddrPtr = NULL;

int32 g_blkSize = UT_OS_FS_BLOCK_SIZE;
int32 g_blkCnt  = UT_OS_FS_MAX_BLOCKS;

int32  g_skipTestCase = -1;
char*  g_skipTestCaseResult = " ";

char  g_fsLongName[OS_MAX_PATH_LEN+5];
char  g_physDriveName[OS_MAX_PATH_LEN];

char  g_volNames[UT_OS_FILESYS_LIST_LEN][UT_OS_XS_TEXT_LEN];
char  g_devNames[UT_OS_FILESYS_LIST_LEN][UT_OS_XS_TEXT_LEN];
char  g_mntNames[UT_OS_FILESYS_LIST_LEN][UT_OS_XS_TEXT_LEN];

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_init_fs_misc(void);
void UT_os_init_checkfs_test(void);

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

/*--------------------------------------------------------------------------------*/

void UT_os_init_checkfs_test()
{
	g_skipTestCase = 3;
	g_skipTestCaseResult = UT_OS_NA;
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

    UT_OS_LOG_MACRO("OSAL Unit Test Output File for osfilesys APIs\n")

    OS_API_Init();

    UT_os_print_volumetable("VOLUME TABLE _AFTER_ OS_API_Init() call");

    UT_os_init_fs_misc();

    UT_os_makefs_test();
    UT_os_initfs_test();
    UT_os_removefs_test();

    UT_os_mount_test();
    UT_os_unmount_test();

    UT_os_getphysdrivename_test();
    UT_os_translatepath_test();

    UT_os_init_checkfs_test();
    UT_os_checkfs_test();

    UT_os_fsblocksfree_test();
    UT_os_fsbytesfree_test();

    UT_os_teardown("ut_osfilesys");

    return (0);
}

/*================================================================================*
** End of File: ut_osfilesys_test.c
**================================================================================*/
