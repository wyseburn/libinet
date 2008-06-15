/**
 *  Version:     @(#)libinet/message.hxx    0.0.2 08/06/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     
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

#ifndef __LIBINET_MESSAGE_H__
#define __LIBINET_MESSAGE_H__

#include "serialize.hxx"

namespace inet
{
    struct msg_header
    {
        inet_uint16 id_;  // max valid id 65535
        inet_uint32 body_len_;
        msg_header() : id_(0), body_len_(0) {}
        INET_S11N_2(msg_header, inet_uint16, id_, inet_uint32, body_len_);
    };

    template <typename Type = void>
    struct msg_id
    {
        static const inet_uint16 _msg_id = 0;
    };
} // namespace

#define INET_MESSAGE_1(msgid, classname, member1type, member1name)                           \
INET_S11N_TRAITS_1(classname, member1type, member1name)                                      \
namespace inet { template <> struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_2(msgid, classname, member1type, member1name, member2type, member2name) \
INET_S11N_TRAITS_2(classname, member1type, member1name, member2type, member2name)            \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_3(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name)                                                                    \
INET_S11N_TRAITS_3(classname, member1type, member1name, member2type, member2name,            \
member3type, member3name)                                                                    \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_4(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name)                                          \
INET_S11N_TRAITS_4(classname, member1type, member1name, member2type, member2name,            \
member3type, member3name, member4type, member4name)                                          \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_5(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name)                \
INET_S11N_TRAITS_5(classname, member1type, member1name, member2type, member2name,            \
member3type, member3name, member4type, member4name, member5type, member5name)                \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_6(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name)                                                                                 \
INET_S11N_TRAITS_6(classname, member1type, member1name, member2type, member2name,            \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name)                                                                                 \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}


#define INET_MESSAGE_7(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name)                                                       \
INET_S11N_TRAITS_7(classname, member1type, member1name, member2type, member2name,            \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name)                                                       \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}


#define INET_MESSAGE_8(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name)                             \
INET_S11N_TRAITS_8(classname, member1type, member1name, member2type, member2name,            \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name)                             \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_9(msgid, classname, member1type, member1name, member2type, member2name, \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)   \
INET_S11N_TRAITS_9(classname, member1type, member1name, member2type, member2name,            \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name)   \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_10(msgid, classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name)                                                                  \
INET_S11N_TRAITS_10(classname, member1type, member1name, member2type, member2name,           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name)                                                                  \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_11(msgid, classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name)                                      \
INET_S11N_TRAITS_11(classname, member1type, member1name, member2type, member2name,           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name)                                      \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_12(msgid, classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name)          \
INET_S11N_TRAITS_12(classname, member1type, member1name, member2type, member2name,           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name)          \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_13(msgid, classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name,          \
member13type, member13name)                                                                  \
INET_S11N_TRAITS_13(classname, member1type, member1name, member2type, member2name,           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name,          \
member13type, member13name)                                                                  \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_14(msgid, classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name,          \
member13type, member13name, member14type, member14name)                                      \
INET_S11N_TRAITS_14(classname, member1type, member1name, member2type, member2name,           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name,          \
member13type, member13name, member14type, member14name)                                      \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#define INET_MESSAGE_15(msgid, classname, member1type, member1name, member2type, member2name,\
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name,          \
member13type, member13name, member14type, member14name, member15type, member15name)          \
INET_S11N_TRAITS_15(classname, member1type, member1name, member2type, member2name,           \
member3type, member3name, member4type, member4name, member5type, member5name, member6type,   \
member6name, member7type, member7name, member8type, member8name, member9type, member9name,   \
member10type, member10name, member11type, member11name, member12type, member12name,          \
member13type, member13name, member14type, member14name, member15type, member15name)          \
namespace inet {template<>  struct msg_id<classname > {static const inet_uint16 _msg_id = msgid;};}

#endif // #ifndef __LIBINET_MESSAGE_H__

