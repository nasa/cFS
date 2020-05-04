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
 * \file     ut-adaptor-idmap.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_IDMAP_H_
#define INCLUDE_UT_ADAPTOR_IDMAP_H_

#include <common_types.h>
#include <OCS_semLib.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not part of the implementation API.
 *
 *****************************************************/
int32 UT_Call_OS_VxWorks_TableMutex_Init(uint32 idtype);
void UT_IdMapTest_SetImplTableMutex(uint32 idtype, OCS_SEM_ID vxid);


#endif  /* INCLUDE_UT_ADAPTOR_IDMAP_H_ */

