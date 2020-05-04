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
 * \file     ut-adaptor-binsem.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_BINSEM_H_
#define INCLUDE_UT_ADAPTOR_BINSEM_H_

#include <common_types.h>
#include <utstubs.h>
#include <OCS_taskLib.h>
#include <OCS_semLib.h>


extern void*  const UT_Ref_OS_impl_bin_sem_table;
extern size_t const UT_Ref_OS_impl_bin_sem_table_SIZE;

/*
 * This also needs to expose the keys for the stubs to
 * helper functions that the test case needs to configure.
 *
 * This is because the test case cannot directly include
 * the internal header file which provides this API.
 */
extern const UT_EntryKey_t UT_StubKey_GenericSemTake;
extern const UT_EntryKey_t UT_StubKey_GenericSemGive;

extern int32 UT_Call_OS_VxWorks_BinSemAPI_Impl_Init(void);

#endif  /* INCLUDE_UT_ADAPTOR_BINSEM_H_ */

