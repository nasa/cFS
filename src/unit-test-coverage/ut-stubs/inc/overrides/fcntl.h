/* OSAL coverage stub replacement for fcntl.h */
#ifndef _OSAL_STUB_FCNTL_H_
#define _OSAL_STUB_FCNTL_H_

/* ----------------------------------------- */
/* constants normally defined in fcntl.h */
/* ----------------------------------------- */

#include "utbits/filemodes.h"

#define OCS_O_RDONLY                     0x1501
#define OCS_O_WRONLY                     0x1502
#define OCS_O_RDWR                       0x1503
#define OCS_O_CREAT                      0x1504
#define OCS_O_TRUNC                      0x1508
#define OCS_O_NONBLOCK                   0x1510


#define OCS_F_GETFL                      0x1D01
#define OCS_F_SETFL                      0x1D02

/* ----------------------------------------- */
/* types normally defined in fcntl.h */
/* ----------------------------------------- */
typedef unsigned int    OCS_mode_t;

/* ----------------------------------------- */
/* prototypes normally declared in fcntl.h */
/* ----------------------------------------- */

extern int OCS_fcntl (int fd, int cmd, ...);
extern int OCS_open (const char * file, int oflag, ...);



#endif /* _OSAL_STUB_FCNTL_H_ */

