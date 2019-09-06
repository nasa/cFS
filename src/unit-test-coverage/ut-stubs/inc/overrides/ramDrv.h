/* OSAL coverage stub replacement for ramDrv.h */
#ifndef _OSAL_STUB_RAMDRV_H_
#define _OSAL_STUB_RAMDRV_H_

/* ----------------------------------------- */
/* constants normally defined in ramDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in ramDrv.h */
/* ----------------------------------------- */
typedef int OCS_BLK_DEV;

/* ----------------------------------------- */
/* prototypes normally declared in ramDrv.h */
/* ----------------------------------------- */
extern OCS_BLK_DEV *OCS_ramDevCreate (char *ramAddr, int bytesPerSec, int secPerTrack,
                       int nSectors, int secOffset);




#endif /* _OSAL_STUB_RAMDRV_H_ */

