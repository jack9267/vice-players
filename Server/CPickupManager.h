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

#define MAX_PICKUPS 128

class CPickupManager
{
private:
	bool m_bPickupSlotState[MAX_PICKUPS];
	CPickup * m_pPickups[MAX_PICKUPS];

public:
	CPickupManager();
	~CPickupManager();

	EntityId New(int iModel, int iType, Vector3 * vecPos);
	bool Delete(EntityId PickupID);

	CPickup * GetAt(EntityId PickupID)
	{
		if(PickupID > MAX_PICKUPS)
		{
			return NULL;
		}

		return m_pPickups[PickupID];
	};

	bool GetSlotState(EntityId PickupID)
	{
		if(PickupID > MAX_PICKUPS)
		{
			return false;
		}

		return m_bPickupSlotState[PickupID];
	};

	void InitForPlayer(EntityId playerID);
};