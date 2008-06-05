/**
 *  Version:     @(#)TypeCheck.hxx    0.0.1 28/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     A C++ object type check module. 
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty 
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 *  the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with this library; if not, write to the 
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
 *  Boston, MA 02111-1307  USA
 */

#ifndef __INET_TYPE_CHECK_H__
#define __INET_TYPE_CHECK_H__

#include "Compat.hxx"

namespace std // std type declared
{
    template <class FirstType, class SecondType> struct pair;
    template <class Type, class Traits, class AllocatorType> class basic_string;
    template <class Type, class AllocatorType> class vector;
    template <class Type, class AllocatorType> class deque;
    template <class Type, class AllocatorType> class list;
    template <class KeyType, class CompType, class AllocatorType> class set;
    template <class KeyType, class CompType, class AllocatorType> class multiset;
    template <class KeyType, class MappedType, class CompType, class AllocatorType> class map;
    template <class KeyType, class MappedType, class CompType, class AllocatorType> class multimap;
} // namespace std;

namespace INet
{
    template <bool Boolean, class Type = void>
    struct EnableIf { typedef Type type; }; 
    template <class Type> struct EnableIf<false, Type> {};

    template <class Type> struct IsInteger { static const bool value = false; };
    template <> struct IsInteger<bool> { static const bool value = true; };
    template <> struct IsInteger<char> { static const bool value = true; };
    template <> struct IsInteger<long> { static const bool value = true; };
    template <> struct IsInteger<unsigned long> { static const bool value = true; };
#ifndef _WIN32
    template <> struct IsInteger<Int8> { static const bool value = true; };
#endif
    template <> struct IsInteger<Int16> { static const bool value = true; };
    template <> struct IsInteger<Int32> { static const bool value = true; };
    template <> struct IsInteger<Int64> { static const bool value = true; };
    template <> struct IsInteger<UInt8> { static const bool value = true; };
    template <> struct IsInteger<UInt16> { static const bool value = true; };
    template <> struct IsInteger<UInt32> { static const bool value = true; };
    template <> struct IsInteger<UInt64> { static const bool value = true; };

    template <class Type> struct IsStdPair { static const bool value = false; };
    template <class FirstType, class SecondType> 
    struct IsStdPair<std::pair<FirstType, SecondType> > 
    { static const bool value = true; };

    template <class Type> struct IsStdBasicString { static const bool value = false; };
    template <class Type, class Traits, class AllocatorType>
    struct IsStdBasicString<std::basic_string<Type, Traits, AllocatorType> >
    { static const bool value = true; };

    template <class Type>
    struct IsStdVector { static const bool value = false; };
    template <class ValueType, class AllocatorType>
    struct IsStdVector<std::vector<ValueType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct IsStdDeque { static const bool value = false; }; 
    template <class ValueType, class AllocatorType>
    struct IsStdDeque<std::deque<ValueType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct IsStdList { static const bool value = false; };
    template <class ValueType, class AllocatorType>
    struct IsStdList<std::list<ValueType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct IsStdSet { static const bool value = false; }; 
    template <class KeyType, class CompType, class AllocatorType>
    struct IsStdSet<std::set<KeyType, CompType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct IsStdMultiset { static const bool value = false; };
    template <class KeyType, class CompType, class AllocatorType>
    struct IsStdMultiset<std::multiset<KeyType, CompType, AllocatorType> > 
    { static const bool value = true; };

    template <class Type>
    struct IsStdMap { static const bool value = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct IsStdMap<std::map<KeyType, MappedType, CompType, AllocatorType> >
    { static const bool value = true; };

    template <class Type>
    struct IsStdMultimap { static const bool value = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct IsStdMultimap<std::multimap<KeyType, MappedType, CompType, AllocatorType> >
    { static const bool value = true; };

    template <class Type, class Enable = void>
    struct IsStdSequenceContainer { static const bool value = false; }; 
    template <class Type> struct IsStdSequenceContainer<Type, 
    typename EnableIf<IsStdVector<Type>::value || IsStdList<Type>::value || 
    IsStdDeque<Type>::value || IsStdBasicString<Type>::value>::type>
    { static const bool value = true; };

    template <class Type, class Enable = void>
    struct IsStdAssociativeContainer { static const bool value = false; };
    template <class Type> struct IsStdAssociativeContainer<Type, 
    typename EnableIf<IsStdSet<Type>::value || IsStdMultiset<Type>::value || 
    IsStdSet<Type>::value || IsStdMultiset<Type>::value>::type>
    { static const bool value = true; };

    template <class Type, class Enable = void>
    struct IsStdContainer { static const bool value = false; };
    template <class Type> struct IsStdContainer<Type, 
    typename EnableIf<IsStdSequenceContainer<Type>::value || 
    IsStdAssociativeContainer<Type>::value>::type>
    { static const bool value = true; };
}

#endif // __INET_TYPE_CHECK_H__

