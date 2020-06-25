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
 * File: osapi-version.h
 *
 * Purpose:
 *  The OSAL version numbers
 */

#ifndef _osapi_version_h_
#define _osapi_version_h_

#define OS_MAJOR_VERSION 5  /**< @brief Major version number */
#define OS_MINOR_VERSION 0  /**< @brief Minor version number */
#define OS_REVISION      21  /**< @brief Revision number */
#define OS_MISSION_REV   0  /**< @brief Mission revision */

/**
 * Combine the revision components into a single value that application code can check against
 * e.g. "#if OSAL_API_VERSION >= 40100" would check if some feature added in OSAL 4.1 is present.
 */
#define OSAL_API_VERSION ((OS_MAJOR_VERSION * 10000) + (OS_MINOR_VERSION * 100) + OS_REVISION)

#endif /* _osapi_version_h_ */

/************************/
/*  End of File Comment */
/************************/
