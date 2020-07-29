/*
** $Id: sch_api.h 1.3 2017/06/21 15:28:57EDT mdeschu Exp  $
**
** Copyright (c) 2007-2014 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: This file contains api functions to change and obtain SCH Schedule Processing Enable State
** 
** Author:
**
** References: 
**
*/

#ifndef _sch_api_h_
#define _sch_api_h_

/*
** Includes
*/

#include "cfe.h"

/*
** Exported Functions
*/

/*****************************************************************************/
/**
** \brief Informs SCH Application that processing of schedule table can resume
**
** \par Description
**      This API decrements the disable schedule table processing reference count.
**      Once all disable references have been eliminated, the SCH application will
**      resume schedule table processing.
**
** \par Assumptions, External Events, and Notes:
**          None
**
******************************************************************************/
void SCH_EnableProcessing(void);


/*****************************************************************************/
/**
** \brief Informs SCH Application that processing of schedule table should halt
**
** \par Description
**      This API increments the disable schedule table processing reference count.
**      Each time this API is called must be matched with a similar call to the
**      SCH_EnableProcessing API before the SCH Application will resume
**      processing of the schedule table.
**
** \par Assumptions, External Events, and Notes:
**          None
**
******************************************************************************/
void SCH_DisableProcessing(void);


/*****************************************************************************/
/**
** \brief Obtains SCH Application processing state
**
** \par Description
**      This API returns a TRUE/FALSE indication as to whether the SCH Application
**      is allowed to process the contents of the Schedule table.
**
** \par Assumptions, External Events, and Notes:
**          None
**
** \returns
** \retcode #TRUE   \retdesc Schedule Table Processing is Enabled   \endcode
** \retcode #FALSE  \retdesc Schedule Table Processing is Disabled  \endcode
** \endreturns
**
**
******************************************************************************/
boolean SCH_GetProcessingState(void);

#endif /* _sch_api_ */

/************************/
/*  End of File Comment */
/************************/

