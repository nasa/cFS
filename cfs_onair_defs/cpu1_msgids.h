/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/**
 * @file
 *
 * Purpose:
 *   This header file contains the Message Id's for messages used by the
 *   cFE core.
 *
 * Author:   R.McGraw/SSI
 *
 * Notes:
 *   This file should not contain messages defined by cFE external
 *   applications.
 *
 */

#ifndef CPU1_MSGIDS_H
#define CPU1_MSGIDS_H

/*
** Includes
*/

#include "cfe_mission_cfg.h"

/**
 * \brief Platform command message ID base offset
 *
 * Example mechanism for setting default command bits and deconflicting MIDs across multiple
 * platforms in a mission.  For any sufficiently complex mission this method is
 * typically replaced by a centralized message ID management scheme.
 *
 * 0x1800 - Nominal value for default message ID implementation (V1). This sets the command
 *          field and the secondary header present field.  Typical V1 command MID range
 *          is 0x1800-1FFF.  Additional cpus can deconflict message IDs by incrementing
 *          this value to provide sub-allocations (0x1900 for example).
 * 0x0080 - Command bit for MISSION_MSGID_V2 message ID implementation (V2).  Although
 *          this can be used for the value below due to the relatively small set
 *          of MIDs in the framework it will not scale so an alternative
 *          method of deconfliction is recommended.
 */
#define CFE_PLATFORM_CMD_MID_BASE 0x1800

/**
 * \brief Platform telemetry message ID base offset
 *
 * 0x0800 - Nominal for message ID V1
 * 0x0000 - Potential value for MISSION_MSGID_V2, but limited to a range of
 *          0x0000-0x007F since the command bit is 0x0080.  Alternative
 *          method of deconfliction is recommended.
 *
 * See #CFE_PLATFORM_CMD_MID_BASE for more information
 */
#define CFE_PLATFORM_TLM_MID_BASE 0x0800

/**
 * \brief "Global" command message ID base offset
 *
 * 0x1860 - Nominal value for message ID V1
 * 0x00E0 - Potential value for MISSION_MSGID_V2, note command bit is 0x0080.
 *          Works in limited cases only, alternative method of deconfliction
 *          is recommended.
 * See #CFE_PLATFORM_CMD_MID_BASE for more information
 */
#define CFE_PLATFORM_CMD_MID_BASE_GLOB 0x1860

/*
** cFE Command Message Id's
*/
#define CFE_EVS_CMD_MID  CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_EVS_CMD_MSG  /* 0x1801 */
#define CFE_TEST_CMD_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TEST_CMD_MSG /* 0x1802 */
#define CFE_SB_CMD_MID   CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_SB_CMD_MSG   /* 0x1803 */
#define CFE_TBL_CMD_MID  CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TBL_CMD_MSG  /* 0x1804 */
#define CFE_TIME_CMD_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TIME_CMD_MSG /* 0x1805 */
#define CFE_ES_CMD_MID   CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_ES_CMD_MSG   /* 0x1806 */

#define CFE_ES_SEND_HK_MID  CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_ES_SEND_HK_MSG  /* 0x1808 */
#define CFE_EVS_SEND_HK_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_EVS_SEND_HK_MSG /* 0x1809 */
/* Message ID 0x180A is available  */
#define CFE_SB_SEND_HK_MID   CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_SB_SEND_HK_MSG   /* 0x180B */
#define CFE_TBL_SEND_HK_MID  CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TBL_SEND_HK_MSG  /* 0x180C */
#define CFE_TIME_SEND_HK_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TIME_SEND_HK_MSG /* 0x180D */

#define CFE_SB_SUB_RPT_CTRL_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_SB_SUB_RPT_CTRL_MSG /* 0x180E */

#define CFE_TIME_TONE_CMD_MID CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TIME_TONE_CMD_MSG /* 0x1810 */
#define CFE_TIME_1HZ_CMD_MID  CFE_PLATFORM_CMD_MID_BASE + CFE_MISSION_TIME_1HZ_CMD_MSG  /* 0x1811 */

/*
** cFE Global Command Message Id's
*/
#define CFE_TIME_DATA_CMD_MID CFE_PLATFORM_CMD_MID_BASE_GLOB + CFE_MISSION_TIME_DATA_CMD_MSG /* 0x1860 */
#define CFE_TIME_SEND_CMD_MID CFE_PLATFORM_CMD_MID_BASE_GLOB + CFE_MISSION_TIME_SEND_CMD_MSG /* 0x1862 */

/*
** CFE Telemetry Message Id's
*/
#define CFE_ES_HK_TLM_MID           CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_ES_HK_TLM_MSG           /* 0x0800 */
#define CFE_EVS_HK_TLM_MID          CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_EVS_HK_TLM_MSG          /* 0x0801 */
#define CFE_TEST_HK_TLM_MID         CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_TEST_HK_TLM_MSG         /* 0x0802 */
#define CFE_SB_HK_TLM_MID           CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_SB_HK_TLM_MSG           /* 0x0803 */
#define CFE_TBL_HK_TLM_MID          CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_TBL_HK_TLM_MSG          /* 0x0804 */
#define CFE_TIME_HK_TLM_MID         CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_TIME_HK_TLM_MSG         /* 0x0805 */
#define CFE_TIME_DIAG_TLM_MID       CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_TIME_DIAG_TLM_MSG       /* 0x0806 */
#define CFE_EVS_LONG_EVENT_MSG_MID  CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_EVS_LONG_EVENT_MSG_MSG  /* 0x0808 */
#define CFE_EVS_SHORT_EVENT_MSG_MID CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_EVS_SHORT_EVENT_MSG_MSG /* 0x0809 */
#define CFE_SB_STATS_TLM_MID        CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_SB_STATS_TLM_MSG        /* 0x080A */
#define CFE_ES_APP_TLM_MID          CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_ES_APP_TLM_MSG          /* 0x080B */
#define CFE_TBL_REG_TLM_MID         CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_TBL_REG_TLM_MSG         /* 0x080C */
#define CFE_SB_ALLSUBS_TLM_MID      CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_SB_ALLSUBS_TLM_MSG      /* 0x080D */
#define CFE_SB_ONESUB_TLM_MID       CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_SB_ONESUB_TLM_MSG       /* 0x080E */
#define CFE_ES_MEMSTATS_TLM_MID     CFE_PLATFORM_TLM_MID_BASE + CFE_MISSION_ES_MEMSTATS_TLM_MSG     /* 0x0810 */

#endif /* CPU1_MSGIDS_H */
