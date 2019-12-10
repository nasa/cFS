/* OSAL coverage stub replacement for stat.h */
#ifndef _OSAL_STUB_STAT_H_
#define _OSAL_STUB_STAT_H_

#include <OCS_basetypes.h>
#include <OCS_sys_types.h>

/* ----------------------------------------- */
/* constants normally defined in sys/stat.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* constants normally defined in sys/statvfs.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/stat.h */
/* ----------------------------------------- */

struct OCS_stat
{
    OCS_mode_t   st_mode;
    OCS_off_t    st_size;
    OCS_time_t   st_mtime;
    OCS_uid_t    st_uid;
    OCS_gid_t    st_gid;
};

/* ----------------------------------------- */
/* types normally defined in sys/statvfs.h */
/* ----------------------------------------- */
struct OCS_statvfs
{
    OCS_off_t f_bsize;
    OCS_off_t f_bfree;
    OCS_off_t f_blocks;
};

/* ----------------------------------------- */
/* prototypes normally declared in sys/stat.h */
/* ----------------------------------------- */

extern int OCS_fchmod (int fd, OCS_mode_t mode);
extern int OCS_chmod (const char *path, OCS_mode_t mode);
extern int OCS_mkdir (const char * path, ...);
extern int OCS_stat (const char * file, struct OCS_stat * buf);


/* ----------------------------------------- */
/* prototypes normally declared in sys/statvfs.h */
/* ----------------------------------------- */

extern int OCS_statvfs (const char * file, struct OCS_statvfs * buf);



#endif /* _OSAL_STUB_STAT_H_ */

