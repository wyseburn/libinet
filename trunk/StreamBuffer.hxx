/**
 *  Version:     @(#)Buffer.hxx    0.0.3 04/06/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     
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

#ifndef __INET_STREAM_BUFFER_H__
#define __INET_STREAM_BUFFER_H__

#include <cassert>
#include <string>

#include "Compat.hxx"
#include "Util.hxx"

namespace INet
{
    struct StreamBuffer
    {
        struct Node 
        {
            UInt32    mOff; // valid data position for read
            UInt32    mLen; // data length 
            INET_QUEUE_ENTRY(Node) mEntries; 
            Node() : mOff(0), mLen(0) {}
        };

        UInt32 mNodeDataSize;
        INET_QUEUE_HEAD(NodeQueue, Node) mData; 
        UInt32 mCacheSize;
        UInt32 mCacheLen;
        INET_QUEUE_HEAD(NodeCacheQueue, Node) mCache;
		
        StreamBuffer(UInt32 nodeDataSize = 4080, UInt32 cacheSize = 5) 
            : mNodeDataSize(nodeDataSize), mCacheSize(cacheSize), mCacheLen(0)
        {
            INET_QUEUE_INIT(&mData);
            INET_QUEUE_INIT(&mCache);
        }

        ~StreamBuffer() 
        {
            Node* node;
            while (node = INET_QUEUE_FIRST(&mData))
            {
                INET_QUEUE_REMOVE(&mData, node, mEntries);
                delete node;
            }

            while (node = INET_QUEUE_FIRST(&mCache))
            {
                INET_QUEUE_REMOVE(&mData, node, mEntries);
                delete node;
            }
        }

        Node* allocNode()
        {
            Node* node = NULL;
            if (mCacheLen <= 0) 
            {
                node = (Node *)malloc(mNodeDataSize + sizeof(Node));
                return new (node) Node();
            }
				
            node = INET_QUEUE_FIRST(&mCache);
            INET_QUEUE_REMOVE(&mCache, node, mEntries);
            mCacheLen--;
            return new (node) Node();
        }

        void deallocNode(Node* node)
        {
            assert(node);
            if (mCacheLen >= mCacheSize)
            {
                delete node;
                return;
            }
            INET_QUEUE_INSERT_TAIL(&mCache, node, mEntries);
            mCacheLen++;
        }

        void pushNode(Node* node)
        {
            assert(node);
            INET_QUEUE_INSERT_TAIL(&mData, node, mEntries);
        }

        Node* popNode()
        {
            Node* node = INET_QUEUE_FIRST(&mData);
            if (node)
            {
                INET_QUEUE_REMOVE(&mData, node, mEntries);
            }
            return node;
        }

        UInt32 length() const
        {
            UInt32 length = 0;
            Node* node;
            INET_QUEUE_FOREACH(node, &mData, mEntries)
            length += node->mLen;
            return length;
        }

        UInt32 read(void* buf, UInt32 nBytes)
        {
            UInt32 drain = 0;
            Node* node = INET_QUEUE_FIRST(&mData);
            if (node->mOff + nBytes <= mNodeDataSize)
            {
                drain = node->mLen < nBytes ? node->mLen : nBytes;
                memcpy(buf, (char *)node + sizeof(Node) + node->mOff, drain);
                node->mOff += drain;
                node->mLen -= drain;
                return drain;
            }

            UInt32 len = 0, blank = 0; 
            while (node && len < nBytes)
            {
                blank = mNodeDataSize - node->mOff;
                if (blank == 0)
                {
                    Node* tmp = INET_QUEUE_NEXT(node, mEntries);
                    INET_QUEUE_REMOVE(&mData, node, mEntries);
                    deallocNode(node);
                    node = tmp;
                    continue;
		}
                drain = blank < nBytes - drain ? blank : nBytes - drain;
                memcpy((char *)buf + len, (char *)node + sizeof(Node) + node->mOff, drain);
                node->mOff += drain;
                node->mLen -= drain;
                len += drain;
            }
            return len;
        }

        void append(const UInt8* src, UInt32 nBytes)
        {
            assert(src);
            Node* node = INET_QUEUE_LAST(&mData, NodeQueue);
            if (node == NULL)
            {
                node = allocNode();
                pushNode(node);
            }

            if (node->mOff + node->mLen + nBytes <= mNodeDataSize)
            {
                memcpy((char *)node + sizeof(Node) + node->mOff + node->mLen, src, nBytes);
                node->mLen += nBytes;
                return;
            }

            UInt32 drain = mNodeDataSize - node->mOff - node->mLen;
            memcpy((char *)node + sizeof(Node) + node->mOff + node->mLen, src, drain);
            node->mLen += drain;
            if (!INET_QUEUE_NEXT(node, mEntries))
            {
                Node* tmp = allocNode();
                INET_QUEUE_INSERT_AFTER(&mData, node, tmp, mEntries);
            }
            node = INET_QUEUE_NEXT(node, mEntries);
            append(src + drain, nBytes - drain);
        }

        template <typename _T_> 
        void append(_T_ value) { append((UInt8 *)&value, sizeof(value)); }

        template <typename _T_>
        _T_ read() 
        {
            _T_ obj;
            UInt32 len = read((char *)&obj, sizeof(_T_));
            assert(len == sizeof(_T_));
            return obj; 
        }

        StreamBuffer& operator << (bool value) 
        { 
            append<Int8>((Int8)value);  
            return *this; 
        }

        StreamBuffer& operator >> (bool& value) 
        { 
            value = read<Int8>() == 0 ? true : false; 
            return *this; 
        }

        StreamBuffer& operator << (char value) 
        { append<Int8>((char)value); 
            return *this; 
        }

        StreamBuffer& operator >> (char& value) 
        { 
            value = read<char>(); 
            return *this; 
        }
#ifndef _WIN32
        StreamBuffer& operator << (Int8 value) 
        { 
            append<Int8>((Int8)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int8& value) 
        { 
            value = read<Int8>(); 
            return *this; 
        }
#endif

        StreamBuffer& operator << (UInt8 value) 
        { 
            append<UInt8>((UInt8)value); 
            return *this; 
        }
 
        StreamBuffer& operator >> (UInt8& value) 
        {
          value = read<UInt8>(); 
          return *this; 
        }

        StreamBuffer& operator << (Int16 value) 
        { 
            append<Int16>((Int16)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int16& value) 
        { 
            value = read<Int16>(); 
            return *this; 
        }

        StreamBuffer& operator << (UInt16 value) 
        { 
          append<UInt16>((UInt16)value); 
          return *this; 
        }

        StreamBuffer& operator >> (UInt16& value) 
        { 
           value = read<UInt16>(); 
           return *this; 
        }

        StreamBuffer& operator << (long value) 
        { 
            append<Int32>((long)value); 
            return *this; 
        }

        StreamBuffer& operator >> (long& value)
        { 
            value = read<long>(); 
            return *this; 
        }

        StreamBuffer& operator << (unsigned long value) 
        { 
            append<Int32>((unsigned long)value); 
            return *this; 
        }

        StreamBuffer& operator >> (unsigned long& value)
        { 
            value = read<unsigned long>(); 
            return *this; 
        }

        StreamBuffer& operator << (Int32 value) 
        { 
            append<Int32>((Int32)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int32& value)
        { 
            value = read<Int32>(); 
            return *this; 
        }

        StreamBuffer& operator << (UInt32 value) 
        { 
            append<UInt32>((UInt32)value); 
            return *this; 
        }

        StreamBuffer& operator >> (UInt32& value)
        { 
            value = read<UInt32>(); 
            return *this; 
        }

        StreamBuffer& operator << (Int64 value)
        { 
            append<Int64>((UInt32)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int64& value) 
        { 
            value = read<Int64>(); 
            return *this; 
        }

        StreamBuffer& operator << (UInt64 value) 
        { 
            append<UInt64>((UInt64)value); 
            return *this; 
        }

        StreamBuffer& operator >> (UInt64& value) 
        { 
            value = read<UInt64>(); 
            return *this; 
        }

        StreamBuffer& operator << (float value) 
        { 
            append<float>((float)value); 
            return *this; 
        }

        StreamBuffer& operator >> (float& value) 
        { 
            value = read<float>(); 
            return *this; 
        }

        StreamBuffer& operator << (double value) 
        { 
            append<double>((double)value); 
            return *this; 
        }

        StreamBuffer& operator >> (double& value) 
        { 
            value = read<double>(); 
            return *this; 
        }

        StreamBuffer& operator << (const std::string& value)
        {
            append((UInt8 const *)value.c_str(), (UInt32)value.length());
            append((UInt8)0); 
            return *this;
        }

        StreamBuffer& operator << (const char* value)
        {
            append((UInt8 const *)value, value ? (UInt32)strlen(value) : 0);
            append((UInt8)0);
            return *this;
        }

        StreamBuffer& operator >> (std::string& value)
        {
            value.clear();
			Node* node = INET_QUEUE_FIRST(&mData);
            while (node)
            {
                Int8 c = read<Int8>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

        StreamBuffer& operator << (const wchar_t* value)
        {
            append((UInt8 const *)value, value ? (UInt32)wcslen(value)*2 : 0);
            append((UInt16)0);
            return *this;
        }

        StreamBuffer& operator << (const std::wstring& value)
        {
            append((UInt8 const *)value.c_str(), (UInt32)value.length()*2);
            append((UInt16)0);
            return *this;
        }
 
        StreamBuffer& operator >> (std::wstring& value)
        {
            value.clear();
			Node* node = INET_QUEUE_FIRST(&mData);
            while (node)
            {
                UInt16 c = read<UInt16>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

    }; 
} // namespace

#endif // __INET_STREAM_BUFFER_H__
