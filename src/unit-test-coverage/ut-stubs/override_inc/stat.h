/* OSAL coverage stub replacement for stat.h */
#ifndef _OSAL_OVERRIDE_STAT_H_
#define _OSAL_OVERRIDE_STAT_H_

#include <OCS_stat.h>

/* ----------------------------------------- */
/* mappings for declarations in sys/stat.h */
/* ----------------------------------------- */

#define stat          OCS_stat
#define fchmod        OCS_fchmod
#define chmod         OCS_chmod
#define mkdir         OCS_mkdir


/* ----------------------------------------- */
/* mappings for declarations in sys/statvfs.h */
/* ----------------------------------------- */
#define statvfs     OCS_statvfs
#define statfs      OCS_statvfs



#endif /* _OSAL_OVERRIDE_STAT_H_ */
