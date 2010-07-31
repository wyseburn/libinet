/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_WORD_FILTER_H
#define IGAME_OS_WORD_FILTER_H

#include "os/include/Compat.h"
#include "os/include/SingletonObject.h"
#include "os/include/Container.h"

namespace IGame
{
    class WordFilter
    {
    public:
        WordFilter() {}

        ~WordFilter() 
        {
            Clean(&m_Filter);
        }

        void AddWord(const char* word)
        {
            _UInt32 len = (_UInt32)strlen(word);
            Filter* filter = &m_Filter;

            for (_UInt32 i = 0; i < len; i++)
            {
                unsigned char c = word[i];
                if (i == len - 1)
                {
                    filter->m_NodeArray[c].m_Flag |= FilterNode::NODE_IS_END;
                    break;
                }
                else
                {
                    filter->m_NodeArray[c].m_Flag |= FilterNode::NODE_HAS_NEXT;
                }

                if (filter->m_NodeArray[c].m_NextFilter == NULL)
                {
                    Filter* tmpFilter = XNEW (Filter)();
                    filter->m_NodeArray[c].m_NextFilter = tmpFilter;
                }

                filter = (Filter *)filter->m_NodeArray[c].m_NextFilter;
            }
        }

        void AddWords(const std::set<std::string>& wordList)
        {
            for (std::set<std::string>::const_iterator it = wordList.begin();
                it != wordList.end(); it++)
            {
                AddWord(it->c_str());
            }
        }

        void AddWords(const std::vector<std::string>& wordList)
        {
            for (std::vector<std::string>::const_iterator it = wordList.begin();
                it != wordList.end(); it++)
            {
                AddWord(it->c_str());
            }
        }

        void AddWords(const IGame::Set<std::string>& worldList)
        {
            for (IGame::Set<std::string>::Node* node = worldList.Begin();
                node != worldList.End(); node = worldList.Next(node))
            {
                AddWord(node->m_Value.c_str());
            }
        }

        _Int32 Check(const char* str)
        {
            Filter* filter = NULL;
            for (_Int32 i = 0; i < (int)strlen(str) - 1; i++)
            {
                filter = &m_Filter;
                for (_UInt32 j = i; j < strlen(str); j++)
                {
                    unsigned char c = str[j]; 
                    if ((c >= 'A' && c <= 'Z'))
                    {
                        c += 32;
                    }

                    if (filter->m_NodeArray[c].m_Flag == FilterNode::NODE_IS_NULL)
                    {
                        break;
                    }
                    else if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_IS_END)
                    {
                        return i;
                    }
                    else // NODE_HAS_NEXT
                    {
                        filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
                    }
                }
            }
            return -1;
        }

        void CheckAndModify(char* str, const char replace = '*')
        {
            Filter* filter = NULL;
            for (_Int32 i = 0; i < (int)strlen(str) - 1; i++)
            {
                filter = &m_Filter;
                for (_UInt32 j = i; j < strlen(str); j++)
                {
                    unsigned char c = str[j]; 
                    if ((c >= 'A' && c <= 'Z'))
                    {
                        c += 32;
                    }

                    if (filter->m_NodeArray[c].m_Flag == FilterNode::NODE_IS_NULL)
                    {
                        break;
                    }
                    else if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_IS_END)
                    {
                        for (_UInt32 k = i; k <= j; k++)
                        {
                            str[k] = replace;
                        }

                        if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_HAS_NEXT)
                        {
                            filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else // NODE_HAS_NEXT
                    {
                        filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
                    }
                }
            }
        }

        void CheckAndModify(std::string& str, const char replace = '*')
        {
            Filter* filter = NULL;
            for (_Int32 i = 0; i < (int)str.size() - 1; i++)
            {
                filter = &m_Filter;
                for (_UInt32 j = i; j < str.size(); j++)
                {
                    unsigned char c = str[j]; 
                    if ((c >= 'A' && c <= 'Z'))
                    {
                        c += 32;
                    }
                    if (filter->m_NodeArray[c].m_Flag == FilterNode::NODE_IS_NULL)
                    {
                        break;
                    }
                    else if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_IS_END)
                    {
                        for (_UInt32 k = i; k <= j; k++)
                        {
                            str[k] = replace;
                        }

                        if (filter->m_NodeArray[c].m_Flag & FilterNode::NODE_HAS_NEXT)
                        {
                            filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else // NODE_HAS_NEXT
                    {
                        filter = (Filter*)filter->m_NodeArray[c].m_NextFilter;
                    }
                }
            }
        }

    private:

        struct FilterNode
        {
            char m_Flag;
            void* m_NextFilter;

            enum Flag
            {
                NODE_IS_NULL          = 0x00,
                NODE_HAS_NEXT         = 0x01,
                NODE_IS_END           = 0x10,
            };
            FilterNode() : m_Flag(NODE_IS_NULL), m_NextFilter(NULL) {}
        };

        struct Filter
        {
            FilterNode m_NodeArray[256];
        } m_Filter;

        void Clean(Filter* filter)
        {
            for (_UInt32 i = 0; i < 256; i++)
            {
                if (filter->m_NodeArray[i].m_NextFilter)
                {
                    Clean((Filter *)filter->m_NodeArray[i].m_NextFilter);
                    XDELETE((Filter*)filter->m_NodeArray[i].m_NextFilter);
                }
            }
        }
    };
} // namespace IGame


#endif // #ifndef IGAME_OS_WORD_FILTER_H



