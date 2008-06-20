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
    class endian_reverse
    {
    public:
        inline static Type convert(Type val) { return val; }
    };

    template <>
    inline inet_int16 endian_reverse<inet_int16>::convert(inet_int16 input)
    {
        return (inet_int16) ((input << 8) & 0xff00) | ((input >> 8) & 0x00ff);
    }

    template <>
    inline inet_uint16 endian_reverse<inet_uint16>::convert(inet_uint16 input)
    {
        return (inet_uint16) ((input << 8) & 0xff00) | ((input >> 8) & 0x00ff);
    }

    template <>
    inline inet_int32 endian_reverse<inet_int32>::convert(inet_int32 input)
    {
        return (inet_int32) ((input << 24) & 0xff000000) | 
                            ((input << 8 ) & 0x00ff0000) | 
                            ((input >> 8 ) & 0x0000ff00) | 
                            ((input >> 24) & 0x000000ff);
    }

    template <>
    inline inet_uint32 endian_reverse<inet_uint32>::convert(inet_uint32 input)
    {
        return (inet_uint32) ((input << 24) & 0xff000000) | 
                             ((input << 8 ) & 0x00ff0000) | 
                             ((input >> 8 ) & 0x0000ff00) | 
                             ((input >> 24) & 0x000000ff);
    }

    template <>
    inline inet_int64 endian_reverse<inet_int64>::convert(inet_int64 input)
    {
        return (inet_int64) ((input << 56) & 0xff00000000000000ull) |
                            ((input << 40) & 0x00ff000000000000ull) |
                            ((input << 24) & 0x0000ff0000000000ull) |
                            ((input << 8 ) & 0x000000ff00000000ull) |
                            ((input >> 8 ) & 0x00000000ff000000ull) |
                            ((input >> 24) & 0x0000000000ff0000ull) |
                            ((input >> 40) & 0x000000000000ff00ull) |
                            ((input >> 56) & 0x00000000000000ffull);
    }

    template <>
    inline inet_uint64 endian_reverse<inet_uint64>::convert(inet_uint64 input)
    {
        return (inet_uint64) ((input << 56) & 0xff00000000000000ull) |
                             ((input << 40) & 0x00ff000000000000ull) |
                             ((input << 24) & 0x0000ff0000000000ull) |
                             ((input << 8 ) & 0x000000ff00000000ull) |
                             ((input >> 8 ) & 0x00000000ff000000ull) |
                             ((input >> 24) & 0x0000000000ff0000ull) |
                             ((input >> 40) & 0x000000000000ff00ull) |
                             ((input >> 56) & 0x00000000000000ffull);
    }

} // namesapce
                           
#endif // #ifndef __LIBINET_BYTE_ORDER_H__
