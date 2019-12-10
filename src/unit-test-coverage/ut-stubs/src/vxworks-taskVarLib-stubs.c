/* OSAL coverage stub replacement for taskVarLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_taskVarLib.h>


OCS_STATUS OCS_taskVarAdd(int tid, int *pVar)
{
    return (UT_DEFAULT_IMPL(OCS_taskVarAdd));
}



