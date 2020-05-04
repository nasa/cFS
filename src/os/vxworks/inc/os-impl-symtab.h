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
 * \file     os-impl-symtab.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_SYMTAB_H_
#define INCLUDE_OS_IMPL_SYMTAB_H_

#include <osconfig.h>
#include <symLib.h>

typedef struct
{
    uint32  Sizelimit;
    uint32  CurrSize;
    int32   StatusCode;
    int     fd;
} SymbolDumpState_t;


/* A global for storing the state in a SymbolDump call */
extern SymbolDumpState_t OS_VxWorks_SymbolDumpState;

BOOL  OS_SymTableIterator_Impl ( char *name, SYM_VALUE val,  SYM_TYPE type,  _Vx_usr_arg_t arg, SYM_GROUP group );

#endif  /* INCLUDE_OS_IMPL_SYMTAB_H_ */

