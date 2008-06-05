/**
 *  Version:     @(#)Serialize.hxx    0.0.1 29/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     C++ object serialize implement 
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

#ifndef __INET_SERIALIZE_H__
#define __INET_SERIALIZE_H__

#include "Buffer.hxx"
#include "TypeCheck.hxx"

namespace INet
{
    template <class Type, class Enable = void>
    struct SerializeTraits
    {
        static UInt32 serializedSize(const Type& instance)
        {
            return instance.__inetSerializedSize(instance);
        }

        static void serialize(Type& instance, Buffer& buf)
        {
            instance.__inetSerialize(buf);
        }

        static void unserialize(Type& instance, Buffer& buf)
        {
            instance.__inetUnserialize(buf);
        }
    };

    template <class Type>
    struct SerializeTraits<Type, typename EnableIf<IsInteger<Type >::value >::type >
    {
        static UInt32 serializedSize() { return sizeof(Type); }
        static UInt32 serializedSize(const Type& instance) { return sizeof(instance); }

        static void serialize(const Type& instance, Buffer& buf)
        {
            buf << (Type)instance;
        }

        static void unserialize(Type& instance, Buffer& buf)
        {
            buf >> instance;
        }
    };

    template <class Type>
    struct SerializeTraits<Type, typename EnableIf<IsStdPair<Type >::value >::type >
    {
        typedef typename Type::first_type FirstType;
        typedef typename Type::second_type SecondType;

        static UInt32 serializedSize(const Type& instance)
        {
            return SerializeTraits<FirstType >::serializedSize(instance.first) +
                   SerializeTraits<SecondType >::serializedSize(instance.second);
        }

        static void serialize(const Type& instance, Buffer& buf)
        {
            SerializeTraits<FirstType >::serialize(instance.first, buf);
            SerializeTraits<SecondType >::serialize(instance.second, buf);
        }

        static void unserialize(Type& instance, Buffer& buf)
        {
            SerializeTraits<FirstType >::unserialize(instance.first, buf);
            SerializeTraits<SecondType >::unserialize(instance.second, buf);
        }
    };

    template <class Type>
    struct SerializeTraits<Type, typename EnableIf<IsStdContainer<Type >::value >::type >
    {
        typedef typename Type::value_type ElemType;
        typedef typename Type::const_iterator IteratorType;

        static UInt32 serializedSize(const Type& instance)
        {
            UInt32 size = SerializeTraits<UInt32>::serializedSize();
            
            for (IteratorType it = instance.begin(); it != instance.end(); ++it)
            {
                size += SerializeTraits<ElemType >::serializedSize(*it);
            }
            return size;
        }

        static void serialize(const Type& instance, Buffer& buf)
        {
            buf << (UInt32)instance.size();
            for (IteratorType it = instance.begin(); it != instance.end(); ++it)
            {
                SerializeTraits<ElemType >::serialize(*it, buf);
            }
        }

        static void unserialize(Type& instance, Buffer& buf)
        {
            UInt32 size = 0;
            buf >> size;

            for (; size; --size)
            {
                ElemType elem;
                SerializeTraits<ElemType >::unserialize(elem, buf);
                instance.push_back(elem);
            }
        }
    };

    template <class Type>
    static UInt32 serializedSize(const Type& instance)
    {
        return SerializeTraits<Type >::serializedSize(instance);
    }

    template <class Type>
    static bool serialize(const Type& instance, Buffer& buf, bool check = true)
    {
        /*if (check == true)
        {
            if (SerializeTraits<Type >::serializedSize(instance) > buf.length())  
            {
                return false;
            }
        }*/
        
        SerializeTraits<Type >::serialize((Type&)instance, buf);
        return true;
    }

    
    template <class Type>
    static bool unserialize(Type& instance, Buffer& buf)
    {
        /*if (SerializeTraits<Type >::serializedSize(instance) > buf.length())  
        {
            return false;
        }*/
        
        SerializeTraits<Type >::unserialize(instance, buf);
        return true;
    }
}

#define INET_S11N_1(ClassName, Member1Type, Member1Name)                                   \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);         \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);       \
}

#define INET_S11N_TRAITS_1(ClassName, Member1Type, Member1Name)                            \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_2(ClassName, Member1Type, Member1Name, Member2Type, Member2Name)         \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
}

#define INET_S11N_TRAITS_2(ClassName, Member1Type, Member1Name, Member2Type, Member2Name)  \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_3(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,         \
Member3Type, Member3Name)                                                                  \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
}

#define INET_S11N_TRAITS_3(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,  \
Member3Type, Member3Name)                                                                  \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_4(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,         \
Member3Type, Member3Name, Member4Type, Member4Name)                                        \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
}

#define INET_S11N_TRAITS_4(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,  \
Member3Type, Member3Name, Member4Type, Member4Name)                                        \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_5(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,         \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name)              \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
}

#define INET_S11N_TRAITS_5(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,  \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name)              \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_6(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,         \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name)                                                                               \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
}

#define INET_S11N_TRAITS_6(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,  \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name)                                                                               \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_7(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,         \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name)                                                     \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
}

#define INET_S11N_TRAITS_7(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,  \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name)                                                     \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_8(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,         \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name)                           \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
}

#define INET_S11N_TRAITS_8(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,  \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name)                           \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_9(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,         \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name) \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name)+         \
           INet::SerializeTraits<Member9Type >::serializedSize(this->Member9Name);         \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
    INet::SerializeTraits<Member9Type >::serialize(this->Member9Name, buf);                \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
    INet::SerializeTraits<Member9Type >::unserialize(this->Member9Name, buf);              \
}

#define INET_S11N_TRAITS_9(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,  \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name) \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name)+    \
                   SerializeTraits<Member9Type >::serializedSize(instance.Member9Name);    \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
            SerializeTraits<Member9Type >::serialize(instance.Member9Name, buf);           \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
            SerializeTraits<Member9Type >::unserialize(instance.Member9Name, buf);         \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_10(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,        \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name)                                                                \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name)+         \
           INet::SerializeTraits<Member9Type >::serializedSize(this->Member9Name)+         \
           INet::SerializeTraits<Member10Type >::serializedSize(this->Member10Name);       \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
    INet::SerializeTraits<Member9Type >::serialize(this->Member9Name, buf);                \
    INet::SerializeTraits<Member10Type >::serialize(this->Member10Name, buf);              \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
    INet::SerializeTraits<Member9Type >::unserialize(this->Member9Name, buf);              \
    INet::SerializeTraits<Member10Type >::unserialize(this->Member10Name, buf);            \
}

#define INET_S11N_TRAITS_10(ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name)                                                                \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name)+    \
                   SerializeTraits<Member9Type >::serializedSize(instance.Member9Name)+    \
                   SerializeTraits<Member10Type >::serializedSize(instance.Member10Name);  \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
            SerializeTraits<Member9Type >::serialize(instance.Member9Name, buf);           \
            SerializeTraits<Member10Type >::serialize(instance.Member10Name, buf);         \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
            SerializeTraits<Member9Type >::unserialize(instance.Member9Name, buf);         \
            SerializeTraits<Member10Type >::unserialize(instance.Member10Name, buf);       \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_11(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,        \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name)                                    \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name)+         \
           INet::SerializeTraits<Member9Type >::serializedSize(this->Member9Name)+         \
           INet::SerializeTraits<Member10Type >::serializedSize(this->Member10Name)+       \
           INet::SerializeTraits<Member11Type >::serializedSize(this->Member11Name);       \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
    INet::SerializeTraits<Member9Type >::serialize(this->Member9Name, buf);                \
    INet::SerializeTraits<Member10Type >::serialize(this->Member10Name, buf);              \
    INet::SerializeTraits<Member11Type >::serialize(this->Member11Name, buf);              \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
    INet::SerializeTraits<Member9Type >::unserialize(this->Member9Name, buf);              \
    INet::SerializeTraits<Member10Type >::unserialize(this->Member10Name, buf);            \
    INet::SerializeTraits<Member11Type >::unserialize(this->Member11Name, buf);            \
}

#define INET_S11N_TRAITS_11(ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name)                                    \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name)+    \
                   SerializeTraits<Member9Type >::serializedSize(instance.Member9Name)+    \
                   SerializeTraits<Member10Type >::serializedSize(instance.Member10Name)+  \
                   SerializeTraits<Member11Type >::serializedSize(instance.Member11Name);  \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
            SerializeTraits<Member9Type >::serialize(instance.Member9Name, buf);           \
            SerializeTraits<Member10Type >::serialize(instance.Member10Name, buf);         \
            SerializeTraits<Member11Type >::serialize(instance.Member11Name, buf);         \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
            SerializeTraits<Member9Type >::unserialize(instance.Member9Name, buf);         \
            SerializeTraits<Member10Type >::unserialize(instance.Member10Name, buf);       \
            SerializeTraits<Member11Type >::unserialize(instance.Member11Name, buf);       \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_12(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,        \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name)        \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name)+         \
           INet::SerializeTraits<Member9Type >::serializedSize(this->Member9Name)+         \
           INet::SerializeTraits<Member10Type >::serializedSize(this->Member10Name)+       \
           INet::SerializeTraits<Member11Type >::serializedSize(this->Member11Name)+       \
           INet::SerializeTraits<Member12Type >::serializedSize(this->Member12Name);       \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
    INet::SerializeTraits<Member9Type >::serialize(this->Member9Name, buf);                \
    INet::SerializeTraits<Member10Type >::serialize(this->Member10Name, buf);              \
    INet::SerializeTraits<Member11Type >::serialize(this->Member11Name, buf);              \
    INet::SerializeTraits<Member12Type >::serialize(this->Member12Name, buf);              \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
    INet::SerializeTraits<Member9Type >::unserialize(this->Member9Name, buf);              \
    INet::SerializeTraits<Member10Type >::unserialize(this->Member10Name, buf);            \
    INet::SerializeTraits<Member11Type >::unserialize(this->Member11Name, buf);            \
    INet::SerializeTraits<Member12Type >::unserialize(this->Member12Name, buf);            \
}

#define INET_S11N_TRAITS_12(ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Nmae)        \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name)+    \
                   SerializeTraits<Member9Type >::serializedSize(instance.Member9Name)+    \
                   SerializeTraits<Member10Type >::serializedSize(instance.Member10Name)+  \
                   SerializeTraits<Member11Type >::serializedSize(instance.Member11Name)+  \
                   SerializeTraits<Member12Type >::serializedSize(instance.Member12Name);  \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
            SerializeTraits<Member9Type >::serialize(instance.Member9Name, buf);           \
            SerializeTraits<Member10Type >::serialize(instance.Member10Name, buf);         \
            SerializeTraits<Member11Type >::serialize(instance.Member11Name, buf);         \
            SerializeTraits<Member12Type >::serialize(instance.Member12Name, buf);         \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
            SerializeTraits<Member9Type >::unserialize(instance.Member9Name, buf);         \
            SerializeTraits<Member10Type >::unserialize(instance.Member10Name, buf);       \
            SerializeTraits<Member11Type >::unserialize(instance.Member11Name, buf);       \
            SerializeTraits<Member12Type >::unserialize(instance.Member12Name, buf);       \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_13(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,        \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,        \
Member13Type, Member13Name)                                                                \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name)+         \
           INet::SerializeTraits<Member9Type >::serializedSize(this->Member9Name)+         \
           INet::SerializeTraits<Member10Type >::serializedSize(this->Member10Name)+       \
           INet::SerializeTraits<Member11Type >::serializedSize(this->Member11Name)+       \
           INet::SerializeTraits<Member12Type >::serializedSize(this->Member12Name)+       \
           INet::SerializeTraits<Member13Type >::serializedSize(this->Member13Name);       \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
    INet::SerializeTraits<Member9Type >::serialize(this->Member9Name, buf);                \
    INet::SerializeTraits<Member10Type >::serialize(this->Member10Name, buf);              \
    INet::SerializeTraits<Member11Type >::serialize(this->Member11Name, buf);              \
    INet::SerializeTraits<Member12Type >::serialize(this->Member12Name, buf);              \
    INet::SerializeTraits<Member13Type >::serialize(this->Member13Name, buf);              \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
    INet::SerializeTraits<Member9Type >::unserialize(this->Member9Name, buf);              \
    INet::SerializeTraits<Member10Type >::unserialize(this->Member10Name, buf);            \
    INet::SerializeTraits<Member11Type >::unserialize(this->Member11Name, buf);            \
    INet::SerializeTraits<Member12Type >::unserialize(this->Member12Name, buf);            \
    INet::SerializeTraits<Member13Type >::unserialize(this->Member13Name, buf);            \
}

#define INET_S11N_TRAITS_13(ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Nmae,        \
Member13Type, Member13Name)                                                                \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name)+    \
                   SerializeTraits<Member9Type >::serializedSize(instance.Member9Name)+    \
                   SerializeTraits<Member10Type >::serializedSize(instance.Member10Name)+  \
                   SerializeTraits<Member11Type >::serializedSize(instance.Member11Name)+  \
                   SerializeTraits<Member12Type >::serializedSize(instance.Member12Name)+  \
                   SerializeTraits<Member13Type >::serializedSize(instance.Member13Name);  \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
            SerializeTraits<Member9Type >::serialize(instance.Member9Name, buf);           \
            SerializeTraits<Member10Type >::serialize(instance.Member10Name, buf);         \
            SerializeTraits<Member11Type >::serialize(instance.Member11Name, buf);         \
            SerializeTraits<Member12Type >::serialize(instance.Member12Name, buf);         \
            SerializeTraits<Member13Type >::serialize(instance.Member13Name, buf);         \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
            SerializeTraits<Member9Type >::unserialize(instance.Member9Name, buf);         \
            SerializeTraits<Member10Type >::unserialize(instance.Member10Name, buf);       \
            SerializeTraits<Member11Type >::unserialize(instance.Member11Name, buf);       \
            SerializeTraits<Member12Type >::unserialize(instance.Member12Name, buf);       \
            SerializeTraits<Member13Type >::unserialize(instance.Member13Name, buf);       \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_14(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,        \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,        \
Member13Type, Member13Name, Member14Type, Member14Name)                                    \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name)+         \
           INet::SerializeTraits<Member9Type >::serializedSize(this->Member9Name)+         \
           INet::SerializeTraits<Member10Type >::serializedSize(this->Member10Name)+       \
           INet::SerializeTraits<Member11Type >::serializedSize(this->Member11Name)+       \
           INet::SerializeTraits<Member12Type >::serializedSize(this->Member12Name)+       \
           INet::SerializeTraits<Member13Type >::serializedSize(this->Member13Name)+       \
           INet::SerializeTraits<Member14Type >::serializedSize(this->Member14Name);       \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
    INet::SerializeTraits<Member9Type >::serialize(this->Member9Name, buf);                \
    INet::SerializeTraits<Member10Type >::serialize(this->Member10Name, buf);              \
    INet::SerializeTraits<Member11Type >::serialize(this->Member11Name, buf);              \
    INet::SerializeTraits<Member12Type >::serialize(this->Member12Name, buf);              \
    INet::SerializeTraits<Member13Type >::serialize(this->Member13Name, buf);              \
    INet::SerializeTraits<Member14Type >::serialize(this->Member14Name, buf);              \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
    INet::SerializeTraits<Member9Type >::unserialize(this->Member9Name, buf);              \
    INet::SerializeTraits<Member10Type >::unserialize(this->Member10Name, buf);            \
    INet::SerializeTraits<Member11Type >::unserialize(this->Member11Name, buf);            \
    INet::SerializeTraits<Member12Type >::unserialize(this->Member12Name, buf);            \
    INet::SerializeTraits<Member13Type >::unserialize(this->Member13Name, buf);            \
    INet::SerializeTraits<Member14Type >::unserialize(this->Member14Name, buf);            \
}

#define INET_S11N_TRAITS_14(ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Nmae,        \
Member13Type, Member13Name, Member14Type, Member14Name)                                    \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name)+    \
                   SerializeTraits<Member9Type >::serializedSize(instance.Member9Name)+    \
                   SerializeTraits<Member10Type >::serializedSize(instance.Member10Name)+  \
                   SerializeTraits<Member11Type >::serializedSize(instance.Member11Name)+  \
                   SerializeTraits<Member12Type >::serializedSize(instance.Member12Name)+  \
                   SerializeTraits<Member13Type >::serializedSize(instance.Member13Name)+  \
                   SerializeTraits<Member14Type >::serializedSize(instance.Member14Name);  \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
            SerializeTraits<Member9Type >::serialize(instance.Member9Name, buf);           \
            SerializeTraits<Member10Type >::serialize(instance.Member10Name, buf);         \
            SerializeTraits<Member11Type >::serialize(instance.Member11Name, buf);         \
            SerializeTraits<Member12Type >::serialize(instance.Member12Name, buf);         \
            SerializeTraits<Member13Type >::serialize(instance.Member13Name, buf);         \
            SerializeTraits<Member14Type >::serialize(instance.Member14Name, buf);         \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
            SerializeTraits<Member9Type >::unserialize(instance.Member9Name, buf);         \
            SerializeTraits<Member10Type >::unserialize(instance.Member10Name, buf);       \
            SerializeTraits<Member11Type >::unserialize(instance.Member11Name, buf);       \
            SerializeTraits<Member12Type >::unserialize(instance.Member12Name, buf);       \
            SerializeTraits<Member13Type >::unserialize(instance.Member13Name, buf);       \
            SerializeTraits<Member14Type >::unserialize(instance.Member14Name, buf);       \
        }                                                                                  \
    };                                                                                     \
}

#define INET_S11N_15(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,        \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,        \
Member13Type, Member13Name, Member14Type, Member14Name, Member15Type, Member15Name)        \
UInt32 __inetSerializedSize(const ClassName& instance) const                               \
{                                                                                          \
    return INet::SerializeTraits<Member1Type >::serializedSize(this->Member1Name)+         \
           INet::SerializeTraits<Member2Type >::serializedSize(this->Member2Name)+         \
           INet::SerializeTraits<Member3Type >::serializedSize(this->Member3Name)+         \
           INet::SerializeTraits<Member4Type >::serializedSize(this->Member4Name)+         \
           INet::SerializeTraits<Member5Type >::serializedSize(this->Member5Name)+         \
           INet::SerializeTraits<Member6Type >::serializedSize(this->Member6Name)+         \
           INet::SerializeTraits<Member7Type >::serializedSize(this->Member7Name)+         \
           INet::SerializeTraits<Member8Type >::serializedSize(this->Member8Name)+         \
           INet::SerializeTraits<Member9Type >::serializedSize(this->Member9Name)+         \
           INet::SerializeTraits<Member10Type >::serializedSize(this->Member10Name)+       \
           INet::SerializeTraits<Member11Type >::serializedSize(this->Member11Name)+       \
           INet::SerializeTraits<Member12Type >::serializedSize(this->Member12Name)+       \
           INet::SerializeTraits<Member13Type >::serializedSize(this->Member13Name)+       \
           INet::SerializeTraits<Member14Type >::serializedSize(this->Member14Name);       \
           INet::SerializeTraits<Member15Type >::serializedSize(this->Member15Name);       \
}                                                                                          \
void __inetSerialize(INet::Buffer& buf)                                                    \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::serialize(this->Member1Name, buf);                \
    INet::SerializeTraits<Member2Type >::serialize(this->Member2Name, buf);                \
    INet::SerializeTraits<Member3Type >::serialize(this->Member3Name, buf);                \
    INet::SerializeTraits<Member4Type >::serialize(this->Member4Name, buf);                \
    INet::SerializeTraits<Member5Type >::serialize(this->Member5Name, buf);                \
    INet::SerializeTraits<Member6Type >::serialize(this->Member6Name, buf);                \
    INet::SerializeTraits<Member7Type >::serialize(this->Member7Name, buf);                \
    INet::SerializeTraits<Member8Type >::serialize(this->Member8Name, buf);                \
    INet::SerializeTraits<Member9Type >::serialize(this->Member9Name, buf);                \
    INet::SerializeTraits<Member10Type >::serialize(this->Member10Name, buf);              \
    INet::SerializeTraits<Member11Type >::serialize(this->Member11Name, buf);              \
    INet::SerializeTraits<Member12Type >::serialize(this->Member12Name, buf);              \
    INet::SerializeTraits<Member13Type >::serialize(this->Member13Name, buf);              \
    INet::SerializeTraits<Member14Type >::serialize(this->Member14Name, buf);              \
    INet::SerializeTraits<Member15Type >::serialize(this->Member15Name, buf);              \
}                                                                                          \
void __inetUnserialize(INet::Buffer& buf)                                                  \
{                                                                                          \
    INet::SerializeTraits<Member1Type >::unserialize(this->Member1Name, buf);              \
    INet::SerializeTraits<Member2Type >::unserialize(this->Member2Name, buf);              \
    INet::SerializeTraits<Member3Type >::unserialize(this->Member3Name, buf);              \
    INet::SerializeTraits<Member4Type >::unserialize(this->Member4Name, buf);              \
    INet::SerializeTraits<Member5Type >::unserialize(this->Member5Name, buf);              \
    INet::SerializeTraits<Member6Type >::unserialize(this->Member6Name, buf);              \
    INet::SerializeTraits<Member7Type >::unserialize(this->Member7Name, buf);              \
    INet::SerializeTraits<Member8Type >::unserialize(this->Member8Name, buf);              \
    INet::SerializeTraits<Member9Type >::unserialize(this->Member9Name, buf);              \
    INet::SerializeTraits<Member10Type >::unserialize(this->Member10Name, buf);            \
    INet::SerializeTraits<Member11Type >::unserialize(this->Member11Name, buf);            \
    INet::SerializeTraits<Member12Type >::unserialize(this->Member12Name, buf);            \
    INet::SerializeTraits<Member13Type >::unserialize(this->Member13Name, buf);            \
    INet::SerializeTraits<Member14Type >::unserialize(this->Member14Name, buf);            \
    INet::SerializeTraits<Member15Type >::unserialize(this->Member15Name, buf);            \
}

#define INET_S11N_TRAITS_15(ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type, \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name, \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,        \
Member13Type, Member13Name, Member14Type, Member14Name, Member15Type, Member15Name)        \
namespace INet                                                                             \
{                                                                                          \
    template <>                                                                            \
    struct SerializeTraits<ClassName >                                                     \
    {                                                                                      \
        static UInt32 serializedSize(const ClassName& instance)                            \
        {                                                                                  \
            return SerializeTraits<Member1Type >::serializedSize(instance.Member1Name)+    \
                   SerializeTraits<Member2Type >::serializedSize(instance.Member2Name)+    \
                   SerializeTraits<Member3Type >::serializedSize(instance.Member3Name)+    \
                   SerializeTraits<Member4Type >::serializedSize(instance.Member4Name)+    \
                   SerializeTraits<Member5Type >::serializedSize(instance.Member5Name)+    \
                   SerializeTraits<Member6Type >::serializedSize(instance.Member6Name)+    \
                   SerializeTraits<Member7Type >::serializedSize(instance.Member7Name)+    \
                   SerializeTraits<Member8Type >::serializedSize(instance.Member8Name)+    \
                   SerializeTraits<Member9Type >::serializedSize(instance.Member9Name)+    \
                   SerializeTraits<Member10Type >::serializedSize(instance.Member10Name)+  \
                   SerializeTraits<Member11Type >::serializedSize(instance.Member11Name)+  \
                   SerializeTraits<Member12Type >::serializedSize(instance.Member12Name)+  \
                   SerializeTraits<Member13Type >::serializedSize(instance.Member13Name)+  \
                   SerializeTraits<Member14Type >::serializedSize(instance.Member14Name)+  \
                   SerializeTraits<Member15Type >::serializedSize(instance.Member15Name);  \
        }                                                                                  \
        static void serialize(ClassName& instance, Buffer& buf)                            \
        {                                                                                  \
            SerializeTraits<Member1Type >::serialize(instance.Member1Name, buf);           \
            SerializeTraits<Member2Type >::serialize(instance.Member2Name, buf);           \
            SerializeTraits<Member3Type >::serialize(instance.Member3Name, buf);           \
            SerializeTraits<Member4Type >::serialize(instance.Member4Name, buf);           \
            SerializeTraits<Member5Type >::serialize(instance.Member5Name, buf);           \
            SerializeTraits<Member6Type >::serialize(instance.Member6Name, buf);           \
            SerializeTraits<Member7Type >::serialize(instance.Member7Name, buf);           \
            SerializeTraits<Member8Type >::serialize(instance.Member8Name, buf);           \
            SerializeTraits<Member9Type >::serialize(instance.Member9Name, buf);           \
            SerializeTraits<Member10Type >::serialize(instance.Member10Name, buf);         \
            SerializeTraits<Member11Type >::serialize(instance.Member11Name, buf);         \
            SerializeTraits<Member12Type >::serialize(instance.Member12Name, buf);         \
            SerializeTraits<Member13Type >::serialize(instance.Member13Name, buf);         \
            SerializeTraits<Member14Type >::serialize(instance.Member14Name, buf);         \
            SerializeTraits<Member15Type >::serialize(instance.Member15Name, buf);         \
        }                                                                                  \
        static void unserialize(ClassName& instance, Buffer& buf)                          \
        {                                                                                  \
            SerializeTraits<Member1Type >::unserialize(instance.Member1Name, buf);         \
            SerializeTraits<Member2Type >::unserialize(instance.Member2Name, buf);         \
            SerializeTraits<Member3Type >::unserialize(instance.Member3Name, buf);         \
            SerializeTraits<Member4Type >::unserialize(instance.Member4Name, buf);         \
            SerializeTraits<Member5Type >::unserialize(instance.Member5Name, buf);         \
            SerializeTraits<Member6Type >::unserialize(instance.Member6Name, buf);         \
            SerializeTraits<Member7Type >::unserialize(instance.Member7Name, buf);         \
            SerializeTraits<Member8Type >::unserialize(instance.Member8Name, buf);         \
            SerializeTraits<Member9Type >::unserialize(instance.Member9Name, buf);         \
            SerializeTraits<Member10Type >::unserialize(instance.Member10Name, buf);       \
            SerializeTraits<Member11Type >::unserialize(instance.Member11Name, buf);       \
            SerializeTraits<Member12Type >::unserialize(instance.Member12Name, buf);       \
            SerializeTraits<Member13Type >::unserialize(instance.Member13Name, buf);       \
            SerializeTraits<Member14Type >::unserialize(instance.Member14Name, buf);       \
            SerializeTraits<Member15Type >::unserialize(instance.Member15Name, buf);       \
        }                                                                                  \
    };                                                                                     \
}

#endif // #ifndef __INET_SERIALIZE_H__

