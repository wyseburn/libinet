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

typedef char                               int8_t;
typedef __int16                            int16_t;
typedef __int32                            int32_t;
typedef __int64                            int64_t;

typedef unsigned char                      uint8_t;
typedef unsigned __int16                   uint16_t;
typedef unsigned __int32                   uint32_t;
typedef unsigned __int64                   uint64_t;

#elif defined (__linux__) || defined (__FreeBSD__)

#define closesocket(fd)                    close(fd)
#define ERRNO                              errno
#define SOCKET_ERRNUM(errnum)              errnum 

#include <stdint.h>

#endif // #if defined (_WIN32)


#endif // #ifndef __INET_COMPAT_H__
