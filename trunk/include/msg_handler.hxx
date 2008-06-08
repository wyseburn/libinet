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

#define INET_MSG_REGISTER(handler, msgtype, obj, func) \
    (handler).register_handler<msgtype>(new inet::Delegate<bool (const msgtype&)>(obj, func))

namespace inet 
{
    template <inet_uint32 MaxMsgId = 64> // message handler array size
    class msg_handler
    {
    public:
        msg_handler() : pending_flag_(false)
        { 
            memset(handlers_, 0, sizeof(handlers_));
            memset(&default_handler_, 0, sizeof(default_handler_));
        }

        virtual ~msg_handler() 
        { 
            for (int i = 0; i < MaxMsgId; i++) delete handlers_[i].func_;
            if (default_handler_.func_)  delete default_handler_.func_;
        }

        void init(inet::session* session)
        {
            session_ = session;
            INET_ON_RECEIVED(session, this, &msg_handler::on_received);
        }

        template <typename MsgType>
        void register_handler(Delegate<bool (const MsgType&)>* handler)
        {
            inet_int32 id = msg_id<MsgType >::_msg_id;
            assert(id >= 0 && id < MaxMsgId);
            handlers_[id].wrapper_ = &handler_wrapper<MsgType>; 
            handlers_[id].func_ = (void *)handler;
        }

        void register_default_handler(const Delegate<bool (buffer&)>& handler)
        {
            default_handler_.func_ = (void *)&handler;
        }

        template <class MsgType>
        void send_msg(const MsgType& msg)
        {
            assert(session_);
            msg_header header;
            header.id_ = msg_id<MsgType >::_msg_id;
            header.body_len_ = serialize_size(msg);
            if (header.body_len_ > 0)
            {
                serialize(header, session_->send_buffer_);
                serialize(msg, session_->send_buffer_);  
                session_->async_send();
            }
        }

        void on_received(inet::session* session)
        {
            assert(session_);

            while (1)
            {
                if (pending_flag_ == false)
                {
                    if (session->recv_buffer_.length() < sizeof(msg_header))
                        break;

                    unserialize(msghdr_, session_->recv_buffer_);
                    if (msghdr_.id_ < 0 || msghdr_.id_ > MaxMsgId)
                        session_->close();
                }

                if (msghdr_.body_len_ > session_->recv_buffer_.length())
                {
                    pending_flag_ = true;
                    break;
                }

                pending_flag_ = false;

                if (!handlers_[msghdr_.id_].wrapper_ || !handlers_[msghdr_.id_].func_)
                {
                    if (default_handler_.func_)
                        ((Delegate<bool (buffer&)>&)default_handler_.func_)(session_->recv_buffer_); 
                    else
                        session_->close();
                }
                else 
                {
                    handlers_[msghdr_.id_].wrapper_(handlers_[msghdr_.id_].func_, 
                        session_->recv_buffer_);
                }
            }
        }

    private:
        template <typename MsgType>
        static bool handler_wrapper(void* func, buffer& buf)
        {
            MsgType msg; 
            unserialize(msg, buf);
            return ((Delegate<bool (const MsgType&)>*)func)->operator () (msg);
        }

        struct handler
        {
            bool (*wrapper_)(void*, buffer&);
            void* func_; 
        };

        handler default_handler_;
        handler handlers_[MaxMsgId];
        inet::session* session_;
        bool pending_flag_;
        inet::msg_header msghdr_;
    };
} // namespace

#endif // #ifndef __LIBINET_MSG_HANDLER_H__

