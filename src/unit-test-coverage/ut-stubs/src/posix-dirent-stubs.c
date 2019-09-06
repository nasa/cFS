/* OSAL coverage stub replacement for dirent.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/dirent.h>

static int OCS_LOCAL_DIR;
struct OCS_dirent OCS_LOCAL_DIRENT;

int OCS_closedir (OCS_DIR * dirp)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_closedir);

    return Status;
}

OCS_DIR *OCS_opendir (const char * name)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_opendir);

    if (Status != 0)
    {
        return (OCS_DIR*)0;
    }

    return &OCS_LOCAL_DIR;
}

struct OCS_dirent *OCS_readdir (OCS_DIR * dirp)
{
    int32 Status;
    struct OCS_dirent *retval = (struct OCS_dirent *)0;

    Status = UT_DEFAULT_IMPL(OCS_readdir);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OCS_readdir), &retval, sizeof(retval)) < sizeof(retval) &&
            UT_GetStubCount(UT_KEY(OCS_readdir)) < 2)
    {
        memset(&OCS_LOCAL_DIRENT, 0, sizeof(OCS_LOCAL_DIRENT));
        retval = &OCS_LOCAL_DIRENT;
    }

    return retval;
}

void OCS_rewinddir (OCS_DIR * dirp)
{
    UT_DEFAULT_IMPL(OCS_rewinddir);
}


