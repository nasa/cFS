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
 */

#ifndef STUB_MAP_TO_REAL_H_
#define STUB_MAP_TO_REAL_H_

#define EXIT_SUCCESS                 OCS_EXIT_SUCCESS
#define EXIT_FAILURE                 OCS_EXIT_FAILURE

/* clock-related identifiers */
#define CLOCK_REALTIME               OCS_CLOCK_REALTIME
#define CLOCK_MONOTONIC              OCS_CLOCK_MONOTONIC
#define TIMER_ABSTIME                OCS_TIMER_ABSTIME

/* file-related identifiers */
#define O_RDONLY                     OCS_O_RDONLY
#define O_WRONLY                     OCS_O_WRONLY
#define O_RDWR                       OCS_O_RDWR
#define O_CREAT                      OCS_O_CREAT
#define O_TRUNC                      OCS_O_TRUNC
#define S_IRUSR                      OCS_S_IRUSR
#define S_IWUSR                      OCS_S_IWUSR
#define S_IRGRP                      OCS_S_IRGRP
#define S_IWGRP                      OCS_S_IWGRP
#define S_IROTH                      OCS_S_IROTH
#define S_IWOTH                      OCS_S_IWOTH
#define SEEK_SET                     OCS_SEEK_SET
#define SEEK_CUR                     OCS_SEEK_CUR
#define SEEK_END                     OCS_SEEK_END

#define STDOUT_FILENO                OCS_STDOUT_FILENO
#define STDIN_FILENO                 OCS_STDIN_FILENO
#define STDERR_FILENO                OCS_STDERR_FILENO

/* pthread-related identifiers */
#define PTHREAD_PRIO_INHERIT         OCS_PTHREAD_PRIO_INHERIT
#define PTHREAD_MUTEX_RECURSIVE      OCS_PTHREAD_MUTEX_RECURSIVE
#define PTHREAD_EXPLICIT_SCHED       OCS_PTHREAD_EXPLICIT_SCHED

/* signal-related identifiers */
#define SIG_SETMASK                  OCS_SIG_SETMASK
#define SIGHUP                       OCS_SIGHUP
#define SIGINT                       OCS_SIGINT
#define SIGABRT                      OCS_SIGABRT
#define SIGSEGV                      OCS_SIGSEGV
#define SIGBUS                       OCS_SIGBUS
#define SIGFPE                       OCS_SIGFPE
#define SIGILL                       OCS_SIGILL
#define SIGRTMIN                     OCS_SIGRTMIN
#define SIGRTMAX                     OCS_SIGRTMAX

/* scheduler-related identifiers */
#define SCHED_FIFO                   OCS_SCHED_FIFO
#define SCHED_RR                     OCS_SCHED_RR

/* errno identifiers */
#define EINTR                        OCS_EINTR
#define EAGAIN                       OCS_EAGAIN
#define EINVAL                       OCS_EINVAL
#define EMSGSIZE                     OCS_EMSGSIZE
#define ETIMEDOUT                    OCS_ETIMEDOUT
#define errno                        OCS_errno

/*******************************************************************************
 * ENUM definitions - keeping as an enum so the usage semantics are similar
 *******************************************************************************/


/*******************************************************************************
 * TYPE definitions normally supplied by POSIX (any header)
 *******************************************************************************/
#define DIR                         OCS_DIR
#define FILE                        OCS_FILE
#define pthread_t                   OCS_pthread_t
#define pthread_attr_t              OCS_pthread_attr_t
#define pthread_mutex_t             OCS_pthread_mutex_t
#define pthread_mutexattr_t         OCS_pthread_mutexattr_t
#define pthread_cond_t              OCS_pthread_cond_t
#define pthread_condattr_t          OCS_pthread_condattr_t
#define pthread_key_t               OCS_pthread_key_t
#define mqd_t                       OCS_mqd_t
#define sem_t                       OCS_sem_t
#define sigset_t                    OCS_sigset_t
#define sig_atomic_t                OCS_sig_atomic_t
#define ssize_t                     OCS_ssize_t
#define time_t                      OCS_time_t

/*
 * NOTE: The OCS_ structs that are not typedefs must be #defined, because the
 * code will refer to them as "struct YYY" which will not compile if YYY is a typedef.
 */
#define statfs                   OCS_statfs
#define stat                     OCS_stat
#define mq_attr                  OCS_mq_attr
#define timespec                 OCS_timespec
#define dirent                   OCS_dirent
#define sched_param              OCS_sched_param

/*******************************************************************************
 *
 * FUNCTION MAPPINGS for APIs normally supplied by POSIX (any header)
 *
 * This table below also serves as a useful list of POSIX API calls that
 * the OSAL actually uses.
 *
 *******************************************************************************/

#define accept                          OCS_accept
#define bind                            OCS_bind
#define clock_getres                    OCS_clock_getres
#define clock_gettime                   OCS_clock_gettime
#define clock_nanosleep                 OCS_clock_nanosleep
#define clock_settime                   OCS_clock_settime
#define closedir                        OCS_closedir
#define close                           OCS_close
#define connect                         OCS_connect
#define dlclose                         OCS_dlclose
#define dlerror                         OCS_dlerror
#define dlopen                          OCS_dlopen
#define dlsym                           OCS_dlsym
#define exit                            OCS_exit
#define fchmod                          OCS_fchmod
#define fclose                          OCS_fclose
#define fcntl                           OCS_fcntl
#define fgets                           OCS_fgets
#define fopen                           OCS_fopen
#define fputs                           OCS_fputs
#define getegid                         OCS_getegid
#define geteuid                         OCS_geteuid
#define gethostid                       OCS_gethostid
#define gethostname                     OCS_gethostname
#define getpid                          OCS_getpid
#define getsockopt                      OCS_getsockopt
#define htons                           OCS_htons
#define inet_ntop                       OCS_inet_ntop
#define inet_pton                       OCS_inet_pton
#define kill                            OCS_kill
#define listen                          OCS_listen
#define lseek                           OCS_lseek
#define memcpy                          OCS_memcpy
#define memset                          OCS_memset
#define mkdir                           OCS_mkdir
#define mq_close                        OCS_mq_close
#define mq_open                         OCS_mq_open
#define mq_receive                      OCS_mq_receive
#define mq_timedreceive                 OCS_mq_timedreceive
#define mq_timedsend                    OCS_mq_timedsend
#define mq_unlink                       OCS_mq_unlink
#define ntohs                           OCS_ntohs
#define opendir                         OCS_opendir
#define open                            OCS_open
#define printf(...)                     OCS_printf(__VA_ARGS__)
#define pthread_attr_destroy            OCS_pthread_attr_destroy
#define pthread_attr_getschedparam      OCS_pthread_attr_getschedparam
#define pthread_attr_init               OCS_pthread_attr_init
#define pthread_attr_setinheritsched    OCS_pthread_attr_setinheritsched
#define pthread_attr_setschedparam      OCS_pthread_attr_setschedparam
#define pthread_attr_setschedpolicy     OCS_pthread_attr_setschedpolicy
#define pthread_attr_setstacksize       OCS_pthread_attr_setstacksize
#define pthread_cancel                  OCS_pthread_cancel
#define pthread_cond_broadcast          OCS_pthread_cond_broadcast
#define pthread_cond_destroy            OCS_pthread_cond_destroy
#define pthread_cond_init               OCS_pthread_cond_init
#define pthread_cond_signal             OCS_pthread_cond_signal
#define pthread_cond_timedwait          OCS_pthread_cond_timedwait
#define pthread_cond_wait               OCS_pthread_cond_wait
#define pthread_create                  OCS_pthread_create
#define pthread_detach                  OCS_pthread_detach
#define pthread_equal                   OCS_pthread_equal
#define pthread_exit                    OCS_pthread_exit
#define pthread_getschedparam           OCS_pthread_getschedparam
#define pthread_getspecific             OCS_pthread_getspecific
#define pthread_key_create              OCS_pthread_key_create
#define pthread_mutexattr_destroy       OCS_pthread_mutexattr_destroy
#define pthread_mutexattr_init          OCS_pthread_mutexattr_init
#define pthread_mutexattr_setprotocol   OCS_pthread_mutexattr_setprotocol
#define pthread_mutexattr_settype       OCS_pthread_mutexattr_settype
#define pthread_mutex_destroy           OCS_pthread_mutex_destroy
#define pthread_mutex_init              OCS_pthread_mutex_init
#define pthread_mutex_lock              OCS_pthread_mutex_lock
#define pthread_mutex_unlock            OCS_pthread_mutex_unlock
#define pthread_self                    OCS_pthread_self
#define pthread_setschedparam           OCS_pthread_setschedparam
#define pthread_setschedprio            OCS_pthread_setschedprio
#define pthread_setspecific             OCS_pthread_setspecific
#define pthread_sigmask                 OCS_pthread_sigmask
#define readdir                         OCS_readdir
#define read                            OCS_read
#define recvfrom                        OCS_recvfrom
#define remove                          OCS_remove
#define rename                          OCS_rename
#define rewinddir                       OCS_rewinddir
#define rmdir                           OCS_rmdir
#define sched_get_priority_max          OCS_sched_get_priority_max
#define sched_get_priority_min          OCS_sched_get_priority_min
#define select                          OCS_select
#define sem_destroy                     OCS_sem_destroy
#define sem_getvalue                    OCS_sem_getvalue
#define sem_init                        OCS_sem_init
#define sem_post                        OCS_sem_post
#define sem_timedwait                   OCS_sem_timedwait
#define sem_wait                        OCS_sem_wait
#define sendto                          OCS_sendto
#define setsockopt                      OCS_setsockopt
#define sigaddset                       OCS_sigaddset
#define sigdelset                       OCS_sigdelset
#define sigemptyset                     OCS_sigemptyset
#define sigfillset                      OCS_sigfillset
#define sigismember                     OCS_sigismember
#define signal                          OCS_signal
#define sigprocmask                     OCS_sigprocmask
#define sigsuspend                      OCS_sigsuspend
#define sigwait                         OCS_sigwait
#define snprintf                        OCS_snprintf
#define socket                          OCS_socket
#define stat                            OCS_stat
#define statvfs                         OCS_statvfs
#define stdout                          OCS_stdout
#define strcmp                          OCS_strcmp
#define strcpy                          OCS_strcpy
#define strerror                        OCS_strerror
#define strlen                          OCS_strlen
#define strncmp                         OCS_strncmp
#define strncpy                         OCS_strncpy
#define strrchr                         OCS_strrchr
#define strtoul                         OCS_strtoul
#define sysconf                         OCS_sysconf
#define system                          OCS_system
#define timer_create                    OCS_timer_create
#define timer_delete                    OCS_timer_delete
#define timer_settime                   OCS_timer_settime
#define vsnprintf                       OCS_vsnprintf
#define write                           OCS_write

#endif /* STUB_MAP_TO_REAL_H_ */
