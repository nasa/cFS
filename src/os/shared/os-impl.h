/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   os-impl.h
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: Contains functions prototype definitions and variables declarations
 *          for the OS Abstraction Layer, Core OS module
 */

/*
 * NOTE - CONVENTION WITHIN THIS LAYER:
 *  This file contains function prototypes and type declarations that define the
 *  interface between the OS-specific and the shared portions of the OSAPI.
 *
 *  Functions that end in "_Impl" are _OS Specific_ and should be implemented
 *  within the appropriate OS-specific directory (i.e. posix, rtems, vxworks, etc)
 *
 *  NONE of the functions or variables within this file are supposed to be public --
 *  i.e. these are private to OSAL and not exposed to the code above this layer.
 *
 */

#ifndef _osapi_os_impl_
#define _osapi_os_impl_

#include "osapi.h"

/*
 * A "magic number" that when written to the "ShutdownFlag" member
 * of the global state structure indicates an active shutdown request.
 */
#define OS_SHUTDOWN_MAGIC_NUMBER    0xABADC0DE

/*
 * File system table size:
 * Historically this was a PSP/BSP-supplied table with a fixed size
 * of "NUM_TABLE_ENTRIES" which is _not_ part of osconfig.h.
 *
 * To be more consistent with other object types this implementation
 * allows the size of the run-time file system table to be configured
 * using "OS_MAX_FILE_SYSTEMS".
 *
 * For backward compatibility, if this is not defined in osconfig.h,
 * then this shall simply make the internal table to be the same size
 * as NUM_TABLE_ENTRIES.
 */
#ifndef OS_MAX_FILE_SYSTEMS
#define OS_MAX_FILE_SYSTEMS         NUM_TABLE_ENTRIES
#endif

/*
 * Number of console devices
 *
 * Typically this would always be 1, for OS_printf() output.
 */
#ifndef OS_MAX_CONSOLES
#define OS_MAX_CONSOLES             1
#endif


/*
 * Types shared between the implementations and shared code
 */

/*
 * Wrapper for encoding of other types into a generic void* type required as argument
 * to callbacks and pthread entry/return values, etc.
 *
 * Note this can only encode types with sizes <= sizeof(void*)
 */
typedef union
{
   void *opaque_arg;
   OS_ArgCallback_t arg_callback_func;
   OS_TimerCallback_t timer_callback_func;
   osal_task_entry entry_func;
   uint32 value;
} OS_U32ValueWrapper_t;

/*
 * OS_U32ValueWrapper_t must be equal in size to a "void*" as it is used in places
 * where a void* argument is required.  This compile-time assert ensures that.
 * If this fails, then it means one of the types within the union is too large.
 */
CompileTimeAssert(sizeof(OS_U32ValueWrapper_t) == sizeof(void *), U32ValueWrapperSize);


#define OS_OBJECT_EXCL_REQ_FLAG     0x0001

typedef struct
{
   const char *name_entry;
   uint32     active_id;
   uint32     creator;
   uint16     refcount;
   uint16     flags;
}OS_common_record_t;

typedef enum
{
   OS_STREAM_STATE_BOUND      = 0x01,
   OS_STREAM_STATE_CONNECTED  = 0x02,
   OS_STREAM_STATE_READABLE   = 0x04,
   OS_STREAM_STATE_WRITABLE   = 0x08,
} OS_StreamState_t;

/*tasks */
typedef struct
{
   char      task_name[OS_MAX_API_NAME];
   uint32    stack_size;
   uint32    priority;
   osal_task_entry entry_function_pointer;
   osal_task_entry delete_hook_pointer;
   void      *entry_arg;
   uint32    *stack_pointer;
}OS_task_internal_record_t;

/* other objects that have only an API name and no other data */
typedef struct
{
   char      queue_name[OS_MAX_API_NAME];
   uint32    max_size;
   uint32    max_depth;
} OS_queue_internal_record_t;

/* other objects that have only an API name and no other data */
typedef struct
{
   char      obj_name[OS_MAX_API_NAME];
} OS_apiname_internal_record_t;

/* directory objects */
typedef struct
{
   char          dir_name[OS_MAX_PATH_LEN];
#ifndef OSAL_OMIT_DEPRECATED
   os_dirent_t   dirent_object;
#endif
} OS_dir_internal_record_t;

typedef struct
{
   char      stream_name[OS_MAX_PATH_LEN];
   uint8     socket_domain;
   uint8     socket_type;
   uint16    stream_state;
} OS_stream_internal_record_t;

typedef struct
{
    char                timebase_name[OS_MAX_API_NAME];
    OS_TimerSync_t      external_sync;
    uint32              accuracy_usec;
    uint32              first_cb;
    uint32              freerun_time;
    uint32              nominal_start_time;
    uint32              nominal_interval_time;
} OS_timebase_internal_record_t;

#define TIMECB_FLAG_DEDICATED_TIMEBASE      0x1
typedef struct
{
    char                timer_name[OS_MAX_API_NAME];
    uint32              flags;
    uint32              timebase_ref;
    uint32              prev_ref;
    uint32              next_ref;
    uint32              backlog_resets;
    int32               wait_time;
    int32               interval_time;
    OS_ArgCallback_t    callback_ptr;
    void                *callback_arg;
} OS_timecb_internal_record_t;

typedef struct
{
   char                module_name[OS_MAX_API_NAME];
   char                file_name[OS_MAX_PATH_LEN];
   uint32              flags;
   cpuaddr             entry_point;
} OS_module_internal_record_t;

typedef struct
{
    char device_name[OS_MAX_API_NAME];      /**< The name of the underlying block device, if applicable */
    char volume_name[OS_MAX_API_NAME];
    char system_mountpt[OS_MAX_PATH_LEN];   /**< The name/prefix where the contents are accessible in the host operating system */
    char virtual_mountpt[OS_MAX_PATH_LEN];  /**< The name/prefix in the OSAL Virtual File system exposed to applications */
    char *address;
    uint32 blocksize;
    uint32 numblocks;
    uint8 flags;
    uint8 fstype;
} OS_filesys_internal_record_t;

/**
 * This flag will be set on the internal record to indicate
 * that the filesystem is "fixed" and therefore not mountable
 * or unmountable by OSAL on the system side.
 *
 * The filesystem should be configured and mounted at the right
 * spot prior to starting OSAL.
 */
#define OS_FILESYS_FLAG_IS_FIXED               0x01


/**
 * This flag will be set on the internal record to indicate
 * that the low level device driver has been started.
 *
 * On Linux, this might mean that the relevant block device
 * module has been loaded and an appropriate /dev entry exists.
 *
 * On VxWorks, this means that the low-level block device
 * is registered in the kernel and XBD layers.
 */
#define OS_FILESYS_FLAG_IS_READY               0x02

/**
 * This flag will be set on the internal record to indicate
 * that the file system is accessible within the underlying
 * operating system, i.e. that the system_mountpt is valid.
 */
#define OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM      0x10

/**
 * This flag will be set on the internal record to indicate
 * that the file system is mounted within the virtualized
 * file system exposed to applications.
 */
#define OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL     0x20


/**
 * These definitions apply to the "type" field within
 * the file system record.  This field may serve as a
 * hint or guidance for the implementation layer as to
 * what type of file system to use when initializing or
 * mounting the file system.
 */
enum
{
    OS_FILESYS_TYPE_DEFAULT = 0,    /**< Unspecified or unknown file system type */
    OS_FILESYS_TYPE_NORMAL_DISK,    /**< A traditional disk drive or something that emulates one */
    OS_FILESYS_TYPE_VOLATILE_DISK,  /**< A temporary/volatile file system or RAM disk */
    OS_FILESYS_TYPE_MTD,            /**< A "memory technology device" such as FLASH or EEPROM */
    OS_FILESYS_TYPE_MAX
};


/*
 * Variables related to the console buffer.
 * This is a simple ring buffer that decouples
 * the OS_printf() call from actual console output.
 *
 * The implementation layer may optionally spawn a
 * "utility task" or equivalent to forward data, or
 * it may process data immediately.
 */

typedef struct
{
    char device_name[OS_MAX_API_NAME];

    char *BufBase;                    /**< Start of the buffer memory */
    uint32 BufSize;                   /**< Total size of the buffer */
    volatile uint32 ReadPos;          /**< Offset of next byte to read */
    volatile uint32 WritePos;         /**< Offset of next byte to write */
    uint32 OverflowEvents;            /**< Number of lines dropped due to overflow */

} OS_console_internal_record_t;

/* Mapping of integer error number to name */
typedef struct
{
   int32 Number;
   const char *Name;
} OS_ErrorTable_Entry_t;

/* Global variables that are common between implementations */
typedef struct
{
   bool              Initialized;

   /*
    * The console device ID used for OS_printf() calls
    */
   uint32            PrintfConsoleId;

   /*
    * PrintfEnabled and ShutdownFlag are marked "volatile"
    * because they are updated and read by different threads
    */
   volatile bool     PrintfEnabled;
   volatile uint32   ShutdownFlag;
   int32             MicroSecPerTick;
   int32             TicksPerSecond;

#ifdef OS_DEBUG_PRINTF
   uint8             DebugLevel;
#endif

} OS_SharedGlobalVars_t;



/*
 * Global instantiations
 */

extern const OS_ErrorTable_Entry_t  OS_IMPL_ERROR_NAME_TABLE[];

/* The following are quick-access pointers to the various sections of the common table */
extern OS_common_record_t * const OS_global_task_table;
extern OS_common_record_t * const OS_global_queue_table;
extern OS_common_record_t * const OS_global_bin_sem_table;
extern OS_common_record_t * const OS_global_count_sem_table;
extern OS_common_record_t * const OS_global_mutex_table;
extern OS_common_record_t * const OS_global_stream_table;
extern OS_common_record_t * const OS_global_dir_table;
extern OS_common_record_t * const OS_global_timebase_table;
extern OS_common_record_t * const OS_global_timecb_table;
extern OS_common_record_t * const OS_global_module_table;
extern OS_common_record_t * const OS_global_filesys_table;
extern OS_common_record_t * const OS_global_console_table;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_task_internal_record_t           OS_task_table[OS_MAX_TASKS];
extern OS_queue_internal_record_t          OS_queue_table[OS_MAX_QUEUES];
extern OS_apiname_internal_record_t        OS_bin_sem_table[OS_MAX_BIN_SEMAPHORES];
extern OS_apiname_internal_record_t        OS_count_sem_table[OS_MAX_COUNT_SEMAPHORES];
extern OS_apiname_internal_record_t        OS_mutex_table[OS_MAX_MUTEXES];
extern OS_stream_internal_record_t         OS_stream_table[OS_MAX_NUM_OPEN_FILES];
extern OS_dir_internal_record_t            OS_dir_table[OS_MAX_NUM_OPEN_DIRS];
extern OS_timebase_internal_record_t       OS_timebase_table[OS_MAX_TIMEBASES];
extern OS_timecb_internal_record_t         OS_timecb_table[OS_MAX_TIMERS];
#if (OS_MAX_MODULES > 0)
extern OS_module_internal_record_t         OS_module_table[OS_MAX_MODULES];
#endif
extern OS_filesys_internal_record_t        OS_filesys_table[OS_MAX_FILE_SYSTEMS];
extern OS_console_internal_record_t        OS_console_table[OS_MAX_CONSOLES];



/*
 * Shared data structure for global values
 */
extern OS_SharedGlobalVars_t OS_SharedGlobalVars;

/*
 * Flags that can be used with opening of a file (bitmask)
 */
typedef enum
{
   OS_FILE_FLAG_NONE,
   OS_FILE_FLAG_CREATE = 0x01,
   OS_FILE_FLAG_TRUNCATE = 0x02,
} OS_file_flag_t;


/*
 * Type of locking that should occur when checking IDs.
 */
typedef enum
{
   OS_LOCK_MODE_NONE,           /**< Do not lock global table at all (use with caution) */
   OS_LOCK_MODE_GLOBAL,         /**< Lock during operation, and if successful, leave global table locked */
   OS_LOCK_MODE_EXCLUSIVE,      /**< Like OS_LOCK_MODE_GLOBAL but must be exclusive (refcount == zero)  */
   OS_LOCK_MODE_REFCOUNT,       /**< If operation succeeds, increment refcount and unlock global table */
} OS_lock_mode_t;

/*
 * The data type filled in by the "statvfs" call.
 *
 * This is defined here since there is no public API to get this info,
 * only the total bytes free is accessible via the current OSAL API.
 *
 * However, returning the detailed info at this level means that the
 * more detailed information could be made available with a new API call.
 */
typedef struct
{
    uint32 block_size;
    uint64 total_blocks;
    uint64 blocks_free;
} OS_statvfs_t;

/*
 * A function to perform arbitrary record matching.
 *
 * This can be used to find a record based on criteria other than the ID,
 * such as the name or any other record within the structure.
 *
 * Returns true if the id/obj matches the reference, false otherwise.
 */
typedef bool (*OS_ObjectMatchFunc_t)(void *ref, uint32 local_id, const OS_common_record_t *obj);


/****************************************************************************************
                                INITIALIZATION FUNCTIONS
 ****************************************************************************************/

/*
 * Initialization functions for each of the common sub-layers
 * These functions may or may not do anything, but the hook is provided nonetheless.
 */
int32 OS_API_Impl_Init               (uint32 idtype);
int32 OS_ObjectIdInit                (void);
int32 OS_TaskAPI_Init                (void);
int32 OS_QueueAPI_Init               (void);
int32 OS_BinSemAPI_Init              (void);
int32 OS_CountSemAPI_Init            (void);
int32 OS_MutexAPI_Init               (void);
int32 OS_ModuleAPI_Init              (void);
int32 OS_TimeBaseAPI_Init            (void);
int32 OS_TimerCbAPI_Init             (void);
int32 OS_FileAPI_Init                (void);
int32 OS_DirAPI_Init                 (void);
int32 OS_NetworkAPI_Init             (void);
int32 OS_SocketAPI_Init              (void);
int32 OS_FileSysAPI_Init             (void);
int32 OS_ConsoleAPI_Init             (void);

/*
 * The "IdleLoop_Impl" is called by the main thread once
 * everything is running and there is no more work to do.
 *
 * It should suspend the calling thread until a wakeup
 * event happens.
 */
void  OS_IdleLoop_Impl               (void);

/*
 * The "ApplicationShutdown_Impl" should wake up whatever
 * task is currently suspended in "IdleLoop_Impl" and cause
 * that thread to resume and return to its caller.
 *
 * NOTE: This should not block but it may cause the current
 * thread to be preempted by the thread that was woken up,
 * depending on priority levels.
 */
void  OS_ApplicationShutdown_Impl    (void);



/****************************************************************************************
                                ID MAPPING FUNCTIONS
 ****************************************************************************************/

/*
 * Function prototypes for routines implemented in common layers but private to OSAL
 *
 * These implement the basic OSAL ObjectID patterns - that is a 32-bit number that
 * is opaque externally, but internally identifies a specific type of object and
 * corresponding index within the local tables.
 */
uint32 OS_GetMaxForObjectType(uint32 idtype);
uint32 OS_GetBaseForObjectType(uint32 idtype);
int32 OS_ObjectIdMap(uint32 idtype, uint32 idvalue, uint32 *result);
int32 OS_ObjectIdUnMap(uint32 id, uint32 idtype, uint32 *idvalue);
int32 OS_ObjectIdFindByName (uint32 idtype, const char *name, uint32 *object_id);
int32 OS_ObjectIdToArrayIndex(uint32 idtype, uint32 id, uint32 *ArrayIndex);
int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, uint32 idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_common_record_t **record);
int32 OS_ObjectIdGetByName (OS_lock_mode_t lock_mode, uint32 idtype, const char *name, OS_common_record_t **record);
int32 OS_ObjectIdGetById(OS_lock_mode_t lock_mode, uint32 idtype, uint32 id, uint32 *array_index, OS_common_record_t **record);
int32 OS_ObjectIdAllocateNew(uint32 idtype, const char *name, uint32 *array_index, OS_common_record_t **record);
int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_common_record_t *record, uint32 *outid);
int32 OS_ObjectIdRefcountDecr(OS_common_record_t *record);


/*
 * Table locking and unlocking for global objects can be done at the shared code
 * layer but the actual implementation is OS-specific
 */
int32 OS_Lock_Global_Impl(uint32 idtype);
int32 OS_Unlock_Global_Impl(uint32 idtype);



/****************************************************************************************
                          TASK API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * The "OS_TaskEntryPoint" is a generic method implemented in the
 * shared layer that performs housekeeping and then calls the user-specified
 * entry point.  It should be the first thing called in any new task.
 */
void   OS_TaskEntryPoint              (uint32 global_task_id);

int32  OS_TaskMatch_Impl             (uint32 task_id);
int32  OS_TaskCreate_Impl            (uint32 task_id, uint32 flags);
int32  OS_TaskDelete_Impl            (uint32 task_id);
void   OS_TaskExit_Impl              (void);
int32  OS_TaskDelay_Impl             (uint32 millisecond);
int32  OS_TaskSetPriority_Impl       (uint32 task_id, uint32 new_priority);
uint32 OS_TaskGetId_Impl             (void);
int32  OS_TaskGetInfo_Impl           (uint32 task_id, OS_task_prop_t *task_prop);
int32  OS_TaskRegister_Impl          (uint32 global_task_id);




/****************************************************************************************
                 MESSAGE QUEUE API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

int32 OS_QueueCreate_Impl           (uint32 queue_id, uint32 flags);
int32 OS_QueueDelete_Impl           (uint32 queue_id);
int32 OS_QueueGet_Impl              (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout);
int32 OS_QueuePut_Impl              (uint32 queue_id, const void *data, uint32 size, uint32 flags);
int32 OS_QueueGetInfo_Impl          (uint32 queue_id, OS_queue_prop_t *queue_prop);



/****************************************************************************************
                 SEMAPHORE API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * Binary semaphores
 */
int32 OS_BinSemCreate_Impl          (uint32 sem_id, uint32 sem_initial_value, uint32 options);
int32 OS_BinSemFlush_Impl           (uint32 sem_id);
int32 OS_BinSemGive_Impl            (uint32 sem_id);
int32 OS_BinSemTake_Impl            (uint32 sem_id);
int32 OS_BinSemTimedWait_Impl       (uint32 sem_id, uint32 msecs);
int32 OS_BinSemDelete_Impl          (uint32 sem_id);
int32 OS_BinSemGetInfo_Impl         (uint32 sem_id, OS_bin_sem_prop_t *bin_prop);

/*
 * Counting semaphores
 */
int32 OS_CountSemCreate_Impl          (uint32 sem_id, uint32 sem_initial_value, uint32 options);
int32 OS_CountSemGive_Impl            (uint32 sem_id);
int32 OS_CountSemTake_Impl            (uint32 sem_id);
int32 OS_CountSemTimedWait_Impl       (uint32 sem_id, uint32 msecs);
int32 OS_CountSemDelete_Impl          (uint32 sem_id);
int32 OS_CountSemGetInfo_Impl         (uint32 sem_id, OS_count_sem_prop_t *count_prop);

/*
 * Mutex semaphores
 */
int32 OS_MutSemCreate_Impl           (uint32 sem_id, uint32 options);
int32 OS_MutSemGive_Impl             (uint32 sem_id);
int32 OS_MutSemTake_Impl             (uint32 sem_id);
int32 OS_MutSemDelete_Impl           (uint32 sem_id);
int32 OS_MutSemGetIdByName_Impl      (uint32 *sem_id, const char *sem_name);
int32 OS_MutSemGetInfo_Impl          (uint32 sem_id, OS_mut_sem_prop_t *mut_prop);


/****************************************************************************************
                 CLOCK / TIME API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * TimeBase routines implement a low-level timer tick/interrupt;
 * no callbacks to user code here.  All application callbacks are
 * done in the shared layer timer API.
 */
int32 OS_TimeBaseCreate_Impl        (uint32 timebase_id);
int32 OS_TimeBaseSet_Impl           (uint32 timebase_id, int32 start_time, int32 interval_time);
int32 OS_TimeBaseDelete_Impl        (uint32 timebase_id);
void  OS_TimeBaseLock_Impl          (uint32 timebase_id);
void  OS_TimeBaseUnlock_Impl        (uint32 timebase_id);
int32 OS_TimeBaseGetInfo_Impl       (uint32 timer_id, OS_timebase_prop_t *timer_prop);
void  OS_TimeBase_CallbackThread    (uint32 timebase_id);

/*
 * Clock API low-level handlers
 * These simply get/set the kernel RTC (if it has one)
 */
int32 OS_GetLocalTime_Impl(OS_time_t *time_struct);
int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct);




/****************************************************************************************
                 MODULE LOADER API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

int32 OS_ModuleLoad_Impl ( uint32 module_id, char *translated_path );
int32 OS_ModuleUnload_Impl ( uint32 module_id );
int32 OS_ModuleGetInfo_Impl ( uint32 module_id, OS_module_prop_t *module_prop );
int32 OS_SymbolLookup_Impl ( cpuaddr *SymbolAddress, const char *SymbolName );
int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 size_limit );



/****************************************************************************************
                 CONSOLE / DEBUG API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/


int32 OS_ConsoleCreate_Impl(uint32 local_id);

/*
 * Basic Console output implementation
 *
 * This function forwards the data from the console
 * ring buffer into the actual output device/descriptor
 */
void  OS_ConsoleOutput_Impl(uint32 local_id);

/*
 * Console output data notification
 *
 * This is a notification API that is invoked whenever there
 * is new data available in the console output buffer.
 *
 * For a synchronous console service, this may call
 * OS_ConsoleWrite_Impl() directly.  For an async console
 * service, this should wakeup the actual console servicing
 * thread.
 */
void  OS_ConsoleWakeup_Impl(uint32 local_id);



/*
 * The "OS_DEBUG" is a no-op unless OS_DEBUG_PRINTF is enabled.
 * When enabled, it is a macro that includes function/line number info.
 */
#if defined(OS_DEBUG_PRINTF)
/* Debug printfs are compiled in, but also can be disabled by a run-time flag.
 * Note that the ##__VA_ARGS__ syntax works on GCC but might need tweaks for other compilers... */
#define OS_DEBUG_LEV(l,...)   do { if (OS_SharedGlobalVars.DebugLevel >= l) printf("%s():%d:", __func__,__LINE__); printf(__VA_ARGS__); } while (0)
#define OS_DEBUG(...)         OS_DEBUG_LEV(1,__VA_ARGS__)
#else
/* Debug printfs are not compiled in at all */
#define OS_DEBUG(...)
#endif




/****************************************************************************************
                 FILE / DIRECTORY API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

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
int32 OS_GenericSeek_Impl (uint32 local_id, int32 offset, uint32 whence);
int32 OS_GenericRead_Impl (uint32 local_id, void *buffer, uint32 nbytes, int32 timeout);
int32 OS_GenericWrite_Impl(uint32 local_id, const void *buffer, uint32 nbytes, int32 timeout);
int32 OS_GenericClose_Impl(uint32 local_id);


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
int32 OS_FileStat_Impl(const char *local_path, os_fstat_t *filestat);
int32 OS_FileRemove_Impl(const char *local_path);
int32 OS_FileRename_Impl(const char *old_path, const char *new_path);
int32 OS_FileOpen_Impl(uint32 local_id, const char *local_path, int32 flags, int32 access);
int32 OS_FileChmod_Impl(const char *local_path, uint32 access);

/*
 * Output a shell command to an existing open file ID
 */
int32 OS_ShellOutputToFile_Impl(uint32 stream_id, const char* Cmd);

/*
 * Directory API abstraction layer
 */
int32 OS_DirCreate_Impl(const char *local_path, uint32 access);
int32 OS_DirOpen_Impl(uint32 local_id, const char *local_path);
int32 OS_DirClose_Impl(uint32 local_id);
int32 OS_DirRead_Impl(uint32 local_id, os_dirent_t *dirent);
int32 OS_DirRewind_Impl(uint32 local_id);
int32 OS_DirRemove_Impl(const char *local_path);

/*
 * Select API
 * Blocks until specified readable/writable conditions
 * are met on a file id or set of file ids
 */
int32 OS_SelectSingle_Impl(uint32 stream_id, uint32 *SelectFlags, int32 msecs);
int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);


/*
 * File system abstraction layer
 */
int32 OS_FileSysStartVolume_Impl (uint32 filesys_id);
int32 OS_FileSysStopVolume_Impl (uint32 filesys_id);
int32 OS_FileSysFormatVolume_Impl (uint32 filesys_id);
int32 OS_FileSysCheckVolume_Impl (uint32 filesys_id, bool repair);
int32 OS_FileSysStatVolume_Impl (uint32 filesys_id, OS_statvfs_t *result);
int32 OS_FileSysMountVolume_Impl (uint32 filesys_id);
int32 OS_FileSysUnmountVolume_Impl (uint32 filesys_id);


/****************************************************************************************
              NETWORK / SOCKET API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

int32 OS_NetworkGetHostName_Impl (char *host_name, uint32 name_len);
int32 OS_NetworkGetID_Impl       (int32 *IdBuf);

/*
 * Sockets API abstraction layer
 */
int32 OS_SocketOpen_Impl(uint32 sock_id);
int32 OS_SocketBind_Impl(uint32 sock_id, const OS_SockAddr_t *Addr);
int32 OS_SocketAccept_Impl(uint32 sock_id, uint32 connsock_id, OS_SockAddr_t *Addr, int32 timeout);
int32 OS_SocketConnect_Impl(uint32 sock_id, const OS_SockAddr_t *Addr, int32 timeout);
int32 OS_SocketRecvFrom_Impl(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout);
int32 OS_SocketSendTo_Impl(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr);
int32 OS_SocketGetInfo_Impl (uint32 sock_id, OS_socket_prop_t *sock_prop);

int32 OS_SocketAddrInit_Impl(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain);
int32 OS_SocketAddrToString_Impl(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr);
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string);
int32 OS_SocketAddrGetPort_Impl(uint16 *PortNum, const OS_SockAddr_t *Addr);
int32 OS_SocketAddrSetPort_Impl(OS_SockAddr_t *Addr, uint16 PortNum);


/****************************************************************************************
                 INTERRUPT API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * Interrupt API low-level handlers
 *
 * These are defined for completeness but may not be implementable on
 * multi-user platforms where interrupts are handled exclusively at the
 * kernel level.  They may work on single-user RTOS's like
 * VxWorks or RTEMS, but not Linux.  As such they should not be
 * relied upon.
 */
int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter);
int32 OS_IntUnlock_Impl (int32 IntLevel);
int32 OS_IntLock_Impl ( void );
int32 OS_IntEnable_Impl(int32 Level);
int32 OS_IntDisable_Impl(int32 Level);
int32 OS_IntSetMask_Impl ( uint32 MaskSetting );
int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr );

/****************************************************************************************
            FLOATING POINT CONFIG/EXCEPTION API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * FPU API low-level handlers
 * These may also not be implementable on some platforms
 */
int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, void * ExceptionHandler,
                                 int32 parameter);
int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber);
int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber);
int32 OS_FPUExcSetMask_Impl(uint32 mask);
int32 OS_FPUExcGetMask_Impl(uint32 *mask);



/****************************************************************************************
                    MEMORY HEAP API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * This may also not be implementable on some platforms
 */
int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop);



/*********************
   END os-impl.h
 *********************/
#endif  /* _osapi_os_impl_ */
