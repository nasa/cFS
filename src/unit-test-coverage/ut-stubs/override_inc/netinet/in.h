/* OSAL coverage stub replacement for netinet/in.h */
#ifndef _OSAL_OVERRIDE_NETINET_IN_H_
#define _OSAL_OVERRIDE_NETINET_IN_H_

#include <OCS_netinet_in.h>

/* ----------------------------------------- */
/* mappings for declarations in netinet/in.h */
/* ----------------------------------------- */

#define htons    OCS_htons
#define ntohs    OCS_ntohs
#define htonl    OCS_htonl
#define ntohl    OCS_ntohl


#endif /* _OSAL_OVERRIDE_NETINET_IN_H_ */
