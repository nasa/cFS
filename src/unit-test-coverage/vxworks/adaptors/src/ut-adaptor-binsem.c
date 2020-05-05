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
 * \file     ut-adaptor-binsem.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-binsem.h"

#include <os-vxworks.h>
#include <os-impl-binsem.h>


void*  const UT_Ref_OS_impl_bin_sem_table = OS_impl_bin_sem_table;
size_t const UT_Ref_OS_impl_bin_sem_table_SIZE = sizeof(OS_impl_bin_sem_table);

const UT_EntryKey_t UT_StubKey_GenericSemTake = UT_KEY(OS_VxWorks_GenericSemTake);
const UT_EntryKey_t UT_StubKey_GenericSemGive = UT_KEY(OS_VxWorks_GenericSemGive);

int32 UT_Call_OS_VxWorks_BinSemAPI_Impl_Init(void)
{
    return OS_VxWorks_BinSemAPI_Impl_Init();
}


