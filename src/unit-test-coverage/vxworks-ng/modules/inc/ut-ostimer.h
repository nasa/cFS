/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSTIMER_H_
#define _OSAL_UT_OSTIMER_H_

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
int32 OS_VxWorks_TimeBaseAPI_Impl_Init(void);

void Osapi_Internal_ResetState(void);
void Osapi_Internal_Setup(uint32 local_id, int signo);

/**
 * Invokes OS_VxWorks_SigWait() with the given arguments.
 * This is normally a static function but exposed via a non-static wrapper for UT purposes.
 */
int32 Osapi_Internal_CallSigWaitFunc(uint32 local_id);

/* Invokes the static OS_VxWorks_TimeBaseTask() function with given argument */
int Osapi_Internal_CallHelperTaskFunc(int arg);

/* A hook function which sets the timer registration state */
void Osapi_Internal_SetTimeBaseRegState(uint32 local_id, bool is_success);

#endif /* _OSAL_UT_OSTIMER_H_ */

