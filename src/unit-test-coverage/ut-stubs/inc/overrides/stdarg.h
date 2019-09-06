/* OSAL coverage stub replacement for stdarg.h */
#ifndef _OSAL_STUB_STDARG_H_
#define _OSAL_STUB_STDARG_H_

/* ----------------------------------------- */
/* constants normally defined in stdarg.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in stdarg.h */
/* ----------------------------------------- */

typedef void*         OCS_va_list;

/* ----------------------------------------- */
/* prototypes normally declared in stdarg.h */
/* ----------------------------------------- */

#define OCS_va_start(ap, last)  ap = &last
#define OCS_va_end(ap)



#endif /* _OSAL_STUB_STDARG_H_ */

