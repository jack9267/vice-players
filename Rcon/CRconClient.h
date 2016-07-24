/*

	VC:MP Multiplayer Modification
	Copyright 2004-2005 SA:MP Team

	file:
		rconclient.h
	desc:
		Remote Console client header file.

*/

#pragma once

#ifndef SAMP_RCONCLIENT_H
#define SAMP_RCONCLIENT_H

using namespace RakNet;

class CRconClient
{
private:
	RakPeerInterface* m_pRakPeer;

public:
	CRconClient(char* szHostOrIp, int iPort, char* szPass);
	~CRconClient();

	bool IsConnected() { return (m_pRakPeer->GetConnectionState(UNASSIGNED_SYSTEM_ADDRESS) == IS_CONNECTED); };

	void Command(char* szCommand);
	void Process();

	void Packet_ConnectionRequestAccepted(Packet* pPacket);
	void Packet_DisconnectionNotification(Packet* pPacket);
	void Packet_ConnectionLost(Packet* pPacket);
	void Packet_ConnectionBanned(Packet* pPacket);
	void Packet_NoFreeIncomingConnections(Packet* pPacket);
	void Packet_InvalidPassword(Packet* pPacket);
	void Packet_RconResponce(Packet* pPacket);
};

#endif