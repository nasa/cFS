/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-shared-file.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_FILE_H_
#define INCLUDE_OS_SHARED_FILE_H_

#include <os-shared-globaldefs.h>

/*
 * Flags that can be used with opening of a file (bitmask)
 */
typedef enum
{
   OS_FILE_FLAG_NONE,
   OS_FILE_FLAG_CREATE = 0x01,
   OS_FILE_FLAG_TRUNCATE = 0x02,
} OS_file_flag_t;



typedef struct
{
   char      stream_name[OS_MAX_PATH_LEN];
   uint8     socket_domain;
   uint8     socket_type;
   uint16    stream_state;
} OS_stream_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_stream_internal_record_t         OS_stream_table[OS_MAX_NUM_OPEN_FILES];




/****************************************************************************************
                 FILE / DIRECTORY API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_FileAPI_Init

   Purpose: Initialize the OS-independent layer for file resources

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_FileAPI_Init                (void);


/*
 * Generic stream manipulation implementation
 *
 * These generic seek/read/write/close calls are usable for regular files and
 * anything else that is stream-oriented in nature, including pipes, devices,
 * and sockets if supported.  Note that "open" is not generic as this usually
 * requires a different approach depending on whether it is a socket, file, etc.
 * (There is a separate "FileOpen_Impl" later for this purpose).
 *
 * Note that read/write also include an option for a timeout.  This is more
 * relevant for fifos or sockets.  It should be set to OS_PEND for normal
 * behavior on regular files which is to wait forever.
 */

/*----------------------------------------------------------------
   Function: OS_GenericSeek_Impl

    Purpose: Seek to a given position in a file

    Returns: File position (non-negative) on success, or relevant error code (negative)
 ------------------------------------------------------------------*/
int32 OS_GenericSeek_Impl (uint32 local_id, int32 offset, uint32 whence);

/*----------------------------------------------------------------
   Function: OS_GenericRead_Impl

    Purpose: Read from a file descriptor
             This may be a normal file or a socket/pipe

    Returns: Number of bytes read (non-negative) on success, or relevant error code (negative)
 ------------------------------------------------------------------*/
int32 OS_GenericRead_Impl (uint32 local_id, void *buffer, uint32 nbytes, int32 timeout);

/*----------------------------------------------------------------
   Function: OS_GenericWrite_Impl

    Purpose: Write to a file descriptor
             This may be a normal file or a socket/pipe

    Returns: Number of bytes written (non-negative) on success, or relevant error code (negative)
 ------------------------------------------------------------------*/
int32 OS_GenericWrite_Impl(uint32 local_id, const void *buffer, uint32 nbytes, int32 timeout);


/*----------------------------------------------------------------
   Function: OS_GenericClose_Impl

    Purpose: Close a file descriptor
             This may be a normal file or a socket/pipe

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_GenericClose_Impl(uint32 local_id);

/*----------------------------------------------------------------
   Function: OS_FileOpen_Impl

    Purpose: Opens the file indicated by "local_path" with permission
             indicated by "access".

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileOpen_Impl(uint32 local_id, const char *local_path, int32 flags, int32 access);

/*----------------------------------------------------------------
   Function: OS_ShellOutputToFile_Impl

    Purpose: Takes a shell command in and writes the output of that command to the specified file

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(uint32 stream_id, const char* Cmd);



/****************************************************************************************
                             Filename-based Operations
  ***************************************************************************************/


/*
 * These FileXXX_Impl calls are usable for things that operate on pathnames,
 * that is to say they appear in the file system in some way.
 *
 * Mainly intended for regular files but they should work on named pipes or
 * devices too, provided they have a pathname associated with them.
 *
 * Note that sockets (at least the inet variety) usually do _not_ have a
 * filename, so they require a different "open" method, but usually do use
 * the generic read/write/close pattern thereafter.
 */

/*----------------------------------------------------------------
   Function: OS_FileStat_Impl

    Purpose: Output stats on the file indicated by "local_path"

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileStat_Impl(const char *local_path, os_fstat_t *filestat);

/*----------------------------------------------------------------
   Function: OS_FileRemove_Impl

    Purpose: Remove/Unlink the file indicated by "local_path"

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileRemove_Impl(const char *local_path);

/*----------------------------------------------------------------
   Function: OS_FileRename_Impl

    Purpose: Rename "old_path" to "new_path" in the filesystem

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileRename_Impl(const char *old_path, const char *new_path);

/*----------------------------------------------------------------

   Function: OS_FileChmod_Impl

    Purpose: Change permission on an existing file

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileChmod_Impl(const char *local_path, uint32 access);

#endif  /* INCLUDE_OS_SHARED_FILE_H_ */

