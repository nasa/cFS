/*
** $Id: sch_def_schtbl1.c 1.2 2015/03/01 14:01:47EST sstrege Exp  $
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
** Purpose: Scheduler (SCH) default schedule table data
**
** Author: 
**
** Notes:
**
** $Log: sch_def_schtbl1.c  $
** Revision 1.2 2015/03/01 14:01:47EST sstrege 
** Added copyright information
** Revision 1.1 2011/06/30 21:00:59EDT aschoeni 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/unit_test/cf/apps/project.pj
** Revision 1.1 2009/03/27 00:32:26EDT dkobe 
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
#include "sch_msgdefs.h"
#include "sch_tbldefs.h"

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/*
** Schedule Table "group" definitions
*/
#define SCH_GROUP_NONE         (0)

/* Define highest level multi-groups */
#define SCH_GROUP_CDH         (0x000001)                        /* All C&DH Messages        */
#define SCH_GROUP_GNC         (0x000002)                        /* All GNC  Messages        */

/* Define sub multi-groups           */
#define SCH_GROUP_CFS_HK      (  (0x000010) | SCH_GROUP_CDH)    /* CFS HK Messages          */
#define SCH_GROUP_CFE_HK      (  (0x000020) | SCH_GROUP_CDH)    /* cFE HK Messages          */
#define SCH_GROUP_GNC_HK      (  (0x000040) | SCH_GROUP_GNC)    /* GNC HK Messages          */

#define SCH_GROUP_

/* Define groups for messages that appear multiple times in Schedule */
#define SCH_GROUP_MD_WAKEUP   ((0x01000000) | SCH_GROUP_CDH)    /* MD Wakeup (aka Group #1) */


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
    "SCH_DefaultScheduleTable", "SCH.SCHED_DEF", "SCH schedule table",
    "sch_def_schtbl1.tbl", (sizeof(SCH_ScheduleEntry_t) * SCH_TABLE_ENTRIES)
};

/*
** Default schedule table data
*/
SCH_ScheduleEntry_t SCH_DefaultScheduleTable[SCH_TABLE_ENTRIES] =
{

/*
** Structure definition...
**
**    uint8    EnableState  -- SCH_UNUSED, SCH_ENABLED, SCH_DISABLED
**    uint8    Type         -- 0 or SCH_ACTIVITY_SEND_MSG
**    uint16   Frequency    -- how many seconds between Activity execution
**    uint16   Remainder    -- seconds offset to perform Activity
**    uint16   MessageIndex -- Message Index into Message Definition table
**    uint32   GroupData    -- Group and Multi-Group membership definitions
*/

  /* slot #0 */
  {  SCH_ENABLED, SCH_ACTIVITY_SEND_MSG,  1,  0, 0, 0x00000001}, /* Slot #0, Entry #0 - Enabled - Message #0 every second (Multi-Group #0001B) */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_ENABLED, SCH_ACTIVITY_SEND_MSG,  5,  3, 1, 0x00000004}, /* Slot #0, Entry #4 - Enabled - Message #1 every 5 secs start @ seconds = 3  (Multi-Group #0100B) */
  
  /* slot #1 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
    
  /* slot #2 */
  {  SCH_DISABLED, SCH_ACTIVITY_SEND_MSG,  1,  0, 2,  0x02000000}, /* Slot #2, Entry #0 - Disabled - Message #2 every 2 secs (Group #2) */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #3 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_DISABLED, SCH_ACTIVITY_SEND_MSG,  2,  0, 3,  0x01000000}, /* Slot #3, Entry #1 - Disabled - Message #3 every 2 secs (Group #1) */                                       
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #4 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #5 */
  {  SCH_ENABLED, SCH_ACTIVITY_SEND_MSG,  1,  0, 4, 0x00000005}, /* Slot #5, Entry #0 - Enabled - Message #4 every 1 secs (Multi-Group #0101B) */ 
  {  SCH_ENABLED, SCH_ACTIVITY_SEND_MSG,  5,  1, 5, 0x00000002}, /* Slot #5, Entry #1 - Enabled - Message #5 every 1 secs (Multi-Group #0010B) */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #6 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #7 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                    
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #8 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #9 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #10 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #11 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #12 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #13 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #14 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #15 */
  {  SCH_DISABLED, SCH_ACTIVITY_SEND_MSG,  10,  0, 6,  0x02000005}, /* Slot #15, Entry #0 - Disabled - Message #6 every 10 secs (Group #2, Multi-Group #0101B) */
  {  SCH_DISABLED, SCH_ACTIVITY_SEND_MSG,  7,  0, 7,  0x01000002}, /* Slot #15, Entry #1 - Disabled - Message #7 every 7 secs (Group #1, Multi-Group #0010B) */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #16 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #17 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #18 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #19 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #20 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #21 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #22 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #23 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #24 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #25 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #26 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #27 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #28 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #29 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #30 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #31 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #32 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #33 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #34 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #35 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #36 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #37 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #38 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #39 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #40 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #41 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #42 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #43 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #44 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #45 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #46 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #47 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #48 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #49 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #50 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   SCH_ACTIVITY_SEND_MSG,      0,  0, 0,  SCH_GROUP_NONE}, /* Slot #50, Entry #1 - Garbage Data */                                       
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #51 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #52 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #53 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #54 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #55 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #56 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #57 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #58 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #59 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #60 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #61 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #62 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #63 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #64 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #65 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #66 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #67 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #68 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #69 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #70 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #71 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #72 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
 
  /* slot #73 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #74 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
 
  /* slot #75 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #76 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #77 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #78 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #79 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #80 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #81 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #82 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #83 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #84 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #85 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #86 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #87 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #88 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #89 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #90 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #91 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #92 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #93 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #94 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #95 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #96 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #97 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #98 */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        

  /* slot #99 - Left Empty to allow Scheduler to Easily Resynchronize with 1 Hz */
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE},                                        
  {  SCH_UNUSED,   0,      0,  0, 0,  SCH_GROUP_NONE}                                       
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

