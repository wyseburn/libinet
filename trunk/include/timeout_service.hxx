/**
 *  Version:     @(#)libinet/timeout_service.hxx    0.0.2 19/06/2008
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

#ifndef __LIBINET_TIMEOUT_H__
#define __LIBINET_TIMEOUT_H__

#include "compat.hxx"
#include "delegate.hxx"
#include "util.hxx"

#if defined (__linux__) || defined (__FreeBSD__)
#include <sys/time.h>
#endif

namespace inet
{
    typedef delegate<void (void* /*args */)> TIMEOUT_EVENT_CB;
    template <inet_uint32 MaxTimeoutTime/*millisecond*/, inet_uint32 Precision/*millisecond*/>
    class timeout_service
    {
    public:
        timeout_service(inet_uint32 cache_size = 64) 
            : pointer_(0), maxmark_(MaxTimeoutTime / (Precision + 1)) // begin with 0  
            , cache_size_(cache_size), cache_len_(0)
        {
            for (inet_int32 i = 0; i <= maxmark_; ++i)
                INET_LIST_INIT(&handlers_[i]);
            INET_LIST_INIT(&cache_);
  
            {
#if defined (_WIN32)
                benchmark_ = GetTickCount();
#elif defined (__linux__) || defined (__FreeBSD__)
                struct timeval tv;
                gettimeofday(&tv, NULL);
                benchmark_ = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
            }
        }

        ~timeout_service() 
        {
            struct handler* handler;
            for (inet_int32 i = 0; i < maxmark_; ++i)
            {
                while (handler = INET_LIST_FIRST(&handlers_[i]))
                {
                    INET_LIST_REMOVE_HEAD(&handlers_[i], entries_);
                    delete handler;
                }
            }
                
            while (handler = INET_LIST_FIRST(&cache_))
            {
                INET_LIST_REMOVE_HEAD(&cache_, entries_);
                delete handler;
            }
        }

        bool regist(inet_uint32 duration/*millisecond*/, const TIMEOUT_EVENT_CB& cb, void* args)
        {
            if (duration <= 0 || duration > MaxTimeoutTime || cb.empty())
                return false;

            inet_int32 index = (duration % Precision) ? 
                ((duration / Precision) + pointer_) % (maxmark_ + 1) :
                ((duration / Precision) + pointer_) % (maxmark_ + 1) - 1;

            if (index < 0) index = 0;

            struct handler* handler = alloc_handler();
            if (!handler->cb_.exist(cb)) handler->cb_ = cb; 
            handler->args_ = args;

            if (handlers_[index].usrptr_ == NULL)
                INET_LIST_INSERT_HEAD(&handlers_[index], handler, entries_);
            else
                INET_LIST_INSERT_AFTER((struct handler *)handlers_[index].usrptr_, handler, entries_);
            handlers_[index].usrptr_ = handler;
            return true;
        }

        void update()
        {
#if defined (_WIN32)
                inet_uint32 curr_time = GetTickCount();
#elif defined (__linux__) || defined (__FreeBSD__)
                struct timeval tv;
                gettimeofday(&tv, NULL);
                inet_uint32 curr_time = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
            inet_uint32 steps = (curr_time - benchmark_) / Precision;

            if (steps == 0) return;
                
            for (inet_uint32 i = pointer_; i < steps && i <= maxmark_; ++i)
            {
                struct handler* handler;
                while (handler = INET_LIST_FIRST(&handlers_[i]))
                {
                    handler->cb_(handler->args_); 
                    INET_LIST_REMOVE_HEAD(&handlers_[i], entries_);
                    dealloc_handler(handler);
                }
                INET_LIST_INIT(&handlers_[i]);
            }

            if (steps >= maxmark_)
            {
                benchmark_ = curr_time;
                for (inet_int32 i = 0; i < steps % maxmark_ && i < pointer_; ++i)
                {
                    struct handler* handler;
                    while (handler = INET_LIST_FIRST(&handlers_[i]))
                    {
                        handler->cb_(handler->args_); 
                        INET_LIST_REMOVE_HEAD(&handlers_[i], entries_);
                        dealloc_handler(handler);
                    }
                    INET_LIST_INIT(&handlers_[i]);
                }
                pointer_ = 0;
            }
            else
            {
                pointer_ = steps;
            }
        }

    private:
        struct handler
        {
            TIMEOUT_EVENT_CB cb_;
            void* args_;
            INET_LIST_ENTRY(handler) entries_;
            handler() : args_(NULL) {} 
        };

        struct handler* alloc_handler()
        {
            struct handler* handler;
            if (cache_len_ <= 0) 
            {
                handler = (struct handler *)malloc(sizeof(struct handler));
                assert(handler);
                return new (handler) struct handler();
            }
				
            handler = INET_LIST_FIRST(&cache_);
            assert(handler); 
            INET_LIST_REMOVE_HEAD(&cache_, entries_);
            cache_len_--;
            return new (handler) struct handler();
        }

        void dealloc_handler(struct handler* handler)
        {
            assert(handler);
            if (cache_len_ >= cache_size_)
            {
                delete handler;
                return;
            }
            //handler->cb_ -= handler->cb_;
            INET_LIST_INSERT_HEAD(&cache_, handler, entries_);
            cache_len_++;
        }

        INET_LIST_HEAD(handler_list, handler) handlers_[MaxTimeoutTime/(Precision+1)+1];
        INET_LIST_HEAD(handler_cache_list, handler) cache_;

        inet_uint32 maxmark_;
        inet_uint32 benchmark_;     
        inet_uint32 pointer_;
        inet_uint32 cache_size_; 
        inet_uint32 cache_len_;
    };
} // namespace 

#endif // __LIBINET_TIMEOUT_H__
