 /*************************************************************************
 ** File:
 **   $Id: sch_test_utils.h 1.2 2017/06/21 15:28:58EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains the function prototypes and global variables for the unit test utilities for the SCH application.
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 *************************************************************************/

/*
 * Includes
 */

#include "sch_app.h"
#include "ut_cfe_evs_hooks.h"
// FIXME: these need to be implemented/updated in UT-Assert (by copying from sch_test_utils.c/.h):
//#include "ut_cfe_evs_stubs.h"
//#include "ut_cfe_time_stubs.h"
//#include "ut_ostimer_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_time_hooks.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"
#include "ut_cfe_es_stubs.h"
#include <time.h>

/*
 * Function Definitions
 */

void SCH_Test_Setup(void);
void SCH_Test_TearDown(void);

/*
 * Additional UT-Assert Stub Functions and Required Data Structures
 *
 * Note: This code needs to be moved into the UT-Assert library.  We are including it here for now because the 
 * next release of the UT-Assert library is not expected to happen in the near future.
 */

/* ut_osapi_stubs */

typedef enum 
{
    UT_CFE_EVS_REGISTER_INDEX,
    UT_CFE_EVS_SENDEVENT_INDEX,
    UT_CFE_EVS_SENDTIMEDEVENT_INDEX,
    UT_CFE_EVS_SENDEVENTWITHAPPID_INDEX,
    UT_CFE_EVS_CLEANUPAPP_INDEX,
    UT_CFE_EVS_RESETALLFILTERS_INDEX,
    UT_CFE_EVS_MAX_INDEX
} Ut_CFE_EVS_INDEX_t;

typedef struct
{
    int32 (*CFE_EVS_Register)(void *Filters, uint16 NumEventFilters, uint16 FilterScheme);
    int32 (*CFE_EVS_SendEvent)(uint16 EventID, uint16 EventType, const char *EventText);
    int32 (*CFE_EVS_SendTimedEvent)(CFE_TIME_SysTime_t Time, uint16 EventID, uint16 EventType, const char *Spec, ... );
    int32 (*CFE_EVS_SendEventWithAppID)(uint16 EventID, uint16 EventType, uint32 AppID, const char *Spec, ... );
    int32 (*CFE_EVS_CleanUpApp)(uint32 AppId);
    int32 (*CFE_EVS_ResetAllFilters)(void);
} Ut_CFE_EVS_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_CFE_EVS_ReturnCodeTable_t;

void Ut_CFE_EVS_Reset(void);
void Ut_CFE_EVS_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_CFE_EVS_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_CFE_EVS_ContinueReturnCodeAfterCountZero(uint32 Index);
int32 CFE_EVS_ResetAllFilters(void);

/* end ut_osapi_stubs */

/* ut_ostimer_stubs */

typedef enum 
{
    UT_OSTIMER_CREATE_INDEX,
    UT_OSTIMER_SET_INDEX,
    UT_OSTIMER_MAX_INDEX
} Ut_OSTIMER_INDEX_t;

typedef struct
{
    int32 (*OS_TimerCreate)(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr);
    int32 (*OS_TimerSet)(uint32 timer_id, uint32 start_time, uint32 interval_time);
} Ut_OSTIMER_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_OSTIMER_ReturnCodeTable_t;

void Ut_OSTIMER_Reset(void);
void Ut_OSTIMER_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_OSTIMER_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_OSTIMER_ContinueReturnCodeAfterCountZero(uint32 Index);

/* end ut_ostimer_stubs */

/* ut_cfe_time_stubs */

typedef enum 
{
    UT_CFE_TIME_GETUTC_INDEX,
    UT_CFE_TIME_GETTAI_INDEX,
    UT_CFE_TIME_GETTIME_INDEX,
    UT_CFE_TIME_SUB2MICROSECS_INDEX,
    UT_CFE_TIME_SUBTRACT_INDEX,
    UT_CFE_TIME_MICRO2SUBSECS_INDEX,
    UT_CFE_TIME_ADD_INDEX,
    UT_CFE_TIME_PRINT_INDEX,
    UT_CFE_TIME_COMPARE_INDEX,
    UT_CFE_TIME_FS2CFESECONDS_INDEX,
    UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX,
    UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX,
    UT_CFE_TIME_GETMETSUBSECS_INDEX,
    UT_CFE_TIME_GETCLOCKINFO_INDEX,
    UT_CFE_TIME_MAX_INDEX
} Ut_CFE_TIME_INDEX_t;

typedef struct
{
    CFE_TIME_SysTime_t (*CFE_TIME_GetUTC)(void);
    CFE_TIME_SysTime_t (*CFE_TIME_GetTAI)(void);
    CFE_TIME_SysTime_t (*CFE_TIME_GetTime)(void);
    uint32 (*CFE_TIME_Micro2SubSecs)(uint32 MicroSeconds);
    uint32 (*CFE_TIME_Sub2MicroSecs)(uint32 SubSeconds);
    CFE_TIME_SysTime_t (*CFE_TIME_Add)(CFE_TIME_SysTime_t Time1, CFE_TIME_SysTime_t Time2);
    CFE_TIME_SysTime_t (*CFE_TIME_Subtract)(CFE_TIME_SysTime_t Time1, CFE_TIME_SysTime_t Time2);
    void (*CFE_TIME_Print)(char *PrintBuffer, CFE_TIME_SysTime_t TimeToPrint);
    CFE_TIME_Compare_t (*CFE_TIME_Compare)(CFE_TIME_SysTime_t TimeA, CFE_TIME_SysTime_t TimeB);
    uint32 (*CFE_TIME_FS2CFESeconds)(uint32 SecondsFS);
    int32  (*CFE_TIME_RegisterSynchCallback)(CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr);
    int32  (*CFE_TIME_UnregisterSynchCallback)(CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr);
    uint32 (*CFE_TIME_GetMETsubsecs)(void);
    uint16 (*CFE_TIME_GetClockInfo)(void);
} Ut_CFE_TIME_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
    boolean ContinueReturnCodeAfterCountZero;
} Ut_CFE_TIME_ReturnCodeTable_t;

void Ut_CFE_TIME_Reset(void);
void Ut_CFE_TIME_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_CFE_TIME_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Ut_CFE_TIME_ContinueReturnCodeAfterCountZero(uint32 Index);

/* end ut_cfe_time_stubs */

/************************/
/*  End of File Comment */
/************************/
