/**
 *  Version:     @(#)libinet/s11n.hxx    0.1.2 29/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     C++ object s11n implement 
 *
 *  This library is free software; you can redistribute it and/or modify it under 
 *  the terms of the GNU Lesser General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) any 
 *  later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 *  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along 
 *  with this library; if not, write to the Free Software Foundation, Inc., 
 *  59 Temple  Place, Suite 330, Boston, MA 02111-1307  USA
 */

#ifndef __LIBINET_SERIALIZE_H__
#define __LIBINET_SERIALIZE_H__

#include "buffer.hxx"
#include "typecheck.hxx"
#include "byteorder.hxx"
#include "blob.hxx"

#define INET_FLOAT_PRECISION  1000000
#define INET_DOUBLE_PRECISION 1000000000

namespace inet
{
    template <class Type, class enable = void>
    struct s11n_traits
    {
        static inet_uint32 s11n_size(const Type& instance)
        {
            return instance.__inet_s11n_size(instance);
        }

        static void s11n(Type& instance, inet::buffer& buffer)
        {
            instance.__inet_s11n(buffer);
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            return instance.__inet_uns11n(buffer);
        }
    };

    /* Custom memory blob object*/
    template <class Type>
    struct s11n_traits<Type, typename enableif<is_blob<Type >::value>::type>
    {
        static inet_uint32 s11n_size(const Type& instance) 
        { 
            return sizeof(instance.len_) + instance.len_;
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
            inet_uint32 len = instance.len_;
#if !defined (_INET_BYTE_ORDER_NOT_CONVERT)
            len = endian_reverse<inet_uint32>::convert(instance.len_);
#endif
            buffer << (inet_uint32) len;
            buffer.write(instance.data_, instance.len_);
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            if (buffer.length() < sizeof(instance.len_) + instance.len_)
                return false;
            buffer >> instance.len_;
#if !defined (_INET_BYTE_ORDER_NOT_CONVERT)
             instance.len_ = endian_reverse<inet_uint32>::convert(instance.len_);
#endif
             buffer.read(instance.data_, instance.len_);
            return true;
        }
    };

    template <class Type>
    struct s11n_traits<Type, typename enableif<is_integer<Type >::value>::type>
    {
        static inet_uint32 s11n_size(const Type& instance) 
        { 
            return sizeof(instance); 
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            buffer << instance;
#else
            buffer << endian_reverse<Type >::convert(instance);
#endif
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            if (buffer.length() < sizeof(Type))
                return false;
            buffer >> instance;
#if !defined (_INET_BYTE_ORDER_NOT_CONVERT)
            instance = endian_reverse<Type >::convert(instance);
#endif
            return true;
        }
    };

    template <class Type>
    struct s11n_traits<Type, typename enableif<is_float<Type >::value>::type>
    {
        static inet_uint32 s11n_size(const Type& instance) 
        { 
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            return sizeof(instance); 
#else
            return sizeof(instance) * 2;
#endif
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            buffer << instance;
#else
            assert(sizeof(Type) == 4);
            inet_uint32 integer = (inet_uint32)instance;
            inet_uint32 decimal = (inet_uint32)((instance - integer) * INET_FLOAT_PRECISION);
            buffer << endian_reverse<inet_uint32>::convert(integer);
            buffer << endian_reverse<inet_uint32>::convert(decimal);
#endif
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            if (buffer.length() < sizeof(Type))
                return false;
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            buffer >> instance;
#else
            assert(sizeof(Type) == 4);

            inet_uint32 integer, decimal;
            buffer >> integer;
            integer = endian_reverse<inet_uint32>::convert(integer);
            buffer >> decimal;
            decimal = endian_reverse<inet_uint32>::convert(decimal);
            instance = (Type)integer;
            instance += ((Type)(decimal)) / INET_FLOAT_PRECISION;
#endif
            return true;
        }
    };

    template <class Type>
    struct s11n_traits<Type, typename enableif<is_double<Type >::value>::type>
    {
        static inet_uint32 s11n_size(const Type& instance) 
        { 
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            return sizeof(instance); 
#else
            return sizeof(instance) * 2;
#endif
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            buffer << instance;
#else
            inet_uint64 integer = (inet_uint64)instance;
            inet_uint64 decimal = (inet_uint64)((instance - integer) * INET_DOUBLE_PRECISION);
            buffer << endian_reverse<inet_uint64>::convert(integer);
            buffer << endian_reverse<inet_uint64>::convert(decimal);
#endif
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            if (buffer.length() < sizeof(Type))
                return false;
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            buffer >> instance;
#else
            assert(sizeof(Type) == 8);

            inet_uint64 integer, decimal;
            buffer >> integer;
            integer = endian_reverse<inet_uint64>::convert(integer);
            buffer >> decimal;
            decimal = endian_reverse<inet_uint64>::convert(decimal);
            instance = (Type)integer;
            instance += ((Type)(decimal)) / INET_DOUBLE_PRECISION;
#endif
            return true;
        }
    };

    // Due to improve string copy performance, we special basic_string.
    template <class Type>
    struct s11n_traits<Type, typename enableif<is_std_basic_string<Type >::value>::type>
    {
        typedef typename Type::value_type elem_type;

        static inet_uint32 s11n_size(const Type& instance)
        {
            inet_uint32 size = sizeof(inet_uint32);
            size += (inet_uint32)instance.size();
            return size;
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            buffer << (inet_uint32)instance.size();
#else
            buffer << endian_reverse<inet_uint32>::convert((inet_uint32)instance.size());
#endif
            buffer << instance.c_str();
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            if (buffer.length() < sizeof(inet_uint32))
                return false;

            inet_uint32 size = 0;
            buffer >> size;
#if !defined (_INET_BYTE_ORDER_NOT_CONVERT)
            size = endian_reverse<inet_uint32>::convert(size);
#endif
            if (buffer.length() < size) 
                return false;
            buffer >> instance; // performance ???
            return true;
        }
    };

    template <class Type>
    struct s11n_traits<Type, typename enableif<is_std_pair<Type >::value>::type>
    {
        typedef typename Type::first_type first_type;
        typedef typename Type::second_type second_type;

        static inet_uint32 s11n_size(const Type& instance)
        {
            return s11n_traits<first_type >::s11n_size(instance.first) +
                s11n_traits<second_type >::s11n_size(instance.second);
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
            s11n_traits<first_type >::s11n(instance.first, buffer); 
            s11n_traits<second_type >::s11n(instance.second, buffer);
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            return s11n_traits<first_type >::uns11n(instance.first, buffer) &&
                s11n_traits<second_type >::uns11n(instance.second, buffer);
        }
    };

    template <class Type>
    struct s11n_traits<Type, typename enableif<is_std_sequence_container<Type >::value 
        || is_std_set_container<Type >::value>::type>
    {
        typedef typename Type::value_type elem_type;
        typedef typename Type::const_iterator iterator_type;

        static inet_uint32 s11n_size(const Type& instance)
        {
            inet_uint32 size = sizeof(inet_uint32);
            for (iterator_type it = instance.begin(); it != instance.end(); ++it)
                size += s11n_traits<elem_type >::s11n_size(*it);
            return size;
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
#if defined (_INET_BYTE_ORDER_NOT_CONVERT)
            buffer << (inet_uint32)instance.size();
#else
            buffer << endian_reverse<inet_uint32>::convert((inet_uint32)instance.size());
#endif
            for (iterator_type it = instance.begin(); it != instance.end(); ++it)
                s11n_traits<elem_type >::s11n(*it, buffer);
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            inet_uint32 size = 0;
            buffer >> size;
#if !defined (_INET_BYTE_ORDER_NOT_CONVERT)
            size = endian_reverse<inet_uint32>::convert(size);
#endif
            for (; size; --size)
            {
                elem_type elem;
                if (!s11n_traits<elem_type >::uns11n(elem, buffer))
                    return false;
                instance.insert(instance.end(), elem);
            }
            return true;
        }
    };

    template <class Type>
    struct s11n_traits<Type, typename enableif<is_std_map_container<Type >::value>::type>
    {
        typedef typename Type::key_type key_type;
        typedef typename Type::mapped_type mapped_type;
        typedef typename std::pair<key_type, mapped_type > elem_type;
        typedef typename Type::const_iterator iterator_type;

        static inet_uint32 s11n_size(const Type& instance)
        {
            inet_uint32 size = sizeof(inet_uint32);
            for (iterator_type it = instance.begin(); it != instance.end(); ++it)
                size += s11n_traits<elem_type >::s11n_size(*it);
            return size;
        }

        static void s11n(const Type& instance, inet::buffer& buffer)
        {
#if defined (_BYTE_ORDER_NOT_CONVERT)
            buffer << instance.size();
#else
            buffer << endian_reverse<inet_uint32>::convert((inet_uint32)instance.size());
#endif
            for (iterator_type it = instance.begin(); it != instance.end(); ++it)
                s11n_traits<elem_type >::s11n(*it, buffer);
        }

        static bool uns11n(Type& instance, inet::buffer& buffer)
        {
            inet_uint32 size = 0;
            buffer >> size;
#if !defined (_BYTE_ORDER_NOT_CONVERT)
            size = endian_reverse<inet_uint32>::convert(size);
#endif
            for (; size; --size)
            {
                elem_type elem;
                if (!s11n_traits<elem_type >::uns11n(elem, buffer))
                    return false;
                instance.insert(instance.end(), elem);
            }
            return true;
        }
    };

    template <class Type>
    static inet_uint32 serialize_size(const Type& instance)
    {
        return s11n_traits<Type >::s11n_size(instance);
    }

    template <class Type>
    static bool serialize(const Type& instance, inet::buffer& buffer)
    {
        s11n_traits<Type >::s11n((Type&)instance, buffer);
        return true;
    }
    
    template <class Type>
    static bool unserialize(Type& instance, inet::buffer& buffer)
    {
        s11n_traits<Type >::uns11n(instance, buffer);
        return true;
    }
}

#define INET_S11N_1(classname, member1type, member1name)                                  \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n(this->member1name, buffer);              \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer);            \
}

#define INET_S11N_TRAITS_1(classname, member1type, member1name)                           \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_2(classname, member1type, member1name, member2type, member2name)        \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer);            \
}

#define INET_S11N_TRAITS_2(classname, member1type, member1name, member2type, member2name) \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_3(classname, member1type, member1name, member2type, member2name,        \
member3type, member3name)                                                                 \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer);            \
}

#define INET_S11N_TRAITS_3(classname, member1type, member1name, member2type, member2name, \
member3type, member3name)                                                                 \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_4(classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name)                                       \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer);            \
}

#define INET_S11N_TRAITS_4(classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name)                                       \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_5(classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name)             \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer);            \
}

#define INET_S11N_TRAITS_5(classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name)             \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_6(classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name)                                                                              \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer);            \
}

#define INET_S11N_TRAITS_6(classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name)                                                                              \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_7(classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name)                                                    \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer);             \
}

#define INET_S11N_TRAITS_7(classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name)                                                    \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_8(classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name)                          \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer);            \
}

#define INET_S11N_TRAITS_8(classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name)                          \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_9(classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name)\
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name)+                 \
           inet::s11n_traits<member9type >::s11n_size(this->member9name);                 \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
    inet::s11n_traits<member9type >::s11n(this->member9name, buffer);                     \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer)             \
        && inet::s11n_traits<member9type >::uns11n(this->member9name, buffer);            \
}

#define INET_S11N_TRAITS_9(classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name)\
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name)+            \
                   s11n_traits<member9type >::s11n_size(instance.member9name);            \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
            s11n_traits<member9type >::s11n(instance.member9name, buffer);                \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
            s11n_traits<member9type >::uns11n(instance.member9name, buffer);              \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_10(classname, member1type, member1name, member2type, member2name,       \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name)                                                               \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name)+                 \
           inet::s11n_traits<member9type >::s11n_size(this->member9name)+                 \
           inet::s11n_traits<member10type >::s11n_size(this->member10name);               \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
    inet::s11n_traits<member9type >::s11n(this->member9name, buffer);                     \
    inet::s11n_traits<member10type >::s11n(this->member10name, buffer);                   \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer)             \
        && inet::s11n_traits<member9type >::uns11n(this->member9name, buffer)             \
        && inet::s11n_traits<member10type >::uns11n(this->member10name, buffer);          \
}

#define INET_S11N_TRAITS_10(classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name)                                                               \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name)+            \
                   s11n_traits<member9type >::s11n_size(instance.member9name)+            \
                   s11n_traits<member10type >::s11n_size(instance.member10name);          \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
            s11n_traits<member9type >::s11n(instance.member9name, buffer);                \
            s11n_traits<member10type >::s11n(instance.member10name, buffer);              \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
            s11n_traits<member9type >::uns11n(instance.member9name, buffer);              \
            s11n_traits<member10type >::uns11n(instance.member10name, buffer);            \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_11(classname, member1type, member1name, member2type, member2name,       \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name)                                   \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name)+                 \
           inet::s11n_traits<member9type >::s11n_size(this->member9name)+                 \
           inet::s11n_traits<member10type >::s11n_size(this->member10name)+               \
           inet::s11n_traits<member11type >::s11n_size(this->member11name);               \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
    inet::s11n_traits<member9type >::s11n(this->member9name, buffer);                     \
    inet::s11n_traits<member10type >::s11n(this->member10name, buffer);                   \
    inet::s11n_traits<member11type >::s11n(this->member11name, buffer);                   \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer)             \
        && inet::s11n_traits<member9type >::uns11n(this->member9name, buffer)             \
        && inet::s11n_traits<member10type >::uns11n(this->member10name, buffer)           \
        && inet::s11n_traits<member11type >::uns11n(this->member11name, buffer);          \
}

#define INET_S11N_TRAITS_11(classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name)                                   \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name)+            \
                   s11n_traits<member9type >::s11n_size(instance.member9name)+            \
                   s11n_traits<member10type >::s11n_size(instance.member10name)+          \
                   s11n_traits<member11type >::s11n_size(instance.member11name);          \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
            s11n_traits<member9type >::s11n(instance.member9name, buffer);                \
            s11n_traits<member10type >::s11n(instance.member10name, buffer);              \
            s11n_traits<member11type >::s11n(instance.member11name, buffer);              \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
            s11n_traits<member9type >::uns11n(instance.member9name, buffer);              \
            s11n_traits<member10type >::uns11n(instance.member10name, buffer);            \
            s11n_traits<member11type >::uns11n(instance.member11name, buffer);            \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_12(classname, member1type, member1name, member2type, member2name,       \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name)       \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name)+                 \
           inet::s11n_traits<member9type >::s11n_size(this->member9name)+                 \
           inet::s11n_traits<member10type >::s11n_size(this->member10name)+               \
           inet::s11n_traits<member11type >::s11n_size(this->member11name)+               \
           inet::s11n_traits<member12type >::s11n_size(this->member12name);               \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
    inet::s11n_traits<member9type >::s11n(this->member9name, buffer);                     \
    inet::s11n_traits<member10type >::s11n(this->member10name, buffer);                   \
    inet::s11n_traits<member11type >::s11n(this->member11name, buffer);                   \
    inet::s11n_traits<member12type >::s11n(this->member12name, buffer);                   \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer)             \
        && inet::s11n_traits<member9type >::uns11n(this->member9name, buffer)             \
        && inet::s11n_traits<member10type >::uns11n(this->member10name, buffer)           \
        && inet::s11n_traits<member11type >::uns11n(this->member11name, buffer)           \
        && inet::s11n_traits<member12type >::uns11n(this->member12name, buffer);          \
}

#define INET_S11N_TRAITS_12(classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name)       \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name)+            \
                   s11n_traits<member9type >::s11n_size(instance.member9name)+            \
                   s11n_traits<member10type >::s11n_size(instance.member10name)+          \
                   s11n_traits<member11type >::s11n_size(instance.member11name)+          \
                   s11n_traits<member12type >::s11n_size(instance.member12name);          \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
            s11n_traits<member9type >::s11n(instance.member9name, buffer);                \
            s11n_traits<member10type >::s11n(instance.member10name, buffer);              \
            s11n_traits<member11type >::s11n(instance.member11name, buffer);              \
            s11n_traits<member12type >::s11n(instance.member12name, buffer);              \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
            s11n_traits<member9type >::uns11n(instance.member9name, buffer);              \
            s11n_traits<member10type >::uns11n(instance.member10name, buffer);            \
            s11n_traits<member11type >::uns11n(instance.member11name, buffer);            \
            s11n_traits<member12type >::uns11n(instance.member12name, buffer);            \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_13(classname, member1type, member1name, member2type, member2name,       \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name,       \
member13type, member13name)                                                               \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name)+                 \
           inet::s11n_traits<member9type >::s11n_size(this->member9name)+                 \
           inet::s11n_traits<member10type >::s11n_size(this->member10name)+               \
           inet::s11n_traits<member11type >::s11n_size(this->member11name)+               \
           inet::s11n_traits<member12type >::s11n_size(this->member12name)+               \
           inet::s11n_traits<member13type >::s11n_size(this->member13name);               \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
    inet::s11n_traits<member9type >::s11n(this->member9name, buffer);                     \
    inet::s11n_traits<member10type >::s11n(this->member10name, buffer);                   \
    inet::s11n_traits<member11type >::s11n(this->member11name, buffer);                   \
    inet::s11n_traits<member12type >::s11n(this->member12name, buffer);                   \
    inet::s11n_traits<member13type >::s11n(this->member13name, buffer);                   \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer)             \
        && inet::s11n_traits<member9type >::uns11n(this->member9name, buffer)             \
        && inet::s11n_traits<member10type >::uns11n(this->member10name, buffer)           \
        && inet::s11n_traits<member11type >::uns11n(this->member11name, buffer)           \
        && inet::s11n_traits<member12type >::uns11n(this->member12name, buffer)           \
        && inet::s11n_traits<member13type >::uns11n(this->member13name, buffer);          \
}

#define INET_S11N_TRAITS_13(classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name,       \
member13type, member13name)                                                               \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name)+            \
                   s11n_traits<member9type >::s11n_size(instance.member9name)+            \
                   s11n_traits<member10type >::s11n_size(instance.member10name)+          \
                   s11n_traits<member11type >::s11n_size(instance.member11name)+          \
                   s11n_traits<member12type >::s11n_size(instance.member12name)+          \
                   s11n_traits<member13type >::s11n_size(instance.member13name);          \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
            s11n_traits<member9type >::s11n(instance.member9name, buffer);                \
            s11n_traits<member10type >::s11n(instance.member10name, buffer);              \
            s11n_traits<member11type >::s11n(instance.member11name, buffer);              \
            s11n_traits<member12type >::s11n(instance.member12name, buffer);              \
            s11n_traits<member13type >::s11n(instance.member13name, buffer);              \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
            s11n_traits<member9type >::uns11n(instance.member9name, buffer);              \
            s11n_traits<member10type >::uns11n(instance.member10name, buffer);            \
            s11n_traits<member11type >::uns11n(instance.member11name, buffer);            \
            s11n_traits<member12type >::uns11n(instance.member12name, buffer);            \
            s11n_traits<member13type >::uns11n(instance.member13name, buffer);            \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_14(classname, member1type, member1name, member2type, member2name,       \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name,       \
member13type, member13name, member14type, member14name)                                   \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name)+                 \
           inet::s11n_traits<member9type >::s11n_size(this->member9name)+                 \
           inet::s11n_traits<member10type >::s11n_size(this->member10name)+               \
           inet::s11n_traits<member11type >::s11n_size(this->member11name)+               \
           inet::s11n_traits<member12type >::s11n_size(this->member12name)+               \
           inet::s11n_traits<member13type >::s11n_size(this->member13name)+               \
           inet::s11n_traits<member14type >::s11n_size(this->member14name);               \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
    inet::s11n_traits<member9type >::s11n(this->member9name, buffer);                     \
    inet::s11n_traits<member10type >::s11n(this->member10name, buffer);                   \
    inet::s11n_traits<member11type >::s11n(this->member11name, buffer);                   \
    inet::s11n_traits<member12type >::s11n(this->member12name, buffer);                   \
    inet::s11n_traits<member13type >::s11n(this->member13name, buffer);                   \
    inet::s11n_traits<member14type >::s11n(this->member14name, buffer);                   \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer)             \
        && inet::s11n_traits<member9type >::uns11n(this->member9name, buffer)             \
        && inet::s11n_traits<member10type >::uns11n(this->member10name, buffer)           \
        && inet::s11n_traits<member11type >::uns11n(this->member11name, buffer)           \
        && inet::s11n_traits<member12type >::uns11n(this->member12name, buffer)           \
        && inet::s11n_traits<member13type >::uns11n(this->member13name, buffer)           \
        && inet::s11n_traits<member14type >::uns11n(this->member14name, buffer);          \
}

#define INET_S11N_TRAITS_14(classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name,       \
member13type, member13name, member14type, member14name)                                   \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name)+            \
                   s11n_traits<member9type >::s11n_size(instance.member9name)+            \
                   s11n_traits<member10type >::s11n_size(instance.member10name)+          \
                   s11n_traits<member11type >::s11n_size(instance.member11name)+          \
                   s11n_traits<member12type >::s11n_size(instance.member12name)+          \
                   s11n_traits<member13type >::s11n_size(instance.member13name)+          \
                   s11n_traits<member14type >::s11n_size(instance.member14name);          \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
            s11n_traits<member9type >::s11n(instance.member9name, buffer);                \
            s11n_traits<member10type >::s11n(instance.member10name, buffer);              \
            s11n_traits<member11type >::s11n(instance.member11name, buffer);              \
            s11n_traits<member12type >::s11n(instance.member12name, buffer);              \
            s11n_traits<member13type >::s11n(instance.member13name, buffer);              \
            s11n_traits<member14type >::s11n(instance.member14name, buffer);              \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
            s11n_traits<member9type >::uns11n(instance.member9name, buffer);              \
            s11n_traits<member10type >::uns11n(instance.member10name, buffer);            \
            s11n_traits<member11type >::uns11n(instance.member11name, buffer);            \
            s11n_traits<member12type >::uns11n(instance.member12name, buffer);            \
            s11n_traits<member13type >::uns11n(instance.member13name, buffer);            \
            s11n_traits<member14type >::uns11n(instance.member14name, buffer);            \
        }                                                                                 \
    };                                                                                    \
}

#define INET_S11N_15(classname, member1type, member1name, member2type, member2name,       \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name,       \
member13type, member13name, member14type, member14name, member15type, member15name)       \
inet_uint32 __inet_s11n_size(const classname& instance) const                             \
{                                                                                         \
    return inet::s11n_traits<member1type >::s11n_size(this->member1name)+                 \
           inet::s11n_traits<member2type >::s11n_size(this->member2name)+                 \
           inet::s11n_traits<member3type >::s11n_size(this->member3name)+                 \
           inet::s11n_traits<member4type >::s11n_size(this->member4name)+                 \
           inet::s11n_traits<member5type >::s11n_size(this->member5name)+                 \
           inet::s11n_traits<member6type >::s11n_size(this->member6name)+                 \
           inet::s11n_traits<member7type >::s11n_size(this->member7name)+                 \
           inet::s11n_traits<member8type >::s11n_size(this->member8name)+                 \
           inet::s11n_traits<member9type >::s11n_size(this->member9name)+                 \
           inet::s11n_traits<member10type >::s11n_size(this->member10name)+               \
           inet::s11n_traits<member11type >::s11n_size(this->member11name)+               \
           inet::s11n_traits<member12type >::s11n_size(this->member12name)+               \
           inet::s11n_traits<member13type >::s11n_size(this->member13name)+               \
           inet::s11n_traits<member14type >::s11n_size(this->member14name);               \
           inet::s11n_traits<member15type >::s11n_size(this->member15name);               \
}                                                                                         \
void __inet_s11n(inet::buffer& buffer)                                                    \
{                                                                                         \
    inet::s11n_traits<member1type >::s11n(this->member1name, buffer);                     \
    inet::s11n_traits<member2type >::s11n(this->member2name, buffer);                     \
    inet::s11n_traits<member3type >::s11n(this->member3name, buffer);                     \
    inet::s11n_traits<member4type >::s11n(this->member4name, buffer);                     \
    inet::s11n_traits<member5type >::s11n(this->member5name, buffer);                     \
    inet::s11n_traits<member6type >::s11n(this->member6name, buffer);                     \
    inet::s11n_traits<member7type >::s11n(this->member7name, buffer);                     \
    inet::s11n_traits<member8type >::s11n(this->member8name, buffer);                     \
    inet::s11n_traits<member9type >::s11n(this->member9name, buffer);                     \
    inet::s11n_traits<member10type >::s11n(this->member10name, buffer);                   \
    inet::s11n_traits<member11type >::s11n(this->member11name, buffer);                   \
    inet::s11n_traits<member12type >::s11n(this->member12name, buffer);                   \
    inet::s11n_traits<member13type >::s11n(this->member13name, buffer);                   \
    inet::s11n_traits<member14type >::s11n(this->member14name, buffer);                   \
    inet::s11n_traits<member15type >::s11n(this->member15name, buffer);                   \
}                                                                                         \
bool __inet_uns11n(inet::buffer& buffer)                                                  \
{                                                                                         \
    return inet::s11n_traits<member1type >::uns11n(this->member1name, buffer)             \
        && inet::s11n_traits<member2type >::uns11n(this->member2name, buffer)             \
        && inet::s11n_traits<member3type >::uns11n(this->member3name, buffer)             \
        && inet::s11n_traits<member4type >::uns11n(this->member4name, buffer)             \
        && inet::s11n_traits<member5type >::uns11n(this->member5name, buffer)             \
        && inet::s11n_traits<member6type >::uns11n(this->member6name, buffer)             \
        && inet::s11n_traits<member7type >::uns11n(this->member7name, buffer)             \
        && inet::s11n_traits<member8type >::uns11n(this->member8name, buffer)             \
        && inet::s11n_traits<member9type >::uns11n(this->member9name, buffer)             \
        && inet::s11n_traits<member10type >::uns11n(this->member10name, buffer)           \
        && inet::s11n_traits<member11type >::uns11n(this->member11name, buffer)           \
        && inet::s11n_traits<member12type >::uns11n(this->member12name, buffer)           \
        && inet::s11n_traits<member13type >::uns11n(this->member13name, buffer)           \
        && inet::s11n_traits<member14type >::uns11n(this->member14name, buffer)           \
        && inet::s11n_traits<member15type >::uns11n(this->member15name, buffer);          \
}

#define INET_S11N_TRAITS_15(classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,\
member6name, member7type, member7name, member8type, member8name, member9type, member9name,\
member10type, member10name, member11type, member11name, member12type, member12name,       \
member13type, member13name, member14type, member14name, member15type, member15name)       \
namespace inet                                                                            \
{                                                                                         \
    template <>                                                                           \
    struct s11n_traits<classname >                                                        \
    {                                                                                     \
        static inet_uint32 s11n_size(const classname& instance)                           \
        {                                                                                 \
            return s11n_traits<member1type >::s11n_size(instance.member1name)+            \
                   s11n_traits<member2type >::s11n_size(instance.member2name)+            \
                   s11n_traits<member3type >::s11n_size(instance.member3name)+            \
                   s11n_traits<member4type >::s11n_size(instance.member4name)+            \
                   s11n_traits<member5type >::s11n_size(instance.member5name)+            \
                   s11n_traits<member6type >::s11n_size(instance.member6name)+            \
                   s11n_traits<member7type >::s11n_size(instance.member7name)+            \
                   s11n_traits<member8type >::s11n_size(instance.member8name)+            \
                   s11n_traits<member9type >::s11n_size(instance.member9name)+            \
                   s11n_traits<member10type >::s11n_size(instance.member10name)+          \
                   s11n_traits<member11type >::s11n_size(instance.member11name)+          \
                   s11n_traits<member12type >::s11n_size(instance.member12name)+          \
                   s11n_traits<member13type >::s11n_size(instance.member13name)+          \
                   s11n_traits<member14type >::s11n_size(instance.member14name)+          \
                   s11n_traits<member15type >::s11n_size(instance.member15name);          \
        }                                                                                 \
        static void s11n(classname& instance, inet::buffer& buffer)                       \
        {                                                                                 \
            s11n_traits<member1type >::s11n(instance.member1name, buffer);                \
            s11n_traits<member2type >::s11n(instance.member2name, buffer);                \
            s11n_traits<member3type >::s11n(instance.member3name, buffer);                \
            s11n_traits<member4type >::s11n(instance.member4name, buffer);                \
            s11n_traits<member5type >::s11n(instance.member5name, buffer);                \
            s11n_traits<member6type >::s11n(instance.member6name, buffer);                \
            s11n_traits<member7type >::s11n(instance.member7name, buffer);                \
            s11n_traits<member8type >::s11n(instance.member8name, buffer);                \
            s11n_traits<member9type >::s11n(instance.member9name, buffer);                \
            s11n_traits<member10type >::s11n(instance.member10name, buffer);              \
            s11n_traits<member11type >::s11n(instance.member11name, buffer);              \
            s11n_traits<member12type >::s11n(instance.member12name, buffer);              \
            s11n_traits<member13type >::s11n(instance.member13name, buffer);              \
            s11n_traits<member14type >::s11n(instance.member14name, buffer);              \
            s11n_traits<member15type >::s11n(instance.member15name, buffer);              \
        }                                                                                 \
        static void uns11n(classname& instance, inet::buffer& buffer)                     \
        {                                                                                 \
            s11n_traits<member1type >::uns11n(instance.member1name, buffer);              \
            s11n_traits<member2type >::uns11n(instance.member2name, buffer);              \
            s11n_traits<member3type >::uns11n(instance.member3name, buffer);              \
            s11n_traits<member4type >::uns11n(instance.member4name, buffer);              \
            s11n_traits<member5type >::uns11n(instance.member5name, buffer);              \
            s11n_traits<member6type >::uns11n(instance.member6name, buffer);              \
            s11n_traits<member7type >::uns11n(instance.member7name, buffer);              \
            s11n_traits<member8type >::uns11n(instance.member8name, buffer);              \
            s11n_traits<member9type >::uns11n(instance.member9name, buffer);              \
            s11n_traits<member10type >::uns11n(instance.member10name, buffer);            \
            s11n_traits<member11type >::uns11n(instance.member11name, buffer);            \
            s11n_traits<member12type >::uns11n(instance.member12name, buffer);            \
            s11n_traits<member13type >::uns11n(instance.member13name, buffer);            \
            s11n_traits<member14type >::uns11n(instance.member14name, buffer);            \
            s11n_traits<member15type >::uns11n(instance.member15name, buffer);            \
        }                                                                                 \
    };                                                                                    \
}

#endif // #ifndef __LIBINET_SERIALIZE_H__

