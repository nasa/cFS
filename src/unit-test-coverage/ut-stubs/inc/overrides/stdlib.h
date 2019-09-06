/* OSAL coverage stub replacement for stdlib.h */
#ifndef _OSAL_STUB_STDLIB_H_
#define _OSAL_STUB_STDLIB_H_

/* ----------------------------------------- */
/* constants normally defined in stdlib.h */
/* ----------------------------------------- */

#define OCS_EXIT_SUCCESS            0x0100
#define OCS_EXIT_FAILURE            0x0101


/* ----------------------------------------- */
/* types normally defined in stdlib.h */
/* ----------------------------------------- */
typedef unsigned long OCS_size_t;

/* ----------------------------------------- */
/* prototypes normally declared in stdlib.h */
/* ----------------------------------------- */

extern void OCS_exit (int status);
extern unsigned long int OCS_strtoul (const char * nptr, char ** endptr, int base);
extern int OCS_system (const char * command);
extern void *OCS_malloc(OCS_size_t sz);
extern void OCS_free(void *ptr);


#endif /* _OSAL_STUB_STDLIB_H_ */

