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
 * \file cfe_psp_eeprom_stub.c
 *
 *  Created on: Jul 17, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * This is a stub implementation of the PSP EEPROM API calls that return CFE_PSP_ERROR_NOT_IMPLEMENTED
 */

#include "cfe_psp.h"
#include "cfe_psp_module.h"

CFE_PSP_MODULE_DECLARE_SIMPLE(eeprom_stub);

void eeprom_stub_Init(uint32 PspModuleId)
{
    /* Nothing to init */
}


int32 CFE_PSP_EepromWrite32( cpuaddr MemoryAddress, uint32 uint32Value )
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromWrite16( cpuaddr MemoryAddress, uint16 uint16Value )
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromWrite8( cpuaddr MemoryAddress, uint8 ByteValue )
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromWriteEnable(uint32 Bank)
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromWriteDisable(uint32 Bank)
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromPowerUp(uint32 Bank)
{
    return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

int32 CFE_PSP_EepromPowerDown(uint32 Bank)
{
   return(CFE_PSP_ERROR_NOT_IMPLEMENTED);
}

