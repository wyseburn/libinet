/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_SINGLETON_OBJECT_H
#define IGAME_OS_SINGLETON_OBJECT_H

#include "os/include/Singleton.h"
#include "os/include/MemAllocator.h"
#include "os/include/EncodingConvert.h"

namespace IGame
{
    class SingletonObject : public Singleton<SingletonObject>
    {
    protected:
        SingletonObject() : m_Data(NULL), m_G2UConv("EUC-CN", "UTF-8"), m_U2GConv("UTF-8", "EUC-CN") {}
        virtual ~SingletonObject() {}

    public:
        MemAllocator<8, 32> m_Allocator;
        EncodingConvert m_G2UConv;
        EncodingConvert m_U2GConv;
        friend class Singleton<SingletonObject>;
        char* m_Data;
    };
} // namespace IGame

#define G2U(s) IGame::SingletonObject::Instance().m_G2UConv.Convert(s)
#define U2G(s) IGame::SingletonObject::Instance().m_U2GConv.Convert(s)

//#ifdef _USE_MEM_MANAGER
#define XALLOC IGame::SingletonObject::Instance().m_Allocator.Alloc
#define XFREE IGame::SingletonObject::Instance().m_Allocator.Dealloc
#define XNEW(ObjType) new (XALLOC(sizeof(ObjType))) ObjType
template <typename ObjType> void DestroyFunc(ObjType* obj) { obj->~ObjType(); XFREE(obj); }
#define XDELETE(obj) do { if (obj) DestroyFunc(obj); } while(0)
#define SAFE_DELETE(obj) do { XDELETE(obj); obj = NULL; } while(0)
//#else
//#define XALLOC malloc
//#define XFREE free
//#define XNEW new
//#define XDELETE delete
//#define SAFE_DELETE(obj) do { XDELETE (obj); obj = NULL; } while(0)
//#endif 

// Usage: 
// char* p = XALLOC(size);
// XFREE(p)
// char* p = XNEW(char)();
// SAFE_DELETE(p)

#endif // IGAME_OS_SINGLETON_OBJECT_H


