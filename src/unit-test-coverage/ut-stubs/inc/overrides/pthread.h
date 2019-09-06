/* OSAL coverage stub replacement for pthread.h */
#ifndef _OSAL_STUB_PTHREAD_H_
#define _OSAL_STUB_PTHREAD_H_

/* ----------------------------------------- */
/* constants normally defined in pthread.h */
/* ----------------------------------------- */

#define OCS_PTHREAD_PRIO_INHERIT         0x1000
#define OCS_PTHREAD_MUTEX_RECURSIVE      0x1001
#define OCS_PTHREAD_EXPLICIT_SCHED       0x1002

/* ----------------------------------------- */
/* types normally defined in pthread.h */
/* ----------------------------------------- */

typedef int             OCS_pthread_t;
typedef int             OCS_pthread_attr_t;
typedef int             OCS_pthread_mutex_t;
typedef int             OCS_pthread_mutexattr_t;
typedef int             OCS_pthread_cond_t;
typedef int             OCS_pthread_condattr_t;
typedef int             OCS_pthread_key_t;
typedef unsigned long   OCS_size_t;
typedef unsigned int    OCS_sigset_t;

struct OCS_sched_param;
struct OCS_timespec;

/* ----------------------------------------- */
/* prototypes normally declared in pthread.h */
/* ----------------------------------------- */

extern int OCS_pthread_attr_destroy (OCS_pthread_attr_t * attr);
extern int OCS_pthread_attr_getschedparam (const OCS_pthread_attr_t * attr, struct OCS_sched_param * param);
extern int OCS_pthread_attr_init (OCS_pthread_attr_t * attr);
extern int OCS_pthread_attr_setinheritsched (OCS_pthread_attr_t * attr, int inherit);
extern int OCS_pthread_attr_setschedparam (OCS_pthread_attr_t * attr, const struct OCS_sched_param * param);
extern int OCS_pthread_attr_setschedpolicy (OCS_pthread_attr_t * attr, int policy);
extern int OCS_pthread_attr_setstacksize (OCS_pthread_attr_t * attr, OCS_size_t stacksize);
extern int OCS_pthread_cancel (OCS_pthread_t th);
extern int OCS_pthread_cond_broadcast (OCS_pthread_cond_t * cond);
extern int OCS_pthread_cond_destroy (OCS_pthread_cond_t * cond);
extern int OCS_pthread_cond_init (OCS_pthread_cond_t * cond, const OCS_pthread_condattr_t * cond_attr);
extern int OCS_pthread_cond_signal (OCS_pthread_cond_t * cond);
extern int OCS_pthread_cond_timedwait (OCS_pthread_cond_t * cond, OCS_pthread_mutex_t * mutex, const struct OCS_timespec * abstime);
extern int OCS_pthread_cond_wait (OCS_pthread_cond_t * cond, OCS_pthread_mutex_t * mutex);
extern int OCS_pthread_create (OCS_pthread_t * newthread, const OCS_pthread_attr_t * attr, void *(* start_routine) (void *), void * arg);
extern int OCS_pthread_detach (OCS_pthread_t th);
extern int OCS_pthread_equal (OCS_pthread_t thread1, OCS_pthread_t thread2);
extern void OCS_pthread_exit (void * retval);
extern int OCS_pthread_getschedparam (OCS_pthread_t target_thread, int * policy, struct OCS_sched_param * param);
extern void *OCS_pthread_getspecific (OCS_pthread_key_t key);
extern int OCS_pthread_key_create (OCS_pthread_key_t * key, void (* destr_function) (void *));
extern int OCS_pthread_mutexattr_destroy (OCS_pthread_mutexattr_t * attr);
extern int OCS_pthread_mutexattr_init (OCS_pthread_mutexattr_t * attr);
extern int OCS_pthread_mutexattr_setprotocol (OCS_pthread_mutexattr_t * attr, int protocol);
extern int OCS_pthread_mutexattr_settype (OCS_pthread_mutexattr_t * attr, int kind);
extern int OCS_pthread_mutex_destroy (OCS_pthread_mutex_t * mutex);
extern int OCS_pthread_mutex_init (OCS_pthread_mutex_t * mutex, const OCS_pthread_mutexattr_t * mutexattr);
extern int OCS_pthread_mutex_lock (OCS_pthread_mutex_t * mutex);
extern int OCS_pthread_mutex_unlock (OCS_pthread_mutex_t * mutex);
extern OCS_pthread_t OCS_pthread_self (void);
extern int OCS_pthread_setschedparam (OCS_pthread_t target_thread, int policy, const struct OCS_sched_param * param);
extern int OCS_pthread_setschedprio (OCS_pthread_t target_thread, int prio);
extern int OCS_pthread_setspecific (OCS_pthread_key_t key, const void * pointer);
extern int OCS_pthread_sigmask (int how, const OCS_sigset_t *set, OCS_sigset_t * oldset);



#endif /* _OSAL_STUB_PTHREAD_H_ */

