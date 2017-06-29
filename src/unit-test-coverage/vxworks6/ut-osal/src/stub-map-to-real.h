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
 * Placeholder for file content description
 */

#ifndef STUB_MAP_TO_REAL_H_
#define STUB_MAP_TO_REAL_H_

/* Include all the stub definitions */
#include "vxworks6-coverage-stubs.h"

/* in all cases, UNDEFINE the macro if the C library already defined it as a macro */

#define LOAD_ALL_SYMBOLS             VCS_LOAD_ALL_SYMBOLS
#define O_RDONLY                     VCS_O_RDONLY
#define O_WRONLY                     VCS_O_WRONLY
#define O_RDWR                       VCS_O_RDWR
#define O_CREAT                      VCS_O_CREAT
#define O_TRUNC                      VCS_O_TRUNC
#define MSG_Q_FIFO                   VCS_MSG_Q_FIFO
#define MSG_Q_PRIORITY               VCS_MSG_Q_PRIORITY
#define WIND_MSG_Q_OPTION_MASK       VCS_WIND_MSG_Q_OPTION_MASK
#define MSG_PRI_NORMAL               VCS_MSG_PRI_NORMAL
#define MSG_PRI_URGENT               VCS_MSG_PRI_URGENT
#define M_objLib                     VCS_M_objLib
#define S_objLib_OBJ_ID_ERROR        VCS_S_objLib_OBJ_ID_ERROR
#define S_objLib_OBJ_UNAVAILABLE     VCS_S_objLib_OBJ_UNAVAILABLE
#define S_objLib_OBJ_DELETED         VCS_S_objLib_OBJ_DELETED
#define S_objLib_OBJ_TIMEOUT         VCS_S_objLib_OBJ_TIMEOUT
#define S_objLib_OBJ_NO_METHOD       VCS_S_objLib_OBJ_NO_METHOD
#define VX_FP_TASK                   VCS_VX_FP_TASK
#define INUM_TO_IVEC(uint32)         VCS_INUM_TO_IVEC(uint32)
#define NULLDEV                      VCS_NULLDEV
#define DOS_CHK_ONLY                 VCS_DOS_CHK_ONLY
#define DOS_CHK_REPAIR               VCS_DOS_CHK_REPAIR
#define DOS_CHK_VERB_0               VCS_DOS_CHK_VERB_0
#define DOS_CHK_VERB_SILENT          VCS_DOS_CHK_VERB_SILENT
#define DOS_OPT_BLANK                VCS_DOS_OPT_BLANK
#define FIOUNMOUNT                   VCS_FIOUNMOUNT
#define FIOCHKDSK                    VCS_FIOCHKDSK
#define S_IRUSR                      VCS_S_IRUSR
#define S_IWUSR                      VCS_S_IWUSR
#define S_IRGRP                      VCS_S_IRGRP
#define S_IWGRP                      VCS_S_IWGRP
#define S_IROTH                      VCS_S_IROTH
#define S_IWOTH                      VCS_S_IWOTH
#define CLOCK_REALTIME               VCS_CLOCK_REALTIME
#define EXIT_SUCCESS                 VCS_EXIT_SUCCESS
#define EXIT_FAILURE                 VCS_EXIT_FAILURE
#define SEEK_SET                     VCS_SEEK_SET
#define SEEK_CUR                     VCS_SEEK_CUR
#define SEEK_END                     VCS_SEEK_END


/*******************************************************************************
 * ENUM definitions - keeping as an enum so the usage semantics are similar
 *******************************************************************************/

#define SEM_Q_FIFO                  VCS_SEM_Q_FIFO
#define SEM_Q_PRIORITY              VCS_SEM_Q_PRIORITY
#define SEM_DELETE_SAFE             VCS_SEM_DELETE_SAFE
#define SEM_INVERSION_SAFE          VCS_SEM_INVERSION_SAFE
#define SEM_EVENTSEND_ERR_NOTIFY    VCS_SEM_EVENTSEND_ERR_NOTIFY
#define SEM_EMPTY                   VCS_SEM_EMPTY
#define SEM_FULL                    VCS_SEM_FULL
#define ERROR                       VCS_ERROR
#define OK                          VCS_OK
#define WAIT_FOREVER                VCS_WAIT_FOREVER
#define NO_WAIT                     VCS_NO_WAIT


/*******************************************************************************
 * TYPE definitions normally supplied by VxWorks (any header)
 *******************************************************************************/

#define SEM_B_STATE                 VCS_SEM_B_STATE
#define STATUS                      VCS_STATUS
#define BLK_DEV                     VCS_BLK_DEV
#define device_t                    VCS_device_t
#define timer_t                     VCS_timer_t
#define BOOL                        VCS_BOOL
#define SEM_ID                      VCS_SEM_ID
#define MODULE_ID                   VCS_MODULE_ID
#define SYM_TYPE                    VCS_SYM_TYPE
#define PART_ID                     VCS_PART_ID
#define UINT                        VCS_UINT
#define UINT16                      VCS_UINT16
#define UINT32                      VCS_UINT32
#define MSG_Q_ID                    VCS_MSG_Q_ID
#define FUNCPTR                     VCS_FUNCPTR
#define VOIDFUNCPTR                 VCS_VOIDFUNCPTR
#define MEM_PART_STATS              VCS_MEM_PART_STATS
#define SEMAPHORE                   VCS_SEMAPHORE
#define MODULE_INFO                 VCS_MODULE_INFO
#define SYMBOL                      VCS_SYMBOL
#define SYMTAB                      VCS_SYMTAB
#define SYMTAB_ID                   VCS_SYMTAB_ID
#define clockid_t                   VCS_clockid_t
#define mode_t                      VCS_mode_t
#define DIR                         VCS_DIR


/* NOTE: The VCS_ structs that are not typedefs must be #defined, because the
 * code will refer to them as "struct YYY" which will not compile if YYY is a typedef.
 */
#define semaphore                VCS_semaphore
#define statfs                   VCS_statfs
#define stat                     VCS_stat
#define symbol                   VCS_symbol
#define symtab                   VCS_symtab



/*******************************************************************************
 * FUNCTION PROTOTYPE definitions normally supplied by VxWorks (any header)
 *******************************************************************************/

#define loadModule               VCS_loadModule
#define moduleInfoGet            VCS_moduleInfoGet
#define ramDevCreate             VCS_ramDevCreate
#define symFindByName            VCS_symFindByName
#define symEach                  VCS_symEach
#define timer_connect            VCS_timer_connect
#define unldByModuleId           VCS_unldByModuleId
#define semMCreate               VCS_semMCreate
#define semTake                  VCS_semTake
#define semGive                  VCS_semGive
#define semBInitialize           VCS_semBInitialize
#define VX_BINARY_SEMAPHORE      VCS_VX_BINARY_SEMAPHORE
#define open                     VCS_open
#define close                    VCS_close
#define read                     VCS_read
#define write                    VCS_write
#define lseek                    VCS_lseek
#define cp                       VCS_cp
#define rmdir                    VCS_rmdir
#define intConnect               VCS_intConnect
#define intDisable               VCS_intDisable
#define intEnable                VCS_intEnable
#define intLock                  VCS_intLock
#define intUnlock                VCS_intUnlock
#define memPartShow              VCS_memPartShow
#define memPartInfoGet           VCS_memPartInfoGet
#define msgQCreate               VCS_msgQCreate
#define msgQDelete               VCS_msgQDelete
#define msgQReceive              VCS_msgQReceive
#define msgQSend                 VCS_msgQSend
#define semBCreate               VCS_semBCreate
#define semCCreate               VCS_semCCreate
#define semDelete                VCS_semDelete
#define semFlush                 VCS_semFlush
#define shellGenericInit         VCS_shellGenericInit
#define taskDelay                VCS_taskDelay
#define taskDelete               VCS_taskDelete
#define taskDeleteForce          VCS_taskDeleteForce
#define taskExit                 VCS_taskExit
#define taskIdSelf               VCS_taskIdSelf
#define taskNameToId             VCS_taskNameToId
#define taskPrioritySet          VCS_taskPrioritySet
#define taskSpawn                VCS_taskSpawn
#define taskVarAdd               VCS_taskVarAdd
#define sysClkRateGet            VCS_sysClkRateGet
#define vxFpscrGet               VCS_vxFpscrGet
#define vxFpscrSet               VCS_vxFpscrSet
#define dosFsVolFormat           VCS_dosFsVolFormat
#define nSectors                 VCS_nSectors
#define xbdBlkDevCreateSync      VCS_xbdBlkDevCreateSync
#define memSysPartId             VCS_memSysPartId
#define errno                    VCS_errno
#define timespec                 VCS_timespec
#define itimerspec               VCS_itimerspec
#define mkdir                    VCS_mkdir
#define open                     VCS_open
#define gethostname              VCS_gethostname
#define hostGetByName            VCS_hostGetByName
#define ioctl                    VCS_ioctl
#define clock_getres             VCS_clock_getres
#define timer_create             VCS_timer_create
#define timer_delete             VCS_timer_delete
#define clock_gettime            VCS_clock_gettime
#define clock_settime            VCS_clock_settime
#define timer_settime            VCS_timer_settime
#define strncpy                  VCS_strncpy
#define strlen                   VCS_strlen
#define strcmp                   VCS_strcmp
#define strcpy                   VCS_strcpy
#define strncpy                  VCS_strncpy
#define vsnprintf                VCS_vsnprintf
#define opendir                  VCS_opendir
#define closedir                 VCS_closedir
#define rewinddir                VCS_rewinddir
#define dirent                   VCS_dirent
#define readdir                  VCS_readdir
#define strrchr                  VCS_strrchr
#define strchr                   VCS_strchr
#define open                     VCS_open
#define exit                     VCS_exit
#define memset                   VCS_memset
#define printf                   VCS_printf
#define remove                   VCS_remove
#define rename                   VCS_rename
#define strcat                   VCS_strcat
#define strncat                  VCS_strncat
#define strncmp                  VCS_strncmp
#define snprintf                 VCS_snprintf

#endif /* STUB_MAP_TO_REAL_H_ */
