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
** File   :	cfe_psp_eeprom.c
**
** Author :	Ezra Yeheskeli
**
** Purpose:
**		   This file  contains some of the OS APIs abstraction layer.
**         It contains the processor architecture specific calls.
**
**  16-Nov-2003 Ezra Yeheskeli
**          - First Creation.
**
*/

/*
** Include section
*/
#include <stdio.h>

#include "cfe_psp.h"

/*
** global memory
*/

/*
 ** Name: CFE_PSP_EepromWrite32
 **
 ** Purpose:
 **
 ** Assumptions and Notes:
 **
 ** Parameters:
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 ** Return Values:
 **	 CFE_PSP_SUCCESS
 **	 CFE_PSP_ERROR_TIMEOUT write operation did not go through after a specific
 **   timeout.
 **	 CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit addressing
 **   scheme.
 */
int32 CFE_PSP_EepromWrite32( cpuaddr MemoryAddress, uint32 uint32Value )
{
   uint32 ret_value = CFE_PSP_SUCCESS;
    
   /* check 32 bit alignment  */
   if( MemoryAddress & 0x00000003)
   {
      return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
   }
   
   /* make the Write */
   *((uint32 *)MemoryAddress) = uint32Value;
	
   return(ret_value) ;
}

/*
 ** Name: CFE_PSP_EepromWrite16
 **
 ** Purpose:
 **
 ** Assumptions and Notes:
 **
 ** Parameters:
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 ** Return Values:
 **   CFE_PSP_SUCCESS
 **	 CFE_PSP_ERROR_TIMEOUT write operation did not go through after a specific
 **   timeout.
 **   CFE_PSP_ERROR_ADD_MISALIGNED The Address is not aligned to 16 bit addressing
 **   scheme.
 */
int32 CFE_PSP_EepromWrite16( cpuaddr MemoryAddress, uint16 uint16Value )
{
   uint32 write32;
   uint32 temp32;
   uint32 aligned_address ;
    
   /* 
   ** check 16 bit alignment  , check the 1st lsb 
   */
   if( MemoryAddress & 0x00000001)
   {
      return(CFE_PSP_ERROR_ADDRESS_MISALIGNED) ;
   }
    
   temp32 = uint16Value ;
	
#ifdef SOFTWARE_LITTLE_BIT_ORDER   
   /*
   ** Implementation for Little Endian architectures ( x86 )
   */

   /* 
   ** check the 2nd lsb to see if it's the 1st or 2nd 16 bit word
   */
   if( (MemoryAddress & 0x00000002) )
   {
      /* 
      ** writing the 16 high bit order of 32 bit field 
      */
      aligned_address = MemoryAddress - 2 ;
      CFE_PSP_MemRead32 ( aligned_address  ,&write32)  ;     
      write32 = (write32 & 0x0000FFFF) | (temp32 << 16 ) ;
   } 
   else
   {
      /* 
      ** writing the 16 low bit order of 32 bit field 
      */
      aligned_address = MemoryAddress;
      CFE_PSP_MemRead32 (  aligned_address, &write32 ) ;
      write32 = ( write32 & 0xFFFF0000 ) | ( temp32 );  
     
   }   

#else

   /*
   ** Implementation for Big Endian architectures (PPC, Coldfire )
   */
   /* 
   ** check the 2nd lsb to see if it's the 1st or 2nd 16 bit word
   */
   if( (MemoryAddress & 0x00000002) )
   {
      /* 
      ** writing the 16 high bit order of 32 bit field 
      */
      aligned_address = MemoryAddress - 2 ;
      CFE_PSP_MemRead32 ( aligned_address  ,&write32)  ;     
      write32 = (write32 & 0xFFFF0000) | (temp32) ;
   }
   else
   {
      /* 
      ** writing the 16 low bit order of 32 bit field 
      */
      aligned_address = MemoryAddress;
      CFE_PSP_MemRead32 (  aligned_address, &write32 ) ;
      write32 = ( write32 & 0x0000FFFF ) | ( temp32 << 16 );  
     
   }   
#endif	
	
   return(CFE_PSP_EepromWrite32(aligned_address,write32)) ;
    
}


/*
 ** Name: CFE_PSP_EepromWrite8
 **
 ** Purpose:
 **
 ** Assumptions and Notes:
 **
 ** Parameters:
 **
 ** Global Inputs: None
 **
 ** Global Outputs: None
 **
 **
 ** Return Values:
 **   CFE_PSP_SUCCESS
 **	 CFE_PSP_ERROR_TIMEOUT write operation did not go through after a specific
 **   timeout.
 */

int32 CFE_PSP_EepromWrite8( cpuaddr MemoryAddress, uint8 ByteValue )
{
   uint32 aligned_address ;
   uint16 write16 ,temp16;
     
   temp16 = ByteValue ;
 
 
#ifdef SOFTWARE_LITTLE_BIT_ORDER  
   /*
   ** Implementation for Little Endian architectures ( x86 )
   */
   /* 
   ** check the 1st lsb 
   */
   if( MemoryAddress & 0x00000001)
   {
      /* 
      ** writing the 8 high bit order of 16 bit field 
      */
      aligned_address = MemoryAddress - 1;
      CFE_PSP_MemRead16 ( aligned_address  ,&write16)  ;
      write16 = (write16 & 0x00FF) | ( temp16 << 8) ;
   }
   else
   {
      /* 
      ** writing the 8 low bit order of 16 bit field 
      */
      aligned_address = MemoryAddress ;
      CFE_PSP_MemRead16 (  aligned_address, &write16 ) ;
      write16 = (temp16 ) | (write16 & 0xFF00 ) ;
   }
#else
    
   /*
   ** Implementation for Big Endian architectures (PPC, Coldfire )
   */

   /* 
   ** check the 1st lsb 
   */
   if( MemoryAddress & 0x00000001)
   {
      /* 
      ** writing the 8 high bit order of 16 bit field 
      */
      aligned_address = MemoryAddress - 1;
      CFE_PSP_MemRead16 ( aligned_address  ,&write16)  ;
      write16 = (write16 & 0xFF00) | ( temp16) ;
   }
   else
   {
      /* 
      ** writing the 8 low bit order of 16 bit field 
      */
      aligned_address = MemoryAddress ;
      CFE_PSP_MemRead16 (  aligned_address, &write16 ) ;
      write16 = (temp16 << 8 ) | (write16 & 0x00FF ) ;
   }

#endif

   return(CFE_PSP_EepromWrite16(aligned_address,write16)) ;
    
}

/*
** Name: CFE_PSP_EepromWriteEnable
**
** Purpose:
**		Eable the eeprom for write operation
**
** Assumptions and Notes:
**
** Parameters:
**   Bank: Which bank of EEPROM 
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**   CFE_PSP_SUCCESS
*/
int32 CFE_PSP_EepromWriteEnable(uint32 Bank)
{
   return(CFE_PSP_SUCCESS) ;
}

/*
** Name: CFE_PSP_EepromWriteDisable
**
** Purpose:
**		Disable  the eeprom from write operation
**
** Assumptions and Notes:
**
** Parameters:
**   Bank: Which bank of EEPROM 
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**   CFE_PSP_SUCCESS
*/
int32 CFE_PSP_EepromWriteDisable(uint32 Bank)
{
   return(CFE_PSP_SUCCESS) ;
}


/*
** Name: CFE_PSP_EepromPowerUp
**
** Purpose:
**		Power up the eeprom
** Assumptions and Notes:
**
** Parameters:
**   Bank: Which bank of EEPROM 
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**   CFE_PSP_SUCCESS
*/
int32 CFE_PSP_EepromPowerUp(uint32 Bank)
{
   return(CFE_PSP_SUCCESS) ;
}



/*
** Name: CFE_PSP_EepromPowerDown
**
** Purpose:
**		Power down the eeprom
** Assumptions and Notes:
**
** Parameters:
**   Bank: Which bank of EEPROM 
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**   CFE_PSP_SUCCESS
*/
int32 CFE_PSP_EepromPowerDown(uint32 Bank)
{
   return(CFE_PSP_SUCCESS) ;
}

