//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"

extern CNetworkManager *pNetowkManager;
extern CRcon	*pRcon;
extern CScripts	*pScripts;

#ifndef WIN32
#	define stricmp strcasecmp
#endif

//----------------------------------------------------

using namespace RakNet;

//----------------------------------------------------

CPlayerManager::CPlayerManager()
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		m_bPlayerSlotState[i] = FALSE;
		m_pPlayers[i] = NULL;
	}
}

//----------------------------------------------------

CPlayerManager::~CPlayerManager()
{	
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		Delete(i, 0);
	}
}

//----------------------------------------------------

BOOL CPlayerManager::New(EntityId playerID, PCHAR szPlayerName, PCHAR szSerial)
{
	m_pPlayers[playerID] = new CPlayer();

	if(m_pPlayers[playerID])
	{
		strcpy(m_szPlayerName[playerID],szPlayerName);
		m_pPlayers[playerID]->SetID(playerID);
		m_pPlayers[playerID]->SetSerial(szSerial);
		m_bPlayerSlotState[playerID] = TRUE;
		m_iPlayerScore[playerID] = 0;
		m_bIsAnAdmin[playerID] = FALSE;

		// Notify all the other players of a newcommer with
		// a 'ServerJoin' join RPC 
		RakNet::BitStream bsSend;
		bsSend.Write(playerID);
		bsSend.Write(strlen(szPlayerName));
		bsSend.Write(szPlayerName,strlen(szPlayerName));
		pNetowkManager->GetRPC4()->Call("ServerJoin", &bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerID),true);

		logprintf("[join] %u %s [Serial: %s]",playerID,szPlayerName,szSerial);

		pScripts->onPlayerConnect(playerID);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CPlayerManager::Delete(EntityId playerID, BYTE byteReason)
{
	if(!GetSlotState(playerID) || !m_pPlayers[playerID])
	{
		return FALSE;
	}

	pScripts->onPlayerDisconnect(playerID, byteReason);

	logprintf("[part] %u %s %u",playerID,m_szPlayerName[playerID],byteReason);
	
	// Notify all the other players that this client is quiting.
	RakNet::BitStream bsSend;
	bsSend.Write(playerID);
	bsSend.Write(byteReason);
	pNetowkManager->GetRPC4()->Call("ServerQuit", &bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerID),true);
	
	m_bPlayerSlotState[playerID] = FALSE;
	delete m_pPlayers[playerID];
	m_pPlayers[playerID] = NULL;

	return TRUE;
}

//----------------------------------------------------

void CPlayerManager::Process()
{
	EntityId playerID = 0;

	while(playerID != MAX_PLAYERS) {
		if(TRUE == m_bPlayerSlotState[playerID]) {
			m_pPlayers[playerID]->Process();
		}
		playerID++;
	}
}

//----------------------------------------------------
// Add information to the pool about a kill.
// Return constant describing the scoring.

BYTE CPlayerManager::AddResponsibleDeath(BYTE byteWhoKilled, BYTE byteWhoDied)
{
	CPlayer *pWhoKilled;
	CPlayer *pWhoDied;

	if( byteWhoKilled != INVALID_ENTITY_ID &&
		byteWhoKilled < MAX_PLAYERS &&
		byteWhoDied < MAX_PLAYERS ) {

		if(m_bPlayerSlotState[byteWhoKilled] && m_bPlayerSlotState[byteWhoDied]) {

			pWhoKilled = m_pPlayers[byteWhoKilled];
			pWhoDied = m_pPlayers[byteWhoDied];

			if( (pWhoKilled->GetTeam() == 255) ||
				(pWhoKilled->GetTeam() != pWhoDied->GetTeam()) )
			{
				m_iPlayerScore[byteWhoKilled]++;

				pRcon->OutputDeathMessage(VALID_KILL,byteWhoDied,byteWhoKilled);

				return VALID_KILL;
			}
			else {
				m_iPlayerScore[byteWhoKilled]--;

				pRcon->OutputDeathMessage(TEAM_KILL,byteWhoDied,byteWhoKilled);

				return TEAM_KILL;
			}
		}
		return SELF_KILL;
	}

	if(byteWhoKilled == INVALID_ENTITY_ID && byteWhoDied < MAX_PLAYERS)
	{
		if(m_bPlayerSlotState[byteWhoDied]) {

			pRcon->OutputDeathMessage(SELF_KILL,byteWhoDied,0);

			return SELF_KILL;
		}
	}

	pRcon->OutputDeathMessage(SELF_KILL,byteWhoDied,0);
	
	return SELF_KILL;						
}

//----------------------------------------------------

float CPlayerManager::GetDistanceFromPlayerToPlayer(EntityId player1, EntityId player2)
{
	Vector3	*vecFromPlayer;
	Vector3	*vecThisPlayer;
	float	fSX,fSY;

	CPlayer * pPlayer1 = GetAt(player1);
	CPlayer * pPlayer2 = GetAt(player2);

	vecFromPlayer = &pPlayer1->m_vecPos;
	vecThisPlayer = &pPlayer2->m_vecPos;
	
	fSX = (vecThisPlayer->X - vecFromPlayer->X) * (vecThisPlayer->X - vecFromPlayer->X);
	fSY = (vecThisPlayer->Y - vecFromPlayer->Y) * (vecThisPlayer->Y - vecFromPlayer->Y);
	
	return (float)sqrt(fSX + fSY);	
}

//----------------------------------------------------

BOOL CPlayerManager::IsNickInUse(PCHAR szNick)
{
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(m_bPlayerSlotState[i])
		{
			if(!stricmp(GetPlayerName(i), szNick))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

//----------------------------------------------------

BOOL CPlayerManager::IsConnected(EntityId playerID)
{
	if(!GetSlotState(playerID) || !m_pPlayers[playerID])
	{
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------

void CPlayerManager::SetGameTime(BYTE hours, BYTE minutes)
{
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerManager->GetSlotState(i)) {
			pPlayerManager->GetAt(i)->SetGameTime(hours, minutes);
		}
	}
}

//----------------------------------------------------