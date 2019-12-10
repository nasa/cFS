/* OSAL coverage stub replacement for dlfcn.h */
#ifndef _OSAL_OVERRIDE_DLFCN_H_
#define _OSAL_OVERRIDE_DLFCN_H_

#include <OCS_dlfcn.h>

/* ----------------------------------------- */
/* mappings for declarations in dlfcn.h */
/* ----------------------------------------- */

#define dlclose                    OCS_dlclose
#define dlerror                    OCS_dlerror
#define dlopen                     OCS_dlopen
#define dlsym                      OCS_dlsym


#endif /* _OSAL_OVERRIDE_DLFCN_H_ */
