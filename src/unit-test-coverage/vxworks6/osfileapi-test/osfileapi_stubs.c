/*
 * File: osfileapu_stubs.c
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 *
 * Modification History:
 *   05/28/2015  Alan Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */

#include <string.h>

#include "osapi.h"              /* cfe.h not available from within osal. */

#include "osfileapi_stubs.h"

int VCS_errno;

OsFileApi_HookTable_t           OsFileApi_HookTable;
OsFileApi_ReturnCodeTable_t     OsFileApi_ReturnCodeTable[OSFILEAPI_MAX_INDEX];


const char testFileName[] = "/test";
int32 nSemTake            = 0;
int32 nSemGive            = 0;
int32 nRewinddirCalls     = 0;

void OsFileApi_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < OSFILEAPI_MAX_INDEX) {
        OsFileApi_ReturnCodeTable[Index].Value = RtnVal;
        OsFileApi_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", (unsigned int)Index);
    }
}


boolean OsFileApi_UseReturnCode(uint32 Index)
{
    if (OsFileApi_ReturnCodeTable[Index].Count > 0) {
        OsFileApi_ReturnCodeTable[Index].Count--;
        if (OsFileApi_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}


uint32 OsFileApi_CheckCount(uint32 Index)
{
    return (OsFileApi_ReturnCodeTable[Index].Count);
}

void OsFileApi_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == OSFILEAPI_STRLEN_INDEX)          { OsFileApi_HookTable.strlen = FunPtr; }
    else if (Index == OSFILEAPI_STRCMP_INDEX)          { OsFileApi_HookTable.strcmp = FunPtr; }
    else if (Index == OSFILEAPI_OPEN_INDEX)            { OsFileApi_HookTable.open = FunPtr; }
    else if (Index == OSFILEAPI_SEMMCREATE_INDEX)      { OsFileApi_HookTable.semMCreate = FunPtr; }
    else if (Index == OSFILEAPI_SEMTAKE_INDEX)         { OsFileApi_HookTable.semTake = FunPtr; }
    else if (Index == OSFILEAPI_SEMGIVE_INDEX)         { OsFileApi_HookTable.semGive = FunPtr; }
    else
    {
        printf("Unsupported OsFileApi Index In SetFunctionHook Call %u\n", (unsigned int)Index);
    }
}


void OsFileApi_Reset(void)
{
    memset(&OsFileApi_HookTable, 0, sizeof(OsFileApi_HookTable));
    memset(&OsFileApi_ReturnCodeTable, 0, sizeof(OsFileApi_ReturnCodeTable));
    nSemTake = 0;
    nSemGive = 0;
    nRewinddirCalls = 0;
}

int32 getNSemTake()
{
    return nSemTake;
}

int32 getNSemGive()
{
    return nSemGive;
}

int32 getRewinddirCalls()
{
    return nRewinddirCalls;
}

size_t VCS_strlen(const char *str)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_STRLEN_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_STRLEN_INDEX].Value;

    return strlen(str);
}

int VCS_strcmp(const char *str1, const char *str2)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_STRCMP_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_STRCMP_INDEX].Value;

    return strcmp(str1, str2);
}

char *VCS_strcpy(char *str1, const char *str2)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_STRCPY_INDEX))
        return (char *)OsFileApi_ReturnCodeTable[OSFILEAPI_STRCPY_INDEX].Value;

    return strcpy(str1, str2);
}

char *VCS_strncpy(char *str1, const char *str2, size_t size)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_STRNCPY_INDEX))
        return (char *)OsFileApi_ReturnCodeTable[OSFILEAPI_STRNCPY_INDEX].Value;

    return strncpy(str1, str2, size);
}

char *VCS_strrchr(const char *string, int c)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_STRRCHR_INDEX))
    {
        return (char *)OsFileApi_ReturnCodeTable[OSFILEAPI_STRRCHR_INDEX].Value;
    }

    return strrchr(string, c);
}

char *VCS_strchr(const char *string, int c)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_STRCHR_INDEX))
    {
        return (char *)OsFileApi_ReturnCodeTable[OSFILEAPI_STRCHR_INDEX].Value;
    }

    return strchr(string, c);
}


int VCS_open(const char *filename, int oflag, ...)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_OPEN_INDEX))
    {
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_OPEN_INDEX].Value;
    }
    else if (strcmp(filename, testFileName) == 0)
    {
        return oflag; /* to allow verification that flags is set correctly */
    }

    return VCS_OK;
    //return open(filename, 0, 0);
}

VCS_STATUS VCS_close (int fd)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_CLOSE_INDEX)) {
	/*printf("Inside VCS_close(), returning table value.\n");*/
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_CLOSE_INDEX].Value;
    }
    else if (OsFileApi_ReturnCodeTable[OSFILEAPI_CLOSE_INDEX].Count > 0) /* deal with multiple calls */
    {
	/*printf("Inside VCS_close(), faking VCS_OK. \n");*/
        return VCS_OK;
    }

    /*printf("Inside VCS_close(), calling close() !!!!\n");*/
    return VCS_OK;
    //return close(fd);
}

int VCS_read (int fd, char *buffer, int maxbytes)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_READ_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_READ_INDEX].Value;

    return 0;
    //return read(fd, buffer, maxbytes);
}

int VCS_write(int fd, char *buffer, int nbytes)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_WRITE_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_WRITE_INDEX].Value;

    return 0;
    //return write(fd, buffer, nbytes);
}


int VCS_lseek(int fd, long offset, int whence)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_LSEEK_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_LSEEK_INDEX].Value;
    else if (offset < 0)
    {
        return whence;
    }

    return 0; //lseek(fd, offset, whence);
}


VCS_STATUS VCS_mkdir(const char *dirName)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_MKDIR_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_MKDIR_INDEX].Value;

    return VCS_OK; //mkdir(dirName, 0);
}

VCS_STATUS VCS_rmdir(const char *dirName)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_RMDIR_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_RMDIR_INDEX].Value;

    return VCS_OK; //rmdir(dirName);
}

VCS_DIR *VCS_opendir(const char * dirName)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_OPENDIR_INDEX))
            return (VCS_DIR *)OsFileApi_ReturnCodeTable[OSFILEAPI_OPENDIR_INDEX].Value;

    return NULL; //(DIR *)dirName;
}

int VCS_closedir(VCS_DIR * dirp)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_CLOSEDIR_INDEX))
            return OsFileApi_ReturnCodeTable[OSFILEAPI_CLOSEDIR_INDEX].Value;

    return 0;
}

struct VCS_dirent *VCS_readdir (VCS_DIR *__dirp)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_READDIR_INDEX))
            return (struct VCS_dirent *)OsFileApi_ReturnCodeTable[OSFILEAPI_READDIR_INDEX].Value;

    return NULL;
}

void VCS_rewinddir(VCS_DIR *dirp)
{
    nRewinddirCalls += 1;
}

int VCS_stat(const char *path, struct VCS_stat *buf)
{
    //struct stat stbuf;
    if (OsFileApi_UseReturnCode(OSFILEAPI_STAT_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_STAT_INDEX].Value;

    return VCS_OK; //stat(path, &stbuf);
}

int VCS_rename(const char *old_filename, const char *new_filename)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_RENAME_INDEX))
        return (int)OsFileApi_ReturnCodeTable[OSFILEAPI_RENAME_INDEX].Value;

    return VCS_OK; //rename(old_filename, new_filename);
}

int VCS_remove(const char *filename)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_REMOVE_INDEX))
        return (int)OsFileApi_ReturnCodeTable[OSFILEAPI_REMOVE_INDEX].Value;

    return VCS_OK; //remove(filename);
}

VCS_STATUS VCS_cp(const char *src, const char *dest)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_CP_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_CP_INDEX].Value;

    return 0;
}

VCS_SEM_ID VCS_semMCreate(int options)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_SEMMCREATE_INDEX))
        return (VCS_SEM_ID)OsFileApi_ReturnCodeTable[OSFILEAPI_SEMMCREATE_INDEX].Value;

    return (VCS_SEM_ID)NULL;
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

int VCS_shellGenericInit(const char *arg1, const char *arg2, const char *arg3, char **arg4,
                     int arg5, int arg6, int arg7, int arg8, int arg9)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_SHELLGENERICINIT_INDEX))
    {
        /*printf("Inside linux fake shellGenericInit(), returning table value.\n");*/
        return OsFileApi_ReturnCodeTable[OSFILEAPI_SHELLGENERICINIT_INDEX].Value;
    }

    return VCS_OK;
}

#if 0
int shellGenericInit(const char *arg1, const char *arg2, const char *arg3, char **arg4,
                     int arg5, int arg6, int arg7, int arg8, int arg9);
int VCS_shellGenericInit(const char *arg1, const char *arg2, const char *arg3, char **arg4,
                     int arg5, int arg6, int arg7, int arg8, int arg9)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_SHELLGENERICINIT_INDEX)) 
    {
	/*printf("Inside VCS_shellGenericInit(), returning table value.\n");*/
        return OsFileApi_ReturnCodeTable[OSFILEAPI_SHELLGENERICINIT_INDEX].Value;
    }

    /*printf("Inside VCS_shellGenericInit(), calling shellGenericInit()!!!\n");*/
    return shellGenericInit(arg1, arg2, arg3, arg4,
                     arg5, arg6, arg7, arg8, arg9);
}
#endif

VCS_STATUS VCS_taskDelay(int ticks)
{
    return VCS_ERROR;
}

int VCS_taskNameToId(char *name)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_TASKNAMETOID_INDEX))
        return OsFileApi_ReturnCodeTable[OSFILEAPI_TASKNAMETOID_INDEX].Value;

    return VCS_ERROR;
}

int VCS_sysClkRateGet(void)
{
    return 1;
}

int32 OS_TranslatePath(const char* VirtualPath, char* LocalPath)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_OSTRANSLATEPATH_INDEX))
        return OsFileApi_ReturnCodeTable[OSFILEAPI_OSTRANSLATEPATH_INDEX].Value;

    strcpy(LocalPath, VirtualPath);

    return OS_FS_SUCCESS;
}

uint32 OS_FindCreator(void)
{
    if (OsFileApi_UseReturnCode(OSFILEAPI_OSFINDCREATOR_INDEX))
        return (size_t)OsFileApi_ReturnCodeTable[OSFILEAPI_OSFINDCREATOR_INDEX].Value;

    return 0;
}

