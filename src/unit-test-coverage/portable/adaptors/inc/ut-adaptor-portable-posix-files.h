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
 * \file     ut-adaptor-portable-posix-files.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_PORTABLE_POSIX_FILES_H_
#define INCLUDE_UT_ADAPTOR_PORTABLE_POSIX_FILES_H_

#include <common_types.h>

unsigned int UT_PortablePosixFileTest_GetSelfEUID(void);
unsigned int UT_PortablePosixFileTest_GetSelfEGID(void);

#endif  /* INCLUDE_UT_ADAPTOR_PORTABLE_POSIX_FILES_H_ */

