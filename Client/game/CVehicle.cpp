//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------
// TODO: Create/Destroy/SetModelIndex functions

#include "StdInc.h"

extern CGame		*pGame;
extern CChatWindow  *pChatWindow;

//-----------------------------------------------------------

CVehicle::CVehicle(int iType, float fPosX, float fPosY, float fPosZ, float fRotation)
{	
	DWORD dwRetID=0;
	m_dwGTAId = 0;
	m_bIsInvulnerable = FALSE;

	// Is the model not loaded?
	if(!pGame->IsModelLoaded(iType))
	{
		// Request the model
		pGame->RequestModel(iType);

		// Load all requested models
		pGame->LoadRequestedModels();

		// Wait for the model to load
		while(!pGame->IsModelLoaded(iType))
		{
			Sleep(2);
		}
	}

	ScriptCommand(&create_car, iType, fPosX, fPosY, fPosZ, &dwRetID);
	ScriptCommand(&set_car_z_angle, dwRetID, fRotation);
	SetEntity((ENTITY_TYPE*)CPools::GetVehicleFromIndex(dwRetID));
	m_dwGTAId = dwRetID;

	Vector3 vPos;
	GetPosition(&vPos);
	vPos.Z = fPosZ;
	SetPosition(vPos);

	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	pVehicle->dwDoorsLocked = 0;
	//m_pEntity->byteLockedFlags = 1;

	SetInvulnerable(TRUE);
	ToggleRhinoInstantExplosionUponContact(FALSE);
	m_bDead = FALSE;
}

//-----------------------------------------------------------

CVehicle::~CVehicle()
{
	// Get the vehicle pointer
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	// Remove ourself from the world
	CWorld::Remove((ENTITY_TYPE *)pVehicle);

	// Remove all references to ourself
	CWorld::Remove((ENTITY_TYPE *)pVehicle);

	// Call class destructor
	_asm
	{
		mov ecx, pVehicle
		mov ebx, [ecx]
		push 1
		call [ebx+8]
	}

	// Set our pointer to null
	SetEntity(NULL);
}

//-----------------------------------------------------------

VEHICLE_TYPE * CVehicle::GetVehicle()
{
	return (VEHICLE_TYPE *)GetEntity();
}

//-----------------------------------------------------------

void CVehicle::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	Vector3 vPos;
	Vector3 vecMoveSpeed;

	if(!GetEntity()) return;

	GetPosition(&vPos);
	GetMoveSpeed(&vecMoveSpeed);

	if(vPos.X > fPX) // greatest X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = -0.3f;
		}
		SetMoveSpeed(vecMoveSpeed);
		return;
	}
	else if(vPos.X < fZX)  // least X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = 0.3f;
		}
		SetMoveSpeed(vecMoveSpeed);
		return;
	}
	else if(vPos.Y > fPY) // Y coord check
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = -0.3f;
		}

		SetMoveSpeed(vecMoveSpeed);
		return;
	}
	else if(vPos.Y < fNY)
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = 0.3f;
		}

		SetMoveSpeed(vecMoveSpeed);
		return;
	}
}

//-----------------------------------------------------------

BOOL CVehicle::HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	Vector3 vPos;

	if(!GetEntity()) return FALSE;

	GetPosition(&vPos);

	if(vPos.X > fPX) {
		return TRUE;
	}
	else if(vPos.X < fZX) {
		return TRUE;
	}
	else if(vPos.Y > fPY) {
		return TRUE;
	}
	else if(vPos.Y < fNY) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float CVehicle::GetDistanceFromLocalPlayerPed()
{
	Vector3 vThisVehicle;
	Vector3 vFromPlayer;
	float  fSX,fSY;

	CPlayerPed *pLocalPlayerPed = pGame->FindPlayerPed();

	if(!GetEntity()) return 10000.0f; // very far away
	if(!pLocalPlayerPed) return 10000.0f; // very far away
	
	GetPosition(&vThisVehicle);
	pLocalPlayerPed->GetPosition(&vFromPlayer);
	
	fSX = (vThisVehicle.X - vFromPlayer.X) * (vThisVehicle.X - vFromPlayer.X);
	fSY = (vThisVehicle.Y - vFromPlayer.Y) * (vThisVehicle.Y - vFromPlayer.Y);
	
	return (float)sqrt(fSX + fSY);
}

//-----------------------------------------------------------

void CVehicle::SetInvulnerable(BOOL bInv)
{
	if(!GetEntity()) return;

	if(bInv && m_bIsInvulnerable == FALSE) {
		SetImmunities(1, 1, 1, 1, 1);
		m_bIsInvulnerable = TRUE;
	}
	else if(!bInv && m_bIsInvulnerable == TRUE)	{
		SetImmunities(0, 0, 0, 0, 0);
		m_bIsInvulnerable = FALSE;
	}
}
//-----------------------------------------------------------

void CVehicle::SetLockedState(int iLocked)
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();

	if(pPhysical)
	{
		pPhysical->byteLockedFlags = iLocked;
	}
}

//-----------------------------------------------------------
// Can be used to fix control state of motorbikes
// which have passengers without drivers.

void CVehicle::VerifyControlState()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		if(!pVehicle->pDriver && (pVehicle->physical.entity.nControlFlags == 0x2))
		{
			pVehicle->physical.entity.nControlFlags = 0x22;
		}
	}
}

//-----------------------------------------------------------

float CVehicle::GetHealth()
{	
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		return pVehicle->fHealth;
	}

	return 0.0f;
}

//-----------------------------------------------------------

void CVehicle::SetHealth(float fHealth)
{	
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		pVehicle->fHealth = fHealth;
	}
}	

//-----------------------------------------------------------

void CVehicle::SetColor(int iColor1, int iColor2)
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		pVehicle->byteColors[0] = (BYTE)iColor1;
		pVehicle->byteColors[1] = (BYTE)iColor2;
	}
}

//-----------------------------------------------------------
 
BYTE CVehicle::GetSubtype()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		DWORD dwFlags = pVehicle->pHandling->dwFlags;
		_asm
		{
			mov eax, dwFlags
			and eax, 0F0000h
			jz ret_car
			sub eax, 10000h
			jz ret_bike
			sub eax, 10000h
			jz ret_heli
			sub eax, 20000h
			jz ret_boat
			sub eax, 40000h
			jz ret_plane
		}

		return VEHICLE_SUBTYPE_NONE;

ret_car:
		return VEHICLE_SUBTYPE_CAR;
ret_bike:
		return VEHICLE_SUBTYPE_BIKE;
ret_heli:
		return VEHICLE_SUBTYPE_HELI;
ret_boat:
		return VEHICLE_SUBTYPE_BOAT;
ret_plane:
		return VEHICLE_SUBTYPE_PLANE;
	}

	return VEHICLE_SUBTYPE_NONE;
}

//-----------------------------------------------------------

BOOL CVehicle::HasSunk()
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();

	if(pPhysical)
	{
		if(pPhysical->byteSunkFlags & 0x10)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsDriverLocalPlayer()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		if(pVehicle->pDriver == GamePool_FindPlayerPed())
		{
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

BYTE CVehicle::GetMaxPassengers()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		return pVehicle->byteMaxPassengers;
	}

	return 0;
}

//-----------------------------------------------------------

PED_TYPE * CVehicle::GetDriver()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();

	if(pVehicle)
	{
		return pVehicle->pDriver;
	}

	return NULL;
}

//-----------------------------------------------------------

void CVehicle::SetImmunities(int iIm1, int iIm2, int iIm3, int iIm4, int iIm5)
{
	ENTITY_TYPE * pEntity = (ENTITY_TYPE *)GetEntity();

	if(pEntity)
	{
		if(iIm1) {
			pEntity->byteUnkFlags2 = (pEntity->byteUnkFlags2 & 0xFD | 2);
		} else {
			pEntity->byteUnkFlags2 &= 0xFD;
		}

		if(iIm2) {
			pEntity->byteUnkFlags2 = (pEntity->byteUnkFlags2 & 0xFB | 4);
		} else {
			pEntity->byteUnkFlags2 &= 0xFB;
		}

		if(iIm3) {
			pEntity->byteUnkFlags1 = (pEntity->byteUnkFlags1 & 0xFD | 2);
		} else {
			pEntity->byteUnkFlags1 &= 0xFD;
		}

		if(iIm4) {
			pEntity->byteUnkFlags2 = (pEntity->byteUnkFlags2 & 0xF7 | 8);
		} else {
			pEntity->byteUnkFlags2 &= 0xF7u;
		}

		if(iIm5) {
			pEntity->byteUnkFlags2 = (pEntity->byteUnkFlags2 & 0xEF | 0x10);
		} else {
			pEntity->byteUnkFlags2 &= 0xEFu;
		}
	}
}

//-----------------------------------------------------------

void CVehicle::SetDead(BOOL bDead)
{
	m_bDead = bDead;
}

//-----------------------------------------------------------

BOOL CVehicle::IsDead()
{
	return m_bDead;
}

//-----------------------------------------------------------

void CVehicle::PopTrunk()
{
	ScriptCommand(&pop_vehicle_trunk, m_dwGTAId);
}

//-----------------------------------------------------------

void CVehicle::ToggleRhinoInstantExplosionUponContact(int iToggle)
{
	if(GetModelIndex() == 162) // Rhino
	{
		AUTOMOBILE_TYPE * pAutomobile = (AUTOMOBILE_TYPE *)GetEntity();

		if(pAutomobile)
		{
			if(iToggle)
			{
				pAutomobile->byteUnknownFlags = (pAutomobile->byteUnknownFlags & 0xBF | 0x40);
			}
			else
			{
				pAutomobile->byteUnknownFlags &= 0xBF;
			}
		}
	}
}

//-----------------------------------------------------------
