/* OSAL coverage stub replacement for stdlib.h */
#ifndef _OSAL_OVERRIDE_STDLIB_H_
#define _OSAL_OVERRIDE_STDLIB_H_

#include <OCS_stdlib.h>

/* ----------------------------------------- */
/* mappings for declarations in stdlib.h */
/* ----------------------------------------- */

#define EXIT_SUCCESS   OCS_EXIT_SUCCESS
#define EXIT_FAILURE   OCS_EXIT_FAILURE
#define exit           OCS_exit
#define strtoul        OCS_strtoul
#define system         OCS_system
#define malloc         OCS_malloc
#define free           OCS_free


#endif /* _OSAL_OVERRIDE_STDLIB_H_ */
