/* OSAL coverage stub replacement for moduleLib.h */
#ifndef _OSAL_STUB_MODULELIB_H_
#define _OSAL_STUB_MODULELIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in moduleLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in moduleLib.h */
/* ----------------------------------------- */
typedef struct OCS_MODULE { int m; }    OCS_MODULE;
typedef OCS_MODULE*                     OCS_MODULE_ID;

typedef struct OCS_MODULE_INFO
{
    struct
    {
        unsigned long textAddr;
        unsigned long textSize;
        unsigned long dataAddr;
        unsigned long dataSize;
        unsigned long bssAddr;
        unsigned long bssSize;
    } segInfo;

} OCS_MODULE_INFO;

/* ----------------------------------------- */
/* prototypes normally declared in moduleLib.h */
/* ----------------------------------------- */

extern OCS_STATUS OCS_moduleInfoGet(OCS_MODULE_ID moduleId, OCS_MODULE_INFO * pModuleInfo);



#endif /* _OSAL_STUB_MODULELIB_H_ */

