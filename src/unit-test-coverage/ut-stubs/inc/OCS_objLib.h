/* OSAL coverage stub replacement for objLib.h */
#ifndef _OSAL_STUB_OBJLIB_H_
#define _OSAL_STUB_OBJLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in objLib.h */
/* ----------------------------------------- */
/* VxWorks-specific  errnos */
#define OCS_M_objLib                     0x1810
#define OCS_S_objLib_OBJ_ID_ERROR        (OCS_M_objLib | 1)
#define OCS_S_objLib_OBJ_UNAVAILABLE     (OCS_M_objLib | 2)
#define OCS_S_objLib_OBJ_DELETED         (OCS_M_objLib | 3)
#define OCS_S_objLib_OBJ_TIMEOUT         (OCS_M_objLib | 4)
#define OCS_S_objLib_OBJ_NO_METHOD       (OCS_M_objLib | 5)



/* ----------------------------------------- */
/* types normally defined in objLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in objLib.h */
/* ----------------------------------------- */



#endif /* _OSAL_STUB_OBJLIB_H_ */

