/* OSAL coverage stub replacement for errno.h */
#ifndef _OSAL_STUB_ERRNO_H_
#define _OSAL_STUB_ERRNO_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in errno.h */
/* ----------------------------------------- */
/* POSIX-specific  errnos */
#define OCS_EINTR                        0x1801
#define OCS_EAGAIN                       0x1802
#define OCS_EINVAL                       0x1803
#define OCS_EMSGSIZE                     0x1804
#define OCS_ETIMEDOUT                    0x1805
#define OCS_ESPIPE                       0x1806


/* ----------------------------------------- */
/* types normally defined in errno.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in errno.h */
/* ----------------------------------------- */

extern int OCS_errno;

#endif /* _OSAL_STUB_ERRNO_H_ */

