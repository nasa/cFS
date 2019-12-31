/* OSAL coverage stub replacement for sys/mman.h */
#ifndef _OSAL_STUB_SYS_MMAN_H_
#define _OSAL_STUB_SYS_MMAN_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in sys/mman.h */
/* ----------------------------------------- */
#define OCS_MAP_SHARED      0x2001
#define OCS_MAP_PRIVATE     0x2002
#define OCS_PROT_EXEC       0x2004
#define OCS_PROT_READ       0x2008
#define OCS_PROT_WRITE      0x2010
#define OCS_PROT_NONE       0x2020
#define OCS_MAP_FIXED       0x2080

/* ----------------------------------------- */
/* types normally defined in sys/mman.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in sys/mman.h */
/* ----------------------------------------- */
void *OCS_mmap(void *addr, size_t length, int prot, int flags,
                  int fd, OCS_off_t offset);
int OCS_munmap(void *addr, size_t length);


#endif /* _OSAL_STUB_SYS_MMAN_H_ */

