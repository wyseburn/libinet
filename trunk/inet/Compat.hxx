/**
 *  Version:     @(#)Compat.hxx    0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     The platform compatiable header.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty 
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 *  the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with this library; if not, write to the 
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
 *  Boston, MA 02111-1307  USA
 */

#ifndef __INET_COMPAT_H__
#define __INET_COMPAT_H__

#if defined (_WIN32)

#pragma warning (disable:4099)
#pragma warning (disable:4348)

#define strcasecmp                         stricmp
#define strncasecmp                        strnicmp

#define ERRNO                              WSAGetLastError()
#define SOCKET_ERRNUM(errno)               WSA##errno

typedef char                               Int8;
typedef __int16                            Int16;
typedef __int32                            Int32;
typedef __int64                            Int64;

typedef unsigned char                      UInt8;
typedef unsigned __int16                   UInt16;
typedef unsigned __int32                   UInt32;
typedef unsigned __int64                   UInt64;

#elif defined (__linux__) || defined (__FreeBSD__)

typedef int8_t                             Int8;
typedef int16_t                            Int16;
typedef int32_t                            Int32;
typedef int64_t                            Int64;

typedef uint8_t                            UInt8;
typedef uint16_t                           UInt16;
typedef uint32_t                           UInt32;
typedef uint64_t                           UInt64;

#define closesocket(fd)                    close(fd)
#define ERRNO                              errno
#define SOCKET_ERRNUM(errnum)              errnum 

#include <stdint.h>

#endif // #if defined (_WIN32)

#endif // #ifndef __INET_COMPAT_H__
