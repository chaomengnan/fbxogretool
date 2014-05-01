#ifndef XK_DEBUG_STR_H
#define XK_DEBUG_STR_H

#include "XkConfig.h"

#ifdef WIN32
//#ifndef _USRDLL
	#include <windows.h>
//#else
	//#include "stdafx.h"
//#endif
#endif

#include <stdio.h>
#include <stdarg.h>

/////////////////////////////////////////////////////////////////////////
//用于连接字符串的宏
/////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#define WIDEN2(x,y,z) x "(" #y "):" z "\r\n"
#define WIDEN(x,y,z) WIDEN2(x , y,z)
#define __WFILE__ WIDEN( __FILE__ , __LINE__, __FUNCTION__)
#else
#define WIDEN2(x,y,z) x "(" #y ")\n"
#define WIDEN(x,y,z) WIDEN2(x , y,z)
#define __WFILE__ WIDEN( __FILE__ , __LINE__, "")
#endif

#define JOIN2(x,y) x " | " y
#define JOIN(x,y) JOIN2(x,y)
#define MYJOIN(x) JOIN(x,__WFILE__)

static void XkDebugString(const char* szFormat,...)
{
	int nListCount = 0;
	va_list pArgList;

	int nMaxLength = 1024;
	char szBuf[1024] = { 0 };

	va_start(pArgList,szFormat);
#ifdef WIN32
    nListCount += _vsnprintf_s(szBuf+nListCount, 1024, nMaxLength-nListCount, szFormat, pArgList);
#else
	nListCount += vsnprintf(szBuf+nListCount,nMaxLength-nListCount,szFormat,pArgList);
#endif
	va_end(pArgList);

	if(nListCount > (nMaxLength-1)) nListCount = nMaxLength - 1;
	*(szBuf+nListCount) = '\0';

#ifdef WIN32
	OutputDebugStringA((LPCSTR)szBuf);
#else
	printf("%s\n",szBuf);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DBGSTRING宏，打印调试信息，可以通过SNH_DEBUG开关来禁用或者启用
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32 
	#if XK_ENABLE_DEBUG_STR == 1
		#define DBGSTRING(format,...) XkDebugString(format,__VA_ARGS__)
	#else
#define DBGSTRING(format,...) /\
/SnhDebugString(format,__VA_ARGS__) 
	#endif
#else	/* Other OS */
    #if XK_ENABLE_DEBUG_STR == 1 
        #define DBGSTRING(format, args...) XkDebugString(format, ##args)
    #else
#define DBGSTRING(format,...) /\
/XkDebugString(format,__VA_ARGS__)
    #endif /* Other OS */
#endif /* WIN32 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//XDBGSTRING宏，打印调试信息，并包含源代码文件和函数信息，可以通过SNH_DEBUG开关来禁用或者启用
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
	#if XK_ENABLE_DEBUG_STR == 1
		#define XDBGSTRING(format,...) XkDebugString(MYJOIN(format),__VA_ARGS__)
	#else
#define XDBGSTRING(format,...) /\
/XkDebugString(format,__VA_ARGS__) 
	#endif
#else	/* Other OS */
#if XK_ENABLE_DEBUG_STR == 1 
        #define XDBGSTRING(format, args...) XkDebugString(MYJOIN(format), ##args)
    #else
#define XDBGSTRING(format,...) /\
/XkDebugString(format,__VA_ARGS__)
    #endif /* Other OS */
#endif /* WIN32 */

#if XK_ENABLE_DEBUG_STR == 1
class XkBeginEnd
{
public:
    XkBeginEnd(const char* func, const char* szMsg = NULL) : szFunc(func)
    {
        if(szMsg != NULL)
            XkDebugString("******************** %s Begin\t Message: %s", szFunc, szMsg);
        else
            XkDebugString("******************** %s Begin", szFunc);
    }

    ~XkBeginEnd()
    {
        XkDebugString("******************** %s End", szFunc);
    }

private:
    const char* szFunc;
};
#define DBG_BEGIN_END(msg) XkBeginEnd b______e____(__FUNCTION__, msg);
#define DBG_BEGIN_END0() XkBeginEnd b______e______(__FUNCTION__);
#else
#define DBG_BEGIN_END(msg) /\
/
#define DBG_BEGIN_END0() /\
/
#endif

#endif
