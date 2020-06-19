/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
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
** File: cfe_perfids.h
**
** Purpose: This file contains the cFE performance IDs
**
** Design Notes:
**   Each performance id is used to identify something that needs to be
**   measured.  Performance ids are limited to the range of 0 to
**   CFE_PLATFORM_ES_PERF_MAX_IDS - 1.  Any performance ids outside of this range
**   will be ignored and will be flagged as an error.  Note that
**   performance ids 0-31 are reserved for the cFE Core.
**
** References:
**
*/

#ifndef _cfe_perfids_
#define _cfe_perfids_


#define CFE_MISSION_ES_PERF_EXIT_BIT            31      /**< \brief bit (31) is reserved by the perf utilities */

/** \name cFE Performance Monitor IDs (Reserved IDs 0-31) */
/** \{ */
#define CFE_MISSION_ES_MAIN_PERF_ID             1    /**< \brief Performance ID for Executive Services Task */
#define CFE_MISSION_EVS_MAIN_PERF_ID            2    /**< \brief Performance ID for Events Services Task */
#define CFE_MISSION_TBL_MAIN_PERF_ID            3    /**< \brief Performance ID for Table Services Task */
#define CFE_MISSION_SB_MAIN_PERF_ID             4    /**< \brief Performance ID for Software Bus Services Task */
#define CFE_MISSION_SB_MSG_LIM_PERF_ID          5    /**< \brief Performance ID for Software Bus Msg Limit Errors */
#define CFE_MISSION_SB_PIPE_OFLOW_PERF_ID       27   /**< \brief Performance ID for Software Bus Pipe Overflow Errors */


#define CFE_MISSION_TIME_MAIN_PERF_ID           6    /**< \brief Performance ID for Time Services Task */
#define CFE_MISSION_TIME_TONE1HZISR_PERF_ID     7    /**< \brief Performance ID for 1 Hz Tone ISR */
#define CFE_MISSION_TIME_LOCAL1HZISR_PERF_ID    8    /**< \brief Performance ID for 1 Hz Local ISR */

#define CFE_MISSION_TIME_SENDMET_PERF_ID        9    /**< \brief Performance ID for Time ToneSendMET */
#define CFE_MISSION_TIME_LOCAL1HZTASK_PERF_ID   10   /**< \brief Performance ID for 1 Hz Local Task */
#define CFE_MISSION_TIME_TONE1HZTASK_PERF_ID    11   /**< \brief Performance ID for 1 Hz Tone Task */


/** \} */


#endif   /* _cfe_perfids_ */
