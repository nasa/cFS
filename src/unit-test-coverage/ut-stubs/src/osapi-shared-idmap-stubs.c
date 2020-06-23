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

/**
 * \file     osapi-shared-idmap-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "os-shared-idmap.h"

OS_common_record_t OS_stub_task_table[OS_MAX_TASKS];
OS_common_record_t OS_stub_queue_table[OS_MAX_QUEUES];
OS_common_record_t OS_stub_bin_sem_table[OS_MAX_BIN_SEMAPHORES];
OS_common_record_t OS_stub_count_sem_table[OS_MAX_COUNT_SEMAPHORES];
OS_common_record_t OS_stub_mutex_table[OS_MAX_MUTEXES];
OS_common_record_t OS_stub_console_table[OS_MAX_CONSOLES];
OS_common_record_t OS_stub_module_table[OS_MAX_MODULES];
OS_common_record_t OS_stub_filesys_table[OS_MAX_FILE_SYSTEMS];
OS_common_record_t OS_stub_timebase_table[OS_MAX_TIMEBASES];
OS_common_record_t OS_stub_timecb_table[OS_MAX_TIMERS];
OS_common_record_t OS_stub_stream_table[OS_MAX_NUM_OPEN_FILES];
OS_common_record_t OS_stub_dir_table[OS_MAX_NUM_OPEN_DIRS];

OS_common_record_t * const OS_global_task_table         = OS_stub_task_table;
OS_common_record_t * const OS_global_queue_table        = OS_stub_queue_table;
OS_common_record_t * const OS_global_bin_sem_table      = OS_stub_bin_sem_table;
OS_common_record_t * const OS_global_count_sem_table    = OS_stub_count_sem_table;
OS_common_record_t * const OS_global_mutex_table        = OS_stub_mutex_table;
OS_common_record_t * const OS_global_stream_table       = OS_stub_stream_table;
OS_common_record_t * const OS_global_dir_table          = OS_stub_dir_table;
OS_common_record_t * const OS_global_timebase_table     = OS_stub_timebase_table;
OS_common_record_t * const OS_global_timecb_table       = OS_stub_timecb_table;
OS_common_record_t * const OS_global_module_table       = OS_stub_module_table;
OS_common_record_t * const OS_global_filesys_table      = OS_stub_filesys_table;
OS_common_record_t * const OS_global_console_table      = OS_stub_console_table;
