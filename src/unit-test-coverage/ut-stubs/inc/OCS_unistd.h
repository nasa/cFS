/* OSAL coverage stub replacement for unistd.h */
#ifndef _OSAL_STUB_UNISTD_H_
#define _OSAL_STUB_UNISTD_H_

#include <OCS_basetypes.h>
#include <OCS_sys_types.h>

/* ----------------------------------------- */
/* constants normally defined in unistd.h */
/* ----------------------------------------- */

#define OCS_SEEK_SET        0x1C01
#define OCS_SEEK_CUR        0x1C02
#define OCS_SEEK_END        0x1C03
#define OCS_STDIN_FILENO    0x1C04
#define OCS_STDOUT_FILENO   0x1C05
#define OCS_STDERR_FILENO   0x1C06


/* ----------------------------------------- */
/* types normally defined in unistd.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in unistd.h */
/* ----------------------------------------- */

extern int OCS_close (int fd);
extern OCS_gid_t OCS_getegid (void);
extern OCS_uid_t OCS_geteuid (void);
extern long int OCS_gethostid (void);
extern int OCS_gethostname (char * name, size_t len);
extern OCS_pid_t OCS_getpid (void);
extern OCS_off_t OCS_lseek (int fd, OCS_off_t offset, int whence);
extern OCS_ssize_t OCS_read (int fd, void * buf, size_t nbytes);
extern int OCS_rmdir (const char * path);
extern long int OCS_sysconf (int name);
extern OCS_ssize_t OCS_write (int fd, const void * buf, size_t n);


#endif /* _OSAL_STUB_UNISTD_H_ */

