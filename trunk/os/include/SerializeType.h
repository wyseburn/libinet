/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_SERIALIZE_TYPE_H
#define IGAME_OS_SERIALIZE_TYPE_H

#include "Compat.h"

namespace std // std type declared
{
    template <class FirstType, class SecondType > struct pair;
    template <class Type, class Traits, class AllocatorType > class basic_string;;
    template <class Type, class AllocatorType > class vector;
    template <class Type, class AllocatorType > class deque;
    template <class Type, class AllocatorType > class list;
    template <class KeyType, class CompType, class AllocatorType > class set;
    template <class KeyType, class CompType, class AllocatorType > class multiset;
    template <class KeyType, class MappedType, class CompType, class AllocatorType > class map;
    template <class KeyType, class MappedType, class CompType, class AllocatorType > class multimap;
} // namespace std;

namespace IGame
{
    template <bool Boolean, class Type = void>
    struct EnableIf { typedef Type type; }; 
    template <class Type> struct EnableIf<false, Type > {};

    template <class Type> struct IsBoolean { static const bool s_Value = false; };
    template <> struct IsBoolean<bool> { static const bool s_Value = true; };

    template <class Type> struct IsInteger { static const bool s_Value = false; };
    template <> struct IsInteger<wchar_t> { static const bool s_Value = true; };
    template <> struct IsInteger<long> { static const bool s_Value = true; };
    template <> struct IsInteger<unsigned long> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int8> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int16> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int32> { static const bool s_Value = true; };
    template <> struct IsInteger<_Int64> { static const bool s_Value = true; };
    template <> struct IsInteger<_UInt8> { static const bool s_Value = true; };
    template <> struct IsInteger<_UInt16> { static const bool s_Value = true; };
    template <> struct IsInteger<_UInt32> { static const bool s_Value = true; };
    template <> struct IsInteger<_UInt64> { static const bool s_Value = true; };
    template <> struct IsInteger<void*> { static const bool s_Value = true; };

    template <class Type> struct IsFloat { static const bool s_Value = false; };
    template <> struct IsFloat<float> { static const bool s_Value = true; };

    template <class Type> struct IsDouble { static const bool s_Value = false; };
    template <> struct IsDouble<double> { static const bool s_Value = true; };

    template <class Type> struct IsStdBasicString { static const bool s_Value = false; };
    template <class Type, class Traits, class AllocatorType>
    struct IsStdBasicString<std::basic_string<Type, Traits, AllocatorType > >
    { static const bool s_Value = true; };

    template <class Type>
    struct IsStdVector { static const bool s_Value = false; };
    template <class ValueType, class AllocatorType>
    struct IsStdVector<std::vector<ValueType, AllocatorType > > 
    { static const bool s_Value = true; };

    template <class Type>
    struct IsStdDeque { static const bool s_Value = false; }; 
    template <class ValueType, class AllocatorType>
    struct IsStdDeque<std::deque<ValueType, AllocatorType > > 
    { static const bool s_Value = true; };

    template <class Type>
    struct IsStdList { static const bool s_Value = false; };
    template <class ValueType, class AllocatorType>
    struct IsStdList<std::list<ValueType, AllocatorType > > 
    { static const bool s_Value = true; };

    template <class Type>
    struct IsStdSet { static const bool s_Value = false; }; 
    template <class KeyType, class CompType, class AllocatorType>
    struct IsStdSet<std::set<KeyType, CompType, AllocatorType > > 
    { static const bool s_Value = true; };

    template <class Type>
    struct IsStdMultiset { static const bool s_Value = false; };
    template <class KeyType, class CompType, class AllocatorType>
    struct IsStdMultiset<std::multiset<KeyType, CompType, AllocatorType > > 
    { static const bool s_Value = true; };

    template <class Type, class Enable = void>
    struct IsStdSetContainer { static const bool s_Value = false; }; 
    template <class Type> struct IsStdSetContainer<Type, 
    typename EnableIf<IsStdSet<Type >::s_Value || IsStdMultiset<Type >::s_Value>::type>
    { static const bool s_Value = true; };

    template <class Type> struct IsStdPair { static const bool s_Value = false; };
    template <class FirstType, class SecondType> 
    struct IsStdPair<std::pair<FirstType, SecondType > > 
    { static const bool s_Value = true; };

    template <class Type>
    struct IsStdMap { static const bool s_Value = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct IsStdMap<std::map<KeyType, MappedType, CompType, AllocatorType > >
    { static const bool s_Value = true; };

    template <class Type>
    struct IsStdMultimap { static const bool s_Value = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct IsStdMultimap<std::multimap<KeyType, MappedType, CompType, AllocatorType > >
    { static const bool s_Value = true; };

    template <class Type, class Enable = void>
    struct IsStdMapContainer { static const bool s_Value = false; }; 
    template <class Type> struct IsStdMapContainer<Type, 
    typename EnableIf<IsStdMap<Type >::s_Value || IsStdMultimap<Type >::s_Value>::type>
    { static const bool s_Value = true; };

    //template <class Type, class Enable = void>
    //struct IsStdSequenceContainer { static const bool s_Value = false; }; 
    //template <class Type> struct IsStdSequenceContainer<Type, 
    //typename EnableIf<IsStdVector<Type >::s_Value || IsStdList<Type >::s_Value || 
    //IsStdDeque<Type >::s_Value || IsStdBasicString<Type >::s_Value>::type>
    //{ static const bool s_Value = true; };

    // Due to improve string copy performance, we aren't conside string is a container.
    template <class Type, class Enable = void>
    struct IsStdSequenceContainer { static const bool s_Value = false; }; 
    template <class Type> struct IsStdSequenceContainer<Type, 
    typename EnableIf<IsStdVector<Type >::s_Value || IsStdList<Type >::s_Value || 
    IsStdDeque<Type >::s_Value>::type>
    { static const bool s_Value = true; };

    template <class Type, class Enable = void>
    struct IsStdAssociativeContainer { static const bool s_Value = false; };
    template <class Type> struct IsStdAssociativeContainer<Type, 
    typename EnableIf<IsStdSetContainer<Type >::s_Value || IsStdMapContainer<Type >::s_Value>::type>
    { static const bool s_Value = true; };

    template <class Type, class Enable = void>
    struct IsStdContainer { static const bool s_Value = false; };
    template <class Type> struct IsStdContainer<Type, 
    typename EnableIf<IsStdSequenceContainer<Type >::s_Value || 
    IsStdAssociativeContainer<Type >::s_Value>::type>
    { static const bool s_Value = true; };
} // namespace IGame

#endif // #ifndef IGAME_OS_SERIALIZE_TYPE_H


