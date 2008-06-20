/**
 *  Version:     @(#)libinet/blob.hxx    0.0.1 20/06/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     Blob type data part user need to convert byte order 
 *               if there is a integer object
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
 
#ifndef __LIBINET_BLOB_H__
#define __LIBINET_BLOB_H__

#include "compat.hxx"

namespace inet
{
    struct blob 
    { 
        inet_uint32 len_; 
        inet_int8* data_; 
        blob() : len_(0), data_(0) {}
        blob(const blob& b) {len_ = b.len_; data_ = b.data_;}
        blob& operator = (const blob& b) {len_ = b.len_;data_ = b.data_;return *this;}
    };
    template <class Type> struct is_blob { static const bool value = false; };
    template <> struct is_blob<blob> { static const bool value = true; };
}

#endif // ifndef __LIBINET_BLOB_H__
