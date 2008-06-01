/**
 *  Version:    @(#)inet/delegate.hxx    0.0.1 23/05/2008
 *  Authors:    Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :    Copy from boost project. 
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

#ifndef __INET_DELEGATE_H__
#define __INET_DELEGATE_H__

#include <stdexcept>  // for std::runtime_error
#include <utility>    // for std::pair

// Macros for template metaprogramming

#define INET_JOIN(a, b)        INET_DO_JOIN(a, b)
#define INET_DO_JOIN(a, b)     INET_DO_JOIN2(a, b)
#define INET_DO_JOIN2(a, b)    a##b

#define INET_MAKE_PARAMS1_0(t)
#define INET_MAKE_PARAMS1_1(t)    t##1
#define INET_MAKE_PARAMS1_2(t)    t##1, ##t##2
#define INET_MAKE_PARAMS1_3(t)    t##1, ##t##2, ##t##3
#define INET_MAKE_PARAMS1_4(t)    t##1, ##t##2, ##t##3, ##t##4
#define INET_MAKE_PARAMS1_5(t)    t##1, ##t##2, ##t##3, ##t##4, ##t##5
#define INET_MAKE_PARAMS1_6(t)    t##1, ##t##2, ##t##3, ##t##4, ##t##5, ##t##6

#define INET_MAKE_PARAMS2_0(t1, t2)
#define INET_MAKE_PARAMS2_1(t1, t2)   t1##1 t2##1
#define INET_MAKE_PARAMS2_2(t1, t2)   t1##1 t2##1, t1##2 t2##2
#define INET_MAKE_PARAMS2_3(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3
#define INET_MAKE_PARAMS2_4(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4
#define INET_MAKE_PARAMS2_5(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5
#define INET_MAKE_PARAMS2_6(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5, t1##6 t2##6

#define INET_MAKE_PARAMS1(n, t)         INET_JOIN(INET_MAKE_PARAMS1_, n) (t)
#define INET_MAKE_PARAMS2(n, t1, t2)    INET_JOIN(INET_MAKE_PARAMS2_, n) (t1, t2)

namespace INet 
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

#define INET_DELEGATE_NUM_ARGS	0 // Delegate<R ()>
#include "DelegateTemplate.hxx"
#undef INET_DELEGATE_NUM_ARGS

#define INET_DELEGATE_NUM_ARGS	1 // Delegate<R (T1)>
#include "DelegateTemplate.hxx"
#undef INET_DELEGATE_NUM_ARGS

#define INET_DELEGATE_NUM_ARGS	2 // Delegate<R (T1, T2)>
#include "DelegateTemplate.hxx"
#undef INET_DELEGATE_NUM_ARGS

#define INET_DELEGATE_NUM_ARGS	3 // Delegate<R (T1, T2, T3)>
#include "DelegateTemplate.hxx"
#undef INET_DELEGATE_NUM_ARGS

#define INET_DELEGATE_NUM_ARGS	4 // Delegate<R (T1, T2, T3, T4)>
#include "DelegateTemplate.hxx"
#undef INET_DELEGATE_NUM_ARGS

#define INET_DELEGATE_NUM_ARGS	5 // Delegate<R (T1, T2, T3, T4, T5)>
#include "DelegateTemplate.hxx"
#undef INET_DELEGATE_NUM_ARGS

#define INET_DELEGATE_NUM_ARGS	6 // Delegate<R (T1, T2, T3, T4, T5, T6)>
#include "DelegateTemplate.hxx"
#undef INET_DELEGATE_NUM_ARGS

#pragma warning(pop)

#endif // #ifndef __INET_DELEGATE_H__
