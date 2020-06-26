/************************************************************************
** File: ds_msgids.h
**
** NASA Docket No. GSC-16,126-1, and identified as "Core Flight Software System
** (CFS) Data Storage Application Version 2” 
**
** Copyright © 2007-2014 United States Government as represented by the
** Administrator of the National Aeronautics and Space Administration. All Rights
** Reserved. 
** 
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
** http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License.
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
