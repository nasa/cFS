/*
** $Id: sch_def_msgtbl.c 1.3 2017/06/21 15:28:56EDT mdeschu Exp  $
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
** Purpose: Scheduler (SCH) default message definition table data
**
** Author: 
**
** Notes:
**
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
**  system. These messages can be uncommented, and the CFE_MAKE_BIG16(SCH_UNUSED_MID) entry just
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
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },

  /*
  **  cFE housekeeping request messages
  */
    /* command ID #1 - Executive Services HK Request   */
  { { CFE_MAKE_BIG16(CFE_ES_SEND_HK_MID),   CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } },
    /* command ID #2 - Event Services HK Request     */
  { { CFE_MAKE_BIG16(CFE_EVS_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } },
    /* command ID #3 - Software Bus HK Request       */
  { { CFE_MAKE_BIG16(CFE_SB_SEND_HK_MID),   CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } },
    /* command ID #4 - Time Services HK Request      */
  { { CFE_MAKE_BIG16(CFE_TIME_SEND_HK_MID), CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } },
    /* command ID #5 - Table Services HK Request     */
  { { CFE_MAKE_BIG16(CFE_TBL_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } },

  /*
  **  CFS housekeeping request messages
  */
    /* command ID #6 - Checksum HK Request           */
/*{ { CFE_MAKE_BIG16(CS_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #7 - Data Store HK Request         */
/*{ { CFE_MAKE_BIG16(DS_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #8 - File Manager HK Request       */
/*{ { CFE_MAKE_BIG16(FM_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #9 - Housekeeping HK Request       */
/*{ { CFE_MAKE_BIG16(HK_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },

    /* command ID #10 - Health & Safety HK Request   */
/*{ { CFE_MAKE_BIG16(HS_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #11 - Limit Checker HK Request     */
/*{ { CFE_MAKE_BIG16(LC_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #12 - Memory Dwell HK Request      */
/*{ { CFE_MAKE_BIG16(MD_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #13 - Memory Manager HK Request    */
/*{ { CFE_MAKE_BIG16(MM_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #14 - Stored Command HK Request    */
/*{ { CFE_MAKE_BIG16(SC_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #15 - Scheduler HK Request         */
  { { CFE_MAKE_BIG16(SCH_SEND_HK_MID), CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } },

  /*
  **  CFS routine messages
  */
    /* command ID #16 - HK Send Combined Housekeeping Msg #1 */
/*{ { CFE_MAKE_BIG16(HK_SEND_COMBINED_PKT_MID), CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0003), 0x0000, HK_COMBINED_PKT1_MID } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #17 - HK Send Combined Housekeeping Msg #2 */
/*{ { CFE_MAKE_BIG16(HK_SEND_COMBINED_PKT_MID), CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0003), 0x0000, HK_COMBINED_PKT2_MID } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #18 - HK Send Combined Housekeeping Msg #3 */
/*{ { CFE_MAKE_BIG16(HK_SEND_COMBINED_PKT_MID), CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0003), 0x0000, HK_COMBINED_PKT3_MID } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #19 - HK Send Combined Housekeeping Msg #4 */
/*{ { CFE_MAKE_BIG16(HK_SEND_COMBINED_PKT_MID), CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0003), 0x0000, HK_COMBINED_PKT4_MID } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #20 - CS Background Cycle               */
/*{ { CFE_MAKE_BIG16(CS_BACKGROUND_CYCLE_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #21 - SC 1 Hz Wakeup                    */
/*{ { CFE_MAKE_BIG16(SC_1HZ_WAKEUP_MID),        CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #22 - LC Sample Action Points           */
/*{ { CFE_MAKE_BIG16(LC_SAMPLE_AP_MID),         CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0005), 0x0000, LC_ALL_ACTIONPOINTS, 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #23 - DS 1 HZ Wakeup                    */
/*{ { CFE_MAKE_BIG16(DS_1HZ_WAKEUP_MID),        CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #24 - MD Wakeup                         */
/*{ { CFE_MAKE_BIG16(MD_WAKEUP_MID),            CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #25 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #26 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #27 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #28 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #29 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },

  /*
  **  Mission Defined Messages
  */
    /* command ID #30 - Command Ingest HK Request Example */
/*{ { CFE_MAKE_BIG16(CI_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #31 - Telemetry Output HK Request Example */
/*{ { CFE_MAKE_BIG16(TO_SEND_HK_MID),  CFE_MAKE_BIG16(0xC000), CFE_MAKE_BIG16(0x0001), 0x0000 } }, */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #32 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #33 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #34 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },  
    /* command ID #35 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #36 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #37 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #38 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #39 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
  
    /* command ID #40 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #41 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #42 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #43 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #44 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },  
    /* command ID #45 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #46 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #47 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #48 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #49 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },

    /* command ID #50 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #51 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #52 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #53 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #54 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },  
    /* command ID #55 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #56 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #57 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #58 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #59 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },

    /* command ID #60 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #61 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #62 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #63 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #64 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },  
    /* command ID #65 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #66 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #67 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #68 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #69 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },

    /* command ID #70 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #71 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #72 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #73 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #74 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #75 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #76 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #77 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #78 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #79 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
  
    /* command ID #80 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #81 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #82 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #83 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #84 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #85 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #86 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #87 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #88 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #89 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
  
    /* command ID #90 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #91 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #92 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #93 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #94 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #95 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #96 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #97 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #98 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #99 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
  
    /* command ID #100 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #101 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #102 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #103 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #104 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #105 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #106 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #107 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #108 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #109 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
  
    /* command ID #110 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #111 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #112 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #113 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #114 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #115 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #116 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #117 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #118 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #119 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
  
    /* command ID #120 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #121 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #122 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #123 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #124 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #125 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #126 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } },
    /* command ID #127 */
  { { CFE_MAKE_BIG16(SCH_UNUSED_MID) } }

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

