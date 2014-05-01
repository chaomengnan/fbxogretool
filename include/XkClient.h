#ifndef XK_CLIENT_H
#define XK_CLIENT_H

#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <RakPeerInterface.h>
#include "XkTypes.h"
#include "XkMsgCallback.h"

namespace Xk {

class Client : Poco::Runnable
{
public:
	Client(MsgCallback* pCallback = NULL);
	~Client();

    bool Connect(const char* szAddress,
        const char* szPasswd,
        int nPort,
        int nTimeout = 10);

    bool Connect(int nTimeout = 10);

    void SetName(const std::string& strName);

    void Disconnect();

    bool IsRunning();

    bool IsConnected();

    void Stop();

    bool Start();

    bool Send(int nID, const void* pBuf, unsigned int nLength);

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
    bool            m_bRunning;
    RakNet::Packet* m_packet;
    RakNet::RakPeerInterface* m_peer;
    Poco::Thread    m_thread;

    MsgCallback*    m_pCallback;
    std::string     m_strName;
};

}//namespace Xk
#endif