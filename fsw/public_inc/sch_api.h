
/*
** $Id: sch_api.h 1.1 2011/06/30 13:56:03EDT aschoeni Exp  $
**
** Purpose: This file contains api functions to change and obtain SCH Schedule Processing Enable State
** 
** Author:
**
** References: 
**
** $Log: sch_api.h  $
** Revision 1.1 2011/06/30 13:56:03EDT aschoeni 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/public_inc/project.pj
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

