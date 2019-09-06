/* OSAL coverage stub replacement for sysLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/sysLib.h>

int OCS_sysClkRateGet(void)
{
    return (UT_DEFAULT_IMPL_RC(OCS_sysClkRateGet,10000));
}



