/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_STREAMBUFFER_SERIALIZE_IMPLS_H
#define IGAME_OS_STREAMBUFFER_SERIALIZE_IMPLS_H

#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>

#include "os/include/ByteOrder.h"
#include "os/include/Blob.h"
#include "os/include/StreamBuffer.h"
#include "os/include/Container.h"
#include "os/include/SerializeImpls.h"

#define IGAME_FLOAT_PRECISION  1000000
#define IGAME_DOUBLE_PRECISION 1000000000

namespace IGame
{
    template <class Type> struct IsIGameStreamBufferData 
    { 
        static const bool s_Value = false; 
    };

    template <> struct IsIGameStreamBufferData<IGame::StreamBuffer> 
    { 
        static const bool s_Value = true; 
    };

    template <class DataType >
    struct S11NBooleanImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            data << obj;
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            if (data.Length() < sizeof(ObjType))
                return false;
            data >> obj;
            return true;
        }
    };

    template <class DataType >
    struct S11NIntegerImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::Convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            if (data.Length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::Convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NFloatImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            return sizeof(obj); 
#else
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << obj;
            return sizeof(obj);
#else
            assert(sizeof(ObjType) == 4);
            _Int32 integer = (_Int32)obj;
            _Int32 decimal = (_Int32)((obj - integer) * IGAME_FLOAT_PRECISION);
            data << EndianReverse<_Int32>::Convert(integer);
            data << EndianReverse<_Int32>::Convert(decimal);
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            if (data.Length() < sizeof(ObjType))
                return false;
            data >> obj;
#else
            assert(sizeof(ObjType) == 4);
            if (data.Length() < sizeof(ObjType) * 2)
                return false;
            _Int32 integer, decimal;
            data >> integer;
            integer = EndianReverse<_Int32>::Convert(integer);
            data >> decimal;
            decimal = EndianReverse<_Int32>::Convert(decimal);
            obj = (ObjType)integer;
            obj += ((ObjType)(decimal)) / IGAME_FLOAT_PRECISION;
#endif
            return true;
        }
    };

    template <class DataType >
    struct S11NDoubleImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            return sizeof(obj); 
#else
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << obj;
            return sizeof(obj);
#else
            _Int64 integer = (_Int64)obj;
            _Int64 decimal = (_Int64)((obj - integer) * IGAME_DOUBLE_PRECISION);
            data << EndianReverse<_Int64>::Convert(integer);
            data << EndianReverse<_Int64>::Convert(decimal);
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            if (data.Length() < sizeof(ObjType))
                return false;
            data >> obj;
#else
            assert(sizeof(ObjType) == 8);
            if (data.Length() < sizeof(ObjType) * 2)
                return false;
            _Int64 integer, decimal;
            data >> integer;
            integer = EndianReverse<_Int64>::Convert(integer);
            data >> decimal;
            decimal = EndianReverse<_Int64>::Convert(decimal);
            obj = (ObjType)integer;
            obj += ((ObjType)(decimal)) / IGAME_DOUBLE_PRECISION;
#endif
            return true;
        }
    };

    template <class DataType >
    struct S11NStdBasicStringImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            _UInt32 size = sizeof(_UInt32);
            size += (_UInt32)obj.size();
            return size;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            _UInt32 size = (_UInt32)obj.size();
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<_UInt32>::Convert(size);
#endif
            if (mask != 0)
            {
                for (_Int32 i = 0; i < (_Int32)obj.size(); i++ )
                {
                    data << (ElemType)(obj[i] ^ (ElemType)(obj.size() + 0));
                }
                return sizeof(_UInt32) + (_UInt32)obj.size();
            }
            else
            {
                data.Write((_Int8 const *)obj.c_str(), (_UInt32)obj.size() * sizeof(ElemType));
                return sizeof(_UInt32) + (_UInt32)obj.size();
            }
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            if (data.Length() < sizeof(_UInt32))
                return false;

            _UInt32 size;
            data >> size;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<_UInt32>::Convert(size);
#endif
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

            if (data.Length() < size) 
                return false;

            if (mask != 0)
            {
                ElemType c;
                for (_UInt32 i = 0; i < size; i++)
                {
                    data >> c; 
                    obj.push_back(c ^ (ElemType)(size + 0));
                }
            }
            else
            {
                ElemType tmpbuf[2048];
                _UInt32 readlen;
                obj.clear();
                while (size > 0) 
                {
                    readlen = size < 2046 ? size : 2046; 
                    data.Read(tmpbuf, readlen * sizeof(ElemType)); 
                    tmpbuf[readlen] = 0;
                    obj += tmpbuf;
                    size -= readlen;
                }
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NStdPairImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            return S11NTraits<FirstType >::Size(data, obj.first) + 
                S11NTraits<SecondType >::Size(data, obj.second);
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            return S11NTraits<FirstType >::Serialize(data, (FirstType&)obj.first, name, mask) 
                + S11NTraits<SecondType >::Serialize(data, (SecondType&)obj.second, name, mask);
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            return S11NTraits<FirstType >::Unserialize(data, obj.first, name, mask) &&
                S11NTraits<SecondType >::Unserialize(data, obj.second, name, mask);
        }
    };

    template <class DataType >
    struct S11NStdMapImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            _UInt32 size = sizeof(_UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                size += S11NTraits<ElemType >::Size(data, *it);
            return size;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            _UInt32 size = obj.size();
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<_UInt32>::Convert(size);
#endif
            _UInt32 len = 0;
            len += sizeof(_UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                len += S11NTraits<ElemType >::Serialize(data, (ElemType&)*it, name, mask);
            return len;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            if (data.Length() < sizeof(_UInt32))
                return false;
            _UInt32 size;
            data >> size;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<_UInt32>::Convert(size);
#endif
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::Unserialize(data, elem, name, mask))
                    return false;
                obj.insert(obj.end(), elem);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NStdContainerImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            _UInt32 size = sizeof(_UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                size += S11NTraits<ElemType >::Size(data, *it);
            return size;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            _UInt32 size = (_UInt32)obj.size();
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<_UInt32>::Convert(size);
#endif
            _UInt32 len = 0;
            len += sizeof(_UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                len += S11NTraits<ElemType >::Serialize(data, (ElemType&)*it, name, mask);
            return len;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            if (data.Length() < sizeof(_UInt32))
                return false;
            _UInt32 size;
            data >> size;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<_UInt32>::Convert(size);
#endif
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::Unserialize(data, elem, name, mask))
                    return false;
                obj.insert(obj.end(), elem);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameBufferImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            return sizeof(_UInt32) + obj.Length();
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            _UInt32 len = obj.Length();
            _UInt32 len1 = len;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            len1 = EndianReverse<_UInt32>::Convert(len);
#endif
            if (mask != 0)
            {
                len1 ^= (_UInt32)(mask + 0);
            }

            data << (_UInt32) len1;
            data += obj;
            return sizeof(_UInt32) + len;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            if (data.Length() < sizeof(_UInt32))
                return false;
            _UInt32 len;
            data >> len;

            if (mask != 0)
            {
                len ^= (_UInt32)(mask + 0);
            }

#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            len = EndianReverse<_UInt32>::Convert(len);
#endif
            if (data.Length() < len)
                return false;
            data.Read(obj, len);
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameBlobImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj.m_Len) + obj.m_Len;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            _UInt32 len = obj.m_Len;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            len = EndianReverse<_UInt32>::Convert(obj.m_Len);
#endif
            if (mask != 0)
            {
                len ^= (_UInt32)(mask + 0);
            }

            data << (_UInt32) len;
            data.Write(obj.m_Data, obj.m_Len);
            return sizeof(_UInt32) + obj.m_Len;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            if (data.Length() < sizeof(obj.m_Len))
                return false;

            data >> obj.m_Len;
            if (mask != 0)
            {
                obj.m_Len ^= (_UInt32)(mask + 0);
            }
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
             obj.m_Len = EndianReverse<_UInt32>::Convert(obj.m_Len);
#endif
             
             if (data.Length() < obj.m_Len)
                 return false;
             data.Read(obj.m_Data, obj.m_Len);
            return true;
        }
    };


    template <class DataType >
    struct S11NIGameListImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            _UInt32 size = sizeof(_UInt32);
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
                size += S11NTraits<ElemType >::Size(data, node->m_Value);
            return size;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            _UInt32 size = obj.Size();
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<_UInt32>::Convert(size);
#endif

            _UInt32 len = 0;
            len += sizeof(_UInt32);

            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
                len += S11NTraits<ElemType >::Serialize(data, node->m_Value, name, mask);
            return len;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            if (data.Length() < sizeof(_UInt32))
                return false;
            _UInt32 size;
            data >> size;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<_UInt32>::Convert(size);
#endif
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::Unserialize(data, elem, name, mask))
                    return false;
                obj.InsertTail(elem);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameMapImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::node_type NodeType;

            _UInt32 size = sizeof(_UInt32);
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
                size += S11NTraits<ElemType >::Size(data, std::make_pair(node->m_Key, node->m_Value));
            return size;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::node_type NodeType;

            _UInt32 size = obj.Size();
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<_UInt32>::Convert(size);
#endif

            _UInt32 len = 0;
            len += sizeof(_UInt32);

            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
                len += S11NTraits<ElemType >::Serialize(data, (const ElemType&)std::make_pair(node->m_Key, node->m_Value), name, mask);
            return len;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::node_type NodeType;

            if (data.Length() < sizeof(_UInt32))
                return false;
            _UInt32 size;
            data >> size;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<_UInt32>::Convert(size);
#endif
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::Unserialize(data, elem, name, mask))
                    return false;
                obj.Insert(elem.first, elem.second);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameSetImpls<DataType, typename EnableIf<IsIGameStreamBufferData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            _UInt32 size = sizeof(_UInt32);
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
                size += S11NTraits<ElemType >::Size(data, node->m_Value);
            return size;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            _UInt32 size = obj.Size();
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }
#if defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<_UInt32>::Convert(size);
#endif

            _UInt32 len = 0;
            len += sizeof(_UInt32);

            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
                len += S11NTraits<ElemType >::Serialize(data, node->m_Value, name, mask);
            return len;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            if (data.Length() < sizeof(_UInt32))
                return false;
            _UInt32 size;
            data >> size;
#if !defined (_IGAME_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<_UInt32>::Convert(size);
#endif
            if (mask != 0)
            {
                size ^= (_UInt32)(mask + 0);
            }

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::Unserialize(data, elem, name, mask))
                    return false;
                obj.Insert(elem);
            }
            return true;
        }
    };
} // namespace IGame

#endif // #ifndef IGAME_OS_STREAMBUFFER_SERIALIZE_IMPLS_H

