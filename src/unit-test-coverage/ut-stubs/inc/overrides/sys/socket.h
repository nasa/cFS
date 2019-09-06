/* OSAL coverage stub replacement for sys/socket.h */
#ifndef _OSAL_STUB_SYS_SOCKET_H_
#define _OSAL_STUB_SYS_SOCKET_H_

/* ----------------------------------------- */
/* constants normally defined in sys/socket.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/socket.h */
/* ----------------------------------------- */
typedef unsigned long   OCS_size_t;
typedef unsigned long   OCS_socklen_t;
typedef long            OCS_ssize_t;

struct OCS_sockaddr
{
    char sa[4];
};

/* ----------------------------------------- */
/* prototypes normally declared in sys/socket.h */
/* ----------------------------------------- */

extern int OCS_accept (int fd, struct OCS_sockaddr * addr, OCS_socklen_t * addr_len);
extern int OCS_bind (int fd, const struct OCS_sockaddr * addr, OCS_socklen_t len);
extern int OCS_connect (int fd, const struct OCS_sockaddr * addr, OCS_socklen_t len);
extern int OCS_getsockopt (int fd, int level, int optname, void * optval, OCS_socklen_t * optlen);
extern int OCS_listen (int fd, int n);
extern OCS_ssize_t OCS_recvfrom (int fd, void * buf, OCS_size_t n, int flags, struct OCS_sockaddr * addr, OCS_socklen_t * addr_len);
extern OCS_ssize_t OCS_sendto (int fd, const void * buf, OCS_size_t n, int flags, const struct OCS_sockaddr * addr, OCS_socklen_t addr_len);
extern int OCS_setsockopt (int fd, int level, int optname, const void * optval, OCS_socklen_t optlen);
extern int OCS_socket (int domain, int type, int protocol);


#endif /* _OSAL_STUB_SYS_SOCKET_H_ */

