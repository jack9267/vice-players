//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//---------------------------------------------------

#pragma once

typedef struct _VEHICLE_SPAWN_INFO
{
	BYTE byteVehicleType;
	Vector3 vecPos;
	float fRotation;
	int iColor1;
	int iColor2;
} VEHICLE_SPAWN_INFO;

//----------------------------------------------------

class CVehicleManager
{
private:	
	BOOL				m_bVehicleSlotState[MAX_VEHICLES];
	CVehicle			*m_pVehicles[MAX_VEHICLES];
	VEHICLE_SPAWN_INFO	m_SpawnInfo[MAX_VEHICLES];
	BOOL				m_bIsActive[MAX_VEHICLES];
	BOOL				m_bIsWasted[MAX_VEHICLES];

public:
	
	CVehicleManager();
	~CVehicleManager();

	BOOL New(EntityId vehicleID, BYTE byteVehicleType,
			 Vector3 * vecPos, float fRotation,
			 int iColor1, int iColor2,
			 Vector3 * vecSpawnPos, float fSpawnRotation);

	BOOL Delete(EntityId vehicleID);	

	CVehicle* GetAt(EntityId vehicleID) {
		if(vehicleID > MAX_VEHICLES) { return NULL; }
		return m_pVehicles[vehicleID];
	};

	BOOL GetSlotState(EntityId vehicleID) {
		if(vehicleID > MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[vehicleID];
	};

	BOOL Spawn( EntityId vehicleID, BYTE byteVehicleType,
				Vector3 * vecPos, float fRotation,
				int iColor1, int iColor2 );

	EntityId FindIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle);
	int FindGtaIDFromID(EntityId vehicleID);

	void SendVehicleDeath(EntityId vehicleID);

	void Process();
};

//----------------------------------------------------