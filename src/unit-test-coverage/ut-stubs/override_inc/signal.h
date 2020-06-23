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

/* OSAL coverage stub replacement for signal.h */
#ifndef _OSAL_OVERRIDE_SIGNAL_H_
#define _OSAL_OVERRIDE_SIGNAL_H_

#include <OCS_signal.h>

/* ----------------------------------------- */
/* mappings for declarations in signal.h */
/* ----------------------------------------- */

#define SIG_SETMASK            OCS_SIG_SETMASK
#define SIGHUP                 OCS_SIGHUP
#define SIGINT                 OCS_SIGINT
#define SIGABRT                OCS_SIGABRT
#define SIGSEGV                OCS_SIGSEGV
#define SIGBUS                 OCS_SIGBUS
#define SIGFPE                 OCS_SIGFPE
#define SIGILL                 OCS_SIGILL
#define SIGRTMIN               OCS_SIGRTMIN
#define SIGRTMAX               OCS_SIGRTMAX
#define SIGEV_SIGNAL           OCS_SIGEV_SIGNAL

#define sighandler_t           OCS_sighandler_t
#define sigset_t               OCS_sigset_t
#define sig_atomic_t           OCS_sig_atomic_t
#define sigevent               OCS_sigevent


#define kill                   OCS_kill
#define sigaddset              OCS_sigaddset
#define sigdelset              OCS_sigdelset
#define sigemptyset            OCS_sigemptyset
#define sigfillset             OCS_sigfillset
#define sigismember            OCS_sigismember
#define signal                 OCS_signal
#define sigprocmask            OCS_sigprocmask
#define sigsuspend             OCS_sigsuspend
#define sigwait                OCS_sigwait

#endif /* _OSAL_OVERRIDE_SIGNAL_H_ */
