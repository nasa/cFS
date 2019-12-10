/* OSAL coverage stub replacement for semaphore.h */
#ifndef _OSAL_OVERRIDE_SEMAPHORE_H_
#define _OSAL_OVERRIDE_SEMAPHORE_H_

#include <OCS_semaphore.h>
#include <time.h>

/* ----------------------------------------- */
/* mappings for declarations in semaphore.h */
/* ----------------------------------------- */
#define sem_t           OCS_sem_t
#define sem_destroy     OCS_sem_destroy
#define sem_getvalue    OCS_sem_getvalue
#define sem_init        OCS_sem_init
#define sem_post        OCS_sem_post
#define sem_timedwait   OCS_sem_timedwait
#define sem_wait        OCS_sem_wait


#endif /* _OSAL_OVERRIDE_SEMAPHORE_H_ */
