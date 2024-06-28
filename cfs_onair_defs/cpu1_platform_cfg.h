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
 *   This header file contains the platform configuration parameters.
 *
 * Notes:
 *   The impact of changing these configurations from their default value is
 *   not yet documented.  Changing these values may impact the performance
 *   and functionality of the system.
 *
 * Author:   R.McGraw/SSI
 *
 */

#ifndef CPU1_PLATFORM_CFG_H
#define CPU1_PLATFORM_CFG_H

/**
**  \cfeescfg Default virtual path for persistent storage
**
**  \par Description:
**       This configures the default location in the virtual file system
**       for persistent/non-volatile storage.  Files such as the startup
**       script, app/library dynamic modules, and configuration tables are
**       expected to be stored in this directory.
**
*/
#define CFE_PLATFORM_ES_NONVOL_DISK_MOUNT_STRING "/cf"

/**
**  \cfeescfg Default virtual path for volatile storage
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
**  \cfesbcfg Maximum Number of Unique Message IDs SB Routing Table can hold
**
**  \par Description:
**       Dictates the maximum number of unique MsgIds the SB routing table will hold.
**       This constant has a direct effect on the size of SB's tables and arrays.
**       Keeping this count as low as possible will save memory.
**       To see the run-time, high-water mark and the current utilization figures
**       regarding this parameter, send an SB command to 'Send Statistics Pkt'.
**
**  \par Limits
**       This must be a power of two if software bus message routing hash implementation
**       is being used.  Lower than 64 will cause unit test failures, and
**       telemetry reporting is impacted below 32.  There is no hard
**       upper limit, but impacts memory footprint.  For software bus message routing
**       search implementation the number of msg ids subscribed to impacts performance.
**
*/
#define CFE_PLATFORM_SB_MAX_MSG_IDS 256

/**
**  \cfesbcfg Maximum Number of Unique Pipes SB Routing Table can hold
**
**  \par Description:
**       Dictates the maximum number of unique Pipes the SB routing table will hold.
**       This constant has a direct effect on the size of SB's tables and arrays.
**       Keeping this count as low as possible will save memory.
**       To see the run-time, high-water mark and the current utilization figures
**       regarding this parameter, send an SB command to 'Send Statistics Pkt'.
**
**  \par Limits
**       This parameter has a lower limit of 1.  This parameter must also be less than
**       or equal to OS_MAX_QUEUES.
**
*/
#define CFE_PLATFORM_SB_MAX_PIPES 64

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
#define CFE_PLATFORM_SB_MAX_DEST_PER_PKT 16

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
#define CFE_PLATFORM_SB_DEFAULT_MSG_LIMIT 4

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
#define CFE_PLATFORM_SB_BUF_MEMORY_BYTES 524288

/**
**  \cfesbcfg Highest Valid Message Id
**
**  \par Description:
**       The value of this constant dictates the range of valid message ID's, from 0
**       to CFE_PLATFORM_SB_HIGHEST_VALID_MSGID (inclusive).
**
**       Although this can be defined differently across platforms, each platform can
**       only publish/subscribe to message ids within their allowable range. Typically
**       this value is set the same across all mission platforms to avoid this complexity.
**
**  \par Limits
**       CFE_SB_INVALID_MSG is set to the maximum representable number of type CFE_SB_MsgId_t.
**       CFE_PLATFORM_SB_HIGHEST_VALID_MSGID lower limit is 1, up to CFE_SB_INVALID_MSG_ID - 1.
**
**       When using the direct message map implementation for software bus routing, this
**       value is used to size the map where a value of 0x1FFF results in a 16 KBytes map
**       and 0xFFFF is 128 KBytes.
**
**       When using the hash implementation for software bus routing, a multiple of the
**       CFE_PLATFORM_SB_MAX_MSG_IDS is used to size the message map.  In that case
**       the range selected here does not impact message map memory use, so it's
**       reasonable to use up to the full range supported by the message ID implementation.
*/
#define CFE_PLATFORM_SB_HIGHEST_VALID_MSGID 0x1FFF

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
#define CFE_PLATFORM_SB_DEFAULT_ROUTING_FILENAME "/ram/cfe_sb_route.dat"

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
#define CFE_PLATFORM_SB_DEFAULT_PIPE_FILENAME "/ram/cfe_sb_pipe.dat"

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
#define CFE_PLATFORM_SB_DEFAULT_MAP_FILENAME "/ram/cfe_sb_msgmap.dat"

/**
**  \cfesbcfg SB Event Filtering
**
**  \par Description:
**       This group of configuration parameters dictates what SB events will be
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
#define CFE_PLATFORM_SB_FILTERED_EVENT1 CFE_SB_SEND_NO_SUBS_EID
#define CFE_PLATFORM_SB_FILTER_MASK1    CFE_EVS_FIRST_4_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT2 CFE_SB_DUP_SUBSCRIP_EID
#define CFE_PLATFORM_SB_FILTER_MASK2    CFE_EVS_FIRST_4_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT3 CFE_SB_MSGID_LIM_ERR_EID
#define CFE_PLATFORM_SB_FILTER_MASK3    CFE_EVS_FIRST_16_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT4 CFE_SB_Q_FULL_ERR_EID
#define CFE_PLATFORM_SB_FILTER_MASK4    CFE_EVS_FIRST_16_STOP

#define CFE_PLATFORM_SB_FILTERED_EVENT5 0
#define CFE_PLATFORM_SB_FILTER_MASK5    CFE_EVS_NO_FILTER

#define CFE_PLATFORM_SB_FILTERED_EVENT6 0
#define CFE_PLATFORM_SB_FILTER_MASK6    CFE_EVS_NO_FILTER

#define CFE_PLATFORM_SB_FILTERED_EVENT7 0
#define CFE_PLATFORM_SB_FILTER_MASK7    CFE_EVS_NO_FILTER

#define CFE_PLATFORM_SB_FILTERED_EVENT8 0
#define CFE_PLATFORM_SB_FILTER_MASK8    CFE_EVS_NO_FILTER

/**
**  \cfeescfg Define SB Memory Pool Block Sizes
**
**  \par Description:
**       Software Bus Memory Pool Block Sizes
**
**  \par Limits
**       These sizes MUST be increasing and MUST be an integral multiple of 4.
**       The number of block sizes defined cannot exceed
**       #CFE_PLATFORM_ES_POOL_MAX_BUCKETS
*/
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_01 8
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_02 16
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_03 20
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_04 36
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_05 64
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_06 96
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_07 128
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_08 160
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_09 256
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_10 512
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_11 1024
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_12 2048
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_13 4096
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_14 8192
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_15 16384
#define CFE_PLATFORM_SB_MEM_BLOCK_SIZE_16 32768
#define CFE_PLATFORM_SB_MAX_BLOCK_SIZE    (CFE_MISSION_SB_MAX_SB_MSG_SIZE + 128)

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
#define CFE_PLATFORM_TIME_CFG_SERVER true
#define CFE_PLATFORM_TIME_CFG_CLIENT false

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
#define CFE_PLATFORM_TIME_CFG_VIRTUAL true

/**
**  \cfetimecfg Include or Exclude the Primary/Redundant Tone Selection Cmd
**
**  \par Description:
**       Depending on the specific hardware system configuration, it may be possible
**       to switch between a primary and redundant tone signal.  If supported by
**       hardware, this definition will enable command interfaces to select the
**       active tone signal. Both Time Clients and Time Servers support this feature.
**       Note: Set the CFE_PLATFORM_TIME_CFG_SIGNAL define to true to enable tone signal commands.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TIME_CFG_SIGNAL false

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
#define CFE_PLATFORM_TIME_CFG_SOURCE false

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
#define CFE_PLATFORM_TIME_CFG_SRC_MET  false
#define CFE_PLATFORM_TIME_CFG_SRC_GPS  false
#define CFE_PLATFORM_TIME_CFG_SRC_TIME false

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
#define CFE_PLATFORM_TIME_MAX_DELTA_SECS 0
#define CFE_PLATFORM_TIME_MAX_DELTA_SUBS 500000

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
#define CFE_PLATFORM_TIME_MAX_LOCAL_SECS 27
#define CFE_PLATFORM_TIME_MAX_LOCAL_SUBS 0

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
#define CFE_PLATFORM_TIME_CFG_TONE_LIMIT 20000

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
#define CFE_PLATFORM_TIME_CFG_START_FLY 2

/**
**  \cfetimecfg Define Periodic Time to Update Local Clock Tone Latch
**
**  \par Description:
**       Define Periodic Time to Update Local Clock Tone Latch. Applies only when
**       in flywheel mode. This define dictates the period at which the simulated
**       'last tone' time is updated. Units are seconds.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TIME_CFG_LATCH_FLY 8

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
#define CFE_PLATFORM_ES_ER_LOG_MAX_CONTEXT_SIZE 256

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
#define CFE_PLATFORM_ES_SYSTEM_LOG_SIZE 3072

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
#define CFE_PLATFORM_ES_OBJECT_TABLE_SIZE 30

/**
**  \cfeescfg Define Max Number of Generic Counters
**
**  \par Description:
**       Defines the maximum number of Generic Counters that can be registered.
**
**  \par Limits
**       This parameter has a lower limit of 1 and an upper limit of 65535.
*/
#define CFE_PLATFORM_ES_MAX_GEN_COUNTERS 8

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
**       configuration parameter. millisecond units.
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
**      -# If the App is responding and Calls it's RunLoop function, it will drop out
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
**       parameter. Units are number of #CFE_PLATFORM_ES_APP_SCAN_RATE cycles.
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
#define CFE_PLATFORM_ES_RAM_DISK_SECTOR_SIZE 512

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
#define CFE_PLATFORM_ES_RAM_DISK_NUM_SECTORS 4096

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
**       parameter.Units are percentage. A setting of zero will turn this feature
**       off.
*/
#define CFE_PLATFORM_ES_RAM_DISK_PERCENT_RESERVED 30

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
**       on this configuration parameter.
*/
#define CFE_PLATFORM_ES_CDS_SIZE (128 * 1024)

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
**       on this configuration parameter.
*/
#define CFE_PLATFORM_ES_USER_RESERVED_SIZE (1024 * 1024)

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
**       type, but circular dependencies in the headers prevent it from being defined
**       this way.
**       NOTE: Changing this value changes memory allocation, and may
**       require changes to platform specific values (in CFE_PSP) such as
**       USER_RESERVED_MEM in VxWorks depending on the memory areas
**       being used for preserved data and on OS specific behavior.
**
**  \par Limits
**       There is a lower limit of 153600 (150KBytes) and an upper limit of UINT_MAX
**       (4 Gigabytes) on this configuration parameter.
*/
#define CFE_PLATFORM_ES_RESET_AREA_SIZE (170 * 1024)

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
#define CFE_PLATFORM_ES_MEMPOOL_ALIGN_SIZE_MIN 4

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
#define CFE_PLATFORM_ES_NONVOL_STARTUP_FILE "/cf/cfe_es_startup.scr"

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
#define CFE_PLATFORM_ES_VOLATILE_STARTUP_FILE "/ram/cfe_es_startup.scr"

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
#define CFE_PLATFORM_ES_DEFAULT_APP_LOG_FILE "/ram/cfe_es_app_info.log"

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
#define CFE_PLATFORM_ES_DEFAULT_TASK_LOG_FILE "/ram/cfe_es_taskinfo.log"

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
#define CFE_PLATFORM_ES_DEFAULT_SYSLOG_FILE "/ram/cfe_es_syslog.log"

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
#define CFE_PLATFORM_ES_DEFAULT_ER_LOG_FILE "/ram/cfe_erlog.log"

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
#define CFE_PLATFORM_ES_DEFAULT_PERF_DUMP_FILENAME "/ram/cfe_es_perf.dat"

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
#define CFE_PLATFORM_ES_DEFAULT_CDS_REG_DUMP_FILE "/ram/cfe_cds_reg.log"

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
**       parameter.
*/
#define CFE_PLATFORM_ES_DEFAULT_POR_SYSLOG_MODE 0

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
**       parameter.
*/
#define CFE_PLATFORM_ES_DEFAULT_PR_SYSLOG_MODE 1

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
**       however, the maximum buffer size is system dependent and should be verified.
**       The units are number of entries. An entry is defined by a 32 bit data word followed
**       by a 64 bit time stamp.
*/
#define CFE_PLATFORM_ES_PERF_DATA_BUFFER_SIZE 10000

/**
**  \cfeescfg Define Filter Mask Setting for Disabling All Performance Entries
**
**  \par Description:
**       Defines the filter mask for disabling all performance entries. The value is a
**       bit mask.  For each bit, 0 means the corresponding entry is disabled and
**       1 means it is enabled.
*/
#define CFE_PLATFORM_ES_PERF_FILTMASK_NONE 0

/**
**  \cfeescfg Define Filter Mask Setting for Enabling All Performance Entries
**
**  \par Description:
**       Defines the filter mask for enabling all performance entries. The value is a
**       bit mask.  For each bit, 0 means the corresponding entry is disabled and
**       1 means it is enabled.
*/
#define CFE_PLATFORM_ES_PERF_FILTMASK_ALL ~CFE_PLATFORM_ES_PERF_FILTMASK_NONE

/**
**  \cfeescfg Define Default Filter Mask Setting for Performance Data Buffer
**
**  \par Description:
**       Defines the default filter mask for the performance data buffer. The value is a
**       bit mask.  For each bit, 0 means the corresponding entry is disabled and 1
**       means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_FILTMASK_INIT CFE_PLATFORM_ES_PERF_FILTMASK_ALL

/**
**  \cfeescfg Define Default Filter Trigger Setting for Disabling All Performance Entries
**
**  \par Description:
**       Defines the default trigger mask for disabling all performance data entries. The value
**       is a bit mask.  For each bit, 0 means the trigger for the corresponding entry is
**       disabled and 1 means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_TRIGMASK_NONE 0

/**
**  \cfeescfg Define Filter Trigger Setting for Enabling All Performance Entries
**
**  \par Description:
**       Defines the trigger mask for enabling all performance data entries. The value is
**       a bit mask.  For each bit, 0 means the trigger for the corresponding entry is
**       disabled and 1 means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_TRIGMASK_ALL ~CFE_PLATFORM_ES_PERF_TRIGMASK_NONE

/**
**  \cfeescfg Define Default Filter Trigger Setting for Performance Data Buffer
**
**  \par Description:
**       Defines the default trigger mask for the performance data buffer. The value is a
**       32-bit mask.  For each bit, 0 means the trigger for the corresponding entry is
**       disabled and 1 means it is enabled.
**
*/
#define CFE_PLATFORM_ES_PERF_TRIGMASK_INIT CFE_PLATFORM_ES_PERF_TRIGMASK_NONE

/**
**  \cfeescfg Define Performance Analyzer Child Task Priority
**
**  \par Description:
**       This parameter defines the priority of the child task spawned by the
**       Executive Services to write performance data to a file.  Lower numbers
**       are higher priority, with 1 being the highest priority in the case of a
**       child task.
**
**  \par Limits
**       Valid range for a child task is 1 to 255 however, the priority cannot
**       be higher (lower number) than the ES parent application priority.
*/
#define CFE_PLATFORM_ES_PERF_CHILD_PRIORITY 200

/**
**  \cfeescfg Define Performance Analyzer Child Task Stack Size
**
**  \par Description:
**       This parameter defines the stack size of the child task spawned by the
**       Executive Services to write performance data to a file.
**
**  \par Limits
**       It is recommended this parameter be greater than or equal to 4KB. This parameter
**       is limited by the maximum value allowed by the data type. In this case, the data
**       type is an unsigned 32-bit integer, so the valid range is 0 to 0xFFFFFFFF.
*/
#define CFE_PLATFORM_ES_PERF_CHILD_STACK_SIZE 4096

/**
**  \cfeescfg Define Performance Analyzer Child Task Delay
**
**  \par Description:
**       This parameter defines the delay time (in milliseconds) between performance
**       data file writes performed by the Executive Services Performance Analyzer
**       Child Task.
**
**  \par Limits
**       It is recommended this parameter be greater than or equal to 20ms. This parameter
**       is limited by the maximum value allowed by the data type. In this case, the data
**       type is an unsigned 32-bit integer, so the valid range is 0 to 0xFFFFFFFF.
*/
#define CFE_PLATFORM_ES_PERF_CHILD_MS_DELAY 20

/**
**  \cfeescfg Define Performance Analyzer Child Task Number of Entries Between Delay
**
**  \par Description:
**       This parameter defines the number of performance analyzer entries the Performance
**       Analyzer Child Task will write to the file between delays.
**
*/
#define CFE_PLATFORM_ES_PERF_ENTRIES_BTWN_DLYS 50

/**
**  \cfeescfg Define Default Stack Size for an Application
**
**  \par Description:
**       This parameter defines a default stack size. This parameter is used by the
**       cFE Core Applications.
**
**  \par Limits
**       There is a lower limit of 2048.  There are no restrictions on the upper limit
**       however, the maximum stack size is system dependent and should be verified.
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
#define CFE_PLATFORM_EVS_START_TASK_PRIORITY 61

/**
**  \cfeescfg Define EVS Task Stack Size
**
**  \par Description:
**       Defines the cFE_EVS Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration parameter.  There
**       are no restrictions on the upper limit however, the maximum stack size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_EVS_START_TASK_STACK_SIZE CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

/**
**  \cfeescfg Define SB Task Priority
**
**  \par Description:
**       Defines the cFE_SB Task priority.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_SB_START_TASK_PRIORITY 64

/**
**  \cfeescfg Define SB Task Stack Size
**
**  \par Description:
**       Defines the cFE_SB Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration parameter.  There
**       are no restrictions on the upper limit however, the maximum stack size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_SB_START_TASK_STACK_SIZE CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

/**
**  \cfeescfg Define ES Task Priority
**
**  \par Description:
**       Defines the cFE_ES Task priority.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_ES_START_TASK_PRIORITY 68

/**
**  \cfeescfg Define ES Task Stack Size
**
**  \par Description:
**       Defines the cFE_ES Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration parameter.  There
**       are no restrictions on the upper limit however, the maximum stack size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_ES_START_TASK_STACK_SIZE CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

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
**       configuration parameters.  Remember that the meaning of each task
**       priority is inverted -- a "lower" number has a "higher" priority.
*/
#define CFE_PLATFORM_TIME_START_TASK_PRIORITY 60
#define CFE_PLATFORM_TIME_TONE_TASK_PRIORITY  25
#define CFE_PLATFORM_TIME_1HZ_TASK_PRIORITY   25

/**
**  \cfetimecfg Define TIME Task Stack Sizes
**
**  \par Description:
**       Defines the cFE_TIME Main Task Stack Size
**       Defines the cFE_TIME Tone Task Stack Size
**       Defines the cFE_TIME 1HZ Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on these configuration parameters.  There
**       are no restrictions on the upper limit however, the maximum stack size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_TIME_START_TASK_STACK_SIZE CFE_PLATFORM_ES_DEFAULT_STACK_SIZE
#define CFE_PLATFORM_TIME_TONE_TASK_STACK_SIZE  4096
#define CFE_PLATFORM_TIME_1HZ_TASK_STACK_SIZE   8192

/**
**  \cfeescfg Define TBL Task Priority
**
**  \par Description:
**       Defines the cFE_TBL Task priority.
**
**  \par Limits
**       Not Applicable
*/
#define CFE_PLATFORM_TBL_START_TASK_PRIORITY 70

/**
**  \cfeescfg Define TBL Task Stack Size
**
**  \par Description:
**       Defines the cFE_TBL Task Stack Size
**
**  \par Limits
**       There is a lower limit of 2048 on this configuration parameter.  There
**       are no restrictions on the upper limit however, the maximum stack size
**       is system dependent and should be verified.  Most operating systems provide
**       tools for measuring the amount of stack used by a task during operation. It
**       is always a good idea to verify that no more than 1/2 of the stack is used.
*/
#define CFE_PLATFORM_TBL_START_TASK_STACK_SIZE CFE_PLATFORM_ES_DEFAULT_STACK_SIZE

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
#define CFE_PLATFORM_ES_CDS_MAX_NUM_ENTRIES 512

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
#define CFE_PLATFORM_ES_MAX_PROCESSOR_RESETS 2

/** \cfeescfg Maximum number of block sizes in pool structures
**
**  \par Description:
**      The upper limit for the number of block sizes supported in the generic
**      pool implementation, which in turn implements the memory pools and CDS.
**
**  \par Limits:
**       Must be at least one.  No specific upper limit, but the number is
**       anticipated to be reasonably small (i.e. tens, not hundreds).  Large
**       values have not been tested.
**
**       The ES and CDS block size lists must correlate with this value
*/
#define CFE_PLATFORM_ES_POOL_MAX_BUCKETS 17

/** \cfeescfg Maximum number of memory pools
**
**  \par Description:
**      The upper limit for the number of memory pools that can concurrently
**      exist within the system.
**
**      The CFE_SB and CFE_TBL core subsystems each define a memory pool.
**
**      Individual applications may also create memory pools, so this value
**      should be set sufficiently high enough to support the applications
**      being used on this platform.
**
**  \par Limits:
**       Must be at least 2 to support CFE core - SB and TBL pools.  No
**       specific upper limit.
*/
#define CFE_PLATFORM_ES_MAX_MEMORY_POOLS 10

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
**       enforced although the table size definitions may be reduced.
*/
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_01 8
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_02 16
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_03 32
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_04 48
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_05 64
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_06 96
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_07 128
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_08 160
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_09 256
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_10 512
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_11 1024
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_12 2048
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_13 4096
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_14 8192
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_15 16384
#define CFE_PLATFORM_ES_MEM_BLOCK_SIZE_16 32768
#define CFE_PLATFORM_ES_MAX_BLOCK_SIZE    80000

/**
**  \cfeescfg Define ES Critical Data Store Memory Pool Block Sizes
**
**  \par Description:
**       Intermediate ES Critical Data Store Memory Pool Block Sizes
**
**  \par Limits
**       These sizes MUST be increasing and MUST be an integral multiple of 4.
*/
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_01 8
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_02 16
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_03 32
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_04 48
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_05 64
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_06 96
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_07 128
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_08 160
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_09 256
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_10 512
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_11 1024
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_12 2048
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_13 4096
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_14 8192
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_15 16384
#define CFE_PLATFORM_ES_CDS_MEM_BLOCK_SIZE_16 32768
#define CFE_PLATFORM_ES_CDS_MAX_BLOCK_SIZE    80000

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
#define CFE_PLATFORM_EVS_MAX_EVENT_FILTERS 8

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
#define CFE_PLATFORM_EVS_DEFAULT_LOG_FILE "/ram/cfe_evs.log"

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
#define CFE_PLATFORM_EVS_LOG_MAX 20

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
#define CFE_PLATFORM_EVS_DEFAULT_APP_DATA_FILE "/ram/cfe_evs_app.dat"

/**
**  \cfeevscfg Default EVS Output Port State
**
**  \par Description:
**       Defines the default port state (enabled or disabled) for the four output
**       ports defined within the Event Service. Port 1 is usually the uart output
**       terminal. To enable a port, set the proper bit to a 1. Bit 0 is port 1,
**       bit 1 is port2 etc.
**
**  \par Limits
**       The valid settings are 0x0 to 0xF.
*/
#define CFE_PLATFORM_EVS_PORT_DEFAULT 0x0001

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
#define CFE_PLATFORM_EVS_DEFAULT_TYPE_FLAG 0xE

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
#define CFE_PLATFORM_EVS_DEFAULT_LOG_MODE 1

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
#define CFE_PLATFORM_TBL_BUF_MEMORY_BYTES 524288

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
#define CFE_PLATFORM_TBL_MAX_DBL_TABLE_SIZE 16384

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
#define CFE_PLATFORM_TBL_MAX_SNGL_TABLE_SIZE 16384

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
#define CFE_PLATFORM_TBL_MAX_NUM_TABLES 128

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
#define CFE_PLATFORM_TBL_MAX_CRITICAL_TABLES 32

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
#define CFE_PLATFORM_TBL_MAX_NUM_HANDLES 256

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
#define CFE_PLATFORM_TBL_MAX_SIMULTANEOUS_LOADS 4

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
#define CFE_PLATFORM_TBL_MAX_NUM_VALIDATIONS 10

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
#define CFE_PLATFORM_TBL_DEFAULT_REG_DUMP_FILE "/ram/cfe_tbl_reg.log"

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
#define CFE_PLATFORM_TBL_VALID_SCID_COUNT 0

/* macro to construct 32 bit value from 4 chars */
#define CFE_PLATFORM_TBL_U32FROM4CHARS(_C1, _C2, _C3, _C4) \
    ((uint32)(_C1) << 24 | (uint32)(_C2) << 16 | (uint32)(_C3) << 8 | (uint32)(_C4))

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
#define CFE_PLATFORM_TBL_VALID_SCID_1 (0x42)
#define CFE_PLATFORM_TBL_VALID_SCID_2 (CFE_PLATFORM_TBL_U32FROM4CHARS('a', 'b', 'c', 'd'))

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
#define CFE_PLATFORM_TBL_VALID_PRID_COUNT 0

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
#define CFE_PLATFORM_TBL_VALID_PRID_1 (1)
#define CFE_PLATFORM_TBL_VALID_PRID_2 (CFE_PLATFORM_TBL_U32FROM4CHARS('a', 'b', 'c', 'd'))
#define CFE_PLATFORM_TBL_VALID_PRID_3 0
#define CFE_PLATFORM_TBL_VALID_PRID_4 0

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
#define CFE_PLATFORM_ES_STARTUP_SYNC_POLL_MSEC 50

/** \cfeescfg CFE core application startup timeout
**
**  \par Description:
**      The upper limit for the amount of time that the cFE core applications
**      (ES, SB, EVS, TIME, TBL) are each allotted to reach their respective
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
#define CFE_PLATFORM_CORE_MAX_STARTUP_MSEC 30000

/** \cfeescfg Startup script timeout
**
**  \par Description:
**      The upper limit for the total amount of time that all apps listed in the CFE ES startup
**      script may take to all become ready.
**
**      Unlike the "core" app timeout, this is a soft limit; if the allotted time is exceeded,
**      it probably indicates an issue with one of the apps, but does not cause CFE ES to take
**      any additional action other than logging the event to the syslog.
**
**      Units are in milliseconds
**
**  \par Limits:
**       Must be defined as an integer value that is greater than
**       or equal to zero.
*/
#define CFE_PLATFORM_ES_STARTUP_SCRIPT_TIMEOUT_MSEC 1000

#endif /* CPU1_PLATFORM_CFG_H */
