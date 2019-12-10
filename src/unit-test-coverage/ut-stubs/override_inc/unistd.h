/* OSAL coverage stub replacement for unistd.h */
#ifndef _OSAL_OVERRIDE_UNISTD_H_
#define _OSAL_OVERRIDE_UNISTD_H_

#include <OCS_unistd.h>

/* ----------------------------------------- */
/* mappings for declarations in unistd.h */
/* ----------------------------------------- */

#define SEEK_SET        OCS_SEEK_SET
#define SEEK_CUR        OCS_SEEK_CUR
#define SEEK_END        OCS_SEEK_END
#define STDIN_FILENO    OCS_STDIN_FILENO
#define STDOUT_FILENO   OCS_STDOUT_FILENO
#define STDERR_FILENO   OCS_STDERR_FILENO

#define close             OCS_close
#define getegid           OCS_getegid
#define geteuid           OCS_geteuid
#define gethostid         OCS_gethostid
#define gethostname       OCS_gethostname
#define getpid            OCS_getpid
#define lseek             OCS_lseek
#define read              OCS_read
#define rmdir             OCS_rmdir
#define sysconf           OCS_sysconf
#define write             OCS_write


#endif /* _OSAL_OVERRIDE_UNISTD_H_ */

