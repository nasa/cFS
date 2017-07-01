/************************************************************************
** File:
**   $Id: sch_platform_cfg.h 1.8 2015/03/01 14:01:36EST sstrege Exp  $
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
**  The CFS Scheduler (SCH) Application platform configuration header file
**
** Notes:
**
** $Log: sch_platform_cfg.h  $
** Revision 1.8 2015/03/01 14:01:36EST sstrege 
** Added copyright information
** Revision 1.7 2011/06/30 15:38:11EDT aschoeni 
** moved table filenames from /ram to /cf/apps
** Revision 1.6 2011/06/30 14:03:38EDT aschoeni 
** Added sch library for inhibiting schedule
** Revision 1.5 2011/06/02 16:18:19EDT aschoeni 
** Updated platform configuration and verification
** Revision 1.4 2011/03/30 17:25:21EDT aschoeni 
** Added a minimum message ID limit for defined messages
** Revision 1.3 2011/03/29 18:23:09EDT aschoeni 
** Moved Mission Revision to Platform Config file
** Revision 1.2 2009/03/27 00:24:20EDT dkobe 
** Added consecutive noisy major frame counter and platform config parameter to compare counter to
** Revision 1.1 2008/10/16 15:08:01EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/platform_inc/project.pj
**
*************************************************************************/
#ifndef _sch_platform_cfg_h_
#define _sch_platform_cfg_h_



/*************************************************************************
** Macro definitions
**************************************************************************/

/**
**  \schcfg Software Bus Command Pipe Depth 
**
**  \par Description:
**       Dictates the number of messages to SCH that can be queued while awaiting
**       processing by the SCH Application.
**
**  \par Limits
**       Must be greater than zero
*/
#define SCH_PIPE_DEPTH      12              /**< \brief SCH Command Pipe Depth */

/*
** Schedule table definitions...
*/
/**
**  \schcfg Minor Frame Frequency (in Hz) 
**
**  \par Description:
**       Dictates the number of minor frame slots within each Major Frame.
**
**  \par Limits
**       Must be 2 or more and less than 65536
*/
#define SCH_TOTAL_SLOTS         100    /* SCH wake-up rate (Hz) */


/**
**  \schcfg Maximum number of Activities per Minor Frame 
**
**  \par Description:
**       Dictates the number of activities that can be defined for each Minor Frame.
**
**  \par Limits
**       Must be at least one
*/
#define SCH_ENTRIES_PER_SLOT    5


/**
**  \schcfg Maximum Number of Message Definitions in Message Definition Table 
**
**  \par Description:
**       Dictates the number of messages that can be defined in Message Definition Table.
**
**  \par Limits
**       Must be at least one
*/
#define SCH_MAX_MESSAGES      128

/**
**  \schcfg Maximum Message ID allowed in Message Definition Table 
**
**  \par Description:
**       Dictates the maximum message ID that can be used in the Message Definition Table.
**
**  \par Limits
**       Must be less than or equal to SCH_MDT_MAX_MSG_ID and greater than or equal to 0
*/
#define SCH_MDT_MIN_MSG_ID    0

/**
**  \schcfg Minimum Message ID allowed in Message Definition Table 
**
**  \par Description:
**       Dictates the minimum message ID that can be used in the Message Definition Table.
**
**  \par Limits
**       Must be less than or equal to #CFE_SB_HIGHEST_VALID_MSGID and greater than SCH_MDT_MIN_MSG_ID
*/
#define SCH_MDT_MAX_MSG_ID    CFE_SB_HIGHEST_VALID_MSGID


/**
**  \schcfg Maximum Length, in Words, of a Message 
**
**  \par Description:
**       Dictates the maximum number of words that can be assigned to a particular message
**       in the Message Definition Table.
**
**  \par Limits
**       Must be at least large enough to hold the smallest possible message header 
* **     (see #CFE_SB_TLM_HDR_SIZE and #CFE_SB_CMD_HDR_SIZE)
*/
#define SCH_MAX_MSG_WORDS      64   /* max message length (in words) */


/*
** Limits for how far we can get behind and how much we can do at once...
*/
/**
**  \schcfg Maximum Number of slots allowed for catch-up before skipping 
**
**  \par Description:
**       Dictates the number of Minor Frames that will be processed in "Catch Up"
**       mode before giving up and skipping ahead.
**
**  \par Limits
**       
*/
#define SCH_MAX_LAG_COUNT             (SCH_TOTAL_SLOTS / 2)


/**
**  \schcfg Maximum Number of Slots to be processed when in "Catch Up" mode 
**
**  \par Description:
**       Dictates the maximum number of slots SCH will process when trying to
**       "Catch Up" to the correct slot for the current time.
**
**  \par Limits
**       Must be at least one
*/
#define SCH_MAX_SLOTS_PER_WAKEUP      5

/*
** Conversion factor for how many microseconds in a wake-up period...
*/
/**
**  \schcfg Major Frame Period (in microseconds) 
**
**  \par Description:
**       Dictates the number microseconds in a Major Frame.
**
**  \par Limits
**       Must be greater than zero
*/
#define SCH_MICROS_PER_MAJOR_FRAME    1000000


/**
**  \schcfg Additional time allowed in Sync Slot to wait for Major Frame Sync (in microseconds) 
**
**  \par Description:
**       Dictates the additional time allowed in the Syncronization Slot
**       to allow the Major Frame Sync signal to be received and re-synchronize
**       processing.
**
**  \par Limits
**       Must be less than the normal slot period
*/
#define SCH_SYNC_SLOT_DRIFT_WINDOW   5000 


/**
**  \schcfg Time, in milliseconds, to wait for all applications to be started and ready to run 
**
**  \par Description:
**       Dictates the timeout for the #CFE_ES_WaitForStartupSync call that SCH uses to wait
**       for all of the Applications specified in the startup script to finish initialization.
**       SCH will wait this amount of time before assuming all startup script applications have
**       been started and will then begin nominal schedule processing.
**
**  \par Limits
**       None
*/
#define SCH_STARTUP_SYNC_TIMEOUT   50000 


/**
**  \schcfg Time, in microseconds, to wait for first Major Frame Sync to arrive 
**
**  \par Description:
**       Dictates the time allowed for the first Major Frame sync signal to arrive
**       before assuming it is not going to occur and switching to a freewheeling
**       mode.
**
**  \par Limits
**       Must be greater than or equal to the Major Frame Period
*/
#define SCH_STARTUP_PERIOD   (5*SCH_MICROS_PER_MAJOR_FRAME) 


/*
** Specifies the limit on the number of consecutive noisy Major Frame signals
** before we begin to ignore them.
*/
/**
**  \schcfg Maximum Number of consecutive Noisy Major Frame signals before they are ignored 
**
**  \par Description:
**       Dictates the number of consecutive "Noisy" Major Frame Signals (i.e. - signals that 
**       occur outside the expected window of their occurence) until the Major Frame signal
**       is automatically ignored and the Minor Frame Timer is used instead.
**
**  \par Limits
**       This value should never be set to less than two because a single "noisy" Major
**       Frame signal is likely when turning on or switching the 1 Hz signal on the spacecraft.
*/
#define SCH_MAX_NOISY_MAJORF             2


/**
**  \schcfg Scheduler API Library Usage Status 
**
**  \par Description:
**       Determines whether or not the Scheduler application is using the API library allowing
**       for external tasks to disable and enable schedule processing. Note that if the library
**       is to be used it must be generated as a separate object, and loaded prior to the
**       appplication.
**
**  \par Limits
**       This value must either be 0 when not using or including the library, or 1 if the library
**       is going to be used.
*/
#define SCH_LIB_PRESENCE            1


/**
**  \schcfg Scheduler API Library Initial Inhibition Count 
**
**  \par Description:
**       Sets the number of times the scheduler must be enabled following library initialization.
**       This allows the scheduler to come up either enabled (set to 0) or disabled (set to
**       a value greater than or equal to 1)
**
**  \par Limits
**       This value must be an unsigned 32 bit integer.
*/
#define SCH_LIB_DIS_CTR             0


/**
**  \schcfg Default SCH Schedule Definition Table Filename
**
**  \par Description:
**       The value of this constant defines the default filename of 
**       the SCH Schedule Definition Table
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed 
**       the #OS_MAX_PATH_LEN value.
*/

#define SCH_SCHEDULE_FILENAME  "/cf/apps/sch_def_schtbl.tbl"


/**
**  \schcfg Default SCH Message Definition Table Filename
**
**  \par Description:
**       The value of this constant defines the default filename of the 
**       SCH Message Definition Table
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed 
**       the #OS_MAX_PATH_LEN value.
*/

#define SCH_MESSAGE_FILENAME   "/cf/apps/sch_def_msgtbl.tbl"


/** \schcfg Mission specific version number for SCH application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "sch_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/

#define SCH_MISSION_REV            0


#endif /* _sch_platform_cfg_h_ */

/************************/
/*  End of File Comment */
/************************/
