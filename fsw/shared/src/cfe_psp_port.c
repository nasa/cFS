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
** File   :	cfe_pep_memport.c
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
** Name: CFE_PSP_PortRead8
**
** Purpose:
**         Read one byte of memory.
**
**
** Parameters:
**	PortAddress : Address to be read
**  ByteValue  : The address content will be copied to the location pointed by
**            this argument
**
** Global Inputs: None
**
** Global Outputs: None
**
**
**
** Return Values: CFE_PSP_SUCCESS
*/
int32 CFE_PSP_PortRead8( cpuaddr PortAddress, uint8 *ByteValue )
{

	(*ByteValue) = (uint8)*((uint8 *)PortAddress) ;

	return(CFE_PSP_SUCCESS) ;
}

/*
** Name: CFE_PSP_PortWrite8
**
** Purpose:
**         Write one byte of memory.
**
**
** Parameters:
**	PortAddress : Address to be written to
**  ByteValue  : The content pointed by this argument will be copied to the
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
*/
int32 CFE_PSP_PortWrite8 ( cpuaddr PortAddress, uint8 ByteValue )
{
   *((uint8 *)PortAddress) = ByteValue;
	return(CFE_PSP_SUCCESS) ;

}

/*
** Name: CFE_PSP_PortRead16
**
** Purpose:
**         Read  2 bytes of memory.
**
**
** Parameters:
**	PortAddress : Address to be read
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
int32 CFE_PSP_PortRead16( cpuaddr PortAddress, uint16 *uint16Value )
{
	/* check 16 bit alignment  , check the 1st lsb */
	if( PortAddress & 0x00000001)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}
	(*uint16Value) = *((uint16 *)PortAddress) ;
	return(CFE_PSP_SUCCESS) ;

}


/*
** Name: CFE_PSP_PortWrite16
**
** Purpose:
**         Write 2 byte of memory.
**
**
** Parameters:
**	PortAddress : Address to be written to
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
**		CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit
**      addressing scheme.
*/
int32 CFE_PSP_PortWrite16 ( cpuaddr PortAddress, uint16 uint16Value )
{
	/* check 16 bit alignment  , check the 1st lsb */
	if( PortAddress & 0x00000001)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}
   *((uint16 *)PortAddress) = uint16Value;
	return(CFE_PSP_SUCCESS) ;
}

/*
** Name: CFE_PSP_PortRead32
**
** Purpose:
**         Read 4 bytes of memory.
**
**
** Parameters:
**	PortAddress : Address to be read
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
int32 CFE_PSP_PortRead32( cpuaddr PortAddress, uint32 *uint32Value )
{
	/* check 32 bit alignment  */
	if( PortAddress & 0x00000003)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}
	(*uint32Value) = *((uint32 *)PortAddress) ;
	return(CFE_PSP_SUCCESS) ;

}

/*
** Name: CFE_PSP_PortWrite32
**
** Purpose:
**         Write 4 byte of memory.
**
**
** Parameters:
**	PortAddress : Address to be written to
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
int32 CFE_PSP_PortWrite32 ( cpuaddr PortAddress, uint32 uint32Value )
{
	/* check 32 bit alignment  */
	if( PortAddress & 0x00000003)
	{
		return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
	}
   *((uint32 *)PortAddress) = uint32Value;
	return(CFE_PSP_SUCCESS) ;

}

