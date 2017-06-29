/*
 * File: osfileapi_stubs.h
 *
 * Purpose:
 *   Provide stubs for unit testing
 *
 * History:
 *   05/28/2015  A. Asp, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSFILEAPI_STUBS_H_
#define _OSFILEAPI_STUBS_H_

#include "uttools.h"
#include "vxworks6-coverage-stubs.h"


/* Define missing types */
typedef struct
{
    int32 Dir;
} __dirstream;

typedef enum
{
    OSFILEAPI_STRLEN_INDEX,
    OSFILEAPI_STRCMP_INDEX,
    OSFILEAPI_STRCPY_INDEX,
    OSFILEAPI_STRNCPY_INDEX,
    OSFILEAPI_STRRCHR_INDEX,
    OSFILEAPI_STRCHR_INDEX,
    OSFILEAPI_OPEN_INDEX,
    OSFILEAPI_CLOSE_INDEX,
    OSFILEAPI_READ_INDEX,
    OSFILEAPI_WRITE_INDEX,
    OSFILEAPI_LSEEK_INDEX,
    OSFILEAPI_STAT_INDEX,
    OSFILEAPI_REMOVE_INDEX,
    OSFILEAPI_RENAME_INDEX,
    OSFILEAPI_CP_INDEX,
    OSFILEAPI_MKDIR_INDEX,
    OSFILEAPI_RMDIR_INDEX,
    OSFILEAPI_OPENDIR_INDEX,
    OSFILEAPI_CLOSEDIR_INDEX,
    OSFILEAPI_READDIR_INDEX,
    OSFILEAPI_SEMMCREATE_INDEX,
    OSFILEAPI_SEMTAKE_INDEX,
    OSFILEAPI_SEMGIVE_INDEX,
    OSFILEAPI_SHELLGENERICINIT_INDEX,
    OSFILEAPI_TASKDELAY_INDEX,
    OSFILEAPI_TASKNAMETOID_INDEX,
    OSFILEAPI_SYSCLKRATEGET_INDEX,
    OSFILEAPI_OSTRANSLATEPATH_INDEX,
    OSFILEAPI_OSFINDCREATOR_INDEX,
    OSFILEAPI_MAX_INDEX
} OsFileApi_Index_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} OsFileApi_ReturnCodeTable_t;

typedef struct
{
    size_t  (*strlen)(const char *str);
    int     (*strcmp)(const char *str1, const char *str2);
    char *  (*strcpy)(const char *str1, const char *str2);
    char *  (*strncpy)(const char *str1, const char *str2, size_t size);
    int     (*open)(const char *, int, int);
    VCS_STATUS  (*close)(int fd);
    int     (*read)(int fd, char *buffer, size_t maxbytes);
    int     (*write)(int fd, char *buffer, size_t nbytes);
    int     (*lseek)(int fd, long offset, int whence);
    VCS_STATUS  (*mkdir)(const char *dirName);
    VCS_STATUS  (*cp)(const char *src, const char *dest);
    VCS_STATUS  (*rmdir)(const char *dirName);
    VCS_SEM_ID  (*semMCreate)(uint16 *);
    VCS_STATUS  (*semTake)(VCS_SEM_ID, int);
    VCS_STATUS  (*semGive)(VCS_SEM_ID);
    int     (*shellGenericInit)(const char *arg1, const char *arg2, const char *arg3, char **arg4,
                         int arg5, int arg6, int arg7, int arg8, int arg9);
    VCS_STATUS  (*taskDelay)(int ticks);
    int     (*taskNameToId)(char *name);
    int     (*sysClkRateGet)(void);
    int32   (*OS_TranslatePath)(const char* VirtualPath, char* LocalPath);
    uint32  (*OS_FindCreator)(void);
} OsFileApi_HookTable_t;

extern const char testFileName[];


void OsFileApi_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void OsFileApi_SetFunctionHook(uint32 Index, void *FunPtr);
void OsFileApi_Reset(void);
uint32 OsFileApi_CheckCount(uint32 Index);

int32 getNSemTake(void);
int32 getNSemGive(void);
int32 getRewinddirCalls(void);

#endif
