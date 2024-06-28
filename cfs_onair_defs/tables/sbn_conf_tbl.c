#include "sbn_tbl.h"
#include "cfe_tbl_filedef.h"

SBN_ConfTbl_t SBN_ConfTbl = {.ProtocolModules = {{/* [0] */
                                                  .Name        = "TCP",
                                                  .LibFileName = "/cf/sbn_tcp.so",
                                                  .LibSymbol   = "SBN_TCP_Ops",
                                                  .BaseEID     = 0x0100}},
                             .ProtocolCnt     = 1,
                             //.FilterModules   = {{/* [0] */
                             //                   .Name        = "Remap",
                             //                   .LibFileName = "/cf/sbn_f_remap.so",
                             //                   .LibSymbol   = "SBN_F_Remap",
                             //                  .BaseEID     = 0x1000}},
                             .FilterCnt       = 0,

                             .Peers =
                                 {
                                     {/* [0] */
                                      .ProcessorID  = 1,
                                      .SpacecraftID = 0x42,
                                      .NetNum       = 0,
                                      .ProtocolName = "TCP",
                                      .Filters      = {"Remap"},
                                      .Address      = "127.0.0.1:2234",
                                      .TaskFlags    = SBN_TASK_POLL},
                                     {/* [1] */
                                      .ProcessorID  = 2,
                                      .SpacecraftID = 0x42,
                                      .NetNum       = 0,
                                      .ProtocolName = "TCP",
                                      .Filters      = {"Remap"},
                                      .Address      = "127.0.0.1:2235",
                                      .TaskFlags    = SBN_TASK_POLL}
                                 },
                             .PeerCnt = 2};

CFE_TBL_FILEDEF(SBN_ConfTbl, SBN.SBN_ConfTbl, SBN Configuration Table, sbn_conf_tbl.tbl)
