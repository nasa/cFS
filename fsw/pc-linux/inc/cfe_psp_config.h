/*
**  GSC-18128-1, "Core Flight Executive Version 6.6"
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

/*
** cfe_psp_config.h
*/

#ifndef _cfe_psp_config_
#define _cfe_psp_config_



#include "common_types.h"


/*
** This define sets the number of memory ranges that are defined in the memory range defintion
** table.
*/
#define CFE_PSP_MEM_TABLE_SIZE 10


/*
** Processor Context type. 
** This is needed to determine the size of the context entry in the ER log.
** Although this file is in a CPU directory, it really is OS dependant, so supporting
** multiple OSs on the same CPU architecture ( i.e. x86/linux, x86/windows, x86/osx ) 
** will require IFDEFS. 
*/
typedef struct 
{
   uint32   regs[32];
  
} CFE_PSP_ExceptionContext_t;

#define CFE_PSP_CPU_CONTEXT_SIZE (sizeof(CFE_PSP_ExceptionContext_t))


/*
** Watchdog minimum and maximum values ( in milliseconds )
*/
#define CFE_PSP_WATCHDOG_MIN (0)
#define CFE_PSP_WATCHDOG_MAX (0xFFFFFFFF) 

/*
** Number of EEPROM banks on this platform
*/
#define CFE_PSP_NUM_EEPROM_BANKS 1

#endif

