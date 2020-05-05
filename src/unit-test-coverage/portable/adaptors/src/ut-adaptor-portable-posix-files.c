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
 * \file     ut-adaptor-portable-posix-files.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */


/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-portable-posix-files.h"

#include <os-impl-files.h>

unsigned int UT_PortablePosixFileTest_GetSelfEUID(void)
{
    return OS_IMPL_SELF_EUID;
}

unsigned int UT_PortablePosixFileTest_GetSelfEGID(void)
{
    return OS_IMPL_SELF_EGID;
}

