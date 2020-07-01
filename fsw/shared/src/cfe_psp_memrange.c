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
** File   :	cfe_psp_memrange.c
**
** Author :	Alan Cudmore
**
** Purpose:
**		   This file contains the memory range functions for the cFE Platform Support Package.
**       The memory range is a table of valid memory address ranges maintained by the cFE.
**
**
*/

/*
** Include section
*/

#include "cfe_psp.h"

/*
 * The "extern" declaration for the MemRange table is in the configdata header
 */
#include "cfe_psp_config.h"
#include "cfe_psp_configdata.h"

/*
** Name: CFE_PSP_MemValidateRange
**
** Purpose:
**		Validate the memory range and type using the global CFE_PSP_MemoryTable
**
** Assumptions and Notes:
**
** Parameters: 
**    Address -- A 32 bit starting address of the memory range
**    Size    -- A 32 bit size of the memory range ( Address + Size = End Address )
**    MemoryType -- The memory type to validate, including but not limited to:
**              CFE_PSP_MEM_RAM, CFE_PSP_MEM_EEPROM, or CFE_PSP_MEM_ANY
**              Any defined CFE_PSP_MEM_* enumeration can be specified
**
** Global Inputs: None
**
** Global Outputs: None
**
** Return Values:
**   CFE_PSP_SUCCESS -- Memory range and type information is valid and can be used.
**   CFE_PSP_INVALID_MEM_ADDR -- Starting address is not valid
**   CFE_PSP_INVALID_MEM_TYPE -- Memory type associated with the range does not match the passed in type.
**   CFE_PSP_INVALID_MEM_RANGE -- The Memory range associated with the address is not large enough to contain
**                            Address + Size.
*/
int32 CFE_PSP_MemValidateRange(cpuaddr Address, uint32 Size, uint32 MemoryType)
{
   cpuaddr StartAddressToTest = Address;
   cpuaddr EndAddressToTest = Address + Size - 1;
   cpuaddr StartAddressInTable;
   cpuaddr EndAddressInTable;
   uint32 TypeInTable;
   int32  ReturnCode = CFE_PSP_INVALID_MEM_ADDR;
   uint32 i;
   
   /*
   ** Before searching table, do a preliminary parameter validation
   */
   if ( MemoryType != CFE_PSP_MEM_ANY && MemoryType != CFE_PSP_MEM_RAM && MemoryType != CFE_PSP_MEM_EEPROM )
   {
      return(CFE_PSP_INVALID_MEM_TYPE);
   }
 
   if ( EndAddressToTest < StartAddressToTest )
   {
      return(CFE_PSP_INVALID_MEM_RANGE);
   }
   
   for ( i = 0; i < CFE_PSP_MEM_TABLE_SIZE; i++ )
   {
      /*
      ** Only look at valid memory table entries
      */
      if ( CFE_PSP_MemoryTable[i].MemoryType != CFE_PSP_MEM_INVALID )
      {
         StartAddressInTable = CFE_PSP_MemoryTable[i].StartAddr;
         EndAddressInTable   = CFE_PSP_MemoryTable[i].StartAddr + CFE_PSP_MemoryTable[i].Size - 1;
         TypeInTable         = CFE_PSP_MemoryTable[i].MemoryType;
      
         /*
         ** Step 1: Get the Address to Fit within the range
         */
         if (( StartAddressToTest >= StartAddressInTable ) && ( StartAddressToTest <= EndAddressInTable ))
         {
            /*
            ** Step 2: Does the End Address Fit within the Range?
            **         should not have to test the lower address,
            **         since the StartAddressToTest is already in the range.
            **         Can it be fooled by overflowing the 32 bit int?
            */
            if ( EndAddressToTest <= EndAddressInTable )
            {
               /*
               ** Step 3: Is the type OK?
               */
               if ( MemoryType == CFE_PSP_MEM_ANY )
               {
                  ReturnCode = CFE_PSP_SUCCESS;
                  break;  /* The range is valid, break out of the loop */
               }
               else if ( MemoryType == CFE_PSP_MEM_RAM && TypeInTable == CFE_PSP_MEM_RAM )
               {
                  ReturnCode = CFE_PSP_SUCCESS;
                  break; /* The range is valid, break out of the loop */
               }   
               else if ( MemoryType == CFE_PSP_MEM_EEPROM && TypeInTable == CFE_PSP_MEM_EEPROM )
               {
                  ReturnCode = CFE_PSP_SUCCESS;
                  break; /* The range is valid, break out of the loop */
               }
               else
               {
                  ReturnCode = CFE_PSP_INVALID_MEM_TYPE;
                  /* The range is not valid, move to the next entry */
               }
            }
            else
            {
               ReturnCode = CFE_PSP_INVALID_MEM_RANGE;
               /* The range is not valid, move to the next entry */
            }
         }
         else
         {
            ReturnCode = CFE_PSP_INVALID_MEM_ADDR;
            /* The range is not valid, move to the next entry */
         }
      } /* End if MemoryType != CFE_PSP_MEM_INVALID */
      
   } /* End for */
   return(ReturnCode);
}

/*
** Name: CFE_PSP_MemRanges
**
** Purpose:
**		Return the number of memory ranges in the CFE_PSP_MemoryTable
**
** Assumptions and Notes:
**
** Parameters: 
**    None
**
** Global Inputs: None
**
** Global Outputs: None
**
** Return Values:
**   Positive integer number of entries in the memory range table
*/
uint32 CFE_PSP_MemRanges(void)
{
	return(CFE_PSP_MEM_TABLE_SIZE) ;
}


/*
** Name: CFE_PSP_MemRangeSet
**
** Purpose:
**		This function populates one of the records in the CFE_PSP_MemoryTable.
**
** Assumptions and Notes:
**    Because the table is fixed size, the entries are set by using the integer index.
**    No validation is done with the address or size. 
**
** Parameters: 
**    RangeNum --   A 32 bit integer ( starting with 0 ) specifying the MemoryTable entry.
**    MemoryType -- The memory type to validate, including but not limited to:
**              CFE_PSP_MEM_RAM, CFE_PSP_MEM_EEPROM, or CFE_PSP_MEM_ANY
**              Any defined CFE_PSP_MEM_* enumeration can be specified
**    Address --    A 32 bit starting address of the memory range
**    Size    --    A 32 bit size of the memory range ( Address + Size = End Address )
**    WordSize --   The minimum addressable size of the range:
**                     ( CFE_PSP_MEM_SIZE_BYTE, CFE_PSP_MEM_SIZE_WORD, CFE_PSP_MEM_SIZE_DWORD )
**    Attributes -- The attributes of the Memory Range:
**                     (CFE_PSP_MEM_ATTR_WRITE, CFE_PSP_MEM_ATTR_READ, CFE_PSP_MEM_ATTR_READWRITE)
**
** Global Inputs: Reads CFE_PSP_MemoryTable
**
** Global Outputs: Changes CFE_PSP_MemoryTable
**
** Return Values:
**   CFE_PSP_SUCCESS -- Memory range set successfuly.
**   CFE_PSP_INVALID_MEM_RANGE -- The index into the table is invalid
**   CFE_PSP_INVALID_MEM_ADDR -- Starting address is not valid
**   CFE_PSP_INVALID_MEM_TYPE -- Memory type associated with the range does not match the passed in type.
**   OP_INVALID_MEM_SIZE -- The Memory range associated with the address is not large enough to contain
**                            Address + Size.
**   CFE_PSP_INVALID_MEM_WORDSIZE -- The WordSIze parameter is not one of the predefined types.
**   CFE_PSP_INVALID_MEM_ATTR -- The Attributes parameter is not one of the predefined types.
*/
int32  CFE_PSP_MemRangeSet      (uint32 RangeNum, uint32 MemoryType, cpuaddr StartAddr,
                                 uint32 Size,     uint32 WordSize,   uint32 Attributes)
{

   if ( RangeNum >= CFE_PSP_MEM_TABLE_SIZE )
   {
       return(CFE_PSP_INVALID_MEM_RANGE);
   }

   if ( ( MemoryType != CFE_PSP_MEM_RAM ) && ( MemoryType != CFE_PSP_MEM_EEPROM ) )
   {
       return(CFE_PSP_INVALID_MEM_TYPE);
   }
   
   if ( ( WordSize != CFE_PSP_MEM_SIZE_BYTE ) && ( WordSize != CFE_PSP_MEM_SIZE_WORD ) &&
        ( WordSize != CFE_PSP_MEM_SIZE_DWORD ) )
   {
       return(CFE_PSP_INVALID_MEM_WORDSIZE);
   }
   
   if ( ( Attributes != CFE_PSP_MEM_ATTR_READ ) && ( Attributes != CFE_PSP_MEM_ATTR_WRITE ) &&
        ( Attributes != CFE_PSP_MEM_ATTR_READWRITE ))
   {
      return(CFE_PSP_INVALID_MEM_ATTR);
   }
   
   /*
   ** Parameters check out, add the range
   */
   CFE_PSP_MemoryTable[RangeNum].MemoryType = MemoryType;
   CFE_PSP_MemoryTable[RangeNum].StartAddr = StartAddr;
   CFE_PSP_MemoryTable[RangeNum].Size = Size;
   CFE_PSP_MemoryTable[RangeNum].WordSize = WordSize;
   CFE_PSP_MemoryTable[RangeNum].Attributes = Attributes;
   
	return(CFE_PSP_SUCCESS) ;
}

/*
** Name: CFE_PSP_MemRangeGet
**
** Purpose:
**		This function retrieves one of the records in the CFE_PSP_MemoryTable.
**
** Assumptions and Notes:
**    Becasue the table is fixed size, the entries are accessed by using the integer index.
**
** Parameters: 
**    RangeNum --   A 32 bit integer ( starting with 0 ) specifying the MemoryTable entry.
**    *MemoryType -- A pointer to the 32 bit integer where the Memory Type is stored.
**                   Any defined CFE_PSP_MEM_* enumeration can be specified
**    *Address --    A pointer to the 32 bit integer where the 32 bit starting address of the memory range
**                   is stored.
**    *Size    --    A pointer to the 32 bit integer where the 32 bit size of the memory range 
**                   is stored.
**    *WordSize --   A pointer to the 32 bit integer where the the minimum addressable size of the range:
**                     ( CFE_PSP_MEM_SIZE_BYTE, CFE_PSP_MEM_SIZE_WORD, CFE_PSP_MEM_SIZE_DWORD )
**    Attributes -- The attributes of the Memory Range:
**                     (CFE_PSP_MEM_ATTR_WRITE, CFE_PSP_MEM_ATTR_READ, CFE_PSP_MEM_ATTR_READWRITE)
**
** Global Inputs: Reads CFE_PSP_MemoryTable
**
** Global Outputs: Changes CFE_PSP_MemoryTable
**
** Return Values:
**   CFE_PSP_SUCCESS -- Memory range returned successfuly.
**   CFE_PSP_INVALID_POINTER   -- Parameter error
**   CFE_PSP_INVALID_MEM_RANGE -- The index into the table is invalid
*/
int32  CFE_PSP_MemRangeGet      (uint32 RangeNum, uint32 *MemoryType, cpuaddr *StartAddr,
                            uint32 *Size,     uint32 *WordSize,   uint32 *Attributes)
{

   if ( MemoryType == NULL || StartAddr == NULL || Size == NULL || WordSize == NULL || Attributes == NULL )
   {
       return(CFE_PSP_INVALID_POINTER);
   }
   
   if ( RangeNum >= CFE_PSP_MEM_TABLE_SIZE )
   {
       return(CFE_PSP_INVALID_MEM_RANGE);
   }
   
   *MemoryType = CFE_PSP_MemoryTable[RangeNum].MemoryType;
   *StartAddr  = CFE_PSP_MemoryTable[RangeNum].StartAddr;
   *Size       = CFE_PSP_MemoryTable[RangeNum].Size;
   *WordSize   = CFE_PSP_MemoryTable[RangeNum].WordSize;
   *Attributes = CFE_PSP_MemoryTable[RangeNum].Attributes;
   
	return(CFE_PSP_SUCCESS) ;
}
