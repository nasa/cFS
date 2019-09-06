/* OSAL coverage stub replacement for sys/stat.h */
#ifndef _OSAL_STUB_SYS_STAT_H_
#define _OSAL_STUB_SYS_STAT_H_

/* ----------------------------------------- */
/* constants normally defined in sys/stat.h */
/* ----------------------------------------- */

/* note these constants also exist in fcntl.h */
#include "utbits/filemodes.h"


/* ----------------------------------------- */
/* types normally defined in sys/stat.h */
/* ----------------------------------------- */
typedef unsigned int    OCS_mode_t;

struct OCS_stat
{
    unsigned int st_mode;
    unsigned int st_size;
    unsigned int st_mtime;
    unsigned int st_uid;
    unsigned int st_gid;
};

/* ----------------------------------------- */
/* prototypes normally declared in sys/stat.h */
/* ----------------------------------------- */

extern int OCS_fchmod (int fd, OCS_mode_t mode);
extern int OCS_chmod (const char *path, OCS_mode_t mode);
extern int OCS_mkdir (const char * path, OCS_mode_t mode);
extern int OCS_stat (const char * file, struct OCS_stat * buf);



#endif /* _OSAL_STUB_SYS_STAT_H_ */

