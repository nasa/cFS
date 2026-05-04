/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *  The CFS Checksum (CS) Application Default Tables Table Definition
 */

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "cs_msgdefs.h"
#include "cs_platform_cfg.h"
#include "cs_tbldefs.h"

CS_Def_Tables_Table_Entry_t CS_TablesTable[CS_MAX_NUM_TABLES_TABLE_ENTRIES] = {
    /*         State             Name   */

    /*   0 */
    { .State = CS_ChecksumState_DISABLED, .Name = "SAMPLE_APP.ExampleTable" },
    /*   1 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   2 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   3 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   4 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   5 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   6 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   7 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   8 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*   9 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  10 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  11 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  12 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  13 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  14 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  15 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  16 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  17 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  18 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  19 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  20 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  21 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  22 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        },
    /*  23 */
    { .State = CS_ChecksumState_EMPTY,    .Name = ""                        }
};

/*
** Table file header
*/
CFE_TBL_FILEDEF(CS_TablesTable, CS.DefTablesTbl, CS Tables Tbl, cs_tablestbl.tbl)
