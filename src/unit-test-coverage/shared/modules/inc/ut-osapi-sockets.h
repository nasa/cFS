/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSAPI_SOCKETS_H_
#define _OSAL_UT_OSAPI_SOCKETS_H_

#include <common_types.h>
#include <os-impl.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/**
 * Purges all state tables and resets back to initial conditions
 * Helps avoid cross-test dependencies
 */
void Osapi_Internal_ResetState(void);

/**
 * Invoke the OS_CreateSocketName() static helper function
 */
void Osapi_Call_CreateSocketName_Static(OS_stream_internal_record_t *sock,
        const OS_SockAddr_t *Addr, const char *parent_name);

#endif /* _OSAL_UT_OSAPI_SOCKETS_H_ */

