/*************************************************************************
** File:
**   $Id: cfs_utils.h 1.6 2015/03/02 18:30:04EST sstrege Exp  $
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
**   Specification for the CFS shared library functions.
**
** References:
**
**   $Log: cfs_utils.h  $
**   Revision 1.6 2015/03/02 18:30:04EST sstrege 
**   Added copyright information
**   Revision 1.5 2009/08/31 17:46:42EDT lwalling 
**   Add function CFS_VerifyString() to CFS Library
**   Revision 1.4 2009/05/26 13:31:07EDT lwalling 
**   Create common packet filter function in global CFS library
**   Revision 1.3 2008/10/06 09:52:08EDT rjmcgraw 
**   DCR4401:1 Removed extraneous tabs and unprintable characters
**   Revision 1.2 2008/05/15 09:39:41EDT rjmcgraw 
**   DCR2179:1 Changed doxygen link OS_SymLookup to OS_SymbolLookup
**   Revision 1.1 2008/05/13 13:35:14EDT rjmcgraw 
**   Initial revision
**   Member added to project cfs_lib
**  
*************************************************************************/
#ifndef _cfs_utils_h_
#define _cfs_utils_h_

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

/************************************************************************
** Type Definitions
*************************************************************************/
/** 
**  \brief Symbolic Address Type
*/
typedef struct
{
    uint32            Offset;/**< \brief Optional offset that is used as the
                                absolute address if the SymName string is NUL */
    char              SymName[OS_MAX_SYM_LEN];/**< \brief Symbol name string  */
} CFS_SymAddr_t;

/*************************************************************************
** Exported Functions
*************************************************************************/
/************************************************************************/
/** \brief Compute CRC from a file
**  
**  \par Description
**       This function will compute the cyclic redundancy check (CRC)
**       value of data in a file 
**
**  \par Assumptions, External Events, and Notes:
**       The computation starts from the current location of the
**       file pointer to the end of file
**       
**  \param [in]   FileHandle   The open file handle of the file to scan
**
**  \param [in]   CrcPtr       Where to store the computed CRC.
**
**  \param [in]   TypeCRC      One of the enumerated CRC types supported
**                             by the #CFE_ES_CalculateCRC function
**
**  \param [out]  *CrcPtr      The computed CRC. Only updated if the return
**                             value is #OS_SUCCESS
**
**  \returns
**  \retstmt Returns #OS_SUCCESS if no error \endcode
**  \retstmt The return code from #OS_read if an error occured \endcode
**  \endreturns
** 
**  \sa #CFE_ES_CalculateCRC, #OS_read
** 
*************************************************************************/
int32 CFS_ComputeCRCFromFile(uint32 FileHandle, 
                             uint32 *CrcPtr, 
                             uint32 TypeCRC);

/************************************************************************/
/** \brief Check for invalid filename
**  
**  \par Description
**       This function will scan a filename string for invalid characters 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   Filename   The file name string to scan
**
**  \param [in]   Length     The string length of Filename
** 
**  \returns
**  \retstmt Returns TRUE if the file name is valid          \endcode
**  \retstmt Returns FALSE if an invalid character was found \endcode
**  \endreturns
** 
*************************************************************************/
boolean CFS_IsValidFilename(char   *Filename, 
                            uint32 Length);
 
/************************************************************************/
/** \brief Resolve symbolic address
**  
**  \par Description
**       This routine will resolve a symbol name and optional address
**       offset to an absolute address
**
**  \par Assumptions, External Events, and Notes:
**       If the symbol name is a NUL (empty) string, then the offset
**       becomes the absolute address
**       
**  \param [in]   SymAddr          A #CFS_SymAddr_t pointer that holds
**                                 the symbol name and optional offset 
**
**  \param [in]   ResolvedAddr     A pointer to the location to store
**                                 the resolved address in
**
**  \param [out]  *ResolvedAddr    The fully resolved address. Only valid
**                                 if the return value is TRUE
* 
**  \returns
**  \retstmt Returns TRUE if the resolution was successful      \endcode
**  \retstmt Returns FALSE if the resolution was not successful \endcode
**  \endreturns
**
**  \sa #OS_SymbolLookup
**
*************************************************************************/
boolean CFS_ResolveSymAddr(CFS_SymAddr_t *SymAddr, 
                           uint32        *ResolvedAddr);

/************************************************************************/
/** \brief Verify 32 bit alignment
**  
**  \par Description
**       This routine will check an address and data size argument pair
**       for correct 32 bit alignment
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   Address   The address to check for proper alignment
**
**  \param [in]   Size      The size in bytes to check for proper
**                          alignment 
**
**  \returns
**  \retstmt Returns TRUE if both Address and Size are 32 bit aligned      \endcode
**  \retstmt Returns FALSE if either Address or Size is not 32 bit aligned \endcode
**  \endreturns
** 
**  \sa #CFS_Verify16Aligned
**
*************************************************************************/
boolean CFS_Verify32Aligned(uint32 Address, 
                            uint32 Size);

/************************************************************************/
/** \brief Verify 16 bit alignment
**  
**  \par Description
**       This routine will check an address and data size argument pair
**       for correct 16 bit alignment
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   Address   The address to check for proper alignment
**
**  \param [in]   Size      The size in bytes to check for proper
**                          alignment 
**
**  \returns
**  \retstmt Returns TRUE if both Address and Size are 16 bit aligned      \endcode
**  \retstmt Returns FALSE if either Address or Size is not 16 bit aligned \endcode
**  \endreturns
** 
**  \sa #CFS_Verify32Aligned
**
*************************************************************************/
boolean CFS_Verify16Aligned(uint32 Address, 
                            uint32 Size);

/************************************************************************/
/** \brief Determine whether Software Bus message packet is filtered
**  
**  \par Description
**       This routine will apply the CFS filter algorithm to the packet
**       to determine whether the packet should be filtered.  This same
**       algorithm may be used by multiple applications (DS, TO, etc.)
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr    Pointer to a Software Bus message packet
**
**  \param [in]   FilterType    Packet sequence count (1) or timestamp (2)
**
**  \param [in]   Algorithm_N   Algorithm parameter N "pass this many"
**  \param [in]   Algorithm_X   Algorithm parameter X "out of this many"
**  \param [in]   Algorithm_O   Algorithm parameter O "at this offset"
**
**  \returns
**  \retstmt Returns TRUE if the packet should be filtered (not used)  \endcode
**  \retstmt Returns FALSE if the packet should not be filtered (used) \endcode
**  \endreturns
** 
**  \sa #CFS_IsPacketFiltered
**
*************************************************************************/
boolean CFS_IsPacketFiltered(CFE_SB_MsgPtr_t MessagePtr,
                             uint16          FilterType,
                             uint16          Algorithm_N,
                             uint16          Algorithm_X,
                             uint16          Algorithm_O);

/*******************************************************************/
/*  \brief Verify string data
**  
**  \par Description
**       This common function performs the following evaluations:
**       - Is the buffer empty? (if yes, result OK if not required)
**       - Does string have a terminator? (if not, result BAD)
**       - Does filename have valid chars? (only filenames tested)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Pointer to buffer containing string
**  \param [in]  Length (in bytes) of buffer
**  \param [in]  Is string required or optional?
**  \param [in]  Is string part of a filename? (limited chars)
**
**  \sa #CFS_VerifyString
*/
boolean CFS_VerifyString(char *Buffer, int32 BufferSize,
                         boolean StringIsRequired, boolean NeedCharTest);

#endif /* _cfs_utils_h_ */

/************************/
/*  End of File Comment */
/************************/
