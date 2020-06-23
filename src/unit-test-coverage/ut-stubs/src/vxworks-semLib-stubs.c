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


