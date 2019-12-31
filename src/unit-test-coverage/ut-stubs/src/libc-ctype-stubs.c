/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for string.h */
#include <ctype.h>
#include "utstubs.h"

#include <OCS_ctype.h>

int OCS_isgraph(int c)
{
    return UT_DEFAULT_IMPL_RC(OCS_isgraph, isgraph(c));
}

