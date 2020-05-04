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
 * \file     OCS_symLib.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 * OSAL coverage stub replacement for symLib.h
 */

#ifndef INCLUDE_OCS_SYMLIB_H_
#define INCLUDE_OCS_SYMLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in symLib.h */
/* ----------------------------------------- */
#define OCS_LOAD_ALL_SYMBOLS        0x1A00
#define OCS_SYM_FIND_BY_NAME        0x1A01

/* ----------------------------------------- */
/* types normally defined in symLib.h */
/* ----------------------------------------- */
typedef int OCS_SYM_TYPE;
typedef uintptr_t OCS_SYM_VALUE;
typedef int OCS_SYM_GROUP;

typedef struct OCS_SYMBOL  OCS_SYMBOL;
typedef struct OCS_SYMTAB  OCS_SYMTAB;


typedef OCS_SYMTAB * OCS_SYMTAB_ID;
typedef OCS_SYMBOL * OCS_SYMBOL_ID;

typedef struct OCS_SYMBOL_DESC
{
    unsigned int    mask;
    char *          name;
    OCS_SYM_VALUE   value;
} OCS_SYMBOL_DESC;

typedef OCS_BOOL (*OCS_symEach_Routine_t) (char *, OCS_SYM_VALUE, OCS_SYM_TYPE, OCS_Vx_usr_arg_t, OCS_SYM_GROUP);

/* ----------------------------------------- */
/* prototypes normally declared in symLib.h */
/* ----------------------------------------- */

extern OCS_SYMTAB_ID OCS_sysSymTbl;


extern OCS_STATUS OCS_symFindByName(OCS_SYMTAB_ID symTblId, char * name, char ** pValue, OCS_SYM_TYPE * pType);
extern OCS_SYMBOL * OCS_symEach(OCS_SYMTAB_ID symTblId, OCS_symEach_Routine_t routine, int routineArg);
extern OCS_STATUS   OCS_symFind     (OCS_SYMTAB_ID symTblId, OCS_SYMBOL_DESC * pSymbol);


#endif  /* INCLUDE_OCS_SYMLIB_H_ */

