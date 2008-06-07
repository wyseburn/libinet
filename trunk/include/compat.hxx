/*
 *  Version:     @(#)libinet/compat.hxx    0.0.1 23/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     The platform compatiable header.
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

#ifndef __LIBINET_COMPAT_H__
#define __LIBINET_COMPAT_H__

#if defined (_WIN32)

#pragma warning (disable:4099)
#pragma warning (disable:4348)

#define strcasecmp                         stricmp
#define strncasecmp                        strnicmp
#define ERRNO                              WSAGetLastError()
#define SOCKET_ERRNUM(errno)               WSA##errno

typedef char                               inet_int8;
typedef __int16                            inet_int16;
typedef __int32                            inet_int32;
typedef __int64                            inet_int64;

typedef unsigned char                      inet_uint8;
typedef unsigned __int16                   inet_uint16;
typedef unsigned __int32                   inet_uint32;
typedef unsigned __int64                   inet_uint64;

#elif defined (__linux__) || defined (__FreeBSD__)
#include <stdint.h>

typedef int8_t                             inet_int8;
typedef int16_t                            inet_int16;
typedef int32_t                            inet_int32;
typedef int64_t                            inet_int64;

typedef uint8_t                            inet_uint8;
typedef uint16_t                           inet_uint16;
typedef uint32_t                           inet_uint32;
typedef uint64_t                           inet_uint64;

#define closesocket(fd)                    close(fd)
#define ERRNO                              errno
#define SOCKET_ERRNUM(errnum)              errnum 

#endif // #if defined (_WIN32)

#endif // #ifndef __LIBINET_COMPAT_H__
