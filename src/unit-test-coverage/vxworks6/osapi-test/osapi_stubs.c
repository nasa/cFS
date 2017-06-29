/*
 * File: osapi_stubs.c
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 *
 * Modification History:
 *   06/25/2015  Alan Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */

#include <string.h>

#include "osapi_stubs.h"
#include "osapi_adaptor.h"

extern VCS_SEM_ID OS_task_table_sem;
extern VCS_SEM_ID OS_queue_table_sem;
extern VCS_SEM_ID OS_bin_sem_table_sem;
extern VCS_SEM_ID OS_count_sem_table_sem;
extern VCS_SEM_ID OS_mut_sem_table_sem;

VCS_PART_ID VCS_memSysPartId;
int VCS_errno;

OsApi_HookTable_t           OsApi_HookTable;
OsApi_ReturnCodeTable_t     OsApi_ReturnCodeTable[OSAPI_MAX_INDEX];


int32         nSemTake[OS_MAX_SEM] = {0};
int32         nSemGive[OS_MAX_SEM] = {0};
int32         nTaskDeleteForce     = 0;
int           taskSpawnFlags       = 0;
boolean       exitCalled           = FALSE;
int           taskDelayTicks       = 0;
int           msgQReceiveTimeout   = -2;
uint32        semBInitialValue     = 99;
VCS_SEM_ID        semIdCalled          = 0;
int32         semTakeTicks         = 0;
uint32        time_sec             = 0;
uint32        time_nsec            = 0;
unsigned long heap_freeBytes       = 0;
unsigned long heap_freeBlocks      = 0;
unsigned long heap_maxFreeBlock    = 0;
uint32        fpuMask              = 0;
uint32        nVsnprinf_calls      = 0;
uint32        nLogMsg_calls        = 0;


void OsApi_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < OSAPI_MAX_INDEX) {
        OsApi_ReturnCodeTable[Index].Value = RtnVal;
        OsApi_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", (unsigned int)Index);
    }
}


boolean OsApi_UseReturnCode(uint32 Index)
{
    if (OsApi_ReturnCodeTable[Index].Count > 0) {
        OsApi_ReturnCodeTable[Index].Count--;
        if (OsApi_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}


void OsApi_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == OSAPI_STRLEN_INDEX)          { OsApi_HookTable.strlen = FunPtr; }
    else if (Index == OSAPI_STRCMP_INDEX)          { OsApi_HookTable.strcmp = FunPtr; }
    else if (Index == OSAPI_SEMMCREATE_INDEX)      { OsApi_HookTable.semMCreate = FunPtr; }
    else if (Index == OSAPI_SEMTAKE_INDEX)         { OsApi_HookTable.semTake = FunPtr; }
    else if (Index == OSAPI_SEMGIVE_INDEX)         { OsApi_HookTable.semGive = FunPtr; }
    else
    {
        printf("Unsupported OsApi Index In SetFunctionHook Call %u\n", (unsigned int)Index);
    }
}


void OsApi_Reset(void)
{
    uint32 ii = 0;

    memset(&OsApi_HookTable, 0, sizeof(OsApi_HookTable));
    memset(&OsApi_ReturnCodeTable, 0, sizeof(OsApi_ReturnCodeTable));

    for (ii = 0; ii < OS_MAX_SEM; ++ii)
    {
        nSemTake[ii] = 0;
        nSemGive[ii] = 0;
    }
    nTaskDeleteForce   = 0;
    taskSpawnFlags     = -1;
    exitCalled         = FALSE;
    taskDelayTicks     = 0;
    msgQReceiveTimeout = -2;
    semBInitialValue   = 99;
    semIdCalled        = 0;
    semTakeTicks       = -1;
    time_sec           = 0;
    time_nsec          = 0;
    heap_freeBytes     = 0;
    heap_freeBlocks    = 0;
    heap_maxFreeBlock  = 0;
    fpuMask            = 0;
    nVsnprinf_calls    = 0;
    nLogMsg_calls      = 0;
}


int32 getNSemTake(uint32 id)
{
    if (id < OS_MAX_SEM)
    {
        return nSemTake[id];
    }

    return 0;
}

int32 getSemTakeTicks()
{
    return semTakeTicks;
}


int32 getNSemGive(uint32 id)
{
    if (id < OS_MAX_SEM)
    {
        return nSemGive[id];
    }

    return 0;
}

VCS_SEM_ID getSemGiveArg()
{
    return semIdCalled;
}


int32 getNTaskDeleteForce()
{
    return nTaskDeleteForce;
}


int getTaskSpawnFlags()
{
    return taskSpawnFlags;
}

boolean getTaskExitCalled()
{
    return exitCalled;
}

int getTaskDelayTicks()
{
    return taskDelayTicks;
}

int getMsgQReceiveTimeout()
{
    return msgQReceiveTimeout;
}

uint32 getSemBInitialValue()
{
    return semBInitialValue;
}

uint32 getSeconds()
{
    return time_sec;
}

uint32 getNSeconds()
{
    return time_nsec;
}

void setTime(uint32 sec, uint32 nsec)
{
    time_sec  = sec;
    time_nsec = nsec;
}

void setHeapInfo(unsigned long freeBytes, unsigned long freeBlocks, unsigned long maxFreeBlock)
{
    heap_freeBytes    = freeBytes;
    heap_freeBlocks   = freeBlocks;
    heap_maxFreeBlock = maxFreeBlock;
}

uint32 getFpuMask()
{
    return fpuMask;
}

uint32 getNVsnprintf_calls()
{
    return nVsnprinf_calls;
}

uint32 getNLogMsg_calls()
{
    return nLogMsg_calls;
}



/* Standard C library functions */
void VCS_exit(int code)
{
    if (OsApi_UseReturnCode(OSAPI_EXIT_INDEX))
    {
        exitCalled = TRUE;
    }

}

size_t VCS_strlen(const char *str)
{
    if (OsApi_UseReturnCode(OSAPI_STRLEN_INDEX))
        return (size_t)OsApi_ReturnCodeTable[OSAPI_STRLEN_INDEX].Value;

    return strlen(str);
}

int VCS_strcmp(const char *str1, const char *str2)
{
    if (OsApi_UseReturnCode(OSAPI_STRCMP_INDEX))
        return (size_t)OsApi_ReturnCodeTable[OSAPI_STRCMP_INDEX].Value;

    return strcmp(str1, str2);
}

char *VCS_strcpy(char *str1, const char *str2)
{
    if (OsApi_UseReturnCode(OSAPI_STRCPY_INDEX))
        return (char *)OsApi_ReturnCodeTable[OSAPI_STRCPY_INDEX].Value;

    return strcpy(str1, str2);
}

int VCS_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    nVsnprinf_calls++;

    return vsnprintf(str, size, format, ap);
}


/* VxWorks functions */
int VCS_clock_gettime(VCS_clockid_t clock_id, struct VCS_timespec *tp)
{
    tp->tv_sec  = time_sec;
    tp->tv_nsec = time_nsec;

    if (OsApi_UseReturnCode(OSAPI_CLOCKGETTIME_INDEX))
    {
        return (int)OsApi_ReturnCodeTable[OSAPI_CLOCKGETTIME_INDEX].Value;
    }

    return 1;
}


int VCS_clock_settime(VCS_clockid_t clock_id, const struct VCS_timespec * tp)
{
    time_sec  = tp->tv_sec;
    time_nsec = tp->tv_nsec;

    if (OsApi_UseReturnCode(OSAPI_CLOCKSETTIME_INDEX))
    {
        return (int)OsApi_ReturnCodeTable[OSAPI_CLOCKSETTIME_INDEX].Value;
    }

    return 1;
}



VCS_STATUS VCS_intConnect(VCS_VOIDFUNCPTR *vector, VCS_VOIDFUNCPTR routine, int parameter)
{
    if (OsApi_UseReturnCode(OSAPI_INTCONNECT_INDEX))
    {
        return (int)OsApi_ReturnCodeTable[OSAPI_INTCONNECT_INDEX].Value;
    }

    return 1;
}


int VCS_intDisable(int level)
{
    if (OsApi_UseReturnCode(OSAPI_INTDISABLE_INDEX))
    {
        return (int)OsApi_ReturnCodeTable[OSAPI_INTDISABLE_INDEX].Value;
    }

    return 1;
}


int VCS_intEnable(int level)
{
    if (OsApi_UseReturnCode(OSAPI_INTENABLE_INDEX))
    {
        return (int)OsApi_ReturnCodeTable[OSAPI_INTENABLE_INDEX].Value;
    }

    return 1;
}


int VCS_intLock(void)
{
    if (OsApi_UseReturnCode(OSAPI_INTLOCK_INDEX))
    {
        return (int)OsApi_ReturnCodeTable[OSAPI_INTLOCK_INDEX].Value;
    }

    return 1;
}


int VCS_intUnlock(int lockKey)
{
    if (OsApi_UseReturnCode(OSAPI_INTUNLOCK_INDEX))
    {
        return (int)OsApi_ReturnCodeTable[OSAPI_INTUNLOCK_INDEX].Value;
    }

    return 1;
}


int VCS_logMsg(char * fmt, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
{
    nLogMsg_calls++;

    return 0;
}

VCS_STATUS VCS_memPartShow(VCS_PART_ID partId, int type)
{
    return 1;
}


VCS_STATUS VCS_memPartInfoGet(VCS_PART_ID partId, VCS_MEM_PART_STATS * ppartStats)
{
    if (ppartStats != NULL)
    {
        ppartStats->numBytesFree     = heap_freeBytes;
        ppartStats->numBlocksFree    = heap_freeBlocks;
        ppartStats->maxBlockSizeFree = heap_maxFreeBlock;
    }

    if (OsApi_UseReturnCode(OSAPI_MEMPARTINFOGET_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_MEMPARTINFOGET_INDEX].Value;
    }

    return 1;
}


VCS_MSG_Q_ID VCS_msgQCreate(int maxMsgs, int maxMsgLength, int options)
{
    if (OsApi_UseReturnCode(OSAPI_MSGQCREATE_INDEX))
    {
        return (VCS_MSG_Q_ID)OsApi_ReturnCodeTable[OSAPI_MSGQCREATE_INDEX].Value;
    }

    return (VCS_MSG_Q_ID)1;
}


VCS_STATUS VCS_msgQDelete(VCS_MSG_Q_ID msgQId)
{
    if (OsApi_UseReturnCode(OSAPI_MSGQDELETE_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_MSGQDELETE_INDEX].Value;
    }

    return VCS_OK;
}


int VCS_msgQReceive(VCS_MSG_Q_ID msgQId, char * buffer, uint32 maxNBytes, int timeout)
{
    msgQReceiveTimeout = timeout;

    if (OsApi_UseReturnCode(OSAPI_MSGQRECEIVE_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_MSGQRECEIVE_INDEX].Value;
    }

    return 1;
}


VCS_STATUS VCS_msgQSend(VCS_MSG_Q_ID msgQId, char *buffer, uint32 nBytes, int timeout, int priority)
{
    if (OsApi_UseReturnCode(OSAPI_MSGQSEND_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_MSGQSEND_INDEX].Value;
    }

    return VCS_OK;
}


VCS_SEM_ID VCS_semBCreate(int options, VCS_SEM_B_STATE initialState)
{
    semBInitialValue = initialState;

    if (OsApi_UseReturnCode(OSAPI_SEMBCREATE_INDEX))
    {
        return (VCS_SEM_ID)OsApi_ReturnCodeTable[OSAPI_SEMBCREATE_INDEX].Value;
    }

    return (VCS_SEM_ID)1;
}


VCS_SEM_ID VCS_semCCreate(int flags, int count)
{
    if (OsApi_UseReturnCode(OSAPI_SEMCCREATE_INDEX))
    {
        return (VCS_SEM_ID)OsApi_ReturnCodeTable[OSAPI_SEMCCREATE_INDEX].Value;
    }

    return (VCS_SEM_ID)1;
}


VCS_STATUS VCS_semDelete(VCS_SEM_ID semId)
{
    if (OsApi_UseReturnCode(OSAPI_SEMDELETE_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_SEMDELETE_INDEX].Value;
    }

    return VCS_OK;
}


VCS_STATUS VCS_semFlush(VCS_SEM_ID semId)
{
    if (OsApi_UseReturnCode(OSAPI_SEMFLUSH_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_SEMFLUSH_INDEX].Value;
    }

    return VCS_OK;
}


VCS_SEM_ID VCS_semMCreate(int options)
{
    if (OsApi_UseReturnCode(OSAPI_SEMMCREATE_INDEX))
    {
        return (VCS_SEM_ID)OsApi_ReturnCodeTable[OSAPI_SEMMCREATE_INDEX].Value;
    }

    return (VCS_SEM_ID)1;
}


uint32 getSemIdIndex(VCS_SEM_ID semId)
{
    uint32 idx = 0;

    if (semId == NULL)
    {
        return idx;
    }

    if      (semId == OsApi_Adaptor_Get_Os_Task_Table_Sem())
    {
        idx = OS_TASK_TABLE_SEM;
    }
    else if (semId == OsApi_Adaptor_Get_Os_Queue_Table_Sem())
    {
        idx = OS_QUEUE_TABLE_SEM;
    }
    else if (semId == OsApi_Adaptor_Get_Os_Bin_Sem_Table_Sem())
    {
        idx = OS_BIN_SEM_TABLE_SEM;
    }
    else if (semId == OsApi_Adaptor_Get_Os_Count_Sem_Table_Sem())
    {
        idx = OS_COUNT_SEM_TABLE_SEM;
    }
    else if (semId == OsApi_Adaptor_Get_Os_Mut_Sem_Table_Sem())
    {
        idx = OS_MUT_SEM_TABLE_SEM;
    }

    return idx;
}


VCS_STATUS VCS_semTake(VCS_SEM_ID semId, int timeout)
{
    semTakeTicks = timeout;

    if (OsApi_UseReturnCode(OSAPI_SEMTAKE_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_SEMTAKE_INDEX].Value;
    }

    if (semId != NULL)
    {
        nSemTake[getSemIdIndex(semId)]++;
    }

    return VCS_OK;
}


VCS_STATUS VCS_semGive(VCS_SEM_ID semId)
{
    semIdCalled = semId;

    if (OsApi_UseReturnCode(OSAPI_SEMGIVE_INDEX))
    {
        return (VCS_STATUS)OsApi_ReturnCodeTable[OSAPI_SEMGIVE_INDEX].Value;
    }

    if (semId != NULL)
    {
        nSemGive[getSemIdIndex(semId)]++;
    }

    return VCS_OK;
}


int VCS_sysClkRateGet(void)
{
    if (OsApi_UseReturnCode(OSAPI_SYSCLKRATEGET_INDEX))
    {
        return OsApi_ReturnCodeTable[OSAPI_SYSCLKRATEGET_INDEX].Value;
    }

    return 1;
}


VCS_STATUS VCS_taskDelay(int ticks)
{
    taskDelayTicks = ticks;

    if (OsApi_UseReturnCode(OSAPI_TASKDELAY_INDEX))
    {
        return OsApi_ReturnCodeTable[OSAPI_TASKDELAY_INDEX].Value;
    }

    return VCS_OK;
}


VCS_STATUS VCS_taskDelete(int tid)
{
    if (OsApi_UseReturnCode(OSAPI_TASKDELETE_INDEX))
    {
        return OsApi_ReturnCodeTable[OSAPI_TASKDELETE_INDEX].Value;
    }

    return VCS_OK;
}


VCS_STATUS VCS_taskDeleteForce(int tid)
{
    nTaskDeleteForce++;

    return VCS_OK;
}


void VCS_taskExit(int code)
{
    exitCalled = TRUE;
}


int VCS_taskIdSelf(void)
{
    if (OsApi_UseReturnCode(OSAPI_TASKIDSELF_INDEX))
    {
        return OsApi_ReturnCodeTable[OSAPI_TASKIDSELF_INDEX].Value;
    }

    return 0;
}


VCS_STATUS VCS_taskPrioritySet(int tid, int newPriority)
{
    if (OsApi_UseReturnCode(OSAPI_TASKPRIORITYSET_INDEX))
    {
        return OsApi_ReturnCodeTable[OSAPI_TASKPRIORITYSET_INDEX].Value;
    }

    return VCS_OK;
}


int VCS_taskSpawn(char *  name,
              int     priority,
              int     options,
              int     stackSize,
              VCS_FUNCPTR entryPt,
              int     arg1,
              int     arg2,
              int     arg3,
              int     arg4,
              int     arg5,
              int     arg6,
              int     arg7,
              int     arg8,
              int     arg9,
              int     arg10)
{
    taskSpawnFlags = options;

    if (OsApi_UseReturnCode(OSAPI_TASKSPAWN_INDEX))
        return OsApi_ReturnCodeTable[OSAPI_TASKSPAWN_INDEX].Value;

    return 1;
}


VCS_STATUS VCS_taskVarAdd(int tid, int *pVar)
{
    if (OsApi_UseReturnCode(OSAPI_TASKVARADD_INDEX))
        return OsApi_ReturnCodeTable[OSAPI_TASKVARADD_INDEX].Value;

    return VCS_OK;
}

uint32 VCS_vxFpscrGet()
{
    if (OsApi_UseReturnCode(OSAPI_VXFPSCRGET_INDEX))
        return OsApi_ReturnCodeTable[OSAPI_VXFPSCRGET_INDEX].Value;

    return 1;
}

void VCS_vxFpscrSet (uint32 mask)
{
    fpuMask = mask;
}

void VCS_memset(void *ptr, int c, unsigned int size)
{
    memset(ptr, c, size);
}

int VCS_printf(const char *format, ...)
{
    va_list va;
    int ret;

    va_start(va, format);
    ret = vprintf(format, va);
    va_end(va);

    return ret;
}

VCS_SEM_ID VCS_semBInitialize(char *pSemMem, int options, VCS_SEM_B_STATE initialState)
{
    return (VCS_SEM_ID)pSemMem;
}


/* OSAL functions */
int32 OS_FS_Init(void)
{
    if (OsApi_UseReturnCode(OSAPI_OSFSINIT_INDEX))
        return OsApi_ReturnCodeTable[OSAPI_OSFSINIT_INDEX].Value;

    return 0;
}


int32 OS_ModuleTableInit(void)
{
    if (OsApi_UseReturnCode(OSAPI_OSMODULETABLEINIT_INDEX))
        return OsApi_ReturnCodeTable[OSAPI_OSMODULETABLEINIT_INDEX].Value;

    return 1;
}


int32 OS_TimerAPIInit(void)
{
    if (OsApi_UseReturnCode(OSAPI_OSTIMERAPIINIT_INDEX))
        return OsApi_ReturnCodeTable[OSAPI_OSTIMERAPIINIT_INDEX].Value;

    return 1;
}

/* These 3 functions are referenced by OS_DeleteAllObjects() */
int32 OS_TimerDelete(uint32 timer_id)
{
    return 0;
}

int32 OS_ModuleUnload(uint32 module_id)
{
    return 0;
}

int32 OS_close(int32  filedes)
{
    return 0;
}


