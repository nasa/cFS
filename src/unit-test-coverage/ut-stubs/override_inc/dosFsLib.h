/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for dosFsLib.h */
#ifndef _OSAL_OVERRIDE_DOSFSLIB_H_
#define _OSAL_OVERRIDE_DOSFSLIB_H_

#include <OCS_dosFsLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in dosFsLib.h */
/* ----------------------------------------- */
#define DOS_CHK_ONLY                    OCS_DOS_CHK_ONLY
#define DOS_CHK_REPAIR                  OCS_DOS_CHK_REPAIR
#define DOS_CHK_VERB_0                  OCS_DOS_CHK_VERB_0
#define DOS_CHK_VERB_SILENT             OCS_DOS_CHK_VERB_SILENT
#define DOS_OPT_BLANK                   OCS_DOS_OPT_BLANK

#define dosFsVolFormat                  OCS_dosFsVolFormat



#endif /* _OSAL_OVERRIDE_DOSFSLIB_H_ */
