/*
 * File: osapi_stubs.h
 *
 * Purpose:
 *   Provide stubs for unit testing
 *
 * History:
 *   06/25/2015  A. Asp, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSAPI_STUBS_H_
#define _OSAPI_STUBS_H_

#include "common_types.h"
#include "uttools.h"
#include "vxworks6-coverage-stubs.h"

/* Define test enums and structs */
typedef enum
{
    OS_TASK_TABLE_SEM,
    OS_QUEUE_TABLE_SEM,
    OS_BIN_SEM_TABLE_SEM,
    OS_COUNT_SEM_TABLE_SEM,
    OS_MUT_SEM_TABLE_SEM,
    OS_MAX_SEM
} OsApi_Semaphore_t;


typedef enum
{
    OSAPI_EXIT_INDEX,
    OSAPI_STRLEN_INDEX,
    OSAPI_STRCMP_INDEX,
    OSAPI_STRCPY_INDEX,
    OSAPI_CLOCKGETTIME_INDEX,
    OSAPI_CLOCKSETTIME_INDEX,
    OSAPI_INTCONNECT_INDEX,
    OSAPI_INTDISABLE_INDEX,
    OSAPI_INTENABLE_INDEX,
    OSAPI_INTLOCK_INDEX,
    OSAPI_INTUNLOCK_INDEX,
    OSAPI_MEMPARTINFOGET_INDEX,
    OSAPI_MSGQCREATE_INDEX,
    OSAPI_MSGQDELETE_INDEX,
    OSAPI_MSGQRECEIVE_INDEX,
    OSAPI_MSGQSEND_INDEX,
    OSAPI_SEMBCREATE_INDEX,
    OSAPI_SEMCCREATE_INDEX,
    OSAPI_SEMDELETE_INDEX,
    OSAPI_SEMFLUSH_INDEX,
    OSAPI_SEMMCREATE_INDEX,
    OSAPI_SEMTAKE_INDEX,
    OSAPI_SEMGIVE_INDEX,
    OSAPI_SYSCLKRATEGET_INDEX,
    OSAPI_TASKDELAY_INDEX,
    OSAPI_TASKDELETE_INDEX,
    OSAPI_TASKIDSELF_INDEX,
    OSAPI_TASKNAMETOID_INDEX,
    OSAPI_TASKPRIORITYSET_INDEX,
    OSAPI_TASKSPAWN_INDEX,
    OSAPI_TASKVARADD_INDEX,
    OSAPI_OSFSINIT_INDEX,
    OSAPI_OSMODULETABLEINIT_INDEX,
    OSAPI_OSTIMERAPIINIT_INDEX,
    OSAPI_VXFPSCRGET_INDEX,
    OSAPI_MAX_INDEX
} OsApi_Index_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} OsApi_ReturnCodeTable_t;

typedef struct
{
    size_t  (*strlen)(const char *str);
    int     (*strcmp)(const char *str1, const char *str2);
    char *  (*strcpy)(const char *str1, const char *str2);
    VCS_SEM_ID  (*semMCreate)(uint16 *);
    VCS_STATUS  (*semTake)(VCS_SEM_ID, int);
    VCS_STATUS  (*semGive)(VCS_SEM_ID);
    VCS_STATUS  (*taskDelay)(int ticks);
    int     (*taskNameToId)(char *name);
    int     (*sysClkRateGet)(void);
    uint32  (*OS_FindCreator)(void);
} OsApi_HookTable_t;


void OsApi_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void OsApi_SetFunctionHook(uint32 Index, void *FunPtr);
void OsApi_Reset(void);

uint32  getFpuMask(void);
int     getMsgQReceiveTimeout(void);
uint32  getNLogMsg_calls(void);
uint32  getNSeconds(void);
int32   getNSemTake(uint32 id);
int32   getNSemGive(uint32 id);
int32   getNTaskDeleteForce(void);
uint32  getNVsnprintf_calls(void);
uint32  getSeconds(void);
uint32  getSemBInitialValue(void);
VCS_SEM_ID  getSemGiveArg(void);
int32   getSemTakeTicks(void);
int     getTaskDelayTicks(void);
boolean getTaskExitCalled(void);
int     getTaskSpawnFlags(void);

void    setHeapInfo(unsigned long freeBytes, unsigned long freeBlocks, unsigned long maxFreeBlock);
void    setTime(uint32 sec, uint32 nsec);


#endif
