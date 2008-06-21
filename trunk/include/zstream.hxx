/**
 *  Version:     @(#)libinet/zstream.hxx    0.0.1 21/06/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     
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

#ifndef __LIBINET_ZSTREAM_H__
#define __LIBINET_ZSTREAM_H__

#include "compat.hxx"

namespace inet
{
    /* Compress data */
    inet_int32 zcompress(const inet_int8* data,  inet_uint32 datalen, inet_int8* zdata, inet_uint32& zdatalen);

    /* Uncompress data */
    inet_int32 zdecompress(const inet_int8* zdata, inet_uint32 zdatalen, inet_int8* data, inet_uint32& datalen);

} // namespace

#endif // #ifndef __LIBINET_ZSTREAM_H__
