/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifdef WIN32
#include <atlbase.h>
#include <atlcom.h>
#include <atlconv.h>
#else
#include <iconv.h>
#endif
#include <cassert>

#include "os/include/EncodingConvert.h"

using namespace IGame;

EncodingConvert::EncodingConvert(const char* from, const char* to)
{
    m_From = from;
    m_To = to;

#ifndef _WIN32
    m_Conv = iconv_open(to, from);
    if (m_Conv == (iconv_t)-1)
    {
        assert(0);
    }
#else
    if (from == "EUC-CN")
    {
        m_IsG2U = true;
    }
    else if (from == "UTF-8")
    {
        m_IsG2U = false;
    }
    else
    {
        assert(0);
    }
#endif
}

EncodingConvert::~EncodingConvert()
{
#ifndef _WIN32
    if (m_Conv != (iconv_t)-1)
    {
        iconv_close(m_Conv);
    }
#endif
}

std::string 
EncodingConvert::Convert(const char* src)
{
    std::string retString;
#ifdef _WIN32
    if (m_IsG2U)
    {
        retString = (const char*)CW2A(CA2W(src, CP_ACP), CP_UTF8);
    }
    else
    {
        retString = (const char*)CW2A(CA2W(src, CP_UTF8), CP_ACP);
    }
#else
    if (m_Conv == (iconv_t)-1)
    {
        printf("failed to convert from %s to %s\n", m_From.c_str(), m_To.c_str());
        return src;
    }

    char tmpBuf[8192];
    size_t srcLen = strlen(src);
    size_t dstLen = sizeof(tmpBuf);
    char* dstBuf = (char*)tmpBuf;
    
    if (iconv(m_Conv, (char**)&src, &srcLen, (char**)&dstBuf, &dstLen) == -1)
    {
        printf("failed to convert from %s to %s\n", m_From.c_str(), m_To.c_str());
        return src;
    }
    *dstBuf = '\0';
    retString = tmpBuf;
#endif
    return retString;
}


