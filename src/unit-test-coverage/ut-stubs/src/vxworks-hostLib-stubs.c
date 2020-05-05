/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     vxworks-hostLib-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* OSAL coverage stub replacement for hostLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_hostLib.h>

int   OCS_hostGetByName (char *name)
{
    return (UT_DEFAULT_IMPL(OCS_hostGetByName));
}
