#ifndef XK_SERVER_H
#define XK_SERVER_H

#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <RakPeerInterface.h>
#include "XkTypes.h"
#include "XkMsgCallback.h"

namespace Xk {
    
class Server : public Poco::Runnable
{
public:
    Server(MsgCallback* pCallback = NULL);
    ~Server();

    void SetName(const std::string& strName);

    bool Start(const char* szAddress, const char* szPasswd, int nPort, int nMaxCount);

    bool Disconnect(XK_GUID guid);

    bool IsRunning();

    void Stop();

    bool Send(XK_GUID guid, int nID, const void* pBuf, unsigned int nLength);

    bool GetMsg(MSG_PACKET& packet);

    void ReleaseMsg();

    void run();

    RakNet::RakPeerInterface* GetPeer()
    {
        return m_peer;
    }

private:
	std::string		m_strAddress;
	std::string		m_strPasswd;
	int             m_nPort;
    int             m_nMaxCount;
    bool            m_bRunning;
    RakNet::Packet* m_packet;
    RakNet::RakPeerInterface* m_peer;
    Poco::Thread    m_thread;

    MsgCallback*    m_pCallback;
    std::string     m_strName;
};

}//namespace Xk

#endif