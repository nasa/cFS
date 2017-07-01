/************************************************************************
** File:
**   $Id: sch_msgids.h 1.3 2015/03/01 14:02:06EST sstrege Exp  $
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
**  The CFS Scheduler (SCH) Application Message IDs header file
**
** Notes:
**
** $Log: sch_msgids.h  $
** Revision 1.3 2015/03/01 14:02:06EST sstrege 
** Added copyright information
** Revision 1.2 2011/06/02 16:19:39EDT aschoeni 
** Updated platform config and verification
** Revision 1.1 2008/10/16 15:08:00EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/platform_inc/project.pj
**
*************************************************************************/
#ifndef _sch_msgids_h_
#define _sch_msgids_h_

/**************************
** SCH Command Message IDs
***************************/
#define SCH_CMD_MID                    0x1895 /**< \brief SCH Ground Commands Message ID */
#define SCH_SEND_HK_MID                0x1896 /**< \brief SCH Send Housekeeping Message ID */
#define SCH_UNUSED_MID                 0x1897 /**< \brief SCH MDT Unused Message Message ID */
/*
#define SCH_SPARE1                     0x1898
#define SCH_SPARE2                     0x1899
*/

/***************************
** SCH Telemetry Message IDs
****************************/
#define SCH_HK_TLM_MID                 0x0897 /**< \brief SCH Housekeeping Telemetry Message ID */
#define SCH_DIAG_TLM_MID               0x0898 /**< \brief SCH Diagnostic Telemetry Message ID */
/* 
#define SCH_TLM_SPARE1                 0x0899
#define SCH_TLM_SPARE2                 0x089A
*/

#endif /* _sch_msgids_h_ */

/************************/
/*  End of File Comment */
/************************/
