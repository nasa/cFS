/* OSAL coverage stub replacement for drv/hdisk/ataDrv.h */
#ifndef _OSAL_STUB_DRV_HDISK_ATADRV_H_
#define _OSAL_STUB_DRV_HDISK_ATADRV_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>
#include <OCS_blkIo.h>

/* ----------------------------------------- */
/* constants normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */
extern OCS_BLK_DEV *OCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset);



#endif /* _OSAL_STUB_DRV_HDISK_ATADRV_H_ */

