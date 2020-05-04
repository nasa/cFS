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
 * \file     ut-adaptor-files.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_FILES_H_
#define INCLUDE_UT_ADAPTOR_FILES_H_

#include <common_types.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/


/*
 * Prototype for table init function (needs to be called from UT)
 */
extern int32 UT_Call_OS_VxWorks_StreamAPI_Impl_Init(void);

/*
 * Allow UT to get the value of the OS_IMPL_SELF_EUID and
 * OS_IMPL_SELF_EGID constants.  These might be assigned
 * at runtime by the init function (above) or they might be
 * defined at compile time.
 */
unsigned int UT_FileTest_GetSelfEUID(void);
unsigned int UT_FileTest_GetSelfEGID(void);

void UT_FileTest_Set_Selectable(uint32 local_id, bool is_selectable);

#endif  /* INCLUDE_UT_ADAPTOR_FILES_H_ */

