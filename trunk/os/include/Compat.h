/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_COMPAT_H
#define IGAME_OS_COMPAT_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <cassert>

#if defined (_WIN32)

#pragma warning (disable:4099)
#pragma warning (disable:4348)
#pragma warning (disable:4267)
#pragma warning (disable:4800)
#pragma warning (disable:4018)
#pragma warning (disable:4267)
#pragma warning (disable:4244)
#pragma warning (disable:6281)

#define strcasecmp                         _stricmp
#define strncasecmp                        _strnicmp
#define GET_LAST_ERROR()                   WSAGetLastError()
#define SOCKET_ERRNUM(errno)               WSA##errno

typedef char                               _Int8;
typedef short                              _Int16;
typedef int                                _Int32;
typedef long long                          _Int64;

typedef unsigned char                      _UInt8;
typedef unsigned short                     _UInt16;
typedef unsigned int                       _UInt32;
typedef unsigned long long                 _UInt64;

#elif defined (__linux__) || defined (__FreeBSD__)
#include <stdint.h>
#include <errno.h>

typedef char                               _Int8;
typedef short                              _Int16;
typedef int                                _Int32;
typedef int64_t                            _Int64;

typedef unsigned char                      _UInt8;
typedef unsigned short                     _UInt16;
typedef unsigned int                       _UInt32;
typedef uint64_t                           _UInt64;

#define closesocket(fd)                    close(fd)
#define GET_LAST_ERROR()                   errno
#define SOCKET_ERRNUM(errnum)              errnum 

#endif // #if defined (_WIN32)

namespace Compat
{
    template <bool Boolean, class Type = void>
    struct EnableIf { typedef Type type; }; 
    template <class Type> struct EnableIf<false, Type > {};

    template <class Type> struct IsInteger { static const bool s_Value = false; };
    template <> struct IsInteger<long> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int8> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int16> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int32> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int64> { static const bool s_Value = true; };

    template <class Type> struct IsStdBasicString { static const bool s_Value = false; };
    template <class Type, class Traits, class AllocatorType>
    struct IsStdBasicString<std::basic_string<Type, Traits, AllocatorType > >
    { static const bool s_Value = true; };

    template <class Type, class Enable = void>
    struct ItoaTraits
    {
        static bool Itoa(Type n, _Int8* buf, _UInt32 buflen, _Int32 radix)
        {
            if (buf == 0) return false;

            char* p = buf;

            do 
            {
                *p++ = "0123456789"[n % radix];
                n /= radix;
                if ((p - buf) > buflen) return 0;
            } while (n);

            buf[p - buf] = 0;

            int i, j;
            for (i = 0, j = (int)strlen(buf) - 1; i < j; i++, j--)
            {
                buf[j] = buf[i] + buf[j];
                buf[i] = buf[j] - buf[i];
                buf[j] = buf[j] - buf[i];
            }
            return false;
        }
    };

    template <class Type >
    struct ItoaTraits<Type, typename EnableIf<IsStdBasicString<Type >::s_Value>::type>
    {
        static bool Itoa(Type n, _Int8* buf, _UInt32 buflen, _Int32 radix)
        {
            _Int32 len = n.length();
            if (len >= buflen) 
            {
                return false;
            }
            strncpy(buf, n.c_str(), len);
            buf[len] = 0;
            return true;
        }
    };

    template <class Type >
    struct ItoaTraits<Type, typename EnableIf<IsInteger<Type >::s_Value>::type>
    {
        static bool Itoa(Type n, _Int8* buf, _UInt32 buflen, _Int32 radix)
        {
            if (buf == 0) return false;

            bool sign = true;
            if (n < 0)
            {
                sign = false;
                n = -n;
            }

            _Int8* p = sign ? buf : buf + 1;

            do 
            {
                *p++ = "0123456789"[n % radix];
                n /= radix;
                if ((p - buf) > buflen) return 0;
            } while (n);

            buf[p - buf] = 0;

            
            int i, j;
            for (i = i = sign ? 0 : 1, j = (int)strlen(buf) - 1; i < j; i++, j--)
            {
                buf[j] = buf[i] + buf[j];
                buf[i] = buf[j] - buf[i];
                buf[j] = buf[j] - buf[i];
            }

            if (sign == false)
            {
                buf[0] = '-';
            }
            
            return true;
        }
    };
}

template <typename Type>
Type Atoi(const char* s)
{
    Type n;
    int i, sign;

    assert(s);

    while ((*s == 32 || *s == 9)) s++;
    sign = (*s == '-') ? -1 : 1;
    while (*s == '-') s++;
    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
        n = 10 * n + (s[i] - '0');
    return sign * n;
}

// Return 0 success.
template <typename Type>
bool Itoa(Type n, _Int8* buf, _UInt32 buflen, _Int32 radix)
{
    return Compat::ItoaTraits<Type >::Itoa(n, buf, buflen, radix);
}

// Only supprot for std::string
template <typename Type>
void StrTrim(Type& src)
{
    _Int32 i = 0;
    _Int32 tailPos = 0;

    for (_Int32 cpyPos = i; i < src.length(); i++)
    {
        if (src[i] != 32 && src[i] != 9)
        {
            src[cpyPos] = src[i];
            cpyPos++;
            tailPos = cpyPos;
        }
        else if (tailPos)
        {
            src[cpyPos] = src[i];
            cpyPos++;
        }
    }

    if (tailPos) 
    {
        src.resize(tailPos);
    }
}

// Only support for std::string and const char*
template <typename Type>
void StrSplit(Type& src, const std::string& sep, std::vector<std::string>& dst, bool trim = false)
{
    std::string s;
    std::string tmpSrc = src; // TODO: Optimized for performance
    for (std::string::iterator it = tmpSrc.begin(); it != tmpSrc.end(); it++)
    {
        if (sep.find(*it) != std::string::npos)
        {
            if (s.length()) dst.push_back(s);
            s = "";
        }
        else
        {
            s += *it;
        }
    }

    if (s.length())
    {
        if(trim)
        {
            StrTrim(s);
        }
        dst.push_back(s);
    }
}

#endif // #ifndef IGAME_OS_COMPAT_H




