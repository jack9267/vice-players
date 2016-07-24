//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): bpeterson
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#pragma once

class CPickup 
{
private:
	EntityId m_PickupID;
	int m_iModel;
	int m_iType;
	Vector3 m_vecPos;

public:
	CPickup(int iModel, int iType, Vector3 * vecPos);
	~CPickup();

	void SetID(EntityId pickupID) { m_PickupID = pickupID; };
	
	void SpawnForPlayer(EntityId forPlayerID);
	void DestroyForPlayer(EntityId forPlayerID);
	void SpawnForWorld();
	void DestroyForWorld();
};