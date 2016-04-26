/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_STREAMBUFFER_H
#define IGAME_OS_STREAMBUFFER_H

#include "os/include/Compat.h"
#include "os/include/DataStruct.h"
#include "os/include/SingletonObject.h"

namespace IGame 
{
    struct StreamBuffer 
    {
        struct Node 
        {
            Node() : m_Off(0), m_Len(0) {}
            _UInt32 m_Off; // valid data position for read 
            _UInt32 m_Len; // data length
            IGAME_DLIST_ENTRY(Node) m_Entries; 
        };

        _UInt32 m_NodeDataSize;
        IGAME_DLIST_HEAD(NodeQue, Node) m_Data;
        _Int32 m_CacheSize;
        _Int32 m_CacheLen;
        IGAME_DLIST_HEAD(NodeCacheQue, Node) m_Cache;
		
        StreamBuffer(_UInt32 nodeDataSize = 4076, _Int32 cacheSize = 5) 
            : m_NodeDataSize(nodeDataSize), m_CacheSize(cacheSize), m_CacheLen(0)
        {
            IGAME_DLIST_INIT(&m_Data);
            IGAME_DLIST_INIT(&m_Cache);
        }

        ~StreamBuffer() 
        {
            Node* node;
            while (node = IGAME_DLIST_FIRST(&m_Data))
            {
                IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                XFREE(node);
            }

            while (node = IGAME_DLIST_FIRST(&m_Cache))
            {
                IGAME_DLIST_REMOVE(&m_Cache, node, m_Entries);
                XFREE(node);
            }
        }

        /**
         * Allocate a memory block which size is m_NodeDataSize. You can return it
         * through the same object function DeallocNode or PushNode 
         */
        inline Node* AllocNode()
        {
            Node* node = NULL;
            if (m_CacheLen <= 0) 
            {
                node = (Node *)XALLOC(m_NodeDataSize + sizeof(Node));
                assert(node);
                return new (node) Node();
            }
				
            node = IGAME_DLIST_FIRST(&m_Cache);
            assert(node); 
            IGAME_DLIST_REMOVE(&m_Cache, node, m_Entries);
            m_CacheLen--;
            return new (node) Node();
        }

        inline void DeallocNode(Node* node)
        {
            assert(node);
            if (m_CacheLen >= m_CacheSize)
            {
                XFREE(node);
                return;
            }
            IGAME_DLIST_INSERT_TAIL(&m_Cache, node, m_Entries);
            m_CacheLen++;
        }

        /**
         * The argument node must be allocated by the same object of AllocNode or PopNode
         */
        inline void PushNode(Node* node)
        {
            assert(node);
            IGAME_DLIST_INSERT_TAIL(&m_Data, node, m_Entries);
        }

        inline Node* PopNode()
        {
            Node* node = IGAME_DLIST_FIRST(&m_Data);
            if (node)
            {
                IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
            }
            return node;
        }

        void Print() /* string output */
        {
            Node* node;
            IGAME_DLIST_FOREACH(node, &m_Data, m_Entries)
            {
                if (node->m_Len + node->m_Off < m_NodeDataSize) 
                    *((char *)node + sizeof(Node) + node->m_Off + node->m_Len) = 0;
                std::cout << (char *)node + sizeof(Node) + node->m_Off;
            }
            std::cout << std::endl;
        }

        _UInt32 Length() const
        {
            _UInt32 length = 0;
            Node* node;
            IGAME_DLIST_FOREACH(node, &m_Data, m_Entries)
            {
                length += node->m_Len;
            }
            return length;
        }

        void Clear()
        {
            Node* node;
            while (node = IGAME_DLIST_FIRST(&m_Data))
            {
                IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                DeallocNode(node);
            }
        }

        void Drain(_UInt32 nbytes)
        {
            Node* node = IGAME_DLIST_FIRST(&m_Data);
            while (node && nbytes > 0 && nbytes >= node->m_Len)
            {
                nbytes -= node->m_Len;
                Node* tmp = IGAME_DLIST_NEXT(node, m_Entries);
                IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                DeallocNode(node);
                node = tmp;
            }
 
            if (node && nbytes > 0)
            {
                node->m_Off += nbytes;
                node->m_Len -= nbytes;
            }
        }

        /**
         * The object's data will be erased for nbytes length
         */
        _UInt32 Read(void* buf, _UInt32 nbytes)
        {
            Node* node = IGAME_DLIST_FIRST(&m_Data);
            if (node && nbytes <= node->m_Len)
            {
                memcpy(buf, (char *)node + sizeof(Node) + node->m_Off, nbytes);
                node->m_Off += nbytes;
                node->m_Len -= nbytes;
                if (node->m_Len == 0)
                {
                    IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                    DeallocNode(node);
                }
                return nbytes;
            }

            _UInt32 retlen = 0, drain = 0;
            while (node && retlen < nbytes)
            {
                if (node->m_Len == 0)
                {
                    Node* tmp = IGAME_DLIST_NEXT(node, m_Entries);
                    IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                    DeallocNode(node);
                    node = tmp;
                    continue;
                }
                drain = node->m_Len < nbytes - retlen ? node->m_Len : nbytes - retlen; 
                memcpy((char *)buf + retlen, (char *)node + sizeof(Node) + node->m_Off, drain);
                node->m_Off += drain;
                node->m_Len -= drain;
                retlen += drain;
            }
            return retlen;
        }

        /**
         * The object's data will be erased for nbytes length
         */
        _UInt32 Read(StreamBuffer& buf, _UInt32 nbytes)
        {
            Node* node = IGAME_DLIST_FIRST(&m_Data);
            if (node && nbytes <= node->m_Len)
            {
                buf.Write((char *)node + sizeof(Node) + node->m_Off, nbytes); 
                node->m_Off += nbytes;
                node->m_Len -= nbytes;
                if (node->m_Len == 0)
                {
                    IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                    DeallocNode(node);
                }
                return nbytes; 
            }

            _UInt32 retlen = 0, drain = 0; 
            while (node && retlen < nbytes)
            {
                if (node->m_Len == 0)
                {
                    Node* tmp = IGAME_DLIST_NEXT(node, m_Entries);
                    IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                    DeallocNode(node);
                    node = tmp;
                    continue;
                }
                drain = node->m_Len < nbytes - retlen ? node->m_Len : nbytes - retlen; 
                buf.Write((char *)node + sizeof(Node) + node->m_Off, drain);
                node->m_Off += drain;
                node->m_Len -= drain;
                retlen += drain;
            }
            return retlen; 
        }
 
        template <typename _T_>
        _T_ Read() 
        {
            _T_ obj;
            _UInt32 len = Read((char *)&obj, sizeof(_T_));
            assert(len == sizeof(_T_));
            return obj; 
        }

        void Write(const _Int8* src, _UInt32 nbytes)
        {
            assert(src);
            Node* node = IGAME_DLIST_LAST(&m_Data, NodeQue);
            if (node == NULL || node->m_Off + node->m_Len + 8 >= m_NodeDataSize)
            {
                PushNode(AllocNode());
                node = IGAME_DLIST_LAST(&m_Data, NodeQue);
            }

            if (node->m_Off + node->m_Len + nbytes <= m_NodeDataSize)
            {
                memcpy((char *)node + sizeof(Node) + node->m_Off + node->m_Len, src, nbytes);
                node->m_Len += nbytes;
                return;
            }

            _UInt32 blank = m_NodeDataSize - node->m_Off - node->m_Len;
            memcpy((char *)node + sizeof(Node) + node->m_Off + node->m_Len, src, blank);
            node->m_Len += blank;
            Write(src + blank, nbytes - blank);
        }

        template <typename _T_> 
        void Write(_T_ value) 
        { 
            Write((_Int8 *)&value, sizeof(value)); 
        }

        /**
         * The source object's data will be not erased
         */
        StreamBuffer& operator = (StreamBuffer& other)
        {
            Node* node;
            while (node = IGAME_DLIST_FIRST(&m_Data))
            {
                IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                DeallocNode(node);
            }
            this->operator += (other);
            return *this;
        }

        /**
         * The source object's data will be not erased
         */
        StreamBuffer& operator += (const StreamBuffer& other)
        {
            Node* node;
            IGAME_DLIST_FOREACH(node, &other.m_Data, m_Entries)
            {
                Write((const _Int8 *)((char *)node + sizeof(Node) + node->m_Off), node->m_Len);
            }
            return *this;
        }

        /**
         * The source object's data will be erased
         */
        StreamBuffer& operator >> (StreamBuffer& value)
        {
            value += *this;
            Node* node;
            while (node = IGAME_DLIST_FIRST(&m_Data))
            {
                IGAME_DLIST_REMOVE(&m_Data, node, m_Entries);
                XFREE(node);
            }
            return *this;
        }

        StreamBuffer& operator << (bool value) 
        { 
            Write<_Int8>((_Int8)value);  
            return *this; 
        }

        StreamBuffer& operator >> (bool& value) 
        { 
            value = Read<_Int8>() == 0 ? false : true; 
            return *this; 
        }

        StreamBuffer& operator << (_Int8 value) 
        { 
            Write<_Int8>((_Int8)value); 
            return *this; 
        }

        StreamBuffer& operator >> (_Int8& value) 
        { 
            value = Read<_Int8>(); 
            return *this; 
        }

        StreamBuffer& operator << (_UInt8 value) 
        { 
            Write<_UInt8>((_UInt8)value); 
            return *this; 
        }
 
        StreamBuffer& operator >> (_UInt8& value) 
        {
          value = Read<_UInt8>(); 
          return *this; 
        }

        StreamBuffer& operator << (_Int16 value) 
        { 
            Write<_Int16>((_Int16)value); 
            return *this; 
        }

        StreamBuffer& operator >> (_Int16& value) 
        { 
            value = Read<_Int16>(); 
            return *this; 
        }

        StreamBuffer& operator << (_UInt16 value) 
        { 
          Write<_UInt16>((_UInt16)value); 
          return *this; 
        }

        StreamBuffer& operator >> (_UInt16& value) 
        { 
           value = Read<_UInt16>(); 
           return *this; 
        }

        StreamBuffer& operator << (long value) 
        { 
            Write<_Int32>((long)value); 
            return *this; 
        }

        StreamBuffer& operator >> (long& value)
        { 
            value = Read<long>(); 
            return *this; 
        }

        StreamBuffer& operator << (unsigned long value) 
        { 
            Write<_Int32>((unsigned long)value); 
            return *this; 
        }

        StreamBuffer& operator >> (unsigned long& value)
        { 
            value = Read<unsigned long>(); 
            return *this; 
        }

        StreamBuffer& operator << (_Int32 value) 
        { 
            Write<_Int32>((_Int32)value); 
            return *this; 
        }

        StreamBuffer& operator >> (_Int32& value)
        { 
            value = Read<_Int32>(); 
            return *this; 
        }

        StreamBuffer& operator << (_UInt32 value) 
        { 
            Write<_UInt32>((_UInt32)value); 
            return *this; 
        }

        StreamBuffer& operator >> (_UInt32& value)
        { 
            value = Read<_UInt32>(); 
            return *this; 
        }

        StreamBuffer& operator << (_Int64 value)
        { 
            Write<_Int64>((_Int64)value); 
            return *this; 
        }

        StreamBuffer& operator >> (_Int64& value) 
        { 
            value = Read<_Int64>(); 
            return *this; 
        }

        StreamBuffer& operator << (_UInt64 value) 
        { 
            Write<_UInt64>((_UInt64)value); 
            return *this; 
        }

        StreamBuffer& operator >> (_UInt64& value) 
        { 
            value = Read<_UInt64>(); 
            return *this; 
        }

        StreamBuffer& operator << (float value) 
        { 
            Write<float>((float)value); 
            return *this; 
        }

        StreamBuffer& operator >> (float& value) 
        { 
            value = Read<float>(); 
            return *this; 
        }

        StreamBuffer& operator << (double value) 
        { 
            Write<double>((double)value); 
            return *this; 
        }

        StreamBuffer& operator >> (double& value) 
        { 
            value = Read<double>(); 
            return *this; 
        }

        StreamBuffer& operator << (const wchar_t& value)
        {
            Write<wchar_t>((wchar_t)value); 
            return *this;
        }

        StreamBuffer& operator >> (wchar_t& value)
        {
            value = Read<wchar_t>();
            return *this;
        }

        StreamBuffer& operator << (const std::string& value)
        {
            Write((_Int8 const *)value.c_str(), (_UInt32)value.length());
            Write((_Int8)0); 
            return *this;
        }

        StreamBuffer& operator << (const char* value)
        {
            Write((_Int8 const *)value, value ? (_UInt32)strlen(value) : 0);
            Write((_Int8)0);
            return *this;
        }

        StreamBuffer& operator >> (std::string& value)
        {
            //value.clear();
            while (IGAME_DLIST_FIRST(&m_Data))
            {
                _Int8 c = Read<_Int8>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

        StreamBuffer& operator << (const wchar_t* value)
        {
            Write((_Int8 const *)value, value ? (_UInt32)wcslen(value)*2 : 0);
            Write((_UInt16)0);
            return *this;
        }

        StreamBuffer& operator << (const std::wstring& value)
        {
            Write((_Int8 const *)value.c_str(), (_UInt32)value.length()*2);
            Write((_UInt16)0);
            return *this;
        }
 
        StreamBuffer& operator >> (std::wstring& value)
        {
            //value.clear();
            while (IGAME_DLIST_FIRST(&m_Data))
            {
                _UInt16 c = Read<_UInt16>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }
    }; 

    template <class Type> struct IsIGameStreamBuffer 
    { 
        static const bool s_Value = false; 
    };

    template <> struct IsIGameStreamBuffer<StreamBuffer> 
    { 
        static const bool s_Value = true; 
    };

} // namespace IGame

#endif // #ifndef IGAME_OS_STREAMBUFFER_H


