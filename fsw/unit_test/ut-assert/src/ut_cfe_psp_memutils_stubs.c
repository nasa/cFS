/*
**
** File: ut_cfe_psp_memutils_stubs.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_psp_memutils_stubs.c 1.1 2011/05/04 11:20:54EDT rmcgraw Exp  $
**
** Purpose: Unit test stubs for cFE PSP Memory Utilities routines
**
** $Log: ut_cfe_psp_memutils_stubs.c  $
** Revision 1.1 2011/05/04 11:20:54EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:39EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/02/15 11:13:02EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

/*
**  Include Files
*/
#include "cfe.h"
#include <string.h>

int32 CFE_PSP_MemCpy(void *Dest, void *Src, uint32 Size)
{
    memcpy(Dest, Src, Size);
    return(CFE_PSP_SUCCESS);
}

int32 CFE_PSP_MemSet(void *Dest, uint8 Value, uint32 Size)
{
    memset(Dest, Value, Size);
    return(CFE_PSP_SUCCESS);
}
