/*
**  $Id: psp_version.h 1.2.2.3 2014/10/01 15:41:26GMT-05:00 sstrege Exp  $
**
**
**      GSC-18128-1, "Core Flight Executive Version 6.6"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
**
**
**  Purpose:
**  Provide version identifiers for the cFE Platform Support Packages (PSP).
**
*/

#ifndef _psp_version_
#define _psp_version_


/*
** Macro Definitions
*/
#define CFE_PSP_IMPL_MAJOR_VERSION          1
#define CFE_PSP_IMPL_MINOR_VERSION          3
#define CFE_PSP_IMPL_REVISION               0
#define CFE_PSP_IMPL_MISSION_REV            0

#ifndef _ENHANCED_BUILD_

#define CFE_PSP_MAJOR_VERSION          CFE_PSP_IMPL_MAJOR_VERSION
#define CFE_PSP_MINOR_VERSION          CFE_PSP_IMPL_MINOR_VERSION
#define CFE_PSP_REVISION               CFE_PSP_IMPL_REVISION
#define CFE_PSP_MISSION_REV            CFE_PSP_IMPL_MISSION_REV

/* For backwards compatibility */
#define CFE_PSP_SUBMINOR_VERSION       CFE_PSP_IMPL_REVISION 

#endif

#endif  /* _psp_version_ */

