/*
 * Filename: osapi_testcase_queues.c
 *
 * Purpose: This file contains unit test cases
 *
 * Notes:
 *
 * Modification History:
 *   06/27/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi_testcase.h"



/* Prototypes for non-exported functions */



/* -------------------- Special Test Case Variables ------------------------- */
static uint32 queue_id;
static const char *queue_name = "testName";
static uint32 queue_depth;
static uint32 data_size;
static uint32 flags;
static uint32 data;
static uint32 size_copied;
static OsApi_Adaptor_OS_queue_record_t *osQueueRecord_ptr;


/* -------------------------------------------------------------------------- */
/* Utility functions */



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_QueueCreate Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_QueueCreate_NullId(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueCreate(NULL, queue_name, queue_depth, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueCreate_NullName(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueCreate(&queue_id, NULL, queue_depth, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueCreate_NameTooLong(void)
{
    int32 expected = OS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_QueueCreate(&queue_id, queue_name, queue_depth, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueCreate_NoFreeIds(void)
{
    int32 expected = OS_ERR_NO_FREE_IDS;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueCreate(&queue_id, queue_name, queue_depth, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NO_FREE_IDS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueCreate_NameTaken(void)
{
    int32 expected = OS_ERR_NAME_TAKEN;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(OS_MAX_QUEUES - 1, TRUE, 0, 0, "", 0);
    OsApi_Adaptor_setQueueTableEntry(1, FALSE, 0, 0, (char *)queue_name, 0);/* for full MCDC coverage */

    /* Execute Test */
    actual = OS_QueueCreate(&queue_id, queue_name, queue_depth, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TAKEN");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueCreate_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(1, TRUE, 0, 0, "", 0);

    OsApi_SetReturnCode(OSAPI_MSGQCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_QueueCreate(&queue_id, queue_name, queue_depth, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueCreate_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 12;
    uint32 creator_idx = 13;
    OsApi_Adaptor_setTaskTableEntry(creator_idx, FALSE, task_id, "testTask", 2, 3, 4, deleteFnc);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, task_id, 1);

    uint32 idx = 6;
    OsApi_Adaptor_setQueueTableEntry(idx, TRUE, 0, 0, "", 0);

    VCS_MSG_Q_ID msg_id = (VCS_MSG_Q_ID)300;
    OsApi_SetReturnCode(OSAPI_MSGQCREATE_INDEX, (int32)msg_id, 1);

    /* Execute Test */
    actual = OS_QueueCreate(&queue_id, queue_name, queue_depth, data_size, flags);

    /* Verify Outputs */
    osQueueRecord_ptr = OsApi_Adaptor_getQueueTableEntry(idx);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(osQueueRecord_ptr->id == msg_id, "id == expected");
    UtAssert_True(osQueueRecord_ptr->free == FALSE, "free == expected");
    UtAssert_StrCmp(osQueueRecord_ptr->name, queue_name, "name == expected");
    UtAssert_True(osQueueRecord_ptr->max_size == data_size, "max_size == expected");
    UtAssert_True(osQueueRecord_ptr->creator == creator_idx, "creator == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_QueueDelete Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_QueueDelete_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueDelete(OS_MAX_QUEUES);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueDelete_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(queue_id, TRUE, 0, 0, "", 0);

    /* Execute Test */
    actual = OS_QueueDelete(queue_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueDelete_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_MSGQDELETE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_QueueDelete(queue_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueDelete_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_MSGQDELETE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_QueueDelete(queue_id);

    /* Verify Outputs */
    osQueueRecord_ptr = OsApi_Adaptor_getQueueTableEntry(queue_id);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(osQueueRecord_ptr->id == NULL, "id == expected");
    UtAssert_True(osQueueRecord_ptr->free == TRUE, "free == expected");
    UtAssert_StrCmp(osQueueRecord_ptr->name, "", "name == expected");
    UtAssert_True(osQueueRecord_ptr->max_size == 0, "max_size == expected");
    UtAssert_True(osQueueRecord_ptr->creator == UNINITIALIZED, "creator == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_QueueGet Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueGet(OS_MAX_QUEUES, &data, data_size, &size_copied, OS_PEND);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(queue_id, TRUE, 0, 0, "", 0);

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, OS_PEND);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_NullData(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueGet(queue_id, NULL, 1, &size_copied, OS_PEND);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == 0, "no semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_NullSizeCopied(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, NULL, OS_PEND);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == 0, "no semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_SizeInvalid(void)
{
    int32 expected = OS_QUEUE_INVALID_SIZE;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size + 1, "", 0);

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, OS_PEND);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_QUEUE_INVALID_SIZE");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_TimeoutPend(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 expected_size_copied = 77;
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size, "", 0);
    OsApi_SetReturnCode(OSAPI_MSGQRECEIVE_INDEX, expected_size_copied, 1);

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, OS_PEND);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(size_copied == expected_size_copied, "size_copied == expected");
    UtAssert_True(getMsgQReceiveTimeout() == VCS_WAIT_FOREVER, "timeout == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_TimeoutChkQEmpty(void)
{
    int32 expected = OS_QUEUE_EMPTY;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size, "", 0);
    VCS_errno = VCS_S_objLib_OBJ_UNAVAILABLE;
    OsApi_SetReturnCode(OSAPI_MSGQRECEIVE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, OS_CHECK);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_QUEUE_EMPTY");
    UtAssert_True(size_copied == 0, "size_copied == expected");
    UtAssert_True(getMsgQReceiveTimeout() == VCS_NO_WAIT, "timeout == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_TimeoutChkErr(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size, "", 0);
    OsApi_SetReturnCode(OSAPI_MSGQRECEIVE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, OS_CHECK);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
    UtAssert_True(size_copied == 0, "size_copied == expected");
    UtAssert_True(getMsgQReceiveTimeout() == VCS_NO_WAIT, "timeout == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_TimeoutChkSuccess(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 expected_size_copied = 22;
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size, "", 0);
    OsApi_SetReturnCode(OSAPI_MSGQRECEIVE_INDEX, expected_size_copied, 1);

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, OS_CHECK);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(size_copied == expected_size_copied, "size_copied == expected");
    UtAssert_True(getMsgQReceiveTimeout() == VCS_NO_WAIT, "timeout == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_QTimeout(void)
{
    int32 expected = OS_QUEUE_TIMEOUT;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 timeout = 1;
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size, "", 0);
    VCS_errno = VCS_S_objLib_OBJ_TIMEOUT;
    OsApi_SetReturnCode(OSAPI_MSGQRECEIVE_INDEX, VCS_ERROR, 1);
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1); /* make the math simple */

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, timeout);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_QUEUE_TIMEOUT");
    UtAssert_True(size_copied == 0, "size_copied == expected");
    UtAssert_True(getMsgQReceiveTimeout() == timeout * 1000, "timeout == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 timeout = 1;
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size, "", 0);
    OsApi_SetReturnCode(OSAPI_MSGQRECEIVE_INDEX, VCS_ERROR, 1);
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1); /* make the math simple */

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, timeout);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
    UtAssert_True(size_copied == 0, "size_copied == expected");
    UtAssert_True(getMsgQReceiveTimeout() == timeout * 1000, "timeout == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGet_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    int32 timeout = 1;
    int32 expected_size_copied = 199;
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, data_size, "", 0);
    OsApi_SetReturnCode(OSAPI_MSGQRECEIVE_INDEX, expected_size_copied, 1);
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1); /* make the math simple */

    /* Execute Test */
    actual = OS_QueueGet(queue_id, &data, data_size, &size_copied, timeout);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(size_copied == expected_size_copied, "size_copied == expected");
    UtAssert_True(getMsgQReceiveTimeout() == timeout * 1000, "timeout == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_QueuePut Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_QueuePut_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueuePut(OS_MAX_QUEUES, &data, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueuePut_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setQueueTableEntry(queue_id, TRUE, 0, 0, "", 0);

    /* Execute Test */
    actual = OS_QueuePut(queue_id, &data, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueuePut_NullData(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueuePut(queue_id, NULL, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == 0, "semaphore not taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueuePut_QFull(void)
{
    int32 expected = OS_QUEUE_FULL;
    int32 actual   = 99;

    /* Setup Inputs */
    VCS_errno = VCS_S_objLib_OBJ_UNAVAILABLE;
    OsApi_SetReturnCode(OSAPI_MSGQSEND_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_QueuePut(queue_id, &data, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_QUEUE_FULL");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueuePut_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_MSGQSEND_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_QueuePut(queue_id, &data, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueuePut_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_MSGQSEND_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_QueuePut(queue_id, &data, data_size, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_QueueGetIdByName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetIdByName_NullId(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueGetIdByName(NULL, queue_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetIdByName_NullName(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueGetIdByName(&queue_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetIdByName_NameTooLong(void)
{
    int32 expected = OS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_QueueGetIdByName(&queue_id, queue_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetIdByName_NotFound(void)
{
    int32 expected = OS_ERR_NAME_NOT_FOUND;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueGetIdByName(&queue_id, queue_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_NOT_FOUND");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetIdByName_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 expected_idx = OS_MAX_QUEUES - 1;
    OsApi_Adaptor_setQueueTableEntry(5, TRUE, 0, 0, "", 0); /* for full MCDC coverage */
    OsApi_Adaptor_setQueueTableEntry(expected_idx, FALSE, 0, 0, (char *)queue_name, 0);

    /* Execute Test */
    actual = OS_QueueGetIdByName(&queue_id, queue_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(queue_id == expected_idx, "queue_id == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_QueueGetInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetInfo_NullProp(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_QueueGetInfo(queue_id, NULL);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetInfo_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_queue_prop_t queue_prop;

    /* Execute Test */
    actual = OS_QueueGetInfo(OS_MAX_QUEUES, &queue_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetInfo_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_queue_prop_t queue_prop;
    OsApi_Adaptor_setQueueTableEntry(queue_id, TRUE, 0, 0, "", 0);

    /* Execute Test */
    actual = OS_QueueGetInfo(queue_id, &queue_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_QueueGetInfo_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    static OS_queue_prop_t queue_prop;
    OsApi_Adaptor_setQueueTableEntry(queue_id, FALSE, 0, 0, (char *)queue_name, 8);

    /* Execute Test */
    actual = OS_QueueGetInfo(queue_id, &queue_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(queue_prop.creator == OsApi_Adaptor_getQueueTableEntry(queue_id)->creator,
                  "creator == expected");
    UtAssert_StrCmp(queue_prop.name, queue_name, "name == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_QUEUE_TABLE_SEM) == getNSemGive(OS_QUEUE_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/* ------------------- End of test cases --------------------------------------*/


/* Osapi_Queue_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Queue_Setup(void)
{
    Osapi_Setup();

    queue_id    = 1;
    queue_depth = 2;
    data_size   = 3;
    flags       = 4;
    data        = 5;
    size_copied = 6;
    VCS_errno   = 0;
}


/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void Osapi_AddTestCase_Queues(void)
{
    /* OS_QueueCreate Tests */
    ADD_TEST(Test_OS_QueueCreate_NullId,       Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueCreate_NullName,     Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueCreate_NameTooLong,  Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueCreate_NoFreeIds,    Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueCreate_NameTaken,    Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueCreate_Error,        Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueCreate_Success,      Osapi_Queue_Setup, Osapi_TearDown);

    /* OS_QueueDelete Tests */
    ADD_TEST(Test_OS_QueueDelete_IdInvalid,    Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueDelete_IdIsFree,     Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueDelete_Error,        Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueDelete_Success,      Osapi_Queue_Setup, Osapi_TearDown);

    /* OS_QueueGet Tests */
    ADD_TEST(Test_OS_QueueGet_IdInvalid,       Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_IdIsFree,        Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_NullData,        Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_NullSizeCopied,  Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_SizeInvalid,     Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_TimeoutPend,     Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_TimeoutChkQEmpty,Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_TimeoutChkErr,   Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_TimeoutChkSuccess,Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_QTimeout,        Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_Error,           Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGet_Success,         Osapi_Queue_Setup, Osapi_TearDown);

    /* OS_QueuePut Tests */
    ADD_TEST(Test_OS_QueuePut_IdInvalid,       Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueuePut_IdIsFree,        Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueuePut_NullData,        Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueuePut_QFull,           Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueuePut_Error,           Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueuePut_Success,         Osapi_Queue_Setup, Osapi_TearDown);

    /* OS_QueueGetIdByname Tests */
    ADD_TEST(Test_OS_QueueGetIdByName_NullId,  Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGetIdByName_NullName,Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGetIdByName_NameTooLong,Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGetIdByName_NotFound,Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGetIdByName_Success, Osapi_Queue_Setup, Osapi_TearDown);

    /* OS_QueueGetInfo Tests */
    ADD_TEST(Test_OS_QueueGetInfo_NullProp,    Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGetInfo_IdInvalid,   Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGetInfo_IdIsFree,    Osapi_Queue_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_QueueGetInfo_Success,     Osapi_Queue_Setup, Osapi_TearDown);
}




