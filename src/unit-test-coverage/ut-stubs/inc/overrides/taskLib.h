/* OSAL coverage stub replacement for taskLib.h */
#ifndef _OSAL_STUB_TASKLIB_H_
#define _OSAL_STUB_TASKLIB_H_

/* ----------------------------------------- */
/* constants normally defined in taskLib.h */
/* ----------------------------------------- */
#define OCS_VX_FP_TASK       0x1b01   /* execute with floating-point coprocessor support. */


/* ----------------------------------------- */
/* types normally defined in taskLib.h */
/* ----------------------------------------- */
typedef void* OCS_TASK_ID;
typedef int OCS_BOOL;
typedef int OCS_STATUS;
typedef int (*OCS_FUNCPTR)();

typedef struct OCS_WIND_TCB
{
    int data;
} OCS_WIND_TCB;

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

