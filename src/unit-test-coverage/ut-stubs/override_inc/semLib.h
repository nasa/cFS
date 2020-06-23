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

/* OSAL coverage stub replacement for semLib.h */
#ifndef _OSAL_OVERRIDE_SEMLIB_H_
#define _OSAL_OVERRIDE_SEMLIB_H_

#include <OCS_semLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in semLib.h */
/* ----------------------------------------- */

#define VX_BINARY_SEMAPHORE(x)     OCS_VX_BINARY_SEMAPHORE(x)
#define VX_COUNTING_SEMAPHORE(x)   OCS_VX_COUNTING_SEMAPHORE(x)
#define VX_MUTEX_SEMAPHORE(x)      OCS_VX_MUTEX_SEMAPHORE(x)

#define SEM_ID                     OCS_SEM_ID
#define SEM_B_STATE                OCS_SEM_B_STATE
#define SEM_EMPTY                  OCS_SEM_EMPTY
#define SEM_FULL                   OCS_SEM_FULL
#define SEM_Q_FIFO                 OCS_SEM_Q_FIFO
#define SEM_Q_PRIORITY             OCS_SEM_Q_PRIORITY
#define SEM_DELETE_SAFE            OCS_SEM_DELETE_SAFE
#define SEM_INVERSION_SAFE         OCS_SEM_INVERSION_SAFE
#define SEM_EVENTSEND_ERR_NOTIFY   OCS_SEM_EVENTSEND_ERR_NOTIFY

#define semBInitialize          OCS_semBInitialize
#define semBCreate              OCS_semBCreate
#define semMInitialize          OCS_semMInitialize
#define semMCreate              OCS_semMCreate
#define semCInitialize          OCS_semCInitialize
#define semCCreate              OCS_semCCreate
#define semDelete               OCS_semDelete
#define semFlush                OCS_semFlush
#define semTake                 OCS_semTake
#define semGive                 OCS_semGive



#endif /* _OSAL_OVERRIDE_SEMLIB_H_ */
