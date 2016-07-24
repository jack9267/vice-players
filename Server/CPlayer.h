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

#define UPDATE_TYPE_NONE		0
#define UPDATE_TYPE_FULL_ONFOOT	1
#define UPDATE_TYPE_MINI_ONFOOT 2
#define UPDATE_TYPE_FULL_INCAR	3
#define UPDATE_TYPE_MINI_INCAR  4
#define UPDATE_TYPE_PASSENGER	5

// Packs vehicle health into a byte
#define PACK_VEHICLE_HEALTH(f)		(BYTE)(f / 4)
#define UNPACK_VEHICLE_HEALTH(b)	(float)b * 4

typedef struct _C_VECTOR1 {
	short X,Y,Z;
} C_VECTOR1;

typedef struct _PLAYER_SPAWN_INFO
{
	BYTE byteTeam;
	BYTE byteSkin;
	Vector3 vecPos;
	float fRotation;
	int iSpawnWeapons[3];
	int iSpawnWeaponsAmmo[3];
	bool bLoaded;
} PLAYER_SPAWN_INFO;

typedef struct _PLAYER_SYNC_DATA
{
	WORD wKeys;
	Vector3 vecPos;
	float fRotation;
	BYTE byteCurrentWeapon;
	BYTE byteShootingFlags;
	BYTE byteHealth;
	BYTE byteArmour;
} PLAYER_SYNC_DATA;

typedef struct _VEHICLE_SYNC_DATA
{
	EntityId vehicleID;
	WORD wKeys;
	Vector3 vecRoll;
	Vector3 vecDirection;
	// No more compression
	//C_VECTOR1 cvecRoll;
	//C_VECTOR1 cvecDirection;
	Vector3 vecPos;
	Vector3 vecMoveSpeed;
	Vector3 vecTurnSpeed;
	BYTE byteVehicleHealth;
	BYTE bytePlayerHealth;
	BYTE bytePlayerArmour;
} VEHICLE_SYNC_DATA;

typedef struct _S_CAMERA_AIM
{
	Vector3 vecA1; // float f1x,f1y,f1z
	Vector3 vecAPos1; // float pos1x,pos1y,pos1z
	Vector3 vecAPos2; // float pos2x,pos2y,pos2z
	Vector3 vecA2; // float f2x,f2y,f2z
} S_CAMERA_AIM;

//----------------------------------------------------

class CPlayer
{
private:
	BYTE					m_bytePlayerID;
	BOOL					m_bIsActive;
	BOOL					m_bIsWasted;
	BYTE					m_byteUpdateFromNetwork;
	PLAYER_SPAWN_INFO		m_SpawnInfo;
	BOOL					m_bHasSpawnInfo;
	int						m_iMoney;
	float					m_iGravity;
	std::string				m_szSerial;
	int						m_iIsPaused;

public:

	WORD					m_wKeys;
	Vector3					m_vecRoll;
	Vector3					m_vecDirection;
	//C_VECTOR1				m_cvecRoll;
	//C_VECTOR1				m_cvecDirection;
	Vector3					m_vecPos;
	Vector3					m_vecMoveSpeed;
	Vector3					m_vecTurnSpeed;
	float					m_fRotation;
	float					m_fVehicleHealth;
	BYTE					m_byteHealth;
	BYTE					m_byteArmour;
	BYTE					m_byteCurrentWeapon;
	BYTE					m_byteShootingFlags;

	bool					m_bHasAim;
	S_CAMERA_AIM			m_Aiming; // server's version of the player aiming.

	BOOL					m_bIsInVehicle;
	BOOL					m_bIsAPassenger;
	BYTE					m_vehicleID;

	CPlayer();
	~CPlayer() {};

	BOOL IsActive() { return m_bIsActive; };
	BOOL IsWasted() { return m_bIsWasted; };

	void Process();
	BOOL ValidateSyncData();
	void BroadcastSyncData();	

	void Say(PCHAR szText, BYTE byteTextLength);
	void SetID(EntityId playerID) { m_bytePlayerID = playerID; };	
	
	void StoreOnFootFullSyncData(PLAYER_SYNC_DATA * pPlayerSyncData);

	void StoreAimSyncData(S_CAMERA_AIM * pAim);

	void SetReportedHealth(BYTE byteHealth) { m_byteHealth = byteHealth; };
	void SetReportedArmour(BYTE byteArmour) { m_byteArmour = byteArmour; };

	void StoreInCarFullSyncData(VEHICLE_SYNC_DATA * pVehicleSyncData);

	void SetSpawnInfo(BYTE byteTeam, BYTE byteSkin, Vector3 * vecPos, float fRotation,
		int iSpawnWeapon1, int iSpawnWeapon1Ammo, int iSpawnWeapon2, int iSpawnWeapon2Ammo,
		int iSpawnWeapon3, int iSpawnWeapon3Ammo);

	PLAYER_SPAWN_INFO * GetSpawnInfo() { return &m_SpawnInfo; };

	BYTE GetTeam() {
		if(m_bHasSpawnInfo) { return m_SpawnInfo.byteTeam; }
		else { return 255; }
	};
	BYTE GetSkin() {
		if(m_bHasSpawnInfo) { return m_SpawnInfo.byteSkin; }
		else { return 255; }
	};

	void UpdatePosition(float x, float y, float z) {
		m_vecPos.X=x; 
		m_vecPos.Y=y; 
		m_vecPos.Z=z;
	};

	std::string GetSerial() { return m_szSerial; };
	void SetSerial(const char * szSerial) { m_szSerial = szSerial; };

	int IsPaused() { return m_iIsPaused; };
	void SetPause(int bPause) { m_iIsPaused = bPause; };

	void HandleDeath(BYTE byteReason, BYTE byteWhoWasResponsible);
	void Spawn();
	void SpawnForWorld(BYTE byteTeam,BYTE byteSkin,Vector3 * vecPos,float fRotation);
	void SpawnForPlayer(BYTE byteForSystemAddress);

	void EnterVehicle(EntityId vehicleID,BYTE bytePassenger);
	void ExitVehicle(EntityId vehicleID);

	WORD GetKeys();
	void GetPosition(Vector3 * vecPosition);
	void GetMoveSpeed(Vector3 * vecMoveSpeed);
	void GetTurnSpeed(Vector3 * vecTurnSpeed);
	float GetRotation();
	BYTE GetHealth();
	BYTE GetArmour();
	int GetCash();
	float GetGravity();
	BYTE GetCurrentWeapon();
	BYTE GetAction();
	BOOL IsAPassenger();
	BOOL IsInVehicle() { return m_bIsInVehicle; };
	BYTE GetVehicleID();

	void SetSpawnSkin(int iSkin) { m_SpawnInfo.byteSkin = iSkin; };

	void SetGameTime(BYTE hours, BYTE minutes);

	void SetCameraPos(Vector3 vPos);
	void SetCameraRot(Vector3 vPos);
	void SetCameraLookAt(Vector3 vPoint);
	void SetCameraBehindPlayer();
	void SetCash(int Cash);
	void SetGravity(float amount);
	
};

//----------------------------------------------------

