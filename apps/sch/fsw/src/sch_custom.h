/*************************************************************************
** File:
**   $Id: sch_custom.h 1.5 2017/06/21 15:29:23EDT mdeschu Exp  $
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
**   Specification for the CFS Scheduler (SCH) mission specific
**   custom function interface
**
** Notes:
** 
**************************************************************************/
#ifndef _sch_custom_
#define _sch_custom_

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

/*************************************************************************
** Macro definitions
**************************************************************************/

/*
** Timer Characteristics
*/
#define SCH_TIMER_NAME   "SCH_MINOR_TIMER"

/*************************************************************************
** Exported Functions
*************************************************************************/

/************************************************************************/
/** \brief Custom Early Initialization
**  
**  \par Description
**       This function is intended to perform the creation of the
**       minor frame timer. It is called during #SCH_TimerInit
**       It may be updated to include other initializations, or 
**       modifications to already set scheduler parameters.
**
**  \par Assumptions, External Events, and Notes:
**       SCH_AppData.ClockAccuracy will be set to the clock resolution 
**       in microseconds. 
**       CFE_SUCCESS will be returned if all creation was performed
**       properly.
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_PSP_TimerInit        \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_CustomEarlyInit(void);

/************************************************************************/
/** \brief Custom Late Initialization
**  
**  \par Description
**       This function is intended to perform a wait for startup sync
**       followed by the initialization and starting of the major frame
**       timer, and the starting of the minor frame timer. It may be 
**       updated to include other initializations, or modifications to 
**       already set scheduler parameters, or remove actions previously
**       performed in #SCH_CustomEarlyInit . This function is called
**       following the completion of #SCH_AppInit
**
**  \par Assumptions, External Events, and Notes:
**       Any startup synchronization is included in this function
**       CFE_SUCCESS will be returned if all initialization was performed
**       properly.
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_PSP_TimerInit        \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_CustomLateInit(void);

/************************************************************************/
/** \brief Obtains the Current Slot (Minor Frame) number
**  
**  \par Description
**       This function determines the current slot (minor frame) number.
**       It corrects for any minor frame overlap caused by poor minor frame
**       timing, if necessary. This function and associated subfunction
**       was added to the customizable section as on slower platforms
**       this function can have noticible overhead, and several simplifying
**       assumptions can be made on a platform specific basis.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retstmt Returns slot index from zero to (#SCH_TOTAL_SLOTS-1) \endcode
**  \endreturns
**
*************************************************************************/
uint32 SCH_CustomGetCurrentSlotNumber(void);

/************************************************************************/
/** \brief Custom Cleanup
**  
**  \par Description
**       This function is intended to perform any clean up of custom
**       initialization that would be necessary on an application exit
**
**  \par Assumptions, External Events, and Notes:
**       Any resources that will not be cleaned up automatically be CFE
**       need to be cleaned up in this function.
**       
*************************************************************************/
void SCH_CustomCleanup(void);

/************************************************************************/
/** \brief Computes a minor slot number from a MET subseconds zero point
**  
**  \par Description
**       This function determines the current slot (minor frame) number if
**       one were to assume that slot zero started when the MET microseconds
**       are equal to zero. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retstmt Returns slot index from zero to (#SCH_TOTAL_SLOTS-1) \endcode
**  \endreturns
**
*************************************************************************/
uint32 SCH_GetMETSlotNumber(void);

/************************************************************************/
/** \brief Performs Major Frame Synchronization
**  
**  \par Description
**       This function is called by cFE TIME services when a Major Frame
**       synchronization signal is received.  It then synchronizes the
**       minor frame (slot) processing of the Schedule Definition Table. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void  SCH_MajorFrameCallback(void);

/************************************************************************/
/** \brief Performs Minor Frame time step
**  
**  \par Description
**       This function is called by an OSAL timer when the minor frame
**       timing reference sends a signal.  The Scheduler Application uses
**       this to drive the Application's processing of each minor frame. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void  SCH_MinorFrameCallback(uint32 TimerId);

#endif /* _sch_custom_ */
/************************/
/*  End of File Comment */
/************************/
