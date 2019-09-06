/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSAPI_H_
#define _OSAL_UT_OSAPI_H_

#include <common_types.h>
#include <overrides/taskLib.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/*
 * Prototype for table init functions (need to be called from UT)
 */
int32 OS_VxWorks_TaskAPI_Impl_Init(void);
int32 OS_VxWorks_QueueAPI_Impl_Init(void);
int32 OS_VxWorks_BinSemAPI_Impl_Init(void);
int32 OS_VxWorks_CountSemAPI_Impl_Init(void);
int32 OS_VxWorks_MutexAPI_Impl_Init(void);

/**
 * Purges all state tables and resets back to initial conditions
 * Helps avoid cross-test dependencies
 */
void Osapi_Internal_ResetState(void);

/**
 * Sets the "vxid" field on an internal task table entry.
 */
void Osapi_Internal_SetImplTaskId(uint32 local_id, OCS_TASK_ID TaskId);

/**
 * Invokes OS_SymTableIterator_Impl() with the given arguments.
 * This is normally a static function but exposed via a non-static wrapper for UT purposes.
 */
int Osapi_Internal_CallEntryPoint(int arg);

#endif /* _OSAL_UT_OSAPI_H_ */

