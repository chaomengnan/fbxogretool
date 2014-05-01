#ifndef XK_TEXTCODEC_H
#define XK_TEXTCODEC_H

#include <string>


#ifdef __GNUC__
#define CSET_GBK "GBK"
#define CSET_UTF8 "UTF-8"
#define LC_NAME_zh_CN "zh_CN"
#elif defined(_MSC_VER)
#define CSET_GBK "936"
#define CSET_UTF8 "65001"
#define LC_NAME_zh_CN   "Chinese_People's Republic of China"
#endif

#define LC_NAME_zh_CN_GBK LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8 LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT LC_NAME_zh_CN_UTF8

namespace Xk {

class TextCodec
{
public:
    static int asciiToUnicode(const char* szInput, wchar_t* wszOut, int nOutSize);
    static int utf8ToUnicode(const char* szInput, wchar_t* wszOut, int nOutSize);
    static int unicodeToAscii(const wchar_t* wszInput, char* szOut, int nOutSize);
    static int unicodeToUft8(const wchar_t* wszInput, char* szOut, int nOutSize);
    static int utf8ToAscii(const char* szInput, char* szOut, int nOutSize);
    static int utf8ToAscii(std::string& strInput);
    static int asciiToUft8(const char* szInput, char* szOut, int nOutSize);
    static int asciiToUft8(std::string& strInput);
};

}//namespace Xk

#endif