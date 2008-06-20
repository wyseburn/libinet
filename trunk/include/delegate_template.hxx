/**
 *  Version:     @(#)libinet/delegate_template.hxx    0.0.2 23/05/2008
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
 *
 *  NOTE: Must be not include multi-include protect, such as #pragram once
 *  #ifndef ... #define ... #endif in this file.
 */

#include <utility> // for std::pair

#define INET_DELEGATE_TEMPLATE_PARAMS    INET_MAKE_PARAMS1(INET_DELEGATE_NUM_ARGS, class T)
// class T0, class T1, class T2, ...
#define INET_DELEGATE_TEMPLATE_ARGS      INET_MAKE_PARAMS1(INET_DELEGATE_NUM_ARGS, T)
// T0, T1, T2, ...
#define INET_DELEGATE_FUNCTION_PARAMS    INET_MAKE_PARAMS2(INET_DELEGATE_NUM_ARGS, T, a)
// T0 a0, T1 a1, T2 a2, ...
#define INET_DELEGATE_FUNCTION_ARGS      INET_MAKE_PARAMS1(INET_DELEGATE_NUM_ARGS, a)
// a0, a1, a2, ...

// Comma if nonzero number of arguments
#if INET_DELEGATE_NUM_ARGS == 0
#define INET_DELEGATE_COMMA
#else
#define INET_DELEGATE_COMMA ,
#endif

namespace inet
{
    //-------------------------------------------------------------------------
    // class delegate<R (T1, T2, ..., TN)>

    template <class R INET_DELEGATE_COMMA INET_DELEGATE_TEMPLATE_PARAMS>
    class delegate<R (INET_DELEGATE_TEMPLATE_ARGS)>
    {
    // Declaractions
    private:
        class delegate_impl_base
        {
            // Fields
        public:
            delegate_impl_base* previous; // singly-linked list

            // Constructor/Destructor
        protected:
            delegate_impl_base() : previous(NULL) { }
            delegate_impl_base(const delegate_impl_base& other) : previous(NULL) { }
        public:
            virtual ~delegate_impl_base() { }

            // Methods
        public:
            virtual delegate_impl_base* clone() const = 0;
            virtual R invoke(INET_DELEGATE_FUNCTION_PARAMS) const = 0;
        };

        template <class TFunctor>
        struct invoker
        {
            static R invoke(const TFunctor& f INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_PARAMS)
            {
                return (const_cast<TFunctor&>(f))(INET_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TPtr, class TFunctionPtr>
        struct invoker<std::pair<TPtr, TFunctionPtr> >
        {
            static R invoke(const std::pair<TPtr, TFunctionPtr>& mf INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_PARAMS)
            {
                return ((*mf.first).*mf.second)(INET_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TFunctor>
        class delegate_impl : public delegate_impl_base
        {
            // Fields
        public:
            TFunctor functor;

            // Constructor
        public:
            delegate_impl(const TFunctor& f) : functor(f) {}
            delegate_impl(const delegate_impl& other) : functor(other.functor) {}

            // Methods
        public:
            virtual delegate_impl_base* clone() const
            {
                return new delegate_impl(*this);
            }
            virtual R invoke(INET_DELEGATE_FUNCTION_PARAMS) const
            {
                return invoker<TFunctor>::invoke(this->functor INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_ARGS);
            }
        };

        // Fields
    private:
        delegate_impl_base* _last;

        // Constructor/Destructor
    public:
        delegate()
        {
            this->_last = NULL;
        }

        template <class TFunctor>
        delegate(const TFunctor& f)
        {
            this->_last = NULL;
            *this = f;
        }

        template<class TPtr, class TFunctionPtr>
        delegate(const TPtr& obj, const TFunctionPtr& mfp)
        {
            this->_last = NULL;
            *this = std::make_pair(obj, mfp);
        }

        delegate(const delegate& d)
        {
            this->_last = NULL;
            *this = d;
        }

        ~delegate()
        {
            clear();
        }

        // Properties
    public:
        bool empty() const
        {
            return (this->_last == NULL);
        }

        bool multicast() const
        {
            return (this->_last != NULL && this->_last->previous != NULL);
        }

        // Static Methods
    private:
        static delegate_impl_base* clone_delegate_list(delegate_impl_base* list, /*out*/ delegate_impl_base** first)
        {
            delegate_impl_base* list2 = list;
            delegate_impl_base* newList = NULL;
            delegate_impl_base** pp = &newList;
            delegate_impl_base* temp = NULL;

            try
            {
                while (list2 != NULL)
                {
                    temp = list2->clone();
                    *pp = temp;
                    pp = &temp->previous;
                    list2 = list2->previous;
                }
            }
            catch (...)
            {
                free_delegate_list(newList);
                throw;
            }

            if (first != NULL)
                *first = temp;
            return newList;
        }

        static void free_delegate_list(delegate_impl_base* list)
        {
            delegate_impl_base* temp = NULL;
            while (list != NULL)
            {
                temp = list->previous;
                delete list;
                list = temp;
            }
        }

        static void invoke_delegate_list(delegate_impl_base* list INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_PARAMS)
        {
            if (list != NULL)
            {
                if (list->previous != NULL)
                    invoke_delegate_list(list->previous INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_ARGS);
                list->invoke(INET_DELEGATE_FUNCTION_ARGS);
            }
        }

        // Methods
    public:
        template <class TFunctor>
        bool exist(const TFunctor& f) const
        {
            delegate_impl<TFunctor>* last = dynamic_cast<delegate_impl<TFunctor>*>(this->_last);
            while (last)
            {
                if (last->functor == f)
                {
                    return true;
                }
                else
                {
                    last = dynamic_cast<delegate_impl<TFunctor>*>(last->previous);
                }
            }
            return false;
        }

        template <class TFunctor>
        void add(const TFunctor& f)
        {
            if (!exist<TFunctor>(f))
            {
                delegate_impl_base* d = new delegate_impl<TFunctor>(f);
                d->previous = this->_last;
                this->_last = d;
            }
        }

        template<class TPtr, class TFunctionPtr>
        void add(const TPtr& obj, const TFunctionPtr& mfp)
        {
            add(std::make_pair(obj, mfp));
            //if (!Exist<TPtr, TFunctionPtr>(obj, mfp))
            //{
            //    delegate_impl_base* d = new delegate_impl<std::pair<TPtr, TFunctionPtr> >(std::make_pair(obj, mfp));
            //    d->previous = this->_last;
            //    this->_last = d;
            //}
        }

        template <class TFunctor>
        bool remove(const TFunctor& f)
        {
            delegate_impl_base* d = this->_last;
            delegate_impl_base** pp = &this->_last;
            delegate_impl<TFunctor>* impl = NULL;

            while (d != NULL)
            {
                impl = dynamic_cast<delegate_impl<TFunctor>*>(d);
                if (impl != NULL && impl->functor == f)
                {
                    *pp = d->previous;
                    delete impl;
                    return true;
                }
                pp = &d->previous;
                d = d->previous;
            }
            return false;
        }

        template<class TPtr, class TFunctionPtr>
        bool remove(const TPtr& obj, const TFunctionPtr& mfp)
        {
            return remove(std::make_pair(obj, mfp));
        }

        void clear()
        {
            free_delegate_list(this->_last);
            this->_last = NULL;
        }

    private:
        template <class TFunctor>
        bool equals(const TFunctor& f) const
        {
            if (this->_last == NULL || this->_last->previous != NULL)
            return false;

            delegate_impl<TFunctor>* impl = dynamic_cast<delegate_impl<TFunctor>*>(this->_last);
            if (impl == NULL)
                return false;
            return (impl->functor == f);
        }

        // Operators
    public:
        operator bool() const
        {
            return !empty();
        }

        bool operator!() const
        {
            return empty();
        }

        template <class TFunctor>
        delegate& operator=(const TFunctor& f)
        {
            delegate_impl_base* d = new delegate_impl<TFunctor>(f);
            free_delegate_list(this->_last);
            this->_last = d;
            return *this;
        }

        delegate& operator=(const delegate& d)
        {
            if (this != &d)
            {
                delegate_impl_base* list = clone_delegate_list(d._last, NULL);
                free_delegate_list(this->_last);
                this->_last = list;
            }
            return *this;
        }

        template <class TFunctor>
        delegate& operator+=(const TFunctor& f)
        {
            add(f);
            return *this;
        }

        template <class TFunctor>
        delegate& operator-=(const TFunctor& f)
        {
            remove(f);
            return *this;
        }

        //template <class TFunctor>
        //friend bool operator==(const delegate& d, const TFunctor& f)
        //{
        //    return d.exist(f);
        //}

        //template <class TFunctor>
        //friend bool operator==(const TFunctor& f, const delegate& d)
        //{
        //   return (d == f);
        //}

        //template <class TFunctor>
        //friend bool operator!=(const delegate& d, const TFunctor& f)
        //{
        //    return !(d == f);
        //}

        //template <class TFunctor>
        //friend bool operator!=(const TFunctor& f, const delegate& d)
        //{
        //    return (d != f);
        //}

        R operator()(INET_DELEGATE_FUNCTION_PARAMS) const
        {
            if (this->_last == NULL)
                return R();

            if (this->_last->previous != NULL)
                invoke_delegate_list(this->_last->previous INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_ARGS);
            return this->_last->invoke(INET_DELEGATE_FUNCTION_ARGS);
        }
    };
} // namesapce

#undef INET_DELEGATE_TEMPLATE_PARAMS
#undef INET_DELEGATE_TEMPLATE_ARGS
#undef INET_DELEGATE_FUNCTION_PARAMS
#undef INET_DELEGATE_FUNCTION_ARGS
#undef INET_DELEGATE_COMMA

