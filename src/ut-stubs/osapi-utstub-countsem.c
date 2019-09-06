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


UT_DEFAULT_STUB(OS_CountSemAPI_Init,(void))

/*****************************************************************************
 *
 * Stub function for OS_CountSemCreate()
 *
 *****************************************************************************/
int32 OS_CountSemCreate(uint32 *sem_id, const char *sem_name,
                      uint32 sem_initial_value, uint32 options)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemCreate);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(UT_OBJTYPE_COUNTSEM);
    }
    else
    {
        *sem_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_CountSemDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_CountSemDelete.  The user can adjust the response by setting
**        the values in the CountSemDelRtn structure prior to this function
**        being called.  If the value CountSemDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value CountSemDelRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_CountSemDelete(uint32 sem_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_COUNTSEM, sem_id);
    }

    return status;
}


/*****************************************************************************
 *
 * Stub for OS_CountSemGive() function
 *
 *****************************************************************************/
int32 OS_CountSemGive ( uint32 sem_id )
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemGive);

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_CountSemTake() function
 *
 *****************************************************************************/
int32 OS_CountSemTake ( uint32 sem_id )
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemTake);

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_CountSemTimedWait() function
 *
 *****************************************************************************/
int32 OS_CountSemTimedWait ( uint32 sem_id, uint32 msecs )
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemTimedWait);

    return status;
}

/*****************************************************************************
 *
 * Stub for OS_CountSemGetIdByName() function
 *
 *****************************************************************************/
int32 OS_CountSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemGetIdByName);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_CountSemGetIdByName), sem_id, sizeof(*sem_id)) < sizeof(*sem_id))
    {
        *sem_id =  1;
        UT_FIXUP_ID(*sem_id, UT_OBJTYPE_COUNTSEM);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_CountSemGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_CountSemGetInfo.  It sets the counting semaphore structure
**        variables to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_CountSemGetInfo(uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CountSemGetInfo);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_CountSemGetInfo), count_prop, sizeof(*count_prop)) < sizeof(*count_prop))
    {
        count_prop->creator = 1;
        UT_FIXUP_ID(count_prop->creator, UT_OBJTYPE_TASK);
        strncpy(count_prop->name, "Name", OS_MAX_API_NAME - 1);
        count_prop->name[OS_MAX_API_NAME - 1] = '\0';
    }


    return status;
}



