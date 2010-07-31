/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_ENCODING_CONVERT_H
#define IGAME_OS_ENCODING_CONVERT_H

#include <string>

namespace IGame
{
    class EncodingConvert  // convert character set between gbk and utf8.
    {
    public:
        EncodingConvert(const char* from = "EUC-CN", const char* to = "UTF-8");
        ~EncodingConvert();

        std::string Convert(const char* src);
    public:
        bool m_IsG2U; // gbk to utf8
#ifndef _WIN32
        iconv_t m_Conv;
#endif
        std::string m_From;
        std::string m_To;
    };
} // namespace IGame

#endif // #ifndef IGAME_OS_ENCODING_CONVERT_H

