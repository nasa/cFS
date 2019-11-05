/*
** File: osapi-os-filesys.h
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author:  Alan Cudmore Code 582
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, File System module
**
*/

#ifndef _osapi_filesys_
#define _osapi_filesys_

#define OS_READ_ONLY        0
#define OS_WRITE_ONLY       1
#define OS_READ_WRITE       2

#define OS_SEEK_SET         0
#define OS_SEEK_CUR         1
#define OS_SEEK_END         2

#define OS_CHK_ONLY         0
#define OS_REPAIR           1

#define FS_BASED            0
#define RAM_DISK            1
#define EEPROM_DISK         2
#define ATA_DISK            3


/*
** Number of entries in the internal volume table
*/
#define NUM_TABLE_ENTRIES 14

/*
** Length of a Device and Volume name 
*/
#define OS_FS_DEV_NAME_LEN  32
#define OS_FS_PHYS_NAME_LEN 64
#define OS_FS_VOL_NAME_LEN  32


/*
** Defines for File System Calls
*/
/*
 * NOTE - these values used to overlap with the
 * other OSAPI error codes.  They now start at -100
 * to avoid this overlap.
 */
#define OS_FS_ERR_PATH_TOO_LONG        (-103)
#define OS_FS_ERR_NAME_TOO_LONG        (-104)
#define OS_FS_ERR_DRIVE_NOT_CREATED    (-106)
#define OS_FS_ERR_DEVICE_NOT_FREE      (-107)
#define OS_FS_ERR_PATH_INVALID         (-108)


/* 
 * Map some codes used by the file API back to the generic counterparts 
 * where there is overlap between them.  Do not duplicate error codes.
 */
#define OS_FS_SUCCESS                  OS_SUCCESS
#define OS_FS_ERROR                    OS_ERROR
#define OS_FS_ERR_INVALID_POINTER      OS_INVALID_POINTER
#define OS_FS_ERR_NO_FREE_FDS          OS_ERR_NO_FREE_IDS
#define OS_FS_ERR_INVALID_FD           OS_ERR_INVALID_ID
#define OS_FS_UNIMPLEMENTED            OS_ERR_NOT_IMPLEMENTED


/* This typedef is for the OS_FS_GetErrorName function, to ensure
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


/*
** Internal structure of the OS volume table for 
** mounted file systems and path translation
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

typedef struct
{
   uint32   MaxFds;                /* Total number of file descriptors */
   uint32   FreeFds;               /* Total number that are free */
   uint32   MaxVolumes;            /* Maximum number of volumes */
   uint32   FreeVolumes;           /* Total number of volumes free */
} os_fsinfo_t;


typedef struct
{
    char Path[OS_MAX_PATH_LEN];
    uint32 User;
    uint8 IsValid;                /* For backward compatibility -- always true if OS_FDGetInfo returned true */
}OS_file_prop_t;


/*
 * NOTE: This used to be directly typedef'ed to the "struct stat" from the C library
 *
 * Some C libraries (glibc in particular) actually #define member names to reference into
 * sub-structures, so attempting to reuse a name like "st_mtime" might not work.
 */

typedef struct
{
   uint32   FileModeBits;
   int32    FileTime;
   uint32   FileSize;
} os_fstat_t;

/* We must also define replacements for the stat structure's mode bits.
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

#define OS_FILESTAT_MODE(x)   ((x).FileModeBits)
#define OS_FILESTAT_ISDIR(x)  ((x).FileModeBits & OS_FILESTAT_MODE_DIR)
#define OS_FILESTAT_EXEC(x)   ((x).FileModeBits & OS_FILESTAT_MODE_EXEC)
#define OS_FILESTAT_WRITE(x)  ((x).FileModeBits & OS_FILESTAT_MODE_WRITE)
#define OS_FILESTAT_READ(x)   ((x).FileModeBits & OS_FILESTAT_MODE_READ)
#define OS_FILESTAT_SIZE(x)   ((x).FileSize)
#define OS_FILESTAT_TIME(x)   ((x).FileTime)


typedef struct
{
   char FileName[OS_MAX_PATH_LEN];
} os_dirent_t;

/*
 * Preserve the old type names for compatibility;
 * but instead of DIR* it is now just a void*
 */
#ifndef OSAL_OMIT_DEPRECATED
/* Provide something to implement os_dirp_t */
typedef void * os_dirp_t;
#endif

/*
 * Macro to access filename part of the dirent structure
 */
#define OS_DIRENTRY_NAME(x)   ((x).FileName)

/*
 * Several old type names can be aliases for compatibility
 */
#ifndef OSAL_OMIT_DEPRECATED
typedef int32              os_fshealth_t;
typedef OS_file_prop_t     OS_FDTableEntry;
#endif

/*
 * Exported Functions
*/


/******************************************************************************
** Standard File system API
******************************************************************************/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Creates a file specified by path
 *
 * Creates a file specified by const char *path, with read/write
 * permissions by access. The file is also automatically opened by the
 * create call.
 *
 * @param[in] path File name to create
 * @param[in] access Intended access mode - OS_WRITE_ONLY or OS_READ_WRITE
 *
 * @note Valid handle IDs are never negative.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @returns A file handle ID on success, or appropriate error code
 * OS_INVALID_POINTER if path is NULL
 * OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
 * OS_ERROR if permissions are unknown or OS call fails
 * OS_ERR_NO_FREE_IDS if there are no free file descriptors left
 */
int32           OS_creat  (const char *path, int32  access);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Opens a file
 *
 * Opens a file. access parameters are OS_READ_ONLY, OS_WRITE_ONLY, or
 * OS_READ_WRITE
 *
 * @param[in] path   File name to create
 * @param[in] access Intended access mode - OS_READ_ONLY, OS_WRITE_ONLY or OS_READ_WRITE
 * @param[in] mode   The file permissions. This parameter is passed through to the 
 *		     native open call, but will be ignored. The file mode (or permissions)
 *                   are ignored by the POSIX open call when the O_CREAT access flag is not passed in. 
 *
 *
 * @note Valid handle IDs are never negative.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @returns A file handle ID on success, or appropriate error code
 * OS_INVALID_POINTER if path is NULL
 * OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
 * OS_ERROR if permissions are unknown or OS call fails
 * OS_ERR_NO_FREE_IDS if there are no free file descriptors left
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if file descriptor could not be closed
 * OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32           OS_close  (uint32  filedes);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Read from a file handle
 *
 * reads up to nbytes from a file, and puts them into buffer.
 *
 * @param[in]  filedes  The handle ID to operate on
 * @param[out] buffer   Storage location for file data
 * @param[in]  nbytes   Maximum number of bytes to read
 *
 * @note All OSAL error codes are negative int32 values.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @returns A non-negative byte count on success, or appropriate error code
 * OS_INVALID_POINTER if buffer is a null pointer
 * OS_ERROR if OS call failed
 * OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32           OS_read   (uint32  filedes, void *buffer, uint32 nbytes);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Write to a file handle
 *
 * writes to a file. copies up to a maximum of nbytes of buffer to the file
 * described in filedes
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] buffer    Source location for file data
 * @param[in] nbytes    Maximum number of bytes to read
 *
 * @note All OSAL error codes are negative int32 values.  Failure of this
 * call can be checked by testing if the result is less than 0.
 *
 * @returns A non-negative byte count on success, or appropriate error code
 * OS_INVALID_POINTER if buffer is NULL
 * OS_ERROR if OS call failed
 * OS_ERR_INVALID_ID if the file descriptor passed in is invalid
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
 * @returns A non-negative byte count on success, or appropriate error code
 * Returns zero if the timeout period expired.
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
 * @returns A non-negative byte count on success, or appropriate error code
 * Returns zero if the timeout period expired.
 */
int32           OS_TimedWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Changes the permissions of a file
 *
 * @param[in] path   File to change
 * @param[in] access Desired access mode - OS_READ_ONLY, OS_WRITE_ONLY or OS_READ_WRITE
 *
 * @note Some file systems do not implement permissions
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32           OS_chmod  (const char *path, uint32 access);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain information about a file or directory
 *
 * returns information about a file or directory in a os_fs_stat structure
 *
 * @param[in]  path      The file to operate on
 * @param[out] filestats Buffer to store file information
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if path or filestats is NULL
 * OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
 * OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long to be stored
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_ERROR if the OS call failed
 */
int32           OS_stat   (const char *path, os_fstat_t  *filestats);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Seeks to the specified position of an open file
 *
 * sets the read/write pointer to a specific offset in a specific file.
 * Whence is either OS_SEEK_SET,OS_SEEK_CUR, or OS_SEEK_END
 *
 * @param[in] filedes   The handle ID to operate on
 * @param[in] offset    The file offset to seek to
 * @param[in] whence    The reference point for offset
 *
 * @returns On success, a non-negative byte offset from the beginning of the file
 * OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * OS_ERROR if OS call failed
 */
int32           OS_lseek  (uint32  filedes, int32 offset, uint32 whence);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a file from the file system
 *
 * Removes a given filename from the drive
 *
 * @note Some file systems permit removal of open files while others do not.  For
 * portability, it is recommended that applications ensure the file is closed prior
 * to removal.
 *
 * @param[in]  path      The file to operate on
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if there is no device or the driver returns error
 * OS_INVALID_POINTER if path is NULL
 * OS_FS_ERR_PATH_TOO_LONG if path is too long to be stored locally
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_FS_ERR_NAME_TOO_LONG if the name of the file to remove is too long
 */
int32           OS_remove (const char *path);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Renames a file
 *
 * Changes the name of a file, where the source and destination
 * reside on the same file system.
 *
 * @note Some file systems permit renaming of open files while others do not.  For
 * portability, it is recommended that applications ensure the file is closed prior
 * to rename.
 *
 * @param[in]  old_filename      The original filename
 * @param[in]  new_filename      The desired filename
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if the file could not be opened or renamed.
 * OS_INVALID_POINTER if old or new are NULL
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * OS_FS_ERR_NAME_TOO_LONG if the new name is too long to be stored locally
 */
int32           OS_rename (const char *old_filename, const char *new_filename);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Copies a single file from src to dest
 *
 * @param[in]  src       The source file to operate on
 * @param[in]  dest      The destination file
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if the file could not be accessed
 * OS_INVALID_POINTER if src or dest are NULL
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
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
 * @param[in]  src       The source file to operate on
 * @param[in]  dest      The destination file
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if the file could not be renamed.
 * OS_INVALID_POINTER if src or dest are NULL
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
 * OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 * OS_FS_SUCCESS if the copying was successfull
 */
int32 OS_FDGetInfo (uint32 filedes, OS_file_prop_t *fd_prop);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Checks to see if a file is open
 *
 * @param[in]  Filename      The file to operate on
 *
 * @returns OS_SUCCESS if the file is open, or appropriate error code
 * OS_ERROR if the file is not open
 */
int32 OS_FileOpenCheck(const char *Filename);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Close all open files
 *
 * Closes All open files that were opened through the OSAL
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR   if one or more file close returned an error
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_FS_ERR_PATH_INVALID if the file is not found
 * OS_ERROR   if the file close returned an error
 */
int32 OS_CloseFileByName(const char *Filename);


/******************************************************************************
** Directory API 
******************************************************************************/

#ifndef OSAL_OMIT_DEPRECATED
/*
 * Opens a directory for searching
 * Replaced by OS_DirectoryOpen()
 */
os_dirp_t       OS_opendir (const char *path);

/*
 * Closes an open directory
 * Replaced by OS_DirectoryClose()
 */
int32           OS_closedir(os_dirp_t directory);

/*
 * Rewinds an open directory
 * Replaced by OS_DirectoryRewind()
 */
void            OS_rewinddir(os_dirp_t directory);

/*
 * Reads the next object in the directory
 * Replaced by OS_DirectoryRead()
 */
os_dirent_t *   OS_readdir (os_dirp_t directory);

#endif



/*-------------------------------------------------------------------------------------*/
/**
 * @brief Opens a directory
 *
 * Prepares for reading the files within a directory
 *
 * @param[out] dir_id    The handle ID of the directory
 * @param[in]  path      The directory to open
 *
 * @returns OS_SUCCESS on success, or appropriate error code
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
 * @returns OS_SUCCESS on success, or appropriate error code
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
 * @returns OS_SUCCESS on success, or appropriate error code
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
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32           OS_DirectoryRead(uint32 dir_id, os_dirent_t *dirent);


/*-------------------------------------------------------------------------------------*/
/**
 * @brief Makes a new directory
 *
 * makes a directory specified by path.
 *
 * @param[in]  path      The new directory name
 * @param[in]  access    The permissions for the directory (reserved for future use)
 *
 * @note current implementations do not utilize the "access" parameter.  Applications
 * should still pass the intended value (OS_READ_WRITE or OS_READ_ONLY) to be compatible
 * with future implementations.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if path is NULL
 * OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_ERROR if the OS call fails
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if path is NULL
 * OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * OS_FS_ER_PATH_TOO_LONG
 * OS_ERROR if the directory remove operation failed
 */
int32           OS_rmdir   (const char *path);

/******************************************************************************
** System Level API 
******************************************************************************/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Create a fixed mapping between an existing directory and a virtual OSAL mount point.
 *
 * This mimics the behavior of a "FS_BASED" entry in the VolumeTable but is registered
 * at runtime.  It is intended to be called by the PSP/BSP prior to starting the OSAL.
 *
 * @param[out]  filesys_id  An OSAL ID reflecting the file system
 * @param[in]   phys_path   The native system directory (an existing mount point)
 * @param[in]   virt_path   The virtual mount point of this filesystem
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32           OS_FileSysAddFixedMap(uint32 *filesys_id, const char *phys_path,
                                const char *virt_path);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Makes a file system on the target
 *
 * Makes a file system on the target
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if devname is NULL
 * OS_FS_ERR_DRIVE_NOT_CREATED if the OS calls to create the the drive failed
 * OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
 * OS_FS_SUCCESS on creating the disk
 */
int32           OS_mkfs        (char *address, const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);
/*-------------------------------------------------------------------------------------*/
/**
 * @brief Mounts a file system
 *
 * mounts a file system / block device at the given mount point
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 */
int32           OS_mount       (const char *devname, const char *mountpoint);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Initializes an existing file system
 *
 * Initializes a file system on the target.
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if devname or volname are  NULL
 * OS_FS_ERR_PATH_TOO_LONG if the name is too long
 * OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
 * OS_FS_ERR_DRIVE_NOT_CREATED on error
 */
int32           OS_initfs      (char *address, const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a file system
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if devname is NULL
 * OS_ERROR is the drive specified cannot be located
 * OS_FS_SUCCESS on removing  the disk
 */
int32           OS_rmfs        (const char *devname);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Unmounts a mounted file system
 *
 * @note Any open file descriptors referencing this file system should
 * be closed prior to unmounting a drive
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if name is NULL
 * OS_FS_ERR_PATH_TOO_LONG if the absolute path given is too long
 * OS_ERROR if the OS calls failed
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
 * @returns non-negative block count on success, or appropriate error code
 * OS_INVALID_POINTER if name is NULL
 * OS_FS_ERR_PATH_TOO_LONG if the name is too long
 * OS_ERROR if the OS call failed
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if name is NULL
 * OS_FS_ERR_PATH_TOO_LONG if the name is too long
 * OS_ERROR if the OS call failed
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if name is NULL
 * OS_ERR_NOT_IMPLEMENTED if not supported
 * OS_ERROR if the OS calls fail
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if either  parameter is NULL
 * OS_ERROR if the mountpoint could not be found
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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if either  parameter is NULL
 */
int32       OS_TranslatePath ( const char *VirtualPath, char *LocalPath);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Returns information about the file system
 *
 * returns information about the file system in an os_fsinfo_t
 * This includes the number of open files and file systems
 *
 * @param[out] filesys_info     Buffer to store filesystem information
 *
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_INVALID_POINTER if filesys_info is NULL
 */
int32       OS_GetFsInfo(os_fsinfo_t  *filesys_info);

/******************************************************************************
** Shell API
******************************************************************************/

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
 * @returns OS_SUCCESS on success, or appropriate error code
 * OS_ERROR if the command was not executed properly
 * OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes);

#endif
