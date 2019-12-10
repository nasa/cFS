/* OSAL coverage stub replacement for ioLib.h */
#ifndef _OSAL_OVERRIDE_IOLIB_H_
#define _OSAL_OVERRIDE_IOLIB_H_

#include <OCS_ioLib.h>
#include <vxWorks.h>


/* ----------------------------------------- */
/* mappings for declarations in ioLib.h */
/* ----------------------------------------- */

#define FIOCHKDSK   OCS_FIOCHKDSK
#define FIOUNMOUNT  OCS_FIOUNMOUNT
#define ioctl       OCS_ioctl


#endif /* _OSAL_OVERRIDE_IOLIB_H_ */
