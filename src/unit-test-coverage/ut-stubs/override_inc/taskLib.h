/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
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
