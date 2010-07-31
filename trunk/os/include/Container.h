/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_CONTAINTER_H
#define IGAME_OS_CONTAINTER_H

#include "os/include/Compat.h"
#include "os/include/DataStruct.h"
#include "os/include/SingletonObject.h"

namespace IGame
{
    template <typename ValueType >
    class List
    {
    public:
        typedef ValueType value_type;

        List() : m_NodeCount(0)
        {
            IGAME_DLIST_INIT(&m_List);
        }

        List(const List<ValueType >& rhs)
        {
            IGAME_DLIST_INIT(&m_List);
            m_NodeCount = 0;
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                InsertTail(node->m_Value);
            }
        }

        ~List()
        {
            this->Clear();
        }

        struct Node
        {
            Node() {};
            Node(const ValueType& value) : m_Value(value) {}

            struct Entries
            {
                struct Node* m_Next;
                struct Node** m_Prev;
            };

            ValueType m_Value;
            Entries m_Entries;
        };

        typedef Node node_type;

        struct _List
        {
            struct Node* m_First;
            struct Node** m_Last;
        };

        _List m_List;
        unsigned int m_NodeCount;

        Node* AllocNode()
        {
            return XNEW(Node) ();
        }

        Node* AllocNode(const ValueType& value)
        {
            return XNEW(Node)(value);
        }

        void DeallocNode(Node* node)
        {
            if (node == NULL) return;
            SAFE_DELETE(node);
        }

        bool Empty() const
        {
            return !Begin();
        }

        unsigned int Size() const
        {
            return m_NodeCount;
        }

        void Clear()
        {
            Node* node;
            while (node = IGAME_DLIST_FIRST(&m_List))
            {
                IGAME_DLIST_REMOVE(&m_List, node, m_Entries);
                SAFE_DELETE(node);
            }
        }

        Node* First()
        {
            return Begin();
        }

        Node* Last()
        {
            return IGAME_DLIST_LAST(&m_List, _List);
        }

        void InsertHead(Node* node)
        {
            assert(node);
            IGAME_DLIST_INSERT_HEAD(&m_List, node, m_Entries);
            m_NodeCount++;
        }

        Node* InsertHead(const ValueType& value)
        {
            Node* node = XNEW(Node) (value);
            InsertHead(node);
            return node;
        }

        void InsertTail(Node* node)
        {
            assert(node);
            IGAME_DLIST_INSERT_TAIL(&m_List, node, m_Entries);
            m_NodeCount++;
        }

        Node* InsertTail(const ValueType& value)
        {
            Node* node = XNEW(Node) (value);
            InsertTail(node);
            return node;
        }

        void InsertAfter(Node* listNode, Node* node)
        {
            assert(node && listNode);
            IGAME_DLIST_INSERT_AFTER(&m_List, listNode, node, m_Entries);
            m_NodeCount++;
        }

        Node* InsertAfter(const ValueType& value, Node* listNode)
        {
            Node* node = XNEW(Node)(value);
            InsertAfter(listNode, node);
            return node;
        }

        void InsertBefore(Node* listNode, Node* node)
        {
            assert(listNode && node);
            IGAME_DLIST_INSERT_BEFORE(listNode, node, m_Entries);
            m_NodeCount++;
        }

        Node* InsertBefore(const ValueType& value, Node* listNode)
        {
            Node* node = XNEW(Node) (value);
            InsertBefore(listNode, node);
            return node;
        }

        Node* Remove(Node* node) // Don't dealloc memory
        {
            if (node == NULL) return NULL;
            Node* nextNode = Next(node);
            IGAME_DLIST_REMOVE(&m_List, node, m_Entries);
            m_NodeCount--;
            return nextNode;
        }

        Node* Erase(Node* node) // Dealloc memory
        {
            Node* nextNode = Remove(node);
            XDELETE(node);
            return nextNode;
        }

        Node* Begin() const
        {
            return IGAME_DLIST_FIRST(&m_List);
        }

        Node* End() const
        {
            return NULL;
        }

        Node* Next(Node* node) const
        {
            if (node == NULL) return NULL;
            return IGAME_DLIST_NEXT(node, m_Entries);
        }

        Node* GetN(_UInt32 index) const
        {
            _UInt32 i = 0;
            for (Node* node = Begin(); node != End(); node = Next(node))
            {
                if (i++ == index)
                {
                    return node;
                }
            }
            return NULL;
        }

        List<ValueType >& operator = (const List<ValueType >& rhs)
        {
            Clear();
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                InsertTail(node->m_Value);
            }
            return *this;
        }
    };

    template <class Type> struct IsIGameList { static const bool s_Value = false; };
    template <class ValueType >
    struct IsIGameList<IGame::List<ValueType > > { static const bool s_Value = true; };

    template <typename KeyType, typename ValueType >
    class Map
    {
    public:
        typedef KeyType key_type;
        typedef ValueType mapped_type;
        
        Map() : m_NodeCount(0)
        {
            IGAME_RB_INIT(&m_Tree);
        }

        Map(const Map<KeyType, ValueType >& rhs)
        {
            IGAME_RB_INIT(&m_Tree);
            m_NodeCount = 0;
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Insert(node->m_Key, node->m_Value);
            }
        }

        ~Map()
        {
            this->Clear();
        }

        struct Node
        {
            Node(const KeyType& key, const ValueType& value) : m_Key(key), m_Value(value) {}
            Node(const KeyType& key) : m_Key(key) {}

            struct Entries {
                struct Node* rbe_left;
                struct Node* rbe_right;
                struct Node* rbe_parent;
                int rbe_color;
            };

            KeyType m_Key;
            ValueType m_Value;
            Entries m_Entries;
        };

        typedef Node node_type;

        struct _Map
        {
            struct Node* rbh_root;
        };

        _Map m_Tree;
        unsigned int m_NodeCount;

        int Compare(Node* a, Node* b)
        {
            if (a->m_Key > b->m_Key) return 1;
            if (a->m_Key < b->m_Key) return -1;
            return 0;
        }

        IGAME_RB_GENERATE(_Map, Node, m_Entries, Compare);

        bool Empty() const
        {
            return !Begin();
        }

        unsigned int Size() const
        {
            return m_NodeCount;
        }

        void Clear()
        {
            Node* node = Begin();
            if (node != NULL)
            {
                while (node = Erase(node));
            }
        }

        Node* Insert(const KeyType& key, const ValueType& value)
        {
            Node* node = XNEW(Node) (key, value);
            assert(node);
            Node* tmpNode = IGAME_RB_INSERT(_Map, &m_Tree, node);
            if (tmpNode != NULL)
            {
                SAFE_DELETE(node);
                tmpNode->m_Value = value;
                return tmpNode;
            }

            m_NodeCount++;
            return node;
        }

        void PushNode(Node* node)
        {
            assert(node);
            Node* tmpNode = IGAME_RB_INSERT(_Map, &m_Tree, node);
            if (tmpNode != NULL)
            {
                IGAME_RB_REMOVE(_Map, &m_Tree, tmpNode);
                SAFE_DELETE(tmpNode);
                IGAME_RB_INSERT(_Map, &m_Tree, node);
            }
            else
            {
                m_NodeCount++;
            }
        }

        void Erase(const KeyType& key)
        {
            Node tmpNode(key);
            Node* node = IGAME_RB_FIND(_Map, &m_Tree, &tmpNode);
            if (node != NULL)
            {
                IGAME_RB_REMOVE(_Map, &m_Tree, node);
                SAFE_DELETE(node);
                m_NodeCount--;
            }
        }

        Node* PopNode(const KeyType& key) // Don't dealloc memory
        {
            Node tmpNode(key);
            Node* node = IGAME_RB_FIND(_Map, &m_Tree, &tmpNode);
            if (node != NULL)
            {
                IGAME_RB_REMOVE(_Map, &m_Tree, node);
                //SAFE_DELETE(node);
                m_NodeCount--;
                return node;
            }
            return NULL;
        }

        Node* Erase(Node* node)
        {
            Node* nextNode = IGAME_RB_NEXT(_Map, &m_Tree, node);
            IGAME_RB_REMOVE(_Map, &m_Tree, node);
            SAFE_DELETE(node);
            m_NodeCount--;
            return nextNode;
        }

        Node* Find(const KeyType& key)
        {
            Node tmpNode(key);
            return IGAME_RB_FIND(_Map, &m_Tree, &tmpNode);
        }

        Node* Begin() const
        {
            return IGAME_RB_MIN(_Map, &m_Tree);
        }

        Node* End() const
        {
            return NULL;
        }

        Node* Next(Node* node) const
        {
            if (node == NULL) return NULL;
            return IGAME_RB_NEXT(_Map, &m_Tree, node);
        }

        Node* Min() const
        {
            return IGAME_RB_MIN(_Map, &m_Tree);
        }

        Node* Max() const
        {
             return IGAME_RB_MAX(_Map, &m_Tree);
        }

        Node* GetN(_UInt32 index) const
        {
            _UInt32 i = 0;
            for (Node* node = Begin(); node != End(); node = Next(node))
            {
                if (i++ == index)
                {
                    return node;
                }
            }
            return NULL;
        }

        ValueType& operator [] (const KeyType& key)
        {
            Node tmpNode(key);
            Node* node = IGAME_RB_FIND(_Map, &m_Tree, &tmpNode);
            if (node == NULL)
            {
                node = XNEW(Node)(key);
                assert(node);
                IGAME_RB_INSERT(_Map, &m_Tree, node);
                m_NodeCount++;
            }
            return node->m_Value;
        }

        Map<KeyType, ValueType >& operator = (const Map<KeyType, ValueType >& rhs)
        {
            Clear();
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Insert(node->m_Key, node->m_Value);
            }
            return *this;
        }

        Map<KeyType, ValueType > operator + (const Map<KeyType, ValueType >& rhs)
        {
            Map<KeyType, ValueType > ret = *this;
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                ret.Insert(node->m_Key, node->m_Value);
            }
            return ret;
        }

        Map<KeyType, ValueType >& operator += (const Map<KeyType, ValueType >& rhs)
        {
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Insert(node->m_Key, node->m_Value);
            }
            return *this;
        }

        Map<KeyType, ValueType > operator - (const Map<KeyType, ValueType >& rhs)
        {
            Map<KeyType, ValueType > ret = *this;
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Node* tmpNode = ret.Find(node->m_Key);
                if (tmpNode)
                {
                    ret.Erase(tmpNode);
                }
            }
            return ret;
        }

        Map<KeyType, ValueType >& operator -= (const Map<KeyType, ValueType >& rhs)
        {
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Node* tmpNode = Find(node->m_Key);
                if (tmpNode)
                {
                    Erase(tmpNode);
                }
            }
            return *this;
        }
    };

    template <class Type> struct IsIGameMap { static const bool s_Value = false; };
    template <class KeyType, class ValueType >
    struct IsIGameMap<IGame::Map<KeyType, ValueType > > { static const bool s_Value = true; };

    template <typename ValueType >
    class Set
    {
    public:
        typedef ValueType value_type;
        
        Set() : m_NodeCount(0)
        {
            IGAME_RB_INIT(&m_Tree);
        }

        Set(const Set<ValueType >& rhs)
        {
            IGAME_RB_INIT(&m_Tree);
            m_NodeCount = 0;
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Insert(node->m_Value);
            }
        }

        ~Set()
        {
            this->Clear();
        }

        struct Node
        {
            Node(const ValueType& value) : m_Value(value) {}

            struct Entries {
                struct Node* rbe_left;
                struct Node* rbe_right;
                struct Node* rbe_parent;
                int rbe_color;
            };

            ValueType m_Value;
            Entries m_Entries;
        };

        typedef Node node_type;

        struct _Set
        {
            struct Node* rbh_root;
        };

        _Set m_Tree;
        unsigned int m_NodeCount;

        int Compare(Node* a, Node* b)
        {
            if (b->m_Value < a->m_Value) return 1;
            if (a->m_Value < b->m_Value) return -1;
            return 0;
        }

        IGAME_RB_GENERATE(_Set, Node, m_Entries, Compare);

        bool Empty() const
        {
            return !Begin();
        }

        unsigned int Size() const
        {
            return m_NodeCount;
        }

        void Clear()
        {
            Node* node = Begin();
            if (node != NULL)
            {
                while (node = Erase(node));
            }
        }

        void Insert(const ValueType& value)
        {
            Node* node = XNEW(Node) (value);
            assert(node);
            Node* tmpNode = IGAME_RB_INSERT(_Set, &m_Tree, node);
            if (tmpNode != NULL)
            {
                SAFE_DELETE(node);
                tmpNode->m_Value = value;
            }
            else
            {
                m_NodeCount++;
            }
        }

        void Erase(const ValueType& value)
        {
            Node tmpNode(value);
            Node* node = IGAME_RB_FIND(_Set, &m_Tree, &tmpNode);
            if (node != NULL)
            {
                IGAME_RB_REMOVE(_Set, &m_Tree, node);
                SAFE_DELETE(node);
                m_NodeCount--;
            }
        }

        Node* Erase(Node* node)
        {
            Node* nextNode = IGAME_RB_NEXT(_Set, &m_Tree, node);
            IGAME_RB_REMOVE(_Set, &m_Tree, node);
            SAFE_DELETE(node);
            m_NodeCount--;
            return nextNode;
        }

        Node* Find(const ValueType& value)
        {
            Node tmpNode(value);
            return IGAME_RB_FIND(_Set, &m_Tree, &tmpNode);
        }

        Node* Begin() const
        {
            return IGAME_RB_MIN(_Set, &m_Tree);
        }

        Node* End() const
        {
            return NULL;
        }

        Node* Next(Node* node) const
        {
            if (node == NULL) return NULL;
            return IGAME_RB_NEXT(_Set, &m_Tree, node);
        }

        Node* GetN(_UInt32 index) const
        {
            _UInt32 i = 0;
            for (Node* node = Begin(); node != End(); node = Next(node))
            {
                if (i++ == index)
                {
                    return node;
                }
            }
            return NULL;
        }

        ValueType& operator [] (const ValueType& value)
        {
            Node tmpNode(value);
            Node* node = IGAME_RB_FIND(_Set, &m_Tree, &tmpNode);
            if (node == NULL)
            {
                node = XNEW(Node)(value);
                assert(node);
                IGAME_RB_INSERT(_Set, &m_Tree, node);
                m_NodeCount++;
            }
            return node->m_Value;
        }

        Set<ValueType >& operator = (const Set<ValueType >& rhs)
        {
            Clear();
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Insert(node->m_Value);
            }
            return *this;
        }

        Set<ValueType > operator + (const Set<ValueType >& rhs)
        {
            Set<ValueType > ret = *this;
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                ret.Insert(node->m_Value);
            }
            return ret;
        }

        Set<ValueType >& operator += (const Set<ValueType >& rhs)
        {
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Insert(node->m_Value);
            }
            return *this;
        }

        Set<ValueType > operator - (const Set<ValueType >& rhs)
        {
            Set<ValueType > ret = *this;
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Node* tmpNode = ret.Find(node->m_Value);
                if (tmpNode)
                {
                    ret.Erase(tmpNode);
                }
            }
            return ret;
        }

        Set<ValueType >& operator -= (const Set<ValueType >& rhs)
        {
            for (Node* node = rhs.Begin(); node != rhs.End(); node = rhs.Next(node))
            {
                Node* tmpNode = Find(node->m_Value);
                if (tmpNode)
                {
                    Erase(tmpNode);
                }
            }
            return *this;
        }
    };

    template <class Type> struct IsIGameSet { static const bool s_Value = false; };
    template <class ValueType >
    struct IsIGameSet<IGame::Set<ValueType > > { static const bool s_Value = true; };

    const _UInt16 IGameArraySize = 65530;

    template <typename PointerType >
    class PointerArray
    {
    public:
        PointerArray()
        {
            for (_UInt32 i = 0; i < IGameArraySize; i++)
            {
                m_Array[i] = NULL;
            }
            m_MaxIndex = 0;
        }

        ~PointerArray()
        {
        }

        _UInt16 GetMaxIndex()
        {
            return m_MaxIndex;
        }

        _UInt16 Insert(PointerType value)
        {
            for (_UInt16 i = 1; i < IGameArraySize; i++)
            {
                if (m_Array[i] == NULL)
                {
                    m_Array[i] = value;
                    if (i > m_MaxIndex)
                    {
                        m_MaxIndex = i;
                    }
                    return i;
                }
            }
            return 0;
        }

        bool Insert(_UInt16 key, PointerType value)
        {
            m_Array[key] = value;
            if (key > m_MaxIndex) m_MaxIndex = key;
            return true;
        }

        PointerType Erase(_UInt16 key)
        {
            PointerType ret = m_Array[key];
            m_Array[key] = NULL;
            if (key == m_MaxIndex)
            {
                _UInt16 i = m_MaxIndex;
                for (; i > 1 && i <= IGameArraySize; i--)
                {
                    if (m_Array[i] != NULL) 
                    {
                        m_MaxIndex = i;
                        return ret;
                    }
                }
                m_MaxIndex = i;
            }
            return ret;
        }

        PointerType Find(_UInt32 key)
        {
            return m_Array[key];
        }

        std::vector<PointerType> ToVector()
        {
            std::vector<PointerType> ret;
            for (_UInt16 i = 0; i <= m_MaxIndex; i++)
            {
                if (m_Array[i] != NULL)
                {
                    ret.push_back(m_Array[i]);
                }
            }
            return ret;
        }

        PointerType operator [] (_UInt16 key)
        {
            return m_Array[key];
        }
     private:
        PointerType m_Array[IGameArraySize+1];
        _UInt16 m_MaxIndex;
    };

    class TireTree
    {
    public:
        TireTree(){}

        ~TireTree() 
        {
            Clean(&m_Tree);
        }

        enum Flag
        {
            BYTE_IS_NULL          = 0x00,
            BYTE_HAS_NEXT         = 0x01,
            BYTE_IS_END           = 0x10,
        };

        struct Byte
        {
            _UInt8 m_Flag;
            _UInt32 m_Key;
            void* m_Next;
            Byte() : m_Flag(BYTE_IS_NULL), m_Key(0), m_Next(NULL) {}
        };

        typedef Map<_UInt8, Byte> TireTreeMap;
        TireTreeMap m_Tree;

        // Construct dictionary, key == 0 indicate null
        void AddWord(const char* word, _UInt32 key = 0)
        {
            TireTreeMap* tree = &m_Tree;
            _Int32 len = (_Int32)strlen(word);

            for (_Int32 i = 0; i < len; i++)
            {
                unsigned char c = word[i];
                TireTreeMap::Node* node = tree->Find(c);
                if (node == NULL)
                {
                    Byte byte;
                    node = tree->Insert(c, byte);
                }

                if (i == len - 1)
                {
                    node->m_Value.m_Flag |= BYTE_IS_END;
                    node->m_Value.m_Key = key;
                    break;
                }
                else
                {
                    node->m_Value.m_Flag |= BYTE_HAS_NEXT;
                }

                if (node->m_Value.m_Next == NULL)
                {
                    TireTreeMap* subTree = XNEW(TireTreeMap)();
                    node->m_Value.m_Next = subTree;
                }

                tree = (TireTreeMap*)node->m_Value.m_Next;
            }
        }

        void DelWord(const char* word)
        {
            TireTreeMap* tree = &m_Tree;
            _Int32 len = (_Int32)strlen(word);

            for (_Int32 i = 0; i < len; i++)
            {
                if (tree == NULL) return;

                unsigned char c = word[i];
                TireTreeMap::Node* node = tree->Find(c);
                if (node == NULL)
                {
                    return;
                }

                if (i == len - 1 && (node->m_Value.m_Flag & BYTE_IS_END))
                {
                    node->m_Value.m_Flag &= ~BYTE_IS_END;
                    node->m_Value.m_Key = 0;
                    return;
                }
                tree = (TireTreeMap*)node->m_Value.m_Next;
            }
        }

        // Fast string find
        _UInt32 GetWord(const char* word)
        {
            TireTreeMap* tree = &m_Tree;
            _Int32 len = (_Int32)strlen(word);

            for (_Int32 i = 0; i < len; i++)
            {
                if (tree == NULL) return 0;

                unsigned char c = word[i];
                TireTreeMap::Node* node = tree->Find(c);
                if (node == NULL)
                {
                    return 0;
                }

                if (i == len - 1 && (node->m_Value.m_Flag & BYTE_IS_END))
                {
                    return node->m_Value.m_Key;
                }

                tree = (TireTreeMap*)node->m_Value.m_Next;
            }
            return 0;
        }

        void Clean(TireTreeMap* tree)
        {
            if (tree == NULL) return;
            for (TireTreeMap::Node* node = tree->Begin(); node != tree->End();)
            {
                Clean((TireTreeMap*)node->m_Value.m_Next);
                XDELETE((TireTreeMap*)node->m_Value.m_Next);
                node = tree->Erase(node);
            }
        }
    };

} // namespace IGame

#endif // #ifndef IGAME_OS_CONTAINTER_H




