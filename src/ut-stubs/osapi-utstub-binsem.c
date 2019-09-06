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

UT_DEFAULT_STUB(OS_BinSemAPI_Init,(void))

/*****************************************************************************/
/**
** \brief OS_BinSemTake stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemTake.  The user can adjust the response by setting
**        the value of UT_BinSemFail, causing it to return a failure
**        indication (-1) on the first or second call to the function.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either -1 or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemTake(uint32 sem_id)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_BinSemTake);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemFlush stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemFlush.  The variable OSBinSemFlushRtn.value is set to the
**        value passed to the function, reset_type, and the variable
**        OSBinSemFlushRtn.count is incremented each time this function is
**        called.  The unit tests compare these values to expected results to
**        verify proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemFlush(uint32 sem_id)
{
    int32   status;

    status = UT_DEFAULT_IMPL(OS_BinSemFlush);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemCreate.  The user can adjust the response by setting the
**        values in the OS_BinSemCreateRtn structure prior to this function
**        being called.  If the value OS_BinSemCreateRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value
**        OS_BinSemCreateRtn.value.  Alternately, the user can cause the
**        function to return a failure result, OS_ERROR, by setting the value
**        of UT_OS_Fail to OS_SEMCREATE_FAIL prior to this function being
**        called.  OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag, OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemCreate(uint32 *sem_id, const char *sem_name,
                      uint32 sem_initial_value, uint32 options)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemCreate);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(UT_OBJTYPE_BINSEM);
    }
    else
    {
        *sem_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemGive stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemGive.  The user can adjust the response by setting the value
**        of UT_BinSemFail prior to this function being called.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns the user-defined value UT_BinSemFail.
**
******************************************************************************/
int32 OS_BinSemGive(uint32 sem_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGive);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemGetInfo.  It sets the binary semaphore structure variables
**        to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemGetInfo(uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGetInfo);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_BinSemGetInfo), bin_prop, sizeof(*bin_prop)) < sizeof(*bin_prop))
    {
        bin_prop->creator =  1;
        UT_FIXUP_ID(bin_prop->creator, UT_OBJTYPE_TASK);
        strncpy(bin_prop->name, "Name", OS_MAX_API_NAME - 1);
        bin_prop->name[OS_MAX_API_NAME - 1] = '\0';
    }

    return status;
}


/*****************************************************************************/
/**
** \brief OS_BinSemDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemDelete.  The user can adjust the response by setting
**        the values in the BinSemDelRtn structure prior to this function
**        being called.  If the value BinSemDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value BinSemDelRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemDelete(uint32 sem_id)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_BINSEM, sem_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_BinSemTimedWait stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_BinSemTimedWait.  The variable OSBinSemTimedWaitRtn.value is set
**        to the value passed to the function, reset_type, and the variable
**        OSBinSemTimedWaitRtn.count is incremented each time this function is
**        called.  The unit tests compare these values to expected results to
**        verify proper system response.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_BinSemTimedWait(uint32 sem_id, uint32 msecs)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemTimedWait);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_BinSemGetIdByName()
 *
 *****************************************************************************/
int32 OS_BinSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BinSemGetIdByName);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_BinSemGetIdByName), sem_id, sizeof(*sem_id)) < sizeof(*sem_id))
    {
        *sem_id =  1;
        UT_FIXUP_ID(*sem_id, UT_OBJTYPE_BINSEM);
    }

    return status;
}

