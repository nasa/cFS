/*
 * File: ostimer_stubs.c
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 *
 * Modification History:
 *   06/29/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 *
 */

#include <string.h>

#include "osapi.h"              /* cfe.h not available from within osal. */

#include "ostimer_stubs.h"


OsTimer_HookTable_t           OsTimer_HookTable;
OsTimer_ReturnCodeTable_t     OsTimer_ReturnCodeTable[OSTIMER_MAX_INDEX];

char testname[] = "testname";
int32 nSemTake            = 0;
int32 nSemGive            = 0;

uint32 callbackCount;

VCS_SEM_ID VCS_semMCreate(int options)
{
    if (OsTimer_UseReturnCode(OSTIMER_SEMMCREATE_INDEX))
        return (VCS_SEM_ID)OsTimer_ReturnCodeTable[OSTIMER_SEMMCREATE_INDEX].Value;

    return NULL;
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

int32 getNSemTake()
{
    return nSemTake;
}

int32 getNSemGive()
{
    return nSemGive;
}

uint32 OS_FindCreator(void)
{
    if (OsTimer_UseReturnCode(OSTIMER_OS_FINDCREATOR_INDEX))
        return (uint32)OsTimer_ReturnCodeTable[OSTIMER_OS_FINDCREATOR_INDEX].Value;

    return 0;
}

int VCS_timer_create (VCS_clockid_t clock_id,
             struct VCS_sigevent *evp,
             VCS_timer_t *timerid)
{
    if (OsTimer_UseReturnCode(OSTIMER_TIMER_CREATE_INDEX))
        return (int)OsTimer_ReturnCodeTable[OSTIMER_TIMER_CREATE_INDEX].Value;

    return 0;
}

int VCS_timer_delete (VCS_timer_t __timerid)
{
    if (OsTimer_UseReturnCode(OSTIMER_TIMER_DELETE_INDEX))
        return (int)OsTimer_ReturnCodeTable[OSTIMER_TIMER_DELETE_INDEX].Value;

    return 0;
}
int VCS_timer_connect (VCS_timer_t timerid, VCS_VOIDFUNCPTR routine, int possible_id)
{
    if (OsTimer_UseReturnCode(OSTIMER_TIMER_CONNECT_INDEX))
        return (int)OsTimer_ReturnCodeTable[OSTIMER_TIMER_CONNECT_INDEX].Value;

    return 0;
}

int VCS_timer_settime (VCS_timer_t timerid, int flags,
                const struct VCS_itimerspec *value,
                struct VCS_itimerspec * ovalue)
{
    if (OsTimer_UseReturnCode(OSTIMER_TIMER_SETTIME_INDEX))
        return (int)OsTimer_ReturnCodeTable[OSTIMER_TIMER_SETTIME_INDEX].Value;

    return 0;
}

void OsTimer_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < OSTIMER_MAX_INDEX) {
        OsTimer_ReturnCodeTable[Index].Value = RtnVal;
        OsTimer_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", (unsigned int)Index);
    }
}


boolean OsTimer_UseReturnCode(uint32 Index)
{
    if (OsTimer_ReturnCodeTable[Index].Count > 0) {
        OsTimer_ReturnCodeTable[Index].Count--;
        if (OsTimer_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}


void OsTimer_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == OSTIMER_SEMMCREATE_INDEX)      { OsTimer_HookTable.semMCreate = FunPtr; }
    else if (Index == OSTIMER_SEMTAKE_INDEX)         { OsTimer_HookTable.semTake = FunPtr; }
    else if (Index == OSTIMER_SEMGIVE_INDEX)         { OsTimer_HookTable.semGive = FunPtr; }
    else if (Index == OSTIMER_CLOCKGETRES_INDEX)     { OsTimer_HookTable.clock_getres = FunPtr; }
    else if (Index == OSTIMER_OS_FINDCREATOR_INDEX)  { OsTimer_HookTable.OS_FindCreator = FunPtr; }
    else
    {
        printf("Unsupported OsTimer Index In SetFunctionHook Call %u\n", (unsigned int)Index);
    }
}


void OsTimer_Reset(void)
{
    memset(&OsTimer_HookTable, 0, sizeof(OsTimer_HookTable));
    memset(&OsTimer_ReturnCodeTable, 0, sizeof(OsTimer_ReturnCodeTable));
    nSemTake = 0;
    nSemGive = 0;
}

int VCS_clock_getres(VCS_clockid_t clock, struct VCS_timespec *clock_resolution)
{
    if (OsTimer_UseReturnCode(OSTIMER_CLOCKGETRES_INDEX)) {
        return (int)OsTimer_ReturnCodeTable[OSTIMER_CLOCKGETRES_INDEX].Value;
    }
    clock_resolution->tv_sec = 0;
    clock_resolution->tv_nsec = 1000;
    return 0;
}

void FakeTimerCallback(uint32 timer_id) {
    callbackCount++;
}

void ResetFakeTimerCallback(void) {
    callbackCount = 0;
}

uint32 GetFakeTimerCallbackCount() {
    return callbackCount;
}

/* C library calls that are just straight-through wrappers in this test */
size_t VCS_strlen(const char *str)
{
    return strlen(str);
}

int VCS_strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

int VCS_strncmp(const char *str1, const char *str2, size_t n)
{
    return strncmp(str1, str2, n);
}

char *VCS_strcpy(char *str1, const char *str2)
{
    return strcpy(str1, str2);
}

char *VCS_strncpy(char *str1, const char *str2, size_t n)
{
    return strncpy(str1, str2, n);
}

void VCS_memset(void *ptr, int c, unsigned int size)
{
    memset(ptr, c, size);
}


