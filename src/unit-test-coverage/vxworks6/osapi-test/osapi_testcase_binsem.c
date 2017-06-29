/*
 * Filename: osapi_testcase_binsem.c
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



/* Prototypes for non-exported functions */



/* -------------------- Special Test Case Variables ------------------------- */
static uint32 sem_id;
static const char *sem_name = "testName";
static uint32 options;
static OsApi_Adaptor_OS_bin_sem_record_t *osBinSemRecord_ptr;

/* -------------------------------------------------------------------------- */
/* Utility functions */



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_BinSemCreate Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemCreate_NullId(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    /* Execute Test */
    actual = OS_BinSemCreate(NULL, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemCreate_NullName(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    /* Execute Test */
    actual = OS_BinSemCreate(&sem_id, NULL, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemCreate_NameTooLong(void)
{
    int32 expected = OS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_BinSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemCreate_NoFreeIds(void)
{
    int32 expected = OS_ERR_NO_FREE_IDS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;

    /* Execute Test */
    actual = OS_BinSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NO_FREE_IDS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemCreate_NameTaken(void)
{
    int32 expected = OS_ERR_NAME_TAKEN;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 sem_initial_value = 1;
    OsApi_Adaptor_setBinSemTableEntry(3, TRUE, 0, "", 0);
    OsApi_Adaptor_setBinSemTableEntry(OS_MAX_BIN_SEMAPHORES - 1, FALSE, 0, sem_name, 0);

    /* Execute Test */
    actual = OS_BinSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TAKEN");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemCreate_Failure(void)
{
    int32 expected = OS_SEM_FAILURE;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 10;
    uint32 sem_initial_value = 20;
    OsApi_Adaptor_setBinSemTableEntry(idx, TRUE, 0, "", 0);

    OsApi_SetReturnCode(OSAPI_SEMBCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_BinSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");
    UtAssert_True(getSemBInitialValue() == 1, "sem_initial_value limited to valid value");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemCreate_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx                   = 10;
    uint32 sem_initial_value     = 0;
    VCS_SEM_ID creator               = (VCS_SEM_ID)55;
    uint32 creatorIdx            = 20;
    OsApi_Adaptor_setBinSemTableEntry(idx, TRUE, 0, "", 0);
    OsApi_Adaptor_setTaskTableEntry(creatorIdx, FALSE, (int32)creator, "", 0, 0, 0, NULL);

    OsApi_SetReturnCode(OSAPI_SEMBCREATE_INDEX, 1, 1);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, (int32)creator, 1);

    /* Execute Test */
    actual = OS_BinSemCreate(&sem_id, sem_name, sem_initial_value, options);

    /* Verify Outputs */
    osBinSemRecord_ptr = OsApi_Adaptor_getBinSemTableEntry(idx);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getSemBInitialValue() == 0, "sem_initial_value == expected");
    UtAssert_True(osBinSemRecord_ptr->free == FALSE, "free == expected");
    UtAssert_StrCmp(osBinSemRecord_ptr->name, sem_name, "name == expected");
    UtAssert_True(osBinSemRecord_ptr->creator == creatorIdx, "creator == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_BinSemDelete Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemDelete_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_BinSemDelete(OS_MAX_BIN_SEMAPHORES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemDelete_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setBinSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_BinSemDelete(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemDelete_Failure(void)
{
    int32 expected = OS_SEM_FAILURE;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMDELETE_INDEX, VCS_ERROR, 1);

/*    OS_bin_sem_table[sem_id].free = FALSE;
    strcpy(OS_bin_sem_table[sem_id].name, sem_name);*/

    /* Execute Test */
    actual = OS_BinSemDelete(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemDelete_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setBinSemTableEntry(sem_id, FALSE, (VCS_SEM_ID)11, sem_name, 9);

    OsApi_SetReturnCode(OSAPI_SEMDELETE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_BinSemDelete(sem_id);

    /* Verify Outputs */
    osBinSemRecord_ptr = OsApi_Adaptor_getBinSemTableEntry(sem_id);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(osBinSemRecord_ptr->free == TRUE, "free == expected");
    UtAssert_StrCmp(osBinSemRecord_ptr->name, "", "name == expected");
    UtAssert_True(osBinSemRecord_ptr->creator == UNINITIALIZED, "creator == expected");
    UtAssert_True(osBinSemRecord_ptr->id == NULL, "id == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_BinSemGive Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGive_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_BinSemGive(OS_MAX_BIN_SEMAPHORES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGive_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setBinSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_BinSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGive_Failure(void)
{
    int32 expected = OS_SEM_FAILURE;
    int32 actual   = 99;

    /* Setup Inputs */
    VCS_SEM_ID id = OsApi_Adaptor_Get_Os_Bin_Sem_Table_Sem();
    OsApi_Adaptor_setBinSemTableEntry(sem_id, FALSE, id, "", 0);

    OsApi_SetReturnCode(OSAPI_SEMGIVE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_BinSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");
    UtAssert_True(getSemGiveArg() == id, "semGive call arg == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGive_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    VCS_SEM_ID id = OsApi_Adaptor_Get_Os_Bin_Sem_Table_Sem();
    OsApi_Adaptor_setBinSemTableEntry(sem_id, FALSE, id, "", 0);

    OsApi_SetReturnCode(OSAPI_SEMGIVE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_BinSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getSemGiveArg() == id, "semGive call arg == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_BinSemFlush Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemFlush_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_BinSemFlush(OS_MAX_BIN_SEMAPHORES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemFlush_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 id = OS_MAX_BIN_SEMAPHORES - 1;
    OsApi_Adaptor_setBinSemTableEntry(id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_BinSemFlush(id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemFlush_Failure(void)
{
    int32 expected = OS_SEM_FAILURE;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMFLUSH_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_BinSemFlush(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemFlush_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMFLUSH_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_BinSemFlush(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_BinSemTake Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTake_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_BinSemTake(OS_MAX_BIN_SEMAPHORES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTake_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setBinSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_BinSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTake_Failure(void)
{
    int32 expected = OS_SEM_FAILURE;
    int32 actual   = 99;

    /* Setup Inputs */
    VCS_SEM_ID id = OsApi_Adaptor_Get_Os_Bin_Sem_Table_Sem();
    OsApi_Adaptor_setBinSemTableEntry(sem_id, FALSE, id, "", 0);

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_BinSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTake_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    sem_id = OS_MAX_BIN_SEMAPHORES - 1;
    VCS_SEM_ID id = OsApi_Adaptor_Get_Os_Bin_Sem_Table_Sem();
    OsApi_Adaptor_setBinSemTableEntry(sem_id, FALSE, id, "", 0);

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_BinSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_BinSemTimedWait Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTimedWait_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 msecs = 0;

    /* Execute Test */
    actual = OS_BinSemTimedWait(OS_MAX_BIN_SEMAPHORES, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTimedWait_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 msecs = 0;
    OsApi_Adaptor_setBinSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_BinSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTimedWait_TimeOut(void)
{
    int32 expected = OS_SEM_TIMEOUT;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 msecs = 0;
    sem_id       = OS_MAX_BIN_SEMAPHORES - 1;
    VCS_errno        = VCS_S_objLib_OBJ_TIMEOUT;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_BinSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_TIMEOUT");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTimedWait_FailWithErr(void)
{
    int32 expected = OS_SEM_FAILURE;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 msecs = 0;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_BinSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTimedWait_FailNotErr(void)
{
    int32 expected = OS_SEM_FAILURE;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 msecs  = 0;
    int32  notErrNorOk = 2;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, notErrNorOk, 1); /* for full MCDC coverage */

    /* Execute Test */
    actual = OS_BinSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemTimedWait_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 msecs = 45;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_OK, 1);
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1); /* make the math simple */

    /* Execute Test */
    actual = OS_BinSemTimedWait(sem_id, msecs);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getSemTakeTicks() == msecs * 1000, "timeoutTicks == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_BinSemGetIdByName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetIdByName_NullId(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *sem_name = "testName";

    /* Execute Test */
    actual = OS_BinSemGetIdByName(NULL, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetIdByName_NullName(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_BinSemGetIdByName(&sem_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetIdByName_NameTooLong(void)
{
    int32 expected = OS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *sem_name = "testName";
    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_BinSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetIdByName_NameNotFound(void)
{
    int32 expected = OS_ERR_NAME_NOT_FOUND;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *sem_name = "testName";

    /* Execute Test */
    actual = OS_BinSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_NOT_FOUND");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetIdByName_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    const char *sem_name = "testName";
    uint32 id = OS_MAX_BIN_SEMAPHORES - 1;
    OsApi_Adaptor_setBinSemTableEntry(0, TRUE, 0, "", 0); /* for full MCDC coverage */
    OsApi_Adaptor_setBinSemTableEntry(id, FALSE, 0, sem_name, 0);

    /* Execute Test */
    actual = OS_BinSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(sem_id == id, "sem_id == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_BinSemGetInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetInfo_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_bin_sem_prop_t bin_prop;

    /* Execute Test */
    actual = OS_BinSemGetInfo(OS_MAX_BIN_SEMAPHORES, &bin_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetInfo_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_bin_sem_prop_t bin_prop;
    OsApi_Adaptor_setBinSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_BinSemGetInfo(sem_id, &bin_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetInfo_NullProp(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_BinSemGetInfo(sem_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_BinSemGetInfo_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_bin_sem_prop_t bin_prop;
    OsApi_Adaptor_setBinSemTableEntry(sem_id, FALSE, 0, "testName", 32);

    /* Execute Test */
    actual = OS_BinSemGetInfo(sem_id, &bin_prop);

    /* Verify Outputs */
    osBinSemRecord_ptr = OsApi_Adaptor_getBinSemTableEntry(sem_id);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(bin_prop.creator == osBinSemRecord_ptr->creator, "creator == expected");
    UtAssert_True(bin_prop.value == 0, "value == expected");
    UtAssert_StrCmp(bin_prop.name, osBinSemRecord_ptr->name, "name == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_BIN_SEM_TABLE_SEM) == getNSemGive(OS_BIN_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/* ------------------- End of test cases --------------------------------------*/


/* Osapi_BinSem_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_BinSem_Setup(void)
{
    Osapi_Setup();

    sem_id    = 3;
    options   = 4;
    VCS_errno     = 0;
}


/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void Osapi_AddTestCase_BinSem(void)
{
    /* OS_BinSemCreate Tests */
    ADD_TEST(Test_OS_BinSemCreate_NullId,      Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemCreate_NullName,    Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemCreate_NameTooLong, Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemCreate_NoFreeIds,   Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemCreate_NameTaken,   Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemCreate_Failure,     Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemCreate_Success,     Osapi_BinSem_Setup, Osapi_TearDown);

    /* OS_BinSemDelete Tests */
    ADD_TEST(Test_OS_BinSemDelete_IdInvalid,   Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemDelete_IdIsFree,    Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemDelete_Failure,     Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemDelete_Success,     Osapi_BinSem_Setup, Osapi_TearDown);

    /* OS_BinSemGive Tests */
    ADD_TEST(Test_OS_BinSemGive_IdInvalid,     Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGive_IdIsFree,      Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGive_Failure,       Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGive_Success,       Osapi_BinSem_Setup, Osapi_TearDown);

    /* OS_BinSemFlush Tests */
    ADD_TEST(Test_OS_BinSemFlush_IdInvalid,    Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemFlush_IdIsFree,     Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemFlush_Failure,      Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemFlush_Success,      Osapi_BinSem_Setup, Osapi_TearDown);

    /* OS_BinSemTake Tests */
    ADD_TEST(Test_OS_BinSemTake_IdInvalid,     Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTake_IdIsFree,      Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTake_Failure,       Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTake_Success,       Osapi_BinSem_Setup, Osapi_TearDown);

    /* OS_BinSemTimedWait Tests */
    ADD_TEST(Test_OS_BinSemTimedWait_IdInvalid,  Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTimedWait_IdIsFree,   Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTimedWait_TimeOut,    Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTimedWait_FailWithErr,Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTimedWait_FailNotErr, Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemTimedWait_Success,    Osapi_BinSem_Setup, Osapi_TearDown);

    /* OS_BinSemGetIdByName Tests */
    ADD_TEST(Test_OS_BinSemGetIdByName_NullId,      Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGetIdByName_NullName,    Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGetIdByName_NameTooLong, Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGetIdByName_NameNotFound,Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGetIdByName_Success,     Osapi_BinSem_Setup, Osapi_TearDown);

    /* OS_BinSemGetInfo Tests */
    ADD_TEST(Test_OS_BinSemGetInfo_IdInvalid,  Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGetInfo_IdIsFree,   Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGetInfo_NullProp,   Osapi_BinSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_BinSemGetInfo_Success,    Osapi_BinSem_Setup, Osapi_TearDown);
}




