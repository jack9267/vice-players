/*

	VC:MP Multiplayer Modification
	Copyright 2004-2005 SA:MP Team

	file:
		rcon.h
	desc:
		Remote Console Header.
	author:
		spookie

*/

#pragma once
#ifndef VCMPSRV_RCON_H
#define VCMPSRV_RCON_H

class CRcon
{
private:
	RakNet::RakPeerInterface* m_pRak;
public:
	CRcon(WORD iPort, char* szPass, WORD iMaxAdmins);
	~CRcon();

	RakNet::RakPeerInterface* GetRakPeer() { return m_pRak; };

	void Process();

	void ConsoleOutput(char* szOutput);
	void ConsolePrintf( char* szFormat, ... );

	void Packet_NewIncomingConnection(RakNet::Packet* pPacket);
	void Packet_DisconnectionNotification(RakNet::Packet* pPacket);
	void Packet_ConnectionLost(RakNet::Packet* pPacket);
	void Packet_ConnectionBanned(RakNet::Packet* pPacket);
	void Packet_RconCommand(RakNet::Packet* pPacket);

	void OutputChatMessage(char * szNick, char * szText);
	void OutputDeathMessage(int iDeathType,BYTE byteWhoDied,BYTE byteWhoKilled);


};

#endif