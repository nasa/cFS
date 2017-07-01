/************************************************************************
** File:
**   $Id: ds_msgids.h 1.3 2017/01/25 12:29:18EST sstrege Exp  $
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
**  The CFS Data Storage (DS) Application Message IDs header file
**
** Notes:
**
****************************************************************************/
#ifndef _ds_msgids_h_
#define _ds_msgids_h_

/**************************
** DS Command Message IDs
***********************/

#define DS_CMD_MID         0x18BB /**< \brief DS Ground Commands Message ID */
#define DS_SEND_HK_MID     0x18BC /**< \brief DS Send Hk Data Cmd Message ID*/


/***************************************
** DS Telemetry Message IDs
***************************************/

#define DS_HK_TLM_MID      0x08B8 /**< \brief DS Hk Telemetry Message ID ****/
#define DS_DIAG_TLM_MID    0x08B9 /**< \brief DS File Info Telemetry Message ID ****/


#endif /* _ds_msgids_h_ */

/************************/
/*  End of File Comment */
/************************/
