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
 * File: osapi.h
 *
 * Author:  Alan Cudmore - Code 582
 *
 * Purpose: Contains functions prototype definitions and variables declarations
 *          for the OS Abstraction Layer, Core OS module
 */

#ifndef _osapi_
#define _osapi_

/*
 * Note - the "osapi-os-filesys.h" file previously included these system headers
 * plus a couple others.  Some existing code used stdio/stdlib functions but did
 * not #include them, relying on the implicit inclusion that osapi-os-filesys
 * provided.
 *
 * osapi-os-filesys.h now does a more complete abstraction and does not base its
 * API on the system API anymore.  However, in order to not break other code that
 * relied on the implicit inclusion of these system files, these are added here.
 *
 * (The reasoning is that stdio/stdlib are widely used and well standardized
 * across all C implementations, even embedded/lightweight C libraries should
 * have some usable version of these includes)
 */
#include <stdio.h>
#include <stdlib.h>

#include "common_types.h"

#ifdef __cplusplus
   extern "C" {
#endif

/** @defgroup OSReturnCodes OSAL Return Code Defines
 * @{
 */
#define OS_SUCCESS                     (0)   /**< @brief Successful execution */
#define OS_ERROR                       (-1)  /**< @brief Failed execution */
#define OS_INVALID_POINTER             (-2)  /**< @brief Invalid pointer */
#define OS_ERROR_ADDRESS_MISALIGNED    (-3)  /**< @brief Address misalignment */
#define OS_ERROR_TIMEOUT               (-4)  /**< @brief Error timeout */
#define OS_INVALID_INT_NUM             (-5)  /**< @brief Invalid Interrupt number */
#define OS_SEM_FAILURE                 (-6)  /**< @brief Semaphore failure */
#define OS_SEM_TIMEOUT                 (-7)  /**< @brief Semaphore timeout */
#define OS_QUEUE_EMPTY                 (-8)  /**< @brief Queue empty */
#define OS_QUEUE_FULL                  (-9)  /**< @brief Queue full */
#define OS_QUEUE_TIMEOUT               (-10) /**< @brief Queue timeout */
#define OS_QUEUE_INVALID_SIZE          (-11) /**< @brief Queue invalid size */
#define OS_QUEUE_ID_ERROR              (-12) /**< @brief Queue ID error */
#define OS_ERR_NAME_TOO_LONG           (-13) /**< @brief name length including null terminator greater than #OS_MAX_API_NAME */
#define OS_ERR_NO_FREE_IDS             (-14) /**< @brief No free IDs */
#define OS_ERR_NAME_TAKEN              (-15) /**< @brief Name taken */
#define OS_ERR_INVALID_ID              (-16) /**< @brief Invalid ID */
#define OS_ERR_NAME_NOT_FOUND          (-17) /**< @brief Name not found */
#define OS_ERR_SEM_NOT_FULL            (-18) /**< @brief Semaphore not full */
#define OS_ERR_INVALID_PRIORITY        (-19) /**< @brief Invalid priority */
#define OS_INVALID_SEM_VALUE           (-20) /**< @brief Invalid semaphore value */
#define OS_ERR_FILE                    (-27) /**< @brief File error */
#define OS_ERR_NOT_IMPLEMENTED         (-28) /**< @brief Not implemented */
#define OS_TIMER_ERR_INVALID_ARGS      (-29) /**< @brief Timer invalid arguments */
#define OS_TIMER_ERR_TIMER_ID          (-30) /**< @brief Timer ID error */
#define OS_TIMER_ERR_UNAVAILABLE       (-31) /**< @brief Timer unavailable */
#define OS_TIMER_ERR_INTERNAL          (-32) /**< @brief Timer internal error */
#define OS_ERR_OBJECT_IN_USE           (-33) /**< @brief Object in use */
#define OS_ERR_BAD_ADDRESS             (-34) /**< @brief Bad address */
#define OS_ERR_INCORRECT_OBJ_STATE     (-35) /**< @brief Incorrect object state */
#define OS_ERR_INCORRECT_OBJ_TYPE      (-36) /**< @brief Incorrect object type */
#define OS_ERR_STREAM_DISCONNECTED     (-37) /**< @brief Stream disconnected */
#define OS_ERR_OPERATION_NOT_SUPPORTED (-38) /**< @brief Requested operation is not support on the supplied object(s) */
/**@}*/

/*
** Defines for Queue Timeout parameters
*/
#define OS_PEND   (-1)
#define OS_CHECK  (0)

#include "osapi-version.h"

/*
** Include the configuration file
*/
#include "osconfig.h"

/*
** Include the OS API modules
*/
#include "osapi-os-core.h"
#include "osapi-os-filesys.h"
#include "osapi-os-net.h"
#include "osapi-os-loader.h"
#include "osapi-os-timer.h"

/*
 ******************************************************************************
 * Items below here are internal OSAL-use definitions and are not part of the
 * OSAL API
 *****************************************************************************
 */

#ifdef __cplusplus
   }
#endif

#endif

