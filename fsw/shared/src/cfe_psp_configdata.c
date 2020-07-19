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
 * \file cfe_psp_configdata.c
 *
 *  Created on: Dec 31, 2014
 *      Author: joseph.p.hickey@nasa.gov
 */

#include <cfe_psp.h>
#include <cfe_psp_config.h>
#include <psp_version.h>
#include <cfe_psp_configdata.h>

/**
 * Instantiation of the PSP/HW configuration data
 *
 * Because this is compiled within the context of the PSP code,
 * this can access the internal PSP macros directly.  External
 * code such as CFE core or apps would not be able to \#include the
 * PSP cfe_psp_config.h or psp_version.h files
 */
Target_PspConfigData GLOBAL_PSP_CONFIGDATA =
{
      .PSP_WatchdogMin = CFE_PSP_WATCHDOG_MIN,
      .PSP_WatchdogMax = CFE_PSP_WATCHDOG_MAX,
      .PSP_MemTableSize = CFE_PSP_MEM_TABLE_SIZE,
      .PSP_MemoryTable = CFE_PSP_MemoryTable,

      .HW_NumEepromBanks = CFE_PSP_NUM_EEPROM_BANKS,

      .PSP_VersionInfo =
      {
            .MajorVersion = CFE_PSP_IMPL_MAJOR_VERSION,
            .MinorVersion = CFE_PSP_IMPL_MINOR_VERSION,
            .Revision = CFE_PSP_IMPL_REVISION,
            .MissionRev = CFE_PSP_IMPL_MISSION_REV,
            .Version = CFE_PSP_IMPL_VERSION
      }
};
