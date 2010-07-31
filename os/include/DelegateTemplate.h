/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#include <utility> // for std::pair
#include "os/include/SingletonObject.h"

#define IGAME_DELEGATE_TEMPLATE_PARAMS    IGAME_MAKE_PARAMS1(IGAME_DELEGATE_NUM_ARGS, class T)
// class T0, class T1, class T2, ...
#define IGAME_DELEGATE_TEMPLATE_ARGS      IGAME_MAKE_PARAMS1(IGAME_DELEGATE_NUM_ARGS, T)
// T0, T1, T2, ...
#define IGAME_DELEGATE_FUNCTION_PARAMS    IGAME_MAKE_PARAMS2(IGAME_DELEGATE_NUM_ARGS, T, a)
// T0 a0, T1 a1, T2 a2, ...
#define IGAME_DELEGATE_FUNCTION_ARGS      IGAME_MAKE_PARAMS1(IGAME_DELEGATE_NUM_ARGS, a)
// a0, a1, a2, ...

// Comma if nonzero number of arguments
#if IGAME_DELEGATE_NUM_ARGS == 0
#define IGAME_DELEGATE_COMMA
#else
#define IGAME_DELEGATE_COMMA ,
#endif

namespace IGame
{
    //-------------------------------------------------------------------------
    // class Delegate<R (T1, T2, ..., TN)>

    template <class R IGAME_DELEGATE_COMMA IGAME_DELEGATE_TEMPLATE_PARAMS>
    class Delegate<R (IGAME_DELEGATE_TEMPLATE_ARGS)>
    {
    // Declaractions
    private:
        class DelegateImplBase
        {
            // Fields
        public:
            DelegateImplBase* m_Previous; // singly-linked list

            // Constructor/Destructor
        protected:
            DelegateImplBase() : m_Previous(NULL) { }
            DelegateImplBase(const DelegateImplBase& other) : m_Previous(NULL) { }
        public:
            virtual ~DelegateImplBase() { }

            // Methods
        public:
            virtual DelegateImplBase* Clone() const = 0;
            virtual R Invoke(IGAME_DELEGATE_FUNCTION_PARAMS) const = 0;
        };

        template <class TFunctor>
        struct Invoker
        {
            static R Invoke(const TFunctor& f IGAME_DELEGATE_COMMA IGAME_DELEGATE_FUNCTION_PARAMS)
            {
                return (const_cast<TFunctor&>(f))(IGAME_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TPtr, class TFunctionPtr>
        struct Invoker<std::pair<TPtr, TFunctionPtr> >
        {
            static R Invoke(const std::pair<TPtr, TFunctionPtr>& mf IGAME_DELEGATE_COMMA IGAME_DELEGATE_FUNCTION_PARAMS)
            {
                return ((*mf.first).*mf.second)(IGAME_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TFunctor>
        class DelegateImpl : public DelegateImplBase
        {
            // Fields
        public:
            TFunctor m_Functor;

            // Constructor
        public:
            DelegateImpl(const TFunctor& f) : m_Functor(f) {}
            DelegateImpl(const DelegateImpl& other) : m_Functor(other.m_Functor) {}

            // Methods
        public:
            virtual DelegateImplBase* Clone() const
            {
                return XNEW(DelegateImpl)(*this);
            }
            virtual R Invoke(IGAME_DELEGATE_FUNCTION_PARAMS) const
            {
                return Invoker<TFunctor>::Invoke(this->m_Functor IGAME_DELEGATE_COMMA IGAME_DELEGATE_FUNCTION_ARGS);
            }
        };

        // Fields
    private:
        DelegateImplBase* m_Last;

        // Constructor/Destructor
    public:
        Delegate()
        {
            this->m_Last = NULL;
        }

        template <class TFunctor>
        Delegate(const TFunctor& f)
        {
            this->m_Last = NULL;
            *this = f;
        }

        template<class TPtr, class TFunctionPtr>
        Delegate(const TPtr& obj, const TFunctionPtr& mfp)
        {
            this->m_Last = NULL;
            *this = std::make_pair(obj, mfp);
        }

        Delegate(const Delegate& d)
        {
            this->m_Last = NULL;
            *this = d;
        }

        ~Delegate()
        {
            Clear();
        }

        // Properties
    public:
        bool Empty() const
        {
            return (this->m_Last == NULL);
        }

        bool Multicast() const
        {
            return (this->m_Last != NULL && this->m_Last->m_Previous != NULL);
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
                    pp = &temp->m_Previous;
                    list2 = list2->m_Previous;
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
                temp = list->m_Previous;
                SAFE_DELETE(list);
                list = temp;
            }
        }

        static void InvokeDelegateList(DelegateImplBase* list IGAME_DELEGATE_COMMA IGAME_DELEGATE_FUNCTION_PARAMS)
        {
            if (list != NULL)
            {
                if (list->m_Previous != NULL)
                    InvokeDelegateList(list->m_Previous IGAME_DELEGATE_COMMA IGAME_DELEGATE_FUNCTION_ARGS);
                list->Invoke(IGAME_DELEGATE_FUNCTION_ARGS);
            }
        }

        // Methods
    public:
        template <class TFunctor>
        bool Exist(const TFunctor& f) const
        {
            DelegateImpl<TFunctor>* last = dynamic_cast<DelegateImpl<TFunctor>*>(this->m_Last);
            while (last)
            {
                if (last->m_Functor == f)
                {
                    return true;
                }
                else
                {
                    last = dynamic_cast<DelegateImpl<TFunctor>*>(last->m_Previous);
                }
            }
            return false;
        }

        template <class TFunctor>
        void Add(const TFunctor& f)
        {
            if (!Exist<TFunctor>(f))
            {
                DelegateImplBase* d = XNEW(DelegateImpl<TFunctor>)(f);
                d->m_Previous = this->m_Last;
                this->m_Last = d;
            }
        }

        template<class TPtr, class TFunctionPtr>
        void Add(const TPtr& obj, const TFunctionPtr& mfp)
        {
            Add(std::make_pair(obj, mfp));
            //if (!Exist<TPtr, TFunctionPtr>(obj, mfp))
            //{
            //    DelegateImplBase* d = new DelegateImpl<std::pair<TPtr, TFunctionPtr> >(std::make_pair(obj, mfp));
            //    d->m_Previous = this->m_Last;
            //    this->m_Last = d;
            //}
        }

        template <class TFunctor>
        bool Remove(const TFunctor& f)
        {
            DelegateImplBase* d = this->m_Last;
            DelegateImplBase** pp = &this->m_Last;
            DelegateImpl<TFunctor>* impl = NULL;

            while (d != NULL)
            {
                impl = dynamic_cast<DelegateImpl<TFunctor>*>(d);
                if (impl != NULL && impl->m_Functor == f)
                {
                    *pp = d->m_Previous;
                    SAFE_DELETE(impl);
                    return true;
                }
                pp = &d->m_Previous;
                d = d->m_Previous;
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
            FreeDelegateList(this->m_Last);
            this->m_Last = NULL;
        }

    private:
        template <class TFunctor>
        bool Equals(const TFunctor& f) const
        {
            if (this->m_Last == NULL || this->m_Last->m_Previous != NULL)
            return false;

            DelegateImpl<TFunctor>* impl = dynamic_cast<DelegateImpl<TFunctor>*>(this->m_Last);
            if (impl == NULL)
                return false;
            return (impl->m_Functor == f);
        }

        // Operators
    public:
        operator bool() const
        {
            return !Empty();
        }

        bool operator!() const
        {
            return Empty();
        }

        template <class TFunctor>
        Delegate& operator=(const TFunctor& f)
        {
            DelegateImplBase* d = XNEW(DelegateImpl<TFunctor>)(f);
            FreeDelegateList(this->m_Last);
            this->m_Last = d;
            return *this;
        }

        Delegate& operator=(const Delegate& d)
        {
            if (this != &d)
            {
                DelegateImplBase* list = CloneDelegateList(d.m_Last, NULL);
                FreeDelegateList(this->m_Last);
                this->m_Last = list;
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

        template <class TFunctor>
        bool operator==(const TFunctor& f)
        {
            return this->Exist(f);
        }

        //template <class TFunctor>
        //friend bool operator==(const Delegate& d, const TFunctor& f)
        //{
        //    return d.Exist(f);
        //}

        //template <class TFunctor>
        //friend bool operator==(const TFunctor& f, const Delegate& d)
        //{
        //   return (d == f);
        //}

        //template <class TFunctor>
        //friend bool operator!=(const Delegate& d, const TFunctor& f)
        //{
        //    return !(d == f);
        //}

        //template <class TFunctor>
        //friend bool operator!=(const TFunctor& f, const Delegate& d)
        //{
        //    return (d != f);
        //}

        R operator()(IGAME_DELEGATE_FUNCTION_PARAMS) const
        {
            if (this->m_Last == NULL)
                return R();

            if (this->m_Last->m_Previous != NULL)
                InvokeDelegateList(this->m_Last->m_Previous IGAME_DELEGATE_COMMA IGAME_DELEGATE_FUNCTION_ARGS);
            return this->m_Last->Invoke(IGAME_DELEGATE_FUNCTION_ARGS);
        }
    };
} // namesapce IGame

#undef IGAME_DELEGATE_TEMPLATE_PARAMS
#undef IGAME_DELEGATE_TEMPLATE_ARGS
#undef IGAME_DELEGATE_FUNCTION_PARAMS
#undef IGAME_DELEGATE_FUNCTION_ARGS
#undef IGAME_DELEGATE_COMMA

