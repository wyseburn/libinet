/*
 *  Version:     @(#)libinet/msg_handlers.hxx    0.0.2 24/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     Message handler manager class interface 
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

#ifndef __LIBINET_MSG_HANDLER_H__
#define __LIBINET_MSG_HANDLER_H__

#include "delegate.hxx"
#include "session.hxx"
#include "message.hxx"
#include "errno.hxx"

#define INET_REGISTER_DEFAULT_MSG(handler, obj, func) \
    (handler)->register_default_handler(DEFAULT_MSG_HANDLER(obj, func))

#define INET_REGISTER_MSG(handler, type, obj, func) \
    (handler)->register_handler<type>(new inet::delegate<bool (const type&)>(obj, func))

namespace inet 
{
    typedef delegate<bool (inet_uint16/*msg id*/, inet_uint32/*msg len*/, buffer&)> DEFAULT_MSG_HANDLER;
    template <inet_uint16 MaxMsgId = 64>
    class msg_handler
    {
    public:
        msg_handler() : pending_flag_(false)
        { 
            memset(handlers_, 0, sizeof(handlers_));
        }

        virtual ~msg_handler() 
        { 
            for (inet_uint16 i = 0; i < MaxMsgId; i++) 
            {
                delete (handlers_[i].func_);
            }
        }

        void init(inet::session* session)
        {
            assert(session);
            session_ = session;
            INET_REGISTER_RECEIVED(session, this, &msg_handler::on_received);
        }

        template <typename MsgType>
        void register_handler(delegate<bool (const MsgType&)>* handler)
        {
            inet_int32 id = msg_id<MsgType >::_msg_id;
            assert(id >= 0 && id < MaxMsgId);
            if (handlers_[id].func_) delete (handlers_[id].func_);
            handlers_[id].wrapper_ = &handler_wrapper<MsgType>; 
            handlers_[id].func_ = (void *)handler;
        }

        void register_default_handler(const DEFAULT_MSG_HANDLER& handler)
        {
            default_handler_ += handler;
        }

        template <class MsgType>
        void send_msg(const MsgType& msg)
        {
            assert(session_);
            msg_header header;
            header.id_ = msg_id<MsgType >::_msg_id;
            header.body_len_ = serialize_size(msg);
            serialize(header, session_->send_buffer_);
            if (header.body_len_ > 0)
                serialize(msg, session_->send_buffer_);  
            session_->async_send();
        }

    private:
        template <typename MsgType>
        static bool handler_wrapper(void* func, buffer& istream)
        {
            MsgType msg; 
            if (!unserialize(msg, istream)) return false;
            return ((delegate<bool (const MsgType&)>*)func)->operator () (msg);
        }

        void on_received(inet::session* session, buffer& istream, buffer& ostream)
        {
            while (1)
            {
                if (pending_flag_ == false)
                {
                    if (istream.length() < sizeof(struct msg_header))
                        break;

                    if (!unserialize(msghdr_, istream))
                    {
                        std::cout << "Failed to unserialized for message header." << std::endl;
                        session->errno_ = INET_ERROR_S11N;
                        session->close();
                    }

                    if (msghdr_.id_ < 0 || msghdr_.id_ > MaxMsgId)
                    {
                        std::cout << "Invalid message id." << std::endl;
                        session->errno_ = INET_ERROR_MSG_ID;
                        session->close();
                    }
                }

                if (msghdr_.body_len_ > istream.length())
                {
                    pending_flag_ = true;
                    break;
                }

                pending_flag_ = false;

                if (!handlers_[msghdr_.id_].wrapper_ || !handlers_[msghdr_.id_].func_)
                {
                    if (!default_handler_.empty())
                    {
                        (default_handler_)(msghdr_.id_, msghdr_.body_len_, istream); 
                    }
                    else
                    {
                        std::cout << "Don't match this message." << std::endl;
                        session->errno_ = INET_ERROR_MSG_HANDLER;
                        session->close();
                    }
                }
                else 
                {
                    if (!handlers_[msghdr_.id_].wrapper_(handlers_[msghdr_.id_].func_, istream))
                    {
                        session->errno_ = INET_ERROR_S11N;
                        session->close();
                    }
                }
            }
        }

        struct handler
        {
            bool (*wrapper_)(void*, buffer&);
            void* func_; 
        };

        DEFAULT_MSG_HANDLER default_handler_;
        handler handlers_[MaxMsgId];
        inet::session* session_;
        bool pending_flag_;
        inet::msg_header msghdr_;
    };
} // namespace

#endif // #ifndef __LIBINET_MSG_HANDLER_H__

