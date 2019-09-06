/* OSAL coverage stub replacement for shellLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/shellLib.h>

OCS_STATUS OCS_shellGenericInit(
        const char * config,
        int stackSize,
        const char * shellName,
        char **pShellName,
        OCS_BOOL interactive,
        OCS_BOOL loginAccess,
        int fdin,
        int fdout,
        int fderr)
{
    return (UT_DEFAULT_IMPL(OCS_shellGenericInit));
}

