/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_SERIALIZE_TRAITS_H
#define IGAME_OS_SERIALIZE_TRAITS_H

namespace IGame
{
    template <class ObjType, class Enable = void>
    struct S11NTraits
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj)
        {
            return obj.IGameSerializeSize(data, obj);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return obj.IGameSerialize(data, obj, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return obj.IGameUnserialize(data, obj, name, mask);
        }
    };

    template <class DataType, class Enable = void>
    struct ObjectInit
    {
        static DataType& Init(DataType& data, const char* name)
        {
            return data;
        }
    };

    template <class DataType, class Enable = void>
    struct ObjectUninit
    {
        static DataType* Uninit(DataType& data, const char* name)
        {
            return &data;
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsBoolean<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NBooleanImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NBooleanImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NBooleanImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsInteger<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NIntegerImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIntegerImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIntegerImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsFloat<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NFloatImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NFloatImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NFloatImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsDouble<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NDoubleImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NDoubleImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NDoubleImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    // STL container data type.

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsStdBasicString<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NStdBasicStringImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdBasicStringImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdBasicStringImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsStdPair<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NStdPairImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdPairImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdPairImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsStdMapContainer<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NStdMapImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdMapImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdMapImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsStdSequenceContainer<ObjType >::s_Value 
        || IsStdSetContainer<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NStdContainerImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdContainerImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NStdContainerImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    // Custom data type for IGame lib.

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsIGameStreamBuffer<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NIGameBufferImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameBufferImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameBufferImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsIGameBlob<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NIGameBlobImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameBlobImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameBlobImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsIGameList<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NIGameListImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameListImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameListImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsIGameMap<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj) 
        { 
            return S11NIGameMapImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameMapImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameMapImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };

    template <class ObjType>
    struct S11NTraits<ObjType, typename EnableIf<IsIGameSet<ObjType >::s_Value>::type>
    {
        template <class DataType >
        static _UInt32 Size(DataType& data, const ObjType& obj)
        { 
            return S11NIGameSetImpls<DataType >::Size(obj, data);
        }

        template <class DataType >
        static _UInt32 Serialize(DataType& data, const ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameSetImpls<DataType >::Serialize((ObjType&)obj, data, name, mask);
        }

        template <class DataType >
        static bool Unserialize(DataType& data, ObjType& obj, const char* name, _UInt32 mask)
        {
            return S11NIGameSetImpls<DataType >::Unserialize(obj, data, name, mask);
        }
    };
    // Common Interface

    template <class ObjType, class DataType >
    static _UInt32 SerializeSize(const ObjType& obj, DataType& data)
    {
        return S11NTraits<ObjType >::Size(data, obj);
    }

    template <class ObjType, class DataType >
    static _UInt32 Serialize(const ObjType& obj, DataType& data, _UInt32 mask = 0)
    {
        return S11NTraits<ObjType >::Serialize(data, (ObjType&)obj, "Anonymity", mask);
    }
    
    template <class ObjType, class DataType >
    static bool Unserialize(ObjType& obj, DataType& data, _UInt32 mask = 0)
    {
        return S11NTraits<ObjType >::Unserialize(data, obj, "Anonymity", mask);
    }
} // namespace IGame

#define IGAME_S11N_0(objtype)                                                                           \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return 0;                                                                                           \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    return 0;                                                                                           \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    return true;                                                                                        \
}                                                     

#define IGAME_S11N_TRAITS_0(objtype)                                                                    \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return 0;                                                                                   \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            return 0;                                                                                   \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            return true;                                                                                \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_1(objtype, m1type, m1name)                                                           \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask);     \
}

#define IGAME_S11N_TRAITS_1(objtype, m1type, m1name)                                                    \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize(data, obj.m1name, #m1name, mask);                   \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_2(objtype, m1type, m1name, m2type, m2name)                                           \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask);     \
}

#define IGAME_S11N_TRAITS_2(objtype, m1type, m1name, m2type, m2name)                                    \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_3(objtype, m1type, m1name, m2type, m2name, m3type, m3name)                           \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask);     \
}

#define IGAME_S11N_TRAITS_3(objtype, m1type, m1name, m2type, m2name, m3type, m3name)                    \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_4(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name)           \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask);     \
}

#define IGAME_S11N_TRAITS_4(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name)    \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_5(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,           \
m5type, m5name)                                                                                         \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask);     \
}

#define IGAME_S11N_TRAITS_5(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,    \
m5type, m5name)                                                                                         \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_6(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,           \
m5type, m5name, m6type, m6name)                                                                         \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask);     \
}

#define IGAME_S11N_TRAITS_6(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,    \
m5type, m5name, m6type, m6name)                                                                         \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_7(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,           \
m5type, m5name, m6type, m6name, m7type, m7name)                                                         \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask);     \
}

#define IGAME_S11N_TRAITS_7(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,    \
m5type, m5name, m6type, m6name, m7type, m7name)                                                         \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_8(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,           \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name)                                         \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask);     \
}

#define IGAME_S11N_TRAITS_8(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,    \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name)                                         \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_9(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,           \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name)                         \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name);                               \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask);        \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask);     \
}

#define IGAME_S11N_TRAITS_9(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,    \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name)                         \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name);                              \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask);       \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask);    \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_10(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name)       \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask);  \
}

#define IGAME_S11N_TRAITS_10(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name)                                                                                                \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_11(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name)                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask);  \
}

#define IGAME_S11N_TRAITS_11(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name)                                                                              \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_12(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name)                                                                     \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask);  \
}

#define IGAME_S11N_TRAITS_12(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name)                                                            \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_13(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name)                                                   \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask);  \
}

#define IGAME_S11N_TRAITS_13(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name)                                          \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_14(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name)                                 \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask);  \
}

#define IGAME_S11N_TRAITS_14(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name)                        \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_15(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name)               \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask);  \
}

#define IGAME_S11N_TRAITS_15(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name)      \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_16(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name)                                                                                                \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask);  \
}

#define IGAME_S11N_TRAITS_16(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name)                                                                                       \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_17(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name)                                                                              \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask);  \
}

#define IGAME_S11N_TRAITS_17(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name)                                                                     \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_18(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name)                                                            \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask);  \
}

#define IGAME_S11N_TRAITS_18(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name)                                                   \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_19(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name)                                          \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask);  \
}

#define IGAME_S11N_TRAITS_19(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name)                                 \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_20(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name)                        \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask);  \
}

#define IGAME_S11N_TRAITS_20(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name)               \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_21(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name)      \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask);  \
}

#define IGAME_S11N_TRAITS_21(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name)                                                                                                \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_22(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name)                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask);  \
}

#define IGAME_S11N_TRAITS_22(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name)                                                                              \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_23(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name)                                                                     \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask);  \
}

#define IGAME_S11N_TRAITS_23(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name)                                                            \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_24(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name)                                                   \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask);  \
}

#define IGAME_S11N_TRAITS_24(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name)                                          \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_25(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name)                                 \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask);  \
}

#define IGAME_S11N_TRAITS_25(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name)                        \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_26(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name)               \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                             \
           IGame::S11NTraits<m26type >::Size<DataType >(data, obj.m26name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+     \
           IGame::S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)   \
        && IGame::S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask);  \
}

#define IGAME_S11N_TRAITS_26(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name)      \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                            \
                   S11NTraits<m26type >::Size<DataType >(data, obj.m26name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+    \
                   S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)  \
                && S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_27(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name, m27type,      \
m27name)                                                                                                \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                             \
           IGame::S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                             \
           IGame::S11NTraits<m27type >::Size<DataType >(data, obj.m27name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+     \
           IGame::S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+     \
           IGame::S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)   \
        && IGame::S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)   \
        && IGame::S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask);  \
}

#define IGAME_S11N_TRAITS_27(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name,      \
m27type, m27name)                                                                                       \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                            \
                   S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                            \
                   S11NTraits<m27type >::Size<DataType >(data, obj.m27name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+    \
                   S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+    \
                   S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)  \
                && S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)  \
                && S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_28(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name, m27type,      \
m27name, m28type, m28name)                                                                              \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                             \
           IGame::S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                             \
           IGame::S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                             \
           IGame::S11NTraits<m28type >::Size<DataType >(data, obj.m28name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+     \
           IGame::S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+     \
           IGame::S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+     \
           IGame::S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)   \
        && IGame::S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)   \
        && IGame::S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)   \
        && IGame::S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask);  \
}

#define IGAME_S11N_TRAITS_28(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name,      \
m27type, m27name, m28type, m28name)                                                                     \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                            \
                   S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                            \
                   S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                            \
                   S11NTraits<m28type >::Size<DataType >(data, obj.m28name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+    \
                   S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+    \
                   S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+    \
                   S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)  \
                && S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)  \
                && S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)  \
                && S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_29(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name, m27type,      \
m27name, m28type, m28name, m29type, m29name)                                                            \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                             \
           IGame::S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                             \
           IGame::S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                             \
           IGame::S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                             \
           IGame::S11NTraits<m29type >::Size<DataType >(data, obj.m29name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+     \
           IGame::S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+     \
           IGame::S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+     \
           IGame::S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+     \
           IGame::S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)   \
        && IGame::S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)   \
        && IGame::S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)   \
        && IGame::S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)   \
        && IGame::S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask);  \
}

#define IGAME_S11N_TRAITS_29(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name,      \
m27type, m27name, m28type, m28name, m29type, m29name)                                                   \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                            \
                   S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                            \
                   S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                            \
                   S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                            \
                   S11NTraits<m29type >::Size<DataType >(data, obj.m29name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+    \
                   S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+    \
                   S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+    \
                   S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+    \
                   S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)  \
                && S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)  \
                && S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)  \
                && S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)  \
                && S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_30(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name, m27type,      \
m27name, m28type, m28name, m29type, m29name, m30type, m30name)                                          \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                             \
           IGame::S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                             \
           IGame::S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                             \
           IGame::S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                             \
           IGame::S11NTraits<m29type >::Size<DataType >(data, obj.m29name)+                             \
           IGame::S11NTraits<m30type >::Size<DataType >(data, obj.m30name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+     \
           IGame::S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+     \
           IGame::S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+     \
           IGame::S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+     \
           IGame::S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask)+     \
           IGame::S11NTraits<m30type >::Serialize<DataType >(subdata, obj.m30name, #m30name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)   \
        && IGame::S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)   \
        && IGame::S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)   \
        && IGame::S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)   \
        && IGame::S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask)   \
        && IGame::S11NTraits<m30type >::Unserialize<DataType >(*subdata, obj.m30name, #m30name, mask);  \
}

#define IGAME_S11N_TRAITS_30(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name,      \
m27type, m27name, m28type, m28name, m29type, m29name, m30type, m30name)                                 \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                            \
                   S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                            \
                   S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                            \
                   S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                            \
                   S11NTraits<m29type >::Size<DataType >(data, obj.m29name)+                            \
                   S11NTraits<m30type >::Size<DataType >(data, obj.m30name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+    \
                   S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+    \
                   S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+    \
                   S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+    \
                   S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask)+    \
                   S11NTraits<m30type >::Serialize<DataType >(subdata, obj.m30name, #m30name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)  \
                && S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)  \
                && S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)  \
                && S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)  \
                && S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask)  \
                && S11NTraits<m30type >::Unserialize<DataType >(*subdata, obj.m30name, #m30name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_31(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name, m27type,      \
m27name, m28type, m28name, m29type, m29name, m30type, m30name, m31type, m31name)                        \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                             \
           IGame::S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                             \
           IGame::S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                             \
           IGame::S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                             \
           IGame::S11NTraits<m29type >::Size<DataType >(data, obj.m29name)+                             \
           IGame::S11NTraits<m30type >::Size<DataType >(data, obj.m30name)+                             \
           IGame::S11NTraits<m31type >::Size<DataType >(data, obj.m31name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+     \
           IGame::S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+     \
           IGame::S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+     \
           IGame::S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+     \
           IGame::S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask)+     \
           IGame::S11NTraits<m30type >::Serialize<DataType >(subdata, obj.m30name, #m30name, mask)+     \
           IGame::S11NTraits<m31type >::Serialize<DataType >(subdata, obj.m31name, #m31name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)   \
        && IGame::S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)   \
        && IGame::S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)   \
        && IGame::S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)   \
        && IGame::S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask)   \
        && IGame::S11NTraits<m30type >::Unserialize<DataType >(*subdata, obj.m30name, #m30name, mask)   \
        && IGame::S11NTraits<m31type >::Unserialize<DataType >(*subdata, obj.m31name, #m31name, mask);  \
}

#define IGAME_S11N_TRAITS_31(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name,      \
m27type, m27name, m28type, m28name, m29type, m29name, m30type, m30name, m31type, m31name)               \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                            \
                   S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                            \
                   S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                            \
                   S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                            \
                   S11NTraits<m29type >::Size<DataType >(data, obj.m29name)+                            \
                   S11NTraits<m30type >::Size<DataType >(data, obj.m30name)+                            \
                   S11NTraits<m31type >::Size<DataType >(data, obj.m31name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+    \
                   S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+    \
                   S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+    \
                   S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+    \
                   S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask)+    \
                   S11NTraits<m30type >::Serialize<DataType >(subdata, obj.m30name, #m30name, mask)+    \
                   S11NTraits<m31type >::Serialize<DataType >(subdata, obj.m31name, #m31name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)  \
                && S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)  \
                && S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)  \
                && S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)  \
                && S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask)  \
                && S11NTraits<m30type >::Unserialize<DataType >(*subdata, obj.m30name, #m30name, mask)  \
                && S11NTraits<m31type >::Unserialize<DataType >(*subdata, obj.m31name, #m31name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#define IGAME_S11N_32(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type, m4name,          \
m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type, m10name,       \
m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name, m16type,      \
m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type, m21name,      \
m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name, m27type,      \
m27name, m28type, m28name, m29type, m29name, m30type, m30name, m31type, m31name, m32type, m32name)      \
template <class DataType >                                                                              \
_UInt32 IGameSerializeSize(DataType& data, const objtype& obj) const                                    \
{                                                                                                       \
    return IGame::S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                               \
           IGame::S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                               \
           IGame::S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                               \
           IGame::S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                               \
           IGame::S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                               \
           IGame::S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                               \
           IGame::S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                               \
           IGame::S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                               \
           IGame::S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                               \
           IGame::S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                             \
           IGame::S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                             \
           IGame::S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                             \
           IGame::S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                             \
           IGame::S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                             \
           IGame::S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                             \
           IGame::S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                             \
           IGame::S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                             \
           IGame::S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                             \
           IGame::S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                             \
           IGame::S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                             \
           IGame::S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                             \
           IGame::S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                             \
           IGame::S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                             \
           IGame::S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                             \
           IGame::S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                             \
           IGame::S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                             \
           IGame::S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                             \
           IGame::S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                             \
           IGame::S11NTraits<m29type >::Size<DataType >(data, obj.m29name)+                             \
           IGame::S11NTraits<m30type >::Size<DataType >(data, obj.m30name)+                             \
           IGame::S11NTraits<m31type >::Size<DataType >(data, obj.m31name)+                             \
           IGame::S11NTraits<m32type >::Size<DataType >(data, obj.m32name);                             \
}                                                                                                       \
template <class DataType >                                                                              \
_UInt32 IGameSerialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                    \
{                                                                                                       \
    DataType& subdata = IGame::ObjectInit<DataType >::Init(data, name);                                 \
    return IGame::S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+        \
           IGame::S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+        \
           IGame::S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+        \
           IGame::S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+        \
           IGame::S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+        \
           IGame::S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+        \
           IGame::S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+        \
           IGame::S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+        \
           IGame::S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+        \
           IGame::S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+     \
           IGame::S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+     \
           IGame::S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+     \
           IGame::S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+     \
           IGame::S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+     \
           IGame::S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+     \
           IGame::S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+     \
           IGame::S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+     \
           IGame::S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+     \
           IGame::S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+     \
           IGame::S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+     \
           IGame::S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+     \
           IGame::S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+     \
           IGame::S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+     \
           IGame::S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+     \
           IGame::S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+     \
           IGame::S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+     \
           IGame::S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+     \
           IGame::S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+     \
           IGame::S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask)+     \
           IGame::S11NTraits<m30type >::Serialize<DataType >(subdata, obj.m30name, #m30name, mask)+     \
           IGame::S11NTraits<m31type >::Serialize<DataType >(subdata, obj.m31name, #m31name, mask)+     \
           IGame::S11NTraits<m32type >::Serialize<DataType >(subdata, obj.m32name, #m32name, mask);     \
}                                                                                                       \
template <class DataType >                                                                              \
bool IGameUnserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)                     \
{                                                                                                       \
    DataType* subdata = IGame::ObjectUninit<DataType >::Uninit(data, name);                             \
    if (subdata == NULL) return false;                                                                  \
    return IGame::S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)      \
        && IGame::S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)      \
        && IGame::S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)      \
        && IGame::S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)      \
        && IGame::S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)      \
        && IGame::S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)      \
        && IGame::S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)      \
        && IGame::S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)      \
        && IGame::S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)      \
        && IGame::S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)   \
        && IGame::S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)   \
        && IGame::S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)   \
        && IGame::S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)   \
        && IGame::S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)   \
        && IGame::S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)   \
        && IGame::S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)   \
        && IGame::S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)   \
        && IGame::S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)   \
        && IGame::S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)   \
        && IGame::S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)   \
        && IGame::S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)   \
        && IGame::S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)   \
        && IGame::S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)   \
        && IGame::S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)   \
        && IGame::S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)   \
        && IGame::S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)   \
        && IGame::S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)   \
        && IGame::S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)   \
        && IGame::S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask)   \
        && IGame::S11NTraits<m30type >::Unserialize<DataType >(*subdata, obj.m30name, #m30name, mask)   \
        && IGame::S11NTraits<m31type >::Unserialize<DataType >(*subdata, obj.m31name, #m31name, mask)   \
        && IGame::S11NTraits<m32type >::Unserialize<DataType >(*subdata, obj.m32name, #m32name, mask);  \
}

#define IGAME_S11N_TRAITS_32(objtype, m1type, m1name, m2type, m2name, m3type, m3name, m4type,           \
m4name, m5type, m5name, m6type, m6name, m7type, m7name, m8type, m8name, m9type, m9name, m10type,        \
m10name, m11type, m11name, m12type, m12name, m13type, m13name, m14type, m14name, m15type, m15name,      \
m16type, m16name, m17type, m17name, m18type, m18name, m19type, m19name, m20type, m20name, m21type,      \
m21name, m22type, m22name, m23type, m23name, m24type, m24name, m25type, m25name, m26type, m26name,      \
m27type, m27name, m28type, m28name, m29type, m29name, m30type, m30name, m31type, m31name, m32type,      \
m32name)                                                                                                \
namespace IGame                                                                                         \
{                                                                                                       \
    template <>                                                                                         \
    struct S11NTraits<objtype >                                                                         \
    {                                                                                                   \
        template <class DataType >                                                                      \
        static _UInt32 Size(DataType& data, const objtype& obj)                                         \
        {                                                                                               \
            return S11NTraits<m1type >::Size<DataType >(data, obj.m1name)+                              \
                   S11NTraits<m2type >::Size<DataType >(data, obj.m2name)+                              \
                   S11NTraits<m3type >::Size<DataType >(data, obj.m3name)+                              \
                   S11NTraits<m4type >::Size<DataType >(data, obj.m4name)+                              \
                   S11NTraits<m5type >::Size<DataType >(data, obj.m5name)+                              \
                   S11NTraits<m6type >::Size<DataType >(data, obj.m6name)+                              \
                   S11NTraits<m7type >::Size<DataType >(data, obj.m7name)+                              \
                   S11NTraits<m8type >::Size<DataType >(data, obj.m8name)+                              \
                   S11NTraits<m9type >::Size<DataType >(data, obj.m9name)+                              \
                   S11NTraits<m10type >::Size<DataType >(data, obj.m10name)+                            \
                   S11NTraits<m11type >::Size<DataType >(data, obj.m11name)+                            \
                   S11NTraits<m12type >::Size<DataType >(data, obj.m12name)+                            \
                   S11NTraits<m13type >::Size<DataType >(data, obj.m13name)+                            \
                   S11NTraits<m14type >::Size<DataType >(data, obj.m14name)+                            \
                   S11NTraits<m15type >::Size<DataType >(data, obj.m15name)+                            \
                   S11NTraits<m16type >::Size<DataType >(data, obj.m16name)+                            \
                   S11NTraits<m17type >::Size<DataType >(data, obj.m17name)+                            \
                   S11NTraits<m18type >::Size<DataType >(data, obj.m18name)+                            \
                   S11NTraits<m19type >::Size<DataType >(data, obj.m19name)+                            \
                   S11NTraits<m20type >::Size<DataType >(data, obj.m20name)+                            \
                   S11NTraits<m21type >::Size<DataType >(data, obj.m21name)+                            \
                   S11NTraits<m22type >::Size<DataType >(data, obj.m22name)+                            \
                   S11NTraits<m23type >::Size<DataType >(data, obj.m23name)+                            \
                   S11NTraits<m24type >::Size<DataType >(data, obj.m24name)+                            \
                   S11NTraits<m25type >::Size<DataType >(data, obj.m25name)+                            \
                   S11NTraits<m26type >::Size<DataType >(data, obj.m26name)+                            \
                   S11NTraits<m27type >::Size<DataType >(data, obj.m27name)+                            \
                   S11NTraits<m28type >::Size<DataType >(data, obj.m28name)+                            \
                   S11NTraits<m29type >::Size<DataType >(data, obj.m29name)+                            \
                   S11NTraits<m30type >::Size<DataType >(data, obj.m30name)+                            \
                   S11NTraits<m31type >::Size<DataType >(data, obj.m31name)+                            \
                   S11NTraits<m32type >::Size<DataType >(data, obj.m32name);                            \
        }                                                                                               \
        template <class DataType >                                                                      \
        static _UInt32 Serialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)          \
        {                                                                                               \
            DataType& subdata = ObjectInit<DataType >::Init(data, name);                                \
            return S11NTraits<m1type >::Serialize<DataType >(subdata, obj.m1name, #m1name, mask)+       \
                   S11NTraits<m2type >::Serialize<DataType >(subdata, obj.m2name, #m2name, mask)+       \
                   S11NTraits<m3type >::Serialize<DataType >(subdata, obj.m3name, #m3name, mask)+       \
                   S11NTraits<m4type >::Serialize<DataType >(subdata, obj.m4name, #m4name, mask)+       \
                   S11NTraits<m5type >::Serialize<DataType >(subdata, obj.m5name, #m5name, mask)+       \
                   S11NTraits<m6type >::Serialize<DataType >(subdata, obj.m6name, #m6name, mask)+       \
                   S11NTraits<m7type >::Serialize<DataType >(subdata, obj.m7name, #m7name, mask)+       \
                   S11NTraits<m8type >::Serialize<DataType >(subdata, obj.m8name, #m8name, mask)+       \
                   S11NTraits<m9type >::Serialize<DataType >(subdata, obj.m9name, #m9name, mask)+       \
                   S11NTraits<m10type >::Serialize<DataType >(subdata, obj.m10name, #m10name, mask)+    \
                   S11NTraits<m11type >::Serialize<DataType >(subdata, obj.m11name, #m11name, mask)+    \
                   S11NTraits<m12type >::Serialize<DataType >(subdata, obj.m12name, #m12name, mask)+    \
                   S11NTraits<m13type >::Serialize<DataType >(subdata, obj.m13name, #m13name, mask)+    \
                   S11NTraits<m14type >::Serialize<DataType >(subdata, obj.m14name, #m14name, mask)+    \
                   S11NTraits<m15type >::Serialize<DataType >(subdata, obj.m15name, #m15name, mask)+    \
                   S11NTraits<m16type >::Serialize<DataType >(subdata, obj.m16name, #m16name, mask)+    \
                   S11NTraits<m17type >::Serialize<DataType >(subdata, obj.m17name, #m17name, mask)+    \
                   S11NTraits<m18type >::Serialize<DataType >(subdata, obj.m18name, #m18name, mask)+    \
                   S11NTraits<m19type >::Serialize<DataType >(subdata, obj.m19name, #m19name, mask)+    \
                   S11NTraits<m20type >::Serialize<DataType >(subdata, obj.m20name, #m20name, mask)+    \
                   S11NTraits<m21type >::Serialize<DataType >(subdata, obj.m21name, #m21name, mask)+    \
                   S11NTraits<m22type >::Serialize<DataType >(subdata, obj.m22name, #m22name, mask)+    \
                   S11NTraits<m23type >::Serialize<DataType >(subdata, obj.m23name, #m23name, mask)+    \
                   S11NTraits<m24type >::Serialize<DataType >(subdata, obj.m24name, #m24name, mask)+    \
                   S11NTraits<m25type >::Serialize<DataType >(subdata, obj.m25name, #m25name, mask)+    \
                   S11NTraits<m26type >::Serialize<DataType >(subdata, obj.m26name, #m26name, mask)+    \
                   S11NTraits<m27type >::Serialize<DataType >(subdata, obj.m27name, #m27name, mask)+    \
                   S11NTraits<m28type >::Serialize<DataType >(subdata, obj.m28name, #m28name, mask)+    \
                   S11NTraits<m29type >::Serialize<DataType >(subdata, obj.m29name, #m29name, mask)+    \
                   S11NTraits<m30type >::Serialize<DataType >(subdata, obj.m30name, #m30name, mask)+    \
                   S11NTraits<m31type >::Serialize<DataType >(subdata, obj.m31name, #m31name, mask)+    \
                   S11NTraits<m32type >::Serialize<DataType >(subdata, obj.m32name, #m32name, mask);    \
        }                                                                                               \
        template <class DataType >                                                                      \
        static bool Unserialize(DataType& data, objtype& obj, const char* name, _UInt32 mask)           \
        {                                                                                               \
            DataType* subdata = ObjectUninit<DataType >::Uninit(data, name);                            \
            if (subdata == NULL) return false;                                                          \
            return S11NTraits<m1type >::Unserialize<DataType >(*subdata, obj.m1name, #m1name, mask)     \
                && S11NTraits<m2type >::Unserialize<DataType >(*subdata, obj.m2name, #m2name, mask)     \
                && S11NTraits<m3type >::Unserialize<DataType >(*subdata, obj.m3name, #m3name, mask)     \
                && S11NTraits<m4type >::Unserialize<DataType >(*subdata, obj.m4name, #m4name, mask)     \
                && S11NTraits<m5type >::Unserialize<DataType >(*subdata, obj.m5name, #m5name, mask)     \
                && S11NTraits<m6type >::Unserialize<DataType >(*subdata, obj.m6name, #m6name, mask)     \
                && S11NTraits<m7type >::Unserialize<DataType >(*subdata, obj.m7name, #m7name, mask)     \
                && S11NTraits<m8type >::Unserialize<DataType >(*subdata, obj.m8name, #m8name, mask)     \
                && S11NTraits<m9type >::Unserialize<DataType >(*subdata, obj.m9name, #m9name, mask)     \
                && S11NTraits<m10type >::Unserialize<DataType >(*subdata, obj.m10name, #m10name, mask)  \
                && S11NTraits<m11type >::Unserialize<DataType >(*subdata, obj.m11name, #m11name, mask)  \
                && S11NTraits<m12type >::Unserialize<DataType >(*subdata, obj.m12name, #m12name, mask)  \
                && S11NTraits<m13type >::Unserialize<DataType >(*subdata, obj.m13name, #m13name, mask)  \
                && S11NTraits<m14type >::Unserialize<DataType >(*subdata, obj.m14name, #m14name, mask)  \
                && S11NTraits<m15type >::Unserialize<DataType >(*subdata, obj.m15name, #m15name, mask)  \
                && S11NTraits<m16type >::Unserialize<DataType >(*subdata, obj.m16name, #m16name, mask)  \
                && S11NTraits<m17type >::Unserialize<DataType >(*subdata, obj.m17name, #m17name, mask)  \
                && S11NTraits<m18type >::Unserialize<DataType >(*subdata, obj.m18name, #m18name, mask)  \
                && S11NTraits<m19type >::Unserialize<DataType >(*subdata, obj.m19name, #m19name, mask)  \
                && S11NTraits<m20type >::Unserialize<DataType >(*subdata, obj.m20name, #m20name, mask)  \
                && S11NTraits<m21type >::Unserialize<DataType >(*subdata, obj.m21name, #m21name, mask)  \
                && S11NTraits<m22type >::Unserialize<DataType >(*subdata, obj.m22name, #m22name, mask)  \
                && S11NTraits<m23type >::Unserialize<DataType >(*subdata, obj.m23name, #m23name, mask)  \
                && S11NTraits<m24type >::Unserialize<DataType >(*subdata, obj.m24name, #m24name, mask)  \
                && S11NTraits<m25type >::Unserialize<DataType >(*subdata, obj.m25name, #m25name, mask)  \
                && S11NTraits<m26type >::Unserialize<DataType >(*subdata, obj.m26name, #m26name, mask)  \
                && S11NTraits<m27type >::Unserialize<DataType >(*subdata, obj.m27name, #m27name, mask)  \
                && S11NTraits<m28type >::Unserialize<DataType >(*subdata, obj.m28name, #m28name, mask)  \
                && S11NTraits<m29type >::Unserialize<DataType >(*subdata, obj.m29name, #m29name, mask)  \
                && S11NTraits<m30type >::Unserialize<DataType >(*subdata, obj.m30name, #m30name, mask)  \
                && S11NTraits<m31type >::Unserialize<DataType >(*subdata, obj.m31name, #m31name, mask)  \
                && S11NTraits<m32type >::Unserialize<DataType >(*subdata, obj.m32name, #m32name, mask); \
        }                                                                                               \
    };                                                                                                  \
}

#endif // #ifndef IGAME_OS_SERIALIZE_TRAITS_H



