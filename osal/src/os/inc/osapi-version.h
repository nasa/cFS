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

 /*! @file osapi-version.h
  * @brief Purpose:
  *  @details Provide version identifiers for cFS' Operating System Abstraction Layer
  *  See @ref cfsversions for version and build number and description
  * 
  */
#ifndef _osapi_version_h_
#define _osapi_version_h_

/*
 * Development Build Macro Definitions 
 */
#define OS_BUILD_NUMBER 247 
#define OS_BUILD_BASELINE "v5.0.0+dev"

/*
 * Version Macro Definitions
 */
#define OS_MAJOR_VERSION 5 /*!< @brief ONLY APPLY for OFFICIAL releases. Major version number. */
#define OS_MINOR_VERSION 0 /*!< @brief ONLY APPLY for OFFICIAL releases. Minor version number. */
#define OS_REVISION      0 /*!< @brief ONLY APPLY for OFFICIAL releases. Revision number. */
#define OS_MISSION_REV   0 /*!< @brief ONLY USED by MISSION Implementations. Mission revision */

/*
 * Tools to construct version string
 */ 
#define OS_STR_HELPER(x) #x /*!< @brief Helper function to concatenate strings from integer */
#define OS_STR(x)        OS_STR_HELPER(x) /*!< @brief Helper function to concatenate strings from integer */

/*! @brief Development Build Version Number. 
 * @details Baseline git tag + Number of commits since baseline. @n
 * See @ref cfsversions for format differences between development and release versions.
 */
#define OS_VERSION OS_BUILD_BASELINE OS_STR(OS_BUILD_NUMBER) 

/*! @brief Development Build Version String.
 * @details Reports the current development build's baseline, number, and name. Also includes a note about the latest official version. @n
 * See @ref cfsversions for format differences between development and release versions. 
*/     
#define OS_VERSION_STRING                                                          \
    " OSAL Development Build\n"                                                    \
    " " OS_VERSION " (Codename: Bootes)\n"  /* Codename for current development */ \
    " Latest Official Version: osal v5.0.0" /* For full support please use official release version */

/*! @brief Combines the revision components into a single value 
 * @details Applications can check against this number @n
 * e.g. "#if OSAL_API_VERSION >= 40100" would check if some feature added in
OSAL 4.1 is present.
 */
#define OSAL_API_VERSION ((OS_MAJOR_VERSION * 10000) + (OS_MINOR_VERSION * 100) + OS_REVISION)

#endif /* _osapi_version_h_ */

/************************/
/*  End of File Comment */
/************************/
