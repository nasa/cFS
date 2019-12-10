/* OSAL coverage stub replacement for moduleLib.h */
#ifndef _OSAL_OVERRIDE_MODULELIB_H_
#define _OSAL_OVERRIDE_MODULELIB_H_

#include <OCS_moduleLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in moduleLib.h */
/* ----------------------------------------- */

#define MODULE_ID      OCS_MODULE_ID
#define MODULE_INFO    OCS_MODULE_INFO

#define moduleInfoGet  OCS_moduleInfoGet



#endif /* _OSAL_OVERRIDE_MODULELIB_H_ */

