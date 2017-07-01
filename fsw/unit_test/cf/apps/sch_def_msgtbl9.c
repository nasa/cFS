/*
** $Id: sch_def_msgtbl9.c 1.2 2015/03/01 14:01:56EST sstrege Exp  $
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
** Purpose: Scheduler (SCH) default message definition table data
**
** Author: 
**
** Notes:
**
** $Log: sch_def_msgtbl9.c  $
** Revision 1.2 2015/03/01 14:01:56EST sstrege 
** Added copyright information
** Revision 1.1 2011/06/30 21:00:55EDT aschoeni 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/unit_test/cf/apps/project.pj
** Revision 1.1 2009/03/27 00:32:24EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/tables/project.pj
*/

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "sch_platform_cfg.h"
#include "sch_tbldefs.h"

#include "cfe_msgids.h"
#include "sch_msgids.h"



/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Type definitions
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Exported data
**
**************************************************************************/

/*
** Table file header
*/
static CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "SCH_DefaultMessageTable", "SCH.MSG_DEFS", "SCH message definitions table",
    "sch_def_msgtbl9.tbl", (sizeof(SCH_MessageEntry_t) * SCH_MAX_MESSAGES)
}; 

/*
** Message Table entry map...
**
**  Entry 0 -- reserved (DO NOT USE)
*/

/*
** Default command definition table data
*/
SCH_MessageEntry_t SCH_DefaultMessageTable[SCH_MAX_MESSAGES] =
{
  /*
  **  DO NOT USE -- entry #0 reserved for "unused" command ID - DO NOT USE
  */
    /* command ID #0 */
  { { SCH_UNUSED_MID } },

    /* command ID #1 */
  { { 0x1901, 0xC000, 0x0001, 0x0000 } },
    /* command ID #2 */
  { { 0x1902, 0xC000, 0x0001, 0x0000 } },
    /* command ID #3 */
  { { 0x1903, 0xC000, 0x0001, 0x0000 } },
    /* command ID #4 */
  { { 0x1904, 0xC000, 0x0001, 0x0000 } },  
    /* command ID #5 */
  { { 0x1905, 0xC000, 0x0001, 0x0000 } },
    /* command ID #6 */
  { { 0x1906, 0xC000, 0x0001, 0x0000 } },
    /* command ID #7 */
  { { 0x1907, 0xC000, 0x0001, 0x0000 } },
    /* command ID #8 */
  { { 0x1900, 0xC000, 0x0001, 0x0000 } },
    /* command ID #9 */
  { { SCH_UNUSED_MID } },

    /* command ID #10 */
  { { SCH_UNUSED_MID } },
    /* command ID #11 */
  { { SCH_UNUSED_MID } },
    /* command ID #12 */
  { { SCH_UNUSED_MID } },
    /* command ID #13 */
  { { SCH_UNUSED_MID } },
    /* command ID #14 */
  { { SCH_UNUSED_MID } },  
    /* command ID #15 */
  { { SCH_UNUSED_MID } },
    /* command ID #16 */
  { { SCH_UNUSED_MID } },
    /* command ID #17 */
  { { SCH_UNUSED_MID } },
    /* command ID #18 */
  { { SCH_UNUSED_MID } },
    /* command ID #19 */
  { { SCH_UNUSED_MID } },

    /* command ID #20 */
  { { SCH_UNUSED_MID } },
    /* command ID #21 */
  { { SCH_UNUSED_MID } },
    /* command ID #22 */
  { { 0x1908, 0xC000, SCH_MAX_MSG_WORDS * 2, 0x0000 } },
    /* command ID #23 */
  { { SCH_UNUSED_MID } },
    /* command ID #24 */
  { { SCH_UNUSED_MID } },  
    /* command ID #25 */
  { { SCH_UNUSED_MID } },
    /* command ID #26 */
  { { SCH_UNUSED_MID } },
    /* command ID #27 */
  { { SCH_UNUSED_MID } },
    /* command ID #28 */
  { { SCH_UNUSED_MID } },
    /* command ID #29 */
  { { SCH_UNUSED_MID } },
  /*
  **  Mission Defined Messages
  */
    /* command ID #30 */
  { { SCH_UNUSED_MID } },
    /* command ID #31 */
  { { SCH_UNUSED_MID } },
    /* command ID #32 */
  { { SCH_UNUSED_MID } },
    /* command ID #33 */
  { { SCH_UNUSED_MID } },
    /* command ID #34 */
  { { SCH_UNUSED_MID } },  
    /* command ID #35 */
  { { SCH_UNUSED_MID } },
    /* command ID #36 */
  { { SCH_UNUSED_MID } },
    /* command ID #37 */
  { { SCH_UNUSED_MID } },
    /* command ID #38 */
  { { SCH_UNUSED_MID } },
    /* command ID #39 */
  { { SCH_UNUSED_MID } },
  
    /* command ID #40 */
  { { SCH_UNUSED_MID } },
    /* command ID #41 */
  { { SCH_UNUSED_MID } },
    /* command ID #42 */
  { { SCH_UNUSED_MID } },
    /* command ID #43 */
  { { SCH_UNUSED_MID } },
    /* command ID #44 */
  { { SCH_UNUSED_MID } },  
    /* command ID #45 */
  { { SCH_UNUSED_MID } },
    /* command ID #46 */
  { { SCH_UNUSED_MID } },
    /* command ID #47 */
  { { SCH_UNUSED_MID } },
    /* command ID #48 */
  { { SCH_UNUSED_MID } },
    /* command ID #49 */
  { { SCH_UNUSED_MID } },

    /* command ID #50 */
  { { SCH_UNUSED_MID } },
    /* command ID #51 */
  { { SCH_UNUSED_MID } },
    /* command ID #52 */
  { { SCH_UNUSED_MID } },
    /* command ID #53 */
  { { SCH_UNUSED_MID } },
    /* command ID #54 */
  { { SCH_UNUSED_MID } },  
    /* command ID #55 */
  { { SCH_UNUSED_MID } },
    /* command ID #56 */
  { { SCH_UNUSED_MID } },
    /* command ID #57 */
  { { SCH_UNUSED_MID } },
    /* command ID #58 */
  { { SCH_UNUSED_MID } },
    /* command ID #59 */
  { { SCH_UNUSED_MID } },

    /* command ID #60 */
  { { SCH_UNUSED_MID } },
    /* command ID #61 */
  { { SCH_UNUSED_MID } },
    /* command ID #62 */
  { { SCH_UNUSED_MID } },
    /* command ID #63 */
  { { SCH_UNUSED_MID } },
    /* command ID #64 */
  { { SCH_UNUSED_MID } },  
    /* command ID #65 */
  { { SCH_UNUSED_MID } },
    /* command ID #66 */
  { { SCH_UNUSED_MID } },
    /* command ID #67 */
  { { SCH_UNUSED_MID } },
    /* command ID #68 */
  { { SCH_UNUSED_MID } },
    /* command ID #69 */
  { { SCH_UNUSED_MID } },

    /* command ID #70 */
  { { SCH_UNUSED_MID } },
    /* command ID #71 */
  { { SCH_UNUSED_MID } },
    /* command ID #72 */
  { { SCH_UNUSED_MID } },
    /* command ID #73 */
  { { SCH_UNUSED_MID } },
    /* command ID #74 */
  { { SCH_UNUSED_MID } },
    /* command ID #75 */
  { { SCH_UNUSED_MID } },
    /* command ID #76 */
  { { SCH_UNUSED_MID } },
    /* command ID #77 */
  { { SCH_UNUSED_MID } },
    /* command ID #78 */
  { { SCH_UNUSED_MID } },
    /* command ID #79 */
  { { SCH_UNUSED_MID } },
  
    /* command ID #80 */
  { { SCH_UNUSED_MID } },
    /* command ID #81 */
  { { SCH_UNUSED_MID } },
    /* command ID #82 */
  { { SCH_UNUSED_MID } },
    /* command ID #83 */
  { { SCH_UNUSED_MID } },
    /* command ID #84 */
  { { SCH_UNUSED_MID } },
    /* command ID #85 */
  { { SCH_UNUSED_MID } },
    /* command ID #86 */
  { { SCH_UNUSED_MID } },
    /* command ID #87 */
  { { SCH_UNUSED_MID } },
    /* command ID #88 */
  { { SCH_UNUSED_MID } },
    /* command ID #89 */
  { { SCH_UNUSED_MID } },
  
    /* command ID #90 */
  { { SCH_UNUSED_MID } },
    /* command ID #91 */
  { { SCH_UNUSED_MID } },
    /* command ID #92 */
  { { SCH_UNUSED_MID } },
    /* command ID #93 */
  { { SCH_UNUSED_MID } },
    /* command ID #94 */
  { { SCH_UNUSED_MID } },
    /* command ID #95 */
  { { SCH_UNUSED_MID } },
    /* command ID #96 */
  { { SCH_UNUSED_MID } },
    /* command ID #97 */
  { { SCH_UNUSED_MID } },
    /* command ID #98 */
  { { SCH_UNUSED_MID } },
    /* command ID #99 */
  { { SCH_UNUSED_MID } },
  
    /* command ID #100 */
  { { SCH_UNUSED_MID } },
    /* command ID #101 */
  { { SCH_UNUSED_MID,} },
    /* command ID #102 */
  { { SCH_UNUSED_MID } },
    /* command ID #103 */
  { { SCH_UNUSED_MID } },
    /* command ID #104 */
  { { SCH_UNUSED_MID } },
    /* command ID #105 */
  { { SCH_UNUSED_MID } },
    /* command ID #106 */
  { { SCH_UNUSED_MID } },
    /* command ID #107 */
  { { SCH_UNUSED_MID } },
    /* command ID #108 */
  { { SCH_UNUSED_MID } },
    /* command ID #109 */
  { { SCH_UNUSED_MID } },
  
    /* command ID #110 */
  { { SCH_UNUSED_MID } },
    /* command ID #111 */
  { { SCH_UNUSED_MID } },
    /* command ID #112 */
  { { SCH_UNUSED_MID } },
    /* command ID #113 */
  { { SCH_UNUSED_MID } },
    /* command ID #114 */
  { { SCH_UNUSED_MID } },
    /* command ID #115 */
  { { SCH_UNUSED_MID } },
    /* command ID #116 */
  { { SCH_UNUSED_MID } },
    /* command ID #117 */
  { { SCH_UNUSED_MID } },
    /* command ID #118 */
  { { SCH_UNUSED_MID } },
    /* command ID #119 */
  { { SCH_UNUSED_MID } },
  
    /* command ID #120 */
  { { SCH_UNUSED_MID } },
    /* command ID #121 */
  { { SCH_UNUSED_MID } },
    /* command ID #122 */
  { { SCH_UNUSED_MID } },
    /* command ID #123 */
  { { SCH_UNUSED_MID } },
    /* command ID #124 */
  { { SCH_UNUSED_MID } },
    /* command ID #125 */
  { { SCH_UNUSED_MID } },
    /* command ID #126 */
  { { SCH_UNUSED_MID } },
    /* command ID #127 */
  { { SCH_UNUSED_MID } }

};


/*************************************************************************
**
** File data
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Local function prototypes
**
**************************************************************************/

/*
** (none)
*/

/************************/
/*  End of File Comment */
/************************/

