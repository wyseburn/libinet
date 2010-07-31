/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_TINY_JSON_H
#define IGAME_OS_TINY_JSON_H

#include "os/include/Compat.h"
#include "os/include/Container.h"

#define IGAME_JSON_TYPE_NONE                 0x00
#define IGAME_JSON_TYPE_ARRAY                0x01
#define IGAME_JSON_TYPE_ARRAY_BEGIN          0x02
#define IGAME_JSON_TYPE_ARRAY_END            0x03
#define IGAME_JSON_TYPE_OBJECT               0x04
#define IGAME_JSON_TYPE_OBJECT_BEGIN         0x05
#define IGAME_JSON_TYPE_OBJECT_END           0x06
#define IGAME_JSON_TYPE_INTEGER              0x07
#define IGAME_JSON_TYPE_FLOAT                0x08
#define IGAME_JSON_TYPE_NULL                 0x09
#define IGAME_JSON_TYPE_TRUE                 0x0a
#define IGAME_JSON_TYPE_FALSE                0x0b
#define IGAME_JSON_TYPE_STRING               0x0c
#define IGAME_JSON_TYPE_KEY                  0x0d
#define IGAME_JSON_TYPE_MAX                  0x0e

namespace IGame
{
    struct TiJsonNode
    {
        TiJsonNode(_UInt8 type = IGAME_JSON_TYPE_OBJECT, 
            const std::string& key = "", const std::string& value = "")
            : m_Type(type), m_Key(key), m_Value(value), m_Parent(NULL), m_This(NULL)
        {
        }

        ~TiJsonNode()
        {
            Clean();
        }

        _UInt8 m_Type;
        std::string m_Key;
        std::string m_Value;
        TiJsonNode* m_Parent;
        List<TiJsonNode*>::Node* m_This;
        List<TiJsonNode*> m_Child;

        void Clean()
        {
            for (List<TiJsonNode*>::Node* node = m_Child.Begin(); node != m_Child.End();)
            {
                if (node && node->m_Value)
                {
                    node->m_Value->Clean();
                }
                XDELETE(node->m_Value);
                node = m_Child.Erase(node);
            }
        }

        _UInt8 Type() { return m_Type; }
        const std::string& Key() { return m_Key; }
        const std::string& Value() { return m_Value; }
        TiJsonNode* Parent() { return m_Parent; }
        TiJsonNode* FirstChild() 
        { 
            List<TiJsonNode*>::Node* node = m_Child.First();
            if (node != NULL)
            {
                return node->m_Value;
            }
            return NULL;
        }

        TiJsonNode* LastChild()
        {
            List<TiJsonNode*>::Node* node = m_Child.Last();
            if (node != NULL)
            {
                return node->m_Value;
            }
            return NULL;
        }

        TiJsonNode* NextChild(TiJsonNode* child)
        {
            if (child->m_This == NULL) return NULL;
            List<TiJsonNode*>::Node* node = m_Child.Next(child->m_This);
            if (node != NULL)
            {
                return node->m_Value;
            }
            return NULL;
        }

        TiJsonNode* GetChild(const std::string& key)
        {
            List<TiJsonNode*>::Node* node = m_Child.First();
            if (node != NULL)
            {
                do 
                { 
                    if (node->m_Value && node->m_Value->m_Key == key)
                    {
                        return node->m_Value;
                    }
                } while (node = m_Child.Next(node));
            }
            return NULL;
        }

        TiJsonNode* LinkChildToHead(_UInt8 type, const std::string& key, const std::string& value)
        {
            TiJsonNode* child = XNEW(TiJsonNode)(type, key, value);
            assert(child);

            child->m_This = m_Child.InsertHead(child);
            if (child->m_This != NULL)
            {
                child->m_Parent = this;
                return child;
            }
            return NULL;
        }

        TiJsonNode* LinkChildToHead(TiJsonNode* child)
        {
            child->m_This = m_Child.InsertHead(child);
            if (child->m_This != NULL)
            {
                child->m_Parent = this;
                return child;
            }
            return NULL;
        }

        TiJsonNode* LinkChildToTail(_UInt8 type, const std::string& key, const std::string& value)
        {
            TiJsonNode* child = XNEW(TiJsonNode)(type, key, value);
            assert(child);

            child->m_This = m_Child.InsertTail(child);
            if (child->m_This != NULL)
            {
                child->m_Parent = this;
                return child;
            }
            return NULL;
        }

        TiJsonNode* LinkChildToTail(TiJsonNode* child)
        {
            child->m_This = m_Child.InsertTail(child);
            if (child->m_This != NULL)
            {
                child->m_Parent = this;
                return child;
            }
            return NULL;
        };

        void ToString(std::string& str, const char prefix = '{', const char postfix = '}') const
        {
            if (prefix != 0) str += prefix;
            for (List<TiJsonNode*>::Node* node = m_Child.Begin(); 
                node != m_Child.End(); node = m_Child.Next(node))
            {
                if (node && node->m_Value)
                {
                    if (node->m_Value->m_Parent)
                    {
                        if (node->m_Value->m_Type == IGAME_JSON_TYPE_OBJECT && 
                            node->m_Value->m_Parent->m_Type == IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "{";
                        }
                    }
                    switch (node->m_Value->m_Type)
                    {
                    case IGAME_JSON_TYPE_INTEGER:
                        if (node->m_Value->m_Parent->m_Type != IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "\"";
                            str += node->m_Value->m_Key.c_str();
                            str += "\"";
                            str += ":";
                        }
                        str += node->m_Value->m_Value.c_str();
                        break;
                    case IGAME_JSON_TYPE_FLOAT:
                        if (node->m_Value->m_Parent->m_Type != IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "\"";
                            str += node->m_Value->m_Key.c_str();
                            str += "\"";
                            str += ":";
                        }
                        str += node->m_Value->m_Value.c_str();
                        break;
                    case IGAME_JSON_TYPE_TRUE:
                        if (node->m_Value->m_Parent->m_Type != IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "\"";
                            str += node->m_Value->m_Key.c_str();
                            str += "\"";
                            str += ":";
                        }
                        str += "true";
                        break;
                    case IGAME_JSON_TYPE_FALSE:
                        if (node->m_Value->m_Parent->m_Type != IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "\"";
                            str += node->m_Value->m_Key.c_str();
                            str += "\"";
                            str += ":";
                        }
                        str += "false";
                        break;
                    case IGAME_JSON_TYPE_STRING:
                        if (node->m_Value->m_Parent->m_Type != IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "\"";
                            str += node->m_Value->m_Key.c_str();
                            str += "\"";
                            str += ":";
                        }
                        str += "\"";
                        str += node->m_Value->m_Value.c_str();
                        str += "\"";
                        break;
                    case IGAME_JSON_TYPE_ARRAY:
                        if (node->m_Value->m_Parent->m_Type != IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "\"";
                            str += node->m_Value->m_Key.c_str();
                            str += "\":";
                        }
                        node->m_Value->ToString(str, '[', ']');
                        break;
                    case IGAME_JSON_TYPE_OBJECT:
                        if (node->m_Value->m_Parent->m_Type != IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "\"";
                            str += node->m_Value->m_Key.c_str();
                            str += "\":";
                            node->m_Value->ToString(str);
                        }
                        else
                        {
                            node->m_Value->ToString(str, 0, 0);
                        }
                        break;
                    default:
                        assert(0);
                    };
                    if (node->m_Value->m_Parent)
                    {
                        if (node->m_Value->m_Type == IGAME_JSON_TYPE_OBJECT && 
                            node->m_Value->m_Parent->m_Type == IGAME_JSON_TYPE_ARRAY)
                        {
                            str += "}";
                        }
                    }
                    str += ",";
                }
            }

            if (postfix != 0)
            {
                if (m_Child.Empty())
                {
                    str += postfix;
                }
                else
                {
                    str[str.length()-1] = postfix;
                }
            }
            else
            {
                if (!m_Child.Empty())
                {
                    str.resize(str.length() - 1);
                }
            }
        }
    };
} // namespace IGame 

#endif // #ifndef IGAME_OS_TINY_JSON_H



