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
#ifndef _OSAL_STUB_TASKLIB_H_
#define _OSAL_STUB_TASKLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in taskLib.h */
/* ----------------------------------------- */
#define OCS_VX_FP_TASK       0x1b01   /* execute with floating-point coprocessor support. */


/* ----------------------------------------- */
/* types normally defined in taskLib.h */
/* ----------------------------------------- */
typedef struct OCS_WIND_TCB
{
    int data;
} OCS_WIND_TCB;

typedef struct OCS_WIND_TCB* OCS_TASK_ID;

typedef struct OCS_TASK_DESC
{
    OCS_TASK_ID tid;
} OCS_TASK_DESC;


/* ----------------------------------------- */
/* prototypes normally declared in taskInfo.h */
/* ----------------------------------------- */

extern const char *OCS_taskName(OCS_TASK_ID task_id);
extern OCS_TASK_ID OCS_taskNameToId(const char *name);
extern OCS_TASK_ID OCS_taskIdDefault(OCS_TASK_ID task_id);
extern OCS_BOOL OCS_taskIsReady(OCS_TASK_ID task_id);
extern OCS_BOOL OCS_taskIsSuspended (OCS_TASK_ID task_id);
extern OCS_STATUS OCS_taskGetInfo(OCS_TASK_ID task_id, OCS_TASK_DESC *desc);



/* ----------------------------------------- */
/* prototypes normally declared in taskLib.h */
/* ----------------------------------------- */

extern OCS_STATUS OCS_taskActivate(OCS_TASK_ID  tid);
extern void OCS_taskExit(int code);
extern OCS_TASK_ID OCS_taskIdSelf(void);
extern OCS_STATUS OCS_taskDelay(int ticks);
extern OCS_STATUS OCS_taskDelete(OCS_TASK_ID tid);
extern OCS_STATUS OCS_taskDeleteForce(OCS_TASK_ID tid);
extern OCS_STATUS OCS_taskSuspend(OCS_TASK_ID tid);
extern OCS_STATUS OCS_taskResume(OCS_TASK_ID tid);
extern OCS_STATUS OCS_taskPrioritySet(OCS_TASK_ID tid, int newPriority);
extern OCS_TASK_ID OCS_taskSpawn(char *  name,
              int     priority,
              int     options,
              int     stackSize,
              OCS_FUNCPTR entryPt,
              int     arg1,
              int     arg2,
              int     arg3,
              int     arg4,
              int     arg5,
              int     arg6,
              int     arg7,
              int     arg8,
              int     arg9,
              int     arg10);

OCS_STATUS OCS_taskInit(OCS_WIND_TCB *pTcb,
        char *name,
        int priority,
        int options,
        char * pStackBase,
        int stackSize,
        OCS_FUNCPTR entryPt,
        int arg1,
        int arg2,
        int arg3,
        int arg4,
        int arg5,
        int arg6,
        int arg7,
        int arg8,
        int arg9,
        int arg10
);

OCS_WIND_TCB *OCS_taskTcb(OCS_TASK_ID tid);


#endif /* _OSAL_STUB_TASKLIB_H_ */

