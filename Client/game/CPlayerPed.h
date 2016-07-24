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
#include "AimSync.h"

//-----------------------------------------------------------

#define FUNC_CPed__RemoveAllWeapons 0x4FF740
#define FUNC_CPed__GiveWeapon 0x4FFA30
#define FUNC_CPed__ResetSkin 0x4EF030
#define FUNC_CPed__Recreate 0x4EEFD0
#define FUNC_CPed__Say 0x5226B0
#define FUNC_CPed__SetDead 0x4F6430
#define FUNC_CPed__ClearObjective 0x521720
#define FUNC_TieMarkerToActor 0x4C3B40

#define MODEL_CELLPHONE 258

enum eObjectiveType
{
	OBJECTIVE_TYPE_KILL_1 = 8,
	OBJECTIVE_TYPE_KILL_2,
	OBJECTIVE_TYPE_AVOID,
	OBJECTIVE_TYPE_FOLLOW = 12,
	OBJECTIVE_TYPE_GO_TO,
	OBJECTIVE_TYPE_EXIT_CAR = 16,
	OBJECTIVE_TYPE_ENTER_CAR_AS_PASSENGER,
	OBJECTIVE_TYPE_ENTER_CAR_AS_DRIVER,
	OBJECTIVE_TYPE_FIRE_AT_TARGET = 21,
	OBJECTIVE_TYPE_RUN_AWAY,
	OBJECTIVE_TYPE_TIE = 29,
	OBJECTIVE_TYPE_AIM = 46
};

//-----------------------------------------------------------

class CPlayerPed : public CPhysical
{
private:
	DWORD		m_dwGTAId;
	BYTE		m_bytePlayerNumber;

public:
	// Constructor/Destructor.	
	CPlayerPed();
	CPlayerPed(int iPlayerNumber, int iSkin, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f);
	~CPlayerPed();

	PED_TYPE *     GetPed();

	void           SetKeys(WORD wKeys);
	WORD           GetKeys();
	bool           IsFiring();

	CAMERA_AIM *   GetCurrentAim();
	void           SetCurrentAim(CAMERA_AIM * pAim);

	void           Create(int iModel, float fX, float fY,float fZ,float fRotation);
	void           Destroy();
	void           SetDead();

	void           ShowMarker(int iMarkerColor);
	int            GetCurrentVehicleID();
	void           Say(UINT uiNum);
	void           SetInitialState();
	BOOL           IsOnScreen();

	void		   SetWaterDeaths(int iToggle);
	void		   SetCellAction(int iToggle);
	void		   Flash(int iItem);
	void		   SetActorBleeding(int iToggle);
	void		   SetSkyColor(int iColorID, int iFade);
	void		   SetDrivebyState(int iToggle);
	void		   SetCameraShakeIntensity(int iIntensity);

	float          GetHealth();
	void           SetHealth(float fHealth);
	void		   SetGameSpeed(float gSpeed);
	float          GetArmour();
	void           SetArmour(float fArmour);
	BOOL           IsDead();
	BOOL           IsInVehicle();
	BYTE           GetAction();
	void           SetAction(BYTE byteTrigger);
	BYTE           GetShootingFlags();
	void           SetShootingFlags(BYTE byteShooting);

	BOOL           EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY);

	float          GetRotation();
	void           SetRotation(float fRotation);

	BOOL           GiveWeapon(int iWeaponID, int iAmmo);
	void           ClearAllWeapons();
	BOOL           SetArmedWeapon(int iWeaponType);
	int            GetCurrentWeapon();
	BOOL           HasAmmoForCurrentWeapon();
	WEAPON_SLOT *  GetCurrentWeaponSlot();
	WEAPON_SLOT *  GetWeaponInSlot(BYTE byteSlot);

	void           SetModel(int iSkin);
	void           CheckAndRepairInvisProblems();
	void           ClearTargetAndVehicle();

	void           ClearLastWeaponDamage();
	void           SetObjective(DWORD * pObjEntity, eObjectiveType objectiveType);
	void           ClearObjective();
	void           SetImmunities(BOOL bBullet, BOOL bFire, BOOL bExplosion, BOOL bDamage, BOOL bUnknown);

	void           PutDirectlyInVehicle(int iVehicleID);
	void           EnterVehicleAsDriver(int iVehicleID);
	void           EnterVehicleAsPassenger(int iVehicleID);
	void           ExitCurrentVehicle();
	void           ForceIntoPassengerSeat(UINT uiVehicleID, UINT uiSeat);

	BOOL           IsAPassenger();

	VEHICLE_TYPE * GetGtaVehicle();

	void           RemoveFromVehicleAndPutAt(float fX, float fY, float fZ);
	void           TogglePlayerControllable(int iControllable);
	BYTE           FindDeathReasonAndResponsiblePlayer(EntityId * nPlayer);
	void           RestartIfWastedAt(Vector3 *vecRestart, float fRotation);
	BYTE           GetPassengerSeat();
};

//-----------------------------------------------------------
