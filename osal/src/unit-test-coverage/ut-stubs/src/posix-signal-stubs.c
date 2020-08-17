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
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_signal.h>

int OCS_kill (OCS_pid_t pid, int sig)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_kill);

    return Status;
}


int OCS_sigaddset (OCS_sigset_t * set, int signo)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigaddset);

    return Status;
}


int OCS_sigdelset (OCS_sigset_t * set, int signo)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigdelset);

    return Status;
}


int OCS_sigemptyset (OCS_sigset_t * set)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigemptyset);

    return Status;
}


int OCS_sigfillset (OCS_sigset_t * set)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigfillset);

    return Status;
}


int OCS_sigismember (const OCS_sigset_t * set, int signo)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigismember);

    return Status;
}


OCS_sighandler_t OCS_signal(int signum, OCS_sighandler_t handler)
{
    UT_DEFAULT_IMPL(OCS_signal);
    return (OCS_sighandler_t)0;
}


int OCS_sigprocmask (int how, const OCS_sigset_t * set, OCS_sigset_t * oset)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigprocmask);

    return Status;
}


int OCS_sigsuspend (const OCS_sigset_t * set)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigsuspend);

    return Status;
}


int OCS_sigwait (const OCS_sigset_t * set, int * sig)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sigwait);

    /*
     * The "sig" value is an output, which the UT test
     * case may need to control.
     */
    if (UT_Stub_CopyToLocal(UT_KEY(OCS_sigwait),sig,sizeof(*sig)) < sizeof(*sig))
    {
        *sig = 1;
    }

    return Status;
}


