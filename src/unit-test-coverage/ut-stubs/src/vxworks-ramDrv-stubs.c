/* OSAL coverage stub replacement for ramDrv.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_ramDrv.h>

static OCS_BLK_DEV LOCAL_RAMDEV = { 0 };

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



