/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert, x86
*
******************************************************/

#ifndef _CSOCK_H
#define _CSOCK_H

#include "main.h"
#include "SocketErrors.h"
#include <string>

using namespace std;

#define SOCK_RECV_LIMIT 1024 // 1 kB

#ifdef WIN32
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <cstdlib>
    #include <sys/select.h>
    #include <errno.h>
    #include <fcntl.h>

    typedef int SOCKET;
#endif

class CSocket
{
public:
    CSocket           (SQVM *pVM, const string& strHost, const unsigned short& usPort);
    ~CSocket          ();

    bool Send         (const string& data);
    bool DoPulse      ();
    bool IsConnected  ();

	void SetID(int id) { m_iSocketID = id; };
	int GetID() { return m_iSocketID; };

private:
    bool ProcessTargetLocation (const string& strHost, const unsigned short& usPort);
    void SetNonBlocking        ();
    int  GetLastSocketError    ();
    void CloseSocket           ();
    int  HandleConnection      (const int& iError);
    void TriggerEvent          (const string eventName, const string arg1 = "");

    string             m_strHost;
    unsigned short     m_usPort;
    bool               m_bConnected;
    SQVM*         m_pVM;
    struct sockaddr_in m_sSockAddr;

	int				m_iSocketID;
    
    SOCKET             m_pSocket;
};

#endif
