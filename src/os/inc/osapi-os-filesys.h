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
/*
 * Initializes the File System functions 
*/

int32           OS_FS_Init(void);

/*
 * Creates a file specified by path
*/
int32           OS_creat  (const char *path, int32  access);

/*
 * Opend a file for reading/writing. Returns file descriptor
*/
int32           OS_open   (const char *path,  int32 access,  uint32 mode);

/*
 * Closes an open file.
*/
int32           OS_close  (uint32  filedes);

/*
 * Reads nbytes bytes from file into buffer
*/
int32           OS_read   (uint32  filedes, void *buffer, uint32 nbytes);

/*
 * Write nybytes bytes of buffer into the file
*/
int32           OS_write  (uint32  filedes, const void *buffer, uint32 nbytes);

/**
 * File/Stream input read with a timeout
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
 */
int32           OS_TimedRead(uint32  filedes, void *buffer, uint32 nbytes, int32 timeout);

/**
 * File/Stream output write with a timeout
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
 */
int32           OS_TimedWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout);

/*
 * Changes the permissions of a file
*/
int32           OS_chmod  (const char *path, uint32 access);

/*
 * Returns file status information in filestats
*/
int32           OS_stat   (const char *path, os_fstat_t  *filestats);

/*
 * Seeks to the specified position of an open file 
*/
int32           OS_lseek  (uint32  filedes, int32 offset, uint32 whence);

/*
 * Removes a file from the file system
*/
int32           OS_remove (const char *path);

/*
 * Renames a file in the file system
*/
int32           OS_rename (const char *old_filename, const char *new_filename);

/* 
 * copies a single file from src to dest
*/
int32 OS_cp (const char *src, const char *dest);

/* 
 * moves a single file from src to dest
*/
int32 OS_mv (const char *src, const char *dest);

/*
 * Copies the info of an open file to the structure
*/
int32 OS_FDGetInfo (uint32 filedes, OS_file_prop_t *fd_prop);

/*
** Check to see if a file is open
*/
int32 OS_FileOpenCheck(const char *Filename);

/*
** Close all open files
*/
int32 OS_CloseAllFiles(void);

/*
** Close a file by filename
*/
int32 OS_CloseFileByName(const char *Filename);


/******************************************************************************
** Directory API 
******************************************************************************/

#ifndef OSAL_OMIT_DEPRECATED
/*
 * Opens a directory for searching
*/
os_dirp_t       OS_opendir (const char *path);

/*
 * Closes an open directory
*/
int32           OS_closedir(os_dirp_t directory);

/*
 * Rewinds an open directory
*/
void            OS_rewinddir(os_dirp_t directory);

/*
 * Reads the next object in the directory
*/
os_dirent_t *   OS_readdir (os_dirp_t directory);

#endif


/*
 * Opens a directory for searching
 * Same as OS_opendir but with a OSAL-style API
 */
int32           OS_DirectoryOpen(uint32 *dir_id, const char *path);

/*
 * Closes an open directory
 * Same as OS_closedir but with a OSAL-style API
 */
int32           OS_DirectoryClose(uint32 dir_id);

/*
 * Rewinds an open directory
 * Same as OS_rewinddir but with a OSAL-style API
 */
int32           OS_DirectoryRewind(uint32 dir_id);

/*
 * Reads the next object in the directory
 * Same as OS_readdir but with a OSAL-style API
 */
int32           OS_DirectoryRead(uint32 dir_id, os_dirent_t *dirent);

/*
 * Makes a new directory
*/
int32           OS_mkdir   (const char *path, uint32 access);

/*
 * Removes an empty directory from the file system.
 */
int32           OS_rmdir   (const char *path);

/******************************************************************************
** System Level API 
******************************************************************************/

/*
 * Create a fixed mapping between an existing directory and a virtual OSAL mount point.
 *
 * This mimics the behavior of a "FS_BASED" entry in the VolumeTable but is registered
 * at runtime.  It is intended to be called by the PSP/BSP prior to starting the
 */
int32           OS_FileSysAddFixedMap(uint32 *filesys_id, const char *phys_path,
                                const char *virt_path);

/*
 * Makes a file system
*/
int32           OS_mkfs        (char *address, const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);
/*
 * Mounts a file system
*/
int32           OS_mount       (const char *devname, const char *mountpoint);

/*
 * Initializes an existing file system
*/
int32           OS_initfs      (char *address, const char *devname, const char *volname,
                                uint32 blocksize, uint32 numblocks);

/*
 * removes a file system 
*/
int32           OS_rmfs        (const char *devname);

/*
 * Unmounts a mounted file system
*/
int32           OS_unmount     (const char *mountpoint);

/*
 * Returns the number of free blocks in a file system
*/
int32           OS_fsBlocksFree (const char *name);

/*
** Returns the number of free bytes in a file system 
** Note the 64 bit data type to support filesystems that
** are greater than 4 Gigabytes
*/
int32 OS_fsBytesFree (const char *name, uint64 *bytes_free);

/*
 * Checks the health of a file system and repairs it if necessary
 *
 * Modified to return an "int32" like all other functions - this used to
 * have its own dedicated return typedef, which was also an integer,
 * even though the same OSAL return codes seem to be used.
 * (VxWorks is the only OS that currently does something in this call)
 */
int32   OS_chkfs       (const char *name, bool repair);

/*
 * Returns in the parameter the physical drive underneith the mount point 
*/
int32       OS_FS_GetPhysDriveName  (char * PhysDriveName, const char * MountPoint);

/*
** Translates a OSAL Virtual file system path to a host Local path
*/
int32       OS_TranslatePath ( const char *VirtualPath, char *LocalPath);

/*             
**  Returns information about the file system in an os_fsinfo_t
*/
int32       OS_GetFsInfo(os_fsinfo_t  *filesys_info);

/******************************************************************************
** Shell API
******************************************************************************/

/* executes the shell command passed into is and writes the output of that 
 * command to the file specified by the given OSAPI file descriptor */
int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes);

#endif
