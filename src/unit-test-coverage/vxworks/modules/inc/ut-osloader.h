/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSLOADER_H_
#define _OSAL_UT_OSLOADER_H_

#include <common_types.h>
#include <overrides/symLib.h>

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
int32 OS_VxWorks_ModuleAPI_Impl_Init(void);

/*
 * Invokes OS_SymTableIterator_Impl() with the given arguments.
 * This is normally a static function but exposed via a non-static wrapper for UT purposes.
 */
int32 Osapi_Internal_CallIteratorFunc(char *name, void* val, uint32 TestSize, uint32 SizeLimit);

#endif /* _OSAL_UT_OSLOADER_H_ */

