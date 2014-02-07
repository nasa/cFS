/*================================================================================*
** File:  ut_osfile_stubs.c
** Owner: Tam Ngo
** Date:  March 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"

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

UT_OsReturnCode_t  g_osFsInit = {0,0};
UT_OsReturnCode_t  g_fileCreate = {0,0};
UT_OsReturnCode_t  g_fileOpen = {0,0};
UT_OsReturnCode_t  g_fileClose = {0,0};
UT_OsReturnCode_t  g_fileRead = {0,0};
UT_OsReturnCode_t  g_fileWrite = {0,0};
UT_OsReturnCode_t  g_fileChmod = {0,0};
UT_OsReturnCode_t  g_fileStat = {0,0};
UT_OsReturnCode_t  g_fileSeek = {0,0};
UT_OsReturnCode_t  g_fileRemove = {0,0};
UT_OsReturnCode_t  g_fileRename = {0,0};
UT_OsReturnCode_t  g_fileCopy = {0,0};
UT_OsReturnCode_t  g_fileMove = {0,0};
UT_OsReturnCode_t  g_fileOpenCheck = {0,0};

UT_OsReturnCode_t  g_shellOutputToFile = {0,0};
UT_OsReturnCode_t  g_getFdInfo = {0,0};
UT_OsReturnCode_t  g_closeAllFiles = {0,0};
UT_OsReturnCode_t  g_closeFileByName = {0,0};

UT_OsReturnCode_t  g_dirMake = {0,0};
UT_OsReturnCode_t  g_dirOpen = {0,0};
UT_OsReturnCode_t  g_dirClose = {0,0};
UT_OsReturnCode_t  g_dirRead = {0,0};
UT_OsReturnCode_t  g_dirRewind = {0,0};
UT_OsReturnCode_t  g_dirRemove = {0,0};

/* As defined in osfileapi.c */
OS_FDTableEntry  OS_FDTable[OS_MAX_NUM_OPEN_FILES];

#ifdef _LINUX_OS_
pthread_mutex_t  OS_FDTableMutex;
#endif

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

int32 OS_FS_Init(void)
{
    return (g_osFsInit.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_creat(const char* path, int32 access)
{
    return (g_fileCreate.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_open(const char* path, int32 access, uint32  mode)
{
    return (g_fileOpen.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_close(int32 filedes)
{
    return (g_fileClose.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_read(int32 filedes, void* buffer, uint32 nbytes)
{
    return (g_fileRead.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_write(int32 filedes, void* buffer, uint32 nbytes)
{
    return (g_fileWrite.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_chmod(const char* path, uint32 access)
{
    return (g_fileChmod.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_stat(const char* path, os_fstat_t* filestats)
{
    return (g_fileStat.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_lseek(int32 filedes, int32 offset, uint32 whence)
{
    return (g_fileSeek.value);
}
/*--------------------------------------------------------------------------------*/

int32 OS_remove(const char *path)
{
    return (g_fileRemove.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_rename(const char* old, const char* new)
{
    return (g_fileRename.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_cp(const char* src, const char* dest)
{
    return (g_fileCopy.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_mv(const char* src, const char* dest)
{
    return (g_fileMove.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_ShellOutputToFile(char* Cmd, int32 OS_fd)
{
    return (g_shellOutputToFile.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_FDGetInfo(int32 filedes, OS_FDTableEntry* fd_prop)
{
    return (g_getFdInfo.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_FileOpenCheck(char* filename)
{
    return (g_fileOpenCheck.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_CloseFileByName(char* filename)
{
    return (g_closeFileByName.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_CloseAllFiles(void)
{
    return (g_closeAllFiles.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_mkdir(const char* path, uint32 access)
{
    return (g_dirMake.value);
}

/*--------------------------------------------------------------------------------*/

os_dirp_t OS_opendir(const char* path)
{
    return ((os_dirp_t)g_dirOpen.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_closedir(os_dirp_t directory)
{
    return (g_dirClose.value);
}

/*--------------------------------------------------------------------------------*/

os_dirent_t* OS_readdir(os_dirp_t directory)
{
    return ((os_dirent_t*)g_dirRead.value);
}

/*--------------------------------------------------------------------------------*/

void  OS_rewinddir(os_dirp_t directory )
{
}

/*--------------------------------------------------------------------------------*/

int32 OS_rmdir(const char* path)
{
    return (g_dirRemove.value);
}

/*================================================================================*
** End of File: ut_osfile_stubs.c
**================================================================================*/
