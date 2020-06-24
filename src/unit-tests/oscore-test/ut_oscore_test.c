/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

const char* g_task_names[UT_OS_TASK_LIST_LEN];
char  g_long_task_name[UT_OS_NAME_BUFF_SIZE];

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
void UT_os_init_task_register_test(void);
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

void UtTest_Setup(void)
{
    UtTest_Add(UT_os_apiinit_test, NULL, NULL, "OS_API_Init");

    UtTest_Add(UT_os_printf_test, NULL, NULL, "OS_printf");
    UtTest_Add(UT_os_printfenable_test, NULL, NULL, "OS_printf_enable");
    UtTest_Add(UT_os_printfdisable_test, NULL, NULL, "OS_printf_disable");

    UtTest_Add(UT_os_bin_sem_create_test, NULL, NULL, "OS_BinSemCreate");
    UtTest_Add(UT_os_bin_sem_delete_test, NULL, NULL, "OS_BinSemDelete");
    UtTest_Add(UT_os_bin_sem_flush_test, NULL, NULL, "OS_BinSemFlush");
    UtTest_Add(UT_os_bin_sem_give_test, NULL, NULL, "OS_BinSemGive");
    UtTest_Add(UT_os_bin_sem_take_test, NULL, NULL, "OS_BinSemTake");
    UtTest_Add(UT_os_bin_sem_timed_wait_test, NULL, NULL, "OS_BinSemTimedWait");
    UtTest_Add(UT_os_bin_sem_get_id_by_name_test, NULL, NULL, "OS_BinSemGetIdByName");
    UtTest_Add(UT_os_bin_sem_get_info_test, NULL, NULL, "OS_BinSemGetInfo");

    UtTest_Add(UT_os_count_sem_create_test, NULL, NULL, "OS_CountSemCreate");
    UtTest_Add(UT_os_count_sem_delete_test, NULL, NULL, "OS_CountSemDelete");
    UtTest_Add(UT_os_count_sem_give_test, NULL, NULL, "OS_CountSemGive");
    UtTest_Add(UT_os_count_sem_take_test, NULL, NULL, "OS_CountSemTake");
    UtTest_Add(UT_os_count_sem_timed_wait_test, NULL, NULL, "OS_CountSemTimedWait");
    UtTest_Add(UT_os_count_sem_get_id_by_name_test, NULL, NULL, "OS_CountSemGetIdByName");
    UtTest_Add(UT_os_count_sem_get_info_test, NULL, NULL, "OS_CountSemGetInfo");

    UtTest_Add(UT_os_mut_sem_create_test, NULL, NULL, "OS_MutSemCreate");
    UtTest_Add(UT_os_mut_sem_delete_test, NULL, NULL, "OS_MutSemDelete");
    UtTest_Add(UT_os_mut_sem_give_test, NULL, NULL, "OS_MutSemGive");
    UtTest_Add(UT_os_mut_sem_take_test, NULL, NULL, "OS_MutSemTake");
    UtTest_Add(UT_os_mut_sem_get_id_by_name_test, NULL, NULL, "OS_MutSemGetIdByName");
    UtTest_Add(UT_os_mut_sem_get_info_test, NULL, NULL, "OS_MutSemGetInfo");

    UtTest_Add(UT_os_queue_create_test, NULL, NULL, "OS_QueueCreate");
    UtTest_Add(UT_os_queue_delete_test, NULL, NULL, "OS_QueueDelete");
    UtTest_Add(UT_os_queue_put_test, NULL, NULL, "OS_QueuePut");
    UtTest_Add(UT_os_queue_get_test, NULL, NULL, "OS_QueueGet");
    UtTest_Add(UT_os_queue_get_id_by_name_test, NULL, NULL, "OS_QueueGetIdByName");
    UtTest_Add(UT_os_queue_get_info_test, NULL, NULL, "OS_QueueGetInfo");

    UtTest_Add(UT_os_select_fd_test, NULL, NULL, "OS_SelectFd");
    UtTest_Add(UT_os_select_single_test, NULL, NULL, "OS_SelectSingle");
    UtTest_Add(UT_os_select_multi_test, NULL, NULL, "OS_SelectMultiple");

    UtTest_Add(
            NULL,
            UT_os_init_task_misc,
            NULL,
            "UT_os_init_task_misc");
    UtTest_Add(
            UT_os_task_create_test,
            UT_os_init_task_create_test,
            NULL,
            "OS_TaskCreate");
    UtTest_Add(
            UT_os_task_delete_test,
            UT_os_init_task_delete_test,
            NULL,
            "OS_TaskDelete");
    UtTest_Add(
            UT_os_task_install_delete_handler_test,
            UT_os_setup_install_delete_handler_test,
            NULL,
            "OS_TaskInstallDeleteHandler");
    UtTest_Add(
            UT_os_task_exit_test,
            UT_os_init_task_exit_test,
            NULL,
            "OS_TaskExit");
    UtTest_Add(
            UT_os_task_delay_test,
            UT_os_init_task_delay_test,
            NULL,
            "OS_TaskDelay");
    UtTest_Add(
            UT_os_task_set_priority_test,
            UT_os_init_task_set_priority_test,
            NULL,
            "OS_TaskSetPriority");
    UtTest_Add(
            UT_os_task_register_test,
            UT_os_init_task_register_test,
            NULL,
            "OS_TaskRegister");
    UtTest_Add(
            UT_os_task_get_id_test,
            UT_os_init_task_get_id_test,
            NULL,
            "OS_TaskGetId");
    UtTest_Add(
            UT_os_task_get_id_by_name_test,
            UT_os_init_task_get_id_by_name_test,
            NULL,
            "OS_TaskGetIdByName");
    UtTest_Add(
            UT_os_task_get_info_test,
            UT_os_init_task_get_info_test,
            NULL,
            "OS_TaskGetInfo");

    UtTest_Add(UT_os_geterrorname_test, NULL, NULL, "OS_GetErrorName");

    UtTest_Add(UT_os_tick2micros_test, NULL, NULL, "OS_Tick2Micros");
    UtTest_Add(UT_os_milli2ticks_test, NULL, NULL, "OS_Milli2Ticks");

    UtTest_Add(UT_os_getlocaltime_test, NULL, NULL, "OS_GetLocalTime");
    UtTest_Add(UT_os_setlocaltime_test, NULL, NULL, "OS_SetLocalTime");

    UtTest_Add(UT_os_heapgetinfo_test, NULL, NULL, "OS_HeapGetInfo");
}

/*================================================================================*
** End of File: ut_oscore_test.c
**================================================================================*/
