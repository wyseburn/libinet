/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_BYTE_ORDER_H
#define IGAME_OS_BYTE_ORDER_H

#if defined (_WIN32)
# include <winsock2.h>
# elif defined (__linux__) || defined (__FreeBSD__)
# include <arpa/inet.h>
#endif

#include "os/include/Compat.h"

namespace IGame
{
    template <typename _T_> 
    class EndianReverse
    {
    public:
        inline static _T_ Convert(_T_ val) { return val; }
    };

    template <>
    inline _Int16 EndianReverse<_Int16>::Convert(_Int16 input)
    {
        return (_Int16) ((input << 8) & 0xff00) | ((input >> 8) & 0x00ff);
    }

    template <>
    inline _UInt16 EndianReverse<_UInt16>::Convert(_UInt16 input)
    {
        return (_UInt16) ((input << 8) & 0xff00) | ((input >> 8) & 0x00ff);
    }

    template <>
    inline _Int32 EndianReverse<_Int32>::Convert(_Int32 input)
    {
        return (_Int32) ((input << 24) & 0xff000000)| 
                        ((input << 8 ) & 0x00ff0000)| 
                        ((input >> 8 ) & 0x0000ff00)| 
                        ((input >> 24) & 0x000000ff);
    }

    template <>
    inline _UInt32 EndianReverse<_UInt32>::Convert(_UInt32 input)
    {
        return (_UInt32) ((input << 24) & 0xff000000)| 
                         ((input << 8 ) & 0x00ff0000)| 
                         ((input >> 8 ) & 0x0000ff00)| 
                         ((input >> 24) & 0x000000ff);
    }

    template <>
    inline _Int64 EndianReverse<_Int64>::Convert(_Int64 input)
    {
        return (_Int64) ((input << 56) & 0xff00000000000000ull)|
                        ((input << 40) & 0x00ff000000000000ull)|
                        ((input << 24) & 0x0000ff0000000000ull)|
                        ((input << 8 ) & 0x000000ff00000000ull)|
                        ((input >> 8 ) & 0x00000000ff000000ull)|
                        ((input >> 24) & 0x0000000000ff0000ull)|
                        ((input >> 40) & 0x000000000000ff00ull)|
                        ((input >> 56) & 0x00000000000000ffull);
    }

    template <>
    inline _UInt64 EndianReverse<_UInt64>::Convert(_UInt64 input)
    {
        return (_UInt64) ((input << 56) & 0xff00000000000000ull)|
                         ((input << 40) & 0x00ff000000000000ull)|
                         ((input << 24) & 0x0000ff0000000000ull)|
                         ((input << 8 ) & 0x000000ff00000000ull)|
                         ((input >> 8 ) & 0x00000000ff000000ull)|
                         ((input >> 24) & 0x0000000000ff0000ull)|
                         ((input >> 40) & 0x000000000000ff00ull)|
                         ((input >> 56) & 0x00000000000000ffull);
    }

} // namesapce IGame
                           
#endif // #ifndef IGAME_BYTE_ORDER_H


