#ifndef XK_REMOTELOGCHANNEL_H
#define XK_REMOTELOGCHANNEL_H

#include <Poco/Channel.h>
#include <Poco/Thread.h>
#include "XkClient.h"

namespace Xk {

#define XK_REMOTELOG_CHANNEL_CLIENT "RemoteLogChannel_Client"
#define XK_REMOTELOG_CHANNEL_THREAD "RemoteLogChannel_ReconnThread"

class RemoteLogChannel : public Poco::Channel, public MsgCallback, public Poco::Runnable
{
public:
    RemoteLogChannel();
    ~RemoteLogChannel();

    void log(const Poco::Message& msg);

public:
    void OnNewConnection(XK_GUID guid);
    void OnDisconnect(XK_GUID guid);
    void OnLostConnection(XK_GUID guid);
    void OnMsg(XK_GUID guid, int nID, const void* pBuf, int nLength);

public:
    void run();

private:
    Poco::Thread    m_thread;
    Client*         m_pClient;
};

}//namespace Xk

#endif