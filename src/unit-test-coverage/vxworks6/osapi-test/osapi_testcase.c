/*
 * Filename: osapi_testcase.c
 *
 * Purpose: This file provides functions for adding unit test cases
 *
 * Notes:
 *
 * Modification History:
 *   06/25/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi_testcase.h"



/* Prototypes for non-exported functions */
void Osapi_AddTestCase_Tasks(void);
void Osapi_AddTestCase_Queues(void);
void Osapi_AddTestCase_BinSem(void);
void Osapi_AddTestCase_CntSem(void);
void Osapi_AddTestCase_MutSem(void);
void Osapi_AddTestCase_Time_Int(void);



/* -------------------- Define variables to match UUT ----------------------- */
const uint32 MAX_PRIORITY = 255; /* matches #define value in uut */
const uint32 UNINITIALIZED = 0;  /* matches #define value in uut */


/* -------------------- Special Test Case Variables ------------------------- */
const uint32 test_OS_task_table_sem      = OS_TASK_TABLE_SEM;
const uint32 test_OS_queue_table_sem     = OS_QUEUE_TABLE_SEM;
const uint32 test_OS_bin_sem_table_sem   = OS_BIN_SEM_TABLE_SEM;
const uint32 test_OS_count_sem_table_sem = OS_COUNT_SEM_TABLE_SEM;
const uint32 test_OS_mut_sem_table_sem   = OS_MUT_SEM_TABLE_SEM;


/* -------------------------------------------------------------------------- */
/*
 * Osapi_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Setup(void)
{
    OsApi_Reset();

    OsApi_Adaptor_Reset_Tables();
    OsApi_Adaptor_Set_Os_Task_Table_Sem((VCS_SEM_ID)&test_OS_task_table_sem);
    OsApi_Adaptor_Set_Os_Queue_Table_Sem((VCS_SEM_ID)&test_OS_queue_table_sem);
    OsApi_Adaptor_Set_Os_Bin_Sem_Table_Sem((VCS_SEM_ID)&test_OS_bin_sem_table_sem);
    OsApi_Adaptor_Set_Os_Count_Sem_Table_Sem((VCS_SEM_ID)&test_OS_count_sem_table_sem);
    OsApi_Adaptor_Set_Os_Mut_Sem_Table_Sem((VCS_SEM_ID)&test_OS_mut_sem_table_sem);
}


/*
 * Osapi_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_TearDown(void)
{

}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* Osapi_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    Osapi_AddTestCase_Tasks();
    Osapi_AddTestCase_Queues();
    Osapi_AddTestCase_BinSem();
    Osapi_AddTestCase_CntSem();
    Osapi_AddTestCase_MutSem();
    Osapi_AddTestCase_Time_Int();
}




