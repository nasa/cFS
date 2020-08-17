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

/**
 * \file     os-impl-files.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_FILES_H_
#define INCLUDE_OS_IMPL_FILES_H_

#include "os-impl-io.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>

/*
 * VxWorks does not have UID/GID so these are defined as 0.
 */
#define OS_IMPL_SELF_EUID 0
#define OS_IMPL_SELF_EGID 0


/*
 * Do not set any additional flags for regular files
 */
#define OS_IMPL_REGULAR_FILE_FLAGS       0


#endif  /* INCLUDE_OS_IMPL_FILES_H_ */

