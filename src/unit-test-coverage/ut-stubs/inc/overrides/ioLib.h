/* OSAL coverage stub replacement for ioLib.h */
#ifndef _OSAL_STUB_IOLIB_H_
#define _OSAL_STUB_IOLIB_H_

/* ----------------------------------------- */
/* constants normally defined in ioLib.h */
/* ----------------------------------------- */

#define OCS_FIOCHKDSK                    0x1E01
#define OCS_FIOUNMOUNT                   0x1E02


/* ----------------------------------------- */
/* types normally defined in ioLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in ioLib.h */
/* ----------------------------------------- */

extern int OCS_ioctl (int fd, unsigned long request, ...);



#endif /* _OSAL_STUB_IOLIB_H_ */

