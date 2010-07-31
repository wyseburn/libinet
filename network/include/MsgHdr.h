/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_MSG_HDR_H
#define IGAME_NETWORK_MSG_HDR_H

#include "os/include/StreamBufferSerializeImpls.h"

#define IGAME_MAX_MSG_ID               10000  // 0 and 0xFFFF was reserved.
#define IGAME_MAX_MSG_LENGTH           1024 * 200

namespace IGame
{
    struct MsgHdr
    {
        MsgHdr() : m_Id(0), m_BodyLen(0) {}
        virtual ~MsgHdr() {}

        _UInt16 m_Id;
        _UInt32 m_BodyLen;

        IGAME_S11N_2(MsgHdr, _UInt16, m_Id, _UInt32, m_BodyLen);
    };

    template <typename Type = void>
    struct MsgId 
    {
        static const _UInt16 s_MsgId = Type::s_MsgId;
    };
} // namespace

#define IGAME_MESSAGE_0(msgid, classname) IGAME_S11N_0(classname)                                     \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_0(msgid, classname) IGAME_S11N_TRAITS_0(classname)                       \
namespace IGame {template<> struct MsgId<classname > { static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_1(msgid, classname, member1type, member1name)                                   \
IGAME_S11N_1(classname, member1type, member1name)                                                     \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_1(msgid, classname, member1type, member1name)                            \
IGAME_S11N_TRAITS_1(classname, member1type, member1name)                                              \
namespace IGame { template <> struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_2(msgid, classname, member1type, member1name, member2type, member2name)         \
IGAME_S11N_2(classname, member1type, member1name, member2type, member2name)                           \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_2(msgid, classname, member1type, member1name, member2type, member2name)  \
IGAME_S11N_TRAITS_2(classname, member1type, member1name, member2type, member2name)                    \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_3(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name)                                                                             \
IGAME_S11N_3(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name)                                                                             \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_3(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name)                                                                             \
IGAME_S11N_TRAITS_3(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name)                                                                             \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_4(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name)                                                   \
IGAME_S11N_4(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name)                                                   \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_4(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name)                                                   \
IGAME_S11N_TRAITS_4(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name)                                                   \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_5(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
IGAME_S11N_5(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_5(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
IGAME_S11N_TRAITS_5(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name)                         \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_6(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
IGAME_S11N_6(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_6(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
IGAME_S11N_TRAITS_6(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name)                                                                                          \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_7(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
IGAME_S11N_7(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_7(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
IGAME_S11N_TRAITS_7(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name)                                                                \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_8(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
IGAME_S11N_8(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_8(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
IGAME_S11N_TRAITS_8(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name)                                      \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_9(msgid, classname, member1type, member1name, member2type, member2name,         \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
IGAME_S11N_9(classname, member1type, member1name, member2type, member2name,                           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_9(msgid, classname, member1type, member1name, member2type, member2name,  \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
IGAME_S11N_TRAITS_9(classname, member1type, member1name, member2type, member2name,                    \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)            \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_10(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
IGAME_S11N_10(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_10(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
IGAME_S11N_TRAITS_10(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name)                                                                           \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_11(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
IGAME_S11N_11(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_11(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
IGAME_S11N_TRAITS_11(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name)                                               \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_12(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
IGAME_S11N_12(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_12(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
IGAME_S11N_TRAITS_12(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name)                   \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_13(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
IGAME_S11N_13(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_13(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
IGAME_S11N_TRAITS_13(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name)                                                                           \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_14(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
IGAME_S11N_14(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_14(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
IGAME_S11N_TRAITS_14(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name)                                               \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_15(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
IGAME_S11N_15(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_15(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
IGAME_S11N_TRAITS_15(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name)                   \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_16(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
IGAME_S11N_16(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_16(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
IGAME_S11N_TRAITS_16(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name)                                                                                         \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_17(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
IGAME_S11N_17(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_17(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
IGAME_S11N_TRAITS_17(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name)                                                             \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_18(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
IGAME_S11N_18(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_18(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
IGAME_S11N_TRAITS_18(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name)                                 \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_19(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
IGAME_S11N_19(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_19(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
IGAME_S11N_TRAITS_19(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name)     \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_20(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
IGAME_S11N_20(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_20(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
IGAME_S11N_TRAITS_20(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name)                                                                           \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_21(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
IGAME_S11N_21(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_21(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
IGAME_S11N_TRAITS_21(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name)                                               \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_22(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
IGAME_S11N_22(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_22(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
IGAME_S11N_TRAITS_22(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name)                   \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_23(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
IGAME_S11N_23(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_23(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
IGAME_S11N_TRAITS_23(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name)                                                                                         \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_24(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
IGAME_S11N_24(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_24(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
IGAME_S11N_TRAITS_24(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name)                                                             \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#define IGAME_MESSAGE_25(msgid, classname, member1type, member1name, member2type, member2name,        \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
IGAME_S11N_25(classname, member1type, member1name, member2type, member2name,                          \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
static const _UInt16 s_MsgId = msgid

#define IGAME_MESSAGE_TRAITS_25(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
IGAME_S11N_TRAITS_25(classname, member1type, member1name, member2type, member2name,                   \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,            \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,            \
member10type, member10name, member11type, member11name, member12type, member12name,                   \
member13type, member13name, member14type, member14name, member15type, member15name, member16type,     \
member16name, member17type, member17name, member18type, member18name, member19type, member19name,     \
member20type, member20name, member21type, member21name, member22type, member22name, member23type,     \
member23name, member24type, member24name, member25type, member25name)                                 \
namespace IGame {template<>  struct MsgId<classname > {static const _UInt16 s_MsgId = msgid;};}

#endif // #ifndef IGAME_NETWORK_MSG_HEADER_H
