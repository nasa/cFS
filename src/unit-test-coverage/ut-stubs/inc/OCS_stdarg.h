/* OSAL coverage stub replacement for stdarg.h */
#ifndef _OSAL_STUB_STDARG_H_
#define _OSAL_STUB_STDARG_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in stdarg.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in stdarg.h */
/* ----------------------------------------- */
typedef struct { void *p; }         OCS_va_list;



/* ----------------------------------------- */
/* prototypes normally declared in stdarg.h */
/* ----------------------------------------- */

#define OCS_va_start(ap, last)  ap.p = &last
#define OCS_va_end(ap)



#endif /* _OSAL_STUB_STDARG_H_ */

