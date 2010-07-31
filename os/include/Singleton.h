/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_SINGLETON_H
#define IGAME_OS_SINGLETON_H

#include <stdlib.h>
#include "os/include/Mutex.h"

namespace IGame
{
    // Singleton object MUST BE initialized manually.
    template <typename Type>
    class Singleton  // Threadsafe support, Can't be used at static field
    {
    public:
        static Type& Instance() // Unique point of access
        {
            //if (s_Instance == 0)
            //{
            //    IGame::Lock lock(s_Mutex);
                if (s_Instance == 0)
                {
                    s_Instance = new(Type)();
                    atexit(Destroy);
                }
            //}
            return *s_Instance;
        }
    protected:
        Singleton() {}
        virtual ~Singleton() {}
    private:
        static void Destroy() // Destroy the only instance
        {
            if (s_Instance != 0)
            {
                delete(s_Instance);
                s_Instance = 0;
            }
        }
        static Type* volatile s_Instance; // The one and oly instance
        //static IGame::Mutex s_Mutex; 
    };

    //template<class Type>
    //IGame::Mutex Singleton<Type>::s_Mutex;
 
    template <typename Type>
    Type* volatile Singleton<Type >::s_Instance = 0;
} // namespace IGame

#endif // #ifndef IGAME_OS_SINGLETON_H

