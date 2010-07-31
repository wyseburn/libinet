/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_TINYXML_SERIALIZE_IMPLS_H
#define IGAME_OS_TINYXML_SERIALIZE_IMPLS_H

#include <string>
#include <sstream>

#include "os/include/TinyXml.h"
#include "os/include/SerializeImpls.h"

namespace IGame
{
    template <class Type> struct IsIGameTinyXmlData 
    { 
        static const bool s_Value = false; 
    };

    template <> struct IsIGameTinyXmlData<TiXmlElement> 
    { 
        static const bool s_Value = true; 
    };

    static const char* TinyXmlDataTypeBool      = "bool";
    static const char* TinyXmlDataTypeNumber    = "number";
    static const char* TinyXmlDataTypeString    = "string";
    static const char* TinyXmlDataTypePair      = "pair";
    static const char* TinyXmlDataTypeMap       = "map";
    static const char* TinyXmlDataTypeArray     = "array";
    static const char* TinyXmlDataTypeObject    = "object";

    static TiXmlElement* AllocTiXmlElement(const char* value)
    {
        return XNEW(TiXmlElement)(value);
    }

    static TiXmlElement* AllocTiXmlElement(TiXmlElement& parent, const char* value)
    {
        TiXmlElement* elem = XNEW(TiXmlElement)(value);
        parent.LinkEndChild(elem);
        return elem;
    }

    template <class DataType>
    struct ObjectInit<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type >
    {
        static DataType& Init(DataType& data, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return data;
            }

            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeObject);
            return *element;
        }
    };

    template <class DataType>
    struct ObjectUninit<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type >
    {
        static DataType* Uninit(DataType& data, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return &data;
            }
            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return NULL;
            TiXmlElement* element = node->ToElement();
            return element;
        }
    };

    template <class DataType >
    struct S11NBooleanImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeBool);
            element->SetAttribute("value", obj ? "true" : "false");
			return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            const char* p = element->Attribute("value");
            if (p == "") return false;
            obj = (strncasecmp(p, "true", 4)) ? false : true;
            return true;
        }
    };

    template <class DataType >
    struct S11NIntegerImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeNumber);
            char buf[128];
            Itoa<ObjType >(obj, buf, 128, 10);
            element->SetAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            const char* p = element->Attribute("value");
            if (p == "") return false;
            obj = Atoi<ObjType >(p);
            return true;
        }
    };

    template <class DataType >
    struct S11NFloatImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeNumber);
            std::stringstream tmpStr;  // TODO: performance optimized.
            tmpStr << obj;
            element->SetAttribute("value", tmpStr.str().c_str());
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            const char* p = element->Attribute("value");
            if (p == "") return false;
            std::stringstream tmpStr; // TODO: performance optimized.
            tmpStr << p;
            tmpStr >> obj;
            return true;
        }
    };

    template <class DataType >
    struct S11NDoubleImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeNumber);
            std::stringstream tmpStr;
            tmpStr << obj;
            element->SetAttribute("value", tmpStr.str().c_str());
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            const char* p = element->Attribute("value");
            if (p == "") return false;
            std::stringstream tmpStr; // TODO: performance optimized.
            tmpStr << p;
            tmpStr >> obj;
            return true;
        }
    };

    template <class DataType >
    struct S11NStdBasicStringImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeString);
            element->SetAttribute("value", obj.c_str()); // Don't support wstring.
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            const char* p = element->Attribute("value");
            if (p == "") return false;
            obj = p;
            return true;
        }
    };

    template <class DataType >
    struct S11NStdPairImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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

            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypePair);

            return S11NTraits<FirstType >::Serialize(*element, (FirstType&)obj.first, "first", mask) 
                + S11NTraits<SecondType >::Serialize(*element, (SecondType&)obj.second, "second", mask);
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();

            return S11NTraits<FirstType >::Unserialize(*element, obj.first, "first", mask) &&
                S11NTraits<SecondType >::Unserialize(*element, obj.second, "second", mask);
        }
    };

    template <class DataType >
    struct S11NStdMapImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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

            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeMap);
            element->SetAttribute("size", obj.size());

            _UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*element, (ElemType&)*it, buf, mask);
                i++;
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

            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            _UInt32 size = Atoi<_UInt32>(element->Attribute("size"));
            _UInt32 i = 0;

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*element, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NStdContainerImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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

            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeArray);
            element->SetAttribute("size", obj.size());

            _UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*element, (ElemType&)*it, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            _UInt32 size = Atoi<_UInt32>(element->Attribute("size"));
            _UInt32 i = 0;

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*element, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameBufferImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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
    struct S11NIGameBlobImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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
    struct S11NIGameListImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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

            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeArray);
            element->SetAttribute("size", obj.Size());

            _UInt32 i = 0;
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*element, (ElemType&)node->m_Value, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            _UInt32 size = Atoi<_UInt32>(element->Attribute("size"));
            _UInt32 i = 0;

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*element, elem,  buf, mask))
                    return false;
                obj.InsertTail(elem);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameMapImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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

            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeMap);
            element->SetAttribute("size", obj.Size());

            _UInt32 i = 0;
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*element, (const ElemType&)std::make_pair(node->m_Key, node->m_Value), buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;

            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            _UInt32 size = Atoi<_UInt32>(element->Attribute("size"));
            _UInt32 i = 0;

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*element, elem,  buf, mask))
                    return false;
                obj.Insert(elem.first, elem.second);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NIGameSetImpls<DataType, typename EnableIf<IsIGameTinyXmlData<DataType >::s_Value>::type>
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

            TiXmlElement* element = IGame::AllocTiXmlElement(data, name);
            element->SetAttribute("type", TinyXmlDataTypeArray);
            element->SetAttribute("size", obj.Size());

            _UInt32 i = 0;
            for (NodeType* node = obj.Begin(); node != obj.End(); node = obj.Next(node))
            {
                char buf[128];
                Itoa(i, buf, 128, 10);
                S11NTraits<ElemType >::Serialize(*element, (ElemType&)node->m_Value, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::node_type NodeType;

            TiXmlNode* node = data.FirstChild(name);
            if (node == NULL) return false;
            TiXmlElement* element = node->ToElement();
            _UInt32 size = Atoi<_UInt32>(element->Attribute("size"));
            _UInt32 i = 0;

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                Itoa(i, buf, 128, 10);

                if (!S11NTraits<ElemType >::Unserialize(*element, elem,  buf, mask))
                    return false;
                obj.Insert(elem);
                i++;
            }
            return true;
        }
    };

} // namespace IGame

#endif // #ifndef IGAME_OS_TINYXML_SERIALIZE_IMPLS_H


