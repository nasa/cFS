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
 * \file     os-shared-errors.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_ERRORS_H_
#define INCLUDE_OS_SHARED_ERRORS_H_

#include <os-shared-globaldefs.h>


/* Mapping of integer error number to name */
typedef struct
{
   int32 Number;
   const char *Name;
} OS_ErrorTable_Entry_t;

extern const OS_ErrorTable_Entry_t  OS_IMPL_ERROR_NAME_TABLE[];

#endif  /* INCLUDE_OS_SHARED_ERRORS_H_ */

