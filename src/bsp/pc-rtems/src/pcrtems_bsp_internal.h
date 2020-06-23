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

/*
 * File:  pcrtems_bsp_internal.h
 *
 * Purpose:
 *   Header file for internal data to the PC-RTEMS BSP
 */

#ifndef _PCRTEMS_BSP_INTERNAL_H_
#define _PCRTEMS_BSP_INTERNAL_H_

/*
** OSAL includes
*/
#include "osapi.h"
#include "bsp-impl.h"

/*
 * BSP compile-time tuning
 */
#define RTEMS_MAX_USER_OPTIONS   4
#define RTEMS_MAX_CMDLINE        256

/*
 * The location which the general purpose file system will be mounted
 */
#define RTEMS_USER_FS_MOUNTPOINT    "/mnt"

/*
 * By default put the shell at the same priority
 * as the utility task which handles OS_printf()
 */
#define RTEMS_SHELL_PRIORITY     OS_UTILITYTASK_PRIORITY

/*
** BSP types
*/
typedef struct
{
    char UserArgBuffer[RTEMS_MAX_CMDLINE];
    bool BatchMode;
} OS_BSP_PcRtemsGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_PcRtemsGlobalData_t OS_BSP_PcRtemsGlobal;

#endif /* _PCRTEMS_BSP_INTERNAL_H_ */
