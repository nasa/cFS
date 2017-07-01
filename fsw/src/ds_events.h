/************************************************************************
** File:
**   $Id: ds_events.h 1.3 2017/01/25 12:05:35EST sstrege Exp  $
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
** Purpose: 
**  The CFS Data Storage (DS) Application event id header file
**
** Notes:
**
*************************************************************************/
#ifndef _ds_events_h_
#define _ds_events_h_


/*************************************************************************
** Macro definitions
**************************************************************************/

/**
**  \brief <tt> 'Application Initialization Event' </tt>
**
**  \event <tt> 'Application initialized, version \%d.\%d.\%d.\%d, data at 0x\%08X' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued if the Data Storage (DS) application successfully
**  completes its initialization.
**
**  The first \c \%d field contains the Major Version Number
**  The second \c \%d field contains the Minor Version Number
**  The third \c \%d field contains the Revision Number
**  The fourth \c \%d field contains the Mission Revision Number
**  The \c 0x\%08X field contains the address of the global data structure (DS_AppData)
*/
#define DS_INIT_EID              1


/**
**  \brief <tt> 'Application Initialization Error' </tt>
**
**  \event <tt> 'Unable to register for EVS services, err = 0x\%08X' </tt>
**  \event <tt> 'Unable to create input pipe, err = 0x\%08X' </tt>
**  \event <tt> 'Unable to subscribe to housekeeping request commands, err = 0x\%08X' </tt>
**  \event <tt> 'Unable to subscribe to 1Hz wakeup commands, err = 0x\%08X' </tt>
**  \event <tt> 'Unable to subscribe to DS commands, err = 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the application receives an error while
**  performing cFE initialization.  The value displayed is the return code
**  from the function that failed.
*/
#define DS_INIT_ERR_EID          2


/**
**  \brief <tt> 'Application Termination Event' </tt>
**
**  \event <tt> 'Application terminating, err = 0x\%08X' </tt>
**
**  \par Type: CRITICAL
**
**  \par Cause:
**
**  This event message is issued when the application performs a voluntary
**  termination.  The termination may be caused by a cFE command requesting
**  the application be terminated - or, the cause may result from the DS
**  application having received an error when invoking a cFE or O/S function
**  necessary for normal operation.
**
**  The value displayed is either zero (if cFE requested the termination) or
**  the return code from the function that failed.
*/
#define DS_EXIT_ERR_EID          3


/**
**  \brief <tt> 'Critical Data Store Access Error' </tt>
**
**  \event <tt> 'Critical Data Store access error = 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  The DS application stores the current values for Destination Filename
**  sequence counts in the Critical Data Store (CDS).  This ensures that DS
**  will not overwrite old data storage files following a processor reset.
**  This event indicates an error at startup as DS is initializing access
**  to the Critical Data Store.  Subsequent CDS errors are ignored by DS.
*/
#define DS_INIT_CDS_ERR_EID      6


/**
**  \brief <tt> 'Table Restored From CDS' </tt>
**
**  \event <tt> 'Destination File Table data restored from CDS' </tt>
**  \event <tt> 'Filter Table data restored from CDS' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  If configured to make DS tables critical, the DS application will try and
**  recover the table data in use prior to a processor reset.  These startup
**  initialization events indicate that the table data has been restored from
**  the Critical Data Store (CDS).
*/
#define DS_INIT_TBL_CDS_EID      7


/**
**  \brief <tt> 'Table Create Error' </tt>
**
**  \event <tt> 'Unable to register Filter Table: Error = 0x\%08X' </tt>
**  \event <tt> 'Unable to register Destination File Table: Error = 0x\%08X' </tt>
**  \event <tt> 'Unable to load default Destination File Table: Filename = '\%s', Error = 0x\%08X' </tt>
**  \event <tt> 'Unable to load default Filter Table: Filename = '\%s', Error = 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  At startup, the DS application must create both a packet filter table and
**  a destination file definition table.  After creating the tables, DS must
**  then load default table data for each of the files.  If DS is unable to
**  create either table then the application will terminate.  If DS is able
**  to create both tables and is then unable to load both tables then DS will
**  operate in a reduced function mode - thus allowing table load error 
**  recovery via the cFE Table Services command interface.
**
**  The values displayed indicate the cFE Table Services API function return
**  code and the table filename, if appropriate.
*/
#define DS_INIT_TBL_ERR_EID      8


/**
**  \brief <tt> 'Destination File Table Validation Results' </tt>
**
**  \event <tt> 'Destination file table verify results: desc text = \%s, good = \%d, bad = \%d, unused = \%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event describes the results of the Destination File Table validation
**  function.  The cFE Table Services Manager will call this function autonomously
**  when the default table is loaded at startup and also whenever a table validate
**  command (that targets this table) is processed.
**
**  The event text will indicate the descriptor text verification result (OK or bad),
**  the number of table entries that were verified without error (good), the number
**  of table entries that had one or more errors (bad) and the number of unused
**  table entries (unused).  Note that the totals also include the results of the
**  verification test for the table description text.  Thus, the sum of good + bad
**  + unused results will equal the number of table entries plus one.
*/
#define DS_FIL_TBL_EID           10


/**
**  \brief <tt> 'Destination File Validation Error' </tt>
**
**  \event <tt> 'Destination file table verify err: invalid descriptor text' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, invalid pathname text' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, invalid basename text' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, invalid extension text' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, filename type = \%d' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, file enable state = \%d' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, max file size = \%d' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, max file age = \%d' </tt>
**  \event <tt> 'Destination file table verify err: index = \%d, sequence count = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These are the errors that the DS application might encounter when verifying
**  the contents of a Destination File Table.  Note that only the first error
**  detected for each table file will result in an event being generated.  The
**  remaining entries are still tested to establish the result counters but no
**  further error events will be generated during this call to the verify function.
**
**  The event text will indicate the table index value (where appropriate), the
**  invalid field name and the field value for non-string fields.  If a string
**  field is invalid then attempts to display the string could result in other
**  errors.
*/
#define DS_FIL_TBL_ERR_EID       11


/**
**  \brief <tt> 'Filter Table Validation Results' </tt>
**
**  \event <tt> 'Filter table verify results: desc text = \%s, good = \%d, bad = \%d, unused = \%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event describes the results of the Packet Filter Table validation
**  function.  The cFE Table Services Manager will call this function autonomously
**  when the default table is loaded at startup and also whenever a table validate
**  command (that targets this table) is processed.
**
**  The event text will indicate the descriptor text verification result (OK or bad),
**  the number of table entries that were verified without error (good), the number
**  of table entries that had one or more errors (bad) and the number of unused
**  table entries (unused).  Note that the totals also include the results of the
**  verification test for the table description text.  Thus, the sum of good + bad
**  + unused results will equal the number of table entries plus one.
*/
#define DS_FLT_TBL_EID           12


/**
**  \brief <tt> 'Filter Table Validation Error' </tt>
**
**  \event <tt> 'Filter table verify err: invalid descriptor text' </tt>
**  \event <tt> 'Filter table verify err: MID = 0x\%04X, index = \%d, filter = \%d, file table index = \%d' </tt>
**  \event <tt> 'Filter table verify err: MID = 0x\%04X, index = \%d, filter = \%d, filter type = \%d' </tt>
**  \event <tt> 'Filter table verify err: MID = 0x\%04X, index = \%d, filter = \%d, filter parms N = \%d, X = \%d, O = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  These are the errors that the DS application might encounter when verifying
**  the contents of a Packet Filter Table.  Note that only the first error
**  detected for each table file will result in an event being generated.  The
**  remaining entries are still tested to establish the result counters but no
**  further error events will be generated during this call to the verify function.
**
**  The event text will indicate the message ID, table index value and filter
**  index value (where appropriate), the field name and the field value for
**  non-string fields.  If a string field is invalid then attempts to display
**  the string could result in other errors.
*/
#define DS_FLT_TBL_ERR_EID       13


/**
**  \brief <tt> 'Filename Create Error' </tt>
**
**  \event <tt> 'FILE NAME error: dest = \%d, path = '\%s', base = '\%s', seq = '\%s', ext = '\%s'' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This error occurs when an attempt to create a destination filename fails.
**  The expected cause of this error is for the sum of the lengths of each
**  filename element to be invalid, even though each individual element
**  length is valid.  Note that each of the individual element strings has
**  been previously verified.
**
**  The event text will indicate the Destination File Table index, the file
**  path string, the file basename string, the filename sequence string and
**  the extension portion of the filename.
**
**  Note that the filename create error handler will also disable the
**  destination to prevent the immediate re-occurrence of the error as more
**  packets are written to this destination.
*/
#define DS_FILE_NAME_ERR_EID     14


/**
**  \brief <tt> 'File Create Error' </tt>
**
**  \event <tt> 'FILE CREATE error: result = \%d, dest = \%d, name = '\%s'' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This error occurs when an attempt to create a destination file fails.
**  The most common cause of this error is insufficient free space available
**  on the target file system (disk).  However, it is possible to specify
**  a filename that is legal but still invalid (i.e. the path string has
**  legal chars but does not exist).
**
**  The event text will indicate the result from the system function call
**  to create the file, the Destination File Table index and the filename.
**
**  Note that the file create error handler will also disable the destination
**  to prevent the immediate re-occurrence of the error as more packets are
**  written to this destination.
*/
#define DS_CREATE_FILE_ERR_EID   15


/**
**  \brief <tt> 'File Write Error' </tt>
**
**  \event <tt> 'FILE WRITE error: result = \%d, length = \%d, dest = \%d, name = '\%s'' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This error occurs when an attempt to write to a destination file fails.
**  The most common cause of this error is insufficient free space available
**  on the target file system (disk).  Other causes for this error imply
**  some sort of operational or file system failure and will require careful
**  analysis.
**
**  The event text will indicate the result from the system function call
**  to write to the file, the Destination File Table index and the filename.
**
**  Note that the file write error handler will also close the file and
**  disable the destination to prevent the immediate re-occurrence of the
**  error as more packets are written to this destination.
*/
#define DS_WRITE_FILE_ERR_EID    16


/**
**  \brief <tt> 'Invalid packet (command code)' </tt>
**
**  \event <tt> 'Invalid command code: MID = 0x\%04X, CC = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This error occurs when a DS application command is received that has
**  an undefined command code (CC).  Ground systems generally prevent
**  these errors so the cause is more likely to result from sending "raw"
**  ground commands or on-board stored commands.
**
**  The event text will indicate the Message ID (MID) and command code.
*/
#define DS_CMD_CODE_ERR_EID      21


/**
**  \brief <tt> 'Housekeeping request (error)' </tt>
**
**  \event <tt> 'Invalid HK request length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a HK request command.
**  The cause of the failure is an invalid command packet length.
*/
#define DS_HK_REQUEST_ERR_EID    22


/**
**  \brief <tt> 'No-op command' </tt>
**
**  \event <tt> 'NOOP command, Version \%d.\%d.\%d.\%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event signals the successful execution of an application NOOP
**  command.  The command is used primarily as an indicator that the DS
**  application can receive commands and generate telemetry.
**
**  The first \c \%d field contains the Major Version Number
**  The second \c \%d field contains the Minor Version Number
**  The third \c \%d field contains the Revision Number
**  The fourth \c \%d field contains the Mission Revision Number
*/
#define DS_NOOP_CMD_EID          31


/**
**  \brief <tt> 'No-op command (error)' </tt>
**
**  \event <tt> 'Invalid NOOP command length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a NOOP command.
**  The cause of the failure is an invalid command packet length.
*/
#define DS_NOOP_CMD_ERR_EID      32


/**
**  \brief <tt> 'Reset Housekeeping Counters command' </tt>
**
**  \event <tt> 'Reset counters command' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a RESET housekeeping
**  counters command.  The command is used primarily to clear counters
**  that have already been examined.
*/
#define DS_RESET_CMD_EID         33


/**
**  \brief <tt> 'Reset Housekeeping Counters command (error)' </tt>
**
**  \event <tt> 'Invalid RESET command length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a RESET command.
**  The cause of the failure is an invalid command packet length.
*/
#define DS_RESET_CMD_ERR_EID     34


/**
**  \brief <tt> 'Set Application Enable/Disable State command' </tt>
**
**  \event <tt> 'APP STATE command: state = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set
**  the application enable/disable state for processing data storage
**  packets.  The DS application is always enabled to receive and
**  process commands.
**
**  The event text will indicate the new setting for the application
**  enable/disable state.
*/
#define DS_ENADIS_CMD_EID        35


/**
**  \brief <tt> 'Set Application Enable/Disable State command (error)' </tt>
**
**  \event <tt> 'Invalid APP STATE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid APP STATE command arg: app state = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  application enable/disable state.  The cause of the failure may
**  be an invalid command packet length or an invalid enable/disable
**  state value.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_ENADIS_CMD_ERR_EID    36


/**
**  \brief <tt> 'Set Filter File Index command' </tt>
**
**  \event <tt> 'FILTER FILE command: MID = 0x\%X, index = \%d, filter = \%d, file = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  The Packet Filter Table consists of an array of packet structures,
**  with each element in the array representing one packet message ID.
**
**  Each packet structure contains an array of filter structures, and
**  each filter structure contains a destination file selection.  Thus,
**  each packet has multiple filters and each filter selects a file.
**
**  File selections are expressed as an index into the Destination File
**  Table.
**  
**  This event signals the successful execution of a command to set the
**  destination file index for a single filter entry in the Packet Filter
**  Table.
**
**  The event text will indicate the packet message ID, the filter table
**  index, the filter array index and the new file table index selection.
*/
#define DS_FILE_CMD_EID          37


/**
**  \brief <tt> 'Set Filter File Index command (error)' </tt>
**
**  \event <tt> 'Invalid FILTER FILE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid FILTER FILE command arg: invalid messageID = 0x\%04X' </tt>
**  \event <tt> 'Invalid FILTER FILE command arg: filter parameters index = \%d' </tt>
**  \event <tt> 'Invalid FILTER FILE command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid FILTER FILE command: packet filter table is not loaded' </tt>
**  \event <tt> 'Invalid FILTER FILE command: Message ID 0x\%04X is not in filter table' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  Destination File Table index for a Packet Filter Table entry.
**  The cause of the failure may be an invalid command packet length,
**  an invalid Message ID or an invalid filter parameters index.
**
**  The failure may also result from not having a Packet Filter Table
**  loaded at the time the command was invoked or because the message
**  ID was not found in the Packet Filter Table.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_FILE_CMD_ERR_EID      38


/**
**  \brief <tt> 'Set Filter Type command' </tt>
**
**  \event <tt> 'FILTER TYPE command: MID = 0x\%04X, index = \%d, filter = \%d, type = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  The Packet Filter Table consists of an array of packet structures,
**  with each element in the array representing one packet message ID.
**
**  Each packet structure contains an array of filter structures, and
**  each filter structure contains a filter type selection.  Thus, each
**  packet has multiple filters and each filter selects a filter type.
**
**  This event signals the successful execution of a command to set the
**  filter type for a single filter entry in the Packet Filter Table.
**
**  The event text will indicate the packet message ID, the filter table
**  index, the filter array index and the new filter type selection.
*/
#define DS_FTYPE_CMD_EID         39


/**
**  \brief <tt> 'Set Filter Type command (error)' </tt>
**
**  \event <tt> 'Invalid FILTER TYPE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid FILTER TYPE command arg: invalid messageID = 0x\%04X' </tt>
**  \event <tt> 'Invalid FILTER TYPE command arg: filter parameters index = \%d' </tt>
**  \event <tt> 'Invalid FILTER TYPE command arg: filter type = \%d' </tt>
**  \event <tt> 'Invalid FILTER TYPE command: packet filter table is not loaded' </tt>
**  \event <tt> 'Invalid FILTER TYPE command: Message ID 0x\%04X is not in filter table' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  filter type for a Packet Filter Table entry.  The cause of the
**  failure may be an invalid command packet length, an invalid message
**  ID, an invalid Filter Parameters index or an invalid filter type.
**
**  The failure may also result from not having a Packet Filter Table
**  loaded at the time the command was invoked or because the Message
**  ID was not found in the Packet Filter Table.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_FTYPE_CMD_ERR_EID     40


/**
**  \brief <tt> 'Set Filter Parameters command' </tt>
**
**  \event <tt> 'FILTER PARMS command: MID = 0x\%04X, index = \%d, filter = \%d, N = \%d, X = \%d, O = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  The Packet Filter Table consists of an array of packet structures,
**  with each element in the array representing one packet message ID.
**
**  Each packet structure contains an array of filter structures, and
**  each filter structure contains a set of filter parameters.  Thus,
**  each packet has multiple filters and each filter has unique filter
**  parameters.
**
**  This event signals the successful execution of a command to set the
**  filter parameters for a single filter entry in the Packet Filter Table.
**
**  The event text will indicate the packet message ID, the filter table
**  index, the filter array index and the new filter parameters (N,X,O).
*/
#define DS_PARMS_CMD_EID         41


/**
**  \brief <tt> 'Set Filter Parameters command (error)' </tt>
**
**  \event <tt> 'Invalid FILTER PARMS command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid FILTER PARMS command arg: invalid messageID = 0x\%04X' </tt>
**  \event <tt> 'Invalid FILTER PARMS command arg: filter parameters index = \%d' </tt>
**  \event <tt> 'Invalid FILTER PARMS command arg: N = \%d, X = \%d, O = \%d' </tt>
**  \event <tt> 'Invalid FILTER PARMS command: packet filter table is not loaded' </tt>
**  \event <tt> 'Invalid FILTER PARMS command: Message ID 0x\%04X is not in filter table' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  filter parameters for a Packet Filter Table entry.  The cause of
**  the failure may be an invalid command packet length, an invalid
**  message ID, an invalid filter parameters index or an invalid filter
**  parameters value (N,X,O).
**
**  The failure may also result from not having a Packet Filter Table
**  loaded at the time the command was invoked or because the Message
**  ID was not found in the Packet Filter Table.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_PARMS_CMD_ERR_EID     42


/**
**  \brief <tt> 'Set Filename Type command' </tt>
**
**  \event <tt> 'DEST TYPE command: file table index = \%d, filename type = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  filename type for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  filename type.
*/
#define DS_NTYPE_CMD_EID         43


/**
**  \brief <tt> 'Set Filename Type command (error)' </tt>
**
**  \event <tt> 'Invalid DEST TYPE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST TYPE command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST TYPE command arg: filename type = \%d' </tt>
**  \event <tt> 'Invalid DEST TYPE command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  filename type for the selected Destination File Table entry.  The
**  cause of the failure may be an invalid command packet length, an
**  invalid file table index or an invalid filename type.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_NTYPE_CMD_ERR_EID     44


/**
**  \brief <tt> 'Set File Enable/Disable State command' </tt>
**
**  \event <tt> 'DEST STATE command: file table index = \%d, file state = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  enable/disable state for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  file enable/disable state.
*/
#define DS_STATE_CMD_EID         45


/**
**  \brief <tt> 'Set File Enable/Disable State command (error)' </tt>
**
**  \event <tt> 'Invalid DEST STATE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST STATE command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST STATE command arg: file state = \%d' </tt>
**  \event <tt> 'Invalid DEST STATE command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  enable/disable state for the selected Destination File Table entry.
**  The cause of the failure may be an invalid command packet length,
**  an invalid file table index or an invalid state selection.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_STATE_CMD_ERR_EID     46


/**
**  \brief <tt> 'Set Filename Path command' </tt>
**
**  \event <tt> 'DEST PATH command: file table index = \%d, pathname = '\%s'' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  path portion of the filename for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  filename path text.
*/
#define DS_PATH_CMD_EID          47


/**
**  \brief <tt> 'Set Filename Path command (error)' </tt>
**
**  \event <tt> 'Invalid DEST PATH command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST PATH command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST PATH command arg: pathname' </tt>
**  \event <tt> 'Invalid DEST PATH command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  filename path text for a Destination File Table entry.  The cause
**  of the failure may be an invalid command packet length, an invalid
**  file table index or invalid filename path text.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_PATH_CMD_ERR_EID      48


/**
**  \brief <tt> 'Set File Basename command' </tt>
**
**  \event <tt> 'DEST BASE command: file table index = \%d, base filename = '\%s'' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  basename portion of the filename for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  file basename text.
*/
#define DS_BASE_CMD_EID          49


/**
**  \brief <tt> 'Set File Basename command (error)' </tt>
**
**  \event <tt> 'Invalid DEST BASE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST BASE command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST BASE command arg: base filename' </tt>
**  \event <tt> 'Invalid DEST BASE command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  base filename text for a Destination File Table entry.  The cause
**  of the failure may be an invalid command packet length, an invalid
**  file table index or invalid base filename text.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_BASE_CMD_ERR_EID      50


/**
**  \brief <tt> 'Set Filename Extension command' </tt>
**
**  \event <tt> 'DEST EXT command: file table index = \%d, extension = '\%s'' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  extension portion of the filename for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  filename extension text.
*/
#define DS_EXT_CMD_EID           51


/**
**  \brief <tt> 'Set Filename Extension command (error)' </tt>
**
**  \event <tt> 'Invalid DEST EXT command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST EXT command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST EXT command arg: extension' </tt>
**  \event <tt> 'Invalid DEST EXT command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  filename extension text for a Destination File Table entry.  The
**  cause of the failure may be an invalid command packet length, an
**  invalid file table index or invalid filename extension text.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_EXT_CMD_ERR_EID       52


/**
**  \brief <tt> 'Set Max File Size command' </tt>
**
**  \event <tt> 'DEST SIZE command: file table index = \%d, size limit = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  file size limit for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  file size limit.
*/
#define DS_SIZE_CMD_EID          53


/**
**  \brief <tt> 'Set Max File Size command (error)' </tt>
**
**  \event <tt> 'Invalid DEST SIZE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST SIZE command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST SIZE command arg: size limit = \%d' </tt>
**  \event <tt> 'Invalid DEST SIZE command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  file size limit for a Destination File Table entry.  The cause
**  of the failure may be an invalid command packet length, an invalid
**  file table index or an invalid file size limit.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_SIZE_CMD_ERR_EID      54


/**
**  \brief <tt> 'Set Max File Age command' </tt>
**
**  \event <tt> 'DEST AGE command: file table index = \%d, age limit = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  file age limit for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  file age limit.
*/
#define DS_AGE_CMD_EID           55


/**
**  \brief <tt> 'Set Max File Age command (error)' </tt>
**
**  \event <tt> 'Invalid DEST AGE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST AGE command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST AGE command arg: age limit = \%d' </tt>
**  \event <tt> 'Invalid DEST AGE command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  file age limit for a Destination File Table entry.  The cause
**  of the failure may be an invalid command packet length, an invalid
**  file table index or an invalid file age limit.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_AGE_CMD_ERR_EID       56


/**
**  \brief <tt> 'Set Sequence Count command' </tt>
**
**  \event <tt> 'DEST COUNT command: file table index = \%d, sequence count = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to set the
**  sequence count for a Destination File Table entry.
**
**  The event text will indicate the file table index, and the new
**  sequence count.
*/
#define DS_SEQ_CMD_EID           57


/**
**  \brief <tt> 'Set Sequence Count command (error)' </tt>
**
**  \event <tt> 'Invalid DEST COUNT command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST COUNT command arg: file table index = \%d' </tt>
**  \event <tt> 'Invalid DEST COUNT command arg: sequence count = \%d' </tt>
**  \event <tt> 'Invalid DEST COUNT command: destination file table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to set the
**  sequence count for a Destination File Table entry.  The cause
**  of the failure may be an invalid command packet length, an invalid
**  file table index or an invalid sequence count.
**
**  The failure may also result from not having a Destination File Table
**  loaded at the time the command was invoked.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_SEQ_CMD_ERR_EID       58


/**
**  \brief <tt> 'Close Destination File command' </tt>
**
**  \event <tt> 'DEST CLOSE command: file table index = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to close
**  the selected Destination File.
**
**  This command will succeed if the file table index is valid,
**  regardless of whether there was an open file.
*/
#define DS_CLOSE_CMD_EID         59


/**
**  \brief <tt> 'Close Destination File command (error)' </tt>
**
**  \event <tt> 'Invalid DEST CLOSE command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid DEST CLOSE command arg: file table index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to close the
**  selected Destination File.  The cause of the failure may be an
**  invalid command packet length or an invalid file table index.  
**
**  The event text will indicate the cause of the failure.
*/
#define DS_CLOSE_CMD_ERR_EID     60


/**
**  \brief <tt> 'File Move Error' </tt>
**
**  \event <tt> 'FILE MOVE error: dir name = '\%s', filename = '\%s'' </tt>
**  \event <tt> 'FILE MOVE error: src = '\%s', tgt = '\%s', result = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed attempt to move a Data Storage file
**  from the temporary working directory it was created in, to the
**  directory specified in the file definition table.  The cause of the
**  failure may be an invalid combined directory and filename length or
**  the system file move function may have returned an error result.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_MOVE_FILE_ERR_EID     61


/**
**  \brief <tt> 'Get File Info command' </tt>
**
**  \event <tt> 'GET FILE INFO command' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to get
**  the file info diagnostic packet.
**
**  This command will succeed even if the selected file does not exist.
*/
#define DS_GET_FILE_INFO_CMD_EID 62


/**
**  \brief <tt> 'Get File Info command (error)' </tt>
**
**  \event <tt> 'Invalid GET FILE INFO command length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to get the
**  file info diagnostic packet.  The cause of the failure is an
**  invalid command packet length.  
**
**  The event text will indicate the expected and actual packet size.
*/
#define DS_GET_FILE_INFO_CMD_ERR_EID     63


/**
**  \brief <tt> 'Add Message ID to Filter Table command' </tt>
**
**  \event <tt> 'ADD MID command: MID = 0x\%04X, index = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to add
**  a new message ID to the Packet Filter Table.
**
**  The Packet Filter Table must be loaded and have an unused entry
**  available for adding the new message ID.  The new message ID must
**  not be zero and must not already exist in the table.
*/
#define DS_ADD_MID_CMD_EID       64


/**
**  \brief <tt> 'Add Message ID to Filter Table command (error)' </tt>
**
**  \event <tt> 'Invalid ADD MID command length: expected = \%d, actual = \%d' </tt>
**  \event <tt> 'Invalid ADD MID command arg: invalid MID = 0x\%4X' </tt>
**  \event <tt> 'Invalid ADD MID command: filter table is not loaded' </tt>
**  \event <tt> 'Invalid ADD MID command: MID = 0x\%4X is already in filter table at index = \%d' </tt>
**  \event <tt> 'Invalid ADD MID command: filter table is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to add a new
**  message ID to the Packet Filter Table.  The cause of the failure
**  may be an invalid command packet length or an invalid message ID.
**
**  The failure may also result from not having a Packet Filter Table
**  loaded at the time the command was invoked.  The loaded table
**  must have an unused entry available for the new message ID and
**  must not already contain the new message ID.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_ADD_MID_CMD_ERR_EID   65


/**
**  \brief <tt> 'Close All Destination Files command' </tt>
**
**  \event <tt> 'DEST CLOSE ALL command' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause:
**
**  This event signals the successful execution of a command to close
**  all open Destination Files.
**
**  This command will succeed regardless of whether there was an open file.
*/
#define DS_CLOSE_ALL_CMD_EID     66


/**
**  \brief <tt> 'Close All Destination Files command (error)' </tt>
**
**  \event <tt> 'Invalid DEST CLOSE ALL command length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event signals the failed execution of a command to close
**  all open Destination Files.  The cause of the failure can only
**  be an invalid command packet length.
**
**  The event text will indicate the cause of the failure.
*/
#define DS_CLOSE_ALL_CMD_ERR_EID 67


#endif /* _ds_events_h_ */

/************************/
/*  End of File Comment */
/************************/
