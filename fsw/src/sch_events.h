/*
** $Id: sch_events.h 1.3 2015/03/01 14:02:08EST sstrege Exp  $
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
** Subsystem: Scheduler (SCH) event message ID's
**
** Author:
**
** Notes:
**
** $Log: sch_events.h  $
** Revision 1.3 2015/03/01 14:02:08EST sstrege 
** Added copyright information
** Revision 1.2 2011/06/30 15:30:40EDT aschoeni 
** add custom functionality
** Revision 1.1 2008/10/16 15:08:36EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
*/

#ifndef _sch_events_
#define _sch_events_

/*************************************************************************
**
** Include section
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/** \brief <tt> 'SCH Initialized. Version %d.%d.%d.%d' </tt>
**  \event <tt> 'SCH Initialized. Version %d.%d.%d.%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the Scheduler App completes its
**  initialization.
**
**  The first \c %d field contains the Application's Major Version Number
**  The second \c %d field contains the Application's Minor Version Number
**  The third \c %d field contains the Application's Revision Number
**  The fourth \c %d field contains the Application's Mission Revision Number
**/
#define SCH_INITSTATS_INF_EID                      1

/** \brief <tt> 'SCH App: terminating, err = 0x%08X' </tt>
**  \event <tt> 'SCH App: terminating, err = 0x%08X' </tt> 
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is always issued whenever the Scheduler Application
**  exits.  Exiting can be caused by an error during initialization, while
**  loading a table or on command via the ES.
**/
#define SCH_APP_EXIT_EID                     2

/** \brief <tt> 'Error Creating SB Pipe, RC=0x%08X' </tt>
**  \event <tt> 'Error Creating SB Pipe, RC=0x%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to create its
**  command pipe via the #CFE_SB_CreatePipe API.
**
**  The \c RC field contains the return code from the #CFE_SB_CreatePipe API.
**/
#define SCH_CR_PIPE_ERR_EID                   3

/** \brief <tt> 'Error Subscribing to HK Request(MID=0x%04X), RC=0x%08X' </tt>
**  \event <tt> 'Error Subscribing to HK Request(MID=0x%04X), RC=0x%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to subscribe to its
**  Housekeeping Request message via the #CFE_SB_Subscribe API.
**
**  The \c MID value identifies the Message ID that Scheduler was attempting to
**  subscribe to.  The \c RC value contains the return code that was obtained from
**  the #CFE_SB_Subscribe API.
**/
#define SCH_SUB_HK_REQ_ERR_EID                4

/** \brief <tt> 'Error Subscribing to GND CMD(MID=0x%04X), RC=0x%08X' </tt>
**  \event <tt> 'Error Subscribing to GND CMD(MID=0x%04X), RC=0x%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to subscribe to its
**  Ground commands via the #CFE_SB_Subscribe API.
**
**  The \c MID value identifies the Message ID that Scheduler was attempting to
**  subscribe to.  The \c RC value contains the return code that was obtained from
**  the #CFE_SB_Subscribe API.
**/
#define SCH_SUB_GND_CMD_ERR_EID               5

/** \brief <tt> 'Error Registering SDT, RC=0x%08X' </tt>
**  \event <tt> 'Error Registering SDT, RC=0x%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to register its
**  Schedule Definition Table with cFE Table Services via the #CFE_TBL_Register API.
**
**  The \c RC value is the return code from the #CFE_TBL_Register API call.
**/
#define SCH_SDT_REG_ERR_EID                   7

/** \brief <tt> 'Error Registering MDT, RC=0x%08X' </tt>
**  \event <tt> 'Error Registering MDT, RC=0x%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to register its
**  Message Definition Table with cFE Table Services via the #CFE_TBL_Register API.
**
**  The \c RC value is the return code from the #CFE_TBL_Register API call.
**/
#define SCH_MDT_REG_ERR_EID                   8

/** \brief <tt> 'Error (RC=0x%08X) Loading SDT with %s' </tt>
**  \event <tt> 'Error (RC=0x%08X) Loading SDT with %s' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to load its
**  Schedule Definition Table with cFE Table Services via the #CFE_TBL_Load API.
**
**  The \c RC value is the return code from the #CFE_TBL_Load API call
**  and \c %s is the filename that was used for the load attempt.
**/
#define SCH_SDT_LOAD_ERR_EID                  9

/** \brief <tt> 'Error (RC=0x%08X) Loading MDT with %s' </tt>
**  \event <tt> 'Error (RC=0x%08X) Loading MDT with %s' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to load its
**  Message Definition Table with cFE Table Services via the #CFE_TBL_Load API.
**
**  The \c RC value is the return code from the #CFE_TBL_Load API call
**  and \c %s is the filename that was used for the load attempt.
**/
#define SCH_MDT_LOAD_ERR_EID                 10

/** \brief <tt> 'Error Acquiring Tbl Ptrs (RC=0x%08X)' </tt>
**  \event <tt> 'Error Acquiring Tbl Ptrs (RC=0x%08X)' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when Scheduler is unable to obtain pointers to
**  the contents of either the Schedule Definition Table or the Message 
**  Definition Table with cFE Table Services via the #CFE_TBL_GetAddress API.
**
**  The \c RC value is the return code from the #CFE_TBL_GetAddress API.
**/
#define SCH_ACQ_PTR_ERR_EID                  11

/** \brief <tt> 'Error creating Timer (RC=0x%08X)' </tt>
**  \event <tt> 'Error creating Timer (RC=0x%08X)' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when an error is detected while
**  creating the Timer via the #SCH_CustomEarlyInit call.
**
**  The \c RC value is the return code from the #SCH_CustomEarlyInit call.
**/
#define SCH_MINOR_FRAME_TIMER_CREATE_ERR_EID 12

/** \brief <tt> 'OS Timer Accuracy (%d > reqd %d usec) requires Minor Frame MET sync' </tt>
**  \event <tt> 'OS Timer Accuracy (%d > reqd %d usec) requires Minor Frame MET sync' </tt> 
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is generated when the minor frame timer obtained from OSAL
**  has a specified clock accuracy that is not adequate enough for the Scheduler
**  to maintain precise enough timing.  The Scheduler will try to maintain conistent
**  timing by implementing an additional minor frame synchronization to the MET clock.
**  This means the Scheduler may end up needing to process more than one minor frame at
**  a time if the minor frame timer does not stay close enough to required time.
**
**  The first \c %d field specifies the clock accuracy (in microseconds) obtained for the 
**  minor frame timer and the second \c %d field specifies the clock accuracy (in 
**  microseconds) that Scheduler has determined is necessary to maintain adequate timing.
**/
#define SCH_MINOR_FRAME_TIMER_ACC_WARN_EID   13

/** \brief <tt> 'Error initializing Timers (RC=0x%08X)' </tt>
**  \event <tt> 'Error initializing Timers (RC=0x%08X)' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated during initialization when the Scheduler Application
**  is attempting to attach to the cFE TIME Services Time Synchronization Signal and start
**  the minor frame timer.
**
**  The \c RC field identifies the return code from the #SCH_CustomLateInit function.
**/
#define SCH_MAJOR_FRAME_SUB_ERR_EID          14

/** \brief <tt> 'Error creating Main Loop Timing Semaphore (RC=0x%08X)' </tt>
**  \event <tt> 'Error creating Main Loop Timing Semaphore (RC=0x%08X)' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when Scheduler is unable to create the necessary
**  semaphore used to synchronize itself with the Major and Minor Frame timing sources.
**
**  The \c RC value is the return code obtained from the #OS_BinSemCreate API.
**/
#define SCH_SEM_CREATE_ERR_EID               15

/** \brief <tt> 'Slot did not increment: slot = %d' </tt>
**  \event <tt> 'Slot did not increment: slot = %d' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is issued when the Scheduler has determined that it has awoken
**  in the same minor frame slot as the last cycle.  It will not perform any activities
**  this cycle and may need to perform two sets of activities next cycle to make up for
**  the lost time.
**
**  The \c %d field identifies the Minor Frame (slot) since the last Major Frame Signal.
**/
#define SCH_SAME_SLOT_EID                    16

/** \brief <tt> 'Slots skipped: slot = %d, count = %d' </tt>
**  \event <tt> 'Slots skipped: slot = %d, count = %d' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a significant jump in minor frame timing has
**  occurred and the Scheduler has determined that catching up is not feasible.
**
**  The \c slot value identifies what the minor frame to be executed is.  The \c count
**  field identifies the number of minor frames that were skipped in order to get to
**  the current minor frame.
**/
#define SCH_SKIPPED_SLOTS_EID                17

/** \brief <tt> 'Multiple slots processed: slot = %d, count = %d' </tt>
**  \event <tt> 'Multiple slots processed: slot = %d, count = %d' </tt> 
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is generated when the Scheduler determines that more than one
**  minor frame needs to be processed in order to keep up with the current time.
**
**  The \c slot value identifies the current minor frame.  The \c count field
**  specifies the number of minor frames processed on this cycle.
**/
#define SCH_MULTI_SLOTS_EID                  18

/** \brief <tt> 'Corrupt data error (1): slot = %d, entry = %d' </tt>
**  \event <tt> 'Corrupt data error (1): slot = %d, entry = %d' </tt> 
**  \event <tt> 'Corrupt data error (2): msg = %d, freq = %d, type = %d, rem = %d' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when an Activity is found in the Schedule Definition
**  Table that has inappropriate values.  The table must have become corrupt since being
**  loaded since the value(s) that are incorrect were verified before the table was loaded.
**
**  The \c slot value identifies the minor frame containing the error.
**  The \c entry value identifies which entry for the specified minor frame is in error.
**  The \c msg value specifies the index into the Message Definition Table.
**  The \c freq value specifies the activity's frequency (in seconds)
**  The \c type value specifies the activity's type
**  The \c rem value specifies the activity's remainder (used to stagger items with the
**  same frequency value.
**/
#define SCH_CORRUPTION_EID                   19

/** \brief <tt> 'Activity error: slot = %d, entry = %d, err = 0x%08X' </tt>
**  \event <tt> 'Activity error: slot = %d, entry = %d, err = 0x%08X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when an Send Message Activity fails to send
**  the message via the #CFE_SB_SendMsg API.
**
**  The \c slot field identifies the minor frame in the Schedule Definition Table
**  containing the activity that failed.
**  The \c entry field identifies the entry in the minor frame that failed.
**  The \c err field specifies the return code from the #CFE_SB_SendMsg API.
**/
#define SCH_PACKET_SEND_EID                  20

/** \brief <tt> 'Major Frame Sync too noisy (Slot %d). Disabling synchronization.' </tt>
**  \event <tt> 'Major Frame Sync too noisy (Slot %d). Disabling synchronization.' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when two consecutive Major Frame Signals are
**  received significantly earlier than expected.
**
**  The \c Slot field identifies the current slot when the Major Frame Signal arrived.  It
**  should have been the last slot in the schedule.
**/
#define SCH_NOISY_MAJOR_FRAME_ERR_EID        21

/** \brief <tt> 'Schedule tbl verify error - idx[%d] ena[%d] typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X]' </tt>
**  \event <tt> 'Schedule tbl verify error - idx[%d] ena[%d] typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X]' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when the operator has requested validation
**  of a new Schedule Definition Table and it contains an error.
**
**  The \c idx field specifies the index into the Schedule Definition Table containing the error.
**  The index is a linear count with the first entry in the table being index zero.
**  The \c ena field specifies the contents of the Enable/Disable field for the entry in error.
**  The \c typ field specifies the contents of the Activity Type field for the entry in error.
**  The \c fre field specifies the contents of the Frequency field for the entry in error.
**  The \c rem field specifies the contents of the Remainder field for the entry in error.
**  The \c msg field specifies the contents of the Message Index for the entry in error.
**  The \c grp field specifies the contents of the Group/Multi-Group field for the entry in error.
**/
#define SCH_SCHEDULE_TBL_ERR_EID             30

/** \brief <tt> 'Schedule table verify results -- good[%d] bad[%d] unused[%d]' </tt>
**  \event <tt> 'Schedule table verify results -- good[%d] bad[%d] unused[%d]' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message provides a statistics summary of the results of validating the
**  contents of a new Schedule Definition Table.
**
**  The \c good field specifies the number of entries that were deemed correct.
**  The \c bad field specifies the number of entries that contained an error.
**  The \c unused field specifies the number of entries that are empty.
**/
#define SCH_SCHEDULE_TABLE_EID               31

/** \brief <tt> 'Message tbl verify err - idx[%d] mid[0x%X] len[%d] buf[%d]' </tt>
**  \event <tt> 'Message tbl verify err - idx[%d] mid[0x%X] len[%d] buf[%d]' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when the operator has requested validation
**  of a new Message Definition Table and it contains an error.
**
**  The \c idx field specifies the index into the Message Definition Table containing the error.
**  The index is a linear count with the first entry in the table being index zero.
**  The \c mid field specifies the contents of the Message ID field for the entry in error.
**  The \c len field specifies the contents of the Message Length field for the entry in error.
**  The \c buf field specifies the index into an empty message that contains data.
**/
#define SCH_MESSAGE_TBL_ERR_EID              32

/** \brief <tt> 'Message tbl verify results - good[%d] bad[%d] unused[%d]' </tt>
**  \event <tt> 'Message tbl verify results - good[%d] bad[%d] unused[%d]' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message provides a statistics summary of the results of validating the
**  contents of a new Message Definition Table.
**
**  The \c good field specifies the number of entries that were deemed correct.
**  The \c bad field specifies the number of entries that contained an error.
**  The \c unused field specifies the number of entries that are empty.
**/
#define SCH_MESSAGE_TABLE_EID                33

/** \brief <tt> 'NO-op command. Version %d.%d.%d.%d' </tt>
**  \event <tt> 'NO-op command. Version %d.%d.%d.%d' </tt> 
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is always automatically issued when Scheduler receives
**  the \link #SCH_NOOP_CC Scheduler NO-OP Command \endlink.
**
**  The first \c %d field contains the Application's Major Version Number
**  The second \c %d field contains the Application's Minor Version Number
**  The third \c %d field contains the Application's Revision Number
**  The fourth \c %d field contains the Application's Mission Revision Number
**/
#define SCH_NOOP_CMD_EID                     40

/** \brief <tt> 'RESET command' </tt>
**  \event <tt> 'RESET command' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is always automatically issued in response 
**  to a Scheduler \link #SCH_RESET_CC Reset Counters command \endlink
**/
#define SCH_RESET_CMD_EID                    41

/** \brief <tt> 'ENABLE command: slot = %d, entry = %d' </tt>
**  \event <tt> 'ENABLE command: slot = %d, entry = %d' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_ENABLE_CC Enable Entry Command \endlink
**  is received and successfully processed.
**
**  The \c slot field specifies the slot index from the Enable command.
**  The \c entry field specifies the entry index from the Enable command.
**/
#define SCH_ENABLE_CMD_EID                   42

/** \brief <tt> 'DISABLE command: slot = %d, entry = %d' </tt>
**  \event <tt> 'DISABLE command: slot = %d, entry = %d' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_DISABLE_CC Disable Entry Command \endlink
**  is received and successfully processed.
**
**  The \c slot field specifies the slot index from the Disable command.
**  The \c entry field specifies the entry index from the Disable command.
**/
#define SCH_DISABLE_CMD_EID                  43

/** \brief <tt> 'ENABLE GROUP command: match count = %d' </tt>
**  \event <tt> 'ENABLE GROUP command: match count = %d' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_ENABLE_GROUP_CC Enable Group Command \endlink
**  is received and successfully processed.
**
**  The \c count field specifies the number of Activities in the Schedule Definition Table
**  that were Enabled due to this command.
**/
#define SCH_ENA_GRP_CMD_EID                  44

/** \brief <tt> 'DISABLE GROUP command: match count = %d' </tt>
**  \event <tt> 'DISABLE GROUP command: match count = %d' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_DISABLE_GROUP_CC Disable Group Command \endlink
**  is received and successfully processed.
**
**  The \c count field specifies the number of Activities in the Schedule Definition Table
**  that were Disabled due to this command.
**/
#define SCH_DIS_GRP_CMD_EID                  45

/** \brief <tt> 'Major Frame Synchronization Enabled' </tt>
**  \event <tt> 'Major Frame Synchronization Enabled' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is always automatically issued when the Scheduler receives
**  a \link #SCH_ENABLE_SYNC_CC Enable Sync Command \endlink.
**/
#define SCH_ENA_SYNC_CMD_EID                 46

/** \brief <tt> 'Transmitting Diagnostic Message' </tt>
**  \event <tt> 'Transmitting Diagnostic Message' </tt> 
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event message is always automatically issued when the Scheduler receives
**  a \link #SCH_SEND_DIAG_TLM_CC Send Diagnostic Telemetry Command \endlink.
**/
#define SCH_SEND_DIAG_CMD_EID                47

/** \brief <tt> 'ENABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)' </tt>
**  \event <tt> 'ENABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_ENABLE_CC Enable Entry Command \endlink
**  is received that contains either a slot or entry index that is out of the acceptable range.
**
**  The \c slot parameters identify first-the slot in the command, second-the maximum allowed slot
**  The \c entry parameters identify first-the entry index in the command, second-the maximum
**  allowed entry index.
**/
#define SCH_ENABLE_CMD_ARG_ERR_EID           50

/** \brief <tt> 'ENABLE command: invalid state = %d, slot = %d, entry = %d' </tt>
**  \event <tt> 'ENABLE command: invalid state = %d, slot = %d, entry = %d' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_ENABLE_CC Enable Entry Command \endlink
**  is received that specifies a slot that does not have an appropriate State.
**
**  The \c state parameter identifies the current state of the specified table entry.  The
**  state should be either #SCH_ENABLED, or #SCH_DISABLED.  If it is #SCH_UNUSED, the operator
**  is not allowed to enable it.  Any other value represents a corrupted table.
**  The \c slot parameter identifies the slot specified in the Enable command.
**  The \c entry parameter identifies the entry index specified in the Enable command.
**/
#define SCH_ENABLE_CMD_ENTRY_ERR_EID         51

/** \brief <tt> 'DISABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)' </tt>
**  \event <tt> 'DISABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_DISABLE_CC Disable Entry Command \endlink
**  is received that contains either a slot or entry index that is out of the acceptable range.
**
**  The \c slot parameters identify first-the slot in the command, second-the maximum allowed slot
**  The \c entry parameters identify first-the entry index in the command, second-the maximum
**  allowed entry index.
**/
#define SCH_DISABLE_CMD_ARG_ERR_EID          52

/** \brief <tt> 'DISABLE command: invalid state = %d, slot = %d, entry = %d' </tt>
**  \event <tt> 'DISABLE command: invalid state = %d, slot = %d, entry = %d' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_DISABLE_CC Disable Entry Command \endlink
**  is received that specifies a slot that does not have an appropriate State.
**
**  The \c state parameter identifies the current state of the specified table entry.  The
**  state should be either #SCH_ENABLED, or #SCH_DISABLED.  If it is #SCH_UNUSED, the operator
**  is not allowed to disable it.  Any other value represents a corrupted table.
**  The \c slot parameter identifies the slot specified in the Disable command.
**  The \c entry parameter identifies the entry index specified in the Disable command.
**/
#define SCH_DISABLE_CMD_ENTRY_ERR_EID        53

/** \brief <tt> 'ENABLE GROUP command: invalid argument, no groups selected' </tt>
**  \event <tt> 'ENABLE GROUP command: invalid argument, no groups selected' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_ENABLE_GROUP_CC Enable Group Command \endlink
**  is received that does not specify either a group or a multi-group.
**/
#define SCH_ENA_GRP_CMD_ERR_EID              54

/** \brief <tt> 'ENABLE GROUP command: Neither Group %d nor Multi-Group 0x%06X found' </tt>
**  \event <tt> 'ENABLE GROUP command: Neither Group %d nor Multi-Group 0x%06X found' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_ENABLE_GROUP_CC Enable Group Command \endlink
**  is received that specifies either a group and/or multi-group that cannot be located as being
**  associated with any Activities defined in the Schedule Definition Table.
**
**  The \c Group field identifies the Group ID number that was in the received Enable Group command.
**  The \c Multi-Group field identifies the Multi-Group bit mask that was in the received Enable Group command.
**/
#define SCH_ENA_GRP_NOT_FOUND_ERR_EID        55

/** \brief <tt> 'DISABLE GROUP command: invalid argument, no groups selected' </tt>
**  \event <tt> 'DISABLE GROUP command: invalid argument, no groups selected' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_DISABLE_GROUP_CC Disable Group Command \endlink
**  is received that does not specify either a group or a multi-group.
**/
#define SCH_DIS_GRP_CMD_ERR_EID              56

/** \brief <tt> 'DISABLE GROUP command: Neither Group %d nor Multi-Group 0x%06X found' </tt>
**  \event <tt> 'DISABLE GROUP command: Neither Group %d nor Multi-Group 0x%06X found' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when a \link #SCH_DISABLE_GROUP_CC Disable Group Command \endlink
**  is received that specifies either a group and/or multi-group that cannot be located as being
**  associated with any Activities defined in the Schedule Definition Table.
**
**  The \c Group field identifies the Group ID number that was in the received Disable Group command.
**  The \c Multi-Group field identifies the Multi-Group bit mask that was in the received Disable Group command.
**/
#define SCH_DIS_GRP_NOT_FOUND_ERR_EID        57

/** \brief <tt> 'Invalid command code: ID = 0x%04X, CC = %d' </tt>
**  \event <tt> 'Invalid command code: ID = 0x%04X, CC = %d' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when Scheduler obtains a message from its
**  command pipe that does not have a valid command code for either the Ground
**  Message ID or the Flight Message ID.
**
**  The \c ID field identifies the Message ID of the message with the inappropriate
**  command code.
**  The \c CC field identifies the Command Code found in the message.
**/
#define SCH_CC_ERR_EID                       58

/** \brief <tt> 'Msg with Invalid message ID Rcvd -- ID = 0x%04X' </tt>
**  \event <tt> 'Msg with Invalid message ID Rcvd -- ID = 0x%04X' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated when Scheduler obtains a message from its
**  command pipe with a message ID that it did not subscribe to.
**
**  The \c ID field identifies the Message ID of the obtained message.
**/
#define SCH_MD_ERR_EID                       59

/** \brief <tt> 'Cmd Msg with Bad length Rcvd: ID = 0x%04X, CC = %d, Exp Len = %d, Len = %d' </tt>
**  \event <tt> 'Cmd Msg with Bad length Rcvd: ID = 0x%04X, CC = %d, Exp Len = %d, Len = %d' </tt> 
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is generated whenever the Scheduler receives a command message whose
**  length does not correctly correspond to the length expected for a message with its command code.
**
**  The \c ID field specifies the Message ID of the message in error.
**  The \c CC field specifies the Command Code.
**  The <B> Exp Len </B> field specifies the Expected Length for a command with this command code.
**  The \c Len field specifies the actual length of the command received.
**/
#define SCH_CMD_LEN_ERR_EID                  60

#endif /* _sch_events_ */

/************************/
/*  End of File Comment */
/************************/
