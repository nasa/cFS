/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-shared-idmap.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_IDMAP_H_
#define INCLUDE_OS_SHARED_IDMAP_H_

#include <os-shared-globaldefs.h>


#define OS_OBJECT_EXCL_REQ_FLAG     0x0001

/*
 * This supplies a non-abstract definition of "OS_common_record_t"
 */
struct OS_common_record
{
   const char *name_entry;
   uint32     active_id;
   uint32     creator;
   uint16     refcount;
   uint16     flags;
};


/*
 * Type of locking that should occur when checking IDs.
 */
typedef enum
{
   OS_LOCK_MODE_NONE,           /**< Do not lock global table at all (use with caution) */
   OS_LOCK_MODE_GLOBAL,         /**< Lock during operation, and if successful, leave global table locked */
   OS_LOCK_MODE_EXCLUSIVE,      /**< Like OS_LOCK_MODE_GLOBAL but must be exclusive (refcount == zero)  */
   OS_LOCK_MODE_REFCOUNT,       /**< If operation succeeds, increment refcount and unlock global table */
} OS_lock_mode_t;


/*
 * A function to perform arbitrary record matching.
 *
 * This can be used to find a record based on criteria other than the ID,
 * such as the name or any other record within the structure.
 *
 * Returns true if the id/obj matches the reference, false otherwise.
 */
typedef bool (*OS_ObjectMatchFunc_t)(void *ref, uint32 local_id, const OS_common_record_t *obj);

/*
 * Global instantiations
 */
/* The following are quick-access pointers to the various sections of the common table */
extern OS_common_record_t * const OS_global_task_table;
extern OS_common_record_t * const OS_global_queue_table;
extern OS_common_record_t * const OS_global_bin_sem_table;
extern OS_common_record_t * const OS_global_count_sem_table;
extern OS_common_record_t * const OS_global_mutex_table;
extern OS_common_record_t * const OS_global_stream_table;
extern OS_common_record_t * const OS_global_dir_table;
extern OS_common_record_t * const OS_global_timebase_table;
extern OS_common_record_t * const OS_global_timecb_table;
extern OS_common_record_t * const OS_global_module_table;
extern OS_common_record_t * const OS_global_filesys_table;
extern OS_common_record_t * const OS_global_console_table;




/****************************************************************************************
                                ID MAPPING FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_ObjectIdInit

   Purpose: Initialize the OS-independent layer for object ID management

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_ObjectIdInit                (void);


/*
 * Table locking and unlocking for global objects can be done at the shared code
 * layer but the actual implementation is OS-specific
 */


/*----------------------------------------------------------------
   Function: OS_Lock_Global_Impl

    Purpose: Locks the global table identified by "idtype"

   Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_Lock_Global_Impl(uint32 idtype);

/*----------------------------------------------------------------
   Function: OS_Unlock_Global_Impl

    Purpose: Unlocks the global table identified by "idtype"

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_Unlock_Global_Impl(uint32 idtype);



/*
   Function prototypes for routines implemented in common layers but private to OSAL

   These implement the basic OSAL ObjectID patterns - that is a 32-bit number that
   is opaque externally, but internally identifies a specific type of object and
   corresponding index within the local tables.
 */

/*----------------------------------------------------------------
   Function: OS_GetMaxForObjectType

    Purpose: Obtains the maximum number of objects for "idtype" in the global table

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
uint32 OS_GetMaxForObjectType(uint32 idtype);

/*----------------------------------------------------------------
   Function: OS_GetBaseForObjectType

    Purpose: Obtains the base object number for "idtype" in the global table

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
uint32 OS_GetBaseForObjectType(uint32 idtype);

/*----------------------------------------------------------------
   Function: OS_ObjectIdMap

    Purpose: Convert an object serial number into a 32-bit OSAL ID of the given type

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdMap(uint32 idtype, uint32 idvalue, uint32 *result);

/*----------------------------------------------------------------
   Function: OS_ObjectIdUnMap

    Purpose: Convert a 32-bit OSAL ID of the expected type into an object serial number

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdUnMap(uint32 id, uint32 idtype, uint32 *idvalue);

/*----------------------------------------------------------------
   Function: OS_ObjectIdFindByName

    Purpose: Finds an entry in the global resource table matching the given name

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdFindByName (uint32 idtype, const char *name, uint32 *object_id);

/*----------------------------------------------------------------
   Function: OS_ObjectIdToArrayIndex

    Purpose: Convert a 32-bit OSAL ID into a zero-based array index

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdToArrayIndex(uint32 idtype, uint32 id, uint32 *ArrayIndex);

/*----------------------------------------------------------------
   Function: OS_ObjectIdGetBySearch

    Purpose: Find and lock an entry in the global resource table
             Search is performed using a user-specified match function
             (Allows searching for items by arbitrary keys)

   Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, uint32 idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_common_record_t **record);

/*----------------------------------------------------------------
   Function: OS_ObjectIdGetByName

    Purpose: Find and lock an entry in the global resource table
             Search is performed using a name match function

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdGetByName (OS_lock_mode_t lock_mode, uint32 idtype, const char *name, OS_common_record_t **record);

/*----------------------------------------------------------------
   Function: OS_ObjectIdGetById

    Purpose: Find and lock an entry in the global resource table
             Lookup is performed by ID value (no searching required)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdGetById(OS_lock_mode_t lock_mode, uint32 idtype, uint32 id, uint32 *array_index, OS_common_record_t **record);

/*----------------------------------------------------------------
   Function: OS_ObjectIdAllocateNew

    Purpose: Issue a new object ID of the given type and associate with the given name
             The array index (0-based) and global record pointers are output back to the caller
             The table will be left in a "locked" state to allow further initialization
             The OS_ObjectIdFinalizeNew() function must be called to complete the operation

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdAllocateNew(uint32 idtype, const char *name, uint32 *array_index, OS_common_record_t **record);

/*----------------------------------------------------------------
   Function: OS_ObjectIdFinalizeNew

    Purpose: Completes the operation initiated by OS_ObjectIdAllocateNew()
             If the operation was successful, the final OSAL ID is returned
             If the operation was unsuccessful, the ID is deleted and returned to the pool.
             The global table is unlocked for future operations

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_common_record_t *record, uint32 *outid);

/*----------------------------------------------------------------
   Function: OS_ObjectIdRefcountDecr

    Purpose: Decrement the reference count
             This releases objects obtained with OS_LOCK_MODE_REFCOUNT mode

   Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdRefcountDecr(OS_common_record_t *record);

/*
 * Internal helper functions
 * These are not normally called outside this unit, but need
 * to be exposed for unit testing.
 */
bool OS_ObjectNameMatch(void *ref, uint32 local_id, const OS_common_record_t *obj);
void OS_ObjectIdInitiateLock(OS_lock_mode_t lock_mode, uint32 idtype);
int32 OS_ObjectIdConvertLock(OS_lock_mode_t lock_mode, uint32 idtype, uint32 reference_id, OS_common_record_t *obj);
int32 OS_ObjectIdSearch(uint32 idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_common_record_t **record);
int32 OS_ObjectIdFindNext(uint32 idtype, uint32 *array_index, OS_common_record_t **record);

#endif  /* INCLUDE_OS_SHARED_IDMAP_H_ */

