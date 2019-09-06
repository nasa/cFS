/* OSAL coverage stub replacement for mqueue.h */
#ifndef _OSAL_STUB_MQUEUE_H_
#define _OSAL_STUB_MQUEUE_H_

/* ----------------------------------------- */
/* constants normally defined in mqueue.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in mqueue.h */
/* ----------------------------------------- */

typedef int             OCS_mqd_t;
typedef long            OCS_ssize_t;
typedef unsigned long   OCS_size_t;

struct OCS_mq_attr
{
    unsigned int mq_maxmsg;
    unsigned int mq_msgsize;
};

struct OCS_timespec;

/* ----------------------------------------- */
/* prototypes normally declared in mqueue.h */
/* ----------------------------------------- */

extern int OCS_mq_close (OCS_mqd_t mqdes);
extern OCS_mqd_t OCS_mq_open (const char * name, int oflag, ...);
extern OCS_ssize_t OCS_mq_receive (OCS_mqd_t mqdes, char * msg_ptr, OCS_size_t msg_len, unsigned int * msg_prio);
extern OCS_ssize_t OCS_mq_timedreceive (OCS_mqd_t mqdes, char * msg_ptr, OCS_size_t msg_len, unsigned int * msg_prio, const struct OCS_timespec * abs_timeout);
extern int OCS_mq_timedsend (OCS_mqd_t mqdes, const char * msg_ptr, OCS_size_t msg_len, unsigned int msg_prio, const struct OCS_timespec * abs_timeout);
extern int OCS_mq_unlink (const char * name);


#endif /* _OSAL_STUB_MQUEUE_H_ */

