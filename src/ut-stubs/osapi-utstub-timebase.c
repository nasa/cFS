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


UT_DEFAULT_STUB(OS_TimeBaseAPI_Init,(void))

/*****************************************************************************
 *
 * Stub for OS_TimeBaseCreate() function
 *
 *****************************************************************************/
int32 OS_TimeBaseCreate(uint32 *timebase_id, const char *timebase_name, OS_TimerSync_t external_sync)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseCreate);

    if (status == OS_SUCCESS)
    {
        *timebase_id = UT_AllocStubObjId(UT_OBJTYPE_TIMEBASE);
    }
    else
    {
        *timebase_id = 0xDEADBEEFU;
    }

    return status;
}


/*****************************************************************************
 *
 * Stub for OS_TimeBaseSet() function
 *
 *****************************************************************************/
int32 OS_TimeBaseSet(uint32 timebase_id, uint32 start_time, uint32 interval_time)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseSet);

    return status;
}


/*****************************************************************************
 *
 * Stub for OS_TimeBaseDelete() function
 *
 *****************************************************************************/
int32 OS_TimeBaseDelete(uint32 timebase_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_TIMEBASE, timebase_id);
    }

    return status;
}


/*****************************************************************************
 *
 * Stub for OS_TimeBaseGetIdByName() function
 *
 *****************************************************************************/
int32 OS_TimeBaseGetIdByName (uint32 *timebase_id, const char *timebase_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseGetIdByName);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetIdByName), timebase_id, sizeof(*timebase_id)) < sizeof(*timebase_id))
    {
        *timebase_id =  1;
        UT_FIXUP_ID(*timebase_id, UT_OBJTYPE_TIMEBASE);
    }

    return status;
}


/*****************************************************************************
 *
 * Stub for OS_TimeBaseGetInfo() function
 *
 *****************************************************************************/
int32 OS_TimeBaseGetInfo (uint32 timebase_id, OS_timebase_prop_t *timebase_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseGetInfo);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetInfo), timebase_prop, sizeof(*timebase_prop)) < sizeof(*timebase_prop))
    {
        timebase_prop->creator = 1;
        UT_FIXUP_ID(timebase_prop->creator, UT_OBJTYPE_TASK);
        strncpy(timebase_prop->name, "Name", OS_MAX_API_NAME - 1);
        timebase_prop->name[OS_MAX_API_NAME - 1] = '\0';
    }


    return status;
}

/*****************************************************************************
 *
 * Stub for OS_TimeBaseGetFreeRun() function
 *
 *****************************************************************************/
int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TimeBaseGetFreeRun);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_TimeBaseGetFreeRun), freerun_val, sizeof(*freerun_val)) < sizeof(*freerun_val))
    {
        int32 tempcount;
        int32 temprc;

        /*
         * Use the call count such that the value increases with each successive call.
         * If that doesn't work then just return a constant nonzero value.
         */
        if (UT_GetStubRetcodeAndCount(UT_KEY(OS_TimeBaseGetFreeRun), &temprc, &tempcount))
        {
            *freerun_val = tempcount;
        }
        else
        {
            *freerun_val = 1;
        }
    }

    return status;

}

/*****************************************************************************
 *
 * Stub for OS_TimeBase_CallbackThread() function
 *
 *****************************************************************************/
void OS_TimeBase_CallbackThread(uint32 timebase_id)
{
    UT_DEFAULT_IMPL(OS_TimeBase_CallbackThread);
}

/*****************************************************************************
 *
 * Stub for OS_Tick2Micros() function
 *
 *****************************************************************************/
int32 OS_Tick2Micros (void)
{
    return UT_DEFAULT_IMPL_RC(OS_Tick2Micros,100);
}

/*****************************************************************************
 *
 * Stub for OS_Milli2Ticks() function
 *
 *****************************************************************************/
int32 OS_Milli2Ticks(uint32 milli_seconds)
{
    return UT_DEFAULT_IMPL_RC(OS_Milli2Ticks,100);
}
