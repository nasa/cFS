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

/******************************************************************************
** File: cfe_platform_cfg.h
**
** Purpose:
**   This header file contains the platform configuration parameters.
** 
** Notes:
**   The impact of changing these configurations from their default value is
**   not yet documented.  Changing these values may impact the performance
**   and functionality of the system.
**
** Author:   R.McGraw/SSI
**
******************************************************************************/

#ifndef _cfe_platform_cfg_
#define _cfe_platform_cfg_

/*
** Allow reference to CFE_MISSION_SPACECRAFT_ID (see CFE_TBL_VALID_ definitions below)
*/
#include "cfe_mission_cfg.h"

/*
** CPU Id for target Processor
*/
#define CFE_PLATFORM_CPU_ID 1

/*
** CPU Name for target Processor
*/
#define CFE_PLATFORM_CPU_NAME "CPU1"

/**
**  \cfesbcfg Maximum Number of Unique Message IDs SB Routing Table can hold
**
**  \par Description:
**       Dictates the maximum number of unique MsgIds the SB routing table will hold.
**       This constant has a direct affect on the size of SB's tables and arrays.
**       Keeping this count as low as possible will save memory.
**       To see the run-time, high-water mark and the current utilization figures
**       regarding this parameter, send an SB command to 'Send Statistics Pkt'.
**
**  \par Limits
**       This parameter has a lower limit of 1 and an upper limit of 1024.
**
*/
#define CFE_PLATFORM_SB_MAX_MSG_IDS              256


/**
**  \cfesbcfg Maximum Number of Unique Pipes SB Routing Table can hold
**
**  \par Description:
**       Dictates the maximum number of unique Pipes the SB routing table will hold.
**       This constant has a direct affect on the size of SB's tables and arrays.
**       Keeping this count as low as possible will save memory.
**       To see the run-time, high-water mark and the current utilization figures
**       regarding this parameter, send an SB command to 'Send Statistics Pkt'.
**
**  \par Limits
**       This parameter has a lower limit of 1.  This parameter must also be less than
**       or equal to OS_MAX_QUEUES.
**
*/
#define CFE_PLATFORM_SB_MAX_PIPES                64


/**
**  \cfesbcfg Maximum Number of unique local destinations a single MsgId can have
**
**  \par Description:
**       Dictates the maximum number of unique local destinations a single MsgId can
**       have.
**
**  \par Limits
**       This parameter has a lower limit of 1.  There are no restrictions on the upper
**       limit however, the maximum number of destinations per packet is system dependent
**       and should be verified.  Destination number values that are checked against this
**       configuration are defined by a 16 bit data word.
**
*/
#define CFE_PLATFORM_SB_MAX_DEST_PER_PKT         16


/**
**  \cfesbcfg Default Subscription Message Limit
**
**  \par Description:
**       Dictates the default Message Limit when using the #CFE_SB_Subscribe API. This will
**       limit the number of messages with a specific message ID that can be received through
**       a subscription. This only changes the default; other message limits can be set on a per
**       subscription basis using #CFE_SB_SubscribeEx .
**
**  \par Limits
**       This parameter has a lower limit of 4 and an upper limit of 65535.
**
*/
#define CFE_PLATFORM_SB_DEFAULT_MSG_LIMIT        4


/**
**  \cfesbcfg Size of the SB buffer memory pool
**
**  \par Description:
**       Dictates the size of the SB memory pool. For each message the SB
**       sends, the SB dynamically allocates from this memory pool, the memory needed
**       to process the message. The memory needed to process each message is msg
**       size + msg descriptor(CFE_SB_BufferD_t). This memory pool is also used
**       to allocate destination descriptors (CFE_SB_DestinationD_t) during the
**       subscription process.
**       To see the run-time, high-water mark and the current utilization figures
**       regarding this parameter, send an SB command to 'Send Statistics Pkt'.
**       Some memory statistics have been added to the SB housekeeping packet.
**       NOTE: It is important to monitor these statistics to ensure the desired
**       memory margin is met.
**
**  \par Limits
**       This parameter has a lower limit of 512 and an upper limit of UINT_MAX (4 Gigabytes).
**
*/
#define CFE_PLATFORM_SB_BUF_MEMORY_BYTES         524288


/**
**  \cfesbcfg Maximum depth allowed when creating an SB pipe
**
**  \par Description:
**       The value of this constant dictates the maximum pipe depth that an
**       application may request. The pipe depth is given as a paramter in the
**       #CFE_SB_CreatePipe API.
**
**  \par Limits
**       This parameter has a lower limit of 1.  There are no restrictions on the
**       upper limit however, the maximum pipe depth is system dependent and should
**       be verified.  Pipe Depth values that are checked against this configuration
**       are defined by a 16 bit data word.
*/
#define CFE_PLATFORM_SB_MAX_PIPE_DEPTH           256


/**
**  \cfesbcfg Highest Valid Message Id
**
**  \par Description:
**       The value of this constant dictates the size of the SB message map. The SB
**       message map is a lookup table that provides the routing table index for
**       fast access into the routing table. The default setting of 0x1FFF was chosen
**       to save memory. This reduces the message map from 128Kbytes to 16Kbytes.
**       See CFE_FSW_DCR 504 for more details.
**     
**       If this value is different in a distributed architecture some platforms may not
**       be able to subscribe to messages generated on other platforms since the message id
**       would exceed the mapping table's highest index. Care would have to be taken to ensure the 
**       constrained platform did not subscribe to message Ids that exceed 
**       CFE_PLATFORM_SB_HIGHEST_VALID_MSGID 
**
**       The recommended case to to have this value the same across all mission platforms
**
**  \par Limits
**       This parameter has a lower limit of 1 and an upper limit of 0xFFFF. Note
**       for current implementations, V2/Extended headers assign 0xFFFFFFFF as the invalid
**       message ID value, and default headers assigns 0xFFFF as the invalid value.  This
**       means for default headers, 0xFFFF is invalid even if you set the value
**       below to it's maximum of 0xFFFF.
**       The allocated message table is this size + 1 (could change based on implementaiton).
*/
#define CFE_PLATFORM_SB_HIGHEST_VALID_MSGID      0x1FFF

/**
**  \cfesbcfg Platform Endian Indicator
**
**  \par Description:
**       The value of this constant indicates the endianess of the target system
**
**  \par Limits
**       This parameter has a lower limit of 0 and an upper limit of 1.
*/
#define CFE_PLATFORM_ENDIAN CCSDS_LITTLE_ENDIAN

/**
**  \cfesbcfg Default Routing Information Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the software
**       bus routing information.  This filename is used only when no filename is
**       specified in the command.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_SB_DEFAULT_ROUTING_FILENAME         "/ram/cfe_sb_route.dat"


/**
**  \cfesbcfg Default Pipe Information Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the software
**       bus pipe information. This filename is used only when no filename is
**       specified in the command.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_SB_DEFAULT_PIPE_FILENAME            "/ram/cfe_sb_pipe.dat"


/**
**  \cfesbcfg Default Message Map Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the software
**       bus message map information. This filename is used only when no filename is
**       specified in the command. The message map is a lookup table (array of 16bit
**       words) that has an element for each possible MsgId value and holds the
**       routing table index for that MsgId. The Msg Map provides fast access to the
**       destinations of a message.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_SB_DEFAULT_MAP_FILENAME             "/ram/cfe_sb_msgmap.dat"


/**
**  \cfesbcfg SB Event Filtering
**
**  \par Description:
**       This group of configuration paramters dictates what SB events will be
**       filtered through EVS. The filtering will begin after the SB task initializes
**       and stay in effect until a cmd to EVS changes it.
**       This allows the operator to set limits on the number of event messages that
**       are sent during system initialization.
**       NOTE: Set all unused event values and mask values to zero
**
**  \par Limits
**       This filtering applies only to SB events.
**       These parameters have a lower limit of 0 and an upper limit of 65535.
*/
#define CFE_PLATFORM_SB_FILTERED_EVENT1    CFE_SB_SEND_NO_SUBS_EID
#define CFE_PLATFORM_SB_FILTER_MASK1       CFE_EVS_FIRST_4_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT2    CFE_SB_DUP_SUBSCRIP_EID
#define CFE_PLATFORM_SB_FILTER_MASK2       CFE_EVS_FIRST_4_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT3    CFE_SB_MSGID_LIM_ERR_EID
#define CFE_PLATFORM_SB_FILTER_MASK3       CFE_EVS_FIRST_16_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT4    CFE_SB_Q_FULL_ERR_EID
#define CFE_PLATFORM_SB_FILTER_MASK4       CFE_EVS_FIRST_16_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT5    0
#define CFE_PLATFORM_SB_FILTER_MASK5       CFE_EVS_NO_FILTER

#define CFE_PLATFORM_SB_FILTERED_EVENT6    0
#define CFE_PLATFORM_SB_FILTER_MASK6       CFE_EVS_NO_FILTER

#define CFE_PLATFORM_SB_FILTERED_EVENT7    0
#define CFE_PLATFORM_SB_FILTER_MASK7       CFE_EVS_NO_FILTER

#define CFE_PLATFORM_SB_FILTERED_EVENT8    0
#define CFE_PLATFORM_SB_FILTER_MASK8       CFE_EVS_NO_FILTER


/**
**  \cfeescfg Define SB Memory Pool Block Sizes
**
**  \par Description:
**       Software Bus Memory Pool Block Sizes
**
**  \par Limits
**       These sizes MUST be increasing and MUST be an integral multiple of 4.
**       The number of block sizes defined cannot exceed
**       #CFE_ES_MAX_MEMPOOL_BLOCK_SIZES
*/
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_01              8
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_02             16
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_03             20
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_04             36
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_05             64
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_06             96
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_07            128
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_08            160
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_09            256
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_10            512
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_11           1024
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_12           2048
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_13           4096
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_14           8192
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_15          16384
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_16          32768
#define CFE_PLATFORM_SB_MAX_BLOCK_SIZE             (CFE_MISSION_SB_MAX_SB_MSG_SIZE + 40)

/**
**  \cfesbcfg Define Default Sender Information Storage Mode
**
**  \par Description:
**       Defines the default mode for the storing of sender information when sending
**       a software bus message. If set to 1, the sender information will be stored.
**       If set to 0, the sender information will not be stored.
**
**  \par Limits
**       There is a lower limit of 0 and an upper limit of 1 on this configuration
**       paramater.
*/
#define CFE_PLATFORM_SB_DEFAULT_REPORT_SENDER      1


/**
**  \cfetimecfg Time Server or Time Client Selection
**
**  \par Description:
**       This configuration parameter selects whether the Time task functions as a
**       time "server" or "client".  A time server generates the "time at the tone"
**       packet which is received by time clients.
**
**  \par Limits
**       Enable one, and only one by defining either CFE_PLATFORM_TIME_CFG_SERVER or
**       CFE_PLATFORM_TIME_CFG_CLIENT AS true.  The other must be defined as false.
*/
#define CFE_PLATFORM_TIME_CFG_SERVER  true
#define CFE_PLATFORM_TIME_CFG_CLIENT  false


/**
** \cfetimecfg Time Tone In Big-Endian Order
**
** \par Description:
**      If this configuration parameter is defined, the CFE time server will
**      publish time tones with payloads in big-endian order, and time clients
**      will expect the tones to be in big-endian order. This is useful for
**      mixed-endian environments. This will become obsolete once EDS is
**      available and the CFE time tone message is defined.
*/
#undef CFE_PLATFORM_TIME_CFG_BIGENDIAN


/**
**  \cfetimecfg Local MET or Virtual MET Selection for Time Servers
**
**  \par Description:
**       Depending on the specific hardware system configuration, it may be possible
**       for Time Servers to read the "local" MET from a h/w register rather than
**       having to track the MET as the count of tone signal interrupts (virtual MET)
**
**       Time Clients must be defined as using a virtual MET.  Also, a Time Server
**       cannot be defined as having both a h/w MET and an external time source (they
**       both cannot synchronize to the same tone).
**
**       Note: "disable" this define (set to false) only for Time Servers with local hardware
**       that supports a h/w MET that is synchronized to the tone signal !!!
**
**  \par Limits
**       Only applies if #CFE_PLATFORM_TIME_CFG_SERVER is set to true.
*/
#define CFE_PLATFORM_TIME_CFG_VIRTUAL  true


/**
**  \cfetimecfg Include or Exclude the Primary/Redundant Tone Selection Cmd
**
**  \par Description:
**       Depending on the specific hardware system configuration, it may be possible
**       to switch between a primary and redundant tone signal.  If supported by
**       hardware, this definitions will enable command interfaces to select the
**       active tone signal. Both Time Clients and Time Servers support this feature.
**       Note: Set the CFE_PLATFORM_TIME_CFG_SIGNAL define to true to enable tone signal commands.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TIME_CFG_SIGNAL  false


/**
**  \cfetimecfg Include or Exclude the Internal/External Time Source Selection Cmd
**
**  \par Description:
**       By default, Time Servers maintain time using an internal MET which may be a
**       h/w register or software counter, depending on available hardware. The
**       following definition enables command interfaces to switch between an
**       internal MET, or external time data received from one of several supported
**       external time sources. Only a Time Server may be configured to use external
**       time data.
**       Note: Set the CFE_PLATFORM_TIME_CFG_SOURCE define to true to include the Time Source
**             Selection Command (command allows selection between the internal
**             or external time source). Then choose the external source with the
**             CFE_TIME_CFG_SRC_??? define.
**
**  \par Limits
**       Only applies if #CFE_PLATFORM_TIME_CFG_SERVER is set to true.
*/
#define CFE_PLATFORM_TIME_CFG_SOURCE  false


/**
**  \cfetimecfg Choose the External Time Source for Server only
**
**  \par Description:
**       If #CFE_PLATFORM_TIME_CFG_SOURCE is set to true, then one of the following external time
**       source types must also be set to true.  Do not set any of the external time
**       source types to true unless #CFE_PLATFORM_TIME_CFG_SOURCE is set to true.
**
**  \par Limits
**       -# If #CFE_PLATFORM_TIME_CFG_SOURCE is set to true then one and only one of the following
**       three external time sources can and must be set true:
**       #CFE_PLATFORM_TIME_CFG_SRC_MET, #CFE_PLATFORM_TIME_CFG_SRC_GPS, #CFE_PLATFORM_TIME_CFG_SRC_TIME
**       -# Only applies if #CFE_PLATFORM_TIME_CFG_SERVER is set to true.
*/
#define CFE_PLATFORM_TIME_CFG_SRC_MET   false
#define CFE_PLATFORM_TIME_CFG_SRC_GPS   false
#define CFE_PLATFORM_TIME_CFG_SRC_TIME  false


/**
**  \cfetimecfg Define the Max Delta Limits for Time Servers using an Ext Time Source
**
**  \par Description:
**       If #CFE_PLATFORM_TIME_CFG_SOURCE is set to true and one of the external time sources is
**       also set to true, then the delta time limits for range checking is used.
**
**       When a new time value is received from an external source, the value is
**       compared against the "expected" time value. If the delta exceeds the
**       following defined amount, then the new time data will be ignored. This range
**       checking is only performed after the clock state has been commanded to
**       "valid". Until then, external time data is accepted unconditionally.
**
**  \par Limits
**       Applies only if both #CFE_PLATFORM_TIME_CFG_SERVER and #CFE_PLATFORM_TIME_CFG_SOURCE are set
**       to true.
*/
#define CFE_PLATFORM_TIME_MAX_DELTA_SECS       0
#define CFE_PLATFORM_TIME_MAX_DELTA_SUBS  500000


/**
**  \cfetimecfg Define the Local Clock Rollover Value in seconds and subseconds
**
**  \par Description:
**       Specifies the capability of the local clock.  Indicates the time at which
**       the local clock rolls over.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TIME_MAX_LOCAL_SECS  27
#define CFE_PLATFORM_TIME_MAX_LOCAL_SUBS   0


/**
**  \cfetimecfg Define Timing Limits From One Tone To The Next
**
**  \par Description:
**       Defines limits to the timing of the 1Hz tone signal. A tone signal is valid
**       only if it arrives within one second (plus or minus the tone limit) from
**       the previous tone signal.Units are microseconds as measured with the local
**       clock.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TIME_CFG_TONE_LIMIT  20000



/**
**  \cfetimecfg Define Time to Start Flywheel Since Last Tone
**
**  \par Description:
**       Define time to enter flywheel mode (in seconds since last tone data update)
**       Units are microseconds as measured with the local clock.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TIME_CFG_START_FLY   2


/**
**  \cfetimecfg Define Periodic Time to Update Local Clock Tone Latch
**
**  \par Description:
**       Define Periodic Time to Update Local Clock Tone Latch. Applies only when
**       in flywheel mode. This define dicates the period at which the simulated
**       'last tone' time is updated. Units are seconds.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TIME_CFG_LATCH_FLY   8


/**
**  \cfeescfg Define Max Number of Applications
**
**  \par Description:
**       Defines the maximum number of applications that can be loaded into the
**       system. This number does not include child tasks.
**
**  \par Limits
**       There is a lower limit of 6.  The lower limit corresponds to the cFE internal
**       applications.  There are no restrictions on the upper limit however, the
**       maximum number of applications is system dependent and should be verified.
**       AppIDs that are checked against this configuration are defined by a 32 bit
**       data word.
*/
#define CFE_PLATFORM_ES_MAX_APPLICATIONS 32


/**
**  \cfeescfg Define Max Number of Shared libraries
**
**  \par Description:
**       Defines the maximum number of cFE Shared libraries that can be loaded into
**       the system.
**
**  \par Limits
**       There is a lower limit of 1.  There are no restrictions on the upper limit
**       however, the maximum number of libraries is system dependent and should be
**       verified.
*/
#define CFE_PLATFORM_ES_MAX_LIBRARIES 10

/**
**  \cfeescfg Define Max Number of ER (Exception and Reset) log entries
**
**  \par Description:
**       Defines the maximum number of ER (Exception and Reset) log entries
**
**  \par Limits
**       There is a lower limit of 1.  There are no restrictions on the upper limit
**       however, the maximum number of log entries is system dependent and should be
**       verified.
*/
#define CFE_PLATFORM_ES_ER_LOG_ENTRIES 20

/** \cfeescfg Maximum size of CPU Context in ES Error Log
**
**  \par Description:
**       This should be large enough to accommodate the CPU context
**       information supplied by the PSP on the given platform.
**
**  \par Limits:
**       Must be greater than zero and a multiple of sizeof(uint32).
**       Limited only by the available memory and the number of entries
**       in the error log. Any context information beyond this size will
**       be truncated.
*/
#define CFE_PLATFORM_ES_ER_LOG_MAX_CONTEXT_SIZE     256


/**
**  \cfeescfg Define Size of the cFE System Log.
**
**  \par Description:
**       Defines the size in bytes of the cFE system log. The system log holds
**       variable length strings that are terminated by a linefeed and null
**       character.
**
**  \par Limits
**       There is a lower limit of 512.  There are no restrictions on the upper limit
**       however, the maximum system log size is system dependent and should be
**       verified.
*/
#define CFE_PLATFORM_ES_SYSTEM_LOG_SIZE  3072


/**
**  \cfeescfg Define Number of entries in the ES Object table
**
**  \par Description:
**       Defines the number of entries in the ES Object table. This table controls
**       the core cFE startup.
**
**  \par Limits
**       There is a lower limit of 15.  There are no restrictions on the upper limit
**       however, the maximum object table size is system dependent and should be
**       verified.
*/
#define CFE_PLATFORM_ES_OBJECT_TABLE_SIZE  30


/**
**  \cfeescfg Define Max Number of Generic Counters
**
**  \par Description:
**       Defines the maximum number of Generic Counters that can be registered.
**
**  \par Limits
**       This parameter has a lower limit of 1 and an upper limit of 65535.
*/
#define CFE_PLATFORM_ES_MAX_GEN_COUNTERS    8


/**
**  \cfeescfg Define ES Application Control Scan Rate
**
**  \par Description:
**       ES Application Control Scan Rate. This parameter controls the speed that ES
**       scans the Application Table looking for App Delete/Restart/Reload requests.
**       All Applications are deleted, restarted, or reloaded by the ES Application.
**       ES will periodically scan for control requests to process. The scan rate is
**       controlled by this parameter, which is given in milliseconds. A value of
**       1000 means that ES will scan the Application Table once per second. Be
**       careful not to set the value of this too low, because ES will use more CPU
**       cycles scanning the table.
**
**  \par Limits
**       There is a lower limit of 100 and an upper limit of 20000 on this
**       configuration paramater. millisecond units.
*/
#define CFE_PLATFORM_ES_APP_SCAN_RATE 1000


/**
**  \cfeescfg Define ES Application Kill Timeout
**
**  \par Description:
**      ES Application Kill Timeout. This parameter controls the number of
**      "scan periods" that ES will wait for an application to Exit after getting
**      the signal Delete, Reload or Restart. The sequence works as follows:
**      -# ES will set the control request for an App to Delete/Restart/Reload and
**         set this kill timer to the value in this parameter.
**      -# If the App is reponding and Calls it's RunLoop function, it will drop out
**         of it's main loop and call CFE_ES_ExitApp. Once it calls Exit App, then
**         ES can delete, restart, or reload the app the next time it scans the app
**         table.
**      -# If the App is not responding, the ES App will decrement this Kill Timeout
**         value each time it runs. If the timeout value reaches zero, ES will kill
**         the app.
**
**      The Kill timeout value depends on the #CFE_PLATFORM_ES_APP_SCAN_RATE. If the Scan Rate
**      is 1000, or 1 second, and this #CFE_PLATFORM_ES_APP_KILL_TIMEOUT is set to 5, then it
**      will take 5 seconds to kill a non-responding App.
**      If the Scan Rate is 250, or 1/4 second, and the #CFE_PLATFORM_ES_APP_KILL_TIMEOUT is
**      set to 2, then it will take 1/2 second to time out.
**
**  \par Limits
**       There is a lower limit of 1 and an upper limit of 100 on this configuration
**       paramater. Units are number of #CFE_PLATFORM_ES_APP_SCAN_RATE cycles.
*/
#define CFE_PLATFORM_ES_APP_KILL_TIMEOUT 5


/**
**  \cfeescfg ES Ram Disk Sector Size
**
**  \par Description:
**       Defines the ram disk sector size. The ram disk is 1 of 4 memory areas that
**       are preserved on a processor reset.
**       NOTE: Changing this value changes memory allocation, and may
**       require changes to platform specific values (in CFE_PSP) such as
**       USER_RESERVED_MEM in VxWorks depending on the memory areas
**       being used for preserved data and on OS specific behavior.
**
**  \par Limits
**       There is a lower limit of 128.  There are no restrictions on the upper limit
**       however, the maximum RAM disk sector size is system dependent and should be
**       verified.
*/
#define CFE_PLATFORM_ES_RAM_DISK_SECTOR_SIZE      512


/**
**  \cfeescfg ES Ram Disk Number of Sectors
**
**  \par Description:
**       Defines the ram disk number of sectors. The ram disk is one of four memory
**       areas that are preserved on a processor reset.
**       NOTE: Changing this value changes memory allocation, and may
**       require changes to platform specific values (in CFE_PSP) such as
**       USER_RESERVED_MEM in VxWorks depending on the memory areas
**       being used for preserved data and on OS specific behavior.
**
**  \par Limits
**       There is a lower limit of 128.  There are no restrictions on the upper limit
**       however, the maximum number of RAM sectors is system dependent and should be
**       verified.
*/
#define CFE_PLATFORM_ES_RAM_DISK_NUM_SECTORS      4096

/**
**  \cfeescfg Percentage of Ram Disk Reserved for Decompressing Apps
**
**  \par Description:
**      The #CFE_PLATFORM_ES_RAM_DISK_PERCENT_RESERVED parameter is used to make sure that the
**      Volatile ( RAM ) Disk has a defined amount of free space during a processor
**      reset. The cFE uses the Volatile disk to decompress cFE applications during
**      system startup. If this Volatile disk happens to get filled with logs and
**      misc files, then a processor reset may not work, because there will be no
**      room to decompress cFE apps. To solve that problem, this parameter sets the
**      "Low Water Mark" for disk space on a Processor reset. It should be set to
**      allow the largest cFE Application to be decompressed.
**      During a Processor reset, if there is not sufficient space left on the disk,
**      it will be re-formatted in order to clear up some space.
**
**      This feature can be turned OFF by setting the parameter to 0.
**
**  \par Limits
**       There is a lower limit of 0 and an upper limit of 75 on this configuration
**       paramater.Units are percentage. A setting of zero will turn this feature
**       off.
*/
#define CFE_PLATFORM_ES_RAM_DISK_PERCENT_RESERVED 30


/**
**  \cfeescfg RAM Disk Mount string
**
**  \par Description:
**      The #CFE_PLATFORM_ES_RAM_DISK_MOUNT_STRING parameter is used to set the cFE mount path
**      for the CFE RAM disk. This is a parameter for missions that do not want to
**      use the default value of "/ram", or for missions that need to have a different
**      value for different CPUs or Spacecraft.
**      Note that the vxWorks OSAL cannot currently handle names that have more than one
**      path separator in it. The names "/ram", "/ramdisk", "/disk123" will all work, but
**      "/disks/ram" will not.
**      Multiple separators can be used with the posix or RTEMS ports.
**
*/
#define CFE_PLATFORM_ES_RAM_DISK_MOUNT_STRING "/ram"


/**
**  \cfeescfg Define Critical Data Store Size
**
**  \par Description:
**       Defines the Critical Data Store (CDS) area size in bytes size. The CDS is
**       one of four memory areas that are preserved during a processor reset.
**       NOTE: Changing this value changes memory allocation, and may
**       require changes to platform specific values (in CFE_PSP) such as
**       USER_RESERVED_MEM in VxWorks depending on the memory areas
**       being used for preserved data and on OS specific behavior.
**
**  \par Limits
**       There is a lower limit of 8192 and an upper limit of UINT_MAX (4 Gigabytes)
**       on this configuration paramater.
*/
#define CFE_PLATFORM_ES_CDS_SIZE                  ( 128 * 1024 )


/**
**  \cfeescfg Define User Reserved Memory Size
**
**  \par Description:
**       User Reserved Memory Size. This is the size in bytes of the cFE User
**       reserved Memory area. This is a block of memory that is available for cFE
**       application use. The address is obtained by calling
**       #CFE_PSP_GetUserReservedArea. The User Reserved Memory is one of four memory
**       areas that are preserved during a processor reset.
**       NOTE: Changing this value changes memory allocation, and may
**       require changes to platform specific values (in CFE_PSP) such as
**       USER_RESERVED_MEM in VxWorks depending on the memory areas
**       being used for preserved data and on OS specific behavior.
**
**  \par Limits
**       There is a lower limit of 1024 and an upper limit of UINT_MAX (4 Gigabytes)
**       on this configuration paramater.
*/
#define CFE_PLATFORM_ES_USER_RESERVED_SIZE         ( 1024 * 1024 )


/**
**  \cfeescfg Define ES Reset Area Size
**
**  \par Description:
**       The ES Reset Area Size. This is the size in bytes of the cFE Reset variable
**       and log area. This is a block of memory used by the cFE to store the system
**       log ER Log and critical reset variables. This is 4 of 4 of the memory areas
**       that are preserved during a processor reset.
**       Note: This area must be sized large enough to hold all of the data
**       structures. It should be automatically sized based on the CFE_ES_ResetData_t
**       type, but circular dependancies in the headers prevent it from being defined
**       this way.
**       NOTE: Changing this value changes memory allocation, and may
**       require changes to platform specific values (in CFE_PSP) such as
**       USER_RESERVED_MEM in VxWorks depending on the memory areas
**       being used for preserved data and on OS specific behavior.
**
**  \par Limits
**       There is a lower limit of 153600 (150KBytes) and an upper limit of UINT_MAX
**       (4 Gigabytes) on this configuration paramater.
*/
#define CFE_PLATFORM_ES_RESET_AREA_SIZE  ( 170 * 1024 )

/**
**  \cfeescfg Define Memory Pool Alignment Size
**
**  \par Description:
**       Ensures that buffers obtained from a memory pool are aligned
**       to a certain minimum block size.  Note the allocator will always
**       align to the minimum required by the CPU architecture.  This may
**       be set greater than the CPU requirement as desired for optimal
**       performance.
**
**       For some architectures/applications it may be beneficial to set this
**       to the cache line size of the target CPU, or to use special SIMD
**       instructions that require a more stringent memory alignment.
**
**  \par Limits
**       This must always be a power of 2, as it is used as a binary address mask.
*/
#define CFE_PLATFORM_ES_MEMPOOL_ALIGN_SIZE_MIN   4


/**
**  \cfeescfg ES Nonvolatile Startup Filename
**
**  \par Description:
**       The value of this constant defines the path and name of the file that
**       contains a list of modules that will be loaded and started by the cFE after
**       the cFE finishes its startup sequence.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_NONVOL_STARTUP_FILE    "/cf/cfe_es_startup.scr"


/**
**  \cfeescfg ES Volatile Startup Filename
**
**  \par Description:
**       The value of this constant defines the path and name of the file that
**       contains a list of modules that will be loaded and started by the cFE after
**       the cFE finishes its startup sequence.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_VOLATILE_STARTUP_FILE  "/ram/cfe_es_startup.scr"

/**
**  \cfeescfg Default Shell Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the shell
**       output after a shell command is received by ES. This file contains the
**       entire shell output. The fsw also sends the shell output in series of fixed
**       size telemetry packets. This filename is used only when no filename
**       is specified in the shell command.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_DEFAULT_SHELL_FILENAME  "/ram/ShellCmd.out"


/**
**  \cfeescfg Define Max Shell Command Size
**
**  \par Description:
**       Defines the maximum size in characters of the shell command.
**
**  \par Limits
**       There is a lower limit of 64 and an upper limit of #OS_MAX_CMD_LEN. Units are
**       characters.
*/
#define CFE_PLATFORM_ES_MAX_SHELL_CMD  64


/**
**  \cfeescfg Define Shell Command Telemetry Pkt Segment Size
**
**  \par Description:
**       Defines the size of the shell command tlm packet segments.The shell command
**       output size is dependant on the shell command itself. If the shell output
**       size is greater than the size of the packet defined here, the fsw will
**       generate a series of tlm packets (of the size defined here) that can be
**       reconstructed by the ground system.
**
**  \par Limits
**       There is a lower limit of 32 and an upper limit of #CFE_SB_MAX_SB_MSG_SIZE.
*/
#define CFE_PLATFORM_ES_MAX_SHELL_PKT    64

/**
**  \cfeescfg Define OS Task Delay Value for ES Shell Command
**
**  \par Description:
**       This parameter defines the length of time (in milliseconds) ES will 
**       delay when sending shell command packets over the software bus to not 
**       flood the pipe on large messages.
** 
**       Note: The milliseconds passed into OS_TaskDelay are converted into the 
**       units the underlying OS uses to measure time passing.  Many platforms 
**       limit the precision of this value however, a delay may not be
**       needed at all in which the value may be set to zero.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_ES_SHELL_OS_DELAY_MILLISEC   200

/**
**  \cfeescfg Default Application Information Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store information
**       pertaining to all of the Applications that are registered with Executive
**       Services. This filename is used only when no filename is specified in the
**       the command to query all system apps.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_DEFAULT_APP_LOG_FILE   "/ram/cfe_es_app_info.log"

/**
**  \cfeescfg Default Application Information Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store information
**       pertaining to all of the Applications that are registered with Executive
**       Services. This filename is used only when no filename is specified in the
**       the command to query all system tasks.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_DEFAULT_TASK_LOG_FILE   "/ram/cfe_es_task_info.log"

/**
**  \cfeescfg Default System Log Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store important
**       information (as ASCII text strings) that might not be able to be sent in an
**       Event Message. This filename is used only when no filename is specified in
**       the command to dump the system log. No file specified in the cmd means the
**       first character in the cmd filename is a NULL terminator (zero).
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_DEFAULT_SYSLOG_FILE   "/ram/cfe_es_syslog.log"

/**
**  \cfeescfg Default Exception and Reset (ER) Log Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the
**       Exception and Reset (ER) Log. This filename is used only when no filename is
**       specified in the command to dump the ER log. No file specified in the cmd
**       means the first character in the cmd filename is a NULL terminator (zero).
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_DEFAULT_ER_LOG_FILE   "/ram/cfe_erlog.log"

/**
**  \cfeescfg Default Performance Data Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the
**       Performance Data. This filename is used only when no filename is specified
**       in the command to stop performance data collecting.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_DEFAULT_PERF_DUMP_FILENAME    "/ram/cfe_es_perf.dat"


/**
**  \cfeescfg Default Critical Data Store Registry Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the
**       Critical Data Store Registry. This filename is used only when no filename is
**       specified in the command to stop performance data collecting.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_ES_DEFAULT_CDS_REG_DUMP_FILE     "/ram/cfe_cds_reg.log"

/**
**  \cfeescfg Define Default System Log Mode following Power On Reset
**
**  \par Description:
**       Defines the default mode for the operation of the ES System log following a power
**       on reset. The log may operate in either Overwrite mode = 0, where once the
**       log becomes full the oldest message in the log will be overwritten, or
**       Discard mode = 1, where once the log becomes full the contents of the log are
**       preserved and the new event is discarded.  This constant may hold a value of
**       either 0 or 1 depending on the desired default.
**       Overwrite Mode = 0, Discard Mode = 1.
**
**  \par Limits
**       There is a lower limit of 0 and an upper limit of 1 on this configuration
**       paramater.
*/
#define CFE_PLATFORM_ES_DEFAULT_POR_SYSLOG_MODE      0

/**
**  \cfeescfg Define Default System Log Mode following Processor Reset
**
**  \par Description:
**       Defines the default mode for the operation of the ES System log following a
**       processor reset. The log may operate in either Overwrite mode = 0, where once
**       the log becomes full the oldest message in the log will be overwritten, or
**       Discard mode = 1, where once the log becomes full the contents of the log are
**       preserved and the new event is discarded.  This constant may hold a value of
**       either 0 or 1 depending on the desired default.
**       Overwrite Mode = 0, Discard Mode = 1.
**
**  \par Limits
**       There is a lower limit of 0 and an upper limit of 1 on this configuration
**       paramater.
*/
#define CFE_PLATFORM_ES_DEFAULT_PR_SYSLOG_MODE      1

/**
**  \cfeescfg Define Max Number of Performance IDs
**
**  \par Description:
**       Defines the maximum number of perf ids allowed.
**
**
**  \par Limits
**       This number must always be divisible by 32. There is a lower limit of 32 and
**       an upper limit of 512 on this configuration paramater.
*/
#define CFE_PLATFORM_ES_PERF_MAX_IDS                  128

/**
**  \cfeescfg Define Max Size of Performance Data Buffer
**
**  \par Description:
**       Defines the maximum size of the performance data buffer. Units are number of
**       performance data entries. An entry is defined by a 32 bit data word followed
**       by a 64 bit time stamp.
**
**  \par Limits
**       There is a lower limit of 1025.  There are no restrictions on the upper limit
**       however, the maximum buffer size size is system dependent and should be verified.
**       The units are number of entries. An entry is defined by a 32 bit data word followed
**       by a 64 bit time stamp.
*/
#define CFE_PLATFORM_ES_PERF_DATA_BUFFER_SIZE           10000


/**
**  \cfeescfg Define Filter Mask Setting for Disabling All Performance Entries
**
**  \par Description:
**       Defines the filter mask for disabling all performance entries. The value is a
**       bit mask.  For each bit, 0 means the corresponding entry is disabled and 
**       1 means it is enabled.
*/
#define CFE_PLATFORM_ES_PERF_FILTMASK_NONE              0

/**
**  \cfeescfg Define Filter Mask Setting for Enabling All Performance Entries
**
**  \par Description:
**       Defines the filter mask for enabling all performance entries. The value is a
**       bit mask.  For each bit, 0 means the corresponding entry is disabled and 
**       1 means it is enabled.
*/
#define CFE_PLATFORM_ES_PERF_FILTMASK_ALL               ~CFE_PLATFORM_ES_PERF_FILTMASK_NONE

/**
**  \cfeescfg Define Default Filter Mask Setting for Performance Data Buffer
**
**  \par Description:
**       Defines the default filter mask for the performance data buffer. The value is a
**       bit mask.  For each bit, 0 means the corresponding entry is disabled and 1 
**       means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_FILTMASK_INIT              CFE_PLATFORM_ES_PERF_FILTMASK_ALL


/**
**  \cfeescfg Define Default Filter Trigger Setting for Disabling All Performance Entries
**
**  \par Description:
**       Defines the default trigger mask for disabling all performance data entries. The value 
**       is a bit mask.  For each bit, 0 means the trigger for the corresponding entry is 
**       disabled and 1 means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_TRIGMASK_NONE              0

/**
**  \cfeescfg Define Filter Trigger Setting for Enabling All Performance Entries
**
**  \par Description:
**       Defines the trigger mask for enabling all performance data entries. The value is 
**       a bit mask.  For each bit, 0 means the trigger for the corresponding entry is 
**       disabled and 1 means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_TRIGMASK_ALL               ~CFE_PLATFORM_ES_PERF_TRIGMASK_NONE

/**
**  \cfeescfg Define Default Filter Trigger Setting for Performance Data Buffer
**
**  \par Description:
**       Defines the default trigger mask for the performance data buffer. The value is a
**       32-bit mask.  For each bit, 0 means the trigger for the corresponding entry is 
**       disabled and 1 means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_TRIGMASK_INIT              CFE_PLATFORM_ES_PERF_TRIGMASK_NONE

/**
**  \cfeescfg Define Performance Analyzer Child Task Priority
**
**  \par Description:
**       This parameter defines the priority of the child task spawed by the
**       Executive Services to write performance data to a file.  Lower numbers 
**       are higher priority, with 1 being the highest priority in the case of a 
**       child task. 
**
**  \par Limits
**       Valid range for a child task is 1 to 255 however, the priority cannot
**       be higher (lower number) than the ES parent application priority.
*/
#define CFE_PLATFORM_ES_PERF_CHILD_PRIORITY                200

/**
**  \cfeescfg Define Performance Analyzer Child Task Stack Size
**
**  \par Description:
**       This parameter defines the stack size of the child task spawed by the
**       Executive Services to write performance data to a file.  
**
**  \par Limits
**       It is recommended this parameter be greater than or equal to 4KB. This parameter
**       is limited by the maximum value allowed by the data type. In this case, the data
**       type is an unsigned 32-bit integer, so the valid range is 0 to 0xFFFFFFFF.
*/
#define CFE_PLATFORM_ES_PERF_CHILD_STACK_SIZE              4096

/**
**  \cfeescfg Define Performance Analyzer Child Task Delay
**
**  \par Description:
**       This parameter defines the delay time (in milliseconds) between performance
**       data file writes performed by the Executive Services Performace Analyzer
**       Child Task.   
**
**  \par Limits
**       It is recommended this parameter be greater than or equal to 20ms. This parameter
**       is limited by the maximum value allowed by the data type. In this case, the data
**       type is an unsigned 32-bit integer, so the valid range is 0 to 0xFFFFFFFF.
*/
#define CFE_PLATFORM_ES_PERF_CHILD_MS_DELAY                20

/**
**  \cfeescfg Define Performance Analyzer Child Task Number of Entries Between Delay
**
**  \par Description:
**       This parameter defines the number of performace analyzer entries the Performace 
**       Analyzer Child Task will write to the file between delays.  
**
*/
#define CFE_PLATFORM_ES_PERF_ENTRIES_BTWN_DLYS             50

/**
**  \cfeescfg Define Default Stack Size for an Application
**
**  \par Description:
**       This parameter defines a default stack size. This parameter is used by the
**       cFE Core Applications.
**
**  \par Limits
**       There is a lower limit of 2048.  There are no restrictions on the upper limit
**       however, the maximum stack size size is system dependent and should be verified.
**       Most operating systems provide tools for measuring the amount of stack used by a
**       task during operation. It is always a good idea to verify that no more than 1/2
**       of the stack is used.
*/
#define CFE_PLATFORM_ES_DEFAULT_STACK_SIZE 8192

/**
**  \cfeescfg Define EVS Task Priority
**
**  \par Description:
**       Defines the cFE_EVS Task priority.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_EVS_START_TASK_PRIORITY               61

/**
**  \cfeescfg Define EVS Task Stack Size
**
**  \par Description:
**       Defines the cFE_EVS Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration paramater.  There
**       are no restrictions on the upper limit however, the maximum stack size size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_EVS_START_TASK_STACK_SIZE             CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

/**
**  \cfeescfg Define SB Task Priority
**
**  \par Description:
**       Defines the cFE_SB Task priority.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_SB_START_TASK_PRIORITY                64

/**
**  \cfeescfg Define SB Task Stack Size
**
**  \par Description:
**       Defines the cFE_SB Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration paramater.  There
**       are no restrictions on the upper limit however, the maximum stack size size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_SB_START_TASK_STACK_SIZE              CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

/**
**  \cfeescfg Define ES Task Priority
**
**  \par Description:
**       Defines the cFE_ES Task priority.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_ES_START_TASK_PRIORITY                68

/**
**  \cfeescfg Define ES Task Stack Size
**
**  \par Description:
**       Defines the cFE_ES Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration paramater.  There
**       are no restrictions on the upper limit however, the maximum stack size size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_ES_START_TASK_STACK_SIZE             CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

/**
**  \cfetimecfg Define TIME Task Priorities
**
**  \par Description:
**       Defines the cFE_TIME Task priority.
**       Defines the cFE_TIME Tone Task priority.
**       Defines the cFE_TIME 1HZ Task priority.
**
**  \par Limits
**       There is a lower limit of zero and an upper limit of 255 on these
**       configuration paramaters.  Remember that the meaning of each task
**       priority is inverted -- a "lower" number has a "higher" priority.
*/
#define CFE_PLATFORM_TIME_START_TASK_PRIORITY              60
#define CFE_PLATFORM_TIME_TONE_TASK_PRIORITY               25
#define CFE_PLATFORM_TIME_1HZ_TASK_PRIORITY                25

/**
**  \cfetimecfg Define TIME Task Stack Sizes
**
**  \par Description:
**       Defines the cFE_TIME Main Task Stack Size
**       Defines the cFE_TIME Tone Task Stack Size
**       Defines the cFE_TIME 1HZ Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on these configuration paramaters.  There
**       are no restrictions on the upper limit however, the maximum stack size size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_TIME_START_TASK_STACK_SIZE            CFE_PLATFORM_ES_DEFAULT_STACK_SIZE
#define CFE_PLATFORM_TIME_TONE_TASK_STACK_SIZE             4096
#define CFE_PLATFORM_TIME_1HZ_TASK_STACK_SIZE              8192

/**
**  \cfeescfg Define TBL Task Priority
**
**  \par Description:
**       Defines the cFE_TBL Task priority.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TBL_START_TASK_PRIORITY               70

/**
**  \cfeescfg Define TBL Task Stack Size
**
**  \par Description:
**       Defines the cFE_TBL Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration paramater.  There
**       are no restrictions on the upper limit however, the maximum stack size size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_TBL_START_TASK_STACK_SIZE             CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

/**
**  \cfeescfg Define Maximum Number of Registered CDS Blocks
**
**  \par Description:
**       Maximum number of registered CDS Blocks
**
**  \par Limits
**       There is a lower limit of 8.  There are no restrictions on the upper limit
**       however, the maximum number of CDS entries is system dependent and
**       should be verified.
*/
#define CFE_PLATFORM_ES_CDS_MAX_NUM_ENTRIES           512


/**
**  \cfeescfg Define Number of Processor Resets Before a Power On Reset
**
**  \par Description:
**       Number of Processor Resets before a Power On Reset is called. If set to 2,
**       then 2 processor resets will occur, and the 3rd processor reset will be a
**       power on reset instead.
**
**  \par Limits
**       There is a lower limit of 0.  There are no restrictions on the upper limit
**       however, the maximum number of processor resets may be system dependent and
**       should be verified.
*/
#define CFE_PLATFORM_ES_MAX_PROCESSOR_RESETS           2


/**
**  \cfeescfg Define Default ES Memory Pool Block Sizes
**
**  \par Description:
**       Default Intermediate ES Memory Pool Block Sizes.  If an application
**       is using the CFE_ES Memory Pool APIs (#CFE_ES_PoolCreate, #CFE_ES_PoolCreateNoSem,
**       #CFE_ES_GetPoolBuf and #CFE_ES_PutPoolBuf) but finds these sizes
**       inappropriate for their use, they may wish to use the #CFE_ES_PoolCreateEx
**       API to specify their own intermediate block sizes
**
**  \par Limits
**       These sizes MUST be increasing and MUST be an integral multiple of 4.  Also,
**       CFE_PLATFORM_ES_MAX_BLOCK_SIZE must be larger than CFE_MISSION_SB_MAX_SB_MSG_SIZE and both
**       CFE_PLATFORM_TBL_MAX_SNGL_TABLE_SIZE and CFE_PLATFORM_TBL_MAX_DBL_TABLE_SIZE.  Note that if Table
**       Services have been removed from the CFE, the table size limits are still
**       enforced although the table size definitions may be reduced.  Refer to the CFS
**       Deployment Guide for information about removing CFE Table Services from the CFE.
*/
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_01              8
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_02             16
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_03             32
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_04             48
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_05             64
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_06             96
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_07            128
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_08            160
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_09            256
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_10            512
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_11           1024
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_12           2048
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_13           4096
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_14           8192
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_15          16384
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_16          32768
#define CFE_PLATFORM_ES_MAX_BLOCK_SIZE             80000


/**
**  \cfeescfg Define ES Critical Data Store Memory Pool Block Sizes
**
**  \par Description:
**       Intermediate ES Critical Data Store Memory Pool Block Sizes
**
**  \par Limits
**       These sizes MUST be increasing and MUST be an integral multiple of 4.
*/
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_01              8
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_02             16
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_03             32
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_04             48
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_05             64
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_06             96
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_07            128
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_08            160
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_09            256
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_10            512
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_11           1024
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_12           2048
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_13           4096
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_14           8192
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_15          16384
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_16          32768
#define CFE_PLATFORM_ES_CDS_MAX_BLOCK_SIZE             80000



/**
**  \cfeevscfg Define Maximum Number of Event Filters per Application
**
**  \par Description:
**       Maximum number of events that may be filtered per application.
**
**  \par Limits
**       There are no restrictions on the lower and upper limits however,
**       the maximum number of event filters is system dependent and should be
**       verified.
*/
#define CFE_PLATFORM_EVS_MAX_EVENT_FILTERS     8


/**
**  \cfeevscfg Enable or Disable EVS Local Event Log
**
**  \par Description:
**       The CFE_PLATFORM_EVS_LOG_ON configuration parameter must be defined to enable EVS
**       event logging. In order to disable the local event log this definition needs
**       to be commented out.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_EVS_LOG_ON


/**
**  \cfeevscfg Default Event Log Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the Event
**       Services local event log. This filename is used only when no filename is
**       specified in the command to dump the event log.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_EVS_DEFAULT_LOG_FILE         "/ram/cfe_evs.log"


/**
**  \cfeevscfg Maximum Number of Events in EVS Local Event Log
**
**  \par Description:
**       Dictates the EVS local event log capacity. Units are the number of events.
**
**  \par Limits
**       There are no restrictions on the lower and upper limits however,
**       the maximum log size is system dependent and should be verified.
*/
#define CFE_PLATFORM_EVS_LOG_MAX               20


/**
**  \cfeevscfg Default EVS Application Data Filename
**
**  \par Description:
**       The value of this constant defines the filename used to store the EVS
**       Application Data(event counts/filtering information). This filename is
**       used only when no filename is specified in the command to dump the event
**       log.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_EVS_DEFAULT_APP_DATA_FILE    "/ram/cfe_evs_app.dat"


/**
**  \cfeevscfg Default EVS Output Port State
**
**  \par Description:
**       Defines the default port state (enabled or deisabled) for the four output
**       ports defined within the Event Service. Port 1 is usually the uart output
**       terminal. To enable a port, set the proper bit to a 1. Bit 0 is port 1,
**       bit 1 is port2 etc.
**
**  \par Limits
**       The valid settings are 0x0 to 0xF.
*/
#define CFE_PLATFORM_EVS_PORT_DEFAULT          0x0001


/**
**  \cfeevscfg Default EVS Event Type Filter Mask
**
**  \par Description:
**       Defines a state of on or off for all four event types. The term event
**       'type' refers to the criticality level and may be Debug, Informational,
**       Error or Critical. Each event type has a bit position. (bit 0 = Debug,
**       bit 1 = Info, bit 2 = Error, bit 3 = Critical). This is a global setting,
**       meaning it applies to all applications. To filter an event type, set its
**       bit to zero. For example,
**       0xE means Debug = OFF, Info = ON, Error = ON, Critical = ON
**
**  \par Limits
**       The valid settings are 0x0 to 0xF.
*/
#define CFE_PLATFORM_EVS_DEFAULT_TYPE_FLAG     0xE



/**
**  \cfeevscfg Default EVS Local Event Log Mode
**
**  \par Description:
**       Defines a state of overwrite(0) or discard(1) for the operation of the
**       EVS local event log. The log may operate in either Overwrite mode = 0,
**       where once the log becomes full the oldest event in the log will be
**       overwritten, or Discard mode = 1, where once the log becomes full the
**       contents of the log are preserved and the new event is discarded.
**       Overwrite Mode = 0, Discard Mode = 1.
**
**  \par Limits
**       The valid settings are 0 or 1
*/
#define CFE_PLATFORM_EVS_DEFAULT_LOG_MODE      1


/**
**  \cfeevscfg Default EVS Message Format Mode
**
**  \par Description:
**       Defines the default message format (long or short) for event messages being
**       sent to the ground. Choose between #CFE_EVS_MsgFormat_LONG or
**       #CFE_EVS_MsgFormat_SHORT.
**
**  \par Limits
**       The valid settings are #CFE_EVS_MsgFormat_LONG or #CFE_EVS_MsgFormat_SHORT
*/
#define CFE_PLATFORM_EVS_DEFAULT_MSG_FORMAT_MODE CFE_EVS_MsgFormat_LONG



/* Platform Configuration Parameters for Table Service (TBL) */

/**
**  \cfetblcfg Size of Table Services Table Memory Pool
**
**  \par Description:
**       Defines the TOTAL size of the memory pool that cFE Table Services allocates
**       from the system.  The size must be large enough to provide memory for each
**       registered table, the inactive buffers for double buffered tables and for
**       the shared inactive buffers for single buffered tables.
**
**  \par Limits
**       The cFE does not place a limit on the size of this parameter.
*/
#define CFE_PLATFORM_TBL_BUF_MEMORY_BYTES        524288

/**
**  \cfetblcfg Maximum Size Allowed for a Double Buffered Table
**
**  \par Description:
**       Defines the maximum allowed size (in bytes) of a double buffered table.
**
**  \par Limits
**       The cFE does not place a limit on the size of this parameter but it must be
**       less than half of #CFE_PLATFORM_TBL_BUF_MEMORY_BYTES.
*/
#define CFE_PLATFORM_TBL_MAX_DBL_TABLE_SIZE    16384

/**
**  \cfetblcfg Maximum Size Allowed for a Single Buffered Table
**
**  \par Description:
**       Defines the maximum allowed size (in bytes) of a single buffered table.
**       \b NOTE: This size determines the size of all shared table buffers.
**       Therefore, this size will be multiplied by #CFE_PLATFORM_TBL_MAX_SIMULTANEOUS_LOADS
**       below when allocating memory for shared tables.
**
**  \par Limits
**       The cFE does not place a limit on the size of this parameter but it must be
**       small enough to allow for #CFE_PLATFORM_TBL_MAX_SIMULTANEOUS_LOADS number of tables
**       to fit into #CFE_PLATFORM_TBL_BUF_MEMORY_BYTES.
*/
#define CFE_PLATFORM_TBL_MAX_SNGL_TABLE_SIZE   16384

/**
**  \cfetblcfg Maximum Number of Tables Allowed to be Registered
**
**  \par Description:
**       Defines the maximum number of tables supported by this processor's Table Services.
**
**  \par Limits
**       This number must be less than 32767.  It should be recognized that this parameter
**       determines the size of the Table Registry.  An excessively high number will waste
**       memory.
*/
#define CFE_PLATFORM_TBL_MAX_NUM_TABLES         128

/**
**  \cfetblcfg Maximum Number of Critical Tables that can be Registered
**
**  \par Description:
**       Defines the maximum number of critical tables supported by this processor's Table Services.
**
**  \par Limits
**       This number must be less than 32767.  It should be recognized that this parameter
**       determines the size of the Critical Table Registry which is maintained in the Critical
**       Data Store.  An excessively high number will waste Critical Data Store memory.  Therefore,
**       this number must not exceed the value defined in CFE_ES_CDS_MAX_CRITICAL_TABLES.
*/
#define CFE_PLATFORM_TBL_MAX_CRITICAL_TABLES     32

/**
**  \cfetblcfg Maximum Number of Table Handles
**
**  \par Description:
**       Defines the maximum number of Table Handles.
**
**  \par Limits
**       This number must be less than 32767.  This number must be at least as big as
**       the number of tables (#CFE_PLATFORM_TBL_MAX_NUM_TABLES) and should be set higher if tables
**       are shared between applications.
*/
#define CFE_PLATFORM_TBL_MAX_NUM_HANDLES        256

/**
**  \cfetblcfg Maximum Number of Simultaneous Loads to Support
**
**  \par Description:
**       Defines the maximum number of single buffered tables that can be
**       loaded simultaneously.  This number is used to determine the number
**       of shared buffers to allocate.
**
**  \par Limits
**       This number must be less than 32767.  An excessively high number will
**       degrade system performance and waste memory.  A number less than 5 is
**       suggested but not required.
*/
#define CFE_PLATFORM_TBL_MAX_SIMULTANEOUS_LOADS   4

/**
**  \cfetblcfg Maximum Number of Simultaneous Table Validations
**
**  \par Description:
**       Defines the maximum number of pending validations that
**       the Table Services can handle at any one time.  When a
**       table has a validation function, a validation request is
**       made of the application to perform that validation.  This
**       number determines how many of those requests can be
**       outstanding at any one time.
**
**  \par Limits
**       This number must be less than 32767.  An excessively high number will
**       degrade system performance and waste memory.  A number less than 20 is
**       suggested but not required.
*/
#define CFE_PLATFORM_TBL_MAX_NUM_VALIDATIONS     10

/**
**  \cfetblcfg Default Filename for a Table Registry Dump
**
**  \par Description:
**       Defines the file name used to store the table registry when
**       no filename is specified in the dump registry command.
**
**  \par Limits
**       The length of each string, including the NULL terminator cannot exceed the
**       #OS_MAX_PATH_LEN value.
*/
#define CFE_PLATFORM_TBL_DEFAULT_REG_DUMP_FILE    "/ram/cfe_tbl_reg.log"

/**
**  \cfetblcfg Number of Spacecraft ID's specified for validation
**
**  \par Description:
**       Defines the number of specified spacecraft ID values that
**       are verified during table loads.  If the number is zero
**       then no validation of the spacecraft ID field in the table
**       file header is performed when tables are loaded.  Non-zero
**       values indicate how many values from the list of spacecraft
**       ID's defined below are compared to the spacecraft ID field
**       in the table file header.  The ELF2CFETBL tool may be used
**       to create table files with specified spacecraft ID values.
**
**  \par Limits
**       This number must be greater than or equal to zero and
**       less than or equal to 2.
*/
#define CFE_PLATFORM_TBL_VALID_SCID_COUNT        0

/* macro to construct 32 bit value from 4 chars */
#define CFE_PLATFORM_TBL_U32FROM4CHARS( _C1, _C2, _C3, _C4 ) \
 ( (uint32)(_C1) << 24 | \
   (uint32)(_C2) << 16 | \
   (uint32)(_C3) << 8 | \
   (uint32)(_C4) )

/**
**  \cfetblcfg Spacecraft ID values used for table load validation
**
**  \par Description:
**       Defines the spacecraft ID values used for validating the
**       spacecraft ID field in the table file header.  To be valid,
**       the spacecraft ID specified in the table file header must
**       match one of the values defined here.
**
**  \par Limits
**       This value can be any 32 bit unsigned integer.
*/
#define CFE_PLATFORM_TBL_VALID_SCID_1            (CFE_MISSION_SPACECRAFT_ID)
#define CFE_PLATFORM_TBL_VALID_SCID_2            (CFE_PLATFORM_TBL_U32FROM4CHARS('a', 'b', 'c', 'd'))

/**
**  \cfetblcfg Number of Processor ID's specified for validation
**
**  \par Description:
**       Defines the number of specified processor ID values that
**       are verified during table loads.  If the number is zero
**       then no validation of the processor ID field in the table
**       file header is performed when tables are loaded.  Non-zero
**       values indicate how many values from the list of processor
**       ID's defined below are compared to the processor ID field
**       in the table file header.  The ELF2CFETBL tool may be used
**       to create table files with specified processor ID values.
**
**  \par Limits
**       This number must be greater than or equal to zero and
**       less than or equal to 4.
*/
#define CFE_PLATFORM_TBL_VALID_PRID_COUNT        0

/**
**  \cfetblcfg Processor ID values used for table load validation
**
**  \par Description:
**       Defines the processor ID values used for validating the
**       processor ID field in the table file header.  To be valid,
**       the spacecraft ID specified in the table file header must
**       match one of the values defined here.
**
**  \par Limits
**       This value can be any 32 bit unsigned integer.
*/
#define CFE_PLATFORM_TBL_VALID_PRID_1            (CFE_PLATFORM_CPU_ID)
#define CFE_PLATFORM_TBL_VALID_PRID_2            (CFE_PLATFORM_TBL_U32FROM4CHARS('a', 'b', 'c', 'd'))
#define CFE_PLATFORM_TBL_VALID_PRID_3            0
#define CFE_PLATFORM_TBL_VALID_PRID_4            0

/** \cfeescfg Mission specific version number for cFE
**
**  \par Description:
**       The cFE version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "cfe_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define CFE_MISSION_REV                  0

/** \cfeescfg Poll timer for startup sync delay
**
**  \par Description:
**      During startup, some tasks may need to synchronize their own initialization
**      with the initialization of other applications in the system.
**
**      CFE ES implements an API to accomplish this, that performs a task delay (sleep)
**      while polling the overall system state until other tasks are ready.
**
**      This value controls the amount of time that the CFE_ES_ApplicationSyncDelay
**      will sleep between each check of the system state.  This should be large enough
**      to allow other tasks to run, but not so large as to noticeably delay the startup
**      completion.
**
**      Units are in milliseconds
**
**  \par Limits:
**       Must be defined as an integer value that is greater than
**       or equal to zero.
*/
#define CFE_PLATFORM_ES_STARTUP_SYNC_POLL_MSEC       50

/** \cfeescfg CFE core application startup timeout
**
**  \par Description:
**      The upper limit for the amount of time that the cFE core applications
**      (ES, SB, EVS, TIME, TBL) are each alloted to reach their respective
**      "ready" states.
**
**      The CFE "main" thread starts individual tasks for each of the core applications
**      (except FS).  Each of these must perform some initialization work before the
**      next core application can be started, so the main thread waits to ensure that the
**      application has reached the "ready" state before starting the next application.
**
**      If any core application fails to start, then it indicates a major problem with
**      the system and startup is aborted.
**
**      Units are in milliseconds
**
**  \par Limits:
**       Must be defined as an integer value that is greater than
**       or equal to zero.
**
*/
#define CFE_PLATFORM_CORE_MAX_STARTUP_MSEC       30000

/** \cfeescfg Startup script timeout
**
**  \par Description:
**      The upper limit for the total amount of time that all apps listed in the CFE ES startup
**      script may take to all become ready.
**
**      Unlike the "core" app timeout, this is a soft limit; if the alloted time is exceeded,
**      it probably indicates an issue with one of the apps, but does not cause CFE ES to take
**      any additional action other than logging the event to the syslog.
**
**      Units are in milliseconds
**
**  \par Limits:
**       Must be defined as an integer value that is greater than
**       or equal to zero.
 */
#define CFE_PLATFORM_ES_STARTUP_SCRIPT_TIMEOUT_MSEC  1000


/*
 * Compatibility layer for CFE release 6.6
 * During development of CFE 6.6 a naming convention was introduced such
 * that all platform-specific symbols use a CFE_PLATFORM_ prefix.
 *
 * The following section provides a mapping from the historical name to
 * the new name for compatibility with existing code.  The code can then be
 * compiled with the CFE_OMIT_DEPRECATED_6_6 macro defined, and an error will
 * be triggered if any of the old symbol names are referenced in the code.
 *
 * It is expected that this compatibility layer will be removed in the next
 * release following 6.6, so all code must be fixed to use the new name.
 */
#ifndef CFE_OMIT_DEPRECATED_6_6

#define CFE_CPU_ID                          CFE_PLATFORM_CPU_ID
#define CFE_CPU_NAME                        CFE_PLATFORM_CPU_NAME
#define CFE_SB_MAX_MSG_IDS                  CFE_PLATFORM_SB_MAX_MSG_IDS
#define CFE_SB_MAX_PIPES                    CFE_PLATFORM_SB_MAX_PIPES
#define CFE_SB_MAX_DEST_PER_PKT             CFE_PLATFORM_SB_MAX_DEST_PER_PKT
#define CFE_SB_DEFAULT_MSG_LIMIT            CFE_PLATFORM_SB_DEFAULT_MSG_LIMIT
#define CFE_SB_BUF_MEMORY_BYTES             CFE_PLATFORM_SB_BUF_MEMORY_BYTES
#define CFE_SB_MAX_PIPE_DEPTH               CFE_PLATFORM_SB_MAX_PIPE_DEPTH
#define CFE_SB_HIGHEST_VALID_MSGID          CFE_PLATFORM_SB_HIGHEST_VALID_MSGID
#define CFE_SB_DEFAULT_ROUTING_FILENAME     CFE_PLATFORM_SB_DEFAULT_ROUTING_FILENAME
#define CFE_SB_DEFAULT_PIPE_FILENAME        CFE_PLATFORM_SB_DEFAULT_PIPE_FILENAME
#define CFE_SB_DEFAULT_MAP_FILENAME         CFE_PLATFORM_SB_DEFAULT_MAP_FILENAME
#define CFE_SB_FILTERED_EVENT1              CFE_PLATFORM_SB_FILTERED_EVENT1
#define CFE_SB_FILTER_MASK1                 CFE_PLATFORM_SB_FILTER_MASK1
#define CFE_SB_FILTERED_EVENT2              CFE_PLATFORM_SB_FILTERED_EVENT2
#define CFE_SB_FILTER_MASK2                 CFE_PLATFORM_SB_FILTER_MASK2
#define CFE_SB_FILTERED_EVENT3              CFE_PLATFORM_SB_FILTERED_EVENT3
#define CFE_SB_FILTER_MASK3                 CFE_PLATFORM_SB_FILTER_MASK3
#define CFE_SB_FILTERED_EVENT4              CFE_PLATFORM_SB_FILTERED_EVENT4
#define CFE_SB_FILTER_MASK4                 CFE_PLATFORM_SB_FILTER_MASK4
#define CFE_SB_FILTERED_EVENT5              CFE_PLATFORM_SB_FILTERED_EVENT5
#define CFE_SB_FILTER_MASK5                 CFE_PLATFORM_SB_FILTER_MASK5
#define CFE_SB_FILTERED_EVENT6              CFE_PLATFORM_SB_FILTERED_EVENT6
#define CFE_SB_FILTER_MASK6                 CFE_PLATFORM_SB_FILTER_MASK6
#define CFE_SB_FILTERED_EVENT7              CFE_PLATFORM_SB_FILTERED_EVENT7
#define CFE_SB_FILTER_MASK7                 CFE_PLATFORM_SB_FILTER_MASK7
#define CFE_SB_FILTERED_EVENT8              CFE_PLATFORM_SB_FILTERED_EVENT8
#define CFE_SB_FILTER_MASK8                 CFE_PLATFORM_SB_FILTER_MASK8
#define CFE_SB_MEM_BLOCK_SIZE_01            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_01
#define CFE_SB_MEM_BLOCK_SIZE_02            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_02
#define CFE_SB_MEM_BLOCK_SIZE_03            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_03
#define CFE_SB_MEM_BLOCK_SIZE_04            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_04
#define CFE_SB_MEM_BLOCK_SIZE_05            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_05
#define CFE_SB_MEM_BLOCK_SIZE_06            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_06
#define CFE_SB_MEM_BLOCK_SIZE_07            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_07
#define CFE_SB_MEM_BLOCK_SIZE_08            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_08
#define CFE_SB_MEM_BLOCK_SIZE_09            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_09
#define CFE_SB_MEM_BLOCK_SIZE_10            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_10
#define CFE_SB_MEM_BLOCK_SIZE_11            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_11
#define CFE_SB_MEM_BLOCK_SIZE_12            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_12
#define CFE_SB_MEM_BLOCK_SIZE_13            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_13
#define CFE_SB_MEM_BLOCK_SIZE_14            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_14
#define CFE_SB_MEM_BLOCK_SIZE_15            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_15
#define CFE_SB_MEM_BLOCK_SIZE_16            CFE_PLATFORM_SB_MEM_BLOCK_SIZE_16
#define CFE_SB_MAX_BLOCK_SIZE               CFE_PLATFORM_SB_MAX_BLOCK_SIZE
#define CFE_SB_DEFAULT_REPORT_SENDER        CFE_PLATFORM_SB_DEFAULT_REPORT_SENDER
#define CFE_TIME_CFG_SERVER                 CFE_PLATFORM_TIME_CFG_SERVER
#define CFE_TIME_CFG_CLIENT                 CFE_PLATFORM_TIME_CFG_CLIENT
#define CFE_TIME_CFG_VIRTUAL                CFE_PLATFORM_TIME_CFG_VIRTUAL
#define CFE_TIME_CFG_SIGNAL                 CFE_PLATFORM_TIME_CFG_SIGNAL
#define CFE_TIME_CFG_SOURCE                 CFE_PLATFORM_TIME_CFG_SOURCE
#define CFE_TIME_CFG_SRC_MET                CFE_PLATFORM_TIME_CFG_SRC_MET
#define CFE_TIME_CFG_SRC_GPS                CFE_PLATFORM_TIME_CFG_SRC_GPS
#define CFE_TIME_CFG_SRC_TIME               CFE_PLATFORM_TIME_CFG_SRC_TIME
#define CFE_TIME_MAX_DELTA_SECS             CFE_PLATFORM_TIME_MAX_DELTA_SECS
#define CFE_TIME_MAX_DELTA_SUBS             CFE_PLATFORM_TIME_MAX_DELTA_SUBS
#define CFE_TIME_MAX_LOCAL_SECS             CFE_PLATFORM_TIME_MAX_LOCAL_SECS
#define CFE_TIME_MAX_LOCAL_SUBS             CFE_PLATFORM_TIME_MAX_LOCAL_SUBS
#define CFE_TIME_CFG_TONE_LIMIT             CFE_PLATFORM_TIME_CFG_TONE_LIMIT
#define CFE_TIME_CFG_START_FLY              CFE_PLATFORM_TIME_CFG_START_FLY
#define CFE_TIME_CFG_LATCH_FLY              CFE_PLATFORM_TIME_CFG_LATCH_FLY
#define CFE_ES_MAX_APPLICATIONS             CFE_PLATFORM_ES_MAX_APPLICATIONS
#define CFE_ES_MAX_LIBRARIES                CFE_PLATFORM_ES_MAX_LIBRARIES
#define CFE_ES_ER_LOG_ENTRIES               CFE_PLATFORM_ES_ER_LOG_ENTRIES
#define CFE_ES_ER_LOG_MAX_CONTEXT_SIZE      CFE_PLATFORM_ES_ER_LOG_MAX_CONTEXT_SIZE
#define CFE_ES_SYSTEM_LOG_SIZE              CFE_PLATFORM_ES_SYSTEM_LOG_SIZE
#define CFE_ES_OBJECT_TABLE_SIZE            CFE_PLATFORM_ES_OBJECT_TABLE_SIZE
#define CFE_ES_MAX_GEN_COUNTERS             CFE_PLATFORM_ES_MAX_GEN_COUNTERS
#define CFE_ES_APP_SCAN_RATE                CFE_PLATFORM_ES_APP_SCAN_RATE
#define CFE_ES_APP_KILL_TIMEOUT             CFE_PLATFORM_ES_APP_KILL_TIMEOUT
#define CFE_ES_RAM_DISK_SECTOR_SIZE         CFE_PLATFORM_ES_RAM_DISK_SECTOR_SIZE
#define CFE_ES_RAM_DISK_NUM_SECTORS         CFE_PLATFORM_ES_RAM_DISK_NUM_SECTORS
#define CFE_ES_RAM_DISK_PERCENT_RESERVED    CFE_PLATFORM_ES_RAM_DISK_PERCENT_RESERVED
#define CFE_ES_RAM_DISK_MOUNT_STRING        CFE_PLATFORM_ES_RAM_DISK_MOUNT_STRING
#define CFE_ES_CDS_SIZE                     CFE_PLATFORM_ES_CDS_SIZE
#define CFE_ES_USER_RESERVED_SIZE           CFE_PLATFORM_ES_USER_RESERVED_SIZE
#define CFE_ES_RESET_AREA_SIZE              CFE_PLATFORM_ES_RESET_AREA_SIZE
#define CFE_ES_NONVOL_STARTUP_FILE          CFE_PLATFORM_ES_NONVOL_STARTUP_FILE
#define CFE_ES_VOLATILE_STARTUP_FILE        CFE_PLATFORM_ES_VOLATILE_STARTUP_FILE
#define CFE_ES_DEFAULT_SHELL_FILENAME       CFE_PLATFORM_ES_DEFAULT_SHELL_FILENAME
#define CFE_ES_MAX_SHELL_CMD                CFE_PLATFORM_ES_MAX_SHELL_CMD
#define CFE_ES_MAX_SHELL_PKT                CFE_PLATFORM_ES_MAX_SHELL_PKT
#define CFE_ES_DEFAULT_APP_LOG_FILE         CFE_PLATFORM_ES_DEFAULT_APP_LOG_FILE
#define CFE_ES_DEFAULT_TASK_LOG_FILE        CFE_PLATFORM_ES_DEFAULT_TASK_LOG_FILE
#define CFE_ES_DEFAULT_SYSLOG_FILE          CFE_PLATFORM_ES_DEFAULT_SYSLOG_FILE
#define CFE_ES_DEFAULT_ER_LOG_FILE          CFE_PLATFORM_ES_DEFAULT_ER_LOG_FILE
#define CFE_ES_DEFAULT_PERF_DUMP_FILENAME   CFE_PLATFORM_ES_DEFAULT_PERF_DUMP_FILENAME
#define CFE_ES_DEFAULT_CDS_REG_DUMP_FILE    CFE_PLATFORM_ES_DEFAULT_CDS_REG_DUMP_FILE
#define CFE_ES_DEFAULT_SYSLOG_MODE          CFE_PLATFORM_ES_DEFAULT_PR_SYSLOG_MODE
#define CFE_ES_PERF_MAX_IDS                 CFE_PLATFORM_ES_PERF_MAX_IDS
#define CFE_ES_PERF_DATA_BUFFER_SIZE        CFE_PLATFORM_ES_PERF_DATA_BUFFER_SIZE
#define CFE_ES_PERF_FILTMASK_NONE           CFE_PLATFORM_ES_PERF_FILTMASK_NONE
#define CFE_ES_PERF_FILTMASK_ALL            CFE_PLATFORM_ES_PERF_FILTMASK_ALL
#define CFE_ES_PERF_FILTMASK_INIT           CFE_PLATFORM_ES_PERF_FILTMASK_INIT
#define CFE_ES_PERF_TRIGMASK_NONE           CFE_PLATFORM_ES_PERF_TRIGMASK_NONE
#define CFE_ES_PERF_TRIGMASK_ALL            CFE_PLATFORM_ES_PERF_TRIGMASK_ALL
#define CFE_ES_PERF_TRIGMASK_INIT           CFE_PLATFORM_ES_PERF_TRIGMASK_INIT
#define CFE_ES_PERF_CHILD_PRIORITY          CFE_PLATFORM_ES_PERF_CHILD_PRIORITY
#define CFE_ES_PERF_CHILD_STACK_SIZE        CFE_PLATFORM_ES_PERF_CHILD_STACK_SIZE
#define CFE_ES_PERF_CHILD_MS_DELAY          CFE_PLATFORM_ES_PERF_CHILD_MS_DELAY
#define CFE_ES_PERF_ENTRIES_BTWN_DLYS       CFE_PLATFORM_ES_PERF_ENTRIES_BTWN_DLYS
#define CFE_ES_DEFAULT_STACK_SIZE           CFE_PLATFORM_ES_DEFAULT_STACK_SIZE
#define CFE_ES_EXCEPTION_FUNCTION           CFE_PLATFORM_ES_EXCEPTION_FUNCTION
#define CFE_EVS_START_TASK_PRIORITY         CFE_PLATFORM_EVS_START_TASK_PRIORITY
#define CFE_EVS_START_TASK_STACK_SIZE       CFE_PLATFORM_EVS_START_TASK_STACK_SIZE
#define CFE_SB_START_TASK_PRIORITY          CFE_PLATFORM_SB_START_TASK_PRIORITY
#define CFE_SB_START_TASK_STACK_SIZE        CFE_PLATFORM_SB_START_TASK_STACK_SIZE
#define CFE_ES_START_TASK_PRIORITY          CFE_PLATFORM_ES_START_TASK_PRIORITY
#define CFE_ES_START_TASK_STACK_SIZE        CFE_PLATFORM_ES_START_TASK_STACK_SIZE
#define CFE_TIME_START_TASK_PRIORITY        CFE_PLATFORM_TIME_START_TASK_PRIORITY
#define CFE_TIME_TONE_TASK_PRIORITY         CFE_PLATFORM_TIME_TONE_TASK_PRIORITY
#define CFE_TIME_1HZ_TASK_PRIORITY          CFE_PLATFORM_TIME_1HZ_TASK_PRIORITY
#define CFE_TIME_START_TASK_STACK_SIZE      CFE_PLATFORM_TIME_START_TASK_STACK_SIZE
#define CFE_TIME_TONE_TASK_STACK_SIZE       CFE_PLATFORM_TIME_TONE_TASK_STACK_SIZE
#define CFE_TIME_1HZ_TASK_STACK_SIZE        CFE_PLATFORM_TIME_1HZ_TASK_STACK_SIZE
#define CFE_TBL_START_TASK_PRIORITY         CFE_PLATFORM_TBL_START_TASK_PRIORITY
#define CFE_TBL_START_TASK_STACK_SIZE       CFE_PLATFORM_TBL_START_TASK_STACK_SIZE
#define CFE_ES_CDS_MAX_NUM_ENTRIES          CFE_PLATFORM_ES_CDS_MAX_NUM_ENTRIES
#define CFE_ES_MAX_PROCESSOR_RESETS         CFE_PLATFORM_ES_MAX_PROCESSOR_RESETS
#define CFE_ES_MEM_BLOCK_SIZE_01            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_01
#define CFE_ES_MEM_BLOCK_SIZE_02            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_02
#define CFE_ES_MEM_BLOCK_SIZE_03            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_03
#define CFE_ES_MEM_BLOCK_SIZE_04            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_04
#define CFE_ES_MEM_BLOCK_SIZE_05            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_05
#define CFE_ES_MEM_BLOCK_SIZE_06            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_06
#define CFE_ES_MEM_BLOCK_SIZE_07            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_07
#define CFE_ES_MEM_BLOCK_SIZE_08            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_08
#define CFE_ES_MEM_BLOCK_SIZE_09            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_09
#define CFE_ES_MEM_BLOCK_SIZE_10            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_10
#define CFE_ES_MEM_BLOCK_SIZE_11            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_11
#define CFE_ES_MEM_BLOCK_SIZE_12            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_12
#define CFE_ES_MEM_BLOCK_SIZE_13            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_13
#define CFE_ES_MEM_BLOCK_SIZE_14            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_14
#define CFE_ES_MEM_BLOCK_SIZE_15            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_15
#define CFE_ES_MEM_BLOCK_SIZE_16            CFE_PLATFORM_ES_MEM_BLOCK_SIZE_16
#define CFE_ES_MAX_BLOCK_SIZE               CFE_PLATFORM_ES_MAX_BLOCK_SIZE
#define CFE_ES_CDS_MEM_BLOCK_SIZE_01        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_01
#define CFE_ES_CDS_MEM_BLOCK_SIZE_02        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_02
#define CFE_ES_CDS_MEM_BLOCK_SIZE_03        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_03
#define CFE_ES_CDS_MEM_BLOCK_SIZE_04        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_04
#define CFE_ES_CDS_MEM_BLOCK_SIZE_05        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_05
#define CFE_ES_CDS_MEM_BLOCK_SIZE_06        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_06
#define CFE_ES_CDS_MEM_BLOCK_SIZE_07        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_07
#define CFE_ES_CDS_MEM_BLOCK_SIZE_08        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_08
#define CFE_ES_CDS_MEM_BLOCK_SIZE_09        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_09
#define CFE_ES_CDS_MEM_BLOCK_SIZE_10        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_10
#define CFE_ES_CDS_MEM_BLOCK_SIZE_11        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_11
#define CFE_ES_CDS_MEM_BLOCK_SIZE_12        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_12
#define CFE_ES_CDS_MEM_BLOCK_SIZE_13        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_13
#define CFE_ES_CDS_MEM_BLOCK_SIZE_14        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_14
#define CFE_ES_CDS_MEM_BLOCK_SIZE_15        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_15
#define CFE_ES_CDS_MEM_BLOCK_SIZE_16        CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_16
#define CFE_ES_CDS_MAX_BLOCK_SIZE           CFE_PLATFORM_ES_CDS_MAX_BLOCK_SIZE
#define CFE_EVS_MAX_EVENT_FILTERS           CFE_PLATFORM_EVS_MAX_EVENT_FILTERS
#define CFE_EVS_LOG_ON                      CFE_PLATFORM_EVS_LOG_ON
#define CFE_EVS_DEFAULT_LOG_FILE            CFE_PLATFORM_EVS_DEFAULT_LOG_FILE
#define CFE_EVS_LOG_MAX                     CFE_PLATFORM_EVS_LOG_MAX
#define CFE_EVS_DEFAULT_APP_DATA_FILE       CFE_PLATFORM_EVS_DEFAULT_APP_DATA_FILE
#define CFE_EVS_PORT_DEFAULT                CFE_PLATFORM_EVS_PORT_DEFAULT
#define CFE_EVS_DEFAULT_TYPE_FLAG           CFE_PLATFORM_EVS_DEFAULT_TYPE_FLAG
#define CFE_EVS_DEFAULT_LOG_MODE            CFE_PLATFORM_EVS_DEFAULT_LOG_MODE
#define CFE_EVS_DEFAULT_MSG_FORMAT_MODE     CFE_PLATFORM_EVS_DEFAULT_MSG_FORMAT_MODE
#define CFE_TBL_BUF_MEMORY_BYTES            CFE_PLATFORM_TBL_BUF_MEMORY_BYTES
#define CFE_TBL_MAX_DBL_TABLE_SIZE          CFE_PLATFORM_TBL_MAX_DBL_TABLE_SIZE
#define CFE_TBL_MAX_SNGL_TABLE_SIZE         CFE_PLATFORM_TBL_MAX_SNGL_TABLE_SIZE
#define CFE_TBL_MAX_NUM_TABLES              CFE_PLATFORM_TBL_MAX_NUM_TABLES
#define CFE_TBL_MAX_CRITICAL_TABLES         CFE_PLATFORM_TBL_MAX_CRITICAL_TABLES
#define CFE_TBL_MAX_NUM_HANDLES             CFE_PLATFORM_TBL_MAX_NUM_HANDLES
#define CFE_TBL_MAX_SIMULTANEOUS_LOADS      CFE_PLATFORM_TBL_MAX_SIMULTANEOUS_LOADS
#define CFE_TBL_MAX_NUM_VALIDATIONS         CFE_PLATFORM_TBL_MAX_NUM_VALIDATIONS
#define CFE_TBL_DEFAULT_REG_DUMP_FILE       CFE_PLATFORM_TBL_DEFAULT_REG_DUMP_FILE
#define CFE_TBL_VALID_SCID_COUNT            CFE_PLATFORM_TBL_VALID_SCID_COUNT
#define CFE_TBL_U32FROM4CHARS               CFE_PLATFORM_TBL_U32FROM4CHARS
#define CFE_TBL_VALID_SCID_1                CFE_PLATFORM_TBL_VALID_SCID_1
#define CFE_TBL_VALID_SCID_2                CFE_PLATFORM_TBL_VALID_SCID_2
#define CFE_TBL_VALID_PRID_COUNT            CFE_PLATFORM_TBL_VALID_PRID_COUNT
#define CFE_TBL_VALID_PRID_1                CFE_PLATFORM_TBL_VALID_PRID_1
#define CFE_TBL_VALID_PRID_2                CFE_PLATFORM_TBL_VALID_PRID_2
#define CFE_TBL_VALID_PRID_3                CFE_PLATFORM_TBL_VALID_PRID_3
#define CFE_TBL_VALID_PRID_4                CFE_PLATFORM_TBL_VALID_PRID_4
#define CFE_ES_STARTUP_SYNC_POLL_MSEC       CFE_PLATFORM_ES_STARTUP_SYNC_POLL_MSEC
#define CFE_CORE_MAX_STARTUP_MSEC           CFE_PLATFORM_CORE_MAX_STARTUP_MSEC
#define CFE_ES_STARTUP_SCRIPT_TIMEOUT_MSEC  CFE_PLATFORM_ES_STARTUP_SCRIPT_TIMEOUT_MSEC

/*
 * This was previously configurable, now fixed.
 * Keeping it here will trigger a "redefined" warning if some mission
 * had configured it as "false" for some reason.
 */
#define CFE_TIME_ENA_1HZ_CMD_PKT            true

#endif  /* CFE_OMIT_DEPRECATED_6_6 */

#endif  /* _cfe_platform_cfg_ */

