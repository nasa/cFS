/*================================================================================*
** File:  ut_oscore_test_arinc653.c
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include <apex_api.h>

#include "ut_oscore_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t  g_logInfo;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

int32  g_skipTestCase = -1;
char*  g_skipTestCaseResult = " ";

char* g_task_names[UT_OS_TASK_LIST_LEN];
char  g_long_task_name[OS_MAX_API_NAME+5];

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_init_task_misc(void);
void UT_os_init_task_create_test(void);
void UT_os_init_task_delete_test(void);
void UT_os_setup_install_delete_handler_test(void);
void UT_os_init_task_exit_test(void);
void UT_os_init_task_set_priority_test(void);
void UT_os_init_task_get_id_test(void);
void UT_os_init_task_get_id_by_name_test(void);
void UT_os_init_task_get_info_test(void);

void UT_os_core_tests(void);

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

void UT_os_init_task_misc()
{
    memset(g_long_task_name, 'X', sizeof(g_long_task_name));
    g_long_task_name[sizeof(g_long_task_name)-1] = '\0';
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_create_test()
{
    memset(g_long_task_name, 'X', sizeof(g_long_task_name));
    g_long_task_name[sizeof(g_long_task_name)-1] = '\0';

    g_task_names[0] = "Task_A";  /* Create_NotImpl */
    g_task_names[1] = "Task_A";  /* Create_NullPtr1 */
    g_task_names[2] = "Task_A";  /* Create_NullPtr2 */
    g_task_names[3] = "Task_B";  /* Create_NullPtr3 */
    g_task_names[4] = "Task_A";  /* Create_LongName */
    g_task_names[5] = "Task_A";  /* Create_InvPrior */
    g_task_names[6] = "Task_A";  /* Create_Full */
    g_task_names[7] = "Task_A";  /* Create_DuplName */
    g_task_names[8] = "Task_B";  /* Create_OsErr */
    g_task_names[9] = "Task_A";  /* Create_Nominal */

    g_skipTestCase = 6;
    g_skipTestCaseResult = UT_OS_NA;
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_delete_test()
{
    g_task_names[0] = "Task_B";  /* Delete_NotImpl */
    g_task_names[1] = "Task_B";  /* Delete_InvArg */
    g_task_names[2] = "Task_B";  /* Delete_OsErr */
    g_task_names[3] = "Task_B";  /* Delete_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_install_delete_handler_test()
{
    g_task_names[0] = "Task_B";  /* DelHandler_NotImpl */
    g_task_names[1] = "Task_B";  /* DelHandler_InvArg */
    g_task_names[2] = "Task_B";  /* DelHandler_Nominal */

    g_skipTestCase = 1;
    g_skipTestCaseResult = UT_OS_NA;
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_exit_test()
{
    g_task_names[0] = "Task_A";  /* Exit_NotImpl */
    g_task_names[1] = "Task_A";  /* Exit Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_delay_test()
{
    g_task_names[0] = "Task_A";  /* Delay_NotImpl */
    g_task_names[1] = "Task_A";  /* Delay_OsErr */
    g_task_names[2] = "Task_A";  /* Delay_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_set_priority_test()
{
    g_task_names[0] = "Task_A";  /* SetPrio_NotImpl */
    g_task_names[1] = "Task_B";  /* SetPrio_InvArg */
    g_task_names[2] = "Task_A";  /* SetPrio_InvPrio */
    g_task_names[3] = "Task_B";  /* SetPrio_OsErr */
    g_task_names[4] = "Task_A";  /* SetPrio_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_register_test()
{
    g_task_names[0] = "Task_A";  /* Register_NotImpl */
    g_task_names[1] = "Task_A";  /* Register_InvArg */
    g_task_names[2] = "Task_A";  /* Register_OsErr */
    g_task_names[3] = "Task_A";  /* Register_Nominal */

    g_skipTestCase = 1;
    g_skipTestCaseResult = UT_OS_NA;
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_get_id_test()
{
    g_task_names[0] = "Task_B";  /* GetId_NotImpl */
	g_task_names[1] = "Task_B";  /* GetId_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_get_id_by_name_test()
{
    g_task_names[0] = "Task_B";  /* GetIdByName_NotImpl */
    g_task_names[1] = "Task_B";  /* GetIdByName_InvPtr1 */
    g_task_names[2] = "Task_B";  /* GetIdByName_InvPtr2 */
    g_task_names[3] = "Task_B";  /* GetIdByName_LongName */
    g_task_names[4] = "Task_B";  /* GetIdByName_NotFound */
    g_task_names[5] = "Task_B";  /* GetIdByName_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_get_info_test()
{
    g_task_names[0] = "Task_B";  /* GetInfo_NotImpl */
    g_task_names[1] = "Task_B";  /* GetInfo_InvId */
    g_task_names[2] = "Task_B";  /* GetInfo_InvPtr */
    g_task_names[3] = "Task_B";  /* GetInfo_Nominal */
}

/*--------------------------------------------------------------------------------*/

void UT_os_core_tests()
{
    UT_OS_LOG_MACRO("\n============================================\n")
    UT_os_printf_test();
    UT_os_printfenable_test();
    UT_os_printfdisable_test();
    UT_OS_LOG_MACRO("============================================\n")

    UT_os_bin_sem_create_test();
    UT_os_bin_sem_delete_test();
    UT_os_bin_sem_flush_test();
    UT_os_bin_sem_give_test();
    UT_os_bin_sem_take_test();
    UT_os_bin_sem_timed_wait_test();
    UT_os_bin_sem_get_id_by_name_test();
    UT_os_bin_sem_get_info_test();

    UT_os_count_sem_create_test();
    UT_os_count_sem_delete_test();
    UT_os_count_sem_give_test();
    UT_os_count_sem_take_test();
    UT_os_count_sem_timed_wait_test();
    UT_os_count_sem_get_id_by_name_test();
    UT_os_count_sem_get_info_test();

    UT_os_mut_sem_create_test();
    UT_os_mut_sem_delete_test();
    UT_os_mut_sem_give_test();
    UT_os_mut_sem_take_test();
    UT_os_mut_sem_get_id_by_name_test();
    UT_os_mut_sem_get_info_test();

    UT_os_queue_create_test();
    UT_os_queue_delete_test();
    UT_os_queue_put_test();
    UT_os_queue_get_test();
    UT_os_queue_get_id_by_name_test();
    UT_os_queue_get_info_test();

    UT_os_init_task_misc();

    UT_os_init_task_create_test();
    UT_os_task_create_test();

    UT_os_init_task_delete_test();
    UT_os_task_delete_test();

    UT_os_setup_install_delete_handler_test();
    UT_os_task_install_delete_handler_test();

    UT_os_init_task_exit_test();
    UT_os_task_exit_test();

    UT_os_init_task_delay_test();
    UT_os_task_delay_test();

    UT_os_init_task_set_priority_test();
    UT_os_task_set_priority_test();

    UT_os_init_task_register_test();
    UT_os_task_register_test();

    UT_os_init_task_get_id_test();
    UT_OS_LOG_MACRO("\n============================================\n")
    UT_os_task_get_id_test();
    UT_OS_LOG_MACRO("============================================\n")

    UT_os_init_task_get_id_by_name_test();
    UT_os_task_get_id_by_name_test();

    UT_os_init_task_get_info_test();
    UT_os_task_get_info_test();

    UT_os_geterrorname_test();

    UT_os_tick2micros_test();
    UT_os_milli2ticks_test();

    UT_OS_LOG_MACRO("\n============================================")
    UT_os_getlocaltime_test();
    UT_OS_LOG_MACRO("============================================\n")
    UT_os_setlocaltime_test();

    UT_os_teardown("ut_oscore");
}

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/

void UT_main()
{
    int32 osalRet=0;
    uint32 tId=0, stackPtr=0;
    RETURN_CODE_TYPE arincRet=NO_ERROR;

    UT_os_setup(UT_OS_LOG_FILENAME);

    /* UT_OS_LOG_OFF, UT_OS_LOG_MINIMAL, UT_OS_LOG_MODERATE, UT_OS_LOG_EVERYTHING */
    UT_os_set_log_verbose(UT_OS_LOG_EVERYTHING);

    UT_OS_LOG_MACRO("OSAL Unit Test Output File for ARINC653-implementation of oscore APIs\n")

    UT_os_apiinit_test();

    osalRet = OS_TaskCreate(&tId, "ut_oscore",
                            (osal_task_entry)&UT_os_core_tests,
                            &stackPtr, 0x2000, 110, 0);

    SET_PARTITION_MODE(NORMAL, &arincRet);
}

/*================================================================================*
** End of File: ut_oscore_test_arinc653.c
**================================================================================*/
