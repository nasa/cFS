/* OSAL coverage stub replacement for shellLib.h */
#ifndef _OSAL_STUB_SHELLLIB_H_
#define _OSAL_STUB_SHELLLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in shellLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in shellLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in shellLib.h */
/* ----------------------------------------- */

extern OCS_STATUS OCS_shellGenericInit(
        const char * config,
        int stackSize,
        const char * shellName,
        char **pShellName,
        OCS_BOOL interactive,
        OCS_BOOL loginAccess,
        int fdin,
        int fdout,
        int fderr
);

#endif /* _OSAL_STUB_SHELLLIB_H_ */

