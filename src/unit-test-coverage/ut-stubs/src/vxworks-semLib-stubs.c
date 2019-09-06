/* OSAL coverage stub replacement for semLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/semLib.h>

static int LOCAL_UT_SEM;

OCS_SEM_ID OCS_semBInitialize(char *pSemMem, int options, OCS_SEM_B_STATE initialState)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semBInitialize);
    if (Status == 0)
    {
        retval = (OCS_SEM_ID)pSemMem;
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
OCS_SEM_ID OCS_semMInitialize(char *pSemMem, int options)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semMInitialize);
    if (Status == 0)
    {
        retval = (OCS_SEM_ID)pSemMem;
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
OCS_SEM_ID OCS_semCInitialize(char *pSemMem, int options, int initialCount)
{
    OCS_SEM_ID retval = NULL;
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_semCInitialize);
    if (Status == 0)
    {
        retval = (OCS_SEM_ID)pSemMem;
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


