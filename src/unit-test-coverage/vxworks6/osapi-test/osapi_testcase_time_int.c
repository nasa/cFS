/*
 * Filename: osapi_testcase_time_int.c
 *
 * Purpose: This file contains unit test cases
 *
 * Notes:
 *  * Currently no test for sysClkRateGet() returning a 0 value in OS_Tick2Micros.
 *    This is not an expected value and there is no protection in the code for it.
 *  * Currently no test for OS_Tick2Micros() returning a 0 value (caused by
 *    integer truncation during the divide).  Is it possible the value from
 *    sysClkRateGet() exceeds the size of the numerator (1,000,000 in this case)?
 *
 * Modification History:
 *   07/07/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi_testcase.h"


#include <limits.h>


/* Global variables defined in osapi.c */
extern uint32 OS_printf_enabled;
extern int32  OS_DroppedMessages;


/* Prototypes for non-exported functions */
uint32 OS_FindCreator(void);


/* -------------------- Special Test Case Variables ------------------------- */
#ifndef INT32_MAX
#define INT32_MAX ((int32)(2147483647)) /* 2**31 - 1 */
#endif
static int32 error_num;
static os_err_name_t err_name;


/* -------------------------------------------------------------------------- */
/* Utility functions */



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_Milli2Ticks Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_Milli2Ticks_Min(void)
{
    int32 expected = 0;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 msecs = 0;

    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1); /* make the math simple */

    /* Execute Test */
    actual = OS_Milli2Ticks(msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_Milli2Ticks_Max(void)
{
    int32 expected = INT32_MAX;
    uint32 actual  = 99;  /* actual return value is int32, check value is limited */

    /* Setup Inputs */
    uint32 msecs = INT32_MAX/500;  /* leave in factor of 2 (1000/500) and check if limited */

    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1); /* make the math simple */

    /* Execute Test */
    actual = OS_Milli2Ticks(msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_Milli2Ticks_DivideByZero(void)
{
    int32 expected = INT_MAX; /* what's a good expected value for div by zero? */
    uint32 actual  = 99;

    /* Setup Inputs */

    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 0, 1);

    /* Execute Test */
    /*actual = OS_Milli2Ticks(msecs); Commented out - no protection for divide by zero */

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  OS_Tick2Micros Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_Tick2Micros_Pos(void)
{
    int32 expected = 1000000;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1, 1);

    /* Execute Test */
    actual = OS_Tick2Micros();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_Tick2Micros_Neg(void)
{
    int32 expected = -1000000;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, -1, 1);

    /* Execute Test */
    actual = OS_Tick2Micros();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_Tick2Micros_DivTruncation(void)
{
    int32 expected = 1;  /* output should be limited */
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000001, 1);

    /* Execute Test */
    actual = OS_Tick2Micros();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_Tick2Micros_DivideByZero(void)
{
    int32 expected = INT_MAX; /* what's a good expected value for div by zero? */
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 0, 1);

    /* Execute Test */
    /*actual = OS_Tick2Micros(); Commented out - no protection for divide by zero */

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  OS_GetLocalTime Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_GetLocalTime_NullTime(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_GetLocalTime(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetLocalTime_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_time_t time;

    OsApi_SetReturnCode(OSAPI_CLOCKGETTIME_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_GetLocalTime(&time);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetLocalTime_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_time_t time;
    uint32 sec  = 21;
    uint32 nsec = 5000;

    setTime(sec, nsec);
    OsApi_SetReturnCode(OSAPI_CLOCKGETTIME_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_GetLocalTime(&time);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(time.seconds == sec, "seconds == expected");
    UtAssert_True(time.microsecs == nsec/1000, "microsecs == expected");
}


/*******************************************************************************
**
**  OS_SetLocalTime Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_SetLocalTime_NullTime(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_SetLocalTime(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_SetLocalTime_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_time_t time;

    OsApi_SetReturnCode(OSAPI_CLOCKSETTIME_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_SetLocalTime(&time);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_SetLocalTime_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_time_t time;

    OsApi_SetReturnCode(OSAPI_CLOCKSETTIME_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_SetLocalTime(&time);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getSeconds() == time.seconds, "tv_sec == expected");
    UtAssert_True(getNSeconds() == time.microsecs * 1000, "tv_nsec == expected");
}


/*******************************************************************************
**
**  OS_IntAttachHandler Tests
**
*******************************************************************************/
void dummyIntFnc(void)
{

}

/*----------------------------------------------------------------------------*/
void Test_OS_IntAttachHandler_NullHandler(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 intNumber = 0;
    int32  parameter = 0;

    /* Execute Test */
    actual = OS_IntAttachHandler(intNumber, NULL, parameter);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_IntAttachHandler_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 intNumber = 0;
    int32  parameter = 0;

    OsApi_SetReturnCode(OSAPI_INTCONNECT_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_IntAttachHandler(intNumber, dummyIntFnc, parameter);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_IntAttachHandler_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 intNumber = 0;
    int32  parameter = 0;

    OsApi_SetReturnCode(OSAPI_INTCONNECT_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_IntAttachHandler(intNumber, dummyIntFnc, parameter);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
}


/*******************************************************************************
**
**  OS_IntDisable Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_IntDisable_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 level = 0;

    OsApi_SetReturnCode(OSAPI_INTDISABLE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_IntDisable(level);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_IntDisable_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 level = 0;

    OsApi_SetReturnCode(OSAPI_INTDISABLE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_IntDisable(level);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
}


/*******************************************************************************
**
**  OS_IntEnable Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_IntEnable_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 level = 0;

    OsApi_SetReturnCode(OSAPI_INTENABLE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_IntEnable(level);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_IntEnable_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 level = 0;

    OsApi_SetReturnCode(OSAPI_INTENABLE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_IntEnable(level);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
}


/*******************************************************************************
**
**  OS_IntUnlock Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_IntUnlock_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 level = 0;

    OsApi_SetReturnCode(OSAPI_INTUNLOCK_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_IntUnlock(level);

    /* Verify Outputs */
#if ( _WRS_VXWORKS_MINOR > 3 )
    UtAssert_True(actual == OS_SUCCESS, "actual == OS_SUCCESS");
#else
    UtAssert_True(actual == expected, "actual == OS_ERROR");
#endif
}

/*----------------------------------------------------------------------------*/
void Test_OS_IntUnlock_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 level = 0;
    int notErrOrOk = 2;

    OsApi_SetReturnCode(OSAPI_INTUNLOCK_INDEX, notErrOrOk, 1);

    /* Execute Test */
    actual = OS_IntUnlock(level);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
}


/*******************************************************************************
**
**  OS_IntLock Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_IntLock(void)
{
    int32 expected = 10;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_INTLOCK_INDEX, expected, 1);

    /* Execute Test */
    actual = OS_IntLock();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  OS_HeapGetInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_HeapGetInfo_NullProp(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_HeapGetInfo(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_HeapGetInfo_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_heap_prop_t prop;

    OsApi_SetReturnCode(OSAPI_MEMPARTINFOGET_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_HeapGetInfo(&prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

/*----------------------------------------------------------------------------*/
void Test_OS_HeapGetInfo_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    unsigned long bytesFree    = 1010;
    unsigned long blocksFree   = 2020;
    unsigned long maxBlockFree = 3030;
    static OS_heap_prop_t prop;

    setHeapInfo(bytesFree, blocksFree, maxBlockFree);
    OsApi_SetReturnCode(OSAPI_MEMPARTINFOGET_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_HeapGetInfo(&prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(prop.free_bytes == bytesFree, "free_bytes == expected");
    UtAssert_True(prop.free_blocks == blocksFree, "free_blocks == expected");
    UtAssert_True(prop.largest_free_block == maxBlockFree, "largest_free_block == expected");
}


/*******************************************************************************
**
**  OS_GetErrorName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_NullName(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_GetErrorName(error_num, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_SUCCESS;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_SUCCESS", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_Error(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERROR;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERROR", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_InvalidPtr(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_INVALID_POINTER;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_INVALID_POINTER", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_AddrMisaligned(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERROR_ADDRESS_MISALIGNED;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERROR_ADDRESS_MISALIGNED", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_Timeout(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERROR_TIMEOUT;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERROR_TIMEOUT", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_InvalidIntNum(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_INVALID_INT_NUM;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_INVALID_INT_NUM", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_SemFailure(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_SEM_FAILURE;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_SEM_FAILURE", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_SemTimeout(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_SEM_TIMEOUT;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_SEM_TIMEOUT", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_QueueEmpty(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_QUEUE_EMPTY;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_QUEUE_EMPTY", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_QueueFull(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_QUEUE_FULL;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_QUEUE_FULL", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_QueueTimeout(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_QUEUE_TIMEOUT;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_QUEUE_TIMEOUT", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_QueueInvalidSize(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_QUEUE_INVALID_SIZE;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_QUEUE_INVALID_SIZE", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_QueueIdError(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_QUEUE_ID_ERROR;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_QUEUE_ID_ERROR", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_NameTooLong(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_NAME_TOO_LONG;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_NAME_TOO_LONG", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_NoFreeIds(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_NO_FREE_IDS;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_NO_FREE_IDS", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_NameTaken(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_NAME_TAKEN;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_NAME_TAKEN", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_InvalidId(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_INVALID_ID;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_INVALID_ID", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_NameNotFound(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_NAME_NOT_FOUND;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_NAME_NOT_FOUND", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_SemNotFull(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_SEM_NOT_FULL;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_SEM_NOT_FULL", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_InvalidPriority(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_INVALID_PRIORITY;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_INVALID_PRIORITY", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_InvalidSemValue(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_INVALID_SEM_VALUE;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_INVALID_SEM_VALUE", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_File(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_FILE;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_FILE", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_NotImplemented(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_ERR_NOT_IMPLEMENTED;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_ERR_NOT_IMPLEMENTED", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_TimerInvalidArgs(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_TIMER_ERR_INVALID_ARGS;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_TIMER_ERR_INVALID_ARGS", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_TimerId(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_TIMER_ERR_TIMER_ID;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_TIMER_ERR_TIMER_ID", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_TimerUnavailable(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_TIMER_ERR_UNAVAILABLE;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_TIMER_ERR_UNAVAILABLE", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_TimerInternal(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = OS_TIMER_ERR_INTERNAL;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp(err_name, "OS_TIMER_ERR_INTERNAL", "err_name == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_GetErrorName_Unknown(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    error_num = 100;

    /* Execute Test */
    actual = OS_GetErrorName(error_num, &err_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
    UtAssert_StrCmp(err_name, "ERROR_UNKNOWN", "err_name == expected");
}


/*******************************************************************************
**
**  OS_FindCreator Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FindCreator_NotFound(void)
{
    uint32 expected = OS_MAX_TASKS;
    uint32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, 2, 1);

    /* Execute Test */
    actual = OS_FindCreator();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FindCreator_Found(void)
{
    uint32 expected = 3;
    uint32 actual   = 99;

    /* Setup Inputs */
    int id = 88;
    OsApi_Adaptor_setTaskTableEntry(expected, FALSE, id, "", 0, 0, 0, NULL);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, id, 1);

    /* Execute Test */
    actual = OS_FindCreator();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  OS_printf Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_printf_PrintDisabled(void)
{
    /* Setup Inputs */
    OsApi_Adaptor_setOsPrintfEnabled(FALSE);

    /* Execute Test */
    OS_printf("testString");

    /* Verify Outputs */
    UtAssert_True(getNVsnprintf_calls() == 0, "vsnprintf not called");
}

/*----------------------------------------------------------------------------*/
void Test_OS_printf_PrintEnabled_Err(void)
{
    /* Setup Inputs */
    OsApi_Adaptor_setOsPrintfEnabled(TRUE);

#ifdef OS_UTILITY_TASK_ON
    OsApi_SetReturnCode(OSAPI_MSGQSEND_INDEX, VCS_ERROR, 1);
    OS_DroppedMessages = 0;
#endif

    /* Execute Test */
    OS_printf("testString");

    /* Verify Outputs */
    UtAssert_True(getNVsnprintf_calls() == 1, "vsnprintf called");
#ifdef OS_UTILITY_TASK_ON
    UtAssert_True(OS_DroppedMessages == 1, "OS_DroppedMessages == expected");
#endif
}

/*----------------------------------------------------------------------------*/
void Test_OS_printf_PrintEnabled_Ok(void)
{
    /* Setup Inputs */
    OsApi_Adaptor_setOsPrintfEnabled(TRUE);

#ifdef OS_UTILITY_TASK_ON
    OsApi_SetReturnCode(OSAPI_MSGQSEND_INDEX, VCS_OK, 1);
    OS_DroppedMessages = 0;
#endif

    /* Execute Test */
    OS_printf("testString");

    /* Verify Outputs */
    UtAssert_True(getNVsnprintf_calls() == 1, "vsnprintf called");
#ifdef OS_UTILITY_TASK_ON
    UtAssert_True(OS_DroppedMessages == 0, "OS_DroppedMessages == expected");
#else
    UtAssert_True(getNLogMsg_calls() == 1, "logMsg called");
#endif
}


/*******************************************************************************
**
**  OS_printf_disable Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_printf_disable(void)
{
    /* Setup Inputs */
    OsApi_Adaptor_setOsPrintfEnabled(TRUE);

    /* Execute Test */
    OS_printf_disable();

    /* Verify Outputs */
    UtAssert_True(OsApi_Adaptor_getOsPrintfEnabled() == FALSE, "OS_printf_enabled == expected");
}


/*******************************************************************************
**
**  OS_printf_enable Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_printf_enable(void)
{
    /* Setup Inputs */
    OsApi_Adaptor_setOsPrintfEnabled(FALSE);

    /* Execute Test */
    OS_printf_enable();

    /* Verify Outputs */
    UtAssert_True(OsApi_Adaptor_getOsPrintfEnabled() == TRUE, "OS_printf_enabled == expected");
}


/*******************************************************************************
**
**  OS_FPUExcSetMask Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FPUExcSetMask(void)
{
    int32 expected = OS_ERR_NOT_IMPLEMENTED;
    int32 actual   = 99;
#if defined(_PPC_) && CPU != PPC440
    expected = OS_SUCCESS;
#endif

    /* Setup Inputs */
    uint32 mask = 0xAA;

    /* Execute Test */
    actual = OS_FPUExcSetMask(mask);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
#if defined(_PPC_) && CPU != PPC440
    UtAssert_True(getFpuMask() == mask, "mask == expected");
#else
    UtAssert_True(TRUE, "INTENTIONALLY SKIPPED: mask == expected");
#endif
}


/*******************************************************************************
**
**  OS_FPUExcGetMask Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_FPUExcGetMask_NullPtr(void)
{
    int32 expected = OS_ERR_NOT_IMPLEMENTED;
    int32 actual   = 99;
#if defined(_PPC_) && CPU != PPC440
    expected = OS_INVALID_POINTER;
#endif

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_FPUExcGetMask(NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_FPUExcGetMask_Success(void)
{
    int32 expected = OS_ERR_NOT_IMPLEMENTED;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 mask = 0;
#if defined(_PPC_) && CPU != PPC440
    int32 expected = OS_SUCCESS;
    uint32 vxMask = 4829;
#else
    uint32 vxMask = mask;
#endif

    OsApi_SetReturnCode(OSAPI_VXFPSCRGET_INDEX, vxMask, 1);

    /* Execute Test */
    actual = OS_FPUExcGetMask(&mask);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_True(vxMask == mask, "mask == expected");
}


/* ------------------- End of test cases --------------------------------------*/


/* Osapi_Time_Int_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Time_Int_Setup(void)
{
    Osapi_Setup();

    memset(err_name, '\0', sizeof(os_err_name_t));
    error_num = 0;
}


/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void Osapi_AddTestCase_Time_Int(void)
{
    /* OS_Milli2Ticks Tests */
    ADD_TEST(Test_OS_Milli2Ticks_Min,          Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_Milli2Ticks_Max,          Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_Milli2Ticks_DivideByZero, Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_Tick2Micros Tests */
    ADD_TEST(Test_OS_Tick2Micros_Pos,          Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_Tick2Micros_Neg,          Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_Tick2Micros_DivTruncation,Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_Tick2Micros_DivideByZero, Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_GetLocalTime Tests */
    ADD_TEST(Test_OS_GetLocalTime_NullTime,  Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetLocalTime_Error,     Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetLocalTime_Success,   Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_SetLocalTime Tests */
    ADD_TEST(Test_OS_SetLocalTime_NullTime,  Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_SetLocalTime_Error,     Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_SetLocalTime_Success,   Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_IntAttachHandler Tests */
    ADD_TEST(Test_OS_IntAttachHandler_NullHandler,Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_IntAttachHandler_Error,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_IntAttachHandler_Success,    Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_IntDisable Tests */
    ADD_TEST(Test_OS_IntDisable_Error,       Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_IntDisable_Success,     Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_IntEnable Tests */
    ADD_TEST(Test_OS_IntEnable_Error,        Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_IntEnable_Success,      Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_IntUnlock Tests */
    ADD_TEST(Test_OS_IntUnlock_Error,        Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_IntUnlock_Success,      Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_IntLock Tests */
    ADD_TEST(Test_OS_IntLock,                Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_HeapGetInfo Tests */
    ADD_TEST(Test_OS_HeapGetInfo_NullProp,   Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_HeapGetInfo_Error,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_HeapGetInfo_Success,    Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_GetErrorName Tests */
    ADD_TEST(Test_OS_GetErrorName_NullName,        Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_Success,         Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_Error,           Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_InvalidPtr,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_AddrMisaligned,  Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_Timeout,         Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_InvalidIntNum,   Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_SemFailure,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_SemTimeout,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_QueueEmpty,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_QueueFull,       Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_QueueTimeout,    Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_QueueInvalidSize,Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_QueueIdError,    Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_NameTooLong,     Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_NoFreeIds,       Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_NameTaken,       Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_InvalidId,       Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_NameNotFound,    Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_SemNotFull,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_InvalidPriority, Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_InvalidSemValue, Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_File,            Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_NotImplemented,  Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_TimerInvalidArgs,Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_TimerId,         Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_TimerUnavailable,Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_TimerInternal,   Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_GetErrorName_Unknown,         Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_FindCreator Tests */
    ADD_TEST(Test_OS_FindCreator_NotFound,         Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_FindCreator_Found,            Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_printf_disable Tests */
    ADD_TEST(Test_OS_printf_PrintDisabled,         Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_printf_PrintEnabled_Err,      Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_printf_PrintEnabled_Ok,       Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_printf_disable Tests */
    ADD_TEST(Test_OS_printf_disable,               Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_printf_enable Tests */
    ADD_TEST(Test_OS_printf_enable,                Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_FPUExcSetMask Tests */
    ADD_TEST(Test_OS_FPUExcSetMask,                Osapi_Time_Int_Setup, Osapi_TearDown);

    /* OS_FPUExcGetMask Tests */
    ADD_TEST(Test_OS_FPUExcGetMask_NullPtr,        Osapi_Time_Int_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_FPUExcGetMask_Success,        Osapi_Time_Int_Setup, Osapi_TearDown);
}




