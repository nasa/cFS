/* OSAL coverage stub replacement for objLib.h */
#ifndef _OSAL_OVERRIDE_OBJLIB_H_
#define _OSAL_OVERRIDE_OBJLIB_H_

#include <OCS_objLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in objLib.h */
/* ----------------------------------------- */

#define M_objLib                  OCS_M_objLib
#define S_objLib_OBJ_ID_ERROR     OCS_S_objLib_OBJ_ID_ERROR
#define S_objLib_OBJ_UNAVAILABLE  OCS_S_objLib_OBJ_UNAVAILABLE
#define S_objLib_OBJ_DELETED      OCS_S_objLib_OBJ_DELETED
#define S_objLib_OBJ_TIMEOUT      OCS_S_objLib_OBJ_TIMEOUT
#define S_objLib_OBJ_NO_METHOD    OCS_S_objLib_OBJ_NO_METHOD


#endif /* _OSAL_OVERRIDE_OBJLIB_H_ */
