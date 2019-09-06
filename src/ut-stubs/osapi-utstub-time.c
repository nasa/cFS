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

UT_DEFAULT_STUB(OS_TimerCbAPI_Init,(void))

/*****************************************************************************
 *
 * Stub function for OS_TimerAdd()
 *
 *****************************************************************************/
int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_ref_id, OS_ArgCallback_t  callback_ptr, void *callback_arg)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerAdd);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(UT_OBJTYPE_TIMECB);
    }
    else
    {
        *timer_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TimerCreate()
 *
 *****************************************************************************/
int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *accuracy, OS_TimerCallback_t  callback_ptr)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerCreate);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(UT_OBJTYPE_TIMECB);
    }
    else
    {
        *timer_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TimerSet()
 *
 *****************************************************************************/
int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerSet);

    return status;
}


/*****************************************************************************/
/**
** \brief OS_TimerDelete stub function
**
** \par Description
**        This function is used as a placeholder for the OS API function
**        OS_TimerDelete.  It always returns OS_ERR_INVALID_ID.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_ERR_INVALID_ID.
**
******************************************************************************/
int32 OS_TimerDelete(uint32 timer_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_TIMECB, timer_id);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TimerGetIdByName()
 *
 *****************************************************************************/
int32 OS_TimerGetIdByName (uint32 *timer_id, const char *timer_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerGetIdByName);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetIdByName), timer_id, sizeof(*timer_id)) < sizeof(*timer_id))
    {
        *timer_id =  1;
        UT_FIXUP_ID(*timer_id, UT_OBJTYPE_TIMECB);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_TimerGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_TimerGetInfo.  The user can adjust the response by setting
**        the values in the OSTimerGetInfoRtn structure prior to this function
**        being called.  If the value OSTimerGetInfoRtn.count is greater than
**        zero then the counter is decremented and the timer creator value is
**        set to the user-defined value OSTimerGetInfoRtn.value.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_TimerGetInfo(uint32 timer_id, OS_timer_prop_t *timer_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimerGetInfo);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetInfo), timer_prop, sizeof(*timer_prop)) < sizeof(*timer_prop))
    {
        memset(timer_prop, 0, sizeof(*timer_prop));
        timer_prop->creator = 1;
        UT_FIXUP_ID(timer_prop->creator, UT_OBJTYPE_TASK);
    }

    return status;
}




