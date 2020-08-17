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

/**
 * \file cfe_psp_exceptionstorage_api.h
 *
 * Provides a generic storage buffer ring for exceptions
 */

#ifndef CFE_PSP_EXCEPTIONSTORAGE_API_H_
#define CFE_PSP_EXCEPTIONSTORAGE_API_H_

#include <cfe_psp.h>

/*
 * Abstract types for exception storage.
 * These are made concrete depending on PSP-specific config information.
 */
struct CFE_PSP_Exception_LogData;



/* -------------------------------------------------------------
 * Functions implemented in shared layer, invoked by impl layer.
 * ------------------------------------------------------------- */

/**
 * \brief Get the next buffer for exception buffer corresponding to sequence
 *
 * This function obtains a storage buffer corresponding to the given sequence number.
 * The pointer to storage memory is directly returned. It is not cleared or modified,
 * and no checks are performed to determine if the sequence number is valid.
 *
 * \param   seq     Sequence number
 * \returns pointer to buffer.
 */
extern struct CFE_PSP_Exception_LogData* CFE_PSP_Exception_GetBuffer(uint32 seq);

/**
 * \brief Get the next buffer for exception context storage
 *
 * This function is invoked by the low level exception handler (typically an ISR/signal)
 * to obtain a buffer for context capture.  The buffer is cleared (memset zero) before
 * returning to the caller.
 *
 * \returns pointer to buffer, or NULL if storage is full.
 */
extern struct CFE_PSP_Exception_LogData* CFE_PSP_Exception_GetNextContextBuffer(void);

/**
 * \brief Finish storage of exception data
 *
 * This function is invoked by the low level exception handler (typically an ISR/signal)
 * once the exception context capture is complete.  This should be invoked after a successful
 * call to CFE_PSP_Exception_GetNextContextBuffer() to commit the information to the log.
 */
extern void CFE_PSP_Exception_WriteComplete(void);

/**
 * \brief Reset the exception storage buffer
 *
 * Marks any pending exceptions as "read".  This resets the state of exception processing.
 */
extern void CFE_PSP_Exception_Reset(void);

/* -------------------------------------------------------------
 * Functions implemented in impl layer, invoked by shared layer.
 * ------------------------------------------------------------- */

/**
 * \brief Translate the exception context data into a user-friendly "reason" string
 *
 * This is called in an application context to determine the cause of the exception.
 *
 * \param[in] Buffer Context data previously stored by ISR/signal handler
 * \param[out] ReasonBuf Buffer to store string
 * \param[in] ReasonSize Size of string buffer
 *
 * \returns CFE_PSP_SUCCESS on success
 */
extern int32 CFE_PSP_ExceptionGetSummary_Impl(const struct CFE_PSP_Exception_LogData* Buffer, char *ReasonBuf, uint32 ReasonSize);

#endif /* CFE_PSP_EXCEPTIONSTORAGE_API_H_ */
