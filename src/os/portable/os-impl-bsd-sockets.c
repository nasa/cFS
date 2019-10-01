/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   os-impl-bsd-sockets.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the network functionality for for
 *      systems which implement the BSD-style socket API.
 *
 * NOTE: This is a "template" file and not a directly usable source file.
 *       It must be adapted/instantiated from within the OS-specific
 *       implementation on platforms that wish to use this template.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * NOTE - This is a code fragment and all the include files should have
 * been handled by the includer (because those vary from OS to OS).
 */

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

typedef union
{
   char data[OS_SOCKADDR_MAX_LEN];
   struct sockaddr sockaddr;
   struct sockaddr_in sockaddr_in;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   struct sockaddr_in6 sockaddr_in6;
#endif
} OS_SockAddr_Accessor_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/


/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/


/****************************************************************************************
                                    Network API
 ***************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl       (char *host_name, uint32 name_len)
{
    int32 return_code;

    if ( gethostname(host_name, name_len) < 0 )
    {
        return_code = OS_ERROR;
    }
    else
    {
        /*
         * posix does not say that the name is always
         * null terminated, so its worthwhile to ensure it
         */
        host_name[name_len - 1] = 0;
        return_code = OS_SUCCESS;
    }

    return(return_code);
} /* end OS_NetworkGetHostName_Impl */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketOpen_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketOpen_Impl(uint32 sock_id)
{
   int os_domain;
   int os_type;
   int os_proto;
   int os_flags;

   os_proto = 0;

   switch(OS_stream_table[sock_id].socket_type)
   {
   case OS_SocketType_DATAGRAM:
      os_type = SOCK_DGRAM;
      break;
   case OS_SocketType_STREAM:
      os_type = SOCK_STREAM;
      break;

   default:
      return OS_ERR_NOT_IMPLEMENTED;
   }

   switch(OS_stream_table[sock_id].socket_domain)
   {
   case OS_SocketDomain_INET:
      os_domain = AF_INET;
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case OS_SocketDomain_INET6:
      os_domain = AF_INET6;
      break;
#endif
   default:
      return OS_ERR_NOT_IMPLEMENTED;
   }

   switch(OS_stream_table[sock_id].socket_domain)
   {
   case OS_SocketDomain_INET:
   case OS_SocketDomain_INET6:
      switch(OS_stream_table[sock_id].socket_type)
      {
      case OS_SocketType_DATAGRAM:
         os_proto = IPPROTO_UDP;
         break;
      case OS_SocketType_STREAM:
         os_proto = IPPROTO_TCP;
         break;
      default:
         break;
      }
      break;
   default:
      break;
   }

   OS_impl_filehandle_table[sock_id].fd = socket(os_domain, os_type, os_proto);
   if (OS_impl_filehandle_table[sock_id].fd < 0)
   {
      return OS_ERROR;
   }

   /*
    * Setting the REUSEADDR flag helps during debugging when there might be frequent
    * code restarts.  However if setting the option fails then it is not worth bailing out over.
    */
   os_flags = 1;
   setsockopt(OS_impl_filehandle_table[sock_id].fd,SOL_SOCKET,SO_REUSEADDR,&os_flags,sizeof(os_flags));

   /*
    * Set the standard options on the filehandle by default --
    * this may set it to non-blocking mode if the implementation supports it.
    * any blocking would be done explicitly via the select() wrappers
    */
   os_flags = fcntl(OS_impl_filehandle_table[sock_id].fd, F_GETFL);
   os_flags |= OS_IMPL_SOCKET_FLAGS;
   fcntl(OS_impl_filehandle_table[sock_id].fd, F_SETFL, os_flags);

   OS_impl_filehandle_table[sock_id].selectable =
           ((os_flags & O_NONBLOCK) != 0);

   return OS_SUCCESS;
} /* end OS_SocketOpen_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketBind_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketBind_Impl(uint32 sock_id, const OS_SockAddr_t *Addr)
{
   int os_result;
   socklen_t addrlen;
   const struct sockaddr *sa;

   sa = (const struct sockaddr *)Addr->AddrData;

   switch(sa->sa_family)
   {
   case AF_INET:
      addrlen = sizeof(struct sockaddr_in);
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case AF_INET6:
      addrlen = sizeof(struct sockaddr_in6);
      break;
#endif
   default:
      addrlen = 0;
      break;
   }

   if (addrlen == 0 || addrlen > OS_SOCKADDR_MAX_LEN)
   {
      return OS_ERR_BAD_ADDRESS;
   }

   os_result = bind(OS_impl_filehandle_table[sock_id].fd, sa, addrlen);
   if (os_result < 0)
   {
      OS_DEBUG("bind: %s\n",strerror(errno));
      return OS_ERROR;
   }

   /* Start listening on the socket (implied for stream sockets) */
   if (OS_stream_table[sock_id].socket_type == OS_SocketType_STREAM)
   {
      os_result = listen(OS_impl_filehandle_table[sock_id].fd, 10);
      if (os_result < 0)
      {
         OS_DEBUG("listen: %s\n",strerror(errno));
         return OS_ERROR;
      }
   }
   return OS_SUCCESS;
} /* end OS_SocketBind_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketConnect_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketConnect_Impl(uint32 sock_id, const OS_SockAddr_t *Addr, int32 timeout)
{
   int32 return_code;
   int os_status;
   int sockopt;
   socklen_t slen;
   uint32 operation;
   const struct sockaddr *sa;

   sa = (const struct sockaddr *)Addr->AddrData;
   switch(sa->sa_family)
   {
   case AF_INET:
      slen = sizeof(struct sockaddr_in);
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case AF_INET6:
      slen = sizeof(struct sockaddr_in6);
      break;
#endif
   default:
      slen = 0;
      break;
   }

   if (slen != Addr->ActualLength)
   {
      return_code = OS_ERR_BAD_ADDRESS;
   }
   else
   {
       return_code = OS_SUCCESS;
       os_status = connect(OS_impl_filehandle_table[sock_id].fd, sa, slen);
       if (os_status < 0)
       {
           if (errno != EINPROGRESS)
           {
               return_code = OS_ERROR;
           }
           else
           {
               operation = OS_STREAM_STATE_WRITABLE;
               if (OS_impl_filehandle_table[sock_id].selectable)
               {
                   return_code = OS_SelectSingle_Impl(sock_id, &operation, timeout);
               }
               if (return_code == OS_SUCCESS)
               {
                   if ((operation & OS_STREAM_STATE_WRITABLE) == 0)
                   {
                       return_code = OS_ERROR_TIMEOUT;
                   }
                   else
                   {
                       sockopt = 0;
                       slen = sizeof(sockopt);
                       os_status = getsockopt(OS_impl_filehandle_table[sock_id].fd, SOL_SOCKET, SO_ERROR, &sockopt, &slen);
                       if (os_status < 0 || sockopt != 0)
                       {
                           return_code = OS_ERROR;
                       }
                   }
               }
           }
       }
   }
   return return_code;
} /* end OS_SocketConnect_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketAccept_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAccept_Impl(uint32 sock_id, uint32 connsock_id, OS_SockAddr_t *Addr, int32 timeout)
{
   int32 return_code;
   uint32 operation;
   socklen_t addrlen;
   int os_flags;

   operation = OS_STREAM_STATE_READABLE;
   if (OS_impl_filehandle_table[sock_id].selectable)
   {
       return_code = OS_SelectSingle_Impl(sock_id, &operation, timeout);
   }
   else
   {
       return_code = OS_SUCCESS;
   }

   if (return_code == OS_SUCCESS)
   {
      if ((operation & OS_STREAM_STATE_READABLE) == 0)
      {
         return_code = OS_ERROR_TIMEOUT;
      }
      else
      {
         addrlen = Addr->ActualLength;
         OS_impl_filehandle_table[connsock_id].fd = accept(OS_impl_filehandle_table[sock_id].fd, (struct sockaddr *)Addr->AddrData, &addrlen);
         if (OS_impl_filehandle_table[connsock_id].fd < 0)
         {
            return_code = OS_ERROR;
         }
         else
         {
             Addr->ActualLength = addrlen;

             /*
              * Set the standard options on the filehandle by default --
              * this may set it to non-blocking mode if the implementation supports it.
              * any blocking would be done explicitly via the select() wrappers
              */
             os_flags = fcntl(OS_impl_filehandle_table[connsock_id].fd, F_GETFL);
             os_flags |= OS_IMPL_SOCKET_FLAGS;
             fcntl(OS_impl_filehandle_table[connsock_id].fd, F_SETFL, os_flags);

             OS_impl_filehandle_table[connsock_id].selectable =
                     ((os_flags & O_NONBLOCK) != 0);
         }
      }
   }

   return return_code;
} /* end OS_SocketAccept_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketRecvFrom_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketRecvFrom_Impl(uint32 sock_id, void *buffer, uint32 buflen, OS_SockAddr_t *RemoteAddr, int32 timeout)
{
   int32 return_code;
   int os_result;
   int waitflags;
   uint32 operation;
   struct sockaddr *sa;
   socklen_t addrlen;

   if (RemoteAddr == NULL)
   {
      sa = NULL;
      addrlen = 0;
   }
   else
   {
      addrlen = OS_SOCKADDR_MAX_LEN;
      sa = (struct sockaddr *)RemoteAddr->AddrData;
   }

   operation = OS_STREAM_STATE_READABLE;
   /*
    * If "O_NONBLOCK" flag is set then use select()
    * Note this is the only way to get a correct timeout
    */
   if (OS_impl_filehandle_table[sock_id].selectable)
   {
       waitflags = MSG_DONTWAIT;
       return_code = OS_SelectSingle_Impl(sock_id, &operation, timeout);
   }
   else
   {
       if (timeout == 0)
       {
           waitflags = MSG_DONTWAIT;
       }
       else
       {
           /* note timeout will not be honored if >0 */
           waitflags = 0;
       }
       return_code = OS_SUCCESS;
   }

   if (return_code == OS_SUCCESS)
   {
      if ((operation & OS_STREAM_STATE_READABLE) == 0)
      {
         return_code = OS_ERROR_TIMEOUT;
      }
      else
      {
         os_result = recvfrom(OS_impl_filehandle_table[sock_id].fd, buffer, buflen, waitflags, sa, &addrlen);
         if (os_result < 0)
         {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
               return_code = OS_QUEUE_EMPTY;
            }
            else
            {
               OS_DEBUG("recvfrom: %s\n",strerror(errno));
               return_code = OS_ERROR;
            }
         }
         else
         {
            return_code = os_result;

            if (RemoteAddr != NULL)
            {
               RemoteAddr->ActualLength = addrlen;
            }
         }
      }
   }


   return return_code;
} /* end OS_SocketRecvFrom_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketSendTo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketSendTo_Impl(uint32 sock_id, const void *buffer, uint32 buflen, const OS_SockAddr_t *RemoteAddr)
{
   int os_result;
   socklen_t addrlen;
   const struct sockaddr *sa;

   sa = (const struct sockaddr *)RemoteAddr->AddrData;
   switch(sa->sa_family)
   {
   case AF_INET:
      addrlen = sizeof(struct sockaddr_in);
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case AF_INET6:
      addrlen = sizeof(struct sockaddr_in6);
      break;
#endif
   default:
      addrlen = 0;
      break;
   }

   if (addrlen != RemoteAddr->ActualLength)
   {
      return OS_ERR_BAD_ADDRESS;
   }

   os_result = sendto(OS_impl_filehandle_table[sock_id].fd, buffer, buflen, MSG_DONTWAIT, sa, addrlen);
   if (os_result < 0)
   {
      OS_DEBUG("sendto: %s\n",strerror(errno));
      return OS_ERROR;
   }

   return os_result;
} /* end OS_SocketSendTo_Impl */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketGetInfo_Impl (uint32 sock_id, OS_socket_prop_t *sock_prop)
{
   return OS_SUCCESS;
} /* end OS_SocketGetInfo_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrInit_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrInit_Impl(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
   sa_family_t sa_family;
   socklen_t addrlen;
   OS_SockAddr_Accessor_t *Accessor;

   memset(Addr, 0, sizeof(OS_SockAddr_t));
   Accessor = (OS_SockAddr_Accessor_t *)Addr->AddrData;

   switch(Domain)
   {
   case OS_SocketDomain_INET:
      sa_family = AF_INET;
      addrlen = sizeof(struct sockaddr_in);
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case OS_SocketDomain_INET6:
      sa_family = AF_INET6;
      addrlen = sizeof(struct sockaddr_in6);
      break;
#endif
   default:
      addrlen = 0;
      break;
   }

   if (addrlen == 0 || addrlen > OS_SOCKADDR_MAX_LEN)
   {
      return OS_ERR_NOT_IMPLEMENTED;
   }

   Addr->ActualLength = addrlen;
   Accessor->sockaddr.sa_family = sa_family;

   return OS_SUCCESS;
} /* end OS_SocketAddrInit_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrToString_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrToString_Impl(char *buffer, uint32 buflen, const OS_SockAddr_t *Addr)
{
   const void *addrbuffer;
   const OS_SockAddr_Accessor_t *Accessor;

   Accessor = (const OS_SockAddr_Accessor_t *)Addr->AddrData;

   switch(Accessor->sockaddr.sa_family)
   {
   case AF_INET:
      addrbuffer = &Accessor->sockaddr_in.sin_addr;
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case AF_INET6:
      addrbuffer = &Accessor->sockaddr_in6.sin6_addr;
      break;
#endif
   default:
      return OS_ERR_BAD_ADDRESS;
      break;
   }

   if (inet_ntop(Accessor->sockaddr.sa_family, addrbuffer, buffer, buflen) == NULL)
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_SocketAddrToString_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrFromString_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrFromString_Impl(OS_SockAddr_t *Addr, const char *string)
{
   void *addrbuffer;
   OS_SockAddr_Accessor_t *Accessor;

   Accessor = (OS_SockAddr_Accessor_t *)Addr->AddrData;

   switch(Accessor->sockaddr.sa_family)
   {
   case AF_INET:
      addrbuffer = &Accessor->sockaddr_in.sin_addr;
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case AF_INET6:
      addrbuffer = &Accessor->sockaddr_in6.sin6_addr;
      break;
#endif
   default:
      return OS_ERR_BAD_ADDRESS;
      break;
   }

   if (inet_pton(Accessor->sockaddr.sa_family, string, addrbuffer) < 0)
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_SocketAddrFromString_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrGetPort_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrGetPort_Impl(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
   in_port_t sa_port;
   const OS_SockAddr_Accessor_t *Accessor;

   Accessor = (const OS_SockAddr_Accessor_t *)Addr->AddrData;

   switch(Accessor->sockaddr.sa_family)
   {
   case AF_INET:
      sa_port = Accessor->sockaddr_in.sin_port;
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case AF_INET6:
      sa_port = Accessor->sockaddr_in6.sin6_port;
      break;
#endif
   default:
      return OS_ERR_BAD_ADDRESS;
      break;
   }

   *PortNum = ntohs(sa_port);

   return OS_SUCCESS;
} /* end OS_SocketAddrGetPort_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrSetPort_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrSetPort_Impl(OS_SockAddr_t *Addr, uint16 PortNum)
{
   in_port_t sa_port;
   OS_SockAddr_Accessor_t *Accessor;

   sa_port = htons(PortNum);
   Accessor = (OS_SockAddr_Accessor_t *)Addr->AddrData;

   switch(Accessor->sockaddr.sa_family)
   {
   case AF_INET:
      Accessor->sockaddr_in.sin_port = sa_port;
      break;
#ifdef OS_NETWORK_SUPPORTS_IPV6
   case AF_INET6:
      Accessor->sockaddr_in6.sin6_port = sa_port;
      break;
#endif
   default:
      return OS_ERR_BAD_ADDRESS;
   }

   return OS_SUCCESS;
} /* end OS_SocketAddrSetPort_Impl */



