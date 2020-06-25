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

/* OSAL coverage stub replacement for taskLib.h */
#ifndef _OSAL_OVERRIDE_TASKLIB_H_
#define _OSAL_OVERRIDE_TASKLIB_H_

#include <OCS_taskLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in taskLib.h */
/* ----------------------------------------- */
#define VX_FP_TASK       OCS_VX_FP_TASK
#define TASK_ID          OCS_TASK_ID
#define WIND_TCB         OCS_WIND_TCB
#define TASK_DESC        OCS_TASK_DESC

#define taskName         OCS_taskName
#define taskNameToId     OCS_taskNameToId
#define taskIdDefault    OCS_taskIdDefault
#define taskIsReady      OCS_taskIsReady
#define taskIsSuspended  OCS_taskIsSuspended
#define taskGetInfo      OCS_taskGetInfo

#define taskActivate     OCS_taskActivate
#define taskExit         OCS_taskExit
#define taskIdSelf       OCS_taskIdSelf
#define taskDelay        OCS_taskDelay
#define taskDelete       OCS_taskDelete
#define taskDeleteForce  OCS_taskDeleteForce
#define taskSuspend      OCS_taskSuspend
#define taskResume       OCS_taskResume
#define taskPrioritySet  OCS_taskPrioritySet
#define taskSpawn        OCS_taskSpawn
#define taskInit         OCS_taskInit
#define taskTcb          OCS_taskTcb


#endif /* _OSAL_OVERRIDE_TASKLIB_H_ */
