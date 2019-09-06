/* OSAL coverage stub replacement for dirent.h */
#ifndef _OSAL_STUB_DIRENT_H_
#define _OSAL_STUB_DIRENT_H_

/* ----------------------------------------- */
/* constants normally defined in dirent.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in dirent.h */
/* ----------------------------------------- */

typedef void            OCS_DIR;

struct OCS_dirent
{
    char d_name[4];
};



/* ----------------------------------------- */
/* prototypes normally declared in dirent.h */
/* ----------------------------------------- */

extern int OCS_closedir (OCS_DIR * dirp);
extern OCS_DIR *OCS_opendir (const char * name);
extern struct OCS_dirent *OCS_readdir (OCS_DIR * dirp);
extern void OCS_rewinddir (OCS_DIR * dirp);


#endif /* _OSAL_STUB_DIRENT_H_ */

