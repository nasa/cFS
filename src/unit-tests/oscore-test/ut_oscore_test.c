/*================================================================================*
** File:  ut_oscore_test.c
** Owner: Tam Ngo/Alan Cudmore
** Date:  May 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

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
void UT_os_init_task_delay_test(void);
void UT_os_init_task_set_priority_test(void);
void UT_os_init_task_get_id_test(void);
void UT_os_init_task_get_id_by_name_test(void);
void UT_os_init_task_get_info_test(void);

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
    g_task_names[0] = "Create_NotImpl";
    g_task_names[1] = "Create_NullPtr1";
    g_task_names[2] = "Create_NullPtr2";
    g_task_names[3] = "Create_NullPtr3";
    g_task_names[4] = "Create_LongName";
    g_task_names[5] = "Create_InvPrior";
    g_task_names[6] = "Create_Full";
    g_task_names[7] = "Create_DuplName";
    g_task_names[8] = "Create_OsErr";
    g_task_names[9] = "Create_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_delete_test()
{
    g_task_names[0] = "Delete_NotImpl";
    g_task_names[1] = "Delete_InvArg";
    g_task_names[2] = "Delete_OsErr";
    g_task_names[3] = "Delete_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_setup_install_delete_handler_test()
{
    g_task_names[0] = "DelHandler_NotImpl";
    g_task_names[1] = "DelHandler_InvArg";
    g_task_names[2] = "DelHandler_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_exit_test()
{
    g_task_names[0] = "Exit_NotImpl";
    g_task_names[1] = "Exit_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_delay_test()
{
    g_task_names[0] = "Delay_NotImpl";
    g_task_names[1] = "Delay_OsErr";
    g_task_names[2] = "Delay_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_set_priority_test()
{
    g_task_names[0] = "SetPrio_NotImpl";
    g_task_names[1] = "SetPrio_InvArg";
    g_task_names[2] = "SetPrio_InvPrio";
    g_task_names[3] = "SetPrio_OsErr";
    g_task_names[4] = "SetPrio_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_register_test()
{
    g_task_names[0] = "Register_NotImpl";
    g_task_names[1] = "Register_InvArg";
    g_task_names[2] = "Register_OsErr";
    g_task_names[3] = "Register_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_get_id_test()
{
    g_task_names[0] = "GetId_NotImpl";
    g_task_names[1] = "GetId_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_get_id_by_name_test()
{
    g_task_names[0] = "GetIdByName_NotImpl";
    g_task_names[1] = "GetIdByName_InvPtr1";
    g_task_names[2] = "GetIdByName_InvPtr2";
    g_task_names[3] = "GetIdByName_LongName";
    g_task_names[4] = "GetIdByName_NotFound";
    g_task_names[5] = "GetIdByName_Nominal";
}

/*--------------------------------------------------------------------------------*/

void UT_os_init_task_get_info_test()
{
    g_task_names[0] = "GetInfo_NotImpl";
    g_task_names[1] = "GetInfo_InvId";
    g_task_names[2] = "GetInfo_InvPtr";
    g_task_names[3] = "GetInfo_Nominal";
}

/*--------------------------------------------------------------------------------*
** Main
**--------------------------------------------------------------------------------*/
#ifdef _OSAL_UNIT_TEST_
   void OS_Application_Startup(void)
#else
   int main(int argc, char* argv[])
#endif
{
    UT_os_setup(UT_OS_LOG_FILENAME);

    /* UT_OS_LOG_OFF, UT_OS_LOG_MINIMAL, UT_OS_LOG_MODERATE, UT_OS_LOG_EVERYTHING */
    UT_os_set_log_verbose(UT_OS_LOG_EVERYTHING);

    UT_OS_LOG_MACRO("OSAL Unit Test Output File for oscore APIs\n")

    UT_os_apiinit_test();

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

    return (0);
}

/*================================================================================*
** End of File: ut_oscore_test.c
**================================================================================*/
