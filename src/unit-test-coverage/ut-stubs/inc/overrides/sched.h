/* OSAL coverage stub replacement for sched.h */
#ifndef _OSAL_STUB_SCHED_H_
#define _OSAL_STUB_SCHED_H_

/* ----------------------------------------- */
/* constants normally defined in sched.h */
/* ----------------------------------------- */
#define OCS_SCHED_FIFO                   0x1200
#define OCS_SCHED_RR                     0x1201

/* ----------------------------------------- */
/* types normally defined in sched.h */
/* ----------------------------------------- */
struct OCS_sched_param
{
    int sched_priority;
};


/* ----------------------------------------- */
/* prototypes normally declared in sched.h */
/* ----------------------------------------- */

extern int OCS_sched_get_priority_max(int policy);
extern int OCS_sched_get_priority_min(int policy);


#endif /* _OSAL_STUB_SCHED_H_ */

