/**
 *  Version:     @(#)inet/delegate_template.hxx    0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     Copy from boost project.
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
#define INET_DELEGATE_COMMA    ,
#endif

namespace INet
{
    //-------------------------------------------------------------------------
    // class Delegate<R (T1, T2, ..., TN)>

    template <class R INET_DELEGATE_COMMA INET_DELEGATE_TEMPLATE_PARAMS>
    class Delegate<R (INET_DELEGATE_TEMPLATE_ARGS)>
    {
    // Declaractions
    private:
        class DelegateImplBase
        {
            // Fields
        public:
            DelegateImplBase* Previous; // singly-linked list

            // Constructor/Destructor
        protected:
            DelegateImplBase() : Previous(NULL) { }
            DelegateImplBase(const DelegateImplBase& other) : Previous(NULL) { }
        public:
            virtual ~DelegateImplBase() { }

            // Methods
        public:
            virtual DelegateImplBase* Clone() const = 0;
            virtual R Invoke(INET_DELEGATE_FUNCTION_PARAMS) const = 0;
        };

        template <class TFunctor>
        struct Invoker
        {
            static R Invoke(const TFunctor& f INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_PARAMS)
            {
                return (const_cast<TFunctor&>(f))(INET_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TPtr, class TFunctionPtr>
        struct Invoker<std::pair<TPtr, TFunctionPtr> >
        {
            static R Invoke(const std::pair<TPtr, TFunctionPtr>& mf INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_PARAMS)
            {
                return ((*mf.first).*mf.second)(INET_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TFunctor>
        class DelegateImpl : public DelegateImplBase
        {
            // Fields
        public:
            TFunctor Functor;

            // Constructor
        public:
            DelegateImpl(const TFunctor& f) : Functor(f) {}
            DelegateImpl(const DelegateImpl& other) : Functor(other.Functor) {}

            // Methods
        public:
            virtual DelegateImplBase* Clone() const
            {
                return new DelegateImpl(*this);
            }
            virtual R Invoke(INET_DELEGATE_FUNCTION_PARAMS) const
            {
                return Invoker<TFunctor>::Invoke(this->Functor INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_ARGS);
            }
        };

        // Fields
    private:
        DelegateImplBase* _last;

        // Constructor/Destructor
    public:
        Delegate()
        {
            this->_last = NULL;
        }

        template <class TFunctor>
        Delegate(const TFunctor& f)
        {
            this->_last = NULL;
            *this = f;
        }

        template<class TPtr, class TFunctionPtr>
        Delegate(const TPtr& obj, const TFunctionPtr& mfp)
        {
            this->_last = NULL;
            *this = std::make_pair(obj, mfp);
        }

        Delegate(const Delegate& d)
        {
            this->_last = NULL;
            *this = d;
        }

        ~Delegate()
        {
            Clear();
        }

        // Properties
    public:
        bool IsEmpty() const
        {
            return (this->_last == NULL);
        }

        bool IsMulticast() const
        {
            return (this->_last != NULL && this->_last->Previous != NULL);
        }

        // Static Methods
    private:
        static DelegateImplBase* CloneDelegateList(DelegateImplBase* list, /*out*/ DelegateImplBase** first)
        {
            DelegateImplBase* list2 = list;
            DelegateImplBase* newList = NULL;
            DelegateImplBase** pp = &newList;
            DelegateImplBase* temp = NULL;

            try
            {
                while (list2 != NULL)
                {
                    temp = list2->Clone();
                    *pp = temp;
                    pp = &temp->Previous;
                    list2 = list2->Previous;
                }
            }
            catch (...)
            {
                FreeDelegateList(newList);
                throw;
            }

            if (first != NULL)
                *first = temp;
            return newList;
        }

        static void FreeDelegateList(DelegateImplBase* list)
        {
            DelegateImplBase* temp = NULL;
            while (list != NULL)
            {
                temp = list->Previous;
                delete list;
                list = temp;
            }
        }

        static void InvokeDelegateList(DelegateImplBase* list INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_PARAMS)
        {
            if (list != NULL)
            {
                if (list->Previous != NULL)
                    InvokeDelegateList(list->Previous INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_ARGS);
                list->Invoke(INET_DELEGATE_FUNCTION_ARGS);
            }
        }

        // Methods
    private:
        template <class TFunctor>
        bool Exist(const TFunctor& f)
        {
            DelegateImpl<TFunctor>* last = dynamic_cast<DelegateImpl<TFunctor>*>(this->_last);
            while (last)
            {
                if (last->Functor == f)
                {
                    return true;
                }
                else
                {
                    last = dynamic_cast<DelegateImpl<TFunctor>*>(last->Previous);
                }
            }
            return false;
        }

        // Methods
    public:
        template <class TFunctor>
        void Add(const TFunctor& f)
        {
            if (!Exist<TFunctor>(f))
            {
                DelegateImplBase* d = new DelegateImpl<TFunctor>(f);
                d->Previous = this->_last;
                this->_last = d;
            }
        }

        template<class TPtr, class TFunctionPtr>
        void Add(const TPtr& obj, const TFunctionPtr& mfp)
        {
            Add(std::make_pair(obj, mfp));
            //if (!Exist<TPtr, TFunctionPtr>(obj, mfp))
            //{
            //    DelegateImplBase* d = new DelegateImpl<std::pair<TPtr, TFunctionPtr> >(std::make_pair(obj, mfp));
            //    d->Previous = this->_last;
            //    this->_last = d;
            //}
        }

        template <class TFunctor>
        bool Remove(const TFunctor& f)
        {
            DelegateImplBase* d = this->_last;
            DelegateImplBase** pp = &this->_last;
            DelegateImpl<TFunctor>* impl = NULL;

            while (d != NULL)
            {
                impl = dynamic_cast<DelegateImpl<TFunctor>*>(d);
                if (impl != NULL && impl->Functor == f)
                {
                    *pp = d->Previous;
                    delete impl;
                    return true;
                }
                pp = &d->Previous;
                d = d->Previous;
            }
            return false;
        }

        template<class TPtr, class TFunctionPtr>
        bool Remove(const TPtr& obj, const TFunctionPtr& mfp)
        {
            return Remove(std::make_pair(obj, mfp));
        }

        void Clear()
        {
            FreeDelegateList(this->_last);
            this->_last = NULL;
        }

    private:
        template <class TFunctor>
        bool Equals(const TFunctor& f) const
        {
            if (this->_last == NULL || this->_last->Previous != NULL)
            return false;

            DelegateImpl<TFunctor>* impl = dynamic_cast<DelegateImpl<TFunctor>*>(this->_last);
            if (impl == NULL)
                return false;
            return (impl->Functor == f);
        }

        // Operators
    public:
        operator bool() const
        {
            return !IsEmpty();
        }

        bool operator!() const
        {
            return IsEmpty();
        }

        template <class TFunctor>
        Delegate& operator=(const TFunctor& f)
        {
            DelegateImplBase* d = new DelegateImpl<TFunctor>(f);
            FreeDelegateList(this->_last);
            this->_last = d;
            return *this;
        }

        Delegate& operator=(const Delegate& d)
        {
            if (this != &d)
            {
                DelegateImplBase* list = CloneDelegateList(d._last, NULL);
                FreeDelegateList(this->_last);
                this->_last = list;
            }
            return *this;
        }

        template <class TFunctor>
        Delegate& operator+=(const TFunctor& f)
        {
            Add(f);
            return *this;
        }

        template <class TFunctor>
        Delegate& operator-=(const TFunctor& f)
        {
            Remove(f);
            return *this;
        }

        R operator()(INET_DELEGATE_FUNCTION_PARAMS) const
        {
            if (this->_last == NULL)
                return R();

            if (this->_last->Previous != NULL)
                InvokeDelegateList(this->_last->Previous INET_DELEGATE_COMMA INET_DELEGATE_FUNCTION_ARGS);
            return this->_last->Invoke(INET_DELEGATE_FUNCTION_ARGS);
        }
    };
} // namesapce

#undef INET_DELEGATE_TEMPLATE_PARAMS
#undef INET_DELEGATE_TEMPLATE_ARGS
#undef INET_DELEGATE_FUNCTION_PARAMS
#undef INET_DELEGATE_FUNCTION_ARGS
#undef INET_DELEGATE_COMMA

