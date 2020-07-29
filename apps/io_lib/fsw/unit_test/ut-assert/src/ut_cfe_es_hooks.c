/*
**
** File: ut_cfe_es_hooks.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_es_hooks.c 1.1 2011/05/04 11:20:51EDT rmcgraw Exp  $
**
** Purpose: Unit test hooks for cFE Executive Services routines
**
** $Log: ut_cfe_es_hooks.c  $
** Revision 1.1 2011/05/04 11:20:51EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:36EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/03/07 17:54:30EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"

int32 Ut_CFE_ES_RunLoopHook(uint32 *ExitStatus)
{
    if (*ExitStatus == CFE_ES_APP_RUN) {
        return(TRUE);
    }
    else { /* CFE_ES_APP_EXIT, CFE_ES_APP_ERROR */
        return(FALSE);
    }
}
