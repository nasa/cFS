/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *   Limit Checker (LC) alternate ADT (actionpoint definition table) for LC COSMOS test
 *
 * @note
 *   This file provides an alternate ADT table to be loaded by the LC COSMOS test
 *
 *   LC will append a trailer string to the end of the text
 *   specified in the "EventText" field with additional information.
 *   See lc_action.h for the format.
 */

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "cfe_evs_extern_typedefs.h"
#include "lc_platform_cfg.h"
#include "lc_msgdefs.h"
#include "lc_tbl.h"
#include "lc_eventids.h"

/*************************************************************************
** Examples
** (note that comment delimiters have been changed to '**')
**
** Actions that trigger off a single watchpoint:
** (see lc_def_wdt.c for companion watchpoint definitions)
**
**    ** #100 **
**    {
**        .DefaultState        = LC_APSTATE_DISABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_DIVINER_SAFE_MODE,
**        .MaxFailsBeforeRTS   = 60,
**        .EventType           = CFE_EVS_EventType_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 100,
**        .EventText           = { "Diviner: low input volt (1)" },
**        .RPNEquation         = { ** (WP_112) **
**                                 112,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
**    ** #101 **
**    {
**        .DefaultState        = LC_APSTATE_DISABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_DIVINER_SAFE_MODE,
**        .MaxFailsBeforeRTS   = 3,
**        .EventType           = CFE_EVS_EventType_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 101,
**        .EventText           = { "Diviner: low input volt (2)" },
**        .RPNEquation         = { ** (WP_113) **
**                                 113,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
** Examples of more complex Reverse Polish Notation expressions:
**
**    ** #43 **
**    {
**        .DefaultState        = LC_APSTATE_ENABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_ACS_EXIT_THRUSTER_MODE,
**        .MaxFailsBeforeRTS   = 10,
**        .EventType           = CFE_EVS_EventType_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 43,
**        .EventText           = { "GNC: delta-V sys attitude" },
**        .RPNEquation         = { ** (WP_26 && !WP_61 && !WP_64 && !WP_45 && WP_46 && WP_47) **
**                                 26, 61,
**                                 LC_RPN_NOT,
**                                 LC_RPN_AND,
**                                 64,
**                                 LC_RPN_NOT,
**                                 LC_RPN_AND,
**                                 45,
**                                 LC_RPN_NOT,
**                                 LC_RPN_AND,
**                                 46,
**                                 LC_RPN_AND,
**                                 47,
**                                 LC_RPN_AND,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
**    ** #47 **
**    {
**        .DefaultState        = LC_APSTATE_ENABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_ACS_POWER_OFF_ALL_RW,
**        .MaxFailsBeforeRTS   = 2,
**        .EventType           = CFE_EVS_EventType_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 47,
**        .EventText           = { "GNC: wheel on, attached" },
**        .RPNEquation         = { ** (!WP_80 && (WP_48 || WP_49 || WP_50 || WP_51))) **
**                                 80,
**                                 LC_RPN_NOT,
**                                 48, 49, 50, 51,
**                                 LC_RPN_OR,
**                                 LC_RPN_OR,
**                                 LC_RPN_OR,
**                                 LC_RPN_AND,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
**    ** #142 **
**    {
**        .DefaultState        = LC_APSTATE_DISABLED,
**        .MaxPassiveEvents    = 2,
**        .MaxPassFailEvents   = 2,
**        .MaxFailPassEvents   = 2,
**        .RTSId               = RTS_ID_LEND_POWER_OFF,
**        .MaxFailsBeforeRTS   = 60,
**        .EventType           = CFE_EVS_EventType_INFORMATION,
**        .EventID             = LC_BASE_AP_EID + 142,
**        .EventText           = { "LEND: comp over temp #1" },
**        .RPNEquation         = { ** (WP_142 && WP_143) || (WP_144 && WP_145) || (WP_146 && WP_147) **
**                                 142, 143,
**                                 LC_RPN_AND,
**                                 144, 145,
**                                 LC_RPN_AND,
**                                 146, 147,
**                                 LC_RPN_AND,
**                                 LC_RPN_OR,
**                                 LC_RPN_OR,
**                                 LC_RPN_EQUAL
**                               }
**    },
**
*************************************************************************/

/*************************************************************************
** Exported Data
*************************************************************************/
/*
** Table file header
*/
CFE_TBL_FileDef_t CFE_TBL_FileDef = { "LC_ADT",
                                      LC_APP_NAME "." LC_ADT_TABLENAME,
                                      "LC actionpoint definition table",
                                      "lc_def_adt-test.tbl",
                                      (sizeof(LC_ADTEntry_t) * LC_MAX_ACTIONPOINTS) };

/*
** Alternate actionpoint definition table (ADT) data
*/
LC_ADTEntry_t LC_ADT[LC_MAX_ACTIONPOINTS] = {
    /* #0 */
    {.DefaultState      = LC_APSTATE_DISABLED,
     .MaxPassiveEvents  = 3,
     .MaxPassFailEvents = 3,
     .MaxFailPassEvents = 3,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 3,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {"Placeholder"},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #1 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #2 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #3 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #4 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #5 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #6 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #7 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #8 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #9 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #10 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #11 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #12 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #13 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #14 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #15 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #16 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #17 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #18 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #19 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #20 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #21 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #22 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #23 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #24 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #25 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #26 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #27 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #28 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #29 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #30 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #31 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #32 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #33 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #34 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #35 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #36 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #37 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #38 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #39 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #40 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #41 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #42 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #43 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #44 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #45 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #46 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #47 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #48 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #49 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #50 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #51 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #52 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #53 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #54 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #55 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #56 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #57 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #58 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #59 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #60 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #61 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #62 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #63 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #64 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #65 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #66 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #67 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #68 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #69 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #70 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #71 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #72 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #73 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #74 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #75 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #76 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #77 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #78 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #79 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #80 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #81 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #82 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #83 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #84 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #85 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #86 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #87 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #88 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #89 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #90 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #91 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #92 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #93 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #94 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #95 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #96 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #97 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #98 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #99 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #100 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #101 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #102 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #103 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #104 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #105 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #106 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #107 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #108 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #109 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #110 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #111 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #112 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #113 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #114 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #115 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #116 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #117 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #118 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #119 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #120 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #121 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #122 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #123 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #124 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #125 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #126 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #127 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #128 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #129 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #130 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #131 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #132 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #133 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #134 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #135 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #136 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #137 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #138 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #139 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #140 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #141 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #142 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #143 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #144 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #145 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #146 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #147 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #148 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #149 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #150 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #151 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #152 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #153 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #154 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #155 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #156 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #157 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #158 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #159 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #160 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #161 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #162 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #163 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #164 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #165 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #166 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #167 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #168 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #169 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #170 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #171 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #172 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #173 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #174 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation =
         {/* (WP_0) */
          0, LC_RPN_EQUAL}},

    /* #175 (unused) */
    {.DefaultState      = LC_APSTATE_NOT_USED,
     .MaxPassiveEvents  = 0,
     .MaxPassFailEvents = 0,
     .MaxFailPassEvents = 0,
     .RTSId             = 0,
     .MaxFailsBeforeRTS = 0,
     .EventType         = CFE_EVS_EventType_INFORMATION,
     .EventID           = 0,
     .EventText         = {" "},
     .RPNEquation       = {              /* (WP_0) */
                     0, LC_RPN_EQUAL}}}; /* end LC_DefaultADT */
