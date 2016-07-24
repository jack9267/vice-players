//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

extern CScripts	*pScripts;

//----------------------------------------------------

CVehicleManager::CVehicleManager()
{
	for(EntityId i = 0; i < MAX_VEHICLES; i++)
	{
		m_bVehicleSlotState[i] = FALSE;
		m_pVehicles[i] = NULL;
	}
}

//----------------------------------------------------

CVehicleManager::~CVehicleManager()
{	
	for(EntityId i = 0; i < MAX_VEHICLES; i++)
	{
		Delete(i);
	}
}

//----------------------------------------------------

BOOL CVehicleManager::New(EntityId vehicleID, BYTE byteVehicleType,
					   Vector3 * vecPos, float fRotation,
					   int iColor1, int iColor2)
{
	m_pVehicles[vehicleID] = new CVehicle(byteVehicleType,vecPos,fRotation,iColor1,iColor2);

	/*
	logprintf("CVehicleManager::New(%u,%.2f,%.2f,%.2f,%.2f,%d,%d)",
		byteVehicleType,vecPos->X,vecPos->Y,vecPos->Z,fRotation,iColor1,iColor2);*/

	if(m_pVehicles[vehicleID])
	{
		m_pVehicles[vehicleID]->SetID(vehicleID);
		m_bVehicleSlotState[vehicleID] = TRUE;
		pScripts->onVehicleCreate(vehicleID);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BYTE CVehicleManager::New(BYTE byteVehicleType,
					   Vector3 * vecPos, float fRotation,
					   int iColor1, int iColor2)
{
	BYTE vehicleID = GetFreeSlot();
	if(New(vehicleID, byteVehicleType, vecPos, fRotation, iColor1, iColor2))
		return vehicleID;

	return false;
}

//----------------------------------------------------

BOOL CVehicleManager::Delete(EntityId vehicleID)
{
	if(!GetSlotState(vehicleID) || !m_pVehicles[vehicleID])
	{
		return FALSE;
	}

	m_bVehicleSlotState[vehicleID] = FALSE;
	delete m_pVehicles[vehicleID];
	m_pVehicles[vehicleID] = NULL;
	pScripts->onVehicleDestroy(vehicleID);

	return TRUE;
}

//----------------------------------------------------

void CVehicleManager::FlagForRespawn(EntityId vehicleID)
{
	if(GetSlotState(vehicleID)) {
		m_pVehicles[vehicleID]->SetWasted(TRUE);
		m_iRespawnDelay[vehicleID] = 150;
	}
}

//----------------------------------------------------

void CVehicleManager::Process()
{
	for(BYTE x = 0; x < MAX_VEHICLES; x++) {
		if(m_pVehicles[x]) {
			// does the vehicle need to be respawned?
			if(m_pVehicles[x]->IsWasted()) {
				if(m_iRespawnDelay[x] != 0) {
					m_iRespawnDelay[x]--;
				} else {
					// respawn this vehicle
					m_pVehicles[x]->Respawn();
					pScripts->onVehicleSpawn(x);
				}
			}

			// update the last driven time
			m_pVehicles[x]->UpdateLastDrivenTime();

			// has the vehicle been driven?
			if(m_pVehicles[x]->HasBeenDriven()) {
				// has this vehicle been inactive for 250 seconds?
				if((GetTickCount() - m_pVehicles[x]->GetTimeSinceLastDriven()) > 250000) {
					// respawn this vehicle
					m_pVehicles[x]->Respawn();
					pScripts->onVehicleSpawn(x);
				}
			}
		}
	}
}

//----------------------------------------------------

BYTE CVehicleManager::GetFreeSlot()
{
	for(BYTE id = 0; id < MAX_VEHICLES; id++)
	{
		if(m_pVehicles[id] == NULL)
			return id;
	}
	return false;
}

//----------------------------------------------------