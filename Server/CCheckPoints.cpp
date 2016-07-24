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
extern CScripts *pScripts;

float GetDistanceBetweenPoints3D(float x, float y, float z, float xx, float yy, float zz)
{
        float newx = xx - x;
        float newy = yy - y;
        float newz = zz - z;

        return sqrt(newx * newx + newy * newy + newz * newz);
}


CCheckpoint::CCheckpoint(EntityId id, Vector3 vecPos, BYTE type, float radius)
{
	m_iID = id;
	memcpy(&m_vecPos, &vecPos, sizeof(Vector3));
	m_byteType = type;
	m_fRadius = radius;
	m_bInCP = false;
}

CCheckpoint::~CCheckpoint()
{
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerManager->GetSlotState(i)) {
			BitStream bsSend;
			bsSend.Write(m_iID);
			pNetowkManager->GetRPC4()->Call("DestroyCheckpoint", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(i), 0);
		}
	}
}

void CCheckpoint::Process()
{

}

void CCheckpoint::InitForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_iID);
	bsSend.Write((char*)&m_vecPos, sizeof(Vector3));
	bsSend.Write(m_fRadius);
	
	pNetowkManager->GetRPC4()->Call("CreateCheckpoint", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
}

void CCheckpoint::InitForWorld()
{
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerManager->GetSlotState(i)) {
			InitForPlayer(i);
		}
	}
}

CCheckpoints::CCheckpoints()
{
	for(EntityId id = 0; id < MAX_CPS; id++)
	{
		m_Checkpoints[id] = NULL;
		m_bSlotState[id] = false;
	}
}

CCheckpoints::~CCheckpoints()
{

}

void CCheckpoints::New(Vector3 vecPos, BYTE type, float radius)
{
	EntityId id = GetFreeSlot();

	m_Checkpoints[id] = new CCheckpoint(id, vecPos, type, radius);
	if(m_Checkpoints[id])
	{
		m_Checkpoints[id]->InitForWorld();
		m_bSlotState[id] = true;
	}
}

void CCheckpoints::Delete(EntityId id)
{
	if(m_bSlotState[id] == false) return;
	delete m_Checkpoints[id];
	m_Checkpoints[id] = NULL;
	m_bSlotState[id] = false;
}

void CCheckpoints::Process()
{
	for(EntityId id = 0; id < MAX_CPS; id++)
	{
		if(!GetSlotState(id)) continue;
		m_Checkpoints[id]->Process();
	}
}

EntityId CCheckpoints::GetFreeSlot()
{
	for(EntityId id = 0; id < MAX_CPS; id++)
	{
		if(m_Checkpoints[id] == NULL)
			return id;
	}
	return false;
}

void CCheckpoints::InitForPlayer(EntityId playerId)
{
	for(EntityId id = 0; id < MAX_CPS; id++)
	{
		if(m_Checkpoints[id] == NULL)
			continue;

		m_Checkpoints[id]->InitForPlayer(playerId);
	}
}

void CCheckpoints::InitForWorld()
{
	for(EntityId id = 0; id < MAX_CPS; id++)
	{
		if(m_Checkpoints[id] == NULL)
			continue;

		m_Checkpoints[id]->InitForWorld();
	}
}