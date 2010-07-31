/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_TINY_JSON_PARSER_H
#define IGAME_OS_TINY_JSON_PARSER_H

#include <stddef.h>
#include "os/include/TinyJson.h"

/* Determine the integer type use to parse non-floating point numbers */
#if __STDC_VERSION__ >= 199901L || HAVE_LONG_LONG == 1
typedef long long _JsonInt
typedef long long JSON_int_t;
#define JSON_PARSER_INTEGER_SSCANF_TOKEN "%lld"
#define JSON_PARSER_INTEGER_SPRINTF_TOKEN "%lld"
#else 
typedef long _JsonInt;
#define JSON_PARSER_INTEGER_SSCANF_TOKEN "%ld"
#define JSON_PARSER_INTEGER_SPRINTF_TOKEN "%ld"
#endif

#define JSON_PARSER_STACK_SIZE 2048
#define JSON_PARSER_PARSE_BUFFER_SIZE 2048

namespace IGame
{
    struct TinyJsonParser
    {
        static bool Parse(TiJsonNode& node, const std::string& data);

        struct Value
        {
            union
            {
                _JsonInt m_IntValue;
                long double m_FloatValue;
                struct 
                {
                    const char* m_Value;
                    _UInt32 m_Length;
                } m_StrValue;
            } m_Data;
        };

        struct Parser
        {
            Parser(TiJsonNode* node, _Int32 depth = -1);
            virtual ~Parser();

            TiJsonNode* m_Node;
            _Int8 m_State;
            _Int8 m_BeforeCommentState;
            _Int8 m_Type;
            _Int8 m_Escaped;
            _Int8 m_Comment;
            _Int8 m_AllowComments;
            _Int8 m_HandleFloatsManually;
            _UInt16 m_Utf16HighSurrogate;
            _Int32 m_Depth;
            _UInt32 m_Top;
            _Int8* m_Stack;
            _UInt32 m_StackCapacity;
            _Int8 m_DecimalPoint;
            _Int8* m_ParseBuffer;
            _UInt32 m_ParseBufferCapacity;
            _UInt32 m_ParseBufferCount;
            _UInt32 m_CommentBeginOffset;
            _Int8 m_StaticStack[JSON_PARSER_STACK_SIZE];
            _Int8 m_StaticParseBuffer[JSON_PARSER_PARSE_BUFFER_SIZE];
        };
    };
} // namespace IGame 

#endif // #ifndef IGAME_OS_TINY_JSON_PARSER_H



