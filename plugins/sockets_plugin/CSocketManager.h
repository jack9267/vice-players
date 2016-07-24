/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert
*
******************************************************/

#ifndef _CSOCKMANAGER_H
#define _CSOCKMANAGER_H

class CSocket;

#include <vector>
#include "main.h"

using namespace std;

class CSocketManager
{
public:
    static void DoPulse();
    
    static void SocketAdd   (CSocket*& pSocket);
    static bool SocketRemove(CSocket*& pSocket);
    static bool GetSocket(CSocket*& pSocket, int ID);

    static void HandleStop  ();
};

#endif
