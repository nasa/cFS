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
 * \file   osapi-queue.c
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
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"


/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_QUEUES) || (OS_MAX_QUEUES <= 0)
#error "osconfig.h must define OS_MAX_QUEUES to a valid value"
#endif



enum
{
   LOCAL_NUM_OBJECTS = OS_MAX_QUEUES,
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_QUEUE
};

OS_queue_internal_record_t    OS_queue_table          [LOCAL_NUM_OBJECTS];


/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_QueueAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueAPI_Init(void)
{
   memset(OS_queue_table, 0, sizeof(OS_queue_table));
   return OS_SUCCESS;
} /* end OS_QueueAPI_Init */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_QueueCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueCreate (uint32 *queue_id, const char *queue_name, uint32 queue_depth, uint32 data_size, uint32 flags)
{
   OS_common_record_t *record;
   int32             return_code;
   uint32            local_id;

   if ( queue_name == NULL || queue_id == NULL )
   {
       return OS_INVALID_POINTER;
   }

   if ( strlen (queue_name) >= OS_MAX_API_NAME )
   {
      return OS_ERR_NAME_TOO_LONG;
   }

   /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
   return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, queue_name, &local_id, &record);
   if(return_code == OS_SUCCESS)
   {
      /* Save all the data to our own internal queue table */
      strcpy(OS_queue_table[local_id].queue_name, queue_name);
      record->name_entry = OS_queue_table[local_id].queue_name;
      OS_queue_table[local_id].max_depth = queue_depth;
      OS_queue_table[local_id].max_size = data_size;


      /* Now call the OS-specific implementation.  This reads info from the queue table. */
      return_code = OS_QueueCreate_Impl(local_id, flags);

      /* Check result, finalize record, and unlock global table. */
      return_code = OS_ObjectIdFinalizeNew(return_code, record, queue_id);
   }

   return return_code;

} /* end OS_QueueCreate */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_QueueDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueDelete (uint32 queue_id)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, queue_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_QueueDelete_Impl(local_id);

      /* Free the entry in the master table now while still locked */
      if (return_code == OS_SUCCESS)
      {
         /* Only need to clear the ID as zero is the "unused" flag */
         record->active_id = 0;
      }

      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   return return_code;

} /* end OS_QueueDelete */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_QueueGet
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGet (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   if (data == NULL || size_copied == NULL)
   {
      return_code = OS_INVALID_POINTER;
   }
   else
   {
      return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, queue_id, &local_id, &record);
      if (return_code == OS_SUCCESS)
      {
         if (size < OS_queue_table[local_id].max_size)
         {
             /*
             ** The buffer that the user is passing in is potentially too small
             */
             *size_copied = 0;
             return_code = OS_QUEUE_INVALID_SIZE;
         }
         else
         {
            return_code = OS_QueueGet_Impl (local_id, data, size, size_copied, timeout);
         }
      }
   }

   return return_code;
} /* end OS_QueueGet */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_QueuePut
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueuePut (uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   if (data == NULL)
   {
      return_code = OS_INVALID_POINTER;
   }
   else
   {
      return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, queue_id, &local_id, &record);
      if (return_code == OS_SUCCESS)
      {
         return_code = OS_QueuePut_Impl (local_id, data, size, flags);
      }
   }

   return return_code;
} /* end OS_QueuePut */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_QueueGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
{
   int32 return_code;

   if (queue_id == NULL || queue_name == NULL)
   {
       return OS_INVALID_POINTER;
   }

   return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, queue_name, queue_id);

   return return_code;

} /* end OS_QueueGetIdByName */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_QueueGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGetInfo (uint32 queue_id, OS_queue_prop_t *queue_prop)
{
   OS_common_record_t *record;
   int32             return_code;
   uint32            local_id;

   /* Check parameters */
   if (queue_prop == NULL)
   {
      return OS_INVALID_POINTER;
   }

   memset(queue_prop,0,sizeof(OS_queue_prop_t));

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL,LOCAL_OBJID_TYPE, queue_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      strncpy(queue_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
      queue_prop->creator =    record->creator;

      /*
       * Currently there are no additional queue details provided by the impl layer -
       * But this could be added in the future (i.e. current/max depth, msg size, etc)
       */

      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   return return_code;

} /* end OS_QueueGetInfo */

