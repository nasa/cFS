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

#include "sbn_tbl.h"
#include "cfe_tbl_filedef.h"

SBN_ConfTbl_t SBN_ConfTbl = {
    .ProtocolModules = {
        { /* [0] */
            .Name        = "UDP",
            .LibFileName = "/cf/sbn_udp.so",
            .LibSymbol   = "SBN_UDP_Ops",
            .BaseEID     = 0x0100
        }
    },
    .ProtocolCnt   = 1,
    .FilterModules = {
        { /* [0] */
            .Name        = "Remap",
            .LibFileName = "/cf/sbn_f_remap.so",
            .LibSymbol   = "SBN_F_Remap",
            .BaseEID     = 0x1000
        }
    },
    .FilterCnt = 1,
    .Peers     = {
        { /* [0] */
            .ProcessorID  = 1,
            .SpacecraftID = 0x42,
            .NetNum       = 0,
            .ProtocolName = "UDP",
            .Filters      = {"Remap"},
            .Address      = "127.0.0.1:3234",
            .TaskFlags    = SBN_TASK_POLL
        },
        { /* [1] */
            .ProcessorID  = 2,
            .SpacecraftID = 0x42,
            .NetNum       = 0,
            .ProtocolName = "UDP",
            .Filters      = {"Remap"},
            .Address      = "127.0.0.1:3235",
            .TaskFlags    = SBN_TASK_POLL
        },
        { /* [2] */
            .ProcessorID  = 3,
            .SpacecraftID = 0x42,
            .NetNum       = 0,
            .ProtocolName = "UDP",
            .Filters      = {"Remap"},
            .Address      = "127.0.0.1:3236",
            .TaskFlags    = SBN_TASK_POLL
        },
    },
    .PeerCnt = 3
};

CFE_TBL_FILEDEF(SBN_ConfTbl, SBN.ConfTbl, SBN Configuration Table, sbn_conf_tbl.tbl)
