/**
 *  Version:     @(#)libinet/type_check.hxx    0.0.1 28/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     A C++ object type check module. 
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

#ifndef __LIBINET_TYPE_CHECK_H__
#define __LIBINET_TYPE_CHECK_H__

#include "compat.hxx"

namespace std // std type declared
{
    template <class FirstType, class SecondType> struct pair;
    template <class Type, class Traits, class AllocatorType> class basic_string;;
    template <class Type, class AllocatorType> class vector;
    template <class Type, class AllocatorType> class deque;
    template <class Type, class AllocatorType> class list;
    template <class KeyType, class CompType, class AllocatorType> class set;
    template <class KeyType, class CompType, class AllocatorType> class multiset;
    template <class KeyType, class MappedType, class CompType, class AllocatorType> class map;
    template <class KeyType, class MappedType, class CompType, class AllocatorType> class multimap;
} // namespace std;

namespace inet
{
    template <bool Boolean, class Type = void>
    struct enableif { typedef Type type; }; 
    template <class Type> struct enableif<false, Type> {};

    template <class Type> struct is_integer { static const bool value = false; };
    template <> struct is_integer<bool> { static const bool value = true; };
    template <> struct is_integer<long> { static const bool value = true; };
    template <> struct is_integer<unsigned long> { static const bool value = true; };
    template <> struct is_integer<wchar_t> { static const bool value = true; };
    template <> struct is_integer<inet_int8> { static const bool value = true; };
    template <> struct is_integer<inet_int16> { static const bool value = true; };
    template <> struct is_integer<inet_int32> { static const bool value = true; };
    template <> struct is_integer<inet_int64> { static const bool value = true; };
    template <> struct is_integer<inet_uint8> { static const bool value = true; };
    template <> struct is_integer<inet_uint16> { static const bool value = true; };
    template <> struct is_integer<inet_uint32> { static const bool value = true; };
    template <> struct is_integer<inet_uint64> { static const bool value = true; };
    
    template <class Type> struct is_std_pair { static const bool value = false; };
    template <class FirstType, class SecondType> 
    struct is_std_pair<std::pair<FirstType, SecondType> > 
    { static const bool value = true; };

    template <class Type> struct is_std_basic_string { static const bool value = false; };
    template <class Type, class Traits, class AllocatorType>
    struct is_std_basic_string<std::basic_string<Type, Traits, AllocatorType> >
    { static const bool value = true; };

    template <class Type>
    struct is_std_vector { static const bool value = false; };
    template <class ValueType, class AllocatorType>
    struct is_std_vector<std::vector<ValueType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct is_std_deque { static const bool value = false; }; 
    template <class ValueType, class AllocatorType>
    struct is_std_deque<std::deque<ValueType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct is_std_list { static const bool value = false; };
    template <class ValueType, class AllocatorType>
    struct is_std_list<std::list<ValueType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct is_std_set { static const bool value = false; }; 
    template <class KeyType, class CompType, class AllocatorType>
    struct is_std_set<std::set<KeyType, CompType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct is_std_multiset { static const bool value = false; };
    template <class KeyType, class CompType, class AllocatorType>
    struct is_std_multiset<std::multiset<KeyType, CompType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct is_std_map { static const bool value = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct is_std_map<std::map<KeyType, MappedType, CompType, AllocatorType> >
    { static const bool value = true; };

    template <class Type>
    struct is_std_multimap { static const bool value = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct is_std_multimap<std::multimap<KeyType, MappedType, CompType, AllocatorType> >
    { static const bool value = true; };

    //template <class Type, class Enable = void>
    //struct is_std_sequence_container { static const bool value = false; }; 
    //template <class Type> struct is_std_sequence_container<Type, 
    //typename enableif<is_std_vector<Type>::value || is_std_list<Type>::value || 
    //is_std_deque<Type>::value || is_std_basic_string<Type>::value>::type>
    //{ static const bool value = true; };

    // Due to improve string copy performance, we aren't conside std::string is a container.
    template <class Type, class Enable = void>
    struct is_std_sequence_container { static const bool value = false; }; 
    template <class Type> struct is_std_sequence_container<Type, 
    typename enableif<is_std_vector<Type>::value || is_std_list<Type>::value || 
    is_std_deque<Type>::value>::type>
    { static const bool value = true; };

    template <class Type, class Enable = void>
    struct is_std_associative_container { static const bool value = false; };
    template <class Type> struct is_std_associative_container<Type, 
    typename enableif<is_std_set<Type>::value || is_std_multiset<Type>::value || 
    is_std_map<Type>::value || is_std_multimap<Type>::value>::type>
    { static const bool value = true; };

    template <class Type, class Enable = void>
    struct is_std_container { static const bool value = false; };
    template <class Type> struct is_std_container<Type, 
    typename enableif<is_std_sequence_container<Type>::value || 
    is_std_associative_container<Type>::value>::type>
    { static const bool value = true; };
}

#endif // #ifndef __LIBINET_TYPE_CHECK_H__

