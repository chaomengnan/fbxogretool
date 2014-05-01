#ifndef XK_MSGCALLBACK_H
#define XK_MSGCALLBACK_H

#include "XkTypes.h"

namespace Xk {

typedef unsigned long long XK_GUID;
const unsigned int ID_PROTO_MSG = XK_USER_MSGID_BEGIN;

class MsgCallback
{
public:
    virtual void OnNewConnection(XK_GUID guid) = 0;
    virtual void OnDisconnect(XK_GUID guid) = 0;
    virtual void OnLostConnection(XK_GUID guid) = 0;
    virtual void OnMsg(XK_GUID guid, int nID, const void* pBuf, int nLength) = 0;
};

}//namespace Xk

#endif