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
 * \file     osapi-mutex-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-mutex.h"


/*
** Mutex API
*/

UT_DEFAULT_STUB(OS_MutSemCreate_Impl,(uint32 sem_id, uint32 options))
UT_DEFAULT_STUB(OS_MutSemGive_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_MutSemTake_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_MutSemDelete_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_MutSemGetInfo_Impl,(uint32 sem_id, OS_mut_sem_prop_t *mut_prop))

