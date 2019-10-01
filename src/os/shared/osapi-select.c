/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osapi-select.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *         This code only uses very basic C library calls that are expected
 *         to be available on every sane C-language compiler.  For everything else,
 *         a platform-specific implementation function is used.
 *
 *         This select API is in a separate compilation unit to aid in unit testing.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"


/*
 *********************************************************************************
 *          SELECT API
 *********************************************************************************
 */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SelectSingle
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs)
{
   int32 return_code;
   uint32 local_id;
   OS_common_record_t *record;

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_STREAM, objid, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_SelectSingle_Impl(local_id, StateFlags, msecs);
      OS_ObjectIdRefcountDecr(record);
   }

   return return_code;
} /* end OS_SelectSingle */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SelectMultiple
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    int32 return_code;

    /*
     * FIXME:
     * This does not currently increment any refcounts.
     * That means a file/socket can be closed while actively inside a
     * OS_SelectMultiple() call in another thread.
     */
    return_code = OS_SelectMultiple_Impl(ReadSet, WriteSet, msecs);

    return return_code;
} /* end OS_SelectMultiple */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SelectFdZero
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectFdZero(OS_FdSet *Set)
{
   memset(Set,0,sizeof(OS_FdSet));
   return OS_SUCCESS;
} /* end OS_SelectFdZero */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SelectFdAdd
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectFdAdd(OS_FdSet *Set, uint32 objid)
{
   int32 return_code;
   uint32 local_id;

   return_code = OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_STREAM, objid, &local_id);
   if (return_code == OS_SUCCESS)
   {
      Set->object_ids[local_id >> 3] |= 1 << (local_id & 0x7);
   }

   return return_code;
} /* end OS_SelectFdAdd */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SelectFdClear
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectFdClear(OS_FdSet *Set, uint32 objid)
{
   int32 return_code;
   uint32 local_id;

   return_code = OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_STREAM, objid, &local_id);
   if (return_code == OS_SUCCESS)
   {
      Set->object_ids[local_id >> 3] &= ~(1 << (local_id & 0x7));
   }

   return return_code;
} /* end OS_SelectFdClear */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_SelectFdIsSet
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
bool OS_SelectFdIsSet(OS_FdSet *Set, uint32 objid)
{
   int32 return_code;
   uint32 local_id;

   return_code = OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_STREAM, objid, &local_id);
   if (return_code != OS_SUCCESS)
   {
      return false;
   }

   return ((Set->object_ids[local_id >> 3] >> (local_id & 0x7)) & 0x1);
} /* end OS_SelectFdIsSet */


