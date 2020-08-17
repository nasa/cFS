/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * File: osapi-os-filesys.h
 *
 * Author:  Alan Cudmore Code 582
 *
 * Purpose: Contains functions prototype definitions and variables declarations
 *          for the OS Abstraction Layer, File System module
 */

#ifndef _osapi_filesys_
#define _osapi_filesys_

/** @defgroup OSFileAccess OSAL File Access Option Defines
 * @{
 */
#define OS_READ_ONLY        0  /**< Read only file access */
#define OS_WRITE_ONLY       1  /**< Write only file access */
#define OS_READ_WRITE       2  /**< Read write file access */
/**@}*/

/** @defgroup OSFileOffset OSAL Refernce Point For Seek Offset Defines
 * @{
 */
#define OS_SEEK_SET         0  /**< Seek offset set */
#define OS_SEEK_CUR         1  /**< Seek offset current */
#define OS_SEEK_END         2  /**< Seek offset end */
/**@}*/

#define OS_CHK_ONLY         0  /**< Unused, API takes bool */
#define OS_REPAIR           1  /**< Unused, API takes bool */

#ifndef OSAL_OMIT_DEPRECATED

/** @defgroup OSVolType OSAL Volume Type Defines
 * @{
 */
#define FS_BASED            0  /**< @deprecated Volume type FS based */
#define RAM_DISK            1  /**< @deprecated Volume type RAM disk */
#define EEPROM_DISK         2  /**< @deprecated Volume type EEPROM disk */
#define ATA_DISK            3  /**< @deprecated Volume type ATA disk */
/**@}*/

/**
 * @brief Number of entries in the internal volume table
 * @deprecated
 */
#define NUM_FILE_SYSTEMS    OS_MAX_FILE_SYSTEMS

#endif
/*
** Length of a Device and Volume name
*/
#define OS_FS_DEV_NAME_LEN  32  /**< Device name length */
#define OS_FS_PHYS_NAME_LEN 64  /**< Physical drive name length */
#define OS_FS_VOL_NAME_LEN  32  /**< Volume name length */

/**
 * @brief Maximum length of a local/native path name string
 *
 * This is a concatenation of the OSAL virtual path with the system
 * mount point or device name
 */
#define OS_MAX_LOCAL_PATH_LEN       (OS_MAX_PATH_LEN + OS_FS_PHYS_NAME_LEN)


/** @addtogroup OSReturnCodes
 * @{
 */
/*
** Defines for File System Calls
*/
/*
 * NOTE - these values used to overlap with the
 * other OSAPI error codes.  They now start at -100
 * to avoid this overlap.
 */
#define OS_FS_ERR_PATH_TOO_LONG        (-103)  /**< @brief FS path too long */
#define OS_FS_ERR_NAME_TOO_LONG        (-104)  /**< @brief FS name too long */
#define OS_FS_ERR_DRIVE_NOT_CREATED    (-106)  /**< @brief FS drive not created */
#define OS_FS_ERR_DEVICE_NOT_FREE      (-107)  /**< @brief FS device not free */
#define OS_FS_ERR_PATH_INVALID         (-108)  /**< @brief FS path invalid */

#ifndef OSAL_OMIT_DEPRECATED
/*
 * Map some codes used by the file API back to the generic counterparts
 * where there is overlap between them.  Do not duplicate error codes.
 */
#define OS_FS_SUCCESS                  OS_SUCCESS               /**< @deprecated Successful execution */
#define OS_FS_ERROR                    OS_ERROR                 /**< @deprecated Failed execution */
#define OS_FS_ERR_INVALID_POINTER      OS_INVALID_POINTER       /**< @deprecated Invalid pointer */
#define OS_FS_ERR_NO_FREE_FDS          OS_ERR_NO_FREE_IDS       /**< @deprecated No free IDs */
#define OS_FS_ERR_INVALID_FD           OS_ERR_INVALID_ID        /**< @deprecated Invalid ID */
#define OS_FS_UNIMPLEMENTED            OS_ERR_NOT_IMPLEMENTED   /**< @deprecated Not implemented */
#endif
/**@}*/

#ifndef OSAL_OMIT_DEPRECATED
/* This typedef is for OS_FS_GetErrorName(), to ensure
 * everyone is making an array of the same length
 *
 * Implementation note for developers:
 *
 * os_fs_err_name_t is now equivalent to the OSAL "os_err_name_t" typedef,
 * to preserve source code compatibility with anything using the OS_FS_GetErrorName api
 *
 * The sizes of strings in OSAL functions are built with os_fs_err_name_t's
 * limits in mind.  Always check the uses of os_fs_err_name_t when changing
 * os_err_name_t.
 */
typedef os_err_name_t os_fs_err_name_t;

/**
 * @brief Internal structure of the OS volume table for
 * mounted file systems and path translation
 *
 * @deprecated Use the OSAL file system API to register volumes
 */
typedef struct
{
    char   DeviceName [OS_FS_DEV_NAME_LEN];
    char   PhysDevName [OS_FS_PHYS_NAME_LEN];
    uint32 VolumeType;
    uint8  VolatileFlag;
    uint8  FreeFlag;
    uint8  IsMounted;
    char   VolumeName [OS_FS_VOL_NAME_LEN];
    char   MountPoint [OS_MAX_PATH_LEN];
    uint32 BlockSize;

} OS_VolumeInfo_t;

#endif


/** @brief OSAL file system info */
typedef struct
{
   uint32   MaxFds;                /**< @brief Total number of file descriptors */
   uint32   FreeFds;               /**< @brief Total number that are free */
   uint32   MaxVolumes;            /**< @brief Maximum number of volumes */
   uint32   FreeVolumes;           /**< @brief Total number of volumes free */
} os_fsinfo_t;

/** @brief OSAL file properties */
typedef struct
{
    char Path[OS_MAX_PATH_LEN];
    uint32 User;
    uint8 IsValid;                /* For backward compatibility -- always true if OS_FDGetInfo returned true */
}OS_file_prop_t;

/**
 * @brief File system status
 *
 * @note This used to be directly typedef'ed to the "struct stat" from the C library
 *
 * Some C libraries (glibc in particular) actually define member names to reference into
 * sub-structures, so attempting to reuse a name like "st_mtime" might not work.
 */
typedef struct
{
   uint32   FileModeBits;
   int32    FileTime;
   uint32   FileSize;
} os_fstat_t;

/**
 * @brief File stat mode bits
 *
 * We must also define replacements for the stat structure's mode bits.
 * This is currently just a small subset since the OSAL just presents a very
 * simplified view of the filesystem to the upper layers.  And since not all
 * OS'es are POSIX, the more POSIX-specific bits are not relevant anyway.
 */
enum
{
   OS_FILESTAT_MODE_EXEC  = 0x00001,
   OS_FILESTAT_MODE_WRITE = 0x00002,
   OS_FILESTAT_MODE_READ =  0x00004,
   OS_FILESTAT_MODE_DIR =   0x10000
};

/** @brief Access file stat mode bits */
#define OS_FILESTAT_MODE(x)   ((x).FileModeBits)
/** @brief File stat is directory logical */
#define OS_FILESTAT_ISDIR(x)  ((x).FileModeBits & OS_FILESTAT_MODE_DIR)
/** @brief File stat is executable logical */
#define OS_FILESTAT_EXEC(x)   ((x).FileModeBits & OS_FILESTAT_MODE_EXEC)
/** @brief File stat is write enabled logical */
#define OS_FILESTAT_WRITE(x)  ((x).FileModeBits & OS_FILESTAT_MODE_WRITE)
/** @brief File stat is read enabled logical */
#define OS_FILESTAT_READ(x)   ((x).FileModeBits & OS_FILESTAT_MODE_READ)
/** @brief Access file stat size field */
#define OS_FILESTAT_SIZE(x)   ((x).FileSize)
/** @brief Access file stat time field */
#define OS_FILESTAT_TIME(x)   ((x).FileTime)

/** @brief Directory entry */
typedef struct
{
   char FileName[OS_MAX_FILE_NAME];
} os_dirent_t;

#ifndef OSAL_OMIT_DEPRECATED
/*
 * Preserve the old type names for compatibility;
 * but instead of DIR* it is now just a void*
 */
/* Provide something to implement os_dirp_t */
typedef void * os_dirp_t; /**< @deprecated */
#endif

/** @brief Access filename part of the dirent structure */
#define OS_DIRENTRY_NAME(x)   ((x).FileName)

#ifndef OSAL_OMIT_DEPRECATED
/*
 * Several old type names can be aliases for compatibility
 */
typedef int32              os_fshealth_t;  /**< @deprecated type no longer used */
typedef OS_file_prop_t     OS_FDTableEntry; /**< @deprecated Use OS_file_prop_t */
#endif

/*
 * Exported Functions
*/

/** @defgroup OSAPIFile OSAL Standard File APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a file specified by path
 *
 * Creates a file specified by const char *path, with read/write
 * permissions by access. The file is also automatically opened by the
 * create call.
 *
 * @param[in] path File name to create
 * @param[in] access Intended access mode - see @ref OSFileAccess
 *
 * @note Valid handle IDs are never negative.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @return A file handle ID or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
 * @retval #OS_ERROR if permissions are unknown or OS call fails
 * @retval #OS_ERR_NO_FREE_IDS if there are no free file descriptors left
 */
int32           OS_creat  (const char *path, int32  access);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Opens a file
 *
 * Opens a file.
 *
 * @param[in] path   File name to create
 * @param[in] access Intended access mode - see @ref OSFileAccess
 * @param[in] mode   The file permissions. This parameter is passed through to the
 *		     native open call, but will be ignored. The file mode (or permissions)
 *                   are ignored by the POSIX open call when the O_CREAT access flag is not passed in.
 *
 * @note Valid handle IDs are never negative.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @return A file handle ID or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
 * @retval #OS_ERROR if permissions are unknown or OS call fails
 * @retval #OS_ERR_NO_FREE_IDS if there are no free file descriptors left
 */
int32           OS_open   (const char *path,  int32 access,  uint32 mode);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Closes an open file handle
 *
 * This closes regular file handles and any other file-like resource, such as
 * network streams or pipes.
 *
 * @param[in] filedes   The handle ID to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if file descriptor could not be closed
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32           OS_close  (uint32  filedes);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Read from a file handle
 *
 * Reads up to nbytes from a file, and puts them into buffer.
 *
 * @param[in]  filedes  The handle ID to operate on
 * @param[out] buffer   Storage location for file data
 * @param[in]  nbytes   Maximum number of bytes to read
 *
 * @note All OSAL error codes are negative int32 values.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @return A non-negative byte count or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if buffer is a null pointer
 * @retval #OS_ERROR if OS call failed
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32           OS_read   (uint32  filedes, void *buffer, uint32 nbytes);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Write to a file handle
 *
 * Writes to a file. copies up to a maximum of nbytes of buffer to the file
 * described in filedes
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] buffer    Source location for file data
 * @param[in] nbytes    Maximum number of bytes to read
 *
 * @note All OSAL error codes are negative int32 values.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @return A non-negative byte count or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if buffer is NULL
 * @retval #OS_ERROR if OS call failed
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32           OS_write  (uint32  filedes, const void *buffer, uint32 nbytes);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief File/Stream input read with a timeout
 *
 * This implements a time-limited read and is primarily intended for use with
 * sockets but may also work with any other stream-like resource that the underlying
 * OS supports.
 *
 * If data is immediately available on the file/socket, this will return that data
 * along with the actual number of bytes that were immediately available.  It will
 * not block.
 *
 * If no data is immediately available, this will wait up to the given timeout for
 * data to appear.  If no data appears within the timeout period, then this returns
 * an error code (not zero).
 *
 * In all cases this will return successfully as soon as at least 1 byte of actual
 * data is available.  It will not attempt to read the entire input buffer.
 *
 * If an EOF condition occurs prior to timeout, this function returns zero.
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] buffer    Source location for file data
 * @param[in] nbytes    Maximum number of bytes to read
 * @param[in] timeout   Maximum time to wait, in milliseconds (OS_PEND = forever)
 *
 * @return Byte count on success, zero for timeout, or appropriate error code,
 *         see @ref OSReturnCodes
 */
int32           OS_TimedRead(uint32  filedes, void *buffer, uint32 nbytes, int32 timeout);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief File/Stream output write with a timeout
 *
 * This implements a time-limited write and is primarily intended for use with
 * sockets but may also work with any other stream-like resource that the underlying
 * OS supports.
 *
 * If output buffer space is immediately available on the file/socket, this will
 * place data into the buffer and return the actual number of bytes that were
 * queued for output.  It will not block.
 *
 * If no output buffer space is immediately available, this will wait up to the
 * given timeout for space to become available.  If no space becomes available within
 * the timeout period, then this returns an error code (not zero).
 *
 * In all cases this will return successfully as soon as at least 1 byte of actual
 * data is output.  It will _not_ attempt to write the entire output buffer.
 *
 * If an EOF condition occurs prior to timeout, this function returns zero.
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] buffer    Source location for file data
 * @param[in] nbytes    Maximum number of bytes to read
 * @param[in] timeout   Maximum time to wait, in milliseconds (OS_PEND = forever)
 *
 * @return Byte count on success, zero for timeout, or appropriate error code,
 *         see @ref OSReturnCodes
 */
int32           OS_TimedWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Changes the permissions of a file
 *
 * @param[in] path   File to change
 * @param[in] access Desired access mode - see @ref OSFileAccess
 *
 * @note Some file systems do not implement permissions
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32           OS_chmod  (const char *path, uint32 access);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a file or directory
 *
 * Returns information about a file or directory in a os_fstat_t structure
 *
 * @param[in]  path      The file to operate on
 * @param[out] filestats Buffer to store file information
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if path or filestats is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long to be stored
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_ERROR if the OS call failed
 */
int32           OS_stat   (const char *path, os_fstat_t  *filestats);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Seeks to the specified position of an open file
 *
 * Sets the read/write pointer to a specific offset in a specific file.
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] offset    The file offset to seek to
 * @param[in] whence    The reference point for offset, see @ref OSFileOffset
 *
 * @return Byte offset from the beginning of the file or appropriate error code,
           see @ref OSReturnCodes
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * @retval #OS_ERROR if OS call failed
 */
int32           OS_lseek  (uint32  filedes, int32 offset, uint32 whence);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a file from the file system
 *
 * Removes a given filename from the drive
 *
 * @note The behvior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a varienty of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  path      The file to operate on
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if there is no device or the driver returns error
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if path is too long to be stored locally
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the name of the file to remove is too long
 */
int32           OS_remove (const char *path);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Renames a file
 *
 * Changes the name of a file, where the source and destination
 * reside on the same file system.
 *
 * @note The behvior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a varienty of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  old_filename      The original filename
 * @param[in]  new_filename      The desired filename
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the file could not be opened or renamed.
 * @retval #OS_INVALID_POINTER if old or new are NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the new name is too long to be stored locally
 */
int32           OS_rename (const char *old_filename, const char *new_filename);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Copies a single file from src to dest
 *
 * @note The behvior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a varienty of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  src       The source file to operate on
 * @param[in]  dest      The destination file
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the file could not be accessed
 * @retval #OS_INVALID_POINTER if src or dest are NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
 */
int32 OS_cp (const char *src, const char *dest);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Move a single file from src to dest
 *
 * This first attempts to rename the file, which is faster if
 * the source and destination reside on the same file system.
 *
 * If this fails, it falls back to copying the file and removing
 * the original.
 *
 * @note The behvior of this API on an open file is not defined at the OSAL level
 * due to dependencies on the underlying OS which may or may not allow the related
 * operation based on a varienty of potential configurations.  For portability,
 * it is recommended that applications ensure the file is closed prior to removal.
 *
 * @param[in]  src       The source file to operate on
 * @param[in]  dest      The destination file
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the file could not be renamed.
 * @retval #OS_INVALID_POINTER if src or dest are NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * @retval #OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
 */
int32 OS_mv (const char *src, const char *dest);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about an open file
 *
 * Copies the information of the given file descriptor into a structure passed in
 *
 * @param[in]  filedes  The handle ID to operate on
 * @param[out] fd_prop  Storage buffer for file information
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32 OS_FDGetInfo (uint32 filedes, OS_file_prop_t *fd_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Checks to see if a file is open
 *
 * This function takes a filename and determines if the file is open. The function
 * will return success if the file is open.
 *
 * @param[in]  Filename      The file to operate on
 *
 * @return OS_SUCCESS if the file is open, or appropriate error code
 * @retval #OS_ERROR if the file is not open
 */
int32 OS_FileOpenCheck(const char *Filename);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Close all open files
 *
 * Closes All open files that were opened through the OSAL
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR   if one or more file close returned an error
 */
int32 OS_CloseAllFiles(void);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Close a file by filename
 *
 * Allows a file to be closed by name.
 * This will only work if the name passed in is the same name used to open
 * the file.
 *
 * @param[in]  Filename      The file to close
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_FS_ERR_PATH_INVALID if the file is not found
 * @retval #OS_ERROR   if the file close returned an error
 */
int32 OS_CloseFileByName(const char *Filename);
/**@}*/

/** @defgroup OSAPIDir OSAL Directory APIs
 * @{
 */

#ifndef OSAL_OMIT_DEPRECATED
/**
 * @brief Opens a directory for searching
 * @deprecated Replaced by OS_DirectoryOpen()
 */
os_dirp_t       OS_opendir (const char *path);

/*
 * @brief Closes an open directory
 * @deprecated Replaced by OS_DirectoryClose()
 */
int32           OS_closedir(os_dirp_t directory);

/*
 * @brief Rewinds an open directory
 * @deprecated Replaced by OS_DirectoryRewind()
 */
void            OS_rewinddir(os_dirp_t directory);

/*
 * @brief Reads the next object in the directory
 * @deprecated Replaced by OS_DirectoryRead()
 */
os_dirent_t *   OS_readdir (os_dirp_t directory);

#endif

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Opens a directory
 *
 * Prepares for reading the files within a directory
 *
 * @param[out] dir_id    The non-zero handle ID of the directory
 * @param[in]  path      The directory to open
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32           OS_DirectoryOpen(uint32 *dir_id, const char *path);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Closes an open directory
 *
 * The directory referred to by dir_id will be closed
 *
 * @param[in] dir_id    The handle ID of the directory
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32           OS_DirectoryClose(uint32 dir_id);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Rewinds an open directory
 *
 * Resets a directory read handle back to the first file.
 *
 * @param[in] dir_id    The handle ID of the directory
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32           OS_DirectoryRewind(uint32 dir_id);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Reads the next name in the directory
 *
 * Obtains directory entry data for the next file from an open directory
 *
 * @param[in]  dir_id    The handle ID of the directory
 * @param[out] dirent    Buffer to store directory entry information
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32           OS_DirectoryRead(uint32 dir_id, os_dirent_t *dirent);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Makes a new directory
 *
 * Makes a directory specified by path.
 *
 * @param[in]  path      The new directory name
 * @param[in]  access    The permissions for the directory (reserved for future use)
 *
 * @note Current implementations do not utilize the "access" parameter.  Applications
 * should still pass the intended value (#OS_READ_WRITE or #OS_READ_ONLY) to be compatible
 * with future implementations.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_ERROR if the OS call fails
 */
int32           OS_mkdir   (const char *path, uint32 access);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a directory from the file system.
 *
 * Removes a directory from  the structure.
 * The directory must be empty prior to this operation.
 *
 * @param[in]  path      The directory to remove
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG
 * @retval #OS_ERROR if the directory remove operation failed
 */
int32           OS_rmdir   (const char *path);
/**@}*/

/** @defgroup OSAPIFileSys OSAL File System Level APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Create a fixed mapping between an existing directory and a virtual OSAL mount point.
 *
 * This mimics the behavior of a "FS_BASED" entry in the VolumeTable but is registered
 * at runtime.  It is intended to be called by the PSP/BSP prior to starting the application.
 *
 * @param[out]  filesys_id  A non-zero OSAL ID reflecting the file system
 * @param[in]   phys_path   The native system directory (an existing mount point)
 * @param[in]   virt_path   The virtual mount point of this filesystem
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32           OS_FileSysAddFixedMap(uint32 *filesys_id, const char *phys_path,
                                const char *virt_path);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Makes a file system on the target
 *
 * Makes a file system on the target.  Highly dependent on underlying OS and
 * dependent on OS volume table definition.
 *
 * @note The "volname" parameter of RAM disks should always begin with the string "RAM",
 *   e.g. "RAMDISK" or "RAM0","RAM1", etc if multiple devices are created.  The underlying
 *   implementation uses this to select the correct filesystem type/format, and this may
 *   also be used to differentiate between RAM disks and real physical disks.
 *
 * @param[in]   address   The address at which to start the new disk.  If address == 0
 *                        space will be allocated by the OS.
 * @param[in]   devname   The underlying kernel device to use, if applicable.
 * @param[in]   volname   The name of the volume (see note)
 * @param[in]   blocksize The size of a single block on the drive
 * @param[in]   numblocks The number of blocks to allocate for the drive
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if devname is NULL
 * @retval #OS_FS_ERR_DRIVE_NOT_CREATED if the OS calls to create the the drive failed
 * @retval #OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
 * @retval #OS_SUCCESS on creating the disk
 */
int32           OS_mkfs        (char *address, const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);
/*-------------------------------------------------------------------------------------*/
/**
 * @brief Mounts a file system
 *
 * Mounts a file system / block device at the given mount point.
 *
 * @param[in]   devname    The name of the drive to mount. devname is the same from #OS_mkfs
 * @param[in]   mountpoint The name to call this disk from now on
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32           OS_mount       (const char *devname, const char *mountpoint);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initializes an existing file system
 *
 * Initializes a file system on the target.
 *
 * @note The "volname" parameter of RAM disks should always begin with the string "RAM",
 *   e.g. "RAMDISK" or "RAM0","RAM1", etc if multiple devices are created.  The underlying
 *   implementation uses this to select the correct filesystem type/format, and this may
 *   also be used to differentiate between RAM disks and real physical disks.
 *
 * @param[in]   address   The address at which to start the new disk.  If address == 0,
 *                        then space will be allocated by the OS
 * @param[in]   devname   The underlying kernel device to use, if applicable.
 * @param[in]   volname   The name of the volume (see note)
 * @param[in]   blocksize The size of a single block on the drive
 * @param[in]   numblocks The number of blocks to allocate for the drive
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if devname or volname are  NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the name is too long
 * @retval #OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
 * @retval #OS_FS_ERR_DRIVE_NOT_CREATED on error
 */
int32           OS_initfs      (char *address, const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a file system
 *
 * This function will remove or un-map the target file system. Note that this is not
 * the same as un-mounting the file system.
 *
 * @param[in]   devname   The name of the "generic" drive
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if devname is NULL
 * @retval #OS_ERROR is the drive specified cannot be located
 */
int32           OS_rmfs        (const char *devname);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unmounts a mounted file system
 *
 * This function will unmount a drive from the file system and make all open file
 * descriptors useless.
 *
 * @note Any open file descriptors referencing this file system should
 * be closed prior to unmounting a drive
 *
 * @param[in]   mountpoint The mount point to remove from #OS_mount
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if name is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the absolute path given is too long
 * @retval #OS_ERROR if the OS calls failed
 */
int32           OS_unmount     (const char *mountpoint);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain number of blocks free
 *
 * Returns the number of free blocks in a volume
 *
 * @param[in]  name      The device/path to operate on
 *
 * @return Block count or appropriate error code, see @ref OSReturnCodes
 * @retval #OS_INVALID_POINTER if name is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the name is too long
 * @retval #OS_ERROR if the OS call failed
 */
int32           OS_fsBlocksFree (const char *name);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtains the number of free bytes in a volume
 *
 * Returns the number of free bytes in a volume
 *
 * @note uses a 64 bit data type to support filesystems that
 * are greater than 4 Gigabytes
 *
 * @param[in]  name       The device/path to operate on
 * @param[out] bytes_free The number of free bytes
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if name is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the name is too long
 * @retval #OS_ERROR if the OS call failed
 */
int32 OS_fsBytesFree (const char *name, uint64 *bytes_free);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Checks the health of a file system and repairs it if necessary
 *
 * Checks the drives for inconsistencies and optionally also repairs it
 *
 * @note not all operating systems implement this function
 *
 * @param[in]  name     The device/path to operate on
 * @param[in]  repair   Whether to also repair inconsistencies
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS             @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER     Name is NULL
 * @retval #OS_ERR_NOT_IMPLEMENTED @copybrief OS_ERR_NOT_IMPLEMENTED
 * @retval #OS_ERROR               @copybrief OS_ERROR
 */
int32   OS_chkfs       (const char *name, bool repair);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtains the physical drive name associated with a mount point
 *
 * Returns the name of the physical volume associated with the drive,
 * when given the OSAL mount point of the drive
 *
 * @param[out] PhysDriveName    Buffer to store physical drive name
 * @param[in]  MountPoint       OSAL mount point
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if either parameter is NULL
 * @retval #OS_ERROR if the mountpoint could not be found
 */
int32       OS_FS_GetPhysDriveName  (char * PhysDriveName, const char * MountPoint);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Translates a OSAL Virtual file system path to a host Local path
 *
 * Translates a virtual path to an actual system path name
 *
 * @param[in]  VirtualPath      OSAL virtual path name
 * @param[out] LocalPath        Buffer to store native/translated path name
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if either parameter is NULL
 */
int32       OS_TranslatePath ( const char *VirtualPath, char *LocalPath);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Returns information about the file system
 *
 * Returns information about the file system in an os_fsinfo_t.
 * This includes the number of open files and file systems
 *
 * @param[out] filesys_info     Buffer to store filesystem information
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if filesys_info is NULL
 */
int32       OS_GetFsInfo(os_fsinfo_t  *filesys_info);
/**@}*/

/** @defgroup OSAPIShell OSAL Shell APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Executes the command and sends output to a file
 *
 * Takes a shell command in and writes the output of that command to the specified file
 * The output file must be opened previously with write access (OS_WRITE_ONLY or OS_READ_WRITE).
 *
 * @param[in] Cmd     Command to pass to shell
 * @param[in] filedes File to send output to.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the command was not executed properly
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes);
/**@}*/

#endif
