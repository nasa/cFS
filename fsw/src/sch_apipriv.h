/************************************************************************
** File:
**   $Id: sch_apipriv.h 1.3 2015/03/01 14:01:27EST sstrege Exp  $
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
** Purpose: 
**  The CFS Scheduler (SCH) Application header file
**
** Notes:
**
** $Log: sch_apipriv.h  $
** Revision 1.3 2015/03/01 14:01:27EST sstrege 
** Added copyright information
** Revision 1.2 2011/07/29 13:23:27EDT aschoeni 
** Added return of OS_SUCCESS to LibInit
** Revision 1.1 2011/06/30 13:57:47EDT aschoeni 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
**
*************************************************************************/
#ifndef _sch_apipriv_
#define _sch_apipriv_

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"

/*************************************************************************
**
** Type definitions
**
**************************************************************************/

/*
** Type definition (SCH lib global data)
*/
typedef struct
{
    uint32         ProcessingDisabledCtr;  /**< \brief Processing disable if greater than zero */
} SCH_LibData_t;

/*************************************************************************
**
** Exported data
**
**************************************************************************/

extern SCH_LibData_t    SCH_LibData;

/*************************************************************************
**
** Exported functions
**
**************************************************************************/

/*
** Library entry point for initialization
*/
/************************************************************************/
/** \brief CFS Scheduler (SCH) Library Entry Point
**  
**  \par Description
**       Scheduler library entry point for initialization.
**       
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
int32  SCH_LibInit(void);

#endif /* _sch_apipriv_ */

/************************/
/*  End of File Comment */
/************************/
