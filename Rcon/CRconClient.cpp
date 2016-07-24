/*

	VC:MP Multiplayer Modification
	Copyright 2004-2005 SA:MP Team

	file:
		rconclient.cpp
	desc:
		Remote Console client.

*/

#include "main.h"

using namespace RakNet;

CRconClient::CRconClient(char* szHostOrIp, int iPort, char* szPass)
{
	m_pRakPeer = RakPeerInterface::GetInstance();
	m_pRakPeer->Startup(1, &SocketDescriptor(), 1);
	if (m_pRakPeer->Connect(szHostOrIp, (WORD)iPort, szPass, strlen(szPass)) == -1)
	{
		logprintf("Connection failed.");
	} else {
		logprintf("Connecting to %s:%d (%s)...", szHostOrIp, iPort, szPass);
	}
}

CRconClient::~CRconClient()
{
	m_pRakPeer->Shutdown(500);
	RakPeerInterface::DestroyInstance(m_pRakPeer);
}

void CRconClient::Command(char* szCommand)
{
	if (!szCommand) return;

	BYTE bytePacketId = ID_RCON_COMMAND;
	BitStream bsCommand;
	bsCommand.Write(bytePacketId);
	DWORD dwCmdLen = (DWORD)strlen(szCommand);
	bsCommand.Write(dwCmdLen);
	bsCommand.Write(szCommand,dwCmdLen);
	m_pRakPeer->Send(&bsCommand, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, TRUE);
}

void CRconClient::Process()
{
	if (!m_pRakPeer)
		return;

	Packet* pPacket;
	while(pPacket = m_pRakPeer->Receive())
	{
		switch (pPacket->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Packet_ConnectionRequestAccepted(pPacket);
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			Packet_DisconnectionNotification(pPacket);
			break;
		case ID_CONNECTION_LOST:
			Packet_ConnectionLost(pPacket);
			break;
		case ID_CONNECTION_BANNED:
			Packet_ConnectionBanned(pPacket);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			Packet_NoFreeIncomingConnections(pPacket);
			break;
		case ID_INVALID_PASSWORD:
			Packet_InvalidPassword(pPacket);
			break;
		case ID_RCON_RESPONSE:
			Packet_RconResponce(pPacket);
			break;
		}
		m_pRakPeer->DeallocatePacket(pPacket);		
	}
}

void CRconClient::Packet_ConnectionRequestAccepted(Packet* pPacket)
{
	logprintf("*** Remote Console connected.\n");
}

void CRconClient::Packet_DisconnectionNotification(Packet* pPacket)
{
	logprintf("*** Remote Console disconnected. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_ConnectionLost(Packet* pPacket)
{
	logprintf("*** Remote Console connection lost. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_ConnectionBanned(Packet* pPacket)
{
	logprintf("*** Remote Console unable to connect. You are banned. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_NoFreeIncomingConnections(Packet* pPacket)
{
	logprintf("*** Unable to connect to Remote Console. Max admin slots filled. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_InvalidPassword(Packet* pPacket)
{
	logprintf("*** Invalid Remote Console password. Exiting...");
	Sleep(3000);
	bQuitApp = true;
}

void CRconClient::Packet_RconResponce(Packet* pPacket)
{
	RakNet::BitStream bs(pPacket->data, pPacket->length, false);
	BYTE packetid;
	DWORD dwResponceLen;
	char szResponce[256];

	bs.Read(packetid);
	bs.Read(dwResponceLen);
	bs.Read(szResponce,dwResponceLen);

	szResponce[dwResponceLen] = '\0';

	logprintf(szResponce);
}