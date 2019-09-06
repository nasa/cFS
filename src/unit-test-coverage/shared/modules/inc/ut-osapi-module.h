/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSAPI_MODULE_H_
#define _OSAL_UT_OSAPI_MODULE_H_

#include <common_types.h>
#include <osapi-os-loader.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/**
 * Purges all state tables and resets back to initial conditions
 * Helps avoid cross-test dependencies
 */
void Osapi_Internal_ResetState(void);

/* A dummy function for the static symbol lookup test.  Not called */
void Test_DummyFunc(void);

int32 Osapi_Call_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName);
int32 Osapi_Call_ModuleLoad_Static(const char *ModuleName);


#endif /* _OSAL_UT_OSAPI_MODULE_H_ */

