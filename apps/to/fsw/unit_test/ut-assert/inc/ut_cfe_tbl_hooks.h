/*
**
** File: ut_cfe_tbl_hooks.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_tbl_hooks.h 1.1 2011/05/04 11:20:22EDT rmcgraw Exp  $
**
** Purpose: Unit test header file for cFE Table Services hooks.
**
** $Log: ut_cfe_tbl_hooks.h  $
** Revision 1.1 2011/05/04 11:20:22EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:56EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.2 2011/02/18 15:57:43EST sslegel 
** Added new hooks and return codes
** Changed Ut_CFE_TBL_LoadHook to automatically call the table validate function
** Revision 1.1 2011/02/15 11:12:34EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_CFE_TBL_HOOKS_H_
#define UT_CFE_TBL_HOOKS_H_

#include "cfe.h"

void        Ut_CFE_TBL_ClearTables(void);
int32       Ut_CFE_TBL_RegisterTable(const char *Name, uint32 Size, uint16 TblOptionFlags, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr);
int32       Ut_CFE_TBL_AddTable(char *Filename, void *TablePtr);
int32       Ut_CFE_TBL_LoadTable(CFE_TBL_Handle_t TblHandle, void *SrcDataPtr);
int32       Ut_CFE_TBL_FindTable(char *Filename);
void       *Ut_CFE_TBL_GetAddress(CFE_TBL_Handle_t TblHandle);
int32       Ut_CFE_TBL_RegisterHook(CFE_TBL_Handle_t *TblHandlePtr, const char *Name, uint32 Size, uint16 TblOptionFlags, CFE_TBL_CallbackFuncPtr_t TblValidationFuncPtr);
int32       Ut_CFE_TBL_LoadHook(CFE_TBL_Handle_t TblHandle, CFE_TBL_SrcEnum_t SrcType, const void *SrcDataPtr);
int32       Ut_CFE_TBL_GetAddressHook(void **TblPtr, CFE_TBL_Handle_t TblHandle);

#endif
