/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for semLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_semLib.h>

static OCS_SEM LOCAL_UT_SEM = { 0 };

OCS_SEM_ID OCS_semBInitialize(OCS_SEM *pSemMem, int options, OCS_SEM_B_STATE initialState)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semBInitialize);
    if (Status == 0)
    {
        retval = pSemMem;
    }

    return retval;
}
OCS_SEM_ID OCS_semBCreate(int options, OCS_SEM_B_STATE initialState)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semBCreate);
    if (Status == 0)
    {
        retval = &LOCAL_UT_SEM;
    }

    return retval;
}
OCS_SEM_ID OCS_semMInitialize(OCS_SEM *pSemMem, int options)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semMInitialize);
    if (Status == 0)
    {
        retval = pSemMem;
    }

    return retval;
}
OCS_SEM_ID OCS_semMCreate(int options)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semMCreate);
    if (Status == 0)
    {
        retval = &LOCAL_UT_SEM;
    }

    return retval;
}
OCS_SEM_ID OCS_semCInitialize(OCS_SEM *pSemMem, int options, int initialCount)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semCInitialize);
    if (Status == 0)
    {
        retval = pSemMem;
    }

    return retval;
}
OCS_SEM_ID OCS_semCCreate(int flags, int count)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semCCreate);
    if (Status == 0)
    {
        retval = &LOCAL_UT_SEM;
    }

    return retval;
}

OCS_STATUS OCS_semDelete(OCS_SEM_ID semId)
{
    return (UT_DEFAULT_IMPL(OCS_semDelete));
}
OCS_STATUS OCS_semFlush(OCS_SEM_ID semId)
{
    return (UT_DEFAULT_IMPL(OCS_semFlush));
}
OCS_STATUS OCS_semTake(OCS_SEM_ID semId, int timeout)
{
    return (UT_DEFAULT_IMPL(OCS_semTake));
}
OCS_STATUS OCS_semGive(OCS_SEM_ID semId)
{
    return (UT_DEFAULT_IMPL(OCS_semGive));
}


