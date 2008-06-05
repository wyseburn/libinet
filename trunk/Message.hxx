/**
 *  Version:     @(#)Message.hxx    0.0.1 08/06/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     
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

#ifndef __INET_MESSAGE_H__
#define __INET_MESSAGE_H__

#include "Serialize.hxx"

namespace INet
{
    struct MsgHeader
    {
        Int32  mId;
        UInt32 mBodyLen;
        MsgHeader() : mId(0), mBodyLen(0) {}
        INET_S11N_2(MsgHeader, Int32, mId, UInt32, mBodyLen);
    };

    template <typename Type = void>
    struct MsgId
    {
        static const Int32 sMsgId = 0;
    };
} // namespace

#define INET_MESSAGE_1(MsgID, ClassName, Member1Type, Member1Name)                           \
INET_S11N_TRAITS_1(ClassName, Member1Type, Member1Name)                                      \
namespace INet { template <> struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_2(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name) \
INET_S11N_TRAITS_2(ClassName, Member1Type, Member1Name, Member2Type, Member2Name)            \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_3(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name)                                                                    \
INET_S11N_TRAITS_3(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,            \
Member3Type, Member3Name)                                                                    \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_4(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name)                                          \
INET_S11N_TRAITS_4(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,            \
Member3Type, Member3Name, Member4Type, Member4Name)                                          \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_5(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name)                \
INET_S11N_TRAITS_5(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,            \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name)                \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_6(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name)                                                                                 \
INET_S11N_TRAITS_6(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,            \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name)                                                                                 \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}


#define INET_MESSAGE_7(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name)                                                       \
INET_S11N_TRAITS_7(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,            \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name)                                                       \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}


#define INET_MESSAGE_8(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name)                             \
INET_S11N_TRAITS_8(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,            \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name)                             \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_9(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name, \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name)   \
INET_S11N_TRAITS_9(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,            \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name)   \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_10(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name,\
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name)                                                                  \
INET_S11N_TRAITS_10(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,           \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name)                                                                  \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_11(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name,\
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name)                                      \
INET_S11N_TRAITS_11(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,           \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name)                                      \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_12(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name,\
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name)          \
INET_S11N_TRAITS_12(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,           \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name)          \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_13(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name,\
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,          \
Member13Type, Member13Name)                                                                  \
INET_S11N_TRAITS_13(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,           \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,          \
Member13Type, Member13Name)                                                                  \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_14(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name,\
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,          \
Member13Type, Member13Name, Member14Type, Member14Name)                                      \
INET_S11N_TRAITS_14(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,           \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,          \
Member13Type, Member13Name, Member14Type, Member14Name)                                      \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#define INET_MESSAGE_15(MsgID, ClassName, Member1Type, Member1Name, Member2Type, Member2Name,\
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,          \
Member13Type, Member13Name, Member14Type, Member14Name, Member15Type, Member15Name)          \
INET_S11N_TRAITS_15(ClassName, Member1Type, Member1Name, Member2Type, Member2Name,           \
Member3Type, Member3Name, Member4Type, Member4Name, Member5Type, Member5Name, Member6Type,   \
Member6Name, Member7Type, Member7Name, Member8Type, Member8Name, Member9Type, Member9Name,   \
Member10Type, Member10Name, Member11Type, Member11Name, Member12Type, Member12Name,          \
Member13Type, Member13Name, Member14Type, Member14Name, Member15Type, Member15Name)          \
namespace INet {template<>  struct MsgId<ClassName > {static const Int32 sMsgID = MsgID;};}

#endif // #ifndef __INET_MESSAGE_H__

