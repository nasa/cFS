/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * File   : bsp_voltab.c
 *
 * BSP Volume table for file systems
 *
 *  Define a default OS volume table, which has no valid entries in it.
 *  This is a deprecated structure in the process of being phased out.
 *
 *  This source file should only be compiled and included in "osal_bsp" when
 *  OSAL_OMIT_DEPRECATED is false/unset.
 *
 *  This serves to decouple the PSP changes and the OSAL_OMIT_DEPRECATED setting -
 *  allowing a PSP to be updated to use only the recommended API and remove the
 *  OS_VolumeTable, while still allowing code to build and link when OSAL_OMIT_DEPRECATED
 *  is not set.
 *
 *  If a classic PSP is still providing this symbol, then the PSP-provided symbol will
 *  be used, and this one will be ignored, preserving old behavior.
 *
 *  If an updated PSP has removed this symbol, then this will be used to satisfy linking
 *  requirements when building without OSAL_OMIT_DEPRECATED.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include "common_types.h"
#include "osapi.h"

OS_VolumeInfo_t OS_VolumeTable [OS_MAX_FILE_SYSTEMS] = 
{
        /* Dev Name  Phys Dev  Vol Type        Volatile?  Free?     IsMounted? Volname  MountPt BlockSz */
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },

        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },

        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
        {"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        }

};



