/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
** cfe_psp_config.h
*/

#ifndef _cfe_psp_config_
#define _cfe_psp_config_



#include "common_types.h"

#include <signal.h>
#include <time.h>
#include <pthread.h>

/*
** This define sets the number of memory ranges that are defined in the memory range defintion
** table.
*/
#define CFE_PSP_MEM_TABLE_SIZE 10

/**
 * This define sets the maximum number of exceptions
 * that can be stored.
 *
 * It must always be a power of two.
 */
#define CFE_PSP_MAX_EXCEPTION_ENTRIES           4
#define CFE_PSP_MAX_EXCEPTION_BACKTRACE_SIZE    16


/*
 * A random 32-bit value that is used as the "validity flag"
 * of the PC-Linux boot record structure.  This is simply
 * a value that is unlikely to occur unless specifically set.
 */
#define CFE_PSP_BOOTRECORD_VALID            ((uint32)0x2aebe984)
#define CFE_PSP_BOOTRECORD_INVALID          (~CFE_PSP_BOOTRECORD_VALID)

/*
 * The amount of time to wait for an orderly shutdown
 * in the event of a call to CFE_PSP_Restart()
 *
 * If this expires, then an abnormal exit/abort() is triggered.
 */
#define CFE_PSP_RESTART_DELAY               10000

/* use the "USR1" signal to wake the idle thread when an exception occurs */
#define CFE_PSP_EXCEPTION_EVENT_SIGNAL      SIGUSR1


/*
** Global variables
*/

/*
** Typedef for the header structure on the reserved memory block
**
** Note that the structure below reserves memory sizes defined
** at compile time directly from cfe_platform_cfg.h above.
** A future enhancement should reserve blocks based on the runtime
** size in GLOBAL_CONFIGDATA.
*/
typedef struct
{
    uint32 ValidityFlag;
    uint32 NextResetType;

} CFE_PSP_ReservedMemoryBootRecord_t;

/*
 * The state of the PSP "idle task"
 *
 * This is the main/initial thread that runs early init,
 * it is NOT an OSAL task.
 *
 * Once initialized, this thread goes idle and waits for
 * asynchronous events to occur, and resumes with an orderly
 * shutdown if requested.
 */
typedef struct
{
    pthread_t ThreadID;
    volatile bool ShutdownReq;
} CFE_PSP_IdleTaskState_t;



/**
 * \brief The data type used by the underlying OS to represent a thread ID.
 */
typedef pthread_t CFE_PSP_Exception_SysTaskId_t;

/**
 * \brief Exception context data which is relevant for offline/post-mortem diagnosis.
 *
 * This may be stored in a persistent exception log file for later analysis.
 */
typedef struct
{
    struct timespec event_time;
    siginfo_t si;

    /*
     * Note this is a variably-filled array based on the number of addresses
     * reported by the library.  It should be last.
     */
    void *bt_addrs[CFE_PSP_MAX_EXCEPTION_BACKTRACE_SIZE];
} CFE_PSP_Exception_ContextDataEntry_t;

/*
** Watchdog minimum and maximum values ( in milliseconds )
*/
#define CFE_PSP_WATCHDOG_MIN (0)
#define CFE_PSP_WATCHDOG_MAX (0xFFFFFFFF) 

/*
** Number of EEPROM banks on this platform
*/
#define CFE_PSP_NUM_EEPROM_BANKS 1

/*
 * Information about the "idle task" --
 * this is used by exception handling to wake it when an event occurs
 */
extern CFE_PSP_IdleTaskState_t  CFE_PSP_IdleTaskState;

#endif

