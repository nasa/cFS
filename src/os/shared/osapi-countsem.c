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
 * \file   osapi-countsem.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
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
#if !defined(OS_MAX_COUNT_SEMAPHORES) || (OS_MAX_COUNT_SEMAPHORES <= 0)
#error "osconfig.h must define OS_MAX_COUNT_SEMAPHORES to a valid value"
#endif


/*
 * Global data for the API
 */
enum
{
   LOCAL_NUM_OBJECTS = OS_MAX_COUNT_SEMAPHORES,
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_COUNTSEM
};

OS_apiname_internal_record_t    OS_count_sem_table          [LOCAL_NUM_OBJECTS];


/****************************************************************************************
                                  SEMAPHORE API
 ***************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemAPI_Init(void)
{
   memset(OS_count_sem_table, 0, sizeof(OS_count_sem_table));
   return OS_SUCCESS;
} /* end OS_CountSemAPI_Init */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemCreate
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemCreate (uint32 *sem_id, const char *sem_name, uint32 sem_initial_value,
                        uint32 options)
{
   OS_common_record_t *record;
   int32             return_code;
   uint32            local_id;

   /* Check for NULL pointers */
   if (sem_id == NULL || sem_name == NULL)
   {
      return OS_INVALID_POINTER;
   }

   if ( strlen (sem_name) >= OS_MAX_API_NAME )
   {
      return OS_ERR_NAME_TOO_LONG;
   }

   /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
   return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, sem_name, &local_id, &record);
   if(return_code == OS_SUCCESS)
   {
      /* Save all the data to our own internal table */
      strcpy(OS_count_sem_table[local_id].obj_name, sem_name);
      record->name_entry = OS_count_sem_table[local_id].obj_name;

      /* Now call the OS-specific implementation.  This reads info from the table. */
      return_code = OS_CountSemCreate_Impl(local_id, sem_initial_value, options);

      /* Check result, finalize record, and unlock global table. */
      return_code = OS_ObjectIdFinalizeNew(return_code, record, sem_id);
   }

   return return_code;

} /* end OS_CountSemCreate */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemDelete
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemDelete (uint32 sem_id)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, sem_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_CountSemDelete_Impl(local_id);

      /* Free the entry in the master table now while still locked */
      if (return_code == OS_SUCCESS)
      {
         /* Only need to clear the ID as zero is the "unused" flag */
         record->active_id = 0;
      }

      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   return return_code;

} /* end OS_CountSemDelete */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGive
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGive ( uint32 sem_id )
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
       return_code = OS_CountSemGive_Impl (local_id);
    }

    return return_code;

} /* end OS_CountSemGive */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTake
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTake ( uint32 sem_id )
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_CountSemTake_Impl (local_id);
   }

   return return_code;
} /* end OS_CountSemTake */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTimedWait
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTimedWait ( uint32 sem_id, uint32 msecs )
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, sem_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_CountSemTimedWait_Impl (local_id, msecs);
   }

   return return_code;
} /* end OS_CountSemTimedWait */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGetIdByName (uint32 *sem_id, const char *sem_name)
{
   int32 return_code;

   if (sem_id == NULL || sem_name == NULL)
   {
       return OS_INVALID_POINTER;
   }

   return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, sem_name, sem_id);

   return return_code;
} /* end OS_CountSemGetIdByName */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGetInfo (uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check parameters */
   if (count_prop == NULL)
   {
      return OS_INVALID_POINTER;
   }

   memset(count_prop,0,sizeof(OS_count_sem_prop_t));

   /* Check Parameters */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, sem_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      strncpy(count_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
      count_prop->creator = record->creator;

      return_code = OS_CountSemGetInfo_Impl (local_id, count_prop);
      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   return return_code;
} /* end OS_CountSemGetInfo */

