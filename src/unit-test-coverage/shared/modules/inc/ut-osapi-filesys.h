/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSAPI_FILESYS_H_
#define _OSAL_UT_OSAPI_FILESYS_H_

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

bool Osapi_Internal_FileSys_FindVirtMountPoint(void *ref, uint32 local_id, const OS_common_record_t *obj);
int32 Osapi_Internal_FileSys_InitLocalFromVolTable(OS_filesys_internal_record_t *local, const OS_VolumeInfo_t *Vol);


#endif /* _OSAL_UT_OSAPI_FILESYS_H_ */

