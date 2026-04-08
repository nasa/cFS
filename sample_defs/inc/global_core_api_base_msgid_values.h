/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
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
 *
 * This header file contains the platform-specific base msg ID values and
 * logic to convert a topic ID to a message ID value.
 *
 */

#ifndef GLOBAL_CFE_CORE_BASE_MSGID_VALUES_H
#define GLOBAL_CFE_CORE_BASE_MSGID_VALUES_H

#define SAMPLE_CPU1_CMD_MID_BASE 0x1800
#define SAMPLE_CPU1_TLM_MID_BASE 0x0800

#define SAMPLE_CPU2_CMD_MID_BASE 0x1900
#define SAMPLE_CPU2_TLM_MID_BASE 0x0900

#define SAMPLE_GLOBAL_CMD_MID_BASE 0x1860
#define SAMPLE_GLOBAL_TLM_MID_BASE 0x0860

#endif /* CFE_CORE_BASE_MSGIDS_H */
