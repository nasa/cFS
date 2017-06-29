/*
 * Filename: osapi_testcase_mutsems.c
 *
 * Purpose: This file contains unit test cases
 *
 * Notes:
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
#include "osapi_adaptor.h"


/* Prototypes for non-exported functions */



/* -------------------- Special Test Case Variables ------------------------- */
static uint32 sem_id;
static const char *sem_name = "testName";
static uint32 options;
static int32 expected;
static int32 actual;
static OsApi_Adaptor_OS_mut_sem_record_t *osMuttSemRecord_ptr;

/* -------------------------------------------------------------------------- */
/* Utility functions */



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_MutSemCreate Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_MutSemCreate_NullId(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemCreate(NULL, sem_name, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemCreate_NullName(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemCreate(&sem_id, NULL, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemCreate_NameTooLong(void)
{
    expected = OS_ERR_NAME_TOO_LONG;

    /* Setup Inputs */

    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_MutSemCreate(&sem_id, sem_name, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemCreate_NoFreeIds(void)
{
    expected = OS_ERR_NO_FREE_IDS;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemCreate(&sem_id, sem_name, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NO_FREE_IDS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemCreate_NameTaken(void)
{
    expected = OS_ERR_NAME_TAKEN;

    /* Setup Inputs */
    OsApi_Adaptor_setMutSemTableEntry(11, TRUE, 0, "", 0); /* for full MCDC coverage */
    OsApi_Adaptor_setMutSemTableEntry(OS_MAX_MUTEXES - 1, FALSE, 0, (char *)sem_name, 0);

    /* Execute Test */
    actual = OS_MutSemCreate(&sem_id, sem_name, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TAKEN");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemCreate_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    uint32 idx = 5;
    OsApi_Adaptor_setMutSemTableEntry(idx, TRUE, 0, "", 0);

    OsApi_SetReturnCode(OSAPI_SEMMCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_MutSemCreate(&sem_id, sem_name, options);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");
    UtAssert_True(OsApi_Adaptor_getMutSemTableEntry(idx)->free == TRUE, "table entry still free");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemCreate_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    uint32 idx                   = 9;
    VCS_SEM_ID creator               = (VCS_SEM_ID)109;
    uint32 creatorIdx            = 19;
    OsApi_Adaptor_setMutSemTableEntry(idx, TRUE, 0, "", 0);
    OsApi_Adaptor_setMutSemTableEntry(creatorIdx, FALSE, creator, "", 0);
    OsApi_Adaptor_setTaskTableEntry(creatorIdx, FALSE, (uint32)creator, "", 0, 0, 0, NULL);

    OsApi_SetReturnCode(OSAPI_SEMMCREATE_INDEX, 1, 1);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, (uint32)creator, 1);

    /* Execute Test */
    actual = OS_MutSemCreate(&sem_id, sem_name, options);

    /* Verify Outputs */
    osMuttSemRecord_ptr = OsApi_Adaptor_getMutSemTableEntry(idx);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(osMuttSemRecord_ptr->free == FALSE, "free == expected");
    UtAssert_StrCmp(osMuttSemRecord_ptr->name, sem_name, "name == expected");
    UtAssert_True(osMuttSemRecord_ptr->creator == creatorIdx, "creator == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_MutSemDelete Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_MutSemDelete_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemDelete(OS_MAX_MUTEXES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemDelete_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    OsApi_Adaptor_setMutSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_MutSemDelete(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemDelete_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMDELETE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_MutSemDelete(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemDelete_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    sem_id = OS_MAX_MUTEXES - 1;
    OsApi_Adaptor_setMutSemTableEntry(sem_id, FALSE, (VCS_SEM_ID)11, sem_name, 9);

    OsApi_SetReturnCode(OSAPI_SEMDELETE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_MutSemDelete(sem_id);

    /* Verify Outputs */
    osMuttSemRecord_ptr = OsApi_Adaptor_getMutSemTableEntry(sem_id);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(osMuttSemRecord_ptr->free == TRUE, "free == expected");
    UtAssert_StrCmp(osMuttSemRecord_ptr->name, "", "name == expected");
    UtAssert_True(osMuttSemRecord_ptr->creator == UNINITIALIZED, "creator == expected");
    UtAssert_True(osMuttSemRecord_ptr->id == NULL, "id == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_MutSemGive Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGive_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemGive(OS_MAX_MUTEXES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGive_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    OsApi_Adaptor_setMutSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_MutSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGive_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMGIVE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_MutSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGive_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    sem_id = OS_MAX_MUTEXES - 1;
    OsApi_SetReturnCode(OSAPI_SEMGIVE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_MutSemGive(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_MutSemTake Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_MutSemTake_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemTake(OS_MAX_MUTEXES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemTake_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    OsApi_Adaptor_setMutSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_MutSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemTake_Failure(void)
{
    expected = OS_SEM_FAILURE;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_MutSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SEM_FAILURE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemTake_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    sem_id = OS_MAX_MUTEXES - 1;

    OsApi_SetReturnCode(OSAPI_SEMTAKE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_MutSemTake(sem_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_MutSemGetIdByName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetIdByName_NullId(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemGetIdByName(NULL, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetIdByName_NullName(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemGetIdByName(&sem_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetIdByName_NameTooLong(void)
{
    expected = OS_ERR_NAME_TOO_LONG;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_MutSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetIdByName_NameNotFound(void)
{
    expected = OS_ERR_NAME_NOT_FOUND;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_MutSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_NOT_FOUND");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetIdByName_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    uint32 id = OS_MAX_MUTEXES - 1;
    OsApi_Adaptor_setMutSemTableEntry(4, TRUE, 0, "", 0); /* for full MCDC coverage */
    OsApi_Adaptor_setMutSemTableEntry(id, FALSE, 0, sem_name, 0);

    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME - 1, 1);

    /* Execute Test */
    actual = OS_MutSemGetIdByName(&sem_id, sem_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(sem_id == id, "id == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_MutSemGetInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetInfo_IdInvalid(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    static OS_mut_sem_prop_t prop;

    /* Execute Test */
    actual = OS_MutSemGetInfo(OS_MAX_MUTEXES, &prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetInfo_IdIsFree(void)
{
    expected = OS_ERR_INVALID_ID;

    /* Setup Inputs */
    static OS_mut_sem_prop_t prop;
    OsApi_Adaptor_setMutSemTableEntry(sem_id, TRUE, 0, "", 0);

    /* Execute Test */
    actual = OS_MutSemGetInfo(sem_id, &prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetInfo_NullProp(void)
{
    expected = OS_INVALID_POINTER;

    /* Setup Inputs */
    sem_id = OS_MAX_MUTEXES - 1;

    /* Execute Test */
    actual = OS_MutSemGetInfo(sem_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_MutSemGetInfo_Success(void)
{
    expected = OS_SUCCESS;

    /* Setup Inputs */
    static OS_mut_sem_prop_t prop;
    OsApi_Adaptor_setMutSemTableEntry(sem_id, FALSE, 0, sem_name, 232);


    /* Execute Test */
    actual = OS_MutSemGetInfo(sem_id, &prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(prop.creator == OsApi_Adaptor_getMutSemTableEntry(sem_id)->creator,
                  "creator == expected");
    UtAssert_StrCmp(prop.name, sem_name, "name == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_MUT_SEM_TABLE_SEM) == getNSemGive(OS_MUT_SEM_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/* ------------------- End of test cases --------------------------------------*/


/* Osapi_MutSem_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_MutSem_Setup(void)
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
void Osapi_AddTestCase_MutSem(void)
{
    /* OS_MutSemCreate Tests */
    ADD_TEST(Test_OS_MutSemCreate_NullId,        Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemCreate_NullName,      Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemCreate_NameTooLong,   Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemCreate_NoFreeIds,     Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemCreate_NameTaken,     Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemCreate_Failure,       Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemCreate_Success,       Osapi_MutSem_Setup, Osapi_TearDown);

    /* OS_MutSemDelete Tests */
    ADD_TEST(Test_OS_MutSemDelete_IdInvalid,   Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemDelete_IdIsFree,    Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemDelete_Failure,     Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemDelete_Success,     Osapi_MutSem_Setup, Osapi_TearDown);

    /* OS_MutSemGive Tests */
    ADD_TEST(Test_OS_MutSemGive_IdInvalid,     Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGive_IdIsFree,      Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGive_Failure,       Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGive_Success,       Osapi_MutSem_Setup, Osapi_TearDown);

    /* OS_MutSemTake Tests */
    ADD_TEST(Test_OS_MutSemTake_IdInvalid,     Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemTake_IdIsFree,      Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemTake_Failure,       Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemTake_Success,       Osapi_MutSem_Setup, Osapi_TearDown);

    /* OS_MutSemGetIdByName Tests */
    ADD_TEST(Test_OS_MutSemGetIdByName_NullId,      Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGetIdByName_NullName,    Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGetIdByName_NameTooLong, Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGetIdByName_NameNotFound,Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGetIdByName_Success,     Osapi_MutSem_Setup, Osapi_TearDown);

    /* OS_MutSemGetInfo Tests */
    ADD_TEST(Test_OS_MutSemGetInfo_IdInvalid,  Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGetInfo_IdIsFree,   Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGetInfo_NullProp,   Osapi_MutSem_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_MutSemGetInfo_Success,    Osapi_MutSem_Setup, Osapi_TearDown);
}




