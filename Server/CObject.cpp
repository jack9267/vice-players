//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------
#include "StdInc.h"

extern CNetworkManager *pNetowkManager;

CObject::CObject(int iModel, Vector3 * vecPos, Vector3 * vecRot)
{
	memcpy(&m_vecPos, vecPos, sizeof(Vector3));
	memcpy(&m_vecRot, vecRot, sizeof(Vector3));

	m_iModel = iModel;

	SpawnForWorld();
}

CObject::~CObject()
{
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(pPlayerManager->GetSlotState(i))
		{
			DestroyForPlayer(i);
		}
	}
}

void CObject::SpawnForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_ObjectID);
	bsSend.Write(m_iModel);
	bsSend.Write(m_vecPos);
	bsSend.Write(m_vecRot);
	
	pNetowkManager->GetRPC4()->Call("ObjectSpawn", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
}

void CObject::SpawnForWorld()
{
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerManager->GetSlotState(i)) {
			SpawnForPlayer(i);
		}
	}
}

void CObject::DestroyForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_ObjectID);

	pNetowkManager->GetRPC4()->Call("ObjectDestroy", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
}