/* OSAL coverage stub replacement for taskLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/taskLib.h>
#include <overrides/errnoLib.h>

static OCS_WIND_TCB OCS_LOCAL_TASK = { 0 };

void OCS_taskExit(int code)
{
    UT_DEFAULT_IMPL(OCS_taskExit);
}
OCS_TASK_ID OCS_taskIdSelf(void)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_taskIdSelf);
    if (Status != 0)
    {
        return ((OCS_TASK_ID)OCS_ERROR);
    }

    return &OCS_LOCAL_TASK;
}
OCS_TASK_ID OCS_taskNameToId(const char *name)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_taskNameToId);
    if (Status != 0)
    {
        return ((OCS_TASK_ID)OCS_ERROR);
    }

    return &OCS_LOCAL_TASK;
}
OCS_STATUS OCS_taskDelay(int ticks)
{
    return (UT_DEFAULT_IMPL(OCS_taskDelay));
}
OCS_STATUS OCS_taskDelete(OCS_TASK_ID tid)
{
    return (UT_DEFAULT_IMPL(OCS_taskDelete));
}
OCS_STATUS OCS_taskDeleteForce(OCS_TASK_ID tid)
{
    return (UT_DEFAULT_IMPL(OCS_taskDeleteForce));
}
OCS_STATUS OCS_taskSuspend(OCS_TASK_ID tid)
{
    return (UT_DEFAULT_IMPL(OCS_taskSuspend));
}
OCS_STATUS OCS_taskResume(OCS_TASK_ID tid)
{
    return (UT_DEFAULT_IMPL(OCS_taskResume));
}
OCS_STATUS OCS_taskPrioritySet(OCS_TASK_ID tid, int newPriority)
{
    return (UT_DEFAULT_IMPL(OCS_taskPrioritySet));
}

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
)
{
    return (UT_DEFAULT_IMPL(OCS_taskInit));
}

OCS_TASK_ID OCS_taskSpawn(char *  name,
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
              int     arg10)

{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_taskSpawn);
    if (Status != 0)
    {
        return ((OCS_TASK_ID)OCS_ERROR);
    }

    return &OCS_LOCAL_TASK;
}

OCS_STATUS OCS_taskActivate(OCS_TASK_ID tid)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_taskActivate);

    return Status;
}

OCS_WIND_TCB *OCS_taskTcb(OCS_TASK_ID tid)
{
    int32 Status;
    OCS_WIND_TCB *LocalTcb;

    Status = UT_DEFAULT_IMPL(OCS_taskTcb);
    if (Status != 0)
    {
        return (NULL);
    }

    if (UT_Stub_CopyToLocal(UT_KEY(OCS_taskTcb), &LocalTcb, sizeof(LocalTcb)) < sizeof(LocalTcb))
    {
        /*
         * On VxWorks the TASK_ID is defined as a direct type cast
         * of the TCB address.  This is actually documented
         * in the API and application code that works with TCBs
         * certainly will depend on this being the case.
         */
        LocalTcb = (OCS_WIND_TCB *)tid;
    }

    return LocalTcb;
}

