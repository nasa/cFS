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

#ifndef CPU2_CFE_CORE_BASE_MSGID_VALUES_H
#define CPU2_CFE_CORE_BASE_MSGID_VALUES_H

#include "global_core_api_base_msgid_values.h"

#define CFE_PLATFORM_BASE_MIDVAL(x) SAMPLE_CPU2_##x##_MID_BASE
#define CFE_GLOBAL_BASE_MIDVAL(x)   SAMPLE_GLOBAL_##x##_MID_BASE

#endif /* CFE_CORE_BASE_MSGIDS_H */
