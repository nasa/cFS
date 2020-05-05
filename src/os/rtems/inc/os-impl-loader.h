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
 * \file     os-impl-loader.h
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_LOADER_H_
#define INCLUDE_OS_IMPL_LOADER_H_

#include <osconfig.h>
#include <dlfcn.h>

#include <rtems/rtl/rtl.h>
#include <rtems/rtl/rtl-unresolved.h>


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/*
 * A local lookup table for RTEMS-specific information.
 * This is not directly visible to the outside world.
 */
typedef struct
{
    /* cppcheck-suppress unusedStructMember */
    void *dl_handle;
} OS_impl_module_internal_record_t;

extern OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];



#endif  /* INCLUDE_OS_IMPL_LOADER_H_ */

