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

#define INET_HTON2 (value) ((((value) << 8) & 0xff00) | (((value) >> 8) & 0x00ff))


#define INET_HTON4 (value) ((((value) << 24) & 0xff000000) \
                           |(((value) << 8 ) & 0x00ff0000) \
                           |(((value) >> 8 ) & 0x0000ff00) \
                           |(((value) >> 24) & 0x000000ff) )

#define INET_HTON8 (value) ((((value) << 56) & 0xff00000000000000ull) \
                           |(((value) << 40) & 0x00ff000000000000ull) \
                           |(((value) << 24) & 0x0000ff0000000000ull) \
                           |(((value) << 8 ) & 0x000000ff00000000ull) \
                           |(((value) >> 8 ) & 0x00000000ff000000ull) \
                           |(((value) >> 24) & 0x0000000000ff0000ull) \
                           |(((value) >> 40) & 0x000000000000ff00ull) \
                           |(((value) >> 56) & 0x00000000000000ffull) ) 
                           
#endif // #ifndef __LIBINET_BYTE_ORDER_H__
