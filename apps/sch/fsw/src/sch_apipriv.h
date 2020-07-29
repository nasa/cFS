/************************************************************************
** File:
**   $Id: sch_apipriv.h 1.4 2017/06/21 15:29:01EDT mdeschu Exp  $
**
**  Copyright (c) 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: 
**  The CFS Scheduler (SCH) API header file
**
** Notes:
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

/************************************************************************/
/** \brief SCH_EnableProcessing()
**  
**  \par Description
**       Enables SCH schedule processing.
**       
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void SCH_EnableProcessing(void);

/************************************************************************/
/** \brief SCH_DisableProcessing()
**  
**  \par Description
**       Disables SCH schedule processing.
**       
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void SCH_DisableProcessing(void);

/************************************************************************/
/** \brief SCH_GetProcessingState()
**  
**  \par Description
**       Obtains the current SCH schedule table processing state.
**       
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
boolean SCH_GetProcessingState(void);

#endif /* _sch_apipriv_ */

/************************/
/*  End of File Comment */
/************************/
