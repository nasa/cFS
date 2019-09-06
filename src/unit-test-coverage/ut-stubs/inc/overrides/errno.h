/* OSAL coverage stub replacement for errno.h */
#ifndef _OSAL_STUB_ERRNO_H_
#define _OSAL_STUB_ERRNO_H_

/* ----------------------------------------- */
/* constants normally defined in errno.h */
/* ----------------------------------------- */
#define OCS_EINTR                        0x1801
#define OCS_EAGAIN                       0x1802
#define OCS_EINVAL                       0x1803
#define OCS_EMSGSIZE                     0x1804
#define OCS_ETIMEDOUT                    0x1805
#define OCS_ESPIPE                       0x1806

/* VxWorks-specific  errnos */
#define OCS_M_objLib                     0x1810
#define OCS_S_objLib_OBJ_ID_ERROR        (OCS_M_objLib | 1)
#define OCS_S_objLib_OBJ_UNAVAILABLE     (OCS_M_objLib | 2)
#define OCS_S_objLib_OBJ_DELETED         (OCS_M_objLib | 3)
#define OCS_S_objLib_OBJ_TIMEOUT         (OCS_M_objLib | 4)
#define OCS_S_objLib_OBJ_NO_METHOD       (OCS_M_objLib | 5)



/* ----------------------------------------- */
/* types normally defined in errno.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in errno.h */
/* ----------------------------------------- */

extern int OCS_errno;

#endif /* _OSAL_STUB_ERRNO_H_ */

