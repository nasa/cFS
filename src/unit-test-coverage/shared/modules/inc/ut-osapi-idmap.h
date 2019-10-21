/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSAPI_IDMAP_H_
#define _OSAL_UT_OSAPI_IDMAP_H_

#include <common_types.h>
#include <os-impl.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/**
 * Purges all state tables and resets back to initial conditions
 * Helps avoid cross-test dependencies
 */
void Osapi_Internal_ResetState(void);

/**
 * Wrapper around the OS_ObjectIdFindNext call so the test code can invoke it
 * (it is defined as static)
 */
int32 Osapi_Call_ObjectIdFindNext(uint32 idtype, uint32 *array_index, OS_common_record_t **record);

/**
 * Wrapper around the OS_ObjectIdConvertLock call so the test code can invoke it
 * (it is defined as static)
 */
int32 Osapi_Call_ObjectIdConvertLock(OS_lock_mode_t lock_mode, uint32 idtype, uint32 reference_id, OS_common_record_t *obj);


#endif /* _OSAL_UT_OSAPI_IDMAP_H_ */

