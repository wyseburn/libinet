/**
 *  Version:     @(#)libinet/zstream.cxx    0.0.1 21/06/2008
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
 *  with this library; if not, write to the Free Software Foundation, Inc., 59 Temple 
 *  Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include <zlib.h>
#include "zstream.hxx"

inet_int32 
inet::zcompress(const inet_int8* data, inet_uint32 datalen, inet_int8* zdata, inet_uint32& zdatalen)
{
	z_stream c_stream;
	int err = 0;

	if(data && datalen > 0)
	{
		c_stream.zalloc = (alloc_func)0;
		c_stream.zfree = (free_func)0;
		c_stream.opaque = (voidpf)0;
		deflateInit(&c_stream, Z_DEFAULT_COMPRESSION);
		c_stream.next_in  = (Bytef *)data;
		c_stream.next_out = (Bytef *)zdata;
		while (c_stream.total_in != datalen && c_stream.total_out < zdatalen) 
		{
			c_stream.avail_in = c_stream.avail_out = 1; /* force small buffers */
			if(deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
		}
		for (;;) {
			c_stream.avail_out = 1;
			if((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
			if(err != Z_OK) return -1;
			
		}
		if(deflateEnd(&c_stream) != Z_OK) return -1;
		zdatalen = c_stream.total_out;
		return 0;
	}
	return -1;
}

inet_int32 
inet::zdecompress(const inet_int8* zdata, inet_uint32 zdatalen, inet_int8* data, inet_uint32& datalen)
{
	int err = 0;
	z_stream d_stream; /* decompression stream */

	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;

	d_stream.next_in  = (Byte *)zdata;
	d_stream.avail_in = 0;
	d_stream.next_out = (Byte *)data;

	if(inflateInit(&d_stream) != Z_OK) return -1;
	while (d_stream.total_out < datalen && d_stream.total_in < zdatalen) {
		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
		if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
		if(err != Z_OK) return -1;
	}
	if(inflateEnd(&d_stream) != Z_OK) return -1;
	datalen = d_stream.total_out;
	return 0;
}

