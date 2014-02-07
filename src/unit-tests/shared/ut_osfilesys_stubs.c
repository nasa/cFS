/*================================================================================*
** File:  ut_osfilesys_stubs.c
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

/* As defined in osfilesys.c */
extern OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES];

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

UT_OsReturnCode_t  g_fsMake = {0,0};
UT_OsReturnCode_t  g_fsRemove = {0,0};
UT_OsReturnCode_t  g_fsInit = {0,0};
UT_OsReturnCode_t  g_fsMount = {0,0};
UT_OsReturnCode_t  g_fsUnmount = {0,0};
UT_OsReturnCode_t  g_fsBlocksFree = {0,0};
UT_OsReturnCode_t  g_fsBytesFree = {0,0};
UT_OsReturnCode_t  g_fsGetPhysDriveName = {0,0};
UT_OsReturnCode_t  g_fsTranslatePath = {0,0};

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

int32 OS_mkfs(char* address, char* devname,char* volname, uint32 blocksize, uint32 numblocks)
{
    return (g_fsMake.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_rmfs(char* devname)
{
    return (g_fsRemove.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_initfs(char* address,char* devname, char* volname, uint32 blocksize, uint32 numblocks)
{
    return (g_fsInit.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_mount(const char* devname, char* mountpoint)
{
    return (g_fsMount.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_unmount(const char* mountpoint)
{
    return (g_fsUnmount.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_fsBlocksFree(const char* name)
{
    return (g_fsBlocksFree.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_fsBytesFree(const char* name, uint64* bytes_free)
{
    return (g_fsBytesFree.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_FS_GetPhysDriveName(char* PhysDriveName, char* MountPoint)
{
    return (g_fsGetPhysDriveName.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TranslatePath(const char* VirtualPath, char* LocalPath)
{
    return (g_fsTranslatePath.value);
}

/*================================================================================*
** End of File: ut_osfilesys_stubs.c
**================================================================================*/
