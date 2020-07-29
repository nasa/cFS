/*==============================================================================
** File Name: to_config.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Title:     TO table definition
**
** $Author: $
** $Revision: $
** $Date:  $
**
** Purpose:   To provide the table for default data config.
**
** Functions Contained:
**    None
**
**
** Limitations, Assumptions, External Events, and Notes:
**  1.   None
**
**
**==============================================================================
*/

/*
#ifndef _TO_CONFIG_
#define _TO_CONFIG_


#ifdef   __cplusplus
extern "C" {
#endif
*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "to_platform_cfg.h"
#include "to_mission_cfg.h"
#include "to_app.h"
#include "to_tbldefs.h"
#include "to_grpids.h"

#include "cfe_msgids.h"

#include "ci_msgids.h"


/*
** Local Defines
*/


/*
** Local Structure Declarations
*/

static CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "to_ConfigTable", "TO.to_config", "TO config table",
    "to_config_2.tbl", sizeof(TO_ConfigTable_t)
};

/*
** Default TO iLoad table data
*/

TO_ConfigTable_t to_ConfigTable =
{
   {
       /* 0 - 9 */
       {TO_HK_TLM_MID,                {0,0},  1,   0x0003,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {CI_HK_TLM_MID,                {0,0},  1,   0x0003,     TO_GROUP_APP | TO_MGROUP_ONE, 0,1},
       {CFE_TBL_HK_TLM_MID,           {0,0},  1,   0x0003,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {CFE_EVS_EVENT_MSG_MID,        {0,0},  32,  0x0003,     TO_GROUP_CFE | TO_MGROUP_ONE, 0,1},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 40 - 49 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 50 - 59 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       
       /* 30 - 39 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 40 - 49 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 50 - 59 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 60 - 69 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 70 - 79 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 80 - 89 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},

       /* 90 - 99 */
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0},
       {TO_UNUSED_ENTRY,              {0,0},  0,   0x0000,     TO_GROUP_NONE,            0,0}
    }
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

/*
#ifdef   __cplusplus
}
#endif

#endif
*/

/* _TO_CONFIG_ */
