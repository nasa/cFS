/*
** $Id: sch_verify.h 1.3 2017/06/21 15:29:22EDT mdeschu Exp  $
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
** Purpose:  CFS Scheduler (SCH) configuration verification
**
** Notes:
**
*/

#ifndef _sch_verify_
#define _sch_verify_

/*************************************************************************/

#if SCH_PIPE_DEPTH < 1
  #error SCH_PIPE_DEPTH must be greater than zero. 
#endif

#if SCH_ENTRIES_PER_SLOT < 1
  #error SCH_ENTRIES_PER_SLOT must be greater than zero.  Schedule Definition Table must have at least one entry per slot.
#endif

#if SCH_MAX_MESSAGES < 1
  #error SCH_MAX_MESSAGES must be greater than zero.  Message Definition Table must have at least one message definition.
#endif

#if SCH_MAX_MESSAGES < 1
  #error SCH_MAX_MESSAGES must be greater than zero.  Message Definition Table must have at least one message definition.
#endif

#if SCH_MDT_MIN_MSG_ID < 0
  #error  SCH_MDT_MIN_MSG_ID must be at least 0.
#endif

#if SCH_MDT_MAX_MSG_ID < SCH_MDT_MIN_MSG_ID
  #error  SCH_MDT_MAX_MSG_ID must be at least SCH_MDT_MIN_MSG_ID.
#endif

#if SCH_MDT_MAX_MSG_ID > CFE_SB_HIGHEST_VALID_MSGID
  #error SCH_MDT_MAX_MSG_ID must be less than or equal to CFE_SB_HIGHEST_VALID_MSGID.
#endif

#if SCH_MAX_LAG_COUNT < 1
  #error SCH_MAX_LAG_COUNT must be at least 1.
#endif

#if SCH_MAX_LAG_COUNT > SCH_TOTAL_SLOTS
  #error SCH_MAX_LAG_COUNT cannot be greater than SCH_TOTAL_SLOTS.
#endif


#if SCH_MAX_SLOTS_PER_WAKEUP < 1
  #error SCH_MAX_SLOTS_PER_WAKEUP must be at least 1.
#endif

#if SCH_MICROS_PER_MAJOR_FRAME < SCH_TOTAL_SLOTS
  #error SCH_MICROS_PER_MAJOR_FRAME must be greater than SCH_TOTAL_SLOTS (i.e. - a normal slot period must be at least 1 microsecond)
#endif

#if SCH_STARTUP_SYNC_TIMEOUT < 0
  #error SCH_STARTUP_SYNC_TIMEOUT must be greater than or equal to zero.
#endif

#if SCH_STARTUP_PERIOD < SCH_MICROS_PER_MAJOR_FRAME
  #error SCH_STARTUP_PERIOD must be greater than or equal to SCH_MICROS_PER_MAJOR_FRAME.
#endif

/*
** Make sure Diagnostic Packet does not exceed mission packet size
** NOTE: The "2" below is for the current size of a CFE_SB_MsgId_t in bytes.  "sizeof" cannot be used in #if statements.
**       and the "14" is the size of a CCSDS Telemetry Header
*/
#ifdef MESSAGE_FORMAT_IS_CCSDS
#if (SCH_NUM_STATUS_BYTES_REQD + 2*SCH_TABLE_ENTRIES + 12 ) > CFE_SB_MAX_SB_MSG_SIZE
  #error SCH Diagnostic Packet exceeds maximum allowed message size (see CFE_SB_MAX_SB_MSG_SIZE)
#endif
#endif

/*
** Validate Number of Minor Frames per Major Frame
*/
#if SCH_TOTAL_SLOTS > 65535
  #error SCH_TOTAL_SLOTS cannot specify more than 65535 minor frames per major frame
#endif

#if SCH_TOTAL_SLOTS < 2
  #error SCH_TOTAL_SLOTS must specify at least 2 minor frames per major frame
#endif

/*
** Ensure Allowed Drift is not larger than a single slot
*/
#if SCH_SYNC_SLOT_DRIFT_WINDOW >= (SCH_MICROS_PER_MAJOR_FRAME/SCH_TOTAL_SLOTS)
  #error SCH_SYNC_SLOT_DRIFT_WINDOW must be less than one normal slot period
#endif

#if SCH_SYNC_SLOT_DRIFT_WINDOW < 0
  #error SCH_SYNC_SLOT_DRIFT_WINDOW must be greater than or equal to zero
#endif

/*
** Noisy Major Frame
*/

#if SCH_MAX_NOISY_MAJORF < 2
  #error SCH_MAX_NOISY_MAJORF must specify at least 2 noisy frames before disregarding
#endif

#ifndef SCH_MISSION_REV
    #error SCH_MISSION_REV must be defined!
#elif (SCH_MISSION_REV < 0)
    #error SCH_MISSION_REV must be greater than or equal to zero!
#endif 


/*************************************************************************/

#endif /* _sch_verify_ */

/************************/
/*  End of File Comment */
/************************/
