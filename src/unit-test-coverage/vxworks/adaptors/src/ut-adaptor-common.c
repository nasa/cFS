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
 * \file     ut-adaptor-common.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include <osconfig.h>
#include <stdlib.h>
#include <string.h>

#include <os-vxworks.h>
#include "ut-adaptor-common.h"

const UT_EntryKey_t UT_StubKey_OS_VxWorks_TableMutex_Init = UT_KEY(OS_VxWorks_TableMutex_Init);

