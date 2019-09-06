/* OSAL coverage stub replacement for loadLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/loadLib.h>
#include <overrides/unldLib.h>

static int LOCAL_MODULE = 0;

OCS_MODULE_ID OCS_loadModule(int fd, unsigned int symFlag)
{
    int32 Status;
    OCS_MODULE_ID retval;

    Status = UT_DEFAULT_IMPL(OCS_loadModule);
    if (Status == 0)
    {
        retval = &LOCAL_MODULE;
    }
    else
    {
        retval = NULL;
    }

    return retval;
}

OCS_STATUS OCS_unldByModuleId(OCS_MODULE_ID moduleId, int options)
{
    return (UT_DEFAULT_IMPL(OCS_unldByModuleId));
}

