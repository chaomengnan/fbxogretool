#ifndef XK_LOG_H
#define XK_LOG_H

#include "XkConfig.h"
#include <string>
#include <Poco/Message.h>

namespace Xk {

class XkLog
{
public:
    static bool SetUp(bool bServer = false);
    static bool SearialMessage(const Poco::Message& msg, std::string& strOutput);
    static bool UnSerialMessage(std::string& strInput, Poco::Message& msg);
};

class XkLogHelp
{
public:
    static std::string GetFormatString(const char* szFormat, ...);
    static void LogDebug(const std::string& strLabel, const std::string& strMsg, const char* szFile, int nLine);
    static void LogWarning(const std::string& strLabel, const std::string& strMsg, const char* szFile, int nLine);
    static void LogError(const std::string& strLabel, const std::string& strMsg, const char* szFile, int nLine);
    static void LogDump(const std::string& strLabel, const std::string& strMsg, const void* pBuffer, size_t nLen);
};

#if XK_ENABLE_LOG_DEBUG == 1
#define XK_DEBUG(format, ...) Xk::XkLogHelp::LogDebug("root", Xk::XkLogHelp::GetFormatString(format, __VA_ARGS__), __FILE__, __LINE__)
#define XK_DEBUG_TO(label, format, ...) Xk::XkLogHelp::LogDebug(label, Xk::XkLogHelp::GetFormatString(format, __VA_ARGS__), __FILE__, __LINE__)
#else
#define XK_DEBUG(format, ...) /\
/
#define XK_DEBUG_TO(format, ...) /\
/
#endif

#if XK_ENABLE_LOG_WARNING == 1
#define XK_WARNING(format, ...) Xk::XkLogHelp::LogWarning("root", Xk::XkLogHelp::GetFormatString(format, __VA_ARGS__), __FILE__, __LINE__)
#define XK_WARNING_TO(label, format, ...) Xk::XkLogHelp::LogWarning(label, Xk::XkLogHelp::GetFormatString(format, __VA_ARGS__), __FILE__, __LINE__)
#else
#define XK_WARNING(format, ...) /\
/
#define XK_WARNING_TO(format, ...) /\
/
#endif

#if XK_ENABLE_LOG_ERROR == 1
#define XK_ERROR(format, ...) Xk::XkLogHelp::LogError("root", Xk::XkLogHelp::GetFormatString(format, __VA_ARGS__), __FILE__, __LINE__)
#define XK_ERROR_TO(label, format, ...) Xk::XkLogHelp::LogError(label, Xk::XkLogHelp::GetFormatString(format, __VA_ARGS__), __FILE__, __LINE__)
#else
#define XK_ERROR(format, ...) /\
/
#define XK_ERROR_TO(format, ...) /\
/
#endif

#if XK_ENABLE_LOG_DUMP == 1
#define XK_DUMP(msg, buffer, length) Xk::XkLogHelp::LogDump("root", msg, buffer, length)
#define XK_DUMP_TO(label, msg, buffer, length) Xk::XkLogHelp::LogDump(label, msg, buffer, length);
#else
#define XK_DUMP(msg, buffer, length) /\
/
#define XK_DUMP_TO(label, msg, buffer, length) /\
/
#endif

}//namespace Xk

#endif
