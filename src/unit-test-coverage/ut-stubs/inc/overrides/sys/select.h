/* OSAL coverage stub replacement for sys/select.h */
#ifndef _OSAL_STUB_SYS_SELECT_H_
#define _OSAL_STUB_SYS_SELECT_H_

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in sys/select.h */
/* ----------------------------------------- */

extern int OCS_select (int nfds, OCS_fd_set * readfds, OCS_fd_set * writefds, OCS_fd_set * exceptfds, struct OCS_timeval * timeout);


#endif /* _OSAL_STUB_SYS_SELECT_H_ */

