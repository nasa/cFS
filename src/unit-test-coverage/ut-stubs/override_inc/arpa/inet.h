/* OSAL coverage stub replacement for arpa/inet.h */
#ifndef _OSAL_OVERRIDE_ARPA_INET_H_
#define _OSAL_OVERRIDE_ARPA_INET_H_

#include <OCS_arpa_inet.h>

/* ----------------------------------------- */
/* mappings for declarations in arpa/inet.h */
/* ----------------------------------------- */
#define inet_ntop    OCS_inet_ntop
#define inet_pton    OCS_inet_pton


#endif /* _OSAL_OVERRIDE_ARPA_INET_H_ */
