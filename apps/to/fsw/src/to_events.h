/******************************************************************************/
/** \file  to_events.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief ID Header File for TO Application
*
*   \par
*       This header file contains definitions of the TO Event IDs
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*******************************************************************************/
    
#ifndef _TO_EVENTS_H_
#define _TO_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Event IDs */
typedef enum
{
    TO_RESERVED_EID       =  0,
    TO_INF_EID            =  1,
    TO_INIT_INF_EID       =  2,
    TO_CMD_INF_EID        =  3,
    TO_TBL_INF_EID        =  4,
    TO_CUSTOM_INF_EID     =  5, 
    TO_ERR_EID            =  6,
    TO_INIT_ERR_EID       =  7,
    TO_CMD_ERR_EID        =  8,
    TO_TBL_ERR_EID        =  9,
    TO_PIPE_ERR_EID       = 10,
    TO_MSGID_ERR_EID      = 11,
    TO_MSGLEN_ERR_EID     = 12,
    TO_CUSTOM_ERR_EID     = 13,
    TO_EVT_CNT
} TO_Events_t;

#ifdef __cplusplus
}
#endif

#endif 

/*==============================================================================
** End of file to_events.h
**============================================================================*/
    
