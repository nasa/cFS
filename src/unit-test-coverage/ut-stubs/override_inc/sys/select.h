/* OSAL coverage stub replacement for sys/select.h */
#ifndef _OSAL_OVERRIDE_SYS_SELECT_H_
#define _OSAL_OVERRIDE_SYS_SELECT_H_

#include <OCS_sys_select.h>

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* mappings for declarations in sys/select.h */
/* ----------------------------------------- */

#define fd_set  OCS_fd_set
#define select  OCS_select


#endif /* _OSAL_OVERRIDE_SYS_SELECT_H_ */
