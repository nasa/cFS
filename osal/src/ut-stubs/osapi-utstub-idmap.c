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


/**
 * \file     osapi-utstub-idmap.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 *
 * NOTE: The Object ID manipulation calls would not be called by applications.
 * However stubs are still defined in order to support things such as
 * coverage testing of the low-level implementation.  This set of stubs
 * is implemented separately here as it is only needed when coverage testing
 * OSAL itself (not for coverage testing other units).
 */

#include "utstub-helpers.h"
#include "os-shared-idmap.h"

UT_DEFAULT_STUB(OS_ObjectIdInit,(void))

/* Lock/Unlock for global tables */
void OS_Lock_Global(uint32 idtype)
{
    UT_DEFAULT_IMPL(OS_Lock_Global);
}
void OS_Unlock_Global(uint32 idtype)
{
    UT_DEFAULT_IMPL(OS_Unlock_Global);
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdMap()
 *
 *****************************************************************************/
int32 OS_ObjectIdMap(uint32 idtype, uint32 idvalue, uint32 *result)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdMap);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdMap), result, sizeof(*result)) < sizeof(*result))
    {
        /* this needs to output something valid or code will break */
        *result = idvalue;
        UT_FIXUP_ID(*result, idtype);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdUnMap()
 *
 *****************************************************************************/
int32 OS_ObjectIdUnMap(uint32 id, uint32 idtype, uint32 *idvalue)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdUnMap);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdUnMap), idvalue, sizeof(*idvalue)) < sizeof(*idvalue))
    {
        /* this needs to output something valid or code will break */
        *idvalue = id & 0xFFFF;
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_GetMaxForObjectType()
 *
 *****************************************************************************/
uint32 OS_GetMaxForObjectType(uint32 idtype)
{
    int32 max;

    if (idtype > UT_OBJTYPE_NONE && idtype < UT_OBJTYPE_MAX)
    {
        max = OSAL_MAX_VALID_PER_TYPE;
    }
    else
    {
        max = 0;
    }

    return UT_DEFAULT_IMPL_RC(OS_GetMaxForObjectType, max);
}

/*****************************************************************************
 *
 * Stub function for OS_GetBaseForObjectType()
 *
 *****************************************************************************/
uint32 OS_GetBaseForObjectType(uint32 idtype)
{
    int32 base;

    if (idtype > UT_OBJTYPE_NONE && idtype < UT_OBJTYPE_MAX)
    {
        base = OSAL_MAX_VALID_PER_TYPE * (idtype - 1);
    }
    else
    {
        base = 0;
    }

    return UT_DEFAULT_IMPL_RC(OS_GetBaseForObjectType, base);
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdToArrayIndex()
 *
 *****************************************************************************/
int32 OS_ObjectIdToArrayIndex(uint32 idtype, uint32 id, uint32 *ArrayIndex)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdToArrayIndex);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdToArrayIndex), ArrayIndex, sizeof(*ArrayIndex)) < sizeof(*ArrayIndex))
    {
        /* this needs to output something valid or code will break */
        *ArrayIndex = id & 0xFFFF;
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdFinalize()
 *
 *****************************************************************************/
int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_common_record_t *record, uint32 *outid)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdFinalizeNew, operation_status);

    /* need to actually write something to the output buffer */
    if (Status == OS_SUCCESS && record != NULL && outid != NULL &&
            UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdFinalizeNew), outid, sizeof(*outid)) < sizeof(*outid))
    {
        *outid = record->active_id;
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdFindMatch()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, uint32 idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_common_record_t **record)
{
    int32 Status;
    static OS_common_record_t fake_record;

    /* by default this stub should return NAME_NOT_FOUND
     * unless the test case has set up otherwise.  To set
     * up a success response, just register a buffer for
     * the function
     */
    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetBySearch);

    if (Status == OS_SUCCESS && record != NULL &&
            UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetBySearch), record, sizeof(*record)) < sizeof(*record))
    {
        memset(&fake_record, 0, sizeof(fake_record));
        fake_record.active_id = 1;
        UT_FIXUP_ID(fake_record.active_id, idtype);
        *record = &fake_record;
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdFindByName()
 *
 *****************************************************************************/
int32 OS_ObjectIdFindByName (uint32 idtype, const char *name, uint32 *object_id)
{
    int32 Status;

    /* by default this stub should return NAME_NOT_FOUND
     * unless the test case has set up otherwise.  To set
     * up a success response, just register a buffer for
     * the function
     */
    Status = UT_DEFAULT_IMPL_RC(OS_ObjectIdFindByName,OS_ERR_NAME_NOT_FOUND);

    if (Status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdFindByName), object_id, sizeof(*object_id)) < sizeof(*object_id))
    {
        *object_id = 1;
        UT_FIXUP_ID(*object_id, idtype);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdGetByName()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetByName (OS_lock_mode_t lock_mode, uint32 idtype, const char *name, OS_common_record_t **record)
{
    int32 Status;
    OS_common_record_t *local_record;
    static OS_common_record_t fake_record;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetByName);

    if (Status == 0)
    {
        if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetByName), &local_record, sizeof(local_record)) < sizeof(local_record))
        {
            memset(&fake_record, 0, sizeof(fake_record));
            fake_record.active_id = 1;
            local_record = &fake_record;
            UT_FIXUP_ID(fake_record.active_id, idtype);
        }

        /* this needs to output something valid or code will break */
        if (record != NULL)
        {
            *record = local_record;
        }
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdGetById()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetById(OS_lock_mode_t check_mode, uint32 idtype, uint32 id, uint32 *array_index, OS_common_record_t **record)
{
    int32 Status;
    uint32 local_id;
    OS_common_record_t *local_record;
    static OS_common_record_t fake_record;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetById);

    if (Status == 0)
    {
        if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetById), &local_id, sizeof(local_id)) < sizeof(local_id))
        {
            local_id = id & 0xFFFF;
        }

        if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetById), &local_record, sizeof(local_record)) < sizeof(local_record))
        {
            memset(&fake_record, 0, sizeof(fake_record));
            fake_record.active_id = id;
            local_record = &fake_record;
        }

        /* this needs to output something valid or code will break */
        if (array_index != NULL)
        {
            *array_index = local_id;
        }
        if (record != NULL)
        {
            *record = local_record;
        }
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdRefcountDecr()
 *
 *****************************************************************************/
int32 OS_ObjectIdRefcountDecr(OS_common_record_t *record)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdRefcountDecr);

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdGetNext()
 *
 *****************************************************************************/
int32 OS_ObjectIdGetNext(uint32 idtype, uint32 *curr_index, OS_common_record_t **record)
{
    int32 Status;
    uint32 local_id;
    OS_common_record_t *local_record;
    static OS_common_record_t fake_record;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdGetNext);

    if (Status == 0)
    {
        if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetNext), &local_id, sizeof(local_id)) < sizeof(local_id))
        {
            local_id = UT_GetStubCount(UT_KEY(OS_ObjectIdGetNext));
        }

        if (local_id >= OSAL_MAX_VALID_PER_TYPE)
        {
            Status = OS_ERROR;
        }
        else
        {
            if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdGetNext), &local_record, sizeof(local_record)) < sizeof(local_record))
            {
                memset(&fake_record, 0, sizeof(fake_record));
                fake_record.active_id = local_id;
                UT_FIXUP_ID(fake_record.active_id, idtype);
                local_record = &fake_record;
            }

            if (curr_index != NULL)
            {
                *curr_index = local_id;
            }

            if (record != NULL)
            {
                *record = local_record;
            }
        }
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_ObjectIdAllocateNew()
 *
 *****************************************************************************/
int32 OS_ObjectIdAllocateNew(uint32 idtype, const char *name, uint32 *array_index, OS_common_record_t **record)
{
    int32 Status;
    uint32 local_id;
    OS_common_record_t *local_record;
    static OS_common_record_t fake_record;

    Status = UT_DEFAULT_IMPL(OS_ObjectIdAllocateNew);

    if (Status == 0)
    {
        if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdAllocateNew), &local_id, sizeof(local_id)) < sizeof(local_id))
        {
            local_id = UT_GetStubCount(UT_KEY(OS_ObjectIdAllocateNew)) & 0xFFFF;
        }

        if (UT_Stub_CopyToLocal(UT_KEY(OS_ObjectIdAllocateNew), &local_record, sizeof(local_record)) < sizeof(local_record))
        {
            memset(&fake_record, 0, sizeof(fake_record));
            fake_record.active_id = local_id;
            UT_FIXUP_ID(fake_record.active_id, idtype);
            local_record = &fake_record;
        }

        *record = local_record;
        *array_index = local_id;
    }

    return Status;
}

/*--------------------------------------------------------------------------------------
     Name: OS_GetResourceName

    Purpose: Stub function for OS_GetResourceName, returns either the test-supplied string
             or an empty string.

    returns: status
---------------------------------------------------------------------------------------*/
int32 OS_GetResourceName(uint32 object_id, char *buffer, uint32 buffer_size)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_GetResourceName), object_id);
    UT_Stub_RegisterContext(UT_KEY(OS_GetResourceName), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_GetResourceName), buffer_size);

    int32 return_code;

    return_code = UT_DEFAULT_IMPL(OS_GetResourceName);

    if (return_code == OS_SUCCESS)
    {
        if (buffer_size > 0 &&
                UT_Stub_CopyToLocal(UT_KEY(OS_GetResourceName), buffer, buffer_size) == 0)
        {
            /* return an empty string by default */
            buffer[0] = 0;
        }
    }

    return return_code;
}

/*--------------------------------------------------------------------------------------
     Name: OS_ConvertToArrayIndex

    Purpose: Converts any abstract ID into a number suitable for use as an array index.
             This is necessary for code that breaks when IDs are converted
             to nonzero ranges.  Note that this does NOT verify the validity of the ID,
             that is left to the caller.  This is only the conversion logic.

    returns: status
---------------------------------------------------------------------------------------*/
int32 OS_ConvertToArrayIndex(uint32 object_id, uint32 *ArrayIndex)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ConvertToArrayIndex), object_id);
    UT_Stub_RegisterContext(UT_KEY(OS_ConvertToArrayIndex), ArrayIndex);

   int32 return_code;

   return_code = UT_DEFAULT_IMPL(OS_ConvertToArrayIndex);

   if (return_code == OS_SUCCESS)
   {
       uint32 ObjType = (object_id >> 16) ^ 0x4000U;

       *ArrayIndex = object_id & 0xFFFFU;
       if (ObjType != UT_OBJTYPE_NONE && ObjType < UT_OBJTYPE_MAX)
       {
           *ArrayIndex %= UT_MAXOBJS[ObjType];
       }
   }
   else
   {
      /*
       * If set to fail, then set the output to something bizarre - if the code
       * actually tries to use this, chances are it will segfault and be fixed
       */
      *ArrayIndex = 0xDEADBEEFU;
   }

   return return_code;
} /* end OS_ConvertToArrayIndex */

/*--------------------------------------------------------------------------------------
     Name: OS_ForEachObjectOfType

    Purpose: Stub function for OS_ForEachObjectOfType

    returns: None
---------------------------------------------------------------------------------------*/
void OS_ForEachObjectOfType     (uint32 objtype, uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObjectOfType), objtype);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObjectOfType), creator_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObjectOfType), callback_ptr);
    UT_Stub_RegisterContext(UT_KEY(OS_ForEachObjectOfType), callback_arg);

    uint32 NextId;
    uint32 IdSize;

    /* Although this is "void", Invoke the default impl to log it and invoke any hooks */
    UT_DEFAULT_IMPL(OS_ForEachObjectOfType);

    while (1)
    {
        IdSize = UT_Stub_CopyToLocal(UT_KEY(OS_ForEachObjectOfType), &NextId, sizeof(NextId));
        if (IdSize < sizeof(NextId))
        {
            break;
        }
        (*callback_ptr)(NextId, callback_arg);
    }
}

/*--------------------------------------------------------------------------------------
     Name: OS_ForEachOject

    Purpose: Loops through all defined OSAL objects and calls callback_ptr on each one
             If creator_id is nonzero then only objects with matching creator id are processed.

    returns: None
---------------------------------------------------------------------------------------*/
void OS_ForEachObject (uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObject), creator_id);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ForEachObject), callback_ptr);
    UT_Stub_RegisterContext(UT_KEY(OS_ForEachObject), callback_arg);

    uint32 NextId;
    uint32 IdSize;

    /* Although this is "void", Invoke the default impl to log it and invoke any hooks */
    UT_DEFAULT_IMPL(OS_ForEachObject);

    while (1)
    {
        IdSize = UT_Stub_CopyToLocal((UT_EntryKey_t)&OS_ForEachObject, &NextId, sizeof(NextId));
        if (IdSize < sizeof(NextId))
        {
            break;
        }
        (*callback_ptr)(NextId, callback_arg);
    }
}

/*---------------------------------------------------------------------------------------
   Name: OS_IdentifyObject

   Purpose: Given an arbitrary object ID, get the type of the object

   returns: The type of object that the ID represents
---------------------------------------------------------------------------------------*/
uint32 OS_IdentifyObject       (uint32 object_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_IdentifyObject), object_id);

    int32 DefaultType;

    switch ((object_id >> 16) ^ 0x4000U)
    {
    case UT_OBJTYPE_TASK:
        DefaultType = OS_OBJECT_TYPE_OS_TASK;
        break;
    case UT_OBJTYPE_QUEUE:
        DefaultType = OS_OBJECT_TYPE_OS_QUEUE;
        break;
    case UT_OBJTYPE_COUNTSEM:
        DefaultType = OS_OBJECT_TYPE_OS_COUNTSEM;
        break;
    case UT_OBJTYPE_BINSEM:
        DefaultType = OS_OBJECT_TYPE_OS_BINSEM;
        break;
    case UT_OBJTYPE_MUTEX:
        DefaultType = OS_OBJECT_TYPE_OS_MUTEX;
        break;
    case UT_OBJTYPE_TIMECB:
        DefaultType = OS_OBJECT_TYPE_OS_TIMECB;
        break;
    case UT_OBJTYPE_MODULE:
        DefaultType = OS_OBJECT_TYPE_OS_MODULE;
        break;
    case UT_OBJTYPE_FILESTREAM:
        DefaultType = OS_OBJECT_TYPE_OS_STREAM;
        break;
    case UT_OBJTYPE_TIMEBASE:
        DefaultType = OS_OBJECT_TYPE_OS_TIMEBASE;
        break;
    case UT_OBJTYPE_DIR:
        DefaultType = OS_OBJECT_TYPE_OS_DIR;
        break;
    case UT_OBJTYPE_FILESYS:
        DefaultType = OS_OBJECT_TYPE_OS_FILESYS;
        break;
    default:
        DefaultType = OS_OBJECT_TYPE_UNDEFINED;
        break;
    }

    return UT_DEFAULT_IMPL_RC(OS_IdentifyObject, DefaultType);
}
