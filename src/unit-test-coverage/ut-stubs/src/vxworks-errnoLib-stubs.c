/* OSAL coverage stub replacement for errnoLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/errnoLib.h>


int OCS_errnoGet (void)
{
    return UT_DEFAULT_IMPL(OCS_errnoGet);
}
