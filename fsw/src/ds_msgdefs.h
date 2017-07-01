/************************************************************************
** File:
**   $Id: ds_msgdefs.h 1.3 2017/01/25 12:05:42EST sstrege Exp  $
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
**  The CFS Data Storage (DS) command codes header file
**
** Notes:
**
*************************************************************************/

#ifndef _ds_msgdefs_h_
#define _ds_msgdefs_h_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS application command codes                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** \dscmd No-Operation Command
**
**  \par Description
**       This command will increment the command execution counter and send an
**       event containing the version number of the application.  The command 
**       is often used as a general test for application "aliveness".         
**
**  \dscmdmnemonic \DS_NOOP
**
**  \par Command Structure
**       #DS_NoopCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_NOOP_CMD_EID informational event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_NOOP_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_NOOP_CC              0


/** \dscmd Reset Housekeeping Telemetry Counters Command
**
**  \par Description
**       This command will set the following housekeeping counters to zero:
**       - Command Execution Counter (\DS_CMDPC)
**       - Command Error Counter (\DS_CMDEC)
**       - Ignored Packet Counter (\DS_IGNOREDPKTCNT)
**       - Filtered Packet Counter (\DS_FILTEREDPKTCNT)
**       - Passed Packet Counter (\DS_PASSEDPKTCNT)
**       - File Access Counter (\DS_FILEWRITECNT)
**       - File Access Error Counter (\DS_FILEWRITEERRCNT)
**       - Destination Table Load Counter (\DS_DESTLOADCNT)
**       - Filter Table Load Counter (\DS_FILTERLOADCNT)
**       - Destination Table Ptr Error Counter (\DS_DESTPTRERRCNT)
**       - Filter Table Ptr Error Counter (\DS_FILTERPTRERRCNT)
**
**  \dscmdmnemonic \DS_RESETCTRS
**
**  \par Command Structure
**       #DS_ResetCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_RESET_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_RESET_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_RESET_CC             1


/** \dscmd Set Enable/Disable State For DS Application
**
**  \par Description
**       This command will modify the Ena/Dis State selection for the
**       DS application.  No packets are stored while DS is disabled.
**
**  \dscmdmnemonic \DS_ENABLE (fixed command arg = enable)
**  \dscmdmnemonic \DS_DISABLE (fixed command arg = disable)
**
**  \par Command Structure
**       #DS_AppStateCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_ENADIS_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid enable/disable state selection
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_ENADIS_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_APP_STATE_CC    2


/** \dscmd Set Destination File For Packet Filter Table Entry
**
**  \par Description
**       This command will modify the Destination File selection for the
**       indicated entry in the Packet Filter Table.
**
**  \dscmdmnemonic \DS_FILTERFILE
**
**  \par Command Structure
**       #DS_FilterFileCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_FILE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid filter table index
**       - Invalid filter parameters index
**       - Invalid destination file selection
**       - Packet filter table is not currently loaded
**       - Cannot modify unused packet filter table entry
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_FILE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_FILTER_FILE_CC   3


/** \dscmd Set Filter Type For Packet Filter Table Entry
**
**  \par Description
**       This command will modify the Filter Type selection for the
**       indicated entry in the Packet Filter Table.
**
**  \dscmdmnemonic \DS_FILTERTYPE
**
**  \par Command Structure
**       #DS_FilterTypeCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_FTYPE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid filter table index
**       - Invalid filter parameters index
**       - Invalid filter type selection
**       - Packet filter table is not currently loaded
**       - Cannot modify unused packet filter table entry
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_FTYPE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_FILTER_TYPE_CC   4


/** \dscmd Set Algorithm Parameters For Packet Filter Table Entry
**
**  \par Description
**       This command will modify the Algorithm Parameters for the
**       indicated entry in the Packet Filter Table.
**
**  \dscmdmnemonic \DS_FILTERPARMS
**
**  \par Command Structure
**       #DS_FilterParmsCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_PARMS_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid filter table index
**       - Invalid filter parameters index
**       - Algorithm_N value cannot be greater than Algorithm_X
**       - Packet filter table is not currently loaded
**       - Cannot modify unused packet filter table entry
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_PARMS_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_FILTER_PARMS_CC  5


/** \dscmd Set Filename Type For Destination File Table Entry
**
**  \par Description
**       This command will modify the Filename Type selection for the
**       indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_FILENAMETYPE
**
**  \par Command Structure
**       #DS_DestTypeCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_NTYPE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid filename type selection
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_NTYPE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_TYPE_CC     6


/** \dscmd Set Ena/Dis State For Destination File Table Entry
**
**  \par Description
**       This command will modify the Ena/Dis State selection for the
**       indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_ENABLEFILE (fixed command arg = enable)
**  \dscmdmnemonic \DS_DISABLEFILE (fixed command arg = disable)
**
**  \par Command Structure
**       #DS_DestStateCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_STATE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid destination state selection
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_STATE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_STATE_CC    7


/** \dscmd Set Pathname For Destination File Table Entry
**
**  \par Description
**       This command will modify the Pathname portion of the filename
**       for the indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_PATHNAME
**
**  \par Command Structure
**       #DS_DestPathCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_PATH_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid pathname string (empty, no terminator, bad characters)
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_PATH_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_PATH_CC     8


/** \dscmd Set Basename For Destination File Table Entry
**
**  \par Description
**       This command will modify the Basename portion of the filename
**       for the indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_BASENAME
**
**  \par Command Structure
**       #DS_DestBaseCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_BASE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid basename string (no terminator, bad characters)
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_BASE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_BASE_CC     9


/** \dscmd Set Extension For Destination File Table Entry
**
**  \par Description
**       This command will modify the Extension portion of the filename
**       for the indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_EXTNAME
**
**  \par Command Structure
**       #DS_DestExtCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_EXT_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid extension string (empty, no terminator, bad characters)
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_EXT_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_EXT_CC     10


/** \dscmd Set Max File Size For Destination File Table Entry
**
**  \par Description
**       This command will modify the max file size selection for the
**       indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_MAXFILESIZE
**
**  \par Command Structure
**       #DS_DestSizeCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_SIZE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid max file size selection
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_SIZE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_SIZE_CC    11


/** \dscmd Set Max File Age For Destination File Table Entry
**
**  \par Description
**       This command will modify the max file age selection for the
**       indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_MAXFILEAGE
**
**  \par Command Structure
**       #DS_DestAgeCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_AGE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid max file age selection
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_AGE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_AGE_CC     12


/** \dscmd Set Sequence Count For Destination File Table Entry
**
**  \par Description
**       This command will modify the sequence count value for the
**       indicated entry in the Destination File Table.
**
**  \dscmdmnemonic \DS_SEQCOUNT
**
**  \par Command Structure
**       #DS_DestCountCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_SEQ_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Invalid sequence count value
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_SEQ_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_SET_DEST_COUNT_CC   13


/** \dscmd Close Destination File
**
**  \par Description
**       This command will close the indicated Destination File.
**
**  \dscmdmnemonic \DS_CLOSEFILE
**
**  \par Command Structure
**       #DS_CloseFileCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_CLOSE_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Invalid destination file table index
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_CLOSE_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_CLOSE_FILE_CC       14

      
/** \dscmd Get File Info Packet
**
**  \par Description
**       This command will send the DS File Info Packet.
**
**  \dscmdmnemonic \DS_GETFILEINFO
**
**  \par Command Structure
**       #DS_GetFileInfoCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_FileInfoPkt_t packet will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_GET_FILE_INFO_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_GET_FILE_INFO_CC    15

      
/** \dscmd Add Message ID To Packet Filter Table
**
**  \par Description
**       This command will change the Message ID selection for an
**       unused Packet Filter Table entry to the indicated value.
**
**  \dscmdmnemonic \DS_ADDMID
**
**  \par Command Structure
**       #DS_AddMidCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_ADD_MID_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Message ID is invalid (can be anything but zero)
**       - Packet filter table is not currently loaded
**       - Message ID already exists in packet filter table
**       - All packet filter table entries are already in use
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_ADD_MID_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_ADD_MID_CC          16


/** \dscmd Close All Destination Files
**
**  \par Description
**       This command will close all open Destination Files.
**       - NOTE: Using this command may incur a performance hit based upon
**               the number and size of the files being closed.
**
**  \dscmdmnemonic \DS_CLOSEALL
**
**  \par Command Structure
**       #DS_CloseAllCmd_t
**
**  \par Command Verification
**       Evidence of success may be found in the following telemetry:
**       - \b \c \DS_CMDPC - command execution counter will increment
**       - The #DS_CLOSE_ALL_CMD_EID debug event message will be sent
**
**  \par Error Conditions
**       This command can fail for the following reasons:
**       - Invalid command packet length
**       - Destination file table is not currently loaded
**
**       Evidence of failure may be found in the following telemetry:
**       - \b \c \DS_CMDEC - command error counter will increment
**       - The #DS_CLOSE_ALL_CMD_ERR_EID error event message will be sent
**
**  \par Criticality
**       None
**
**  \sa
*/
#define DS_CLOSE_ALL_CC        17

      
#endif /* _ds_msgdefs_h_ */

/************************/
/*  End of File Comment */
/************************/
