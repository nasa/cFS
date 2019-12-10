/* OSAL coverage stub replacement for errnoLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_errnoLib.h>


int OCS_errnoGet (void)
{
    return UT_DEFAULT_IMPL(OCS_errnoGet);
}
