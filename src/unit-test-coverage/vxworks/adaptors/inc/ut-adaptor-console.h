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
 * \file     ut-adaptor-console.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_CONSOLE_H_
#define INCLUDE_UT_ADAPTOR_CONSOLE_H_

#include <common_types.h>
#include "ut-adaptor-common.h"

extern void*  const UT_Ref_OS_impl_console_table;
extern size_t const UT_Ref_OS_impl_console_table_SIZE;


/**
 * Invokes the console helper task entry point
 */
extern void UT_ConsoleTest_TaskEntry(int arg);

/**
 * Force the "is_async" field to a given state for coverage testing
 */
extern void UT_ConsoleTest_SetConsoleAsync(uint32 local_id, bool is_async);


#endif  /* INCLUDE_UT_ADAPTOR_CONSOLE_H_ */

