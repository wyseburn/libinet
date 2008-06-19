/**
 *  Version:     @(#)libinet/byte_order.hxx    0.0.3 28/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     Byte order convert implement module 
 *
 *  This library is free software; you can redistribute it and/or modify it under 
 *  the terms of the GNU Lesser General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) any 
 *  later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 *  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along 
 *  with this library; if not, write to the Free Software Foundation, Inc., 
 *  59 Temple  Place, Suite 330, Boston, MA 02111-1307  USA
 */

#ifndef __LIBINET_BYTE_ORDER_H__
#define __LIBINET_BYTE_ORDER_H__

#if defined (_WIN32)
# include <winsock2.h>
# elif defined (__linux__) || defined (__FreeBSD__)
# include <arpa/inet.h>
#endif

#include "compat.hxx"

namespace inet
{
    template <typename Type> 
    class byte_convert
    {
    public:
        inline static Type hton(Type val) { return val; }
        inline static Type ntoh(Type val) { return val; }
    };

    template <>
    inline inet_int16 byte_convert<inet_int16>::hton(inet_int16 hs)
    {
        return (inet_int16)htons((inet_uint16)hs);
    }

    template <>
    inline inet_int16 byte_convert<inet_int16>::ntoh(inet_int16 ns)
    {
        return (inet_int16)ntohs((inet_uint16)ns);
    }

    template <>
    inline inet_uint16 byte_convert<inet_uint16>::hton(inet_uint16 hs)
    {
        return (inet_uint16)htons(hs);
    }

    template <>
    inline inet_uint16 byte_convert<inet_uint16>::ntoh(inet_uint16 ns)
    {
        return (inet_uint16)ntohs(ns);
    }

    template <>
    inline inet_int32 byte_convert<inet_int32>::hton(inet_int32 hl)
    {
        return (inet_int32)htonl((inet_uint32)hl);
    }

    template <>
    inline inet_int32 byte_convert<inet_int32>::ntoh(inet_int32 nl)
    {
        return (inet_int32)ntohl((inet_uint32)nl);
    }

    template <>
    inline inet_uint32 byte_convert<inet_uint32>::hton(inet_uint32 hl)
    {
        return (inet_uint32)htonl(hl);
    }

    template <>
    inline inet_uint32 byte_convert<inet_uint32>::ntoh(inet_uint32 nl)
    {
        return (inet_uint32)ntohl(nl);
    }
} // namesapce
                           
#endif // #ifndef __LIBINET_BYTE_ORDER_H__
