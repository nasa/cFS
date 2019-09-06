/* OSAL coverage stub replacement for pthread.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/pthread.h>

int OCS_pthread_attr_destroy (OCS_pthread_attr_t * attr)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_attr_destroy);

    return Status;
}

int OCS_pthread_attr_getschedparam (const OCS_pthread_attr_t * attr, struct OCS_sched_param * param)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_attr_getschedparam);

    return Status;
}

int OCS_pthread_attr_init (OCS_pthread_attr_t * attr)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_attr_init);

    return Status;
}

int OCS_pthread_attr_setinheritsched (OCS_pthread_attr_t * attr, int inherit)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_attr_setinheritsched);

    return Status;
}

int OCS_pthread_attr_setschedparam (OCS_pthread_attr_t * attr, const struct OCS_sched_param * param)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_attr_setschedparam);

    return Status;
}

int OCS_pthread_attr_setschedpolicy (OCS_pthread_attr_t * attr, int policy)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_attr_setschedpolicy);

    return Status;
}

int OCS_pthread_attr_setstacksize (OCS_pthread_attr_t * attr, size_t stacksize)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_attr_setstacksize);

    return Status;
}

int OCS_pthread_cancel (OCS_pthread_t th)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_cancel);

    return Status;
}

int OCS_pthread_cond_broadcast (OCS_pthread_cond_t * cond)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_cond_broadcast);

    return Status;
}

int OCS_pthread_cond_destroy (OCS_pthread_cond_t * cond)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_cond_destroy);

    return Status;
}

int OCS_pthread_cond_init (OCS_pthread_cond_t * cond, const OCS_pthread_condattr_t * cond_attr)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_cond_init);

    return Status;
}
int OCS_pthread_cond_signal (OCS_pthread_cond_t * cond)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_cond_signal);

    return Status;
}

int OCS_pthread_cond_timedwait (OCS_pthread_cond_t * cond, OCS_pthread_mutex_t * mutex, const struct OCS_timespec * abstime)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_cond_timedwait);

    return Status;
}

int OCS_pthread_cond_wait (OCS_pthread_cond_t * cond, OCS_pthread_mutex_t * mutex)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_cond_wait);

    return Status;
}

int OCS_pthread_create (OCS_pthread_t * newthread, const OCS_pthread_attr_t * attr, void *(* start_routine) (void *), void * arg)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_create);

    return Status;
}

int OCS_pthread_detach (OCS_pthread_t th)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_detach);

    return Status;
}

int OCS_pthread_equal (OCS_pthread_t thread1, OCS_pthread_t thread2)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_pthread_equal, 2);

    if (Status == 2)
    {
        Status = (thread1 == thread2);
    }

    return Status;
}

void OCS_pthread_exit (void * retval)
{
    UT_DEFAULT_IMPL(OCS_pthread_exit);
}

int OCS_pthread_getschedparam (OCS_pthread_t target_thread, int * policy, struct OCS_sched_param * param)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_getschedparam);

    return Status;
}

void *OCS_pthread_getspecific (OCS_pthread_key_t key)
{
    int32 Status;
    void *retval;

    Status = UT_DEFAULT_IMPL(OCS_pthread_getspecific);

    if (Status != 0 ||
            UT_Stub_CopyToLocal(UT_KEY(OCS_pthread_getspecific), &retval, sizeof(retval)) < sizeof(retval))
    {
        retval = NULL;
    }


    return retval;
}

int OCS_pthread_key_create (OCS_pthread_key_t * key, void (* destr_function) (void *))
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_key_create);

    return Status;
}

int OCS_pthread_mutexattr_destroy (OCS_pthread_mutexattr_t * attr)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutexattr_destroy);

    return Status;
}

int OCS_pthread_mutexattr_init (OCS_pthread_mutexattr_t * attr)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutexattr_init);

    return Status;
}

int OCS_pthread_mutexattr_setprotocol (OCS_pthread_mutexattr_t * attr, int protocol)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutexattr_setprotocol);

    return Status;
}

int OCS_pthread_mutexattr_settype (OCS_pthread_mutexattr_t * attr, int kind)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutexattr_settype);

    return Status;
}

int OCS_pthread_mutex_destroy (OCS_pthread_mutex_t * mutex)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutex_destroy);

    return Status;
}

int OCS_pthread_mutex_init (OCS_pthread_mutex_t * mutex, const OCS_pthread_mutexattr_t * mutexattr)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutex_init);

    return Status;
}

int OCS_pthread_mutex_lock (OCS_pthread_mutex_t * mutex)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutex_lock);

    return Status;
}

int OCS_pthread_mutex_unlock (OCS_pthread_mutex_t * mutex)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_mutex_unlock);

    return Status;
}

OCS_pthread_t OCS_pthread_self (void)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_self);

    return Status;
}

int OCS_pthread_setschedparam (OCS_pthread_t target_thread, int policy, const struct OCS_sched_param * param)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_setschedparam);

    return Status;
}

int OCS_pthread_setschedprio (OCS_pthread_t target_thread, int prio)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_setschedprio);

    return Status;
}

int OCS_pthread_setspecific (OCS_pthread_key_t key, const void * pointer)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_setspecific);

    return Status;
}

int OCS_pthread_sigmask (int how, const OCS_sigset_t *set, OCS_sigset_t * oldset)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_pthread_sigmask);

    return Status;
}

