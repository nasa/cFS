/* OSAL coverage stub replacement for string.h */
#include <ctype.h>
#include "utstubs.h"

#include <overrides/ctype.h>

int OCS_isgraph(int c)
{
    return UT_DEFAULT_IMPL_RC(OCS_isgraph, isgraph(c));
}

