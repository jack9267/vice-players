//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"

extern CNetworkManager * pNetowkManager;

//----------------------------------------------------

CSpawnSelection::CSpawnSelection()
{ 
	for(int i = 0; i < MAX_SPAWNS; i++) {
		m_AvailableSpawns[i].bLoaded = false;
	}
	m_iAvailableSpawnCount = 0;
};

//----------------------------------------------------

CSpawnSelection::~CSpawnSelection()
{

}

//----------------------------------------------------

int CSpawnSelection::AddPlayerClass(int team, int model, float x, float y, float z, float rot, int weapon1, int ammo1, int weapon2, int ammo2, int weapon3, int ammo3)
{
	for(int i = 0; i < MAX_SPAWNS; i++) {
		if(!m_AvailableSpawns[i].bLoaded) {
			m_AvailableSpawns[i].byteTeam = team;
			m_AvailableSpawns[i].byteSkin = model;
			m_AvailableSpawns[i].vecPos.X = x;
			m_AvailableSpawns[i].vecPos.Y = y;
			m_AvailableSpawns[i].vecPos.Z = z;
			m_AvailableSpawns[i].fRotation = rot;
			m_AvailableSpawns[i].iSpawnWeapons[0] = weapon1;
			m_AvailableSpawns[i].iSpawnWeaponsAmmo[0] = ammo1;
			m_AvailableSpawns[i].iSpawnWeapons[1] = weapon2;
			m_AvailableSpawns[i].iSpawnWeaponsAmmo[1] = ammo2;
			m_AvailableSpawns[i].iSpawnWeapons[2] = weapon3;
			m_AvailableSpawns[i].iSpawnWeaponsAmmo[2] = ammo3;
			m_AvailableSpawns[i].bLoaded = true;
			m_iAvailableSpawnCount++;
			return i;
		}
	}
	return 0;
}

//----------------------------------------------------

int CSpawnSelection::HandleSpawnClassRequest(EntityId playerID, int iSpawnType)
{
	CPlayer *pPlayer;
	pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);

	if(iSpawnType < 0) {
		iSpawnType = (m_iAvailableSpawnCount - 1);
	}
	else if(iSpawnType > (m_iAvailableSpawnCount - 1)) {
		iSpawnType = 0;
	}

	pPlayer->SetSpawnInfo(
		m_AvailableSpawns[iSpawnType].byteTeam,
		m_AvailableSpawns[iSpawnType].byteSkin,
		&m_AvailableSpawns[iSpawnType].vecPos,
		m_AvailableSpawns[iSpawnType].fRotation,
		m_AvailableSpawns[iSpawnType].iSpawnWeapons[0],
		m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[0],
		m_AvailableSpawns[iSpawnType].iSpawnWeapons[1],
		m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[1],
		m_AvailableSpawns[iSpawnType].iSpawnWeapons[2],
		m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[2]
		);

	return iSpawnType;
}

//----------------------------------------------------
// EOF