/* OSAL coverage stub replacement for ramDrv.h */
#ifndef _OSAL_OVERRIDE_RAMDRV_H_
#define _OSAL_OVERRIDE_RAMDRV_H_

#include <OCS_ramDrv.h>
#include <vxWorks.h>
#include <blkIo.h>

/* ----------------------------------------- */
/* mappings for declarations in ramDrv.h */
/* ----------------------------------------- */

#define ramDevCreate     OCS_ramDevCreate


#endif /* _OSAL_OVERRIDE_RAMDRV_H_ */
