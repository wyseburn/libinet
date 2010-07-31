/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_OBJECT_SYNC_H
#define IGAME_OS_OBJECT_SYNC_H

#include "StreamBuffer.h"

namespace IGame
{
    template <class ObjType, class Enable = void>
    struct ObjectSyncTraits
    {
        static bool GetObjectSync(ObjType& obj, StreamBuffer& data)
        {
            return obj.IGameObjectSyncFunc(data);
        }
    };
    
    template <class ObjType >
    static bool ObjectSync(ObjType& obj, StreamBuffer& data)
    {
        return ObjectSyncTraits<ObjType >::GetObjectSync(obj, data);
    }

} // namespace IGame

#define SET_OBJECT_SYNC(obj, member, data)                                                \
do {                                                                                      \
    _UInt32 offset = ((char*)&obj.member - (char*)&obj);                                  \
    assert(offset < 256);                                                                 \
    data << (_UInt8)offset;                                                               \
    IGame::Serialize(obj.member, data);                                                   \
} while (0)  

#define IGAME_OBJECT_SYNC_1(m1name)                                                       \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_2(m1name, m2name)                                               \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_3(m1name, m2name, m3name)                                       \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_4(m1name, m2name, m3name, m4name)                               \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_5(m1name, m2name, m3name, m4name, m5name)                       \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_6(m1name, m2name, m3name, m4name, m5name, m6name)               \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_7(m1name, m2name, m3name, m4name, m5name, m6name, m7name)       \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_8(m1name, m2name, m3name, m4name, m5name, m6name, m7name,       \
    m8name)                                                                               \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_9(m1name, m2name, m3name, m4name, m5name, m6name, m7name,       \
    m8name, m9name)                                                                       \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_10(m1name, m2name, m3name, m4name, m5name, m6name, m7name,      \
    m8name, m9name, m10name)                                                              \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m10name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m10name, data)) return false;                         \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_11(m1name, m2name, m3name, m4name, m5name, m6name, m7name,      \
    m8name, m9name, m10name, m11name)                                                     \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m10name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m10name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m11name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m11name, data)) return false;                         \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_12(m1name, m2name, m3name, m4name, m5name, m6name, m7name,      \
    m8name, m9name, m10name, m11name, m12name)                                            \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m10name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m10name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m11name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m11name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m12name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m12name, data)) return false;                         \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_13(m1name, m2name, m3name, m4name, m5name, m6name, m7name,      \
    m8name, m9name, m10name, m11name, m12name, m13name)                                   \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m10name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m10name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m11name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m11name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m12name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m12name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m13name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m13name, data)) return false;                         \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_14(m1name, m2name, m3name, m4name, m5name, m6name, m7name,      \
    m8name, m9name, m10name, m11name, m12name, m13name, m14name)                          \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m10name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m10name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m11name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m11name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m12name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m12name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m13name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m13name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m14name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m14name, data)) return false;                         \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_15(m1name, m2name, m3name, m4name, m5name, m6name, m7name,      \
    m8name, m9name, m10name, m11name, m12name, m13name, m14name, m15name)                 \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m10name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m10name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m11name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m11name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m12name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m12name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m13name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m13name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m14name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m14name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m15name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m15name, data)) return false;                         \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#define IGAME_OBJECT_SYNC_16(m1name, m2name, m3name, m4name, m5name, m6name, m7name,      \
    m8name, m9name, m10name, m11name, m12name, m13name, m14name, m15name, m16name)        \
bool IGameObjectSyncFunc(StreamBuffer& data)                                              \
{                                                                                         \
    _UInt8 offset;                                                                        \
    while (data.Length() > 0)                                                             \
    {                                                                                     \
        data >> offset;                                                                   \
        if (((char*)&m1name - (char*)this) == offset)                                     \
        {                                                                                 \
            if (!IGame::Unserialize(m1name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m2name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m2name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m3name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m3name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m4name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m4name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m5name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m5name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m6name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m6name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m7name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m7name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m8name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m8name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m9name - (char*)this) == offset)                                \
        {                                                                                 \
            if (!IGame::Unserialize(m9name, data)) return false;                          \
        }                                                                                 \
        else if (((char*)&m10name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m10name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m11name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m11name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m12name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m12name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m13name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m13name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m14name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m14name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m15name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m15name, data)) return false;                         \
        }                                                                                 \
        else if (((char*)&m16name - (char*)this) == offset)                               \
        {                                                                                 \
            if (!IGame::Unserialize(m16name, data)) return false;                         \
        }                                                                                 \
        else return false;                                                                \
    }                                                                                     \
    return true;                                                                          \
}

#endif // #ifndef IGAME_OS_OBJECT_SYNC_H

