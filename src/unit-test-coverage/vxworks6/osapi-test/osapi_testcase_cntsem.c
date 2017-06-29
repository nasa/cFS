/*
 * Filename: osapi_testcase_cntsems.c
 *
 * Purpose: This file contains unit test cases
 *
 * Notes:
 *
 * Modification History:
 *   06/28/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi_testcase.h"
#include <semaphore.h>



/* Prototypes for non-exported functions */



/* -------------------- Special Test Case Variables ------------------------- */
static uint32 sem_id;
static const char *sem_name = "testName";
static uint32 options;
static int32 expected;
static int32 actual;
static OsApi_Adaptor_OS_count_sem_record_t *osCountSemRecord_ptr;

/* -------------------------------------------------------------------------- */
/* Utility functions */



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_CountSemCreate Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_NullId(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    /* Execute Test */
    actual = OS_CountSemCreate(NULL, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_NullName(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    /* Execute Test */
    actual = OS_CountSemCreate(&sem_id, NULL, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_NameTooLong(void)
{
    expected = OS_ERR_NAME_TOO_LONG;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_CountSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_InitValInvalid(void)
{
    expected = OS_INVALID_SEM_VALUE;

#ifndef SEM_VALUE_MAX
#ifndef INT32_MAX
#define INT32_MAX ((int32)(2147483647)) /* 2**31 - 1 */
#endif
#define SEM_VALUE_MAX INT32_MAX
#endif
    /* Setup Inputs */
    uint32 sem_initial_value = (uint32)SEM_VALUE_MAX + 1;

    /* Execute Test */
    actual = OS_CountSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_SEM_VALUE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_NoFreeIds(void)
{
    expected = OS_ERR_NO_FREE_IDS;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    /* Execute Test */
    actual = OS_CountSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NO_FREE_IDS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_NameTaken(void)
{
    expected = OS_ERR_NAME_TAKEN;

    /* Setup Inputs */
    uint32 sem_initial_value   = 1;
    OsApi_Adaptor_setCountSemTableEntry(3, TRUE, 0, "", 0);
    OsApi_Adaptor_setCountSemTableEntry(OS_MAX_COUNT_SEMAPHORES - 1, FALSE, 0, sem_name, 0);

    /* Execute Test */
    actual = OS_CountSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TAKEN");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    uint32 idx = 10;
    uint32 sem_initial_value = 20;
    OsApi_Adaptor_setCountSemTableEntry(idx, TRUE, 0, "", 0);

    OsApi_SetReturnCode(OSAPI_SEMCCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_CountSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemCreate_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    uint32 idx                   = 10;
    uint32 sem_initial_value     = 0;
    VCS_SEM_ID creator               = (VCS_SEM_ID)55;
    uint32 creatorIdx            = 20;
    OsApi_Adaptor_setCountSemTableEntry(idx, TRUE, 0, "", 0);
    OsApi_Adaptor_setTaskTableEntry(creatorIdx, FALSE, (int32)creator, "", 0, 0, 0, NULL);

    OsApi_SetReturnCode(OSAPI_SEMCCREATE_INDEX, 1, 1);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, (int32)creator, 1);

    /* Execute Test */
    actual = OS_CountSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    osCountSemRecord_ptr = OsApi_Adaptor_getCountSemTableEntry(idx);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(osCountSemRecord_ptr->free == FALSE, "free == expected");
    UtAssert_StrCmp(osCountSemRecord_ptr->name, sem_name, "name == expected");
    UtAssert_True(osCountSemRecord_ptr->creator == creatorIdx, "creator == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CountSemDelete Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CountSemDelete_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_CountSemDelete(OS_MAX_COUNT_SEMAPHORES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemDelete_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    OsApi_Adaptor_setCountSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_CountSemDelete(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemDelete_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMDELETE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_CountSemDelete(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemDelete_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    sem_id = OS_MAX_COUNT_SEMAPHORES - 1;
    OsApi_Adaptor_setCountSemTableEntry(sem_id, FALSE, (VCS_SEM_ID)11, sem_name, 9);

    OsApi_SetReturnCode(OSAPI_SEMDELETE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_CountSemDelete(sem_id);

    /* Verify Outputs */
    osCountSemRecord_ptr = OsApi_Adaptor_getCountSemTableEntry(sem_id);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(osCountSemRecord_ptr->free == TRUE, "free == expected");
    UtAssert_StrCmp(osCountSemRecord_ptr->name, "", "name == expected");
    UtAssert_True(osCountSemRecord_ptr->creator == UNINITIALIZED, "creator == expected");
    UtAssert_True(osCountSemRecord_ptr->id == NULL, "id == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CountSemGive Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGive_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_CountSemGive(OS_MAX_COUNT_SEMAPHORES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGive_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    OsApi_Adaptor_setCountSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_CountSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGive_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMGIVE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_CountSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGive_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    sem_id = OS_MAX_COUNT_SEMAPHORES - 1;
    OsApi_SetReturnCode(OSAPI_SEMGIVE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_CountSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CountSemTake Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTake_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_CountSemTake(OS_MAX_COUNT_SEMAPHORES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTake_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    OsApi_Adaptor_setCountSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_CountSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTake_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_CountSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTake_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    sem_id = OS_MAX_COUNT_SEMAPHORES - 1;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_CountSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CountSemTimedWait Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTimedWait_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    uint32 msecs = 0;

    /* Execute Test */
    actual = OS_CountSemTimedWait(OS_MAX_COUNT_SEMAPHORES, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTimedWait_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    uint32 msecs = 0;
    OsApi_Adaptor_setCountSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_CountSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTimedWait_Timeout(void)
{
    expected = OS_SEM_TIMEOUT;

    /* Setup Inputs */
    uint32 msecs = 0;
    VCS_errno = VCS_S_objLib_OBJ_TIMEOUT;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_CountSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_TIMEOUT");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTimedWait_FailWithErr(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    uint32 msecs = 0;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_CountSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTimedWait_FailNotErr(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    uint32 msecs = 0;
    int32  notErrNorOk = 2;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, notErrNorOk, 1);

    /* Execute Test */
    actual = OS_CountSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemTimedWait_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    uint32 msecs = 321;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_OK, 1);
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1); /* make the math simple */

    /* Execute Test */
    actual = OS_CountSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getSemTakeTicks() == msecs * 1000, "timeoutTicks == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CountSemGetIdByName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetIdByName_NullId(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_CountSemGetIdByName(NULL, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetIdByName_NullName(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_CountSemGetIdByName(&sem_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetIdByName_NameTooLong(void)
{
    expected = OS_ERR_NAME_TOO_LONG;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_CountSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetIdByName_NameNotFound(void)
{
    expected = OS_ERR_NAME_NOT_FOUND;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_CountSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_NOT_FOUND");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetIdByName_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    uint32 id = OS_MAX_COUNT_SEMAPHORES - 1;
    OsApi_Adaptor_setCountSemTableEntry(1, TRUE, 0, "", 0); /* for full MCDC coverage */
    OsApi_Adaptor_setCountSemTableEntry(id, FALSE, 0, sem_name, 0);

    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME - 1, 1);

    /* Execute Test */
    actual = OS_CountSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(sem_id == id, "id == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_CountSemGetInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetInfo_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    static OS_count_sem_prop_t count_prop;

    /* Execute Test */
    actual = OS_CountSemGetInfo(OS_MAX_COUNT_SEMAPHORES, &count_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetInfo_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    static OS_count_sem_prop_t count_prop;
    OsApi_Adaptor_setCountSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_CountSemGetInfo(sem_id, &count_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetInfo_NullProp(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */
    sem_id = OS_MAX_COUNT_SEMAPHORES - 1;

    /* Execute Test */
    actual = OS_CountSemGetInfo(sem_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_CountSemGetInfo_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    static OS_count_sem_prop_t count_prop;
    count_prop.value = 99;
    OsApi_Adaptor_setCountSemTableEntry(sem_id, FALSE, 0, sem_name, 37);

    /* Execute Test */
    actual = OS_CountSemGetInfo(sem_id, &count_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(count_prop.creator == OsApi_Adaptor_getCountSemTableEntry(sem_id)->creator,
                  "creator == expected");
    UtAssert_True(count_prop.value == 0, "value == expected");
    UtAssert_StrCmp(count_prop.name, sem_name, "name == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_COUNT_SEM_TABLE_SEM) == getNSemGive(OS_COUNT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/* ------------------- End of test cases --------------------------------------*/


/* Osapi_CntSem_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_CntSem_Setup(void)
{
    Osapi_Setup();

    sem_id    = 3;
    options   = 4;
    VCS_errno     = 0;

    expected  = 0;
    actual    = 99;
}


/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void Osapi_AddTestCase_CntSem(void)
{
    /* OS_CountSemCreate Tests */
    ADD_TEST(Test_OS_CountSemCreate_NullId,        Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemCreate_NullName,      Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemCreate_NameTooLong,   Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemCreate_InitValInvalid,Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemCreate_NoFreeIds,     Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemCreate_NameTaken,     Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemCreate_Failure,       Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemCreate_Success,       Osapi_CntSem_Setup, Osapi_TearDown);

    /* OS_CountSemDelete Tests */
    ADD_TEST(Test_OS_CountSemDelete_IdInvalid,   Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemDelete_IdIsFree,    Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemDelete_Failure,     Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemDelete_Success,     Osapi_CntSem_Setup, Osapi_TearDown);

    /* OS_CountSemGive Tests */
    ADD_TEST(Test_OS_CountSemGive_IdInvalid,     Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGive_IdIsFree,      Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGive_Failure,       Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGive_Success,       Osapi_CntSem_Setup, Osapi_TearDown);

    /* OS_CountSemTake Tests */
    ADD_TEST(Test_OS_CountSemTake_IdInvalid,     Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTake_IdIsFree,      Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTake_Failure,       Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTake_Success,       Osapi_CntSem_Setup, Osapi_TearDown);

    /* OS_CountSemTimedWait Tests */
    ADD_TEST(Test_OS_CountSemTimedWait_IdInvalid,  Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTimedWait_IdIsFree,   Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTimedWait_Timeout,    Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTimedWait_FailWithErr,Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTimedWait_FailNotErr, Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemTimedWait_Success,    Osapi_CntSem_Setup, Osapi_TearDown);

    /* OS_CountSemGetIdByName Tests */
    ADD_TEST(Test_OS_CountSemGetIdByName_NullId,      Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGetIdByName_NullName,    Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGetIdByName_NameTooLong, Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGetIdByName_NameNotFound,Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGetIdByName_Success,     Osapi_CntSem_Setup, Osapi_TearDown);

    /* OS_CountSemGetInfo Tests */
    ADD_TEST(Test_OS_CountSemGetInfo_IdInvalid,  Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGetInfo_IdIsFree,   Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGetInfo_NullProp,   Osapi_CntSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_CountSemGetInfo_Success,    Osapi_CntSem_Setup, Osapi_TearDown);
}




