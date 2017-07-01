/*
** $Id: sch_def_msgtbl.c 1.4.1.1 2015/03/01 14:13:09EST sstrege Exp  $
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
** $Log: sch_def_msgtbl.c  $
** Revision 1.4.1.1 2015/03/01 14:13:09EST sstrege 
** Added copyright information
** Revision 1.4 2012/07/20 17:09:19EDT aschoeni 
** Fixed table compiler warning
** Revision 1.3 2011/07/21 14:51:01EDT aschoeni 
** removed default entries from table and replaced with unused entries
** Revision 1.2 2011/06/30 20:30:49EDT aschoeni 
** updated table header
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
/* #include "ci_lab_msgids.h" */
/* #include "to_lab_msgids.h" */
/* #include "cs_msgids.h"  */
/* #include "ds_msgids.h"  */
/* #include "fm_msgids.h"  */
/* #include "hk_msgids.h"  */
/* #include "hs_msgids.h"  */
/* #include "lc_msgids.h"  */
/* #include "md_msgids.h"  */
/* #include "mm_msgids.h"  */
/* #include "sc_msgids.h"  */
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
** Message Table entry map...
**
**  Entry 0 -- reserved (DO NOT USE)
**  
**  Several Entries in this default table provide example messages for a default
**  system. These messages can be uncommented, and the SCH_UNUSED_MID entry just
**  below them can be deleted to enable them.
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

  /*
  **  cFE housekeeping request messages
  */
    /* command ID #1 - Executive Services HK Request   */
  { { CFE_ES_SEND_HK_MID,   0xC000, 0x0001, 0x0000 } },
    /* command ID #2 - Event Services HK Request     */
  { { CFE_EVS_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } },
    /* command ID #3 - Software Bus HK Request       */
  { { CFE_SB_SEND_HK_MID,   0xC000, 0x0001, 0x0000 } },
    /* command ID #4 - Time Services HK Request      */
  { { CFE_TIME_SEND_HK_MID, 0xC000, 0x0001, 0x0000 } },
    /* command ID #5 - Table Services HK Request     */
  { { CFE_TBL_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } },

  /*
  **  CFS housekeeping request messages
  */
    /* command ID #6 - Checksum HK Request           */
/*{ { CS_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #7 - Data Store HK Request         */
/*{ { DS_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #8 - File Manager HK Request       */
/*{ { FM_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #9 - Housekeeping HK Request       */
/*{ { HK_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },

    /* command ID #10 - Health & Safety HK Request   */
/*{ { HS_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #11 - Limit Checker HK Request     */
/*{ { LC_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #12 - Memory Dwell HK Request      */
/*{ { MD_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #13 - Memory Manager HK Request    */
/*{ { MM_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #14 - Stored Command HK Request    */
/*{ { SC_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #15 - Scheduler HK Request         */
  { { SCH_SEND_HK_MID, 0xC000, 0x0001, 0x0000 } },

  /*
  **  CFS routine messages
  */
    /* command ID #16 - HK Send Combined Housekeeping Msg #1 */
/*{ { HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003, 0x0000, HK_COMBINED_PKT1_MID } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #17 - HK Send Combined Housekeeping Msg #2 */
/*{ { HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003, 0x0000, HK_COMBINED_PKT2_MID } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #18 - HK Send Combined Housekeeping Msg #3 */
/*{ { HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003, 0x0000, HK_COMBINED_PKT3_MID } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #19 - HK Send Combined Housekeeping Msg #4 */
/*{ { HK_SEND_COMBINED_PKT_MID, 0xC000, 0x0003, 0x0000, HK_COMBINED_PKT4_MID } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #20 - CS Background Cycle               */
/*{ { CS_BACKGROUND_CYCLE_MID, 0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #21 - SC 1 Hz Wakeup                    */
/*{ { SC_1HZ_WAKEUP_MID,       0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #22 - LC Sample Action Points           */
/*{ { LC_SAMPLE_AP_MID,        0xC000, 0x0005, 0x0000, LC_ALL_ACTIONPOINTS, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #23 - DS 1 HZ Wakeup                    */
/*{ { DS_1HZ_WAKEUP_MID,       0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #24 - MD Wakeup                         */
/*{ { MD_WAKEUP_MID,           0xC000, 0x0001, 0x0000 } }, */
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
    /* command ID #30 - Command Ingest HK Request Example */
/*{ { CI_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
  { { SCH_UNUSED_MID } },
    /* command ID #31 - Telemetry Output HK Request Example */
/*{ { TO_SEND_HK_MID,  0xC000, 0x0001, 0x0000 } }, */
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

/*
** Table file header
*/
CFE_TBL_FILEDEF(SCH_DefaultMessageTable, SCH.MSG_DEFS, SCH message definitions table, sch_def_msgtbl.tbl)

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

