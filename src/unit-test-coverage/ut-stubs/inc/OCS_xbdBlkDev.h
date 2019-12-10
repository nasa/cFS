/* OSAL coverage stub replacement for xbdBlkDev.h */
#ifndef _OSAL_STUB_XBDBLKDEV_H_
#define _OSAL_STUB_XBDBLKDEV_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>
#include <OCS_blkIo.h>

/* ----------------------------------------- */
/* constants normally defined in xbdBlkDev.h */
/* ----------------------------------------- */
#define OCS_NULLDEV     ((OCS_device_t)0)

/* ----------------------------------------- */
/* types normally defined in xbdBlkDev.h */
/* ----------------------------------------- */
typedef int OCS_device_t;

/* ----------------------------------------- */
/* prototypes normally declared in xbdBlkDev.h */
/* ----------------------------------------- */
extern OCS_device_t OCS_xbdBlkDevCreateSync  (OCS_BLK_DEV *bd, const char *name);
extern OCS_STATUS OCS_xbdBlkDevDelete  (OCS_device_t dev, OCS_BLK_DEV **ppbd);



#endif /* _OSAL_STUB_XBDBLKDEV_H_ */

