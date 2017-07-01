/************************************************************************
**   $Id: ds_table.h 1.4 2017/01/25 12:05:34EST sstrege Exp  $
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
**  CFS Data Storage (DS) table definitions
**
*************************************************************************/

#ifndef _ds_table_h_
#define _ds_table_h_

#include "cfe.h"


#include "ds_platform_cfg.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS hash table structures and definitions                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define DS_HASH_TABLE_ENTRIES  256
#define DS_HASH_TABLE_MASK     0x00FF

/**  \brief DS Hash Table Linked List structure */
typedef struct DS_HashTag
{
    CFE_SB_MsgId_t      MessageID;     /**< \brief DS filter table entry MessageID */
    uint16              Index;         /**< \brief DS filter table entry index */

    struct DS_HashTag  *Next;          /**< \brief Next hash table linked list element */

} DS_HashLink_t;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS application table structures                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**  \brief DS Packet Filter Table structures */
typedef struct
{
	uint8               FileTableIndex;                        /**< \brief Index into Destination File Table */
    uint8               FilterType;                            /**< \brief Filter type (packet count or time) */

    uint16              Algorithm_N;                           /**< \brief Algorithm value N (pass this many) */
    uint16              Algorithm_X;                           /**< \brief Algorithm value X (out of this many) */
    uint16              Algorithm_O;                           /**< \brief Algorithm value O (at this offset) */

} DS_FilterParms_t;

typedef struct
{
    CFE_SB_MsgId_t      MessageID;                             /**< \brief Packet MessageID (may be cmd or tlm)
                                                                    \details DS defines ID zero to be unused */

    DS_FilterParms_t    Filter[DS_FILTERS_PER_PACKET];         /**< \brief One entry for each packet destination */

} DS_PacketEntry_t;

typedef struct
{
    char                Descriptor[DS_DESCRIPTOR_BUFSIZE];     /**< \brief Description such as "Safehold Filter Table" */
    DS_PacketEntry_t    Packet[DS_PACKETS_IN_FILTER_TABLE];    /**< \brief One entry for each filtered packet */

} DS_FilterTable_t;


/**  \brief DS Destination File Table structures 
     \details Note that the sum of the string buffer sizes exceeds the max for a qualified filename (OS_MAX_PATH_LEN).
     This allows a variable definition of how many characters may be used in the pathname versus the filename.
     The qualified filename length will be verified at run-time as each filename is created. */
typedef struct
{
#if (DS_MOVE_FILES == TRUE)
    char                Movename[DS_PATHNAME_BUFSIZE];         /**< \brief Move files to this dir after close */
#endif
    char                Pathname[DS_PATHNAME_BUFSIZE];         /**< \brief Path portion of filename */
    char                Basename[DS_BASENAME_BUFSIZE];         /**< \brief Base portion of filename */
    char                Extension[DS_EXTENSION_BUFSIZE];       /**< \brief Extension portion of filename */

    uint16              FileNameType;                          /**< \brief Filename type - count vs time */
    uint16              EnableState;                           /**< \brief File enable/disable state */

    uint32              MaxFileSize;                           /**< \brief Max file size (bytes) */
    uint32              MaxFileAge;                            /**< \brief Max file age (seconds) */

    uint32              SequenceCount;                         /**< \brief Sequence count portion of filename */

} DS_DestFileEntry_t;

typedef struct
{
    char                Descriptor[DS_DESCRIPTOR_BUFSIZE];     /**< \brief Description such as "HK Telemetry File" */
    DS_DestFileEntry_t  File[DS_DEST_FILE_CNT];                /**< \brief One entry for each destination data file */

} DS_DestFileTable_t;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Prototypes for functions defined in ds_table.c                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*******************************************************************/
/*  \brief Startup table initialization function
**  
**  \par Description
**       This function creates the Packet Filter and Destination
**       File tables. The function then tries to load default table
**       data for each table. If the Packet Filter Table load is
**       successful, then the function will subscribe to all of the
**       packets referenced by the table.
**
**  \par Called From:
**       - DS startup initialization function
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  (none)
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_DestFileEntry_t
*/
int32 DS_TableInit(void);


/*******************************************************************/
/*  \brief Manage destination file table loads, dumps, etc.
**  
**  \par Description
**       This function will provide cFE Table Services with an
**       opportunity to make updates to the Destination File Table
**       while in the context of the DS application. There is no
**       special handling necessary if a new version of this table
**       is loaded.
**
**  \par Called From:
**       - DS table initialization function
**       - DS housekeeping request command processor
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  (none)
**
**  \sa #DS_DestFileEntry_t, #DS_TableVerifyDestFile
*/
void  DS_TableManageDestFile(void);


/*******************************************************************/
/*  \brief Manage packet filter table loads, dumps, etc.
**  
**  \par Description
**       This function will provide cFE Table Services with an
**       opportunity to make updates to the Packet Filter Table
**       while in the context of the DS application. If a new
**       version of the filter table gets loaded, the function
**       will first unsubscribe to any packets referenced by the
**       previous version of the table, and then subscribe to
**       all packets referenced by the new version of the table.
**
**  \par Called From:
**       - DS table initialization function
**       - DS housekeeping request command processor
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  (none)
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_TableVerifyFilter
*/
void  DS_TableManageFilter(void);


/*******************************************************************/
/*  \brief Verify destination file table data
**  
**  \par Description
**       This function is called by cFE Table Services to verify
**       the contents of a candidate Destination File Table.  This
**       will occur while cFE Table Services is processing another
**       call from the DS application to manage the table. Thus,
**       this verification function will be executed from within
**       the context of the DS application.
**
**  \par Called From:
**       - cFE Table Services
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Pointer to Destination File Table data
**
**  \sa #DS_DestFileEntry_t, #DS_TableVerifyDestFileEntry
*/
int32 DS_TableVerifyDestFile(void *TableData);


/*******************************************************************/
/*  \brief Verify destination file table entry
**  
**  \par Description
**       This function is called from the Destination File Table
**       verification function to verify a single table entry.
**       This function, in turn, calls common utility functions
**       to verify each field within the selected table entry.
**
**  \par Called From:
**       - Destination File Table validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Pointer to selected Destination File Table entry
**  \param [in]  Index of selected Destination File Table entry
**  \param [in]  Previous verification result (event for 1st err)
**
**  \sa #DS_DestFileEntry_t, #DS_TableVerifyDestFile
*/
boolean DS_TableVerifyDestFileEntry(DS_DestFileEntry_t *DestFileEntry,
                                    uint8 TableIndex, int32 ErrorCount);


/*******************************************************************/
/*  \brief Verify packet filter table data
**  
**  \par Description
**       This function is called by cFE Table Services to verify
**       the contents of a candidate Packet Filter Table.  This
**       will occur while cFE Table Services is processing another
**       call from the DS application to manage the table. Thus,
**       this verification function will be executed from within
**       the context of the DS application.
**
**  \par Called From:
**       - cFE Table Services
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Pointer to Packet Filter Table data
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_TableVerifyFilterEntry
*/
int32 DS_TableVerifyFilter(void *TableData);


/*******************************************************************/
/*  \brief Verify packet filter table entry
**  
**  \par Description
**       This function is called from the Packet Filter Table
**       verification function to verify a single table entry.
**       This function, in turn, calls common utility functions
**       to verify each field within the selected table entry.
**
**  \par Called From:
**       - Packet Filter Table validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Pointer to a Packet Filter Table entry
**  \param [in]  Index of selected Packet Filter Table entry
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_TableVerifyFilter
*/
boolean DS_TableVerifyFilterEntry(DS_PacketEntry_t *PacketEntry,
                                  int32 TableIndex, int32 ErrorCount);


/*******************************************************************/
/*  \brief Test for unused table entry
**  
**  \par Description
**       This function returns TRUE if a table entry is unused.
**       Unused is defined as containing nothing but zero's.
**
**  \par Called From:
**       - Packet Filter Table validation function
**       - Destination File Table validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Pointer to the table entry data
**  \param [in]  Length of the table entry data
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_DestFileEntry_t
*/
boolean DS_TableEntryUnused(void *TableEntry, int32 BufferSize);


/*******************************************************************/
/*  \brief Verify destination file index
**  
**  \par Description
**       This function verifies that the indicated packet filter
**       table file table index is within bounds - as defined by
**       platform configuration parameters. Note
**       that the utility functions are also called from the ground
**       command handlers to verify command arguments that nodify
**       the table data.
**
**  \par Called From:
**       - Command handler (set file table index)
**       - Command handler (set filename type)
**       - Command handler (set file enable/disable state)
**       - Command handler (set pathname)
**       - Command handler (set basename)
**       - Command handler (set extension)
**       - Command handler (set max file size limit)
**       - Command handler (set max file age limit)
**       - Command handler (set file sequence count)
**       - Command handler (close file)
**       - Packet Filter Table entry validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Destination File Table Index value
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_DestFileEntry_t
*/
boolean DS_TableVerifyFileIndex(uint16 FileTableIndex);


/*******************************************************************/
/*  \brief Verify packet filter parameters
**  
**  \par Description
**       This function verifies that the indicated packet filter
**       table filter parameters are within bounds.
**       Algorithm N must be <= Algorithm X
**       Algorithm O must be <  Algorithm X
**
**  \par Called From:
**       - Command handler (set filter parms)
**       - Packet Filter Table entry validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Filter Algorithm N value
**  \param [in]  Filter Algorithm X value
**  \param [in]  Filter Algorithm O value
**
**  \sa #DS_TableVerifyType, #DS_TableVerifyState, #DS_DestFileEntry_t
*/
boolean DS_TableVerifyParms(uint16 Algorithm_N,
                            uint16 Algorithm_X,
                            uint16 Algorithm_O);


/*******************************************************************/
/*  \brief Verify packet filter type or filename type
**  
**  \par Description
**       This common function verifies that the indicated packet
**       filter table filter type, or destination file table
**       filename type is within bounds.
**       Must be DS_BY_COUNT or DS_BY_TIME.
**
**  \par Called From:
**       - Command handler (set filter type)
**       - Command handler (set filename type)
**       - Packet Filter Table entry validation function
**       - Destination File Table entry validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Filter Type or Filename Type value
**
**  \sa #DS_TableVerifyState, #DS_TableVerifySize, #DS_DestFileEntry_t
*/
boolean DS_TableVerifyType(uint16 TimeVsCount);


/*******************************************************************/
/*  \brief Verify application or destination file enable/disable state
**  
**  \par Description
**       This function verifies that the indicated destination file
**       enable/disable state is within bounds.
**       Must be DS_ENABLED or DS_DISABLED.
**
**  \par Called From:
**       - Command handler (set application enable/disable state)
**       - Command handler (set file enable/disable state)
**       - Destination File Table entry validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Enable/Disable State value
**
**  \sa #DS_TableVerifySize, #DS_TableVerifyAge, #DS_DestFileEntry_t
*/
boolean DS_TableVerifyState(uint16 EnableState);


/*******************************************************************/
/*  \brief Verify destination file max size limit
**  
**  \par Description
**       This function verifies that the indicated destination file
**       max size limit is within bounds - as defined by platform
**       configuration parameters.
**
**  \par Called From:
**       - Command handler (set file max size limit)
**       - Destination File Table entry validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Maximum Size Limit value
**
**  \sa #DS_TableVerifyAge, #DS_TableVerifyCount, #DS_DestFileEntry_t
*/
boolean DS_TableVerifySize(uint32 MaxFileSize);


/*******************************************************************/
/*  \brief Verify destination file max age limit
**  
**  \par Description
**       This function verifies that the indicated destination file
**       max age limit is within bounds - as defined by platform
**       configuration parameters.
**
**  \par Called From:
**       - Command handler (set file max age limit)
**       - Destination File Table entry validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Maximum Age Limit value
**
**  \sa #DS_TableVerifySize, #DS_TableVerifyCount, #DS_DestFileEntry_t
*/
boolean DS_TableVerifyAge(uint32 MaxFileAge);


/*******************************************************************/
/*  \brief Verify destination file sequence count
**  
**  \par Description
**       This function verifies that the indicated destination file
**       sequence count is within bounds - as defined by platform
**       configuration parameters.
**
**  \par Called From:
**       - Command handler (set file sequence count)
**       - Destination File Table entry validation function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Sequence Count value
**
**  \sa #DS_TableVerifySize, #DS_TableVerifyAge, #DS_DestFileEntry_t
*/
boolean DS_TableVerifyCount(uint32 SequenceCount);


/*******************************************************************/
/*  \brief Subscribe to packet filter table packets
**  
**  \par Description
**       A new Packet Filter Table is available for use. This
**       function is called to subscribe to packets referenced
**       by the new filter table.
**
**  \par Called From:
**       - Packet Filter Table manage function
**       
**  \par Assumptions, External Events, and Notes:
**       Caller has determined that the new filter table exists.
**       
**  \param [in]  (none)
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_TableUnsubscribe
*/
void DS_TableSubscribe(void);


/*******************************************************************/
/*  \brief Unsubscribe to packet filter table packets
**  
**  \par Description
**       A new Packet Filter Table is available for use. Prior to 
**       subscribing to the packets referenced by the new filter
**       table, this function is called to unsubscribe to packets
**       referenced by the old filter table.
**
**  \par Called From:
**       - Packet Filter Table manage function
**       
**  \par Assumptions, External Events, and Notes:
**       Caller has determined that the old filter table exists.
**       
**  \param [in]  (none)
**
**  \sa #DS_PacketEntry_t, #DS_FilterParms_t, #DS_TableSubscribe
*/
void DS_TableUnsubscribe(void);


/*******************************************************************/
/*  \brief Create local area within the Critical Data Store (CDS)
**  
**  \par Description
**       This function creates a new CDS area or gets access to a
**       CDS area created prior to a processor reset. The CDS area
**       will be used to store the current values for destination
**       filename sequence counts.
**
**  \par Called From:
**       - Application initialization function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  (none)
**
**  \sa #DS_DestFileEntry_t
*/
int32 DS_TableCreateCDS(void);


/*******************************************************************/
/*  \brief Update CDS with current filename sequence count values
**  
**  \par Description
**       This function writes the current filename sequence count
**       values to the Critical Data Store. The function is called
**       each time the sequence count values are modified.
**
**  \par Called From:
**       - Destination table data update handler
**       - Destination file creation function
**       - Command handler (set sequence count)
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  (none)
**
**  \sa #DS_DestFileEntry_t
*/
void DS_TableUpdateCDS(void);


/*******************************************************************/
/*  \brief Hash table function
**  
**  \par Description
**       This function converts a cFE MessageID into an index into
**       the hash table. The indexed hash table entry contains a
**       linked list where each link identifies a single entry in
**       the packet filter table. Thus, the scope of searching the
**       Packet Filter Table for a specific MessageID has been reduced
**       from searching the total number of entries in the filter
**       table by a factor equal to the size of the hash table.
**
**  \par Called From:
**       - Hash table creation function (after load filter table)
**       - Find messageID in filter table function
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message ID (#CFE_SB_MsgId_t)
**  \param [out] Index of hash table entry for input message ID
**
**  \sa #DS_HashLink_t, #DS_TableCreateHash, #DS_TableFindMsgID
*/
uint32 DS_TableHashFunction(CFE_SB_MsgId_t MessageID);


/*******************************************************************/
/*  \brief Create hash table function
**  
**  \par Description
**       This function populates the hash table following a new
**       load of the packet filter table. Because there may be
**       more message ID's than hash table entries, the hash table
**       function may translate multiple message ID's into each
**       hash table index. Each hash table entry is a linked list
**       containing a link for each message ID that translates to
**       the same hash table index.
**
**  \par Called From:
**       - Filter table manage updates function (after table load)
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  (none)
**
**  \sa #DS_HashLink_t, #DS_TableHashFunction, #DS_TableFindMsgID
*/
void DS_TableCreateHash(void);


/*******************************************************************/
/*  \brief Adds a message ID to the hash table
**  
**  \par Description
**       This function populates the hash table with a new message ID
**
**  \par Called From:
**       - Creation of Hash Table
**       - Command to add a MID
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message ID (#CFE_SB_MsgId_t)
**  \param [in] Filter table index for message ID
**  \param [out] Hash table index for message ID
**
**  \sa #DS_HashLink_t, #DS_TableHashFunction, #DS_TableFindMsgID
*/
int32 DS_TableAddMsgID(CFE_SB_MsgId_t MessageID, int32 FilterIndex);

/*******************************************************************/
/*  \brief Search packet filter table for message ID
**  
**  \par Description
**       This function searches for a packet filter table entry that
**       matches the input argument message ID.
**
**  \par Called From:
**       - Data storage packet processor
**       - Command handler (set file index)
**       - Command handler (set filter type)
**       - Command handler (set filter parms)
**       - Command handler (add messageID to filter table)
**       
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message ID (#CFE_SB_MsgId_t)
**  \param [out] Filter table index for message ID
**
**  \sa #DS_HashLink_t, #DS_TableHashFunction, #DS_TableCreateHash
*/
int32 DS_TableFindMsgID(CFE_SB_MsgId_t MessageID);


#endif /* _ds_table_h_ */

/************************/
/*  End of File Comment */
/************************/
