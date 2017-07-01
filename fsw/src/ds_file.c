/************************************************************************
**   $Id: ds_file.c 1.6 2017/01/25 12:05:45EST sstrege Exp  $
**
**  Copyright (c) 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
**  CFS Data Storage (DS) file functions
**
*************************************************************************/

#include "cfe.h"
#include "cfe_fs.h"

#include "cfs_utils.h"

#include "ds_platform_cfg.h"
#include "ds_verify.h"

#include "ds_appdefs.h"

#include "ds_msg.h"
#include "ds_app.h"
#include "ds_file.h"
#include "ds_table.h"
#include "ds_events.h"

#include "string.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileStorePacket() - store packet in file(s)                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_FileStorePacket(CFE_SB_MsgId_t MessageID, CFE_SB_MsgPtr_t MessagePtr)
{
    DS_PacketEntry_t *PacketEntry;
    DS_FilterParms_t *FilterParms;
    boolean PassedFilter;
    boolean FilterResult;
    int32 FilterIndex;
    int32 FileIndex;
    int32 i;

    /*
    ** Convert packet MessageID to packet filter table index...
    */
    FilterIndex = DS_TableFindMsgID(MessageID);

    /*
    ** Ignore packets not listed in the packet filter table...
    */
    if (FilterIndex == DS_INDEX_NONE)
    {
        DS_AppData.IgnoredPktCounter++;
    }
    else
    {
        PacketEntry = &DS_AppData.FilterTblPtr->Packet[FilterIndex];
        PassedFilter = FALSE;

        /*
        ** Each packet has multiple filters for multiple files...
        */
        for (i = 0; i < DS_FILTERS_PER_PACKET; i++)
        {
            FilterParms = &PacketEntry->Filter[i];

            /*
            ** Ignore unused and invalid filters...
            */
            if ((FilterParms->Algorithm_N != DS_UNUSED) &&
                (FilterParms->FileTableIndex < DS_DEST_FILE_CNT))
            {
                FileIndex = FilterParms->FileTableIndex;

                /*
                ** Ignore disabled destination files...
                */
                if (DS_AppData.FileStatus[FileIndex].FileState == DS_ENABLED)
                {
                    /*
                    ** Apply filter algorithm to the packet...
                    */
                    FilterResult = CFS_IsPacketFiltered(MessagePtr,
                                                        FilterParms->FilterType,
                                                        FilterParms->Algorithm_N,
                                                        FilterParms->Algorithm_X,
                                                        FilterParms->Algorithm_O);
                    if (FilterResult == FALSE)
                    {
                        /*
                        ** Write unfiltered packets to destination file...
                        */
                        DS_FileSetupWrite(FileIndex, MessagePtr);
                        PassedFilter = TRUE;
                    }
                }
            }
        }

        /*
        ** Count packet as passed if any filters passed...
        */
        if (PassedFilter)
        {
            DS_AppData.PassedPktCounter++;
        }
        else
        {
            DS_AppData.FilteredPktCounter++;
        }
    }


    return;

} /* End of DS_FileStorePacket() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileSetupWrite() - prepare to write packet data to file      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_FileSetupWrite(int32 FileIndex, CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestFileEntry_t *DestFile = &DS_AppData.DestFileTblPtr->File[FileIndex];
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    boolean OpenNewFile = FALSE;
    uint16 PacketLength;

    #if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)
    /*
    ** Store packet time for GPM style file headers...
    */
    CFE_TIME_SysTime_t PacketTime = CFE_SB_GetMsgTime(MessagePtr);
    DS_AppData.CurrentPktTime = PacketTime.Seconds;
    #endif

    /*
    ** Create local pointers for array indexed data...
    */
    PacketLength = CFE_SB_GetTotalMsgLength(MessagePtr);

    if (FileStatus->FileHandle == DS_CLOSED_FILE_HANDLE)
    {
        /*
        ** 1st packet since destination enabled or file closed...
        */
        OpenNewFile = TRUE;
    }
    else
    {
        /*
        ** Test size of existing destination file...
        */
        if ((FileStatus->FileSize + PacketLength) > DestFile->MaxFileSize)
        {
            /*
            ** This packet would cause file to exceed max size limit...
            */
            DS_FileUpdateHeader(FileIndex);
            DS_FileCloseDest(FileIndex);
            OpenNewFile = TRUE;
        }
        else
        {
            /*
            ** File size is OK - write packet data to file...
            */
            DS_FileWriteData(FileIndex, MessagePtr, PacketLength);
        }
    }

    if (OpenNewFile)
    {
        /*
        ** Either the file did not exist or we closed it because
        **   of the size limit test above...
        */
        DS_FileCreateDest(FileIndex);

        if (FileStatus->FileHandle != DS_CLOSED_FILE_HANDLE)
        {
            /*
            ** By writing the first packet without first performing a size
            **   limit test, we avoid issues resulting from having the max
            **   file size set less than the size of one packet...
            */
            DS_FileWriteData(FileIndex, MessagePtr, PacketLength);
        }
    }

    /*
    ** If the write did not occur due to I/O error (create or write)
    **   then current state = file closed and destination disabled...
    */
    return;

} /* End of DS_FileSetupWrite() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileWriteData() - write data to destination file             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_FileWriteData(int32 FileIndex, void *FileData, uint32 DataLength)
{
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    int32 Result;

    /*
    ** Let cFE manage the file I/O...
    */
    Result = OS_write(FileStatus->FileHandle, FileData, DataLength);

    if (Result == DataLength)
    {
        /*
        ** Success - update file size and data rate counters...
        */
        DS_AppData.FileWriteCounter++;

        FileStatus->FileSize   += DataLength;
        FileStatus->FileGrowth += DataLength;

        #if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)
        /*
        ** Current pkt time is now last pkt time for this file...
        */
        DS_AppData.LastPktTime[FileIndex] = DS_AppData.CurrentPktTime;
        #endif
    }
    else
    {
        /*
        ** Error - send event, close file and disable destination...
        */
        DS_FileWriteError(FileIndex, DataLength, Result);
    }

    return;

} /* End of DS_FileWriteData() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileWriteHeader() - write header to destination file         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_FileWriteHeader(int32 FileIndex)
{
    #if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE)

    DS_DestFileEntry_t *DestFile = &DS_AppData.DestFileTblPtr->File[FileIndex];
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    CFE_FS_Header_t CFE_FS_Header;
    DS_FileHeader_t DS_FileHeader;
    int32 Result;

    /*
    ** Initialize selected parts of the cFE file header...
    */
	CFE_PSP_MemSet(&CFE_FS_Header, 0, sizeof(CFE_FS_Header_t));
    CFE_FS_Header.SubType = DS_FILE_HDR_SUBTYPE;
    strcpy(CFE_FS_Header.Description, DS_FILE_HDR_DESCRIPTION);

    /*
    ** Let cFE finish the init and write the primary header...
    */
    Result = CFE_FS_WriteHeader(FileStatus->FileHandle, &CFE_FS_Header);

    if (Result == sizeof(CFE_FS_Header_t))
    {
        /*
        ** Success - update file size and data rate counters...
        */
        DS_AppData.FileWriteCounter++;

        FileStatus->FileSize   += sizeof(CFE_FS_Header_t);
        FileStatus->FileGrowth += sizeof(CFE_FS_Header_t);

        /*
        ** Initialize the DS file header...
        */
        CFE_PSP_MemSet(&DS_FileHeader, 0, sizeof(DS_FileHeader_t));
        DS_FileHeader.FileTableIndex = FileIndex;
        DS_FileHeader.FileNameType = DestFile->FileNameType;
        strcpy(DS_FileHeader.FileName, FileStatus->FileName);

        /*
        ** Manually write the secondary header...
        */
        Result = OS_write(FileStatus->FileHandle, &DS_FileHeader, sizeof(DS_FileHeader_t));

        if (Result == sizeof(DS_FileHeader_t))
        {
            /*
            ** Success - update file size and data rate counters...
            */
            DS_AppData.FileWriteCounter++;

            FileStatus->FileSize   += sizeof(DS_FileHeader_t);
            FileStatus->FileGrowth += sizeof(DS_FileHeader_t);
        }
        else
        {
            /*
            ** Error - send event, close file and disable destination...
            */
            DS_FileWriteError(FileIndex, sizeof(DS_FileHeader_t), Result);
        }
    }
    else
    {
        /*
        ** Error - send event, close file and disable destination...
        */
        DS_FileWriteError(FileIndex, sizeof(CFE_FS_Header_t), Result);
    }

    #elif (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)

    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    DS_FileHeaderGPM_t DS_FileHeaderGPM;
    int32 Result;

    /*
    ** Initialize GPM file header...
    */
	CFE_PSP_MemSet(&DS_FileHeaderGPM, ' ', sizeof(DS_FileHeaderGPM_t));

    DS_FileHeaderGPM.SourceID[0] = 'S';
    DS_FileHeaderGPM.SourceID[1] = 'C';
    DS_FileHeaderGPM.SourceID[2] = 'H';
    DS_FileHeaderGPM.SourceID[3] = 'K';

    DS_FileConvertGPM(DS_FileHeaderGPM.SequenceID, FileStatus->FileCount);

    DS_FileHeaderGPM.StartTime = DS_AppData.CurrentPktTime;

    /*
    ** Write GPM file header to the file...
    */
    Result = OS_write(FileStatus->FileHandle, &DS_FileHeaderGPM, sizeof(DS_FileHeaderGPM_t));

    if (Result == sizeof(DS_FileHeaderGPM_t))
    {
        /*
        ** Success - update file size and data rate counters...
        */
        DS_AppData.FileWriteCounter++;

        FileStatus->FileSize   += sizeof(DS_FileHeaderGPM_t);
        FileStatus->FileGrowth += sizeof(DS_FileHeaderGPM_t);
    }
    else
    {
        /*
        ** Error - send event, close file and disable destination...
        */
        DS_FileWriteError(FileIndex, sizeof(DS_FileHeaderGPM_t), Result);
    }
    #endif

    return;

} /* End of DS_FileWriteHeader() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileWriteError() - file write error handler                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void DS_FileWriteError(uint32 FileIndex, uint32 DataLength, int32 WriteResult)
{
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];

    /*
    ** Send event, close file and disable destination...
    */
    DS_AppData.FileWriteErrCounter++;

    CFE_EVS_SendEvent(DS_WRITE_FILE_ERR_EID, CFE_EVS_ERROR,
                     "FILE WRITE error: result = %d, length = %d, dest = %d, name = '%s'",
                      (int)WriteResult, (int)DataLength, (int)FileIndex, FileStatus->FileName);

    DS_FileCloseDest(FileIndex);

    FileStatus->FileState = DS_DISABLED;

    return;

} /* End of DS_FileWriteError() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileCreateDest() - create destination file                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void DS_FileCreateDest(uint32 FileIndex)
{
    DS_DestFileEntry_t *DestFile = &DS_AppData.DestFileTblPtr->File[FileIndex];
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    int32 Result;

    /*
    ** Create filename from "path + base + sequence count + extension"...
    */
    DS_FileCreateName(FileIndex);

    if (FileStatus->FileName[0] != DS_STRING_TERMINATOR)
    {
        /*
        ** Success - create a new destination file...
        */
        Result = OS_creat(FileStatus->FileName, OS_READ_WRITE);

        if (Result < 0)
        {
            /*
            ** Error - send event, disable destination and reset filename...
            */
            DS_AppData.FileWriteErrCounter++;

            CFE_EVS_SendEvent(DS_CREATE_FILE_ERR_EID, CFE_EVS_ERROR,
                             "FILE CREATE error: result = %d, dest = %d, name = '%s'",
                              (int)Result, (int)FileIndex, FileStatus->FileName);

            CFE_PSP_MemSet(FileStatus->FileName, 0, DS_TOTAL_FNAME_BUFSIZE);

            /*
            ** Something needs to get fixed before we try again...
            */
            FileStatus->FileState = DS_DISABLED;
        }
        else
        {
            /*
            ** Success - store the file handle...
            */
            DS_AppData.FileWriteCounter++;

            FileStatus->FileHandle = Result;

            /*
            ** Initialize and write config specific file header...
            */
            DS_FileWriteHeader(FileIndex);

            /*
            ** Update sequence count if have one and write successful...
            */
            if ((FileStatus->FileHandle != DS_CLOSED_FILE_HANDLE) &&
                (DestFile->FileNameType == DS_BY_COUNT))
            {
                FileStatus->FileCount++;
                if (FileStatus->FileCount > DS_MAX_SEQUENCE_COUNT)
                {
                    FileStatus->FileCount = DestFile->SequenceCount;
                }

                /*
                ** Update Critical Data Store (CDS)...
                */
                DS_TableUpdateCDS();
            }
        }
    }

    return;

} /* End of DS_FileCreateDest() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Create destination filename                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void DS_FileCreateName(uint32 FileIndex)
{
    DS_DestFileEntry_t *DestFile = &DS_AppData.DestFileTblPtr->File[FileIndex];
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    int32 TotalLength;

    char Workname[2 * DS_TOTAL_FNAME_BUFSIZE];
    char Sequence[DS_TOTAL_FNAME_BUFSIZE];

    Workname[0] = DS_STRING_TERMINATOR;
    Sequence[0] = DS_STRING_TERMINATOR;

    /*
    ** Start with the path portion of the filename...
    */
    strcpy(Workname, DestFile->Pathname);
    TotalLength = strlen(Workname);

    /*
    ** Add a path separator (if needed) before appending the base name...
    */

    if (Workname[TotalLength - 1] != DS_PATH_SEPARATOR)
    {
        Workname[TotalLength] = DS_PATH_SEPARATOR;
        Workname[TotalLength + 1] = DS_STRING_TERMINATOR;
    }

    /*
    ** Verify that the path plus the base portion is not too large...
    */
    if ((strlen(Workname) + strlen(DestFile->Basename)) < DS_TOTAL_FNAME_BUFSIZE)
    {
        /*
        ** Append the base portion to the path portion...
        */
        strcat(Workname, DestFile->Basename);

        /*
        ** Create the sequence portion of the filename...
        */
        DS_FileCreateSequence(Sequence, DestFile->FileNameType, FileStatus->FileCount);

        /*
        ** Verify that the path/base plus the sequence portion is not too large...
        */
        if ((strlen(Workname) + strlen(Sequence)) < DS_TOTAL_FNAME_BUFSIZE)
        {
            /*
            ** Append the sequence portion to the path/base portion...
            */
            strcat(Workname, Sequence);

            /*
            ** Check for an optional file extension...
            */
            if (strlen(DestFile->Extension) > 0)
            {
                /*
                ** Add a "." character (if needed) before appending the extension...
                */
                if (DestFile->Extension[0] != '.')
                {
                    strcat(Workname, ".");
                }

                /*
                ** Append the extension portion to the path/base+sequence portion...
                */
                strcat(Workname, DestFile->Extension);
            }

            /*
            ** Final test - is "path/base+sequence.extension" length valid?...
            */
            if (strlen(Workname) < DS_TOTAL_FNAME_BUFSIZE)
            {
                /*
                ** Success - copy workname to filename buffer...
                */
                strcpy(FileStatus->FileName, Workname);
            }
        }
    }

    if (FileStatus->FileName[0] == DS_STRING_TERMINATOR)
    {
        /*
        ** Error - send event and disable destination...
        */
        CFE_EVS_SendEvent(DS_FILE_NAME_ERR_EID, CFE_EVS_ERROR,
           "FILE NAME error: dest = %d, path = '%s', base = '%s', seq = '%s', ext = '%s'",
                          (int)FileIndex, DestFile->Pathname, DestFile->Basename,
                          Sequence, DestFile->Extension);

        /*
        ** Something needs to get fixed before we try again...
        */
        DS_AppData.FileStatus[FileIndex].FileState = DS_DISABLED;
    }

    return;

} /* End of DS_FileCreateName() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileCreateSequence() - set text from count or time           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void DS_FileCreateSequence(char *Buffer, uint32 Type, uint32 Count)
{
    CFE_TIME_SysTime_t TimeToPrint;

    uint32 SequenceCount;
    uint32 NumericDigit;

    int32 BufferIndex;

    /*
    ** Build the sequence portion of the filename (time or count)...
    */
    if (Type == DS_BY_COUNT)
    {
        /*
        ** Get copy of sequence count that can be modified...
        */
        SequenceCount = Count;

        /*
        ** Extract each digit (least significant digit first)...
        */
        for (BufferIndex = DS_SEQUENCE_DIGITS - 1; BufferIndex >= 0; BufferIndex--)
        {
            /*
            ** Extract this digit and prepare for next digit...
            */
            NumericDigit  = SequenceCount % 10;
            SequenceCount = SequenceCount / 10;

            /*
            ** Store this digit as ASCII in sequence string buffer...
            */
            Buffer[BufferIndex] = '0' + NumericDigit;
        }

        /*
        ** Add string terminator...
        */
        Buffer[DS_SEQUENCE_DIGITS] = DS_STRING_TERMINATOR;
    }
    else if (Type == DS_BY_TIME)
    {

        #if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)
        /*
        ** Filename is based on seconds from current packet timestamp...
        */
        TimeToPrint.Seconds = DS_AppData.CurrentPktTime;
        TimeToPrint.Subseconds = 0;
        #else
        /*
        ** Filename is based on seconds from current time...
        */
        TimeToPrint = CFE_TIME_GetTime();
        #endif

        /*
        ** Convert time value to cFE format text string...
        */
        CFE_TIME_Print(Buffer, TimeToPrint);

        /*
        ** cFE time string has format: "YYYY-DDD-HH:MM:SS.sssss"...
        */
        #define CFE_YYYY_INDEX  0
        #define CFE_DDD_INDEX   5
        #define CFE_HH_INDEX    9
        #define CFE_MM_INDEX   12
        #define CFE_SS_INDEX   15
        #define CFE_ssss_INDEX 18

        /*
        ** DS time string has format: "YYYYDDDHHMMSS"...
        */
        #define DS_YYYY_INDEX  0
        #define DS_DDD_INDEX   4
        #define DS_HH_INDEX    7
        #define DS_MM_INDEX    9
        #define DS_SS_INDEX   11
        #define DS_TERM_INDEX 13

        /*
        ** Convert cFE time string to DS time string by moving
        **  the cFE chars to the left to remove extra stuff...
        */

        /*
        ** Step 1: Leave "year" (YYYY) alone - it is already OK...
        */

        /*
        ** Step 2: Move "day of year" (DDD) next to (YYYY)...
        */
        Buffer[DS_DDD_INDEX + 0] = Buffer[CFE_DDD_INDEX + 0];
        Buffer[DS_DDD_INDEX + 1] = Buffer[CFE_DDD_INDEX + 1];
        Buffer[DS_DDD_INDEX + 2] = Buffer[CFE_DDD_INDEX + 2];

        /*
        ** Step 3: Move "hour of day" (HH) next to (DDD)...
        */
        Buffer[DS_HH_INDEX + 0] = Buffer[CFE_HH_INDEX + 0];
        Buffer[DS_HH_INDEX + 1] = Buffer[CFE_HH_INDEX + 1];

        /*
        ** Step 4: Move "minutes" (MM) next to (HH)...
        */
        Buffer[DS_MM_INDEX + 0] = Buffer[CFE_MM_INDEX + 0];
        Buffer[DS_MM_INDEX + 1] = Buffer[CFE_MM_INDEX + 1];

        /*
        ** Step 5: Move "seconds" (SS) next to (MM)...
        */
        Buffer[DS_SS_INDEX + 0] = Buffer[CFE_SS_INDEX + 0];
        Buffer[DS_SS_INDEX + 1] = Buffer[CFE_SS_INDEX + 1];

        /*
        ** Step 6: Skip "subsecs" (ssss) - not in DS format...
        */

        /*
        ** Step 7: Add string terminator...
        */
        Buffer[DS_TERM_INDEX] = DS_STRING_TERMINATOR;
    }
    else
    {
        /*
        ** Bad filename type, init buffer as empty...
        */
        Buffer[0] = DS_STRING_TERMINATOR;
    }

    return;

} /* End of DS_FileCreateSequence() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileUpdateHeader() - update destination file header          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_FileUpdateHeader(int32 FileIndex)
{
    #if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_CFE)

    /*
    ** Update CFE specific header fields...
    */
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    CFE_TIME_SysTime_t CurrentTime = CFE_TIME_GetTime();
    int32 Result;

    Result = OS_lseek(FileStatus->FileHandle, sizeof(CFE_FS_Header_t), SEEK_SET);

    if (Result == sizeof(CFE_FS_Header_t))
    {
        /* update file close time */
        Result = OS_write(FileStatus->FileHandle, &CurrentTime, sizeof(CFE_TIME_SysTime_t));

        if (Result == sizeof(CFE_TIME_SysTime_t))
        {
            DS_AppData.FileUpdateCounter++;
        }
        else
        {
            DS_AppData.FileUpdateErrCounter++;
        }
    }
    else
    {
        DS_AppData.FileUpdateErrCounter++;
    }

    #elif (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)

    /*
    ** Update GPM specific header fields...
    */
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];
    char Buf8[8];
    int32 Result;

    Result = OS_lseek(FileStatus->FileHandle, 16, SEEK_SET);

    if (Result == 16)
    {
        /* update last pkt time in file header */
        Result = OS_write(FileStatus->FileHandle, &DS_AppData.LastPktTime[FileIndex], sizeof(uint32));

        if (Result == sizeof(uint32))
        {
            /* update file size in file header */
            CFE_PSP_MemSet(Buf8, ' ', sizeof(Buf8));
            DS_FileConvertGPM(Buf8, FileStatus->FileSize);
            Result = OS_write(FileStatus->FileHandle, Buf8, sizeof(Buf8));

            if (Result == sizeof(Buf8))
            {
                DS_AppData.FileUpdateCounter++;
            }
            else
            {
                DS_AppData.FileUpdateErrCounter++;
            }
        }
        else
        {
            DS_AppData.FileUpdateErrCounter++;
        }
    }
    else
    {
        DS_AppData.FileUpdateErrCounter++;
    }

    #endif

    return;

} /* End of DS_FileUpdateHeader() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileCloseDest() - close destination file                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void DS_FileCloseDest(int32 FileIndex)
{
    DS_AppFileStatus_t *FileStatus = &DS_AppData.FileStatus[FileIndex];

    #if (DS_MOVE_FILES == TRUE)
    /*
    ** Move file from working directory to downlink directory...
    */
    int32 OS_result;
    int32 PathLength;
    char *FileName;
    char PathName[DS_TOTAL_FNAME_BUFSIZE];

    /*
    ** First, close the file...
    */
    OS_close(FileStatus->FileHandle);

    /*
    ** Move file only if table has a downlink directory name...
    */
    if (DS_AppData.DestFileTblPtr->File[FileIndex].Movename[0] != '\0')
    {
        /*
        ** Make sure directory name does not end with slash character...
        */
        strcpy(PathName, DS_AppData.DestFileTblPtr->File[FileIndex].Movename);
        PathLength = strlen(PathName);
        if (PathName[PathLength - 1] == '/')
        {
            PathName[PathLength - 1] = '\0';
            PathLength--;
        }

        /*
        ** Get a pointer to slash character before the filename...
        */
        FileName = strrchr(FileStatus->FileName, '/');

        /*
        ** Verify that directory name plus filename is not too large...
        */
        if ((PathLength + strlen(FileName)) < DS_TOTAL_FNAME_BUFSIZE)
        {
            /*
            ** Append the filename (with slash) to the directory name...
            */
            strcat(PathName, FileName);

            /*
            ** Use OS function to move/rename the file...
            */
            OS_result = OS_mv(FileStatus->FileName, PathName);
            /* OS_result = OS_rename(FileStatus->FileName, PathName); */

            if (OS_result != OS_SUCCESS)
            {
                /*
                ** Error - send event but leave destination enabled...
                */
                CFE_EVS_SendEvent(DS_MOVE_FILE_ERR_EID, CFE_EVS_ERROR,
                   "FILE MOVE error: src = '%s', tgt = '%s', result = %d",
                    FileStatus->FileName, PathName, OS_result);
            }
        }
        else
        {
            /*
            ** Error - send event but leave destination enabled...
            */
            CFE_EVS_SendEvent(DS_MOVE_FILE_ERR_EID, CFE_EVS_ERROR,
               "FILE MOVE error: dir name = '%s', filename = '%s'",
                PathName, FileName);
        }
    }
    #else
    /*
    ** Close the file...
    */
    OS_close(FileStatus->FileHandle);
    #endif

    /*
    ** Reset status for this destination file...
    */
    FileStatus->FileHandle = DS_CLOSED_FILE_HANDLE;
    FileStatus->FileAge  = 0;
    FileStatus->FileSize = 0;

    /*
    ** Remove previous filename from status data...
    */
    CFE_PSP_MemSet(FileStatus->FileName, 0, DS_TOTAL_FNAME_BUFSIZE);

    return;

} /* End of DS_FileCloseDest() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileTestAge() -- file age processor                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void DS_FileTestAge(uint32 ElapsedSeconds)
{
    uint32 FileIndex;

    /*
    ** Called from HK request command handler (elapsed = platform config)
    */
    if (DS_AppData.DestFileTblPtr != (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Cannot test file age without destination file table...
        */
        for (FileIndex = 0; FileIndex < DS_DEST_FILE_CNT; FileIndex++)
        {
            /*
            ** Update age of open files...
            */
            if (DS_AppData.FileStatus[FileIndex].FileHandle != DS_CLOSED_FILE_HANDLE)
            {
                DS_AppData.FileStatus[FileIndex].FileAge += ElapsedSeconds;

                if (DS_AppData.FileStatus[FileIndex].FileAge >=
                    DS_AppData.DestFileTblPtr->File[FileIndex].MaxFileAge)
                {
                    /*
                    ** Close files that exceed maximum file age...
                    */
                    DS_FileUpdateHeader(FileIndex);
                    DS_FileCloseDest(FileIndex);
                }
            }
        }
    }

    return;

} /* End of DS_FileTestAge() */


#if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_FileConvertGPM() - convert GPM file header value to ASCII    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_FileConvertGPM(char *Buffer, uint32 Value)
{
    uint32 LocalValue = Value;
    int i;

    /* limit value to max of 8 characters */
    if (LocalValue > 99999999)
    {
        LocalValue = 99999999;
    }

    /* start with low order digits */
    for (i = 7; i >= 0; i--)
    {
        /* save lowest digit as ASCII */
        Buffer[i] = '0' + (LocalValue % 10);

        /* remove lowest digit from value */
        LocalValue = LocalValue / 10;
    }

    return;

} /* End of DS_FileConvertGPM() */

#endif


/************************/
/*  End of File Comment */
/************************/

