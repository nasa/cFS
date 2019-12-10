/* OSAL coverage stub replacement for xbdBlkDev.h */
#ifndef _OSAL_OVERRIDE_XBDBLKDEV_H_
#define _OSAL_OVERRIDE_XBDBLKDEV_H_

#include <OCS_xbdBlkDev.h>
#include <vxWorks.h>
#include <blkIo.h>

/* ----------------------------------------- */
/* mappings for declarations in xbdBlkDev.h */
/* ----------------------------------------- */

#define NULLDEV                   OCS_NULLDEV
#define device_t                  OCS_device_t
#define xbdBlkDevCreateSync       OCS_xbdBlkDevCreateSync
#define xbdBlkDevDelete           OCS_xbdBlkDevDelete



#endif /* _OSAL_OVERRIDE_XBDBLKDEV_H_ */

