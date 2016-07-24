//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"

extern CGame *pGame;
extern CNetworkManager *pNetowkManager;
extern CChatWindow *pChatWindow;

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
		if(m_bVehicleSlotState[i])
		{
			Delete(i);
		}
	}
}

//----------------------------------------------------

BOOL CVehicleManager::New( EntityId vehicleID, BYTE byteVehicleType,
					    Vector3 * vecPos, float fRotation,
					    int iColor1, int iColor2,
					    Vector3 * vecSpawnPos, float fSpawnRotation )
{

	// Setup the spawninfo for the next respawn.
	m_SpawnInfo[vehicleID].byteVehicleType = byteVehicleType;
	m_SpawnInfo[vehicleID].vecPos.X = vecSpawnPos->X;
	m_SpawnInfo[vehicleID].vecPos.Y = vecSpawnPos->Y;
	m_SpawnInfo[vehicleID].vecPos.Z = vecSpawnPos->Z;
	m_SpawnInfo[vehicleID].fRotation = fSpawnRotation;
	m_SpawnInfo[vehicleID].iColor1 = iColor1;
	m_SpawnInfo[vehicleID].iColor2 = iColor2;

	// Now go ahead and spawn it at the location we got passed.
	return Spawn(vehicleID,byteVehicleType,vecPos,fRotation,iColor1,iColor2);
}

//----------------------------------------------------

BOOL CVehicleManager::Delete(EntityId vehicleID)
{
	if(!GetSlotState(vehicleID) || !m_pVehicles[vehicleID]) {
		return FALSE;
	}

	m_bVehicleSlotState[vehicleID] = FALSE;
	delete m_pVehicles[vehicleID];
	m_pVehicles[vehicleID] = NULL;

	return TRUE;
}

//----------------------------------------------------

BOOL CVehicleManager::Spawn( EntityId vehicleID, BYTE byteVehicleType,
					      Vector3 * vecPos, float fRotation,
					      int iColor1, int iColor2 )
{	
	if(m_pVehicles[vehicleID] != NULL) {
		Delete(vehicleID);
	}

	m_pVehicles[vehicleID] =  new CVehicle(byteVehicleType,
		vecPos->X,vecPos->Y,vecPos->Z,fRotation);

	if(m_pVehicles[vehicleID])
	{	
		if(iColor1 != (-1)) {
			m_pVehicles[vehicleID]->SetColor(iColor1,iColor2);
		}

		m_bVehicleSlotState[vehicleID] = TRUE;

		m_bIsActive[vehicleID] = TRUE;
		m_bIsWasted[vehicleID] = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

EntityId CVehicleManager::FindIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle)
{
	for(EntityId i = 1; i < MAX_VEHICLES; i++)
	{
		if(m_bVehicleSlotState[i])
		{
			if(pGtaVehicle == m_pVehicles[i]->GetVehicle())
			{
				return i;
			}
		}
	}

	return INVALID_ENTITY_ID;
}

//----------------------------------------------------

int CVehicleManager::FindGtaIDFromID(EntityId vehicleID)
{
	if(m_pVehicles[vehicleID]) {
		return CPools::GetIndexFromVehicle(m_pVehicles[vehicleID]->GetVehicle());
	}

	// not too sure about this
	return 0;
}

//----------------------------------------------------

void CVehicleManager::SendVehicleDeath(EntityId vehicleID)
{
	BitStream bsVehicleDeath;
	bsVehicleDeath.Write(vehicleID);
	pNetowkManager->GetRPC4()->Call("VehicleDeath", &bsVehicleDeath, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
		UNASSIGNED_SYSTEM_ADDRESS, true);
}

//----------------------------------------------------

void CVehicleManager::Process()
{
	CVehicle *pVehicle;
	DWORD dwThisTime = GetTickCount();

	for(BYTE x = 0; x < MAX_VEHICLES; x++)
	{
		if(GetSlotState(x)) { // It's inuse.
			pVehicle = m_pVehicles[x];

			if(m_bIsActive[x]) {
				if((pVehicle->GetHealth() == 0.0f) || 
					(pVehicle->GetSubtype() != VEHICLE_SUBTYPE_BOAT &&
					pVehicle->GetSubtype() != VEHICLE_SUBTYPE_PLANE &&
					pVehicle->HasSunk())) { // It's dead or its not a boat and it has sunk
					if(!pVehicle->IsDead()) {
						SendVehicleDeath(x);
						pVehicle->SetDead(TRUE);
					}
				}
				else 
				{
					if(pVehicle->IsDriverLocalPlayer()) {
						pVehicle->SetInvulnerable(FALSE);
					} else {
						pVehicle->SetInvulnerable(TRUE);
					}

					// Lock vehicles beyond given radius.
					// whats the point in this?
					if(pVehicle->GetDistanceFromLocalPlayerPed() > 300.0f) {
						pVehicle->SetLockedState(1);
					} else {
						pVehicle->SetLockedState(0);
					}

					if(pVehicle->GetSubtype() == VEHICLE_SUBTYPE_BIKE) {
						pVehicle->VerifyControlState();
					}		
				}
			}
		}
	}
}

//----------------------------------------------------