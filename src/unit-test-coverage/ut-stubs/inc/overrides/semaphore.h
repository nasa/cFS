/* OSAL coverage stub replacement for semaphore.h */
#ifndef _OSAL_STUB_SEMAPHORE_H_
#define _OSAL_STUB_SEMAPHORE_H_

/* ----------------------------------------- */
/* constants normally defined in semaphore.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in semaphore.h */
/* ----------------------------------------- */

typedef struct
{
    int v;
} OCS_sem_t;

struct OCS_timespec;

/* ----------------------------------------- */
/* prototypes normally declared in semaphore.h */
/* ----------------------------------------- */

extern int OCS_sem_destroy (OCS_sem_t * sem);
extern int OCS_sem_getvalue (OCS_sem_t * sem, int * sval);
extern int OCS_sem_init (OCS_sem_t * sem, int pshared, unsigned int value);
extern int OCS_sem_post (OCS_sem_t * sem);
extern int OCS_sem_timedwait (OCS_sem_t * sem, const struct OCS_timespec * abstime);
extern int OCS_sem_wait (OCS_sem_t * sem);


#endif /* _OSAL_STUB_SEMAPHORE_H_ */

