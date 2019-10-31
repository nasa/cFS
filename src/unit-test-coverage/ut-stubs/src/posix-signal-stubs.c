/* OSAL coverage stub replacement for signal.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/signal.h>

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


