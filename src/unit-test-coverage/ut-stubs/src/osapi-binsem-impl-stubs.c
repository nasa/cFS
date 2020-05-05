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
 * \file     osapi-binsem-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-binsem.h"


/*
** Semaphore API
*/

UT_DEFAULT_STUB(OS_BinSemCreate_Impl,(uint32 sem_id, uint32 sem_initial_value, uint32 options))
UT_DEFAULT_STUB(OS_BinSemFlush_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemGive_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemTake_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemTimedWait_Impl,(uint32 sem_id, uint32 msecs))
UT_DEFAULT_STUB(OS_BinSemDelete_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemGetInfo_Impl,(uint32 sem_id, OS_bin_sem_prop_t *bin_prop))

