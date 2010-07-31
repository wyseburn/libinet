/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_OBJ_POOL_H
#define IGAME_OS_OBJ_POOL_H

#include "os/include/MemPool.h"

namespace IGame
{
    template <typename Type, _UInt32 Size = 20>
    class ObjPool : public MemPool<sizeof(Type), Size>
    {
    public:
        ObjPool() : MemPool<sizeof(Type), Size>() {}
        virtual ~ObjPool() {} 

        Type* Pop()
        {
            Type* obj = (Type*)MemPool<sizeof(Type), Size>::Alloc();
            obj = new (obj) Type();
            return obj;
        }

        template <typename ArgType1>
        Type* Pop(ArgType1& arg1)
        {
            Type* obj = (Type*)MemPool<sizeof(Type), Size>::Alloc();
            obj = new (obj) Type(arg1);
            return obj;
        }

        template <typename ArgType1, typename ArgType2>
        Type* Pop(ArgType1& arg1, ArgType2& arg2) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), Size>::Alloc();
            obj = new (obj) Type(arg1, arg2);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, typename ArgType3>
        Type* Pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), Size>::Alloc();
            obj = new (obj) Type(arg1, arg2, arg3);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, 
            typename ArgType3, typename ArgType4>
        Type* Pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3, ArgType4& arg4) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), Size>::Alloc();
            obj = new (obj) Type(arg1, arg2, arg3, arg4);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, typename ArgType3, 
            typename ArgType4, typename ArgType5>
        Type* Pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3, ArgType4& arg4, ArgType5& arg5) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), Size>::Alloc();
            obj = new (obj) Type(arg1, arg2, arg3, arg4, arg5);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, typename ArgType3, 
            typename ArgType4, typename ArgType5, typename ArgType6>
        Type* Pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3, 
            ArgType4& arg4, ArgType5& arg5, ArgType6& arg6) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), Size>::Alloc();
            obj = new (obj) Type(arg1, arg2, arg3, arg4, arg5, arg6);
            return obj;
        }

        void Push(void* obj)
        {
            if (obj == NULL) return;
            ((Type*)obj)->~Type();
            MemPool<sizeof(Type), Size>::Dealloc(obj);
        }
    };
} // namespace IGame

#endif // IGAME_OS_OBJ_POOL_H

/**
 * ObjPool Usage:
 * 
 * class ObjA
 * {
 * public:
 *     ObjA() {}
 *     int a;
 * };
 * 
 * class ObjB
 * {
 * public:
 *     ObjB(int b) : mb(b) {}
 *     int mb;
 * };
 *
 * class ObjC
 * {
 * public:
 *     ObjC(char c1, int c2) : mc1(c1), mc2(c2) {}
 *     char mc1;
 *     int mc2;
 * };
 *
 * int main()
 * {
 *     IGame::ObjPool<ObjA, 10> AObjPool;
 *     ObjA* a = AObjPool.Pop();
 *     AObjPool.Push(a);
 *     ObjA* a1 = AObjPool.Pop();
 *
 *     IGame::ObjPool<ObjB, 10> BObjPool;
 *     ObjB* b = XNEW(ObjB)(100);
 *     BObjPool.Push(b);
 *     ObjB* b1 = BObjPool.Pop(200);
 *    
 *     IGame::ObjPool<ObjC, 10> CObjPool;
 *     ObjC* c = CObjPool.Pop('1', 100);
 *     CObjPool.Push(c);
 *     ObjC* c1 = CObjPool.Pop('2', 500);
 *     
 *     return 0;
 * }
 */


