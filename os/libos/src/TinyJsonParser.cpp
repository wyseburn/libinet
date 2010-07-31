/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sstream>
#include "os/include/TinyJsonParser.h"

using namespace IGame;

#ifdef _MSC_VER
#   if _MSC_VER >= 1400 /* Visual Studio 2005 and up */
#      pragma warning(disable:4996) // unsecure sscanf
#   endif
#endif


#define true  1
#define false 0
#define __   -1     /* the universal error code */

#define COUNTOF(x) (sizeof(x)/sizeof(x[0])) 

enum classes {
    C_SPACE,  /* space */
    C_WHITE,  /* other whitespace */
    C_LCURB,  /* {  */
    C_RCURB,  /* } */
    C_LSQRB,  /* [ */
    C_RSQRB,  /* ] */
    C_COLON,  /* : */
    C_COMMA,  /* , */
    C_QUOTE,  /* " */
    C_BACKS,  /* \ */
    C_SLASH,  /* / */
    C_PLUS,   /* + */
    C_MINUS,  /* - */
    C_POINT,  /* . */
    C_ZERO ,  /* 0 */
    C_DIGIT,  /* 123456789 */
    C_LOW_A,  /* a */
    C_LOW_B,  /* b */
    C_LOW_C,  /* c */
    C_LOW_D,  /* d */
    C_LOW_E,  /* e */
    C_LOW_F,  /* f */
    C_LOW_L,  /* l */
    C_LOW_N,  /* n */
    C_LOW_R,  /* r */
    C_LOW_S,  /* s */
    C_LOW_T,  /* t */
    C_LOW_U,  /* u */
    C_ABCDF,  /* ABCDF */
    C_E,      /* E */
    C_ETC,    /* everything else */
    C_STAR,   /* * */   
    NR_CLASSES
};

static int ascii_class[128] = {
/*
    This array maps the 128 ASCII characters into character classes.
    The remaining Unicode characters should be mapped to C_ETC.
    Non-whitespace control characters are errors.
*/
    __,      __,      __,      __,      __,      __,      __,      __,
    __,      C_WHITE, C_WHITE, __,      __,      C_WHITE, __,      __,
    __,      __,      __,      __,      __,      __,      __,      __,
    __,      __,      __,      __,      __,      __,      __,      __,

    C_SPACE, C_ETC,   C_QUOTE, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_STAR,   C_PLUS,  C_COMMA, C_MINUS, C_POINT, C_SLASH,
    C_ZERO,  C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
    C_DIGIT, C_DIGIT, C_COLON, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,

    C_ETC,   C_ABCDF, C_ABCDF, C_ABCDF, C_ABCDF, C_E,     C_ABCDF, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LSQRB, C_BACKS, C_RSQRB, C_ETC,   C_ETC,

    C_ETC,   C_LOW_A, C_LOW_B, C_LOW_C, C_LOW_D, C_LOW_E, C_LOW_F, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_LOW_L, C_ETC,   C_LOW_N, C_ETC,
    C_ETC,   C_ETC,   C_LOW_R, C_LOW_S, C_LOW_T, C_LOW_U, C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LCURB, C_ETC,   C_RCURB, C_ETC,   C_ETC
};


/*
    The state codes.
*/
enum states {
    GO,  /* start    */
    OK,  /* ok       */
    OB,  /* object   */
    KE,  /* key      */
    CO,  /* colon    */
    VA,  /* value    */
    AR,  /* array    */
    ST,  /* string   */
    ES,  /* escape   */
    U1,  /* u1       */
    U2,  /* u2       */
    U3,  /* u3       */
    U4,  /* u4       */
    MI,  /* minus    */
    ZE,  /* zero     */
    IT,  /* integer  */
    FR,  /* fraction */
    E1,  /* e        */
    E2,  /* ex       */
    E3,  /* exp      */
    T1,  /* tr       */
    T2,  /* tru      */
    T3,  /* true     */
    F1,  /* fa       */
    F2,  /* fal      */
    F3,  /* fals     */
    F4,  /* false    */
    N1,  /* nu       */
    N2,  /* nul      */
    N3,  /* null     */
    C1,  /* /        */
    C2,  /* / *     */
    C3,  /* *        */
    FX,  /* *.* *eE* */
    D1,  /* second UTF-16 character decoding started by \ */
    D2,  /* second UTF-16 character proceeded by u */
    NR_STATES
};

enum actions
{
    CB = -10, /* comment begin */
    CE = -11, /* comment end */
    FA = -12, /* false */
    TR = -13, /* false */
    NU = -14, /* null */
    DE = -15, /* double detected by exponent e E */
    DF = -16, /* double detected by fraction . */
    SB = -17, /* string begin */
    MX = -18, /* integer detected by minus */
    ZX = -19, /* integer detected by zero */
    IX = -20, /* integer detected by 1-9 */
    EX = -21, /* next char is m_Escaped */
    UC = -22  /* Unicode character read */
};


static int state_transition_table[NR_STATES][NR_CLASSES] = {
/*
    The state transition table takes the current state and the current symbol,
    and returns either a new state or an action. An action is represented as a
    negative number. A JSON text is accepted if at the end of the text the
    state is OK and if the mode is MODE_DONE.

                 white                                      1-9                                   ABCDF  etc
             space |  {  }  [  ]  :  ,  "  \  /  +  -  .  0  |  a  b  c  d  e  f  l  n  r  s  t  u  |  E  |  * */
/*start  GO*/ {GO,GO,-6,__,-5,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*ok     OK*/ {OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*object OB*/ {OB,OB,__,-9,__,__,__,__,SB,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*key    KE*/ {KE,KE,__,__,__,__,__,__,SB,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*colon  CO*/ {CO,CO,__,__,__,__,-2,__,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*value  VA*/ {VA,VA,-6,__,-5,__,__,__,SB,__,CB,__,MX,__,ZX,IX,__,__,__,__,__,FA,__,NU,__,__,TR,__,__,__,__,__},
/*array  AR*/ {AR,AR,-6,__,-5,-7,__,__,SB,__,CB,__,MX,__,ZX,IX,__,__,__,__,__,FA,__,NU,__,__,TR,__,__,__,__,__},
/*string ST*/ {ST,__,ST,ST,ST,ST,ST,ST,-4,EX,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST},
/*escape ES*/ {__,__,__,__,__,__,__,__,ST,ST,ST,__,__,__,__,__,__,ST,__,__,__,ST,__,ST,ST,__,ST,U1,__,__,__,__},
/*u1     U1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U2,U2,U2,U2,U2,U2,U2,U2,__,__,__,__,__,__,U2,U2,__,__},
/*u2     U2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U3,U3,U3,U3,U3,U3,U3,U3,__,__,__,__,__,__,U3,U3,__,__},
/*u3     U3*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,U4,U4,U4,U4,U4,U4,U4,U4,__,__,__,__,__,__,U4,U4,__,__},
/*u4     U4*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,UC,UC,UC,UC,UC,UC,UC,UC,__,__,__,__,__,__,UC,UC,__,__},
/*minus  MI*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,ZE,IT,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*zero   ZE*/ {OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,DF,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*int    IT*/ {OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,DF,IT,IT,__,__,__,__,DE,__,__,__,__,__,__,__,__,DE,__,__},
/*frac   FR*/ {OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,__,FR,FR,__,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__,__},
/*e      E1*/ {__,__,__,__,__,__,__,__,__,__,__,E2,E2,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*ex     E2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*exp    E3*/ {OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,__,E3,E3,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*tr     T1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,T2,__,__,__,__,__,__,__},
/*tru    T2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,T3,__,__,__,__},
/*true   T3*/ {__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__,__,__},
/*fa     F1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F2,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*fal    F2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F3,__,__,__,__,__,__,__,__,__},
/*fals   F3*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,F4,__,__,__,__,__,__},
/*false  F4*/ {__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__,__,__},
/*nu     N1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,N2,__,__,__,__},
/*nul    N2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,N3,__,__,__,__,__,__,__,__,__},
/*null   N3*/ {__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,__,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__},
/*/      C1*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,C2},
/*/*     C2*/ {C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},
/**      C3*/ {C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,CE,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},
/*_.     FX*/ {OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,__,FR,FR,__,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__,__},
/*\      D1*/ {__,__,__,__,__,__,__,__,__,D2,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},
/*\      D2*/ {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,U1,__,__,__,__},
};


/*
    These modes can be Pushed on the stack.
*/
enum modes {
    MODE_ARRAY = 1, 
    MODE_DONE = 2,  
    MODE_KEY = 3,   
    MODE_OBJECT = 4
};

static bool 
Callback(TinyJsonParser::Parser& parser, _Int32 type, const TinyJsonParser::Value* value)
{
    if (parser.m_Node == NULL) 
    {
        return false;
    }

    switch (type)
    {
    case IGAME_JSON_TYPE_ARRAY_BEGIN:
        {
            if (parser.m_Node->m_Type == IGAME_JSON_TYPE_ARRAY)
            {
                _Int8 tmpKey[128];
                sprintf(tmpKey, "%d", parser.m_Node->m_Child.Size());
                parser.m_Node = parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_ARRAY, tmpKey, "");
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
            else
            {
                parser.m_Node->m_Type = IGAME_JSON_TYPE_ARRAY;
            }
        }
        break;
    case IGAME_JSON_TYPE_ARRAY_END:
        {
            parser.m_Node = parser.m_Node->m_Parent;
            if (parser.m_Node == NULL)
            {
                return false;
            }
        }
        break;
    case IGAME_JSON_TYPE_OBJECT_BEGIN:
        {
            if (parser.m_Node->m_Type == IGAME_JSON_TYPE_ARRAY)
            {
                _Int8 tmpKey[128];
                sprintf(tmpKey, "%d", parser.m_Node->m_Child.Size());
                parser.m_Node = parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_OBJECT, tmpKey, "");
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
            else
            {
                parser.m_Node->m_Type = IGAME_JSON_TYPE_OBJECT;
            }
        }
        break;
    case IGAME_JSON_TYPE_OBJECT_END:
        {
            if (parser.m_Node->m_Type != IGAME_JSON_TYPE_ARRAY)
            {
                parser.m_Node = parser.m_Node->m_Parent;
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
        }
        break;
    case IGAME_JSON_TYPE_INTEGER:
        {
            _Int8 tmpValue[128];
            sprintf(tmpValue, JSON_PARSER_INTEGER_SSCANF_TOKEN, value->m_Data.m_IntValue);

            if (parser.m_Node->m_Type == IGAME_JSON_TYPE_ARRAY)
            {
                _Int8 tmpKey[128];
                sprintf(tmpKey, "%d", parser.m_Node->m_Child.Size());
                parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_INTEGER, tmpKey, tmpValue);
            }
            else
            {
                parser.m_Node->m_Type = IGAME_JSON_TYPE_INTEGER;
                parser.m_Node->m_Value = tmpValue;
                parser.m_Node = parser.m_Node->m_Parent;
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
        }
        break;
    case IGAME_JSON_TYPE_FLOAT:
        {
            _Int8 tmpValue[128];
            sprintf(tmpValue, "%Lf", value->m_Data.m_FloatValue);

            if (parser.m_Node->m_Type == IGAME_JSON_TYPE_ARRAY)
            {
                _Int8 tmpKey[128];
                sprintf(tmpKey, "%d", parser.m_Node->m_Child.Size());
                parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_FLOAT, tmpKey, tmpValue);
            }
            else
            {
                parser.m_Node->m_Type = IGAME_JSON_TYPE_FLOAT;
                parser.m_Node->m_Value = tmpValue;
                parser.m_Node = parser.m_Node->m_Parent;
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
        }
        break;
    case IGAME_JSON_TYPE_NULL:
        {
            
        }
        break;
    case IGAME_JSON_TYPE_TRUE:
        {
            if (parser.m_Node->m_Type == IGAME_JSON_TYPE_ARRAY)
            {
                _Int8 tmpKey[128];
                sprintf(tmpKey, "%d", parser.m_Node->m_Child.Size());
                parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_TRUE, tmpKey, "true");
            }
            else
            {
                parser.m_Node->m_Type = IGAME_JSON_TYPE_TRUE;
                parser.m_Node->m_Value = "true";
                parser.m_Node = parser.m_Node->m_Parent;
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
        }
        break;
    case IGAME_JSON_TYPE_FALSE:
        {
            if (parser.m_Node->m_Type == IGAME_JSON_TYPE_ARRAY)
            {
                _Int8 tmpKey[128];
                sprintf(tmpKey, "%d", parser.m_Node->m_Child.Size());
                parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_FALSE, tmpKey, "false");
            }
            else
            {
                parser.m_Node->m_Type = IGAME_JSON_TYPE_FALSE;
                parser.m_Node->m_Value = "false";
                parser.m_Node = parser.m_Node->m_Parent;
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
        }
        break;
    case IGAME_JSON_TYPE_KEY:
        {
            parser.m_Node = parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_NONE, value->m_Data.m_StrValue.m_Value, "");
            if (parser.m_Node == NULL)
            {
                return false;
            }
        }
        break;
    case IGAME_JSON_TYPE_STRING:
        {
            if (parser.m_Node->m_Type == IGAME_JSON_TYPE_ARRAY)
            {
                _Int8 tmpKey[128];
                sprintf(tmpKey, "%d", parser.m_Node->m_Child.Size());
                parser.m_Node->LinkChildToTail(IGAME_JSON_TYPE_STRING, tmpKey, value->m_Data.m_StrValue.m_Value);
            }
            else
            {
                parser.m_Node->m_Type = IGAME_JSON_TYPE_STRING;
                parser.m_Node->m_Value = value->m_Data.m_StrValue.m_Value;
                parser.m_Node = parser.m_Node->m_Parent;
                if (parser.m_Node == NULL)
                {
                    return false;
                }
            }
        }
        break;
    default:
        return false;
    };
    return true;
}

static _Int32
Push(TinyJsonParser::Parser& parser, _Int32 mode)
{
    // Push a mode onto the stack. Return false if there is overflow.
    parser.m_Top += 1;

    if (parser.m_Depth < 0)
    {
        if (parser.m_Top >= parser.m_StackCapacity)
        {
            parser.m_StackCapacity *= 2;
            _UInt32 bytesToAllocate = parser.m_StackCapacity * sizeof(parser.m_StaticStack[0]);
            if (parser.m_Stack == &parser.m_StaticStack[0])
            {
                parser.m_Stack = (char*)XALLOC(bytesToAllocate);
                memcpy(parser.m_Stack, parser.m_StaticStack, sizeof(parser.m_StaticStack));
            }
            else
            {
                char* tmpPointer = parser.m_Stack;
                parser.m_Stack = (char*)XALLOC(bytesToAllocate);
                memcpy(parser.m_Stack, tmpPointer, parser.m_StackCapacity / 2);
                XDELETE(tmpPointer);
            }
        }
        else
        {
            if (parser.m_Top >= parser.m_Depth) 
            {
                return false;
            }
        }
    }
    parser.m_Stack[parser.m_Top] = mode;
    return true;
}

static int
Pop(TinyJsonParser::Parser& parser, _Int32 mode)
{
    // Pop the stack, assuring that the current mode matches the expectation.
    // Return false if there is underflow or if the modes mismatch.

    if (parser.m_Top < 0 || parser.m_Stack[parser.m_Top] != mode) 
    {
        return false;
    }
    parser.m_Top -= 1;
    return true;
}


#define PARSE_BUFFER_CLEAR(parser)                                   \
do {                                                                 \
        parser.m_ParseBufferCount = 0;                               \
        parser.m_ParseBuffer[0] = 0;                                 \
} while (0)
    
#define PARSE_BUFFER_POP_BACK_CHAR(parser)                           \
do {                                                                 \
        assert(parser.m_ParseBufferCount >= 1);                      \
        --parser.m_ParseBufferCount;                                 \
        parser.m_ParseBuffer[parser.m_ParseBufferCount] = 0;         \
} while (0)    

static void 
GrowParseBuffer(TinyJsonParser::Parser& parser)
{
    _UInt32 bytesToAllocate;
    parser.m_ParseBufferCapacity *= 2;
    bytesToAllocate = parser.m_ParseBufferCapacity * sizeof(parser.m_ParseBuffer[0]);
    if (parser.m_ParseBuffer == &parser.m_StaticParseBuffer[0]) 
    {
        parser.m_ParseBuffer = (char*)XALLOC(bytesToAllocate);
        memcpy(parser.m_ParseBuffer, parser.m_StaticParseBuffer, parser.m_ParseBufferCount);
    } 
    else 
    {
        char* tmpPointer = parser.m_ParseBuffer;
        parser.m_ParseBuffer = (char*)XALLOC(bytesToAllocate);
        memcpy(parser.m_ParseBuffer, tmpPointer, parser.m_ParseBufferCount);
        XDELETE(tmpPointer);
    }
}

#define PARSE_BUFFER_PUSH_BACK_CHAR(parser, c)                                                      \
do {                                                                                                \
        if (parser.m_ParseBufferCount + 1 >= parser.m_ParseBufferCapacity) GrowParseBuffer(parser); \
        parser.m_ParseBuffer[parser.m_ParseBufferCount++] = c;                                      \
        parser.m_ParseBuffer[parser.m_ParseBufferCount] = 0;                                        \
} while (0)

#define ASSERT_IS_NON_CONTAINER_TYPE(parser)                                                        \
    assert(                                                                                         \
        parser.m_Type == IGAME_JSON_TYPE_NULL ||                                                    \
        parser.m_Type == IGAME_JSON_TYPE_FALSE ||                                                   \
        parser.m_Type == IGAME_JSON_TYPE_TRUE ||                                                    \
        parser.m_Type == IGAME_JSON_TYPE_FLOAT ||                                                   \
        parser.m_Type == IGAME_JSON_TYPE_INTEGER ||                                                 \
        parser.m_Type == IGAME_JSON_TYPE_STRING)
    

static _Int32
ParseParseBuffer(TinyJsonParser::Parser& parser)
{
    TinyJsonParser::Value value, *arg = NULL;

    if (parser.m_Type != IGAME_JSON_TYPE_NONE) 
    {
        ASSERT_IS_NON_CONTAINER_TYPE(parser);

        switch(parser.m_Type) 
        {
        case IGAME_JSON_TYPE_FLOAT:
            arg = &value;
            if (parser.m_HandleFloatsManually) 
            {
                value.m_Data.m_StrValue.m_Value = parser.m_ParseBuffer;
                value.m_Data.m_StrValue.m_Length = parser.m_ParseBufferCount;
            } 
            else 
            { 
                sscanf(parser.m_ParseBuffer, "%Lf", &value.m_Data.m_FloatValue);
                /* not checking with end pointer b/c there may be trailing ws */
                //value.m_Data.m_FloatValue = strtold(parser.m_ParseBuffer, NULL);
                //std::stringstream tmpBuf;
                //tmpBuf << parser.m_ParseBuffer;
                //tmpBuf >> value.m_Data.m_FloatValue;
            }
            break;
        case IGAME_JSON_TYPE_INTEGER:
            arg = &value;
            sscanf(parser.m_ParseBuffer, JSON_PARSER_INTEGER_SSCANF_TOKEN, &value.m_Data.m_IntValue);
            break;
        case IGAME_JSON_TYPE_STRING:
            arg = &value;
            value.m_Data.m_StrValue.m_Value = parser.m_ParseBuffer;
            value.m_Data.m_StrValue.m_Length = parser.m_ParseBufferCount;
            break;
        }

        if (!Callback(parser, parser.m_Type, arg))
        {
            return false;
        }
    }

    PARSE_BUFFER_CLEAR(parser);

    return true;
}


#define IS_HIGH_SURROGATE(uc) (((uc) & 0xFC00) == 0xD800)
#define IS_LOW_SURROGATE(uc)  (((uc) & 0xFC00) == 0xDC00)
#define DECODE_SURROGATE_PAIR(hi,lo) ((((hi) & 0x3FF) << 10) + ((lo) & 0x3FF) + 0x10000)
static unsigned char Utf8LeadBits[4] = { 0x00, 0xC0, 0xE0, 0xF0 };

static _Int32 
DecodeUnicodeChar(TinyJsonParser::Parser& parser)
{
    _Int32 i;
    _UInt32 uc = 0;
    _Int8* p;
    _Int32 trailBytes;

    assert(parser.m_ParseBufferCount >= 6);

    p = &parser.m_ParseBuffer[parser.m_ParseBufferCount - 4];

    for (i = 12; i >= 0; i -= 4, ++p) 
    {
        _UInt32 x = *p;

        if (x >= 'a') 
        {
            x -= ('a' - 10);
        } 
        else if (x >= 'A') 
        {
            x -= ('A' - 10);
        } 
        else 
        {
            x &= ~0x30u;
        }

        assert(x < 16);

        uc |= x << i;
    }

    /* clear UTF-16 char from buffer */
    parser.m_ParseBufferCount -= 6;
    parser.m_ParseBuffer[parser.m_ParseBufferCount] = 0;

    /* attempt decoding .. */
    if (parser.m_Utf16HighSurrogate) 
    {
        if (IS_LOW_SURROGATE(uc)) 
        {
            uc = DECODE_SURROGATE_PAIR(parser.m_Utf16HighSurrogate, uc);
            trailBytes = 3;
            parser.m_Utf16HighSurrogate = 0;
        } 
        else 
        {
            /* high surrogate without a following low surrogate */
            return false;
        }
    } else 
    {
        if (uc < 0x80) 
        {
            trailBytes = 0;
        } 
        else if (uc < 0x800) 
        {
            trailBytes = 1;
        } 
        else if (IS_HIGH_SURROGATE(uc)) 
        {
            /* save the high surrogate and wait for the low surrogate */
            parser.m_Utf16HighSurrogate = uc;
            return true;
        } 
        else if (IS_LOW_SURROGATE(uc)) 
        {
            /* low surrogate without a preceding high surrogate */
            return false;
        } 
        else 
        {
            trailBytes = 2;
        }
    }

    parser.m_ParseBuffer[parser.m_ParseBufferCount++] = (char) ((uc >> (trailBytes * 6)) | Utf8LeadBits[trailBytes]);

    for (i = trailBytes * 6 - 6; i >= 0; i -= 6) 
    {
        parser.m_ParseBuffer[parser.m_ParseBufferCount++] = (char) (((uc >> i) & 0x3F) | 0x80);
    }

    parser.m_ParseBuffer[parser.m_ParseBufferCount] = 0;

    return true;
}

static _Int32 
AddEscapedCharToParseBuffer(TinyJsonParser::Parser& parser, _Int32 nextChar)
{
    parser.m_Escaped = 0;
    /* remove the backslash */
    PARSE_BUFFER_POP_BACK_CHAR(parser);;
    switch(nextChar) {
        case 'b':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '\b');
            break;
        case 'f':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '\f');
            break;
        case 'n':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '\n');
            break;
        case 'r':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '\r');
            break;
        case 't':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '\t');
            break;
        case '"':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '"');
            break;
        case '\\':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '\\');
            break;
        case '/':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '/');
            break;
        case 'u':
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, '\\');
            PARSE_BUFFER_PUSH_BACK_CHAR(parser, 'u');
            break;
        default:
            return false;
    }

    return true;
}

#define ADD_CHAR_TO_PARSE_BUFFER(parser, nextChar, nextClass)                                              \
do {                                                                                                       \
        if (parser.m_Escaped) {                                                                            \
            if (!AddEscapedCharToParseBuffer(parser, nextChar))                                            \
                return false;                                                                              \
        } else if (!parser.m_Comment) {                                                                    \
        if ((parser.m_Type != IGAME_JSON_TYPE_NONE) | !((nextClass == C_SPACE) | (nextClass == C_WHITE))) {\
                PARSE_BUFFER_PUSH_BACK_CHAR(parser, (char)nextChar);                                       \
            }                                                                                              \
        }                                                                                                  \
} while (0)
    
#define ASSERT_TYPE_ISNT_STRING_NULL_OR_BOOL(parser)                                                       \
    assert(parser.m_Type != IGAME_JSON_TYPE_FALSE);                                                        \
    assert(parser.m_Type != IGAME_JSON_TYPE_TRUE);                                                         \
    assert(parser.m_Type != IGAME_JSON_TYPE_NULL);                                                         \
    assert(parser.m_Type != IGAME_JSON_TYPE_STRING)


static _Int32
ParserChar(TinyJsonParser::Parser& parser, _Int32 nextChar)
{
    // After calling new_JSON_parser, call this function for each character (or
    // partial character) in your JSON text. It can accept UTF-8, UTF-16, or
    // UTF-32. It returns true if things are looking ok so far. If it rejects the
    // text, it returns false.

    int nextClass, nextState;
    

    // Determine the character's class.

    if (nextChar < 0) 
    {
        return false;
    }

    if (nextChar >= 128) 
    {
        nextClass = C_ETC;
    } 
    else 
    {
        nextClass = ascii_class[nextChar];
        if (nextClass <= __) 
        {
            return false;
        }
    }
    
    ADD_CHAR_TO_PARSE_BUFFER(parser, nextChar, nextClass);
    
    // Get the next state from the state transition table.

    nextState = state_transition_table[parser.m_State][nextClass];
    if (nextState >= 0) 
    {
        // Change the state.

        parser.m_State = nextState;
    } 
    else 
    {
        // Or perform one of the actions.
        switch (nextState) {   
        case UC: // Unicode character
            if(!DecodeUnicodeChar(parser)) 
            {
                return false;
            }
            // check if we need to read a second UTF-16 char
            if (parser.m_Utf16HighSurrogate) 
            {
                parser.m_State = D1;
            } 
            else 
            {
                parser.m_State = ST;
            }
            break;
        case EX: // m_Escaped char
            parser.m_Escaped = 1;
            parser.m_State = ES;
            break;
        case MX: // integer detected by minus 
            parser.m_Type = IGAME_JSON_TYPE_INTEGER;
            parser.m_State = MI;
            break;           
        case ZX: // integer detected by zero
            parser.m_Type = IGAME_JSON_TYPE_INTEGER;
            parser.m_State = ZE;
            break;   
        case IX: // integer detected by 1-9
            parser.m_Type = IGAME_JSON_TYPE_INTEGER;
            parser.m_State = IT;
            break;  
        case DE: // floating point number detected by exponent
            ASSERT_TYPE_ISNT_STRING_NULL_OR_BOOL(parser);
            parser.m_Type = IGAME_JSON_TYPE_FLOAT;
            parser.m_State = E1;
            break;   
        case DF: // floating point number detected by fraction
            ASSERT_TYPE_ISNT_STRING_NULL_OR_BOOL(parser);
            if (!parser.m_HandleFloatsManually) 
            {
                // Some versions of strtod (which underlies sscanf) don't support converting 
                // C-locale formated floating point values.
          
                assert(parser.m_ParseBuffer[parser.m_ParseBufferCount-1] == '.');
                parser.m_ParseBuffer[parser.m_ParseBufferCount-1] = parser.m_DecimalPoint;
            }            
            parser.m_Type = IGAME_JSON_TYPE_FLOAT;
            parser.m_State = FX;
            break;   
        case SB: // string begin
            PARSE_BUFFER_CLEAR(parser);
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_Type = IGAME_JSON_TYPE_STRING;
            parser.m_State = ST;
            break;        
        case NU: // n
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_Type = IGAME_JSON_TYPE_NULL;
            parser.m_State = N1;
            break;        
        case FA: // f
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_Type = IGAME_JSON_TYPE_FALSE;
            parser.m_State = F1;
            break;        
        case TR: // t
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_Type = IGAME_JSON_TYPE_TRUE;
            parser.m_State = T1;
            break;        
        case CE: // closing comment
            parser.m_Comment = 0;
            assert(parser.m_ParseBufferCount == 0);
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_State = parser.m_BeforeCommentState;
            break;        
        case CB: // opening comment
            if (!parser.m_AllowComments) 
            {
                return false;
            }
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            if (!ParseParseBuffer(parser)) 
            {
                return false;
            }
            assert(parser.m_ParseBufferCount == 0);
            assert(parser.m_Type != IGAME_JSON_TYPE_STRING);
            switch (parser.m_Stack[parser.m_Top]) {
            case MODE_ARRAY:
            case MODE_OBJECT:   
                switch(parser.m_State) 
                {
                case VA:
                case AR:
                    parser.m_BeforeCommentState = parser.m_State;
                    break;
                default:
                    parser.m_BeforeCommentState = OK;
                    break;
                }
                break;
            default:
                parser.m_BeforeCommentState = parser.m_State;
                break;
            }
            parser.m_Type = IGAME_JSON_TYPE_NONE;
            parser.m_State = C1;
            parser.m_Comment = 1;
            break;
        case -9: // empty }    
            PARSE_BUFFER_CLEAR(parser);
            if (!Callback(parser, IGAME_JSON_TYPE_OBJECT_END, NULL))
            {
                return false;
            }
            if (!Pop(parser, MODE_KEY)) 
            {
                return false;
            }
            parser.m_State = OK;
            break;

        case -8: // }
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            if (!ParseParseBuffer(parser)) 
            {
                return false;
            }
            if (!Callback(parser, IGAME_JSON_TYPE_OBJECT_END, NULL))
            {
                return false;
            }
            if (!Pop(parser, MODE_OBJECT)) 
            {
                return false;
            }
            parser.m_Type = IGAME_JSON_TYPE_NONE;
            parser.m_State = OK;
            break;

        case -7: // ]
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            if (!ParseParseBuffer(parser)) 
            {
                return false;
            }

            if (!Callback(parser, IGAME_JSON_TYPE_ARRAY_END, NULL))
            {
                return false;
            }
            if (!Pop(parser, MODE_ARRAY)) 
            {
                return false;
            }
            
            parser.m_Type = IGAME_JSON_TYPE_NONE;
            parser.m_State = OK;
            break;

         case -6: // {
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            if (!Callback(parser, IGAME_JSON_TYPE_OBJECT_BEGIN, NULL))
            {
                return false;
            }
            if (!Push(parser, MODE_KEY)) 
            {
                return false;
            }
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_State = OB;
            break;

        case -5: // [
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            if (!Callback(parser, IGAME_JSON_TYPE_ARRAY_BEGIN, NULL))
            {
                return false;
            }
            if (!Push(parser, MODE_ARRAY)) 
            {
                return false;
            }
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_State = AR;
            break;
        case -4: // string end
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            switch (parser.m_Stack[parser.m_Top]) 
            {
            case MODE_KEY:
                assert(parser.m_Type == IGAME_JSON_TYPE_STRING);
                parser.m_Type = IGAME_JSON_TYPE_NONE;
                parser.m_State = CO;

                TinyJsonParser::Value value;
                value.m_Data.m_StrValue.m_Value = parser.m_ParseBuffer;
                value.m_Data.m_StrValue.m_Length = parser.m_ParseBufferCount;
                if (!Callback(parser, IGAME_JSON_TYPE_KEY, &value))
                {
                    return false;
                }
                PARSE_BUFFER_CLEAR(parser);
                break;
            case MODE_ARRAY:
            case MODE_OBJECT:
                assert(parser.m_Type == IGAME_JSON_TYPE_STRING);
                if (!ParseParseBuffer(parser)) 
                {
                    return false;
                }
                parser.m_Type = IGAME_JSON_TYPE_NONE;
                parser.m_State = OK;
                break;
            default:
                return false;
            }
            break;

        case -3: // ,
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            if (!ParseParseBuffer(parser)) 
            {
                return false;
            }
            switch (parser.m_Stack[parser.m_Top]) 
            {
            case MODE_OBJECT:
                // A comma causes a flip from object mode to key mode.
                if (!Pop(parser, MODE_OBJECT) || !Push(parser, MODE_KEY)) 
                {
                    return false;
                }
                assert(parser.m_Type != IGAME_JSON_TYPE_STRING);
                parser.m_Type = IGAME_JSON_TYPE_NONE;
                parser.m_State = KE;
                break;
            case MODE_ARRAY:
                assert(parser.m_Type != IGAME_JSON_TYPE_STRING);
                parser.m_Type = IGAME_JSON_TYPE_NONE;
                parser.m_State = VA;
                break;
            default:
                return false;
            }
            break;

        case -2: // :
            // A colon causes a flip from key mode to object mode.
            PARSE_BUFFER_POP_BACK_CHAR(parser);
            if (!Pop(parser, MODE_KEY) || !Push(parser, MODE_OBJECT)) 
            {
                return false;
            }
            assert(parser.m_Type == IGAME_JSON_TYPE_NONE);
            parser.m_State = VA;
            break;
        default: // Bad action.
            return false;
        }
    }
    return true;
}

static _Int32
ParserDone(TinyJsonParser::Parser& parser)
{
    const int result = parser.m_State == OK && Pop(parser, MODE_DONE);
    return result;
}


static _Int32
ParserIsLegalWhiteSpaceString(const char* s)
{
    _Int32 c, charClass;
    
    if (s == NULL) 
    {
        return false;
    }
    
    for (; *s; ++s) 
    {   
        c = *s;
        
        if (c < 0 || c >= 128) 
        {
            return false;
        }
        
        charClass = ascii_class[c];
        
        if (charClass != C_SPACE && charClass != C_WHITE) 
        {
            return false;
        }
    }
    
    return true;
}

TinyJsonParser::Parser::Parser(TiJsonNode* node, _Int32 depth)
    : m_Node(node), m_State(GO), m_BeforeCommentState(0), m_Type(0), m_Escaped(0)
    , m_Comment(0), m_AllowComments(1), m_HandleFloatsManually(0), m_Utf16HighSurrogate(0)
    , m_Depth(depth), m_Top(-1), m_Stack(0), m_StackCapacity(0), m_DecimalPoint(0) 
    , m_ParseBuffer(0), m_ParseBufferCapacity(0), m_ParseBufferCount(0), m_CommentBeginOffset(0)
{
    memset(m_StaticStack, 0, JSON_PARSER_STACK_SIZE);
    memset(m_StaticParseBuffer, 0, JSON_PARSER_PARSE_BUFFER_SIZE);

    if (depth > 0)
    {
        m_Depth = depth;
        m_StackCapacity = depth;

        if (m_Depth <= JSON_PARSER_STACK_SIZE)
        {
            m_StackCapacity = m_Depth;
            m_Stack = &m_StaticStack[0];
        }
        else
        {
            m_Stack = (char*)XALLOC(m_Depth * sizeof(m_StaticStack[0]) + 1);
        }
    }
    else
    {
        m_StackCapacity = JSON_PARSER_STACK_SIZE;
        m_Depth = -1;
        m_Stack = &m_StaticStack[0];
    }

    m_Top += 1;
    assert(m_Top < m_Depth);
    m_Stack[m_Top] = MODE_DONE;

    m_ParseBuffer = &m_StaticParseBuffer[0];
    m_ParseBufferCapacity = COUNTOF(m_StaticParseBuffer);
    m_ParseBufferCount = 0;
    m_ParseBuffer[0] = 0; 
    m_DecimalPoint = *localeconv()->decimal_point;
}

TinyJsonParser::Parser::~Parser()
{
    if (m_Stack != &m_StaticStack[0])
    {
        SAFE_DELETE(m_Stack);
    }

    if (m_ParseBuffer != &m_StaticParseBuffer[0])
    {
        SAFE_DELETE(m_ParseBuffer);
    }
}

bool 
TinyJsonParser::Parse(TiJsonNode& node, const std::string& data)
{
    _UInt32 depth = data.size();
    if (depth == 0) return false;

    TinyJsonParser::Parser parser(&node, depth);
    
    for (std::string::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        if (!ParserChar(parser, *it))
        {
            return false;
        }
    }

    return true;
}
