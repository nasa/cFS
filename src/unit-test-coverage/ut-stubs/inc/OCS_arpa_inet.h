/* OSAL coverage stub replacement for arpa/inet.h */
#ifndef _OSAL_STUB_ARPA_INET_H_
#define _OSAL_STUB_ARPA_INET_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in arpa/inet.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in arpa/inet.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in arpa/inet.h */
/* ----------------------------------------- */

extern const char *OCS_inet_ntop (int af, const void * cp, char * buf, size_t len);
extern int OCS_inet_pton (int af, const char * cp, void * buf);


#endif /* _OSAL_STUB_ARPA_INET_H_ */

