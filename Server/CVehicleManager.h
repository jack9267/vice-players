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

class CVehicleManager
{
private:
	
	BOOL m_bVehicleSlotState[MAX_VEHICLES];
	BYTE GetFreeSlot();
	CVehicle *m_pVehicles[MAX_VEHICLES];
	int m_iRespawnDelay[MAX_VEHICLES];

public:
	
	CVehicleManager();
	~CVehicleManager();

	BOOL New(EntityId vehicleID, BYTE byteVehicleType, Vector3 * vecPos, float fRotation, int iColor1, int iColor2);
	BYTE New(BYTE byteVehicleType, Vector3 * vecPos, float fRotation, int iColor1, int iColor2);
	BOOL Delete(EntityId vehicleID);	
		
	// Retrieve a vehicle by id
	CVehicle* GetAt(EntityId vehicleID) {
		if(vehicleID > MAX_VEHICLES) { return NULL; }
		return m_pVehicles[vehicleID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(EntityId vehicleID) {
		if(vehicleID > MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[vehicleID];
	};

	void FlagForRespawn(EntityId vehicleID);
	void Process();
};

//----------------------------------------------------