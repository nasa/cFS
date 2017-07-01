/*
** $Id: sch_msg.h 1.5 2015/03/01 14:01:43EST sstrege Exp  $
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
** Subsystem: Scheduler (SCH) packet definitions
**
** Author:
**
** Notes:
**
** $Log: sch_msg.h  $
** Revision 1.5 2015/03/01 14:01:43EST sstrege 
** Added copyright information
** Revision 1.4 2011/06/14 15:26:32EDT aschoeni 
** Reduced command counters to 8 bit
** Revision 1.3 2011/06/14 15:11:43EDT aschoeni 
** Cleaned up comments on reset command
** Revision 1.2 2009/03/27 00:33:24EDT dkobe 
** Corrected doxygen comments to improve User's Guide
** Revision 1.1 2008/10/16 15:08:37EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
*/

/*************************************************************************
**
** Ensure that "this" header is included only once
**
**************************************************************************/

#ifndef _sch_msg_
#define _sch_msg_

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "common_types.h"
#include "sch_msgdefs.h"

/*************************************************************************
**
** SCH command packet structure definitions
**
**************************************************************************/

/*
** SCH_SEND_HK_MID, SCH_NOOP_CC, SCH_RESET_CC, 
** SCH_ENABLE_SYNC_CC, SCH_SEND_DIAG_TLM_CC
*/
/** 
**  \brief No Arguments Command
**
**  For command details see #SCH_NOOP_CC, #SCH_RESET_CC, 
**  #SCH_ENABLE_SYNC_CC or #SCH_SEND_DIAG_TLM_CC
*/
typedef struct
{
    uint8             CmdHeader[CFE_SB_CMD_HDR_SIZE];  /**< \brief cFE Software Bus Command Message Header */

} SCH_NoArgsCmd_t;

/*
** SCH_ENABLE_CC, SCH_DISABLE_CC
*/
/** 
**  \brief Entry Enable/Disable Commands
**
**  For command details see #SCH_ENABLE_CC, #SCH_DISABLE_CC 
*/
typedef struct
{
    uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];          /**< \brief cFE Software Bus Command Message Header */

    uint16   SlotNumber;                              /**< \brief Slot Number of Activity whose state is to change */
                                                      /**< \details Valid Range is zero to (#SCH_TOTAL_SLOTS - 1) */
    uint16   EntryNumber;                             /**< \brief Entry Number of Activity whose state is to change
                                                           \details Valid Range is zero to (#SCH_ENTRIES_PER_SLOT - 1) */

} SCH_EntryCmd_t;

/*
** SCH_ENABLE_GROUP_CC, SCH_DISABLE_GROUP_CC
*/
/** 
**  \brief Entry Enable/Disable Commands
**
**  For command details see #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC 
*/
typedef struct
{
    uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];          /**< \brief cFE Software Bus Command Message Header */

    uint32   GroupData;                               /**< \brief Group and Multi-Group Identifiers
                                                           \details Most Significant Byte contains a Group ID of 1 to 255,
                                                           remaining 24 bits identify 24 Multi-Group Identifiers */
} SCH_GroupCmd_t;


/*************************************************************************
**
** SCH housekeeping telemetry packet definition
**
**************************************************************************/

/** 
**  \schtlm Scheduler HK Telemetry format
**/

typedef struct
{
    uint8    TlmHeader[CFE_SB_TLM_HDR_SIZE];          /**< \brief cFE Software Bus Telemetry Message Header */

    /*
    ** Command execution counters (ground commands)
    */
    uint8    CmdCounter;                              /**< \schtlmmnemonic \SCH_CMDPC
                                                           \brief Command Counter
                                                           \details \ref SCHHK_CmdCounter "Click for more" */
    uint8    ErrCounter;                              /**< \schtlmmnemonic \SCH_CMDEC
                                                           \brief Command Error Counter 
                                                           \details \ref SCHHK_ErrCounter "Click for more"  */
    uint8    SyncToMET;                               /**< \schtlmmnemonic \SCH_SYNCTOMET
                                                           \brief Status indicating whether slots are synched to MET 
                                                           \details \ref SCHHK_SyncToMET "Click for more"  */
    uint8    MajorFrameSource;                        /**< \schtlmmnemonic \SCH_MAJORFRAMESOURCE
                                                           \brief Major Frame Signal source identifier 
                                                           \details \ref SCHHK_MajorFrameSource "Click for more"  */

    /*
    ** Messages sent by schedule table processor
    */
    uint32   ScheduleActivitySuccessCount;            /**< \schtlmmnemonic \SCH_ACTSUCCESSCTR
                                                           \brief Number of successfully performed activities 
                                                           \details \ref SCHHK_ScheduleActivitySuccessCount "Click for more"  */
    uint32   ScheduleActivityFailureCount;            /**< \schtlmmnemonic \SCH_ACTFAILURECTR
                                                           \brief Number of unsuccessful activities attempted 
                                                           \details \ref SCHHK_ScheduleActivityFailureCount "Click for more"  */

    /*
    ** Total schedule table slots processed
    */
    uint32   SlotsProcessedCount;                     /**< \schtlmmnemonic \SCH_SLOTPROCCTR
                                                           \brief Total # of Schedule Slots (Minor Frames) Processed 
                                                           \details \ref SCHHK_SlotsProcessedCount "Click for more"  */

    /*
    ** The number of times that slots were skipped
    ** (not the number of slots that were skipped)
    */
    uint16   SkippedSlotsCount;                       /**< \schtlmmnemonic \SCH_SKIPSLOTCTR
                                                           \brief Number of times that slots were skipped
                                                           \details The number of times that a slot (minor frame) was skipped.
                                                           \ref SCHHK_SkippedSlotsCount "Click for more" 
                                                           \b Note: This is NOT the number of slots that were skipped */

    /*
    ** The number of times that multiple slots were processed
    ** (not the number of slots that were processed)
    */
    uint16   MultipleSlotsCount;                      /**< \schtlmmnemonic \SCH_MULTSLOTCTR
                                                           \brief Number of times that multiple slots processed 
                                                           \details The number of times that multiple slots (minor frames)
                                                           were processed in the same minor frame.  
                                                           \ref SCHHK_MultipleSlotsCount "Click for more"
                                                           \b Note: This is NOT the number of slots that were processed */

    /*
    ** The number of times that SH woke up in the same slot as last time
    */
    uint16   SameSlotCount;                           /**< \schtlmmnemonic \SCH_SAMESLOTCTR
                                                           \brief # of times SCH woke up in the same slot as last time 
                                                           \details \ref SCHHK_SameSlotCount "Click for more"  */

    /*
    ** The number of times that a table entry with bad data was processed
    ** (the entry previously passed validation but then somehow went bad)
    */
    uint16   BadTableDataCount;                       /**< \schtlmmnemonic \SCH_BADTBLDATACTR
                                                           \brief # of times corrupted table entries were processed 
                                                           \details \ref SCHHK_BadTableDataCount "Click for more"  */

    /*
    ** The number of tables verified prior to table load
    */
    uint16   TableVerifySuccessCount;                 /**< \schtlmmnemonic \SCH_TBLPASSVERIFYCTR
                                                           \brief # of times table loads successfully verified 
                                                           \details \ref SCHHK_TableVerifySuccessCount "Click for more"  */
    uint16   TableVerifyFailureCount;                 /**< \schtlmmnemonic \SCH_TBLFAILVERIFYCTR
                                                           \brief # of times table loads unsuccessfully verified 
                                                           \details \ref SCHHK_TableVerifyFailureCount "Click for more"  */
    uint32   TablePassCount;                          /**< \schtlmmnemonic \SCH_TBLPROCCTR
                                                           \brief # of times Schedule Table has been processed 
                                                           \details \ref SCHHK_TablePassCount "Click for more"  */
    uint32   ValidMajorFrameCount;                    /**< \schtlmmnemonic \SCH_VALIDMFCTR
                                                           \brief # of valid Major Frame tones received 
                                                           \details \ref SCHHK_ValidMajorFrameCount "Click for more"  */
    uint32   MissedMajorFrameCount;                   /**< \schtlmmnemonic \SCH_MISSMFCTR
                                                           \brief # of missing Major Frame tones 
                                                           \details \ref SCHHK_MissedMajorFrameCount "Click for more"  */
    uint32   UnexpectedMajorFrameCount;               /**< \schtlmmnemonic \SCH_UNEXPCTDMFCTR
                                                           \brief # of unexpected Major Frame tones 
                                                           \details \ref SCHHK_UnexpectedMajorFrameCount "Click for more"  */
    uint16   MinorFramesSinceTone;                    /**< \schtlmmnemonic \SCH_MINORSINCETONE
                                                           \brief # of Minor Frames since last Major Frame tone 
                                                           \details \ref SCHHK_MinorFramesSinceTone "Click for more"  */
    uint16   NextSlotNumber;                          /**< \schtlmmnemonic \SCH_NEXTSLOT
                                                           \brief Next Minor Frame to be processed 
                                                           \details \ref SCHHK_NextSlotNumber "Click for more"  */
    uint16   LastSyncMETSlot;                         /**< \schtlmmnemonic \SCH_LASTSYNCMETSLOT
                                                           \brief Slot number where Time Sync last occurred 
                                                           \details \ref SCHHK_LastSyncMETSlot "Click for more"  */
    boolean  IgnoreMajorFrame;                        /**< \schtlmmnemonic \SCH_IGNOREMF
                                                           \brief Major Frame too noisy to trust 
                                                           \details \ref SCHHK_IgnoreMajorFrame "Click for more"  */
    boolean  UnexpectedMajorFrame;                    /**< \schtlmmnemonic \SCH_UNEXPCTDMAJORFRAME
                                                           \brief Most Recent Major Frame signal was unexpected 
                                                           \details \ref SCHHK_UnexpectedMajorFrame "Click for more"  */
} SCH_HkPacket_t;


/*************************************************************************
**
** SCH diagnostic telemetry packet definition
**
**************************************************************************/

/** 
**  \schtlm Scheduler Diagnostic Telemetry format
**/

typedef struct
{
    uint8           TlmHeader[CFE_SB_TLM_HDR_SIZE];   /**< \brief cFE Software Bus Telemetry Message Header */

    uint16          EntryStates[SCH_NUM_STATUS_BYTES_REQD/2]; 
                                                      /**< \schtlmmnemonic \SCH_ENTRYSTATES
                                                           \brief States of each Schedule Entry 
                                                           \details Each two bits represents the state for
                                                           a single entry in Schedule Definition Table.
                                                           Unused=0, Enabled=1, Disabled=2
                                                           MSBs are the lowest numbered entry */
    CFE_SB_MsgId_t   MsgIDs[SCH_TABLE_ENTRIES]; 
                                                      /**< \schtlmmnemonic \SCH_MSGIDS
                                                           \brief Message ID of msg associated with each entry */
} SCH_DiagPacket_t;

#endif /* _sch_msg_ */

/************************/
/*  End of File Comment */
/************************/

