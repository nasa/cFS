/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSFILEAPI_H_
#define _OSAL_UT_OSFILEAPI_H_

#include <common_types.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/


/*
 * Prototype for table init function (needs to be called from UT)
 */
int32 OS_VxWorks_StreamAPI_Impl_Init(void);
int32 OS_VxWorks_DirAPI_Impl_Init(void);

/*
 * Allow UT to get the value of the OS_IMPL_SELF_EUID and
 * OS_IMPL_SELF_EGID constants.  These might be assigned
 * at runtime by the init function (above) or they might be
 * defined at compile time.
 */
unsigned int Osapi_Internal_GetSelfEUID(void);
unsigned int Osapi_Internal_GetSelfEGID(void);

void Osapi_Internal_Set_Selectable(uint32 local_id, bool is_selectable);

#endif /* _OSAL_UT_OSTIMER_H_ */

