/**
 *  Version:     @(#)libinet/buffer.hxx    0.1.0 04/06/2008
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

#ifndef __LIBINET_BUFFER_H__
#define __LIBINET_BUFFER_H__

#include <string>
#include <cassert>

#include "util.hxx"
#include "compat.hxx"

namespace inet 
{
    struct buffer
    {
        struct node 
        {
            inet_uint32    off_; // valid data position for read
            inet_uint32    len_; // data length 
            INET_DLIST_ENTRY(node) entries_; 
            node() : off_(0), len_(0) {}
        };

        inet_uint32 node_data_size_;
        INET_DLIST_HEAD(node_queue, node) data_; 
        inet_uint32 cache_size_;
        inet_uint32 cache_len_;
        INET_DLIST_HEAD(node_cache_queue, node) cache_;
		
        buffer(inet_uint32 node_data_size = 4080, inet_uint32 cache_size = 5) 
            : node_data_size_(node_data_size), cache_size_(cache_size), cache_len_(0)
        {
            INET_DLIST_INIT(&data_);
            INET_DLIST_INIT(&cache_);
        }

        ~buffer() 
        {
            buffer::node* node;
            while (node = INET_DLIST_FIRST(&data_))
            {
                INET_DLIST_REMOVE(&data_, node, entries_);
                delete node;
            }

            while (node = INET_DLIST_FIRST(&cache_))
            {
                INET_DLIST_REMOVE(&cache_, node, entries_);
                delete node;
            }
        }

        /**
         * Allocate a memory block which size is node_data_size_. You can return it
         * through the same object function dealloc_node or push_node 
         */
        buffer::node* alloc_node()
        {
            buffer::node* node = NULL;
            if (cache_len_ <= 0) 
            {
                node = (buffer::node *)malloc(node_data_size_ + sizeof(buffer::node));
                assert(node);
                return new (node) buffer::node();
            }
				
            node = INET_DLIST_FIRST(&cache_);
            assert(node); 
            INET_DLIST_REMOVE(&cache_, node, entries_);
            cache_len_--;
            return new (node) buffer::node();
        }

        void dealloc_node(buffer::node* node)
        {
            assert(node);
            if (cache_len_ >= cache_size_)
            {
                delete node;
                return;
            }
            INET_DLIST_INSERT_TAIL(&cache_, node, entries_);
            cache_len_++;
        }

        /**
         * The argument node must be allocated by the same object of alloc_node or pop_node
         */
        void push_node(buffer::node* node)
        {
            assert(node);
            INET_DLIST_INSERT_TAIL(&data_, node, entries_);
        }

        buffer::node* pop_node()
        {
            buffer::node* node = INET_DLIST_FIRST(&data_);
            if (node)
            {
                INET_DLIST_REMOVE(&data_, node, entries_);
            }
            return node;
        }

        inet_uint32 length() const
        {
            inet_uint32 length = 0;
            buffer::node* node;
            INET_DLIST_FOREACH(node, &data_, entries_)
            {
                length += node->len_;
            }
            return length;
        }

        void drain(inet_uint32 nbytes)
        {
            buffer::node* node = INET_DLIST_FIRST(&data_);
            while (node && nbytes > 0 && nbytes >= node->len_)
            {
                nbytes -= node->len_;
                buffer::node* tmp = INET_DLIST_NEXT(node, entries_);
                INET_DLIST_REMOVE(&data_, node, entries_);
                dealloc_node(node);
                node = tmp;
            }
 
            if (node && nbytes > 0)
            {
                node->off_ += nbytes;
                node->len_ -= nbytes;
            }
        }

        /**
         * The object's data will be erased for nbytes length
         */
        inet_uint32 read(void* buf, inet_uint32 nbytes)
        {
            buffer::node* node = INET_DLIST_FIRST(&data_);
            if (nbytes <= node->len_)
            {
                memcpy(buf, (char *)node + sizeof(buffer::node) + node->off_, nbytes);
                node->off_ += nbytes;
                node->len_ -= nbytes;
                if (node->len_ == 0)
                {
                    INET_DLIST_REMOVE(&data_, node, entries_);
                    dealloc_node(node);
                }
                return nbytes;
            }

            inet_uint32 retlen = 0, drain = 0, blank = 0; 
            while (node && retlen < nbytes)
            {
                if (node->len_ == 0)
                {
                    buffer::node* tmp = INET_DLIST_NEXT(node, entries_);
                    INET_DLIST_REMOVE(&data_, node, entries_);
                    dealloc_node(node);
                    node = tmp;
                    continue;
                }
                drain = node->len_ < nbytes - drain ? node->len_ : nbytes - drain; 
                memcpy((char *)buf + retlen, (char *)node + sizeof(buffer::node) + node->off_, drain);
                node->off_ += drain;
                node->len_ -= drain;
                retlen += drain;
            }
            return retlen;
        }

        /**
         * The object's data will be erased for nbytes length
         */
        inet_uint32 read(buffer& buf, inet_uint32 nbytes)
        {
            buffer::node* node = INET_DLIST_FIRST(&data_);
            if (nbytes <= node->len_)
            {
                buf.write((char *)node + sizeof(buffer::node) + node->off_, nbytes); 
                node->off_ += nbytes;
                node->len_ -= nbytes;
                if (node->len_ == 0)
                {
                    INET_DLIST_REMOVE(&data_, node, entries_);
                    dealloc_node(node);
                }
                return nbytes;
            }

            inet_uint32 retlen = 0, drain = 0, blank = 0; 
            while (node && retlen < nbytes)
            {
                if (node->len_ == 0)
                {
                    buffer::node* tmp = INET_DLIST_NEXT(node, entries_);
                    INET_DLIST_REMOVE(&data_, node, entries_);
                    dealloc_node(node);
                    node = tmp;
                    continue;
                }
                drain = node->len_ < nbytes - drain ? node->len_ : nbytes - drain; 
                buf.write((char *)node + sizeof(buffer::node) + node->off_, drain);
                node->off_ += drain;
                node->len_ -= drain;
                retlen += drain;
            }
            return retlen; 
        }
 
        template <typename _T_>
        _T_ read() 
        {
            _T_ obj;
            inet_uint32 len = read((char *)&obj, sizeof(_T_));
            assert(len == sizeof(_T_));
            return obj; 
        }

        void write(const inet_int8* src, inet_uint32 nbytes)
        {
            assert(src);
            buffer::node* node = INET_DLIST_LAST(&data_, node_queue);
            if (node == NULL || node->off_ + node->len_ == node_data_size_)
            {
                push_node(alloc_node());
                node = INET_DLIST_LAST(&data_, node_queue);
            }

            if (node->off_ + node->len_ + nbytes <= node_data_size_)
            {
                memcpy((char *)node + sizeof(buffer::node) + node->off_ + node->len_, src, nbytes);
                node->len_ += nbytes;
                return;
            }

            inet_uint32 blank = node_data_size_ - node->off_ - node->len_;
            memcpy((char *)node + sizeof(buffer::node) + node->off_ + node->len_, src, blank);
            node->len_ += blank;
            write(src + blank, nbytes - blank);
        }

        template <typename _T_> 
        void write(_T_ value) { write((inet_int8 *)&value, sizeof(value)); }

        /**
         * The source object's data will be not erased
         */
        buffer& operator = (buffer& other)
        {
            buffer::node* node;
            while (node = INET_DLIST_FIRST(&data_))
            {
                INET_DLIST_REMOVE(&data_, node, entries_);
                dealloc_node(node);
            }
            this->operator += (other);
            return *this;
        }

        /**
         * The source object's data will be not erased
         */
        buffer& operator += (buffer& other)
        {
            buffer::node* node;
            INET_DLIST_FOREACH(node, &other.data_, entries_)
            {
                write((const inet_int8 *)((char *)node + sizeof(buffer::node) + node->off_), node->len_);
            }
            return *this;
        }

        /**
         * The source object's data will be erased
         */
        buffer& operator >> (buffer& value)
        {
            value += *this;
            buffer::node* node;
            while (node = INET_DLIST_FIRST(&data_))
            {
                INET_DLIST_REMOVE(&data_, node, entries_);
                delete node;
            }
        }

        buffer& operator << (bool value) 
        { 
            write<inet_int8>((inet_int8)value);  
            return *this; 
        }

        buffer& operator >> (bool& value) 
        { 
            value = read<inet_int8>() == 0 ? true : false; 
            return *this; 
        }

        buffer& operator << (inet_int8 value) 
        { 
            write<inet_int8>((inet_int8)value); 
            return *this; 
        }

        buffer& operator >> (inet_int8& value) 
        { 
            value = read<inet_int8>(); 
            return *this; 
        }

        buffer& operator << (inet_uint8 value) 
        { 
            write<inet_uint8>((inet_uint8)value); 
            return *this; 
        }
 
        buffer& operator >> (inet_uint8& value) 
        {
          value = read<inet_uint8>(); 
          return *this; 
        }

        buffer& operator << (inet_int16 value) 
        { 
            write<inet_int16>((inet_int16)value); 
            return *this; 
        }

        buffer& operator >> (inet_int16& value) 
        { 
            value = read<inet_int16>(); 
            return *this; 
        }

        buffer& operator << (inet_uint16 value) 
        { 
          write<inet_uint16>((inet_uint16)value); 
          return *this; 
        }

        buffer& operator >> (inet_uint16& value) 
        { 
           value = read<inet_uint16>(); 
           return *this; 
        }

        buffer& operator << (long value) 
        { 
            write<inet_int32>((long)value); 
            return *this; 
        }

        buffer& operator >> (long& value)
        { 
            value = read<long>(); 
            return *this; 
        }

        buffer& operator << (unsigned long value) 
        { 
            write<inet_int32>((unsigned long)value); 
            return *this; 
        }

        buffer& operator >> (unsigned long& value)
        { 
            value = read<unsigned long>(); 
            return *this; 
        }

        buffer& operator << (inet_int32 value) 
        { 
            write<inet_int32>((inet_int32)value); 
            return *this; 
        }

        buffer& operator >> (inet_int32& value)
        { 
            value = read<inet_int32>(); 
            return *this; 
        }

        buffer& operator << (inet_uint32 value) 
        { 
            write<inet_uint32>((inet_uint32)value); 
            return *this; 
        }

        buffer& operator >> (inet_uint32& value)
        { 
            value = read<inet_uint32>(); 
            return *this; 
        }

        buffer& operator << (inet_int64 value)
        { 
            write<inet_int64>((inet_uint32)value); 
            return *this; 
        }

        buffer& operator >> (inet_int64& value) 
        { 
            value = read<inet_int64>(); 
            return *this; 
        }

        buffer& operator << (inet_uint64 value) 
        { 
            write<inet_uint64>((inet_uint64)value); 
            return *this; 
        }

        buffer& operator >> (inet_uint64& value) 
        { 
            value = read<inet_uint64>(); 
            return *this; 
        }

        buffer& operator << (float value) 
        { 
            write<float>((float)value); 
            return *this; 
        }

        buffer& operator >> (float& value) 
        { 
            value = read<float>(); 
            return *this; 
        }

        buffer& operator << (double value) 
        { 
            write<double>((double)value); 
            return *this; 
        }

        buffer& operator >> (double& value) 
        { 
            value = read<double>(); 
            return *this; 
        }

        buffer& operator << (const std::string& value)
        {
            write((inet_int8 const *)value.c_str(), (inet_uint32)value.length());
            write((inet_int8)0); 
            return *this;
        }

        buffer& operator << (const char* value)
        {
            write((inet_int8 const *)value, value ? (inet_uint32)strlen(value) : 0);
            write((inet_int8)0);
            return *this;
        }

        buffer& operator >> (std::string& value)
        {
            value.clear();
            buffer::node* node = INET_DLIST_FIRST(&data_);
            while (node)
            {
                inet_int8 c = read<inet_int8>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

        buffer& operator << (const wchar_t* value)
        {
            write((inet_int8 const *)value, value ? (inet_uint32)wcslen(value)*2 : 0);
            write((inet_uint16)0);
            return *this;
        }

        buffer& operator << (const std::wstring& value)
        {
            write((inet_int8 const *)value.c_str(), (inet_uint32)value.length()*2);
            write((inet_uint16)0);
            return *this;
        }
 
        buffer& operator >> (std::wstring& value)
        {
            value.clear();
            buffer::node* node = INET_DLIST_FIRST(&data_);
            while (node)
            {
                inet_uint16 c = read<inet_uint16>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

    }; 
} // namespace

#endif // #ifndef __LIBINET_BUFFER_H__

