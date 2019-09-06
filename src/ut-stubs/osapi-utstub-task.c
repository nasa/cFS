/*
 *  Copyright (c) 2004-2018, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

UT_DEFAULT_STUB(OS_TaskAPI_Init,(void))


/*****************************************************************************/
/**
** \brief OS_TaskCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskCreate.  The user can adjust the response by setting the value
**        of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskCreate(uint32 *task_id, const char *task_name,
                    osal_task_entry function_pointer,
                    uint32 *stack_pointer,
                    uint32 stack_size, uint32 priority,
                    uint32 flags)
{
    int32 status = OS_SUCCESS;

    UT_Stub_RegisterContext(UT_KEY(OS_TaskCreate), &function_pointer);
    UT_Stub_RegisterContext(UT_KEY(OS_TaskCreate), stack_pointer);

    status = UT_DEFAULT_IMPL(OS_TaskCreate);

    if (status == OS_SUCCESS)
    {
        *task_id = UT_AllocStubObjId(UT_OBJTYPE_TASK);
    }
    else
    {
        *task_id = 0xDEADBEEFU;
    }


    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskDelete.  The user can adjust the response by
**        setting the value of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskDelete(uint32 task_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TaskDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_TASK, task_id);
    }

    return status;
}


/*****************************************************************************/
/**
** \brief OS_TaskExit stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskExit.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        This function does not return a value.
**
******************************************************************************/
void OS_TaskExit()
{
    /* Although this has no retcode, invoke the hooks provided in the default impl.
     * NOTE: historically CFE UT checks for a "1" output via its sideband methods. */
    UT_DEFAULT_IMPL_RC(OS_TaskExit, 1);
}

/*****************************************************************************/
/**
** \brief OS_TaskDelay stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskDelay.  The user can adjust the response by setting the value
**        of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskDelay(uint32 millisecond)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TaskDelay);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TaskSetPriority()
 *
 *****************************************************************************/
int32 OS_TaskSetPriority (uint32 task_id, uint32 new_priority)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_TaskSetPriority);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TaskRegister stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskRegister.  The user can adjust the response by setting the value
**        of UT_OS_Fail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_SUCCESS or OS_ERROR.
**
******************************************************************************/
int32 OS_TaskRegister(void)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_TaskRegister);

    return status;
}


/*****************************************************************************/
/**
** \brief OS_TaskGetId stub function
**
** \par Description
**        This function is used as a placeholder for the OS API function
**        OS_TaskGetId.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns 1 unless an override value is configured.
**
******************************************************************************/
uint32 OS_TaskGetId(void)
{
    int32 TaskId;

    TaskId = 1;
    UT_FIXUP_ID(TaskId, UT_OBJTYPE_TASK);
    TaskId = UT_DEFAULT_IMPL_RC(OS_TaskGetId, TaskId);

    return TaskId;
}


/*****************************************************************************
 *
 * Stub function for OS_TaskGetIdByName()
 *
 *****************************************************************************/
int32 OS_TaskGetIdByName (uint32 *task_id, const char *task_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TaskGetIdByName);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_TaskGetIdByName), task_id, sizeof(*task_id)) < sizeof(*task_id))
    {
        *task_id =  1;
        UT_FIXUP_ID(*task_id, UT_OBJTYPE_TASK);
    }

    return status;
}


/*****************************************************************************/
/**
** \brief OS_TaskGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TaskGetInfo.  If the input structure, task_prop, is null, it
**        returns OS_INVALID_POINTER.  Otherwise it sets the task structure
**        variables to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either OS_INVALID_POINTER or OS_SUCCESS.
**
******************************************************************************/
int32 OS_TaskGetInfo(uint32 task_id, OS_task_prop_t *task_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TaskGetInfo);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_MutSemGetInfo), task_prop, sizeof(*task_prop)) < sizeof(*task_prop))
    {
        task_prop->creator = 1;
        UT_FIXUP_ID(task_prop->creator, UT_OBJTYPE_TASK);
        task_prop->OStask_id = task_id & 0xFFFF;
        task_prop->stack_size = 100;
        task_prop->priority = 150;
        strncpy(task_prop->name, "UnitTest", OS_MAX_API_NAME - 1);
        task_prop->name[OS_MAX_API_NAME - 1] = '\0';
    }

    return status;
}


/*****************************************************************************
 *
 * Stub function for OS_TaskInstallDeleteHandler()
 *
 *****************************************************************************/
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer)
{
    int32 status;

    UT_Stub_RegisterContext(UT_KEY(OS_TaskInstallDeleteHandler), &function_pointer);

    status = UT_DEFAULT_IMPL(OS_TaskInstallDeleteHandler);

    return (status);
}

/*****************************************************************************
 *
 * Stub function for OS_TaskEntryPoint()
 *
 * This is an internal function but it needs a stub in order to test
 * the low level implementation that uses the shared layer.
 *
 *****************************************************************************/
void OS_TaskEntryPoint(uint32 task_id)
{
    UT_DEFAULT_IMPL(OS_TaskEntryPoint);
}
