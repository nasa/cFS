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
 * \file     ut-adaptor-portable-posix-io.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_PORTABLE_POSIX_IO_H_
#define INCLUDE_UT_ADAPTOR_PORTABLE_POSIX_IO_H_



/**
 * \file     ut-osfileapi.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef _UT_PPOSFILEAPI_H_
#define _UT_PPOSFILEAPI_H_

#include <common_types.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/
void UT_PortablePosixIOTest_Set_Selectable(uint32 local_id, bool is_selectable);

#endif  /* _UT_OSFILEAPI_H_ */


#endif  /* INCLUDE_UT_ADAPTOR_PORTABLE_POSIX_IO_H_ */

