/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_DELEGATE_H
#define IGAME_OS_DELEGATE_H

#include <stdexcept>  // for std::runtime_error
#include <utility>    // for std::pair

// Macros for template metaprogramming

#define IGAME_JOIN(a, b)               a##b

#define IGAME_MAKE_PARAMS1_0(t)
#define IGAME_MAKE_PARAMS1_1(t)        t##1
#define IGAME_MAKE_PARAMS1_2(t)        t##1, t##2
#define IGAME_MAKE_PARAMS1_3(t)        t##1, t##2, t##3
#define IGAME_MAKE_PARAMS1_4(t)        t##1, t##2, t##3, t##4
#define IGAME_MAKE_PARAMS1_5(t)        t##1, t##2, t##3, t##4, t##5
#define IGAME_MAKE_PARAMS1_6(t)        t##1, t##2, t##3, t##4, t##5, t##6

#define IGAME_MAKE_PARAMS2_0(t1, t2)
#define IGAME_MAKE_PARAMS2_1(t1, t2)   t1##1 t2##1
#define IGAME_MAKE_PARAMS2_2(t1, t2)   t1##1 t2##1, t1##2 t2##2
#define IGAME_MAKE_PARAMS2_3(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3
#define IGAME_MAKE_PARAMS2_4(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4
#define IGAME_MAKE_PARAMS2_5(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5
#define IGAME_MAKE_PARAMS2_6(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5, t1##6 t2##6

#define IGAME_MAKE_PARAMS1(n, t)       IGAME_JOIN(IGAME_MAKE_PARAMS1_, n) (t)
#define IGAME_MAKE_PARAMS2(n, t1, t2)  IGAME_JOIN(IGAME_MAKE_PARAMS2_, n) (t1, t2)

namespace IGame
{
    class InvalidOperationException : public std::runtime_error
    {
    public:
        InvalidOperationException() : std::runtime_error("Invalidate operation") {}
        virtual ~InvalidOperationException() throw() {}
    };

    template <class TSignature>
    class Delegate; // no body
}

#pragma warning(push)
#pragma warning(disable: 4715)

#define IGAME_DELEGATE_NUM_ARGS	0 // Delegate<R ()>
#include "DelegateTemplate.h"
#undef IGAME_DELEGATE_NUM_ARGS

#define IGAME_DELEGATE_NUM_ARGS	1 // Delegate<R (T1)>
#include "DelegateTemplate.h"
#undef IGAME_DELEGATE_NUM_ARGS

#define IGAME_DELEGATE_NUM_ARGS	2 // Delegate<R (T1, T2)>
#include "DelegateTemplate.h"
#undef IGAME_DELEGATE_NUM_ARGS

#define IGAME_DELEGATE_NUM_ARGS	3 // Delegate<R (T1, T2, T3)>
#include "DelegateTemplate.h"
#undef IGAME_DELEGATE_NUM_ARGS

#define IGAME_DELEGATE_NUM_ARGS	4 // Delegate<R (T1, T2, T3, T4)>
#include "DelegateTemplate.h"
#undef IGAME_DELEGATE_NUM_ARGS

#define IGAME_DELEGATE_NUM_ARGS	5 // Delegate<R (T1, T2, T3, T4, T5)>
#include "DelegateTemplate.h"
#undef IGAME_DELEGATE_NUM_ARGS

#define IGAME_DELEGATE_NUM_ARGS	6 // Delegate<R (T1, T2, T3, T4, T5, T6)>
#include "DelegateTemplate.h"
#undef IGAME_DELEGATE_NUM_ARGS

#pragma warning(pop)

#endif // #ifndef IGAME_OS_DELEGATE_H


