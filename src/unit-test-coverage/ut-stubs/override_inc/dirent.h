/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for dirent.h */
#ifndef _OSAL_OVERRIDE_DIRENT_H_
#define _OSAL_OVERRIDE_DIRENT_H_

#include <OCS_dirent.h>

/* ----------------------------------------- */
/* mappings for declarations in dirent.h */
/* ----------------------------------------- */

#define DIR                             OCS_DIR
#define dirent                          OCS_dirent
#define closedir                        OCS_closedir
#define opendir                         OCS_opendir
#define readdir                         OCS_readdir
#define rewinddir                       OCS_rewinddir

#endif /* _OSAL_OVERRIDE_DIRENT_H_ */
