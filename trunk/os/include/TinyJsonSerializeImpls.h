/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_TINYJSON_SERIALIZE_IMPLS_H
#define IGAME_OS_TINYJSON_SERIALIZE_IMPLS_H

#include <string>
#include <sstream>

#include "os/include/TinyJsonParser.h"
#include "os/include/SerializeImpls.h"

namespace IGame
{
    template <class Type> struct IsIGameTinyJsonData 
    { 
        static const bool s_Value = false; 
    };

    template <> struct IsIGameTinyJsonData<TiJsonNode> 
    { 
        static const bool s_Value = true; 
    };

    template <class DataType>
    struct ObjectInit<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type >
    {
        static DataType& Init(DataType& data, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return data;
            }

            DataType* child = data.LinkChildToTail(IGAME_JSON_TYPE_OBJECT, name, "");
            assert(child);
            return *child;
        }
    };

    template <class DataType>
    struct ObjectUninit<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type >
    {
        static DataType* Uninit(DataType& data, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return &data;
            }
            DataType* child = data.GetChild(name);
            if (child == NULL) return NULL;
            return child;
        }
    };

    template <class DataType >
    struct S11NBooleanImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            _Int8 type = obj ? IGAME_JSON_TYPE_TRUE : IGAME_JSON_TYPE_FALSE;
            std::string value = obj ? "true" : "false";
            data.LinkChildToTail(type, name, value);
			return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
            obj = (strncasecmp(subData->m_Value.c_str(), "true", 4)) ? false : true;
            return true;
        }
    };

    template <class DataType >
    struct S11NIntegerImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            char buf[128];
            Itoa<ObjType >(obj, buf, 128, 10);
            data.LinkChildToTail(IGAME_JSON_TYPE_INTEGER, name, buf);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
            obj = Atoi<ObjType >(subData->m_Value.c_str());
            return true;
        }
    };

    template <class DataType >
    struct S11NFloatImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            char buf[128];
            sprintf(buf, "%f", obj);
            data.LinkChildToTail(IGAME_JSON_TYPE_FLOAT, name, buf);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
            std::stringstream tmpStr;
            tmpStr << subData->m_Value.c_str();
            tmpStr >> obj;
            return true;
        }
    };

    template <class DataType >
    struct S11NDoubleImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            std::stringstream tmpStr;
            tmpStr << obj;
            tmpStr >> data.m_Value;
            data.LinkChildToTail(IGAME_JSON_TYPE_FLOAT, name, tmpStr.str().c_str());
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
            std::stringstream tmpStr;
            tmpStr << subData->m_Value.c_str();
            tmpStr >> obj;
            return true;
        }
    };

    template <class DataType >
    struct S11NStdBasicStringImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            data.LinkChildToTail(IGAME_JSON_TYPE_STRING, name, obj);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
            obj = subData->m_Value;
            return true;
        }
    };

    template <class DataType >
    struct S11NStdPairImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;
            DataType* subData = data.LinkChildToTail(IGAME_JSON_TYPE_OBJECT, name, "");

            _Int8 tmpBuf[2048];
            Itoa(obj.first, tmpBuf, 2048, 10);
            return S11NTraits<SecondType >::Serialize(*subData, (SecondType&)obj.second, tmpBuf, mask);
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;
            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;

            return S11NTraits<SecondType >::Unserialize(*subData, obj.second, subData->m_Child.Begin()->m_Value->m_Key.c_str(), mask);
        }
    };

    template <class DataType >
    struct S11NStdMapImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            DataType* subData = data.LinkChildToTail(IGAME_JSON_TYPE_OBJECT, name, "");
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                _Int8 tmpBuf[2048];
                Itoa(it->first, tmpBuf, 2048, 10);
                S11NTraits<MappedType >::Serialize(*subData, (MappedType&)it->second, tmpBuf, mask);
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
           
            obj.clear();
            DataType* subSubData = subData->FirstChild();
            if (subSubData == NULL) return true;  // empty container

            do 
            {
                ElemType elem;
                if (!S11NTraits<MappedType >::Unserialize(*subData, elem.second, subData->m_Child.Begin()->m_Value->m_Key.c_str(), mask))
                    return false;
                obj.insert(obj.end(), elem);
            } while (subSubData = subData->NextChild(subSubData));
            return true;
        }
    };

    template <class DataType >
    struct S11NStdContainerImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            DataType* subData = data.LinkChildToTail(IGAME_JSON_TYPE_ARRAY, name, "");

            _UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*subData, (ElemType&)*it, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
           
            _UInt32 i = 0;
            obj.clear();

            DataType* subSubData = subData->FirstChild();
            if (subSubData == NULL) return true;  // empty container

            do 
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*subData, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            } while (subSubData = subData->NextChild(subSubData));
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameBufferImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameBlobImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameListImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            DataType* subData = data.LinkChildToTail(IGAME_JSON_TYPE_ARRAY, name, "");

            _UInt32 i = 0;
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*subData, (ElemType&)node->m_Value, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
           
            _UInt32 i = 0;
            obj.Clear();

            DataType* subSubData = subData->FirstChild();
            if (subSubData == NULL) return true;  // empty container

            do 
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*subData, elem,  buf, mask))
                    return false;
                obj.InsertTail(elem);
                i++;
            } while (subSubData = subData->NextChild(subSubData));
            return true; 
        }
    };

    template <class DataType >
    struct S11NIGameMapImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::node_type NodeType;

            DataType* subData = data.LinkChildToTail(IGAME_JSON_TYPE_OBJECT, name, "");
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
            {
                _Int8 tmpBuf[2048];
                Itoa(node->m_Key, tmpBuf, 2048, 10);
                S11NTraits<MappedType >::Serialize(*subData, (MappedType&)node->m_Value, tmpBuf, mask);
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;

            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
           
            obj.Clear();
            DataType* subSubData = subData->FirstChild();
            if (subSubData == NULL) return true;  // empty container

            do 
            {
                ElemType elem;
                if (!S11NTraits<MappedType >::Unserialize(*subData, elem.second, subData->m_Child.Begin()->m_Value->m_Key.c_str(), mask))
                    return false;
                obj.Insert(elem.first, elem.second);
            } while (subSubData = subData->NextChild(subSubData));
            return true; 
        }
    };

    template <class DataType >
    struct S11NIGameSetImpls<DataType, typename EnableIf<IsIGameTinyJsonData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            DataType* subData = data.LinkChildToTail(IGAME_JSON_TYPE_ARRAY, name, "");

            _UInt32 i = 0;
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*subData, (ElemType&)node->m_Value, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            DataType* subData = data.GetChild(name);
            if (subData == NULL) return false;
           
            _UInt32 i = 0;
            obj.Clear();

            DataType* subSubData = subData->FirstChild();
            if (subSubData == NULL) return true;  // empty container

            do 
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*subData, elem,  buf, mask))
                    return false;
                obj.Insert(elem);
                i++;
            } while (subSubData = subData->NextChild(subSubData));
            return true; 
        }
    };

} // namespace IGame

#endif // #ifndef IGAME_OS_TINYJSON_SERIALIZE_IMPLS_H


