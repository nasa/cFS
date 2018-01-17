/*
**
** File: ut_osapi_stubs.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_osapi_stubs.h 1.2 2011/05/16 16:25:33EDT rmcgraw Exp  $
**
** Purpose: OSAPI Header file for unit test stubs
**
** $Log: ut_osapi_stubs.h  $
** Revision 1.2 2011/05/16 16:25:33EDT rmcgraw 
** Added hook functionality to Count Semaphore APIs
** Revision 1.3 2011/05/16 14:42:41EDT rmcgraw 
** Added SetRtnCode processing to Counting Semaphore APIs
** Revision 1.2 2011/03/08 15:42:03EST rmcgraw 
** Added OS_CountSemGetIdByName
** Revision 1.1 2011/02/15 11:12:35EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_OSAPI_STUBS_H_
#define UT_OSAPI_STUBS_H_

typedef enum 
{
    UT_OSAPI_TASKDELAY_INDEX,
    UT_OSAPI_BINSEMTAKE_INDEX,
    UT_OSAPI_BINSEMTIMEDWAIT_INDEX,
    UT_OSAPI_MUTSEMTAKE_INDEX,
    UT_OSAPI_GETLOCALTIME_INDEX,
    UT_OSAPI_QUEUEGET_INDEX,
    UT_OSAPI_QUEUEPUT_INDEX,
    UT_OSAPI_TASKDELETE_INDEX,
    UT_OSAPI_BINSEMGIVE_INDEX,
    UT_OSAPI_COUNTSEMCREATE_INDEX,
    UT_OSAPI_COUNTSEMDELETE_INDEX,    
    UT_OSAPI_COUNTSEMGIVE_INDEX,
    UT_OSAPI_COUNTSEMTAKE_INDEX,
    UT_OSAPI_COUNTSEMTIMEDWAIT_INDEX,
    UT_OSAPI_COUNTSEMGETIDBYNAME_INDEX,
    UT_OSAPI_COUNTSEMGETINFO_INDEX,
    UT_OSAPI_MAX_INDEX
} Ut_OSAPI_Index_t;

typedef struct
{
    int32 (*OS_TaskDelay)(uint32);
    int32 (*OS_BinSemTake)(uint32);
    int32 (*OS_BinSemTimedWait)(uint32, uint32);
    int32 (*OS_MutSemTake)(uint32);
    int32 (*OS_GetLocalTime)(OS_time_t *);
    int32 (*OS_QueueGet)(uint32, void *, uint32, uint32 *, int32);
    int32 (*OS_QueuePut)(uint32, void *, uint32, uint32);
    int32 (*OS_TaskDelete)(uint32);
    int32 (*OS_BinSemGive)(uint32);
    int32 (*OS_CountSemCreate)(uint32 *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options);
    int32 (*OS_CountSemDelete)(uint32 sem_id);
    int32 (*OS_CountSemGive)(uint32 sem_id);
    int32 (*OS_CountSemTake)(uint32 sem_id);
    int32 (*OS_CountSemTimedWait)(uint32 sem_id, uint32 msecs);
    int32 (*OS_CountSemGetIdByName)(uint32 *sem_id, const char *sem_name);
    int32 (*OS_CountSemGetInfo)(uint32 sem_id, OS_count_sem_prop_t *count_prop);
} Ut_OSAPI_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} Ut_OSAPI_ReturnCodeTable_t;

void Ut_OSAPI_Reset(void);
void Ut_OSAPI_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_OSAPI_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);

#endif
