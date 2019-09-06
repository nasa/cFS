/* OSAL coverage stub replacement for ramDrv.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/ramDrv.h>

static int LOCAL_RAMDEV;

OCS_BLK_DEV *OCS_ramDevCreate (char *ramAddr, int bytesPerSec, int secPerTrack,
                       int nSectors, int secOffset)
{
    int32 Status;
    OCS_BLK_DEV *retval = NULL;

    Status = UT_DEFAULT_IMPL(OCS_ramDevCreate);
    if (Status == 0)
    {
        retval = &LOCAL_RAMDEV;
    }

    return retval;
}



