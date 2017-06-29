/*
 * File: osfilesys_stubs.c
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 *
 * Modification History:
 *   07/14/2015  Alan Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */

#include <string.h>

#include "osfilesys_stubs.h"



OsFileSys_HookTable_t           OsFileSys_HookTable;
OsFileSys_ReturnCodeTable_t     OsFileSys_ReturnCodeTable[OSFILESYS_MAX_INDEX];


/* Utility variables */
static uint32 nSemTake    = 0;
static uint32 nSemGive    = 0;
static uint32 nCloseCalls = 0;
static uint32 nOpenCalls  = 0;


void OsFileSys_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < OSFILESYS_MAX_INDEX) {
        OsFileSys_ReturnCodeTable[Index].Value = RtnVal;
        OsFileSys_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %lu\n", (unsigned long)Index);
    }
}


boolean OsFileSys_UseReturnCode(uint32 Index)
{
    if (OsFileSys_ReturnCodeTable[Index].Count > 0) {
        OsFileSys_ReturnCodeTable[Index].Count--;
        if (OsFileSys_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}


void OsFileSys_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if  (Index == OSFILESYS_OPEN_INDEX)            { OsFileSys_HookTable.open   = FunPtr; }
    else
    {
        printf("Unsupported OsFileSys Index In SetFunctionHook Call %lu\n", (unsigned long)Index);
    }
}


void OsFileSys_Reset(void)
{
    memset(&OsFileSys_HookTable, 0, sizeof(OsFileSys_HookTable));
    memset(&OsFileSys_ReturnCodeTable, 0, sizeof(OsFileSys_ReturnCodeTable));
    memset(&OS_FDTable, 0, sizeof(OS_FDTable));

    nSemTake    = 0;
    nSemGive    = 0;
    nCloseCalls = 0;
    nOpenCalls  = 0;
}

/* Utilities */
uint32 getNSemTake()
{
    return nSemTake;
}

uint32 getNSemGive()
{
    return nSemGive;
}


uint32 getNCloseCalls()
{
    return nCloseCalls;
}

uint32 getNOpenCalls()
{
    return nOpenCalls;
}


/* Stubbed functions */
size_t VCS_strlen(const char *str)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_STRLEN_INDEX))
        return (size_t)OsFileSys_ReturnCodeTable[OSFILESYS_STRLEN_INDEX].Value;

    return strlen(str);
}

int VCS_strcmp(const char *str1, const char *str2)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_STRCMP_INDEX))
        return (int)OsFileSys_ReturnCodeTable[OSFILESYS_STRCMP_INDEX].Value;

    return strcmp(str1, str2);
}

int VCS_strncmp(const char *str1, const char *str2, size_t n)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_STRNCMP_INDEX))
        return (int)OsFileSys_ReturnCodeTable[OSFILESYS_STRNCMP_INDEX].Value;

    return strncmp(str1, str2, n);
}

char *VCS_strcpy(char *str1, const char *str2)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_STRCPY_INDEX))
        return (char *)OsFileSys_ReturnCodeTable[OSFILESYS_STRCPY_INDEX].Value;

    return strcpy(str1, str2);
}

char *VCS_strncpy(char *str1, const char *str2, size_t n)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_STRNCPY_INDEX))
        return (char *)OsFileSys_ReturnCodeTable[OSFILESYS_STRNCPY_INDEX].Value;

    return strncpy(str1, str2, n);
}


int VCS_ioctl (int fd, int function, ...)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_IOCTL_INDEX))
        return OsFileSys_ReturnCodeTable[OSFILESYS_IOCTL_INDEX].Value;

    return 0;
}


int VCS_open (const char *filename, int flags, ...)
{
    nOpenCalls++;

    if (OsFileSys_UseReturnCode(OSFILESYS_OPEN_INDEX))
    {
        return (int)OsFileSys_ReturnCodeTable[OSFILESYS_OPEN_INDEX].Value;
    }

    return 0; //open(filename, flags, mode);
}

VCS_STATUS VCS_close (int fd)
{
    nCloseCalls++;

    if (OsFileSys_UseReturnCode(OSFILESYS_CLOSE_INDEX))
        return (VCS_STATUS)OsFileSys_ReturnCodeTable[OSFILESYS_CLOSE_INDEX].Value;

    return VCS_OK; //close(fd);
}


VCS_STATUS VCS_semTake(VCS_SEM_ID semId, int timeout)
{
    nSemTake += 1;

    return nSemTake;
}


VCS_STATUS VCS_semGive(VCS_SEM_ID semId)
{
    nSemGive += 1;

    return nSemGive;
}


VCS_BLK_DEV *VCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_ATADEVCREATE_INDEX))
        return (VCS_BLK_DEV *)OsFileSys_ReturnCodeTable[OSFILESYS_ATADEVCREATE_INDEX].Value;

    return (VCS_BLK_DEV *)1;
}

VCS_STATUS VCS_dosFsVolFormat(char *path, int opt, VCS_FUNCPTR pPromptFunc)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_DOSFSVOLFORMAT_INDEX))
        return (VCS_STATUS)OsFileSys_ReturnCodeTable[OSFILESYS_DOSFSVOLFORMAT_INDEX].Value;

    return VCS_OK;
}

int VCS_statfs(char *_name, struct VCS_statfs *_pStat)
{
    if (_pStat != NULL)
    {
        _pStat->f_bfree = 50;
        _pStat->f_bsize = 4;
    }

    if (OsFileSys_UseReturnCode(OSFILESYS_STATFS_INDEX))
        return OsFileSys_ReturnCodeTable[OSFILESYS_STATFS_INDEX].Value;

    return VCS_OK;
}

VCS_BLK_DEV *VCS_ramDevCreate (char *ramAddr, int bytesPerSec, int secPerTrack,
                       int nSectors, int secOffset)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_RAMDEVCREATE_INDEX))
        return (VCS_BLK_DEV *)OsFileSys_ReturnCodeTable[OSFILESYS_RAMDEVCREATE_INDEX].Value;

    return (VCS_BLK_DEV *)1;
}


VCS_device_t VCS_xbdBlkDevCreateSync  (VCS_BLK_DEV *bd, const char *name)
{
    if (OsFileSys_UseReturnCode(OSFILESYS_XBDBLKDEVCREATESYNC_INDEX))
        return (VCS_device_t)OsFileSys_ReturnCodeTable[OSFILESYS_XBDBLKDEVCREATESYNC_INDEX].Value;

    return (VCS_device_t)1;
}

void VCS_memset(void *ptr, int c, unsigned int size)
{
    memset(ptr, c, size);
}

int VCS_snprintf(char *buf, unsigned int sz, const char *format, ...)
{
    va_list va;
    int ret;

    va_start(va, format);
    ret = vsnprintf(buf, sz, format, va);
    va_end(va);

    return ret;
}

char *VCS_strcat(char *dest, const char *src)
{
    return (strcat(dest, src));
}

char *VCS_strncat(char *dest, const char *src, unsigned int size)
{
    return (strncat(dest, src, size));
}




