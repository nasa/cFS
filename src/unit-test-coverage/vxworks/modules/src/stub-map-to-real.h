/*
 *  Copyright (c) 2004-2015, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file stub-map-to-real.h
 *
 *  Created on: Oct 29, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Map all symbols to the OCS counterpart for unit testing
 *
 * NOTE: Sorted alphabetically which makes things easier to
 * find as well as possibly reduce merge conflicts when things
 * are added.
 */

#ifndef STUB_MAP_TO_REAL_H_
#define STUB_MAP_TO_REAL_H_

#define accept                          OCS_accept
#define ataDevCreate                    OCS_ataDevCreate
#define bind                            OCS_bind
#define BLK_DEV                         OCS_BLK_DEV
#define BOOL                            OCS_BOOL
#define chmod                           OCS_chmod
#define clock_getres                    OCS_clock_getres
#define clock_gettime                   OCS_clock_gettime
#define clockid_t                       OCS_clockid_t
#define CLOCK_MONOTONIC                 OCS_CLOCK_MONOTONIC
#define clock_nanosleep                 OCS_clock_nanosleep
#define CLOCK_REALTIME                  OCS_CLOCK_REALTIME
#define clock_settime                   OCS_clock_settime
#define closedir                        OCS_closedir
#define close                           OCS_close
#define connect                         OCS_connect
#define cp                              OCS_cp
#define device_t                        OCS_device_t
#define dirent                          OCS_dirent
#define DIR                             OCS_DIR
#define DOS_CHK_ONLY                    OCS_DOS_CHK_ONLY
#define DOS_CHK_REPAIR                  OCS_DOS_CHK_REPAIR
#define DOS_CHK_VERB_0                  OCS_DOS_CHK_VERB_0
#define DOS_CHK_VERB_SILENT             OCS_DOS_CHK_VERB_SILENT
#define dosFsVolFormat                  OCS_dosFsVolFormat
#define DOS_OPT_BLANK                   OCS_DOS_OPT_BLANK
#define errno                           OCS_errno
#define errnoGet                        OCS_errnoGet
#define ERROR                           OCS_ERROR
#define ESPIPE                          OCS_ESPIPE
#define EXIT_FAILURE                    OCS_EXIT_FAILURE
#define EXIT_SUCCESS                    OCS_EXIT_SUCCESS
#define exit                            OCS_exit
#define fclose                          OCS_fclose
#define fcntl                           OCS_fcntl
#define fgets                           OCS_fgets
#define FILE                            OCS_FILE
#define FIOCHKDSK                       OCS_FIOCHKDSK
#define FIOUNMOUNT                      OCS_FIOUNMOUNT
#define fopen                           OCS_fopen
#define fputs                           OCS_fputs
#define free                            OCS_free
#define FUNCPTR                         OCS_FUNCPTR
#define F_GETFL                         OCS_F_GETFL
#define gethostid                       OCS_gethostid
#define gethostname                     OCS_gethostname
#define getpid                          OCS_getpid
#define getsockopt                      OCS_getsockopt
#define hostGetByName                   OCS_hostGetByName
#define htons                           OCS_htons
#define inet_ntop                       OCS_inet_ntop
#define inet_pton                       OCS_inet_pton
#define intConnect                      OCS_intConnect
#define intDisable                      OCS_intDisable
#define intEnable                       OCS_intEnable
#define intLock                         OCS_intLock
#define intUnlock                       OCS_intUnlock
#define INUM_TO_IVEC(i)                 OCS_INUM_TO_IVEC(i)
#define ioctl                           OCS_ioctl
#define itimerspec                      OCS_itimerspec
#define listen                          OCS_listen
#define LOAD_ALL_SYMBOLS                OCS_LOAD_ALL_SYMBOLS
#define loadModule                      OCS_loadModule
#define lseek                           OCS_lseek
#define malloc                          OCS_malloc
#define memcpy                          OCS_memcpy
#define memPartInfoGet                  OCS_memPartInfoGet
#define memPartShow                     OCS_memPartShow
#define MEM_PART_STATS                  OCS_MEM_PART_STATS
#define memset                          OCS_memset
#define memSysPartId                    OCS_memSysPartId
/* note: the VxWorks mkdir() function only has one argument,
 * so this is defined as a macro that adds the second argument
 * to the stub call, so the same OCS_mkdir stub can be used. */
#define mkdir(x)                        OCS_mkdir(x, 0)
#define M_objLib                        OCS_M_objLib
#define mode_t                          OCS_mode_t
#define MODULE_ID                       OCS_MODULE_ID
#define moduleInfoGet                   OCS_moduleInfoGet
#define MODULE_INFO                     OCS_MODULE_INFO
#define MSG_PRI_NORMAL                  OCS_MSG_PRI_NORMAL
#define MSG_PRI_URGENT                  OCS_MSG_PRI_URGENT
#define msgQCreate                      OCS_msgQCreate
#define msgQDelete                      OCS_msgQDelete
#define MSG_Q_FIFO                      OCS_MSG_Q_FIFO
#define MSG_Q_ID                        OCS_MSG_Q_ID
#define MSG_Q_PRIORITY                  OCS_MSG_Q_PRIORITY
#define msgQReceive                     OCS_msgQReceive
#define msgQSend                        OCS_msgQSend
#define NO_WAIT                         OCS_NO_WAIT
#define nSectors                        OCS_nSectors
#define ntohs                           OCS_ntohs
#define NULLDEV                         OCS_NULLDEV
#define O_CREAT                         OCS_O_CREAT
#define OK                              OCS_OK
#define opendir                         OCS_opendir
#define open                            OCS_open
#define off_t                           OCS_off_t
#define O_NONBLOCK                      OCS_O_NONBLOCK
#define O_RDONLY                        OCS_O_RDONLY
#define O_RDWR                          OCS_O_RDWR
#define O_TRUNC                         OCS_O_TRUNC
#define O_WRONLY                        OCS_O_WRONLY
#define PART_ID                         OCS_PART_ID
#define printf(...)                     OCS_printf(__VA_ARGS__)
#define putchar                         OCS_putchar
#define ramDevCreate                    OCS_ramDevCreate
#define readdir                         OCS_readdir
#define read                            OCS_read
#define recvfrom                        OCS_recvfrom
#define remove                          OCS_remove
#define rename                          OCS_rename
#define rewinddir                       OCS_rewinddir
#define rmdir                           OCS_rmdir
#define SEEK_CUR                        OCS_SEEK_CUR
#define SEEK_END                        OCS_SEEK_END
#define SEEK_SET                        OCS_SEEK_SET
#define select                          OCS_select
#define semBCreate                      OCS_semBCreate
#define semBInitialize                  OCS_semBInitialize
#define SEM_B_STATE                     OCS_SEM_B_STATE
#define semCCreate                      OCS_semCCreate
#define semCInitialize                  OCS_semCInitialize
#define semDelete                       OCS_semDelete
#define SEM_DELETE_SAFE                 OCS_SEM_DELETE_SAFE
#define SEM_EMPTY                       OCS_SEM_EMPTY
#define SEM_EVENTSEND_ERR_NOTIFY        OCS_SEM_EVENTSEND_ERR_NOTIFY
#define semFlush                        OCS_semFlush
#define SEM_FULL                        OCS_SEM_FULL
#define semGive                         OCS_semGive
#define SEM_ID                          OCS_SEM_ID
#define SEM_INVERSION_SAFE              OCS_SEM_INVERSION_SAFE
#define semMCreate                      OCS_semMCreate
#define semMInitialize                  OCS_semMInitialize
#define SEM_Q_FIFO                      OCS_SEM_Q_FIFO
#define SEM_Q_PRIORITY                  OCS_SEM_Q_PRIORITY
#define semTake                         OCS_semTake
#define sendto                          OCS_sendto
#define setsockopt                      OCS_setsockopt
#define shellGenericInit                OCS_shellGenericInit
#define sigaddset                       OCS_sigaddset
#define sigemptyset                     OCS_sigemptyset
#define sigevent                        OCS_sigevent
#define SIGEV_SIGNAL                    OCS_SIGEV_SIGNAL
#define sigismember                     OCS_sigismember
#define SIGRTMAX                        OCS_SIGRTMAX
#define SIGRTMIN                        OCS_SIGRTMIN
#define sigset_t                        OCS_sigset_t
#define sigwait                         OCS_sigwait
#define S_ISDIR(x)                      OCS_S_ISDIR(x)
#define S_IFDIR                         OCS_S_IFDIR
#define S_IRGRP                         OCS_S_IRGRP
#define S_IROTH                         OCS_S_IROTH
#define S_IRUSR                         OCS_S_IRUSR
#define S_IWGRP                         OCS_S_IWGRP
#define S_IWOTH                         OCS_S_IWOTH
#define S_IWUSR                         OCS_S_IWUSR
#define S_IXGRP                         OCS_S_IXGRP
#define S_IXOTH                         OCS_S_IXOTH
#define S_IXUSR                         OCS_S_IXUSR
#define snprintf                        OCS_snprintf
#define S_objLib_OBJ_DELETED            OCS_S_objLib_OBJ_DELETED
#define S_objLib_OBJ_ID_ERROR           OCS_S_objLib_OBJ_ID_ERROR
#define S_objLib_OBJ_NO_METHOD          OCS_S_objLib_OBJ_NO_METHOD
#define S_objLib_OBJ_TIMEOUT            OCS_S_objLib_OBJ_TIMEOUT
#define S_objLib_OBJ_UNAVAILABLE        OCS_S_objLib_OBJ_UNAVAILABLE
#define socket                          OCS_socket
#define ssize_t                         OCS_ssize_t
#define statfs                          OCS_statvfs
#define stat                            OCS_stat
#define STATUS                          OCS_STATUS
#define stdout                          OCS_stdout
#define STDOUT_FILENO                   OCS_STDOUT_FILENO
#define strcat                          OCS_strcat
#define strchr                          OCS_strchr
#define strcmp                          OCS_strcmp
#define strcpy                          OCS_strcpy
#define strerror                        OCS_strerror
#define strlen                          OCS_strlen
#define strncat                         OCS_strncat
#define strncmp                         OCS_strncmp
#define strncpy                         OCS_strncpy
#define strrchr                         OCS_strrchr
#define strtoul                         OCS_strtoul
#define SYMBOL_ID                       OCS_SYMBOL_ID
#define SYMBOL                          OCS_SYMBOL
#define symEach                         OCS_symEach
#define symFindByName                   OCS_symFindByName
#define SYMTAB_ID                       OCS_SYMTAB_ID
#define SYMTAB                          OCS_SYMTAB
#define SYM_TYPE                        OCS_SYM_TYPE
#define sysClkRateGet                   OCS_sysClkRateGet
#define sysconf                         OCS_sysconf
#define sysSymTbl                       OCS_sysSymTbl
#define system                          OCS_system
#define TASK_ID                         OCS_TASK_ID
#define taskActivate                    OCS_taskActivate
#define taskDelay                       OCS_taskDelay
#define taskDeleteForce                 OCS_taskDeleteForce
#define taskDelete                      OCS_taskDelete
#define taskExit                        OCS_taskExit
#define taskIdSelf                      OCS_taskIdSelf
#define taskInit                        OCS_taskInit
#define taskNameToId                    OCS_taskNameToId
#define taskPrioritySet                 OCS_taskPrioritySet
#define taskResume                      OCS_taskResume
#define taskSpawn                       OCS_taskSpawn
#define taskSuspend                     OCS_taskSuspend
#define taskTcb                         OCS_taskTcb
#define taskVarAdd                      OCS_taskVarAdd
#define TIMER_ABSTIME                   OCS_TIMER_ABSTIME
#define timer_connect                   OCS_timer_connect
#define timer_create                    OCS_timer_create
#define timer_delete                    OCS_timer_delete
#define timer_gettime                   OCS_timer_gettime
#define timer_settime                   OCS_timer_settime
#define timer_t                         OCS_timer_t
#define timespec                        OCS_timespec
#define time_t                          OCS_time_t
#define UINT16                          OCS_UINT16
#define UINT32                          OCS_UINT32
#define UINT                            OCS_UINT
#define unldByModuleId                  OCS_unldByModuleId
#define VOIDFUNCPTR                     OCS_VOIDFUNCPTR
#define vsnprintf                       OCS_vsnprintf
#define VX_BINARY_SEMAPHORE             OCS_VX_BINARY_SEMAPHORE
#define VX_COUNTING_SEMAPHORE           OCS_VX_COUNTING_SEMAPHORE
#define VX_MUTEX_SEMAPHORE              OCS_VX_MUTEX_SEMAPHORE
#define vxFpscrSet                      OCS_vxFpscrSet
#define VX_FP_TASK                      OCS_VX_FP_TASK
#define WAIT_FOREVER                    OCS_WAIT_FOREVER
#define WIND_MSG_Q_OPTION_MASK          OCS_WIND_MSG_Q_OPTION_MASK
#define WIND_TCB                        OCS_WIND_TCB
#define write                           OCS_write
#define xbdBlkDevCreateSync             OCS_xbdBlkDevCreateSync
#define xbdBlkDevDelete                 OCS_xbdBlkDevDelete

#endif /* STUB_MAP_TO_REAL_H_ */
