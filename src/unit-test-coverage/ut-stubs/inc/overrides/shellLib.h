/* OSAL coverage stub replacement for shellLib.h */
#ifndef _OSAL_STUB_SHELLLIB_H_
#define _OSAL_STUB_SHELLLIB_H_

/* ----------------------------------------- */
/* constants normally defined in shellLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in shellLib.h */
/* ----------------------------------------- */
typedef int OCS_STATUS;
typedef int OCS_BOOL;

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

