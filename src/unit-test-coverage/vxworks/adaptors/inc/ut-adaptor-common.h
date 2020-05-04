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
 * \file     ut-adaptor-common.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_COMMON_H_
#define INCLUDE_UT_ADAPTOR_COMMON_H_

#include <common_types.h>
#include <utstubs.h>
#include <OCS_semLib.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not part of the implementation API.
 *
 *****************************************************/

void UT_CommonTest_SetImplTableMutex(uint32 idtype, OCS_SEM_ID vxid);

extern int32 OS_VxWorks_GenericSemTake(OCS_SEM_ID vxid, int sys_ticks);
extern int32 OS_VxWorks_GenericSemGive(OCS_SEM_ID vxid);

/*
 * This also needs to expose the keys for the stubs to
 * helper functions that the test case needs to configure.
 *
 * This is because the test case cannot directly include
 * the internal header file which provides this API.
 */
extern const UT_EntryKey_t UT_StubKey_OS_VxWorks_TableMutex_Init;

#endif  /* INCLUDE_UT_ADAPTOR_COMMON_H_ */

