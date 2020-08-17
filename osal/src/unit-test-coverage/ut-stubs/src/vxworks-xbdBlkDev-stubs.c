/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* OSAL coverage stub replacement for xbdBlkDev.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_xbdBlkDev.h>

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

