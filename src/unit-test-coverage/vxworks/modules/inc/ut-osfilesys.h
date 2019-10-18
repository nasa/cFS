/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSFILESYS_H_
#define _OSAL_UT_OSFILESYS_H_

#include <common_types.h>
#include <overrides/xbdBlkDev.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

void Osapi_Internal_SetupFileSysEntry(uint32 id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts);




#endif /* _OSAL_UT_OSFILESYS_H_ */

