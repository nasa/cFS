/* OSAL coverage stub replacement for errno.h */
#ifndef _OSAL_OVERRIDE_ERRNO_H_
#define _OSAL_OVERRIDE_ERRNO_H_

#include <OCS_errno.h>

/* ----------------------------------------- */
/* mappings for declarations in errno.h */
/* ----------------------------------------- */
#define EINTR     OCS_EINTR
#define EAGAIN    OCS_EAGAIN
#define EINVAL    OCS_EINVAL
#define EMSGSIZE  OCS_EMSGSIZE
#define ETIMEDOUT OCS_ETIMEDOUT
#define ESPIPE    OCS_ESPIPE

#define errno       OCS_errno

#endif /* _OSAL_OVERRIDE_ERRNO_H_ */
