/* OSAL coverage stub replacement for drv/hdisk/ataDrv.h */
#ifndef _OSAL_STUB_DRV_HDISK_ATADRV_H_
#define _OSAL_STUB_DRV_HDISK_ATADRV_H_

/* ----------------------------------------- */
/* constants normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */
typedef int OCS_BLK_DEV;

/* ----------------------------------------- */
/* prototypes normally declared in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */
extern OCS_BLK_DEV *OCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset);



#endif /* _OSAL_STUB_DRV_HDISK_ATADRV_H_ */

