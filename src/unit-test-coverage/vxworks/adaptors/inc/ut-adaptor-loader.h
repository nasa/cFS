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
 * \file     ut-adaptor-loader.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_LOADER_H_
#define INCLUDE_UT_ADAPTOR_LOADER_H_

#include <common_types.h>
#include <utstubs.h>
#include <OCS_taskLib.h>
#include <OCS_semLib.h>


extern void*  const UT_Ref_OS_impl_module_table;
extern size_t const UT_Ref_OS_impl_module_table_SIZE;

extern int32 UT_Call_OS_VxWorks_ModuleAPI_Impl_Init(void);

#endif  /* INCLUDE_UT_ADAPTOR_LOADER_H_ */

