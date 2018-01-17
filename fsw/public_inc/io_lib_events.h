/*=======================================================================================
** File Name:  io_lib_events.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Title:  ID Header File for IO_LIB Application
**
** $Author:    Guy de Carufel
** $Revision:  $
** $Date:      2015-01-09
**
** Purpose:  This header file contains definitions of IO_LIB Events.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2015-01-09 | Guy de Carufel | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _IO_LIB_EVENTS_H_
#define _IO_LIB_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Event IDs */
typedef enum
{
    IO_LIB_RESERVED_EID             = 0,
    IO_LIB_INF_EID                  = 1,
    IO_LIB_ERR_EID                  = 2,
    IO_LIB_FORMAT_INF_EID           = 3,
    IO_LIB_SERVICE_INF_EID          = 4,
    IO_LIB_FORMAT_ERR_EID           = 5,
    IO_LIB_SERVICE_ERR_EID          = 6,
    IO_LIB_TRANS_UDP_EID            = 7,
    IO_LIB_TRANS_RS422_EID          = 8,
    IO_LIB_TRANS_SELECT_EID         = 9,
    IO_LIB_COP1_EID                 = 10,
    IO_LIB_TM_SDLP_EID              = 11,
    IO_LIB_EVT_CNT 
} IO_LIB_Events_t;

#ifdef __cplusplus
}
#endif

#endif 

/*=======================================================================================
** End of file io_lib_events.h
**=====================================================================================*/
    
