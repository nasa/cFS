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


UT_DEFAULT_STUB(OS_QueueAPI_Init,(void))

/*****************************************************************************/
/**
** \brief OS_QueueCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueCreate.  The user can adjust the response by setting
**        the values in the QueueCreateRtn structure prior to this function
**        being called.  If the value QueueCreateRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueCreateRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        creating the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Similar to the real call, doesn't care about creator or do any
**           mutex locking\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_INVALID_POINTER,
**        OS_ERR_NAME_TOO_LONG, OS_ERR_NO_FREE_IDS, OS_ERR_NAME_TAKEN,
**        or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueCreate(uint32 *queue_id,
                     const char *queue_name,
                     uint32 queue_depth,
                     uint32 data_size,
                     uint32 flags)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueueCreate);

    if (status == OS_SUCCESS)
    {
        *queue_id = UT_AllocStubObjId(UT_OBJTYPE_QUEUE);
    }
    else
    {
        *queue_id = 0xDEADBEEFU;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueDelete.  The user can adjust the response by setting
**        the values in the QueueDelRtn structure prior to this function
**        being called.  If the value QueueDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueDelRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        deleting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Similar to real code without mutex locking\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueDelete(uint32 queue_id)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueueDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_QUEUE, queue_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueGet stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueGet.  The user can adjust the response by setting
**        the values in the QueueGetRtn structure prior to this function
**        being called.  If the value QueueGetRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueueGetRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        getting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Works similar to real function; note that pend on empty queue
**           doesn't block\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_INVALID_POINTER, OS_QUEUE_EMPTY, OS_QUEUE_INVALID_SIZE,
**        or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueGet(uint32 queue_id,
                  void *data,
                  uint32 size,
                  uint32 *size_copied,
                  int32 timeout)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueueGet);

    if (status == OS_SUCCESS)
    {
        *size_copied = UT_Stub_CopyToLocal((UT_EntryKey_t)&OS_QueueGet + queue_id, data, size);
        if (*size_copied == 0)
        {
            status = OS_QUEUE_EMPTY;
        }
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueuePut stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueuePut.  The user can adjust the response by setting
**        the values in the QueuePutRtn structure prior to this function
**        being called.  If the value QueuePutRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value QueuePutRtn.value.
**        Otherwise the return value is dependent on success or failure in
**        putting the queue.
**
** \par Assumptions, External Events, and Notes:
**        1. Same as real function\n
**        2. Emulates socket queue, without use of sockets
**
** \returns
**        Returns either a user-defined status flag, OS_ERR_INVALID_ID,
**        OS_INVALID_POINTER, OS_QUEUE_FULL, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueuePut(uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
    int32   status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_QueuePut);

    if (status == OS_SUCCESS)
    {
        UT_SetDataBuffer((UT_EntryKey_t)&OS_QueueGet + queue_id, (void *)data, size, true);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_QueueGetIdByName()
 *
 *****************************************************************************/
int32 OS_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueGetIdByName);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_QueueGetIdByName), queue_id, sizeof(*queue_id)) < sizeof(*queue_id))
    {
        *queue_id =  1;
        UT_FIXUP_ID(*queue_id, UT_OBJTYPE_QUEUE);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_QueueGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_QueueGetInfo.  It sets the queue structure variables to fixed
**        values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_QueueGetInfo(uint32 sem_id, OS_queue_prop_t *queue_prop)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_QueueGetInfo);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_QueueGetInfo), queue_prop, sizeof(*queue_prop)) < sizeof(*queue_prop))
    {
        queue_prop->creator = 1;
        UT_FIXUP_ID(queue_prop->creator, UT_OBJTYPE_TASK);
        strncpy(queue_prop->name, "Name", OS_MAX_API_NAME - 1);
        queue_prop->name[OS_MAX_API_NAME - 1] = '\0';
    }


    return status;
}


