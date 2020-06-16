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

/**
 * \file cfe_psp_exceptionstorage_types.h
 *
 * Provides a generic storage buffer ring for exceptions
 */

#ifndef CFE_PSP_EXCEPTIONSTORAGE_TYPES_H_
#define CFE_PSP_EXCEPTIONSTORAGE_TYPES_H_

#include <cfe_psp.h>
#include <cfe_psp_config.h>

/*
 * The "MetaData" stores ephemeral exception information
 * which only has meaning within the currently-running process.
 *
 * This data is important for diagnosing the exception, but it
 * is NOT saved to any persistent log because it will not be
 * relevant once the process ends.
 */
struct CFE_PSP_Exception_LogData
{
    uint32 context_id;                          /**< a unique ID assigned to this exception entry */
    uint32 context_size;                        /**< actual size of the "context_info" data */
    CFE_PSP_Exception_SysTaskId_t sys_task_id;  /**< the BSP-specific task info (not osal abstracted id) */
    CFE_PSP_Exception_ContextDataEntry_t context_info;
};


struct CFE_PSP_ExceptionStorage
{
    volatile uint32 NumWritten;
    volatile uint32 NumRead;
    struct CFE_PSP_Exception_LogData Entries[CFE_PSP_MAX_EXCEPTION_ENTRIES];
};

typedef struct CFE_PSP_Exception_LogData    CFE_PSP_Exception_LogData_t;
typedef struct CFE_PSP_ExceptionStorage     CFE_PSP_ExceptionStorage_t;


#endif /* CFE_PSP_EXCEPTIONSTORAGE_TYPES_H_ */
