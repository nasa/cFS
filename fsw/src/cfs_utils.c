/*************************************************************************
** File:
**   $Id: cfs_utils.c 1.14 2015/03/02 18:30:05EST sstrege Exp  $
**
**   Copyright © 2007-2014 United States Government as represented by the 
**   Administrator of the National Aeronautics and Space Administration. 
**   All Other Rights Reserved.  
**
**   This software was created at NASA's Goddard Space Flight Center.
**   This software is governed by the NASA Open Source Agreement and may be 
**   used, distributed and modified only pursuant to the terms of that 
**   agreement.
**
** Purpose: 
**   Shared library functions for CFS applications
**
**   $Log: cfs_utils.c  $
**   Revision 1.14 2015/03/02 18:30:05EST sstrege 
**   Added copyright information
**   Revision 1.13 2011/05/20 13:24:20EDT lwalling 
**   Modify packet filter to match algorithm used by SDO, GPM, etc.
**   Revision 1.12 2010/09/21 14:36:34EDT wmoleski 
**   The sequence count in a ccsds packet header consists of the lower 14 bits. The code was using
**   the entire 16 bit value to determine whether the packet should be filtered. This was changed to
**   extract only the lower 14 bits for the sequence count.
**   Revision 1.11 2010/09/21 08:50:05EDT wmoleski 
**   Deleted the line of code that checked if the currect character was a '%' for the valid filename check function.
**   Revision 1.10 2009/08/31 17:46:41EDT lwalling 
**   Add function CFS_VerifyString() to CFS Library
**   Revision 1.9 2009/08/13 10:04:30EDT lwalling 
**   Change test for (N >= X) to (N > X) in CFS_IsPacketFiltered()
**   Revision 1.8 2009/05/26 13:31:07EDT lwalling 
**   Create common packet filter function in global CFS library
**   Revision 1.7 2008/10/06 09:38:25EDT rjmcgraw 
**   DCR4400:1 Changed return on CFS_LibInit from void to int32
**   Revision 1.6 2008/09/19 15:30:51EDT rjmcgraw 
**   DCR4337:1 Added #include version.h and display version after initialization 
**     is complete
**   Revision 1.5 2008/09/09 10:59:32EDT njyanchik 
**   I accidentally  updated the list of allowable characters with a list of 
**     not-allowed characters. I
**   need to fix it.
**   Revision 1.4 2008/08/08 15:38:01BST njyanchik 
**   Since there was already a function for detecting invalid file names, I 
**     updated the list of the
**   invalid characters to the list that is specified in the DCR.
**   Revision 1.3 2008/05/15 14:40:33BST rjmcgraw 
**   DCR2179:1 Changed fucntion call OS_SymLookup to OS_SymbolLookup
**   Revision 1.2 2008/05/13 13:47:02EDT rjmcgraw 
**   DCR2179:1 Changed include filename from cfs_lib.h to cfs_utils.h
**   Revision 1.1 2008/05/13 13:13:28EDT rjmcgraw 
**   Initial revision
**   Member added to CFS project 
** 
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "cfs_utils.h"
#include <ctype.h>
#include <string.h>
#include "cfs_lib_version.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/
/**
** \name Crc File Buffer Size  */ 
#define FILE_CRC_BUFFER_SIZE   200   /**< \brief Number of bytes per file 
                                          read when computing the crc from 
                                          a file */

#define CFS_STRING_TERMINATOR  '\0'  /**< \brief ASCIIZ string terminator character */

#define CFS_PKT_SEQUENCE_BASED_FILTER_TYPE  1
#define CFS_PKT_TIME_BASED_FILTER_TYPE      2

#define CFS_16_MSB_SUBSECS_SHIFT  16
#define CFS_11_LSB_SECONDS_MASK   0x07FF
#define CFS_11_LSB_SECONDS_SHIFT  4
#define CFS_4_MSB_SUBSECS_MASK    0xF000
#define CFS_4_MSB_SUBSECS_SHIFT   12

/*************************************************************************
** Private Function Prototypes
*************************************************************************/
int32 CFS_LibInit(void);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CFS Library Initialization Routine                              */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CFS_LibInit(void)
{
    
    OS_printf ("CFS Lib Initialized.  Version %d.%d.%d.%d",
                CFS_LIB_MAJOR_VERSION,
                CFS_LIB_MINOR_VERSION, 
                CFS_LIB_REVISION, 
                CFS_LIB_MISSION_REV);
                
    return OS_SUCCESS;
 
}/* End CFS_LibInit */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Compute the CRC of data in a file                               */ 
/* Operates from the current location of the file poiner to EOF    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 CFS_ComputeCRCFromFile(uint32 FileHandle, 
                             uint32 *CrcPtr, 
                             uint32 TypeCRC)
{
        int32   ByteCntr;
        int32   OS_Status = OS_SUCCESS;
        uint32  TempCrc   = 0;
static  uint8   DataArray[FILE_CRC_BUFFER_SIZE];

   do
   {
      /*
      ** Read in some data
      */
      ByteCntr = OS_read(FileHandle, DataArray, sizeof(DataArray));

      /*
      ** If we didn't hit end of file on the last read...
      */
      if (ByteCntr > 0)
      {
         /*
         ** Calculate the CRC based upon the previous CRC calculation
         */
         TempCrc = CFE_ES_CalculateCRC(DataArray, ByteCntr, TempCrc, TypeCRC);
      }
      
   }
   while(ByteCntr > 0);

   /*
   ** Check if we broke out of the loop because of an error return 
   ** from the OS_read call
   */
   if (ByteCntr < 0)
   {
      OS_Status = ByteCntr;
   }
   else
   {
      *CrcPtr = TempCrc;
   }
   
   return(OS_Status);
     
} /* End CFS_ComputeCRCFromFile */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check a file name string for undesirable characters             */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean CFS_IsValidFilename(char   *Filename, 
                            uint32 Length)
{
   boolean Valid = TRUE;
   int32   i;
   
   /*
   ** Test for a NUL string 
   */
   if(Filename[0] == '\0')
   {
      Valid = FALSE;
   }
   else
   {
       /*
        ** Scan string for disallowed characters
        */
       for(i=0; i<Length; i++)
       {
           if (isalnum((int)Filename[i]) 
                  || ( Filename[i] == '`' ) 
                  || ( Filename[i] == '~' ) 
                  || ( Filename[i] == '!' ) 
                  || ( Filename[i] == '@' ) 
                  || ( Filename[i] == '#' ) 
                  || ( Filename[i] == '$' ) 
                  || ( Filename[i] == '^' ) 
                  || ( Filename[i] == '&' )  
                  || ( Filename[i] == '_' ) 
                  || ( Filename[i] == '-' ) 
                  || ( Filename[i] == '/' )
                  || ( Filename[i] == '.' )
                  || ( Filename[i] == '+' )
                  || ( Filename[i] == '=' )
                  || ( Filename[i] == '\0'))
           {
            ;
           }
           else
           {
            Valid = FALSE;
            break;
           }
      
      } /* end for */
   
   } /* end else */
   
   return(Valid);
   
} /* End CFS_IsValidFilename */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Resolve symbol name and offset to absolute address              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */   
boolean CFS_ResolveSymAddr(CFS_SymAddr_t *SymAddr, 
                           uint32        *ResolvedAddr)
{
    boolean   Valid;   
    int32     OS_Status = OS_SUCCESS;
    
   /* 
   ** NUL terminate the very end of the symbol name string array as a
   ** safety measure
   */
   SymAddr->SymName[OS_MAX_SYM_LEN - 1] = '\0';
    
   /* 
   ** If the symbol name string is a nul string
   ** we use the offset as the absolute address
   */ 
   if(strlen(SymAddr->SymName) == 0)
    {
       *ResolvedAddr = SymAddr->Offset;
       Valid = TRUE;
    }
    else
    {
       /* 
       ** If symbol name is not an empty string look it up 
       ** using the OSAL API and add the offset if it succeeds
       */
       OS_Status = OS_SymbolLookup(ResolvedAddr, SymAddr->SymName);
       if (OS_Status == OS_SUCCESS)
        {
          *ResolvedAddr += SymAddr->Offset;
          Valid = TRUE;
        }       
       else
          Valid = FALSE;
    } 
   return (Valid);
   
}/* End CFS_ResolveSymAddr */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify an address and data size are 32 bit aligned              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */   
boolean CFS_Verify32Aligned(uint32 Address, 
                            uint32 Size)
{
   boolean IsAligned;
   
   if(Address % sizeof(uint32) != 0)
   {
      IsAligned = FALSE;
   }
   else if(Size % sizeof(uint32) != 0)
   {
      IsAligned = FALSE;
   }
   else
   {
      IsAligned = TRUE;
   }
  
   return(IsAligned);
   
} /* End CFS_Verify32Aligned */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify an address and data size are 16 bit aligned              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */   
boolean CFS_Verify16Aligned(uint32 Address, 
                            uint32 Size)
{
   boolean IsAligned;
     
   if(Address % sizeof(uint16) != 0)
   {
      IsAligned = FALSE;
   }
   else if(Size % sizeof(uint16) != 0)
   {
      IsAligned = FALSE;
   }
   else
   {
      IsAligned = TRUE;
   }
   
   return(IsAligned);
   
} /* End CFS_Verify16Aligned */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Apply common filter algorithm to Software Bus packet            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */   
boolean CFS_IsPacketFiltered(CFE_SB_MsgPtr_t MessagePtr,
                             uint16          FilterType,
                             uint16          Algorithm_N,
                             uint16          Algorithm_X,
                             uint16          Algorithm_O)
{        
    /* 
    ** Algorithm_N = the filter will pass this many packets
    ** Algorithm_X = out of every group of this many packets
    ** Algorithm_O = starting at this offset within the group
    */ 
    boolean PacketIsFiltered = FALSE;
    CFE_TIME_SysTime_t PacketTime;
    uint16 PacketValue;
    uint16 Seconds;
    uint16 Subsecs;

    /* 
    ** Verify input values (all errors = packet is filtered)...
    */ 
    if (Algorithm_X == 0)
    {
        /* 
        ** Group size of zero will result in divide by zero...
        */ 
        PacketIsFiltered = TRUE;
    }
    else if (Algorithm_N == 0)
    {
        /* 
        ** Pass count of zero will result in zero packets...
        */ 
        PacketIsFiltered = TRUE;
    }
    else if (Algorithm_N > Algorithm_X)
    {
        /* 
        ** Pass count cannot exceed group size...
        */ 
        PacketIsFiltered = TRUE;
    }
    else if (Algorithm_O >= Algorithm_X)
    {
        /* 
        ** Group offset must be less than group size...
        */ 
        PacketIsFiltered = TRUE;
    }
    else if ((FilterType != CFS_PKT_TIME_BASED_FILTER_TYPE) &&
             (FilterType != CFS_PKT_SEQUENCE_BASED_FILTER_TYPE))
    {
        /* 
        ** Invalid - unknown filter type...
        */ 
        PacketIsFiltered = TRUE;
    }
    else
    {
        if (FilterType == CFS_PKT_SEQUENCE_BASED_FILTER_TYPE)
        {
            /* 
            ** Create packet filter value from packet sequence count...
            */ 
            PacketValue = CCSDS_RD_SEQ(MessagePtr->Hdr);
        }
        else
        {
            /* 
            ** Create packet filter value from packet timestamp...
            */ 
            PacketTime = CFE_SB_GetMsgTime(MessagePtr);  
   
            /* 
            ** Get the least significant 11 bits of timestamp seconds...
            */ 
            Seconds = (uint16) PacketTime.Seconds;
            Seconds = Seconds & CFS_11_LSB_SECONDS_MASK;

            /* 
            ** Get the most significant 4 bits of timestamp subsecs...
            */ 
            Subsecs = (uint16) (PacketTime.Subseconds >> CFS_16_MSB_SUBSECS_SHIFT);
            Subsecs = Subsecs  & CFS_4_MSB_SUBSECS_MASK;

            /* 
            ** Shift seconds and subsecs to allow merge...
            */ 
            Seconds = Seconds << CFS_11_LSB_SECONDS_SHIFT;
            Subsecs = Subsecs >> CFS_4_MSB_SUBSECS_SHIFT;

            /* 
            ** Merge seconds and subsecs to create packet filter value...
            */ 
            PacketValue = Seconds | Subsecs;
        }

        /* 
        ** Apply the filter algorithm (common for both filter types)...
        */ 
        if (PacketValue < Algorithm_O)
        {
            /* 
            ** Value is less than offset of passed range...
            */ 
            PacketIsFiltered = TRUE;
        }
        else if (((PacketValue - Algorithm_O) % Algorithm_X) < Algorithm_N)
        {
            /* 
            ** This packet was passed by the filter algorithm...
            */ 
            PacketIsFiltered = FALSE;
        }
        else
        {
            /* 
            ** This packet was filtered by the filter algorithm...
            */ 
            PacketIsFiltered = TRUE;
        }
    }

    return(PacketIsFiltered);

} /* End of CFS_IsPacketFiltered() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CFS_VerifyString() - verify string data                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

boolean CFS_VerifyString(char *Buffer, int32 BufferSize,
                         boolean StringIsRequired, boolean NeedCharTest)
{
    boolean FoundTerminator;
    boolean CharsAreGood;
    boolean Result = FALSE;
    int32 i;

    if (Buffer[0] == CFS_STRING_TERMINATOR)
    {
        /*
        ** Buffer is empty...
        */
        if (StringIsRequired)
        {
            Result = FALSE;
        }
        else
        {
            Result = TRUE;
        }
    }
    else
    {
        /*
        ** Buffer is not empty...
        */
        FoundTerminator = FALSE;
        CharsAreGood = FALSE;

        for (i = 0; i < BufferSize; i++)
        {
            /*
            ** Find string terminator...
            */
            if (Buffer[i] == CFS_STRING_TERMINATOR)
            {
                FoundTerminator = TRUE;

                if (NeedCharTest)
                {
                    /*
                    ** Verify that chars are legal for filenames...
                    */
                    CharsAreGood = CFS_IsValidFilename(Buffer, i);
                }
                else
                {
                    /*
                    ** All chars are OK for descriptive text...
                    */
                    CharsAreGood = TRUE;
                }
                break;
            }
        }

        /*
        ** Verify that string has valid chars and a terminator...
        */
        Result = ((FoundTerminator) && (CharsAreGood));
    }

    return(Result);

} /* End of CFS_VerifyString() */


/************************/
/*  End of File Comment */
/************************/
