/*
** $Id: sch_api.c 1.4 2015/03/01 14:01:58EST sstrege Exp  $
**
**  Copyright © 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: This file contains api functions to modify and obtain the current
**          SCH Schedule Processing state.
** 
** Author:
**
** References: 
**
** $Log: sch_api.c  $
** Revision 1.4 2015/03/01 14:01:58EST sstrege 
** Added copyright information
** Revision 1.3 2011/07/29 13:23:19EDT aschoeni 
** Added return of OS_SUCCESS to LibInit
** Revision 1.2 2011/06/30 15:47:01EDT aschoeni 
** added LibData
** Revision 1.1 2011/06/30 13:57:46EDT aschoeni 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
*/

/*
** Includes
*/

#include "sch_api.h"
#include "sch_apipriv.h"
#include "sch_platform_cfg.h"

/*
** Exported data
*/

SCH_LibData_t SCH_LibData;

/*******************************************************************
**
** SCH_LibInit() -- Initializes the SCH Library
**
** NOTE: For complete prolog information, see 'sch_apipriv.h'.
********************************************************************/
int32 SCH_LibInit(void)
{
    SCH_LibData.ProcessingDisabledCtr = SCH_LIB_DIS_CTR;
    return OS_SUCCESS;
}

/*******************************************************************
**
** SCH_EnableProcessing() -- Enables SCH schedule processing
**
** NOTE: For complete prolog information, see 'sch_api.h'.
********************************************************************/
void SCH_EnableProcessing(void)
{
    if (SCH_LibData.ProcessingDisabledCtr > 0)
    {
        SCH_LibData.ProcessingDisabledCtr--;
    }
}


/*******************************************************************
**
** SCH_DisableProcessing() -- Disables SCH schedule processing
**
** NOTE: For complete prolog information, see 'sch_api.h'.
********************************************************************/
void SCH_DisableProcessing(void)
{
    SCH_LibData.ProcessingDisabledCtr++;
}


/*******************************************************************
**
** SCH_GetProcessingState() -- Obtains the current SCH schedule
**                             table processing state
**
** NOTE: For complete prolog information, see 'sch_api.h'.
********************************************************************/
boolean SCH_GetProcessingState(void)
{
    return (SCH_LibData.ProcessingDisabledCtr == 0);
}
