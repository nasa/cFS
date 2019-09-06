/*
**  GSC-18128-1, "Core Flight Executive Version 6.6"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
** File   : cfe_psp_voltab.c
** Author : Nicholas Yanchik / GSFC Code 582
**
** cFE PSP Volume table for file systems
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include "common_types.h"
#include "osapi.h"
#include "osconfig.h"   

#ifdef VOLTAB_PATH

#define VOLTAB_MACRO                                                                      \
OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES] =                                      \
{                                                                                         \
{"/ramdev0", VOLTAB_PATH, FS_BASED, true,  true,  false, " ",  " ",   0     },            \
{"/cf",      VOLTAB_PATH, FS_BASED, false, false, true,  "CF", "/cf", 512   },            \
{"/ramdev1", " ",                    FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"/ramdev2", " ",                    FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"/ramdev3", " ",                    FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"/ramdev4", " ",                    FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }, \
{"unused",   "unused",               FS_BASED, true,  true,  false, " ",  " ",   0     }  \
};

VOLTAB_MACRO

#else
/* 
** OSAL volume table. This is the only file in the PSP that still has the 
** OS_ naming convention, since it belongs to the OSAL. 
*/
OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES] = 
{
/* Dev Name  Phys Dev  Vol Type        Volatile?  Free?     IsMounted? Volname  MountPt BlockSz */
{"/ramdev0", "./ram",       FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev1", "./ram1",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev2", "./ram2",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev3", "./ram3",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev4", "./ram4",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },

/*
** The following entry is a "pre-mounted" path to a non-volatile device
*/
{"/eedev0",  "./cf",      FS_BASED,        false,     false,    true,     "CF",      "/cf",     512   },

{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        }
};


#endif

