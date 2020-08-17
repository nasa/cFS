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
 * \file cfe_psp_configdata.h
 *
 *  Created on: Dec 31, 2014
 *      Author: joseph.p.hickey@nasa.gov
 */

#ifndef CFE_PSP_CONFIG_H_
#define CFE_PSP_CONFIG_H_

/* osapi.h is required for the definition of OS_VolumeInto_t */
#include <osapi.h>
/* cfe_psp.h is required for the definition of CFE_PSP_MemTable_t */
#include <cfe_psp.h>

/*
** PSP software version record
** (replaces PSP_*_VERSION macros)
*/
typedef const struct
{
   uint8 MajorVersion;
   uint8 MinorVersion;
   uint8 Revision;
   uint8 MissionRev;
   char  Version[16];
} CFE_PSP_VersionInfo_t;



/**
 * PSP/Hardware configuration parameters
 * This structure should be instantiated by the PSP according
 * such that other modules do not need to directly include
 * the PSP configuration at compile time.
 */
typedef const struct
{
   uint32 PSP_WatchdogMin;                /**< PSP Minimum watchdog in milliseconds */
   uint32 PSP_WatchdogMax;                /**< PSP Maximum watchdog in milliseconds */
   uint32 PSP_MemTableSize;               /**< Size of PSP memory table */
   uint32 PSP_ExceptionLogSize;           /**< Size of PSP exception log */
   CFE_PSP_MemTable_t *PSP_MemoryTable;   /**< Pointer to PSP memory table (forward reference) */

   /**
    * Number of EEPROM banks on this platform
    */
   uint32 HW_NumEepromBanks;

   CFE_PSP_VersionInfo_t   PSP_VersionInfo;

} Target_PspConfigData;

/**
 * Extern reference to psp config struct.
 * Allows the actual instantiation to be done outside this module
 */
extern Target_PspConfigData GLOBAL_PSP_CONFIGDATA;

/**
 * Extern reference to the psp memory table
 * Allows the actual instantiation to be done outside this module
 */
extern CFE_PSP_MemTable_t CFE_PSP_MemoryTable[];


#endif /* CFE_PSP_CONFIG_H_ */
