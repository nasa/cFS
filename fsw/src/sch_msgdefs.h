/*
** $Id: sch_msgdefs.h 1.4 2015/03/01 14:01:35EST sstrege Exp  $
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
** Subsystem: Scheduler (SCH) packet macro definitions
**
** Author: David Kobe
**
** Notes:
**   These Macro definitions have been put in this file (instead of 
**   sch_msg.h) so this file can be included directly into ASIST build 
**   test scripts. ASIST RDL files can accept C language #defines but 
**   can't handle type definitions. As a result: DO NOT PUT ANY
**   TYPEDEFS OR STRUCTURE DEFINITIONS IN THIS FILE! 
**   ADD THEM TO sch_msg.h IF NEEDED! 
**
**
** $Log: sch_msgdefs.h  $
** Revision 1.4 2015/03/01 14:01:35EST sstrege 
** Added copyright information
** Revision 1.3 2011/06/14 15:11:32EDT aschoeni 
** Cleaned up comments on reset command
** Revision 1.2 2011/06/02 16:19:03EDT aschoeni 
** Clarified MF_SOURCE as MAJOR_FS (M could also be minor)
** Revision 1.1 2008/10/16 15:08:38EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
*/

#ifndef _sch_msgdefs_
#define _sch_msgdefs_

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "sch_platform_cfg.h"


/*************************************************************************
**
** SCH application definitions
**
**************************************************************************/

/*
** Table value definitions
*/
/** \name Scheduler App Entry States */
/**  \{ */
#define SCH_UNUSED              0
#define SCH_ENABLED             1
#define SCH_DISABLED            2
/** \} */

/*
** Table Entry Activity Types
*/
/** \name Scheduler App Entry Types */
/**  \{ */
#define SCH_ACTIVITY_NONE       0
#define SCH_ACTIVITY_SEND_MSG   1
/** \} */

/*
** Synchronized to Mission Elapsed Time States
*/
/** \name Synchronized to MET States */
/**  \{ */
#define SCH_NOT_SYNCHRONIZED          0
#define SCH_MINOR_SYNCHRONIZED        1
#define SCH_PENDING_MAJOR_SYNCH       2
#define SCH_MAJOR_SYNCHRONIZED        4
/** \} */

/*
** Major Frame Signal Source Identifiers
*/
/** \name Major Frame Signal Source Identifiers */
/**  \{ */
#define SCH_MAJOR_FS_NONE               0
#define SCH_MAJOR_FS_CFE_TIME           1
#define SCH_MAJOR_FS_MINOR_FRAME_TIMER  2
/** \} */

/*
** Schedule table group data field masks
*/
/** \name Scheduler App Entry Group Bit Masks */
/**  \{ */
#define SCH_GROUP_NUMBER_BIT_MASK  0xFF000000
#define SCH_MULTI_GROUP_BIT_MASK   0x00FFFFFF
/** \} */

/*
** Message table definitions
*/
/** \name Scheduler App Miscellaneous Limits */
/**  \{ */
#define SCH_MIN_MSG_WORDS       (CFE_SB_CMD_HDR_SIZE / 2)
#define SCH_TABLE_ENTRIES       (SCH_TOTAL_SLOTS * SCH_ENTRIES_PER_SLOT)
/** \} */

#define SCH_NUM_STATUS_BYTES_REQD    (((SCH_TABLE_ENTRIES+7)/8)*2)
                                     /**< \brief Num Bytes req'd to store entry states in diag pkt 
                                              (2-bits per entry rounded up to nearest 16-bit word) */

/*
**  SCH command packet command codes 
** (SCH_GND_CMD_MID and SCH_SC_CMD_MID)
*/
/** \name CFS Scheduler Command Codes */
/** \{ */

/** \schcmd Scheduler No-Op
**
**  \par Description
**       This command performs no other function than to increment the 
**       command execution counter. The command may be used to verify 
**       general aliveness of the Scheduler Application.
**
**  \schcmdmnemonic \SCH_NOOP
**
**  \par Command Structure
**       #SCH_NoArgsCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_NOOP_CMD_EID informational event message will 
**         be generated
**
**  \par Error Conditions
**       There are no error conditions for this command. If the Scheduler 
**       receives the command, the event is sent (although it 
**       may be filtered by EVS) and the counter is incremented 
**       unconditionally.
**
**  \par Criticality
**       None
**
**  \sa 
*/
#define SCH_NOOP_CC             0   /* no-op command */

/** \schcmd Scheduler Reset Counters
**
**  \par Description
**       This command resets the following counters within the  
**       Scheduler housekeeping telemetry:
**       - Command Execution Counter (\SCH_CMDPC)
**       - Command Error Counter (\SCH_CMDEC)
**       - Schedule Activities Success Counter (\SCH_ACTSUCCESSCTR)
**       - Schedule Activities Failure Counter (\SCH_ACTFAILURECTR)
**       - Schedule Slots Processed Counter (\SCH_SLOTPROCCTR)
**       - Schedule Skipping Slots Counter (\SCH_SKIPSLOTCTR)
**       - Multiple Schedule Slots Processed Counter (\SCH_MULTSLOTCTR)
**       - Awoke in Same Slot Counter (\SCH_SAMESLOTCTR)
**       - Corrupted Table Data Counter (\SCH_BADTBLDATACTR)
**       - Table Loads Successfully Verified Counter (\SCH_TBLPASSVERIFYCTR)
**       - Table Loads Unsuccessfully Verified Counter (\SCH_TBLFAILVERIFYCTR)
**       - Valid Major Frames Received Counter (\SCH_VALIDMFCTR)
**       - Missed Major Frames Received Counter (\SCH_MISSMFCTR)
**       - Unexpected Major Frames Received Counter (\SCH_UNEXPCTDMFCTR)
** 
**  \schcmdmnemonic \SCH_RESETCTRS
**
**  \par Command Structure
**       #SCH_NoArgsCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_RESET_CMD_EID debug event message will 
**         be generated
**
**  \par Error Conditions
**       There are no error conditions for this command. If the Scheduler 
**       receives the command, the event is sent (although it 
**       may be filtered by EVS) and the counter is incremented 
**       unconditionally.
**
**  \par Criticality
**       This command is not inherently dangerous.  However, it is 
**       possible for ground systems and on-board safing procedures 
**       to be designed such that they react to changes in the counter 
**       values that are reset by this command.
**
**  \sa 
*/
#define SCH_RESET_CC            1   /* Reset HK counters */

/** \schcmd Enable Schedule Table Entry
**
**  \par Description
**       This command enables a single activity in the Schedule Definition Table.
**
**  \schcmdmnemonic \SCH_ENABLEENTRY
**
**  \par Command Structure
**       #SCH_EntryCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_ENABLE_CMD_EID debug event message will 
**         be generated
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid Slot (Minor Frame) specified in the command
**       - Invalid Entry ID specified in the command
**       - Current state is neither Enabled or Disabled (i.e. - it is corrupted or empty)
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \SCH_CMDEC  - command error counter will increment
**       - Command specific error event messages are issued for all error cases
**
**  \par Criticality
**       Criticality is dependent entirely on the contents 
**       of the Schedule Definition Table entry being enabled
**
**  \sa #SCH_DISABLE_CC, #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC
*/
#define SCH_ENABLE_CC           2   /* Enable Schedule Table Entry */

/** \schcmd Disable Schedule Entry
**
**  \par Description
**       This command disables a single activity in the Schedule Definition Table.
**
**  \schcmdmnemonic \SCH_DISABLEENTRY
**
**  \par Command Structure
**       #SCH_EntryCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_DISABLE_CMD_EID debug event message will 
**         be generated
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid Slot (Minor Frame) specified in the command
**       - Invalid Entry ID specified in the command
**       - Current state is neither Enabled or Disabled (i.e. - it is corrupted or empty)
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \SCH_CMDEC - command error counter will increment
**       - Command specific error event messages are issued for all error cases
**
**  \par Criticality
**       Criticality is dependent entirely on the contents 
**       of the Schedule Definition Table entry being disabled
**
**  \sa #SCH_ENABLE_CC, #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC
*/
#define SCH_DISABLE_CC          3   /* disable schedule table entry */

/** \schcmd Enable Group and/or Multi-Groups of Schedule Activities
**
**  \par Description
**       This command enables a single group and/or a collection of
**       Multi-Group Activities in the Schedule Definition Table.
**
**  \schcmdmnemonic \SCH_ENABLEGROUP
**
**  \par Command Structure
**       #SCH_GroupCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_ENA_GRP_CMD_EID debug event message will 
**         be generated
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - No Group nor any Multi-Group was specified in the command
**       - No activities with the specified Group and Multi-Group was located
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \SCH_CMDEC - command error counter will increment
**       - Command specific error event messages are issued for all error cases
**
**  \par Criticality
**       Criticality is dependent entirely on the contents 
**       of the Schedule Definition Table entries being enabled
**
**  \sa #SCH_ENABLE_CC, SCH_DISABLE_CC, #SCH_DISABLE_GROUP_CC
*/
#define SCH_ENABLE_GROUP_CC     4   /* enable group of entries */

/** \schcmd Disable Group and/or Multi-Groups of Schedule Activities
**
**  \par Description
**       This command disables a single group and/or a collection of
**       Multi-Group Activities in the Schedule Definition Table.
**
**  \schcmdmnemonic \SCH_DISABLEGROUP
**
**  \par Command Structure
**       #SCH_GroupCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_DIS_GRP_CMD_EID debug event message will 
**         be generated
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - No Group nor any Multi-Group was specified in the command
**       - No activities with the specified Group and Multi-Group was located
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \SCH_CMDEC - command error counter will increment
**       - Command specific error event messages are issued for all error cases
**
**  \par Criticality
**       Criticality is dependent entirely on the contents 
**       of the Schedule Definition Table entries being disabled
**
**  \sa #SCH_ENABLE_CC, SCH_DISABLE_CC, #SCH_ENABLE_GROUP_CC
*/
#define SCH_DISABLE_GROUP_CC    5   /* disable group of entries */

/** \schcmd Enable Major Frame Synchronization
**
**  \par Description
**       This command allows the operator to enable processing and
**       synchronization of the Major Frame Signal to the processing
**       of the Schedule Definition Table.
**
**  \schcmdmnemonic \SCH_ENABLESYNC
**
**  \par Command Structure
**       #SCH_NoArgsCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_ENA_SYNC_CMD_EID debug event message will 
**         be generated
**
**  \par Error Conditions
**       There are no error conditions for this command. If the Scheduler 
**       receives the command, the event is sent (although it 
**       may be filtered by EVS) and the counter is incremented 
**       unconditionally.
**
**  \par Criticality
**       May cause a number of minor frames to be skipped when the major frame
**       synchronization signal is received and acted upon.
**
**  \sa 
*/
#define SCH_ENABLE_SYNC_CC      6   /* enable major frame synchronization */

/** \schcmd Telemeter Scheduler Diagnostic Packet
**
**  \par Description
**       This command generates and sends the Scheduler Application's
**       Diagnostic Telemetry packet.
**
**  \schcmdmnemonic \SCH_SENDDIAG
**
**  \par Command Structure
**       #SCH_NoArgsCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with the 
**       following telemetry:
**       - \b \c \SCH_CMDPC - command execution 
**         counter will increment
**       - The #SCH_SEND_DIAG_CMD_EID debug event message will 
**         be generated
**
**  \par Error Conditions
**       There are no error conditions for this command. If the Scheduler 
**       receives the command, the event is sent (although it 
**       may be filtered by EVS) and the counter is incremented 
**       unconditionally.
**
**  \par Criticality
**       None
**
**  \sa 
*/
#define SCH_SEND_DIAG_TLM_CC    7   /* sends diagnostic message packet */
/** \} */


#endif /* _sch_msgdefs_ */

/************************/
/*  End of File Comment */
/************************/

