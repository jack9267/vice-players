//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

//----------------------------------------------------

#include "main.h"
#include "CPlayer.h"
#include "CPlayerManager.h"
#include "CVehicle.h"
#include "CVehicleManager.h"
#include "CObject.h"
#include "CObjectManager.h"
#include "CTimerPool.h"
#include "CText.h"
#include "CTextManager.h"
#include "CPickup.h"
#include "CPickupManager.h"
#include "CCheckPoints.h"
#include "../RakNet/MessageIdentifiers.h"
#include "netrpc.h"
#include "CSpawnSelection.h"

//----------------------------------------------------

#define UPDATE_TYPE_NONE	0
#define UPDATE_TYPE_FULL	1
#define UPDATE_TYPE_MINIMAL 2

#define GAMESTATE_STOPPED	0
#define GAMESTATE_RUNNING	1

#define INVALID_ID			0xFF

using namespace RakNet;

//----------------------------------------------------

class CNetworkManager
{
private:

	CPlayerManager					*m_pPlayerManager;
	CVehicleManager				*m_pVehicleManager;
	CTimerPool					*m_pTimerPool;
	CObjectManager					*m_pObjectManager;
	CPickupManager					*m_pPickupManager;
	CTextManager					*m_pTextManager;
	CCheckpoints				*m_pCheckpoints;
	RakPeerInterface			*m_pRakPeer;
	static RPC4					*m_pRPC4;
	CSpawnSelection				*m_pGameLogic;
	int							m_iGameState;
	int							m_iMaxPlayers;
	std::string					m_szHostname;
	bool						m_bPassworded;

	// TODO: Make private
public:
	Vector3						m_vecInitPlayerPos;
	Vector3						m_vecInitCameraPos;
	Vector3						m_vecInitCameraLook;
	float						m_WorldBounds[4]; // pos_x neg_x pos_y neg_y
	BYTE						m_byteFriendlyFire;
	BYTE						m_byteShowOnRadar;
	std::string					m_szModeName;
	std::string					m_szMapName;
private:
		
	void UpdateNetwork();
	void SetupInitPositions();

public:

	CNetworkManager(int iMaxPlayers, int iPort, char * szPassword, char * szHostname, BYTE byteFriendlyFire, BYTE byteShowOnRadar);
	~CNetworkManager();

	int GetGameState() { return m_iGameState; };
	CPlayerManager * GetPlayerManager() { return m_pPlayerManager; };
	CVehicleManager * GetVehicleManager() { return m_pVehicleManager; };
	CObjectManager * GetObjectManager() { return m_pObjectManager; };
	CPickupManager * GetPickupManager() { return m_pPickupManager; };
	CTextManager * GetTextManager() { return m_pTextManager; };
	CCheckpoints * GetCheckpoints() { return m_pCheckpoints; };
	CTimerPool * GetTimerPool() { return m_pTimerPool; };
	RakPeerInterface * GetRakPeer() { return m_pRakPeer; };
	RPC4 * GetRPC4() { return m_pRPC4; }
	CSpawnSelection * GetGameLogic() { return m_pGameLogic; };
	int	GetMaxPlayers() { return m_iMaxPlayers; };
	std::string GetHostname() { return m_szHostname; };
	std::string GetMapName() { return m_szMapName; };
	std::string GetModeName() { return m_szModeName; };
	bool IsPassworded() { return m_bPassworded; };
	
	void Process();
	void BroadcastData( BitStream *bitStream, PacketPriority priority,
						PacketReliability reliability,
						char orderingStream,
						EntityId excludedPlayer );

	void PlayerSync(Packet *p);
	void VehicleSync(Packet *p);
	void PassengerSync(Packet *p);
	void KickPlayer(EntityId playerID);
	void AddBan(char * ip_mask);
	void LoadBanList();
};

//----------------------------------------------------
