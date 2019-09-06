/*
** File: osapi.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Alan Cudmore - Code 582
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Core OS module
**
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

#define OS_SUCCESS                     (0)
#define OS_ERROR                       (-1)
#define OS_INVALID_POINTER             (-2)
#define OS_ERROR_ADDRESS_MISALIGNED    (-3)
#define OS_ERROR_TIMEOUT               (-4)
#define OS_INVALID_INT_NUM             (-5)
#define OS_SEM_FAILURE                 (-6)
#define OS_SEM_TIMEOUT                 (-7)
#define OS_QUEUE_EMPTY                 (-8)
#define OS_QUEUE_FULL                  (-9)
#define OS_QUEUE_TIMEOUT               (-10)
#define OS_QUEUE_INVALID_SIZE          (-11)
#define OS_QUEUE_ID_ERROR              (-12)
#define OS_ERR_NAME_TOO_LONG           (-13)
#define OS_ERR_NO_FREE_IDS             (-14)
#define OS_ERR_NAME_TAKEN              (-15)
#define OS_ERR_INVALID_ID              (-16)
#define OS_ERR_NAME_NOT_FOUND          (-17)
#define OS_ERR_SEM_NOT_FULL            (-18)
#define OS_ERR_INVALID_PRIORITY        (-19)
#define OS_INVALID_SEM_VALUE           (-20)
#define OS_ERR_FILE                    (-27)
#define OS_ERR_NOT_IMPLEMENTED         (-28)
#define OS_TIMER_ERR_INVALID_ARGS      (-29)
#define OS_TIMER_ERR_TIMER_ID          (-30)
#define OS_TIMER_ERR_UNAVAILABLE       (-31)
#define OS_TIMER_ERR_INTERNAL          (-32)
#define OS_ERR_OBJECT_IN_USE           (-33)
#define OS_ERR_BAD_ADDRESS             (-34)
#define OS_ERR_INCORRECT_OBJ_STATE     (-35)
#define OS_ERR_INCORRECT_OBJ_TYPE      (-36)
#define OS_ERR_STREAM_DISCONNECTED     (-37)

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

