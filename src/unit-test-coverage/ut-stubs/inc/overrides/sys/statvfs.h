/* OSAL coverage stub replacement for sys/statvfs.h */
#ifndef _OSAL_STUB_SYS_STATVFS_H_
#define _OSAL_STUB_SYS_STATVFS_H_

/* ----------------------------------------- */
/* constants normally defined in sys/statvfs.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/statvfs.h */
/* ----------------------------------------- */
struct OCS_statvfs
{
    long f_bsize;
    long f_bfree;
    long f_blocks;
};

/* ----------------------------------------- */
/* prototypes normally declared in sys/statvfs.h */
/* ----------------------------------------- */

extern int OCS_statvfs (const char * file, struct OCS_statvfs * buf);


#endif /* _OSAL_STUB_SYS_STATVFS_H_ */

