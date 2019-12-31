/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for symLib.h */
#ifndef _OSAL_OVERRIDE_SYMLIB_H_
#define _OSAL_OVERRIDE_SYMLIB_H_

#include <OCS_symLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in symLib.h */
/* ----------------------------------------- */
#define LOAD_ALL_SYMBOLS     OCS_LOAD_ALL_SYMBOLS

#define SYM_TYPE             OCS_SYM_TYPE
#define SYMTAB               OCS_SYMTAB
#define SYMBOL               OCS_SYMBOL
#define SYMTAB_ID            OCS_SYMTAB_ID
#define SYMBOL_ID            OCS_SYMBOL_ID

#define sysSymTbl            OCS_sysSymTbl
#define symFindByName        OCS_symFindByName
#define symEach              OCS_symEach


#endif /* _OSAL_OVERRIDE_SYMLIB_H_ */
