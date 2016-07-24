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

	bool New(EntityId PickupID, int iModel, int iType, Vector3 * vecPos);
	bool Delete(EntityId PickupID);
};