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
 *  File: psp_version.h
 *
 *  Purpose:
 *  Provide version identifiers for the cFE Platform Support Packages (PSP).
 *  See cfe documentation for version and build number descriptions
 *
 */

#ifndef _psp_version_
#define _psp_version_

/* Development Build Macro Definitions */
#define CFE_PSP_IMPL_BUILD_NUMBER 68
#define CFE_PSP_IMPL_BUILD_BASELINE "v1.4.0+dev"

/*
 * Macro Definitions
 * ONLY APPLY for OFFICIAL releases
 */
#define CFE_PSP_IMPL_MAJOR_VERSION 1
#define CFE_PSP_IMPL_MINOR_VERSION 4
#define CFE_PSP_IMPL_REVISION      0
#define CFE_PSP_IMPL_MISSION_REV   0

#define CFE_PSP_IMPL_STR_HELPER(x) #x
#define CFE_PSP_IMPL_STR(x)        CFE_PSP_IMPL_STR_HELPER(x)

/* Baseling git tag + Number of commits since baseline */
#define CFE_PSP_IMPL_VERSION CFE_PSP_IMPL_BUILD_BASELINE CFE_PSP_IMPL_STR(CFE_PSP_IMPL_BUILD_NUMBER)

#define CFE_PSP_IMPL_VERSION_STRING                                                                      \
    " PSP Development Build\n " CFE_PSP_IMPL_VERSION " (Codename: Bootes)" /* Codename for current development */ \
    "\n Last Official Release: psp v1.4.0"                        /* For full support please use this version */

/* Use the following templates for Official Releases ONLY */

/* Official Release format for CFE_PSP_IMPLVERSION */
  /*
  #define CFE_PSP_IMPL_VERSION "v"            \
       CFE_PSP_IMPL_STR(CFE_PSP_IMPL_MAJOR_VERSION) "." \
       CFE_PSP_IMPL_STR(CFE_PSP_IMPL_MINOR_VERSION) "." \
       CFE_PSP_IMPL_STR(CFE_PSP_IMPL_REVISION) "."      \
       CFE_PSP_IMPL_STR(CFE_PSP_IMPL_MISSION_REV)
  */

  /* Official Release CFE_PSP_IMPLVERSION_STRING Format */
  /*
  #define CFE_PSP_IMPL_VERSION_STRING " PSP " CFE_PSP_IMPL_VERSION
  */
  
/* END TEMPLATES */

#endif  /* _psp_version_ */
