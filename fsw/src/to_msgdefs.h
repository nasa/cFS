/************************************************************************
** File:
**  to_msgdefs.h
**
**  Copyright © 2016 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Johnson Space Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: 
**   Specification for the CFS Telemetry Output (TO) command and telemetry 
**   message constant definitions.
**
** Notes:
**   These Macro definitions have been put in this file so this file can 
**   be included directly into ASIST build test scripts. ASIST RDL files 
**   can accept C language #defines but can't handle type definitions. 
**   As a result: DO NOT PUT ANY TYPEDEFS OR STRUCTURE DEFINITIONS IN 
**   THIS FILE! 
**   ADD THEM TO ANOTHER HEADER IF NEEDED! 
**
**   ASIST documentation has yet to be provided for all the items in this file.
**
**  \par Modification History:
**     - 2016-05-11 | Allen Brown | Initial Version
** 
*************************************************************************/

#ifndef _to_msgdefs_
#define _to_msgdefs_

/************************************************************************
** Macro Definitions
*************************************************************************/


/************************************************************************
** Command Code Definitions
*************************************************************************/

/** \tocmd Noop 
**  
**  \par Description
**       Implements the Noop command that insures the TO app is alive
**
**  \tocmdmnemonic \TO_NOOP
**
**  \par Command Structure
**       To be documented.
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry: To be documented.
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       To be documented.
**
**  \par Criticality
**       To be documented.
**
**  \sa #TO_RESET_CC
*/
#define TO_NOOP_CC                      0

/* Note, the rest of these require ASIST documentation. */
#define TO_RESET_CC                     1
#define TO_ENABLE_OUTPUT_CC             2
#define TO_DISABLE_OUTPUT_CC            3
#define TO_PAUSE_OUTPUT_CC              4
#define TO_RESUME_OUTPUT_CC             5
#define TO_ADD_TBL_ENTRY_CC             6
#define TO_REMOVE_TBL_ENTRY_CC          7
#define TO_ENABLE_TBL_ENTRY_CC          8
#define TO_DISABLE_TBL_ENTRY_CC         9
#define TO_ENABLE_GROUP_CC              10
#define TO_DISABLE_GROUP_CC             11
#define TO_ENABLE_ALL_CC                12
#define TO_DISABLE_ALL_CC               13
#define TO_SET_ROUTE_BY_MID_CC          14
#define TO_SET_ROUTE_BY_GROUP_CC        15
#define TO_MANAGE_TABLE_CC              16
#define TO_ACTIVATE_ROUTES_CC           17
#define TO_DEACTIVATE_ROUTES_CC         18
#define TO_SET_ROUTE_PERIOD_CC          19
#define TO_SET_WAKEUP_TIMEOUT_CC        20

/* Public Custom Commands */
#define TO_SEND_DATA_TYPE_CC            30

/* Internal Custom Commands */
#define TO_SET_OCF_DATA_CC              40

#endif /* _to_msgdefs_ */

/************************/
/*  End of File Comment */
/************************/
