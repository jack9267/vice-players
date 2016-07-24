//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "CGame.h"
#include "CPhysical.h"

//-----------------------------------------------------------

class CVehicle : public CPhysical
{
private:
	DWORD m_dwGTAId;
	BOOL  m_bIsInvulnerable;
	BOOL  m_bDead;

public:
	CVehicle(int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f);
	~CVehicle();

	VEHICLE_TYPE * GetVehicle();

	float          GetDistanceFromLocalPlayerPed();
	void           SetLockedState(int iLocked);

	BYTE           GetSubtype();

	float          GetHealth();
	void           SetHealth(float fHealth);
	void           SetColor(int iColor1, int iColor2);
	void           VerifyControlState();

	BOOL           HasSunk();
	BOOL           IsDriverLocalPlayer();
	void           EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY);
	BOOL           HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY);

	BOOL           GetHandbrakeState();
	void           SetHandbrakeState(BOOL bState);
	BYTE           GetMaxPassengers();

	BYTE           GetEntityFlags();
	void           SetEnitityFlags(BYTE byteEntityFlags);

	void           SetInvulnerable(BOOL bInv);
	BOOL           IsInvulnerable() { return m_bIsInvulnerable; };

	PED_TYPE *     GetDriver();

	void           SetImmunities(int iIm1, int iIm2, int iIm3, int iIm4, int iIm5);

	void           SetDead(BOOL bDead);
	BOOL           IsDead();

	void           PopTrunk();
	void           ToggleRhinoInstantExplosionUponContact(int iToggle);
};

//-----------------------------------------------------------
