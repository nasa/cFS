/* OSAL coverage stub replacement for dlfcn.h */
#ifndef _OSAL_STUB_DLFCN_H_
#define _OSAL_STUB_DLFCN_H_

/* ----------------------------------------- */
/* constants normally defined in dlfcn.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in dlfcn.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in dlfcn.h */
/* ----------------------------------------- */

extern int OCS_dlclose (void * handle);
extern char *OCS_dlerror (void);
extern void *OCS_dlopen (const char * file, int mode);
extern void *OCS_dlsym (void * handle, const char * name);


#endif /* _OSAL_STUB_DLFCN_H_ */

