/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_SERIALIZE_IMPLS_H
#define IGAME_OS_SERIALIZE_IMPLS_H

#include "os/include/SerializeType.h"

namespace IGame
{
    template <class DataType, class Enable = void>
    struct S11NBooleanImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NIntegerImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NFloatImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NDoubleImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NStdBasicStringImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NStdPairImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NStdMapImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NStdContainerImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NIGameBufferImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NIGameBlobImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };


    template <class DataType, class Enable = void>
    struct S11NIGameListImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NIGameMapImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };


    template <class DataType, class Enable = void>
    struct S11NIGameSetImpls
    {
        template <class ObjType >
        static _UInt32 Size(const ObjType& obj, DataType& data)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static _UInt32 Serialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return 0;
        }

        template <class ObjType >
        static bool Unserialize(ObjType& obj, DataType& data, const char* name, _UInt32 mask)
        {
            assert(0);
            return true;
        }
    };
} // namespace IGame

#include "os/include/SerializeTraits.h"

#endif // #ifndef IGAME_OS_SERIALIZE_IMPLS_H


