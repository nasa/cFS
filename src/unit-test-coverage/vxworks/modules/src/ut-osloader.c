/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osloader.c"
#include "ut-osloader.h"

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };


/*
 * A UT-specific wrapper function to invoke the Symbol Table Iterator.
 * This is normally static so it needs this wrapper to call it.
 */
int32 Osapi_Internal_CallIteratorFunc(const char *name, void* val, uint32 TestSize, uint32 SizeLimit)
{
    OS_impl_module_global.sym_dump.Sizelimit = SizeLimit;
    OS_impl_module_global.sym_dump.CurrSize = TestSize;
    /*
     * note the internal function is takes a name declared as "char*" to be
     * consistent with the VxWorks API, however the implementation does not
     * modify this argument.
     */
    return OS_SymTableIterator_Impl((char*)name,val,0,0,0);
}

