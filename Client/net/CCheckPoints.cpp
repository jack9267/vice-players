//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): adamix
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------


#include "StdInc.h"

extern CGame * pGame;
extern CChatWindow * pChatWindow;
extern CNetworkManager * pNetowkManager;

// TODO: CSphere
#define MAX_SPHERES 16
#define VAR_Spheres 0x811528

struct SPHERE_TYPE
{
	BYTE byteInUse;
	BYTE pad[1];
	WORD wHandle;
	DWORD dwUniqueId;
	Vector3 vecCenter;
	float fRadius;
};

SPHERE_TYPE * GetSphereFromIndex(EntityId index)
{
	if(index >= MAX_SPHERES)
	{
		return NULL;
	}

	return (SPHERE_TYPE *)(VAR_Spheres + (index * sizeof(SPHERE_TYPE)));
}

EntityId GetFreeSphereIndex()
{
	for(EntityId i = 0; i < MAX_SPHERES; i++)
	{
		SPHERE_TYPE * pSphere = GetSphereFromIndex(i);

		if(pSphere && pSphere->byteInUse == 0)
		{
			return i;
		}
	}

	return 0xFF;
}

EntityId CreateSphere(Vector3 * vecCenter, float fRadius)
{
	EntityId index = GetFreeSphereIndex();

	if(index != 0xFF)
	{
		SPHERE_TYPE * pSphere = GetSphereFromIndex(index);
		pSphere->byteInUse = 1;
		pSphere->dwUniqueId = (index + 0xDEADBEEF);
		memcpy(&pSphere->vecCenter, vecCenter, sizeof(Vector3));
		pSphere->fRadius = fRadius;
	}

	return index;
}

float GetDistanceBetweenPoints3D(float x, float y, float z, float xx, float yy, float zz)
{
    float newx = (xx - x);
    float newy = (yy - y);
    float newz = (zz - z);
    return sqrt(newx * newx + newy * newy + newz * newz);
}


CCheckpoint::CCheckpoint(EntityId id, Vector3 vecPos, BYTE type, float radius)
{
	m_iID = id;
	memcpy(&m_vecPos, &vecPos, sizeof(Vector3));
	m_byteType = type;
	m_fRadius = radius;
	m_sphereID = NULL;
	m_dwMarker = NULL;
	m_bInCP = false;
	m_sphereID = CreateSphere(&vecPos, radius);
	ScriptCommand(&create_icon_marker_without_sphere, vecPos.X, vecPos.Y, vecPos.Z, 0, &m_dwMarker);
	ScriptCommand(&set_marker_color, m_dwMarker, 5);
	pGame->SetMarkerColor(m_dwMarker, 5);
	pGame->ShowMarker(m_dwMarker, 2);
}

CCheckpoint::~CCheckpoint()
{
	GetSphereFromIndex(m_sphereID)->byteInUse = 0;
	if(m_dwMarker != NULL)
	{
		ScriptCommand(&disable_marker, m_dwMarker);
	}
}

void CCheckpoint::Process()
{
	CPlayerPed * pPlayer = pGame->FindPlayerPed();
	Vector3 vecPos;
	pPlayer->GetPosition(&vecPos);

	if(GetDistanceBetweenPoints3D(vecPos.X, vecPos.Y, vecPos.Z, m_vecPos.X, m_vecPos.Y, m_vecPos.Z) < m_fRadius)
	{
		if(!m_bInCP)
		{
			BitStream bsSend;
			bsSend.Write(m_iID);
			bsSend.Write(true);
			pNetowkManager->GetRPC4()->Call("CheckpointEvent", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			m_bInCP = true;
		}
	}
	else
	{
		if(m_bInCP)
		{
			BitStream bsSend;
			bsSend.Write(m_iID);
			bsSend.Write(false);
			pNetowkManager->GetRPC4()->Call("CheckpointEvent", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			m_bInCP = false;
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

void CCheckpoints::New(EntityId id, Vector3 vecPos, BYTE type, float radius)
{
	if(GetSlotState(id) == true) 
	{
		Delete(id);
	}

	m_Checkpoints[id] = new CCheckpoint(id, vecPos, type, radius);
	if(m_Checkpoints[id])
	{
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
