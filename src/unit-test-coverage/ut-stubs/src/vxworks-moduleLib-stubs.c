/* OSAL coverage stub replacement for moduleLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/moduleLib.h>

OCS_STATUS OCS_moduleInfoGet(OCS_MODULE_ID moduleId, OCS_MODULE_INFO * pModuleInfo)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_moduleInfoGet);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OCS_moduleInfoGet), pModuleInfo, sizeof(*pModuleInfo)) < sizeof(*pModuleInfo))
    {
        memset(pModuleInfo, 0, sizeof(*pModuleInfo));
    }

    return Status;
}
