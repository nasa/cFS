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

/*
** File   :	cfe_psp_memram.c
**
** Author :	Ezra Yeheskeli
**
** Purpose:
**		   This file  contains some of the cFE Platform Support Layer.
**         It contains the processor architecture specific calls.
**
**  16-Nov-2003 Ezra Yeheskeli
**          - First Creation.
**
*/

/*
** Include section
*/

#include "cfe_psp.h"


/*
** global memory
*/

/*
 ** Name: CFE_PSP_MemRead8
 **
 ** Purpose:
 **         Read one byte of memory.
 **
 **
 ** Parameters:
 **	MemoryAddress : Address to be read
 **  ByteValue  : The address content will be copied to the location pointed by this argument
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 **
 ** Return Values: CFE_PSP_SUCCESS
 */
int32 CFE_PSP_MemRead8( cpuaddr MemoryAddress, uint8 *ByteValue )
{
    
	(*ByteValue) = *((uint8 *)MemoryAddress) ;
    
	return(CFE_PSP_SUCCESS) ;
}

/*
 ** Name: CFE_PSP_MemWrite8
 **
 ** Purpose:
 **         Write one byte of memory.
 **
 **
 ** Parameters:
 **	MemoryAddress : Address to be written to
 **  ByteValue  : The content pointed by this argument will be copied to the address
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 **
 ** Return Values:
 **		CFE_PSP_SUCCESS
 */
int32 CFE_PSP_MemWrite8 ( cpuaddr MemoryAddress, uint8 ByteValue )
{
    *((uint8 *)MemoryAddress) = ByteValue;
	return(CFE_PSP_SUCCESS) ;
    
}

/*
 ** Name: CFE_PSP_MemRead16
 **
 ** Purpose:
 **         Read  2 bytes of memory.
 **
 **
 ** Parameters:
 **	MemoryAddress : Address to be read
 **  uint16Value : The address content will be copied to the location pointed by
 **            this argument
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 **
 ** Return Values:
 **		CFE_PSP_SUCCESS
 **		CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit
 **      addressing scheme.
 */
int32 CFE_PSP_MemRead16( cpuaddr MemoryAddress, uint16 *uint16Value )
{
	/* check 16 bit alignment  , check the 1st lsb */
	if( MemoryAddress & 0x00000001)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}
	(*uint16Value) = *((uint16 *)MemoryAddress) ;
   return(CFE_PSP_SUCCESS) ;
    
}
/*
 ** Name: CFE_PSP_MemWrite16
 **
 ** Purpose:
 **         Write 2 byte of memory.
 **
 **
 ** Parameters:
 **	MemoryAddress : Address to be written to
 **  uint16Value : The content pointed by this argument will be copied to the
 **            address
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 **
 ** Return Values:
 **      CFE_PSP_SUCCESS
 **		CFE_PSP_ERROR_ADD_MISALIGNED The MemoryAddress is not aligned to 16 bit
 **      addressing scheme.
 */
int32 CFE_PSP_MemWrite16 ( cpuaddr MemoryAddress, uint16 uint16Value )
{
	/* check 16 bit alignment  , check the 1st lsb */
	if( MemoryAddress & 0x00000001)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}
   *((uint16 *)MemoryAddress) = uint16Value;
	return(CFE_PSP_SUCCESS) ;
}
/*
 ** Name: CFE_PSP_MemRead32
 **
 ** Purpose:
 **         Read 4 bytes of memory.
 **
 **
 ** Parameters:
 **	MemoryAddress : Address to be read
 **  uint32Value : The address content will be copied to the location pointed by
 **            this argument
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 **
 ** Return Values:
 **		CFE_PSP_SUCCESS
 **		CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit
 **      addressing scheme.
 */
int32 CFE_PSP_MemRead32( cpuaddr MemoryAddress, uint32 *uint32Value )
{    
	/* check 32 bit alignment  */
	if( MemoryAddress & 0x00000003)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}
	(*uint32Value) = *((uint32 *)MemoryAddress) ;
    
	return(CFE_PSP_SUCCESS) ;
}

/*
 ** Name: CFE_PSP_MemWrite32
 **
 ** Purpose:
 **         Write 4 byte of memory.
 **
 **
 ** Parameters:
 **	MemoryAddress : Address to be written to
 **  uint32Value : The content pointed by this argument will be copied to the
 **            address
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 **
 ** Return Values:
 **		CFE_PSP_SUCCESS
 **		CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit
 **      addressing scheme.
 */
int32 CFE_PSP_MemWrite32 ( cpuaddr MemoryAddress, uint32 uint32Value )
{
    
	/* check 32 bit alignment  */
	if( MemoryAddress & 0x00000003)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}

    *((uint32 *)MemoryAddress) = uint32Value;

	return(CFE_PSP_SUCCESS) ;
}



