/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_BLOB_H
#define IGAME_OS_BLOB_H

#include "os/include/Compat.h"

namespace IGame  
{
    struct Blob 
    { 
        Blob() : m_Len(0), m_Data(0) {}
        Blob(const Blob& blob) 
        {
            m_Len = blob.m_Len;
            m_Data = blob.m_Data;
        }

        Blob& operator = (const Blob& blob)
        {
            m_Len = blob.m_Len;
            m_Data = blob.m_Data;
            return *this;
        }

        _UInt32 m_Len;
        _Int8*  m_Data;
    };

    template <class Type> struct IsIGameBlob 
    { 
        static const bool s_Value = false; 
    };

    template <> struct IsIGameBlob<Blob> 
    { 
        static const bool s_Value = true; 
    };
} // namesapce IGame

#endif // ifndef IGAME_OS_BLOB_H


