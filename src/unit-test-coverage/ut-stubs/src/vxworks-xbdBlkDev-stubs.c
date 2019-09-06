/* OSAL coverage stub replacement for xbdBlkDev.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/xbdBlkDev.h>

OCS_device_t OCS_xbdBlkDevCreateSync  (OCS_BLK_DEV *bd, const char *name)
{
    int32 status = UT_DEFAULT_IMPL(OCS_xbdBlkDevCreateSync);
    OCS_device_t res;

    if (status != 0)
    {
        res = OCS_NULLDEV;  /* NULLDEV (0) means error */
    }
    else
    {
        res = (OCS_device_t)0x123456; /* something non-zero means success */
    }

    return res;
}

OCS_STATUS OCS_xbdBlkDevDelete  (OCS_device_t dev, OCS_BLK_DEV **ppbd)
{
    return (UT_DEFAULT_IMPL(OCS_xbdBlkDevDelete));
}

