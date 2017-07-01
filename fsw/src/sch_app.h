/************************************************************************
** File:
**   $Id: sch_app.h 1.7 2015/03/01 14:01:45EST sstrege Exp  $
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
** Purpose: 
**  The CFS Scheduler (SCH) Application header file
**
** Notes:
**
** $Log: sch_app.h  $
** Revision 1.7 2015/03/01 14:01:45EST sstrege 
** Added copyright information
** Revision 1.6 2011/06/14 15:26:32EDT aschoeni 
** Reduced command counters to 8 bit
** Revision 1.5 2011/06/14 15:11:52EDT aschoeni 
** Cleaned up comments on reset command
** Revision 1.4 2011/05/24 18:08:13EDT aschoeni 
** Moved Major frame event outside of callback
** Revision 1.3 2009/03/27 00:24:22EDT dkobe 
** Added consecutive noisy major frame counter and platform config parameter to compare counter to
** Revision 1.2 2009/03/26 16:05:09EDT dkobe 
** Added logic for determining worst case number of slots that could be executed in response to a poor minor frame timer signal.
** Revision 1.1 2008/10/16 15:08:33EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
**
*************************************************************************/
#ifndef _sch_app_
#define _sch_app_

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"
#include "sch_msg.h"
#include "sch_tbldefs.h"

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/*************************************************************************/
/*************************************************************************/

/*
** Computed Scheduler Parameters
*/
#define SCH_WORST_CLOCK_ACCURACY     (SCH_NORMAL_SLOT_PERIOD/(SCH_TOTAL_SLOTS-1))
                                     /**< \brief Maximum allowed error in minor frame timing */
                                     /**< Worst accuracy determined to be the amount of drift that would
                                          cause the loss of a minor frame over one major frame */
#define SCH_MAX_SYNC_ATTEMPTS        (SCH_TOTAL_SLOTS * 3)
                                     /**< \brief Maximum number of minor frames to sample looking for subsecs = 0 */
                                     /**< Maximum number of minor frame timer expirations to allow before giving up
                                          on finding the slot whose MET subseconds field is zero.  It is assumed that
                                          three complete major frames should be sufficient for finding such a slot. */
/*
** Command pipe definitions
*/
/** \name Scheduler App Pipe Characteristics */
/**  \{ */
#define SCH_PIPE_NAME       "SCH_CMD_PIPE"  /**< \brief SCH Command Pipe Name */
/** \} */

/*
** Event filter table definitions
*/
/** \name Scheduler App Event Filter Characteristics */
/**  \{ */
#define SCH_FILTER_COUNT    5
/** \} */

/*
** Minor Frame Slot Characteristics
*/
/** \name Scheduler App Minor Frame Characteristics */
/**  \{ */
#define SCH_TIME_SYNC_SLOT     (SCH_TOTAL_SLOTS-1)  /**< NOTE: If this is set to anything other than the last slot, 
                                                               logic in SCH_ProcessScheduleTable will need to change */
#define SCH_NORMAL_SLOT_PERIOD (SCH_MICROS_PER_MAJOR_FRAME / SCH_TOTAL_SLOTS)
#define SCH_SYNC_SLOT_PERIOD   (SCH_NORMAL_SLOT_PERIOD + SCH_SYNC_SLOT_DRIFT_WINDOW)
#define SCH_SHORT_SLOT_PERIOD  (SCH_NORMAL_SLOT_PERIOD - SCH_SYNC_SLOT_DRIFT_WINDOW)
/** \} */

/*
** Table names
*/
/** \name Scheduler App Table Names */
/**  \{ */
#define SCH_SCHEDULE_TABLE_NAME  "SCHED_DEF"  /**< \brief Schedule Definition Table Name */
#define SCH_MESSAGE_TABLE_NAME   "MSG_DEFS"   /**< \brief Message Definition Table Name */
/** \} */

/*
** Internal Function Return Codes
*/
/** \name Scheduler App Internal Error Codes */
/**  \{ */
#define SCH_SUCCESS              (0) /**< \brief SCH return code for success */
#define SCH_ERROR               (-1) /**< \brief SCH return code for general error */
#define SCH_BAD_MSG_LENGTH_RC   (-2) /**< \brief SCH return code for unexpected cmd length */
#define SCH_UNKNOWN_ACTIVITY    (-3) /**< \brief SCH return code for unknown Entry Activity Type */
/** \} */

/*************************************************************************
**
** Type definitions
**
**************************************************************************/

/*
** Type definition (SCH app global data)
*/
typedef struct
{
    /*
    ** Operational data 
    */
    CFE_SB_MsgPtr_t       MsgPtr;                         /**< \brief Ptr to most recently received cmd message */
    CFE_SB_PipeId_t       CmdPipe;                        /**< \brief Pipe ID for SCH Command Pipe */
    
    SCH_MessageEntry_t   *MessageTable;                   /**< \brief Ptr to Message Table contents */
    SCH_ScheduleEntry_t  *ScheduleTable;                  /**< \brief Ptr to Schedule Table contents */
    
    CFE_TBL_Handle_t      ScheduleTableHandle;            /**< \brief Handle for Schedule Definition Table */
    CFE_TBL_Handle_t      MessageTableHandle;             /**< \brief Handle for Message Definition Table */
    
    CFE_EVS_BinFilter_t   EventFilters[SCH_FILTER_COUNT]; /**< \brief Array of Event Filters */
    
    SCH_HkPacket_t        HkPacket;                       /**< \brief Housekeeping Telemetry Packet */
    SCH_DiagPacket_t      DiagPacket;                     /**< \brief Diagnostic Telemetry Packet */
    
    uint32                LastProcessCount;               /**< \brief Number of Slots Processed Last Cycle */
    
    uint32                TimerId;                        /**< \brief OSAL assigned timer ID for minor frame timer */
    uint32                TimeSemaphore;                  /**< \brief Semaphore used by time references to control main loop */
    uint32                ClockAccuracy;                  /**< \brief Accuracy of Minor Frame Timer */
    uint32                WorstCaseSlotsPerMinorFrame;    /**< \brief When syncing to MET, worst case # of slots that may need */
                                                          /*   to be processed upon each Minor Frame signal */
    
    
    uint32                AppID;                          /**< \brief SCH Application's Application ID */
    
    /*
    ** Command execution counters (ground commands)
    */
    uint8                 CmdCounter;                     /**< \brief Number of successful ground cmds received */
    uint8                 ErrCounter;                     /**< \brief Number of unsuccessful ground cmds received */
    uint8                 SyncToMET;                      /**< \brief Slots should be aligned with subseconds */
    uint8                 MajorFrameSource;               /**< \brief Major Frame Signal source identifier */
    
    /*
    ** Messages sent by schedule table processor
    */
    uint32                ScheduleActivitySuccessCount;   /**< \brief Number of successfully performed activities */
    uint32                ScheduleActivityFailureCount;   /**< \brief Number of unsuccessful activities attempted */
    
    /*
    ** Total schedule table slots processed
    */
    uint32                SlotsProcessedCount;            /**< \brief Total # of Schedule Slots (Minor Frames) Processed */
    
    /*
    ** The number of times that slots were skipped
    ** (not the number of slots that were skipped)
    */
    uint16                SkippedSlotsCount;              /**< \brief Number of times that slots were skipped */
                                                          /**< The number of times that a slot (minor frame) was skipped.
                                                             \c Note: This is NOT the number of slots that were skipped */
    
    /*
    ** The number of times that multiple slots were processed
    ** (not the number of slots that were processed)
    */
    uint16                MultipleSlotsCount;              /**< \brief Number of times that multiple slots processed */
                                                           /**< The number of times that multiple slots (minor frames)
                                                                were processed in the same minor frame.
                                                              \c Note: This is NOT the number of slots that were processed */
    
    /*
    ** The number of times that SCH woke up in the same slot as last time
    */
    uint16                SameSlotCount;                   /**< \brief # of times SCH woke up in the same slot as last time */
    
    /*
    ** The number of times that a table entry with bad data was processed
    ** (the entry previously passed validation but then somehow went bad)
    */
    uint16                BadTableDataCount;              /**< \brief # of times corrupted table entries were processed */
    
    /*
    ** The number of tables verified prior to table load
    */
    uint16                TableVerifySuccessCount;       /**< \brief # of times table loads successfully verified */
    uint16                TableVerifyFailureCount;       /**< \brief # of times table loads unsuccessfully verified */
    
    uint32                ValidMajorFrameCount;          /**< \brief # of valid Major Frame tones received */
    uint32                MissedMajorFrameCount;         /**< \brief # of missing Major Frame tones */
    uint32                UnexpectedMajorFrameCount;     /**< \brief # of unexpected Major Frame tones */

    uint32                TablePassCount;                /**< \brief # of times Schedule Table has been processed */
    uint32                ConsecutiveNoisyFrameCounter;  /**< \brief # of consecutive noisy Major Frames */

    uint16                MinorFramesSinceTone;          /**< \brief # of Minor Frames since last Major Frame tone */
    uint16                NextSlotNumber;                /**< \brief Next Minor Frame to be processed */
    uint16                LastSyncMETSlot;               /**< \brief MET Slot # where Time Sync last occurred */
    uint16                SyncAttemptsLeft;              /**< \brief Timeout counter used when syncing Major Frame to MET */
    
    boolean               IgnoreMajorFrame;              /**< \brief Major Frame too noisy to trust */
    boolean               IgnoreMajorFrameMsgSent;       /**< \brief Major Frame Event Message has been sent */
    boolean               UnexpectedMajorFrame;          /**< \brief Major Frame signal was unexpected */

} SCH_AppData_t;

/*************************************************************************
**
** Exported data
**
**************************************************************************/

extern SCH_AppData_t    SCH_AppData;

/*************************************************************************
**
** Exported functions
**
**************************************************************************/

/*
** Application entry point and main process loop
*/
/************************************************************************/
/** \brief CFS Scheduler (SCH) Application Entry Point
**  
**  \par Description
**       Scheduler application entry point and main process loop.
**       
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void   SCH_AppMain(void);

#endif /* _sch_app_ */

/************************/
/*  End of File Comment */
/************************/
