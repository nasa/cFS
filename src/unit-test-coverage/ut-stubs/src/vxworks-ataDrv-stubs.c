/* OSAL coverage stub replacement for ataDrv.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/drv/hdisk/ataDrv.h>

static int LOCAL_ATADEV;

OCS_BLK_DEV *OCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset)
{
    int32 Status;
    OCS_BLK_DEV *retval = NULL;

    Status = UT_DEFAULT_IMPL(OCS_ataDevCreate);
    if (Status == 0)
    {
        retval = &LOCAL_ATADEV;
    }

    return retval;
}



