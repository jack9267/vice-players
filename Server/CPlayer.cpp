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

extern CNetworkManager *pNetowkManager;
extern CScripts	*pScripts;

using namespace RakNet;

//----------------------------------------------------

void DecompressVector1(VECTOR_PAD * vec, C_VECTOR1 * c1)
{
	vec->X = (float)c1->X;
	vec->X = (float)((double)vec->X / 10000.0);
	vec->Y = (float)c1->Y;
	vec->Y = (float)((double)vec->Y / 10000.0);
	vec->Z = (float)c1->Z;
	vec->Z = (float)((double)vec->Z / 10000.0);	
}

//----------------------------------------------------

CPlayer::CPlayer()
{
	m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
	m_bytePlayerID = INVALID_ENTITY_ID;
	m_bIsActive = FALSE;
	m_bIsWasted = FALSE;
	m_vehicleID = 0;
	m_iMoney = 0;
	float m_iGravity = 0.008f;
	m_bHasAim = false;
}

//----------------------------------------------------

void CPlayer::Process()
{
	if(m_bIsActive)
	{
		if(m_byteUpdateFromNetwork != UPDATE_TYPE_NONE) 
		{
			if(ValidateSyncData()) {
				BroadcastSyncData();
			}
			m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
		}
	}
}

//----------------------------------------------------
// thx CKY.BAM <g>

BOOL CPlayer::ValidateSyncData()
{
	if(m_vecPos.X > 2500.0f || m_vecPos.X < -2500.0f) {
		return FALSE;
	}
	if(m_vecPos.Y > 2500.0f || m_vecPos.Y < -2500.0f) {
		return FALSE;
	}
	if(m_vecPos.Z > 300.0f || m_vecPos.Z  < 0.0f) {
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------

void CPlayer::BroadcastSyncData()
{
	RakNet::BitStream bsSync;
		
	if(m_byteUpdateFromNetwork == UPDATE_TYPE_FULL_ONFOOT)
	{
		PLAYER_SYNC_DATA playerSyncData;
		bsSync.Write((MessageID)ID_PLAYER_SYNC);
		bsSync.Write(m_bytePlayerID);
		playerSyncData.wKeys = m_wKeys;
		memcpy(&playerSyncData.vecPos, &m_vecPos, sizeof(Vector3));
		playerSyncData.fRotation = m_fRotation;
		playerSyncData.byteCurrentWeapon = m_byteCurrentWeapon;
		playerSyncData.byteShootingFlags = m_byteShootingFlags;
		playerSyncData.byteHealth = m_byteHealth;
		playerSyncData.byteArmour = m_byteArmour;
		bsSync.Write((char *)&playerSyncData, sizeof(PLAYER_SYNC_DATA));

		if(m_bHasAim)
		{
			bsSync.Write1();
			bsSync.Write((char *)&m_Aiming, sizeof(S_CAMERA_AIM));
			m_bHasAim = false;
		}
		else
		{
			bsSync.Write0();
		}
		
		pNetowkManager->BroadcastData(&bsSync,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0,m_bytePlayerID);
	}
	else if(m_byteUpdateFromNetwork == UPDATE_TYPE_FULL_INCAR)
	{
		VEHICLE_SYNC_DATA vehicleSyncData;
		bsSync.Write((MessageID)ID_VEHICLE_SYNC);
		bsSync.Write(m_bytePlayerID);
		vehicleSyncData.vehicleID = m_vehicleID;
		vehicleSyncData.wKeys = m_wKeys;
		memcpy(&vehicleSyncData.vecRoll, &m_vecRoll, sizeof(Vector3));
		memcpy(&vehicleSyncData.vecDirection, &m_vecDirection, sizeof(Vector3));
		memcpy(&vehicleSyncData.vecPos, &m_vecPos, sizeof(Vector3));
		memcpy(&vehicleSyncData.vecMoveSpeed, &m_vecMoveSpeed, sizeof(Vector3));
		memcpy(&vehicleSyncData.vecTurnSpeed, &m_vecTurnSpeed, sizeof(Vector3));
		vehicleSyncData.byteVehicleHealth = PACK_VEHICLE_HEALTH(m_fVehicleHealth);
		vehicleSyncData.bytePlayerHealth = m_byteHealth;
		vehicleSyncData.bytePlayerArmour = m_byteArmour;
		bsSync.Write((char *)&vehicleSyncData, sizeof(VEHICLE_SYNC_DATA));

		pNetowkManager->BroadcastData(&bsSync,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0,m_bytePlayerID);
	}	
}

//----------------------------------------------------

void CPlayer::StoreOnFootFullSyncData(PLAYER_SYNC_DATA * pPlayerSyncData)
{
	if(m_vehicleID != 0) {
		pNetowkManager->GetVehicleManager()->GetAt(m_vehicleID)->SetDriverId(INVALID_ENTITY_ID);
		m_vehicleID = 0;
	}

	// update player data
	m_wKeys = pPlayerSyncData->wKeys;
	memcpy(&m_vecPos, &pPlayerSyncData->vecPos, sizeof(Vector3));
	m_fRotation = pPlayerSyncData->fRotation;
	m_byteCurrentWeapon = pPlayerSyncData->byteCurrentWeapon;
	m_byteShootingFlags = pPlayerSyncData->byteShootingFlags;

	// note: should do something like this for armour too (or add armour to this callback)?
	if(GetHealth() != pPlayerSyncData->byteHealth) {
		pScripts->onPlayerDamage(m_bytePlayerID, GetHealth(), pPlayerSyncData->byteHealth);
	}

	m_byteHealth = pPlayerSyncData->byteHealth;
	m_byteArmour = pPlayerSyncData->byteArmour;

	// update network update type
	m_byteUpdateFromNetwork = UPDATE_TYPE_FULL_ONFOOT;

	// call onPlayerSync scripting callback
	pScripts->onPlayerSync(m_bytePlayerID);
}

//----------------------------------------------------

void CPlayer::StoreAimSyncData(S_CAMERA_AIM * pAim)
{
	m_bHasAim = true;
	memcpy(&m_Aiming, pAim, sizeof(S_CAMERA_AIM));
}

//----------------------------------------------------

void CPlayer::StoreInCarFullSyncData(VEHICLE_SYNC_DATA * pVehicleSyncData)
{
	// get the vehicle pointer
	CVehicle * pVehicle = pNetowkManager->GetVehicleManager()->GetAt(pVehicleSyncData->vehicleID);

	// make sure vehicle is valid
	if(!pVehicle)
	{
		// sending vehicle sync data for invalid vehicle
		return;
	}

	// update player data
	m_vehicleID = pVehicleSyncData->vehicleID;
	m_wKeys = pVehicleSyncData->wKeys;
	memcpy(&m_vecRoll, &pVehicleSyncData->vecRoll, sizeof(Vector3));
	memcpy(&m_vecDirection, &pVehicleSyncData->vecDirection, sizeof(Vector3));
	//memcpy(&m_cvecRoll, &pVehicleSyncData->cvecRoll, sizeof(C_VECTOR1));
	//memcpy(&m_cvecDirection, &pVehicleSyncData->cvecDirection, sizeof(C_VECTOR1));
	memcpy(&m_vecPos, &pVehicleSyncData->vecPos, sizeof(Vector3));
	memcpy(&m_vecMoveSpeed, &pVehicleSyncData->vecMoveSpeed, sizeof(Vector3));
	memcpy(&m_vecTurnSpeed, &pVehicleSyncData->vecTurnSpeed, sizeof(Vector3));
	m_fVehicleHealth = UNPACK_VEHICLE_HEALTH(pVehicleSyncData->byteVehicleHealth);
	m_byteHealth = pVehicleSyncData->bytePlayerHealth;
	m_byteArmour = pVehicleSyncData->bytePlayerArmour;
	m_bIsInVehicle = TRUE;
	m_bIsAPassenger = FALSE;

	// update network update type
	m_byteUpdateFromNetwork = UPDATE_TYPE_FULL_INCAR;

	// call onPlayerSync scripting callback
	pScripts->onPlayerSync(m_bytePlayerID);

	MATRIX4X4 matWorld;

	// copy the roll and direction vectors into the new matrix
	memcpy(&matWorld.vLookRight, &pVehicleSyncData->vecRoll, sizeof(Vector3));
	memcpy(&matWorld.vLookUp, &pVehicleSyncData->vecDirection, sizeof(Vector3));
	// decompress the roll and direction vectors into the new vehicle matrix
	//DecompressVector1(&matWorld.vLookRight, pVehicleSyncData->cvecRoll);
	//DecompressVector1(&matWorld.vLookUp, pVehicleSyncData->cvecDirection);

	// copy the vehicle pos into the new vehicle matrix
	memcpy(&matWorld.vPos, &pVehicleSyncData->vecPos, sizeof(Vector3));

	// update the vehicle data
	pVehicle->Update(m_bytePlayerID, &matWorld, &pVehicleSyncData->vecMoveSpeed, &pVehicleSyncData->vecTurnSpeed, m_fVehicleHealth);
}

//----------------------------------------------------

void CPlayer::Say(PCHAR szText, BYTE byteTextLength)
{

}

//----------------------------------------------------

void CPlayer::HandleDeath(BYTE byteReason, BYTE byteWhoWasResponsible)
{
	RakNet::BitStream bsPlayerDeath;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	m_bIsActive = FALSE;
	m_bIsWasted = TRUE;

	BYTE byteScoringModifier;

	byteScoringModifier = 
		pNetowkManager->GetPlayerManager()->AddResponsibleDeath(byteWhoWasResponsible,m_bytePlayerID);

	bsPlayerDeath.Write(m_bytePlayerID);
	bsPlayerDeath.Write(byteReason);
	bsPlayerDeath.Write(byteWhoWasResponsible);
	bsPlayerDeath.Write(byteScoringModifier);
	
	// Broadcast it
	pNetowkManager->GetRPC4()->Call("Death", &bsPlayerDeath,HIGH_PRIORITY,RELIABLE,0,playerid,true);
	
	logprintf("<%s> died",
		pNetowkManager->GetPlayerManager()->GetPlayerName(m_bytePlayerID),
		byteReason,byteWhoWasResponsible,byteScoringModifier);
}

//----------------------------------------------------

void CPlayer::SetSpawnInfo(BYTE byteTeam, BYTE byteSkin, Vector3 * vecPos, float fRotation,
		int iSpawnWeapon1, int iSpawnWeapon1Ammo, int iSpawnWeapon2, int iSpawnWeapon2Ammo,
		int iSpawnWeapon3, int iSpawnWeapon3Ammo)
{
	m_SpawnInfo.byteTeam = byteTeam;
	m_SpawnInfo.byteSkin = byteSkin;
	m_SpawnInfo.vecPos.X = vecPos->X;
	m_SpawnInfo.vecPos.Y = vecPos->Y;
	m_SpawnInfo.vecPos.Z = vecPos->Z;
	m_SpawnInfo.fRotation = fRotation;
	m_SpawnInfo.iSpawnWeapons[0] = iSpawnWeapon1;
	m_SpawnInfo.iSpawnWeapons[1] = iSpawnWeapon2;
	m_SpawnInfo.iSpawnWeapons[2] = iSpawnWeapon3;
	m_SpawnInfo.iSpawnWeaponsAmmo[0] = iSpawnWeapon1Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[1] = iSpawnWeapon2Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[2] = iSpawnWeapon3Ammo;
	m_bHasSpawnInfo = TRUE;
}

//----------------------------------------------------

void CPlayer::Spawn()
{
	if(m_bHasSpawnInfo) {

		// Reset all their sync attributes.
		m_vecPos.X = m_SpawnInfo.vecPos.X;
		m_vecPos.Y = m_SpawnInfo.vecPos.Y;
		m_vecPos.Z = m_SpawnInfo.vecPos.Z;

		m_fRotation = m_SpawnInfo.fRotation;
		memset(&m_vecMoveSpeed,0,sizeof(Vector3));
		memset(&m_vecTurnSpeed,0,sizeof(Vector3));
		m_fRotation = 0.0f;
		m_bIsInVehicle=FALSE;
		m_bIsAPassenger=FALSE;
		m_vehicleID=0;
				
		SpawnForWorld(m_SpawnInfo.byteTeam,m_SpawnInfo.byteSkin,&m_SpawnInfo.vecPos,m_SpawnInfo.fRotation);
	}
}

//----------------------------------------------------
// This is the method used for respawning.

void CPlayer::SpawnForWorld( BYTE byteTeam, BYTE byteSkin, Vector3 * vecPos, 
							  float fRotation )
{
	RakNet::BitStream bsPlayerSpawn;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	bsPlayerSpawn.Write(m_bytePlayerID);
	bsPlayerSpawn.Write(byteTeam);
	bsPlayerSpawn.Write(byteSkin);
	bsPlayerSpawn.Write(vecPos->X);
	bsPlayerSpawn.Write(vecPos->Y);
	bsPlayerSpawn.Write(vecPos->Z);
	bsPlayerSpawn.Write(fRotation);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[2]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[2]);
	
	pNetowkManager->GetRPC4()->Call("Spawn", &bsPlayerSpawn,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,true);

	m_bIsActive = TRUE;
	m_bIsWasted = FALSE;

	// Set their initial sync position to their spawn position.
	m_vecPos.X = vecPos->X;
	m_vecPos.Y = vecPos->Y;
	m_vecPos.Z = vecPos->Z;

	m_byteShootingFlags = 1;
}

//----------------------------------------------------
// This is the method used for spawning players that
// already exist in the world when the client connects.

void CPlayer::SpawnForPlayer(BYTE byteForSystemAddress)
{
	RakNet::BitStream bsPlayerSpawn;

	bsPlayerSpawn.Write(m_bytePlayerID);
	bsPlayerSpawn.Write(m_SpawnInfo.byteTeam);
	bsPlayerSpawn.Write(m_SpawnInfo.byteSkin);
	bsPlayerSpawn.Write(m_vecPos.X);
	bsPlayerSpawn.Write(m_vecPos.Y);
	bsPlayerSpawn.Write(m_vecPos.Z);
	bsPlayerSpawn.Write(m_fRotation);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[0]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[1]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeapons[2]);
	bsPlayerSpawn.Write(m_SpawnInfo.iSpawnWeaponsAmmo[2]);

	pNetowkManager->GetRPC4()->Call("Spawn", &bsPlayerSpawn,HIGH_PRIORITY,RELIABLE_ORDERED,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(byteForSystemAddress),false);
}

//----------------------------------------------------

void CPlayer::EnterVehicle(EntityId vehicleID, BYTE bytePassenger)
{
	RakNet::BitStream bsVehicle;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	bsVehicle.Write(m_bytePlayerID);
	bsVehicle.Write(vehicleID);
	bsVehicle.Write(bytePassenger);

	pNetowkManager->GetRPC4()->Call("EnterVehicle", &bsVehicle,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,true);
}

//----------------------------------------------------

void CPlayer::ExitVehicle(EntityId vehicleID)
{
	RakNet::BitStream bsVehicle;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	bsVehicle.Write(m_bytePlayerID);
	bsVehicle.Write(vehicleID);

	pNetowkManager->GetRPC4()->Call("ExitVehicle", &bsVehicle,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,true);
}

//----------------------------------------------------

WORD CPlayer::GetKeys()
{
	return m_wKeys;
}

//----------------------------------------------------

void CPlayer::GetPosition(Vector3 * vecPosition)
{
	memcpy(vecPosition, &m_vecPos, sizeof(Vector3));
}

//----------------------------------------------------

void CPlayer::GetMoveSpeed(Vector3 * vecMoveSpeed)
{
	memcpy(vecMoveSpeed, &m_vecMoveSpeed, sizeof(Vector3));
}

//----------------------------------------------------

void CPlayer::GetTurnSpeed(Vector3 * vecTurnSpeed)
{
	memcpy(vecTurnSpeed, &m_vecTurnSpeed, sizeof(Vector3));
}

//----------------------------------------------------

float CPlayer::GetRotation()
{
	return m_fRotation;
}

//----------------------------------------------------

BYTE CPlayer::GetHealth()
{
	return m_byteHealth;
}

//----------------------------------------------------

BYTE CPlayer::GetArmour()
{
	return m_byteArmour;
}

//----------------------------------------------------

BYTE CPlayer::GetCurrentWeapon()
{
	return m_byteCurrentWeapon;
}

//----------------------------------------------------

BYTE CPlayer::GetAction()
{
	return m_byteShootingFlags;
}

//----------------------------------------------------

BOOL CPlayer::IsAPassenger()
{
	return m_bIsAPassenger;
}

//----------------------------------------------------

BYTE CPlayer::GetVehicleID()
{
	return m_vehicleID;
}

//----------------------------------------------------

void CPlayer::SetGameTime(BYTE hours, BYTE minutes)
{
	RakNet::BitStream bsTime;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	bsTime.Write(hours);
	bsTime.Write(minutes);

	pNetowkManager->GetRPC4()->Call("SetGameTime", &bsTime,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,false);
}

//----------------------------------------------------

void CPlayer::SetCameraPos(Vector3 vPos)
{
	RakNet::BitStream bsSend;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	bsSend.Write(vPos.X);
	bsSend.Write(vPos.Y);
	bsSend.Write(vPos.Z);

	pNetowkManager->GetRPC4()->Call("SetCameraPosition", &bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,false);
}

//----------------------------------------------------

void CPlayer::SetCameraRot(Vector3 vRot)
{
	RakNet::BitStream bsSend;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	bsSend.Write(vRot.X);
	bsSend.Write(vRot.Y);
	bsSend.Write(vRot.Z);

	pNetowkManager->GetRPC4()->Call("SetCameraRotation", &bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,false);
}

//----------------------------------------------------

void CPlayer::SetCameraLookAt(Vector3 vPoint)
{
	RakNet::BitStream bsSend;
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	bsSend.Write(vPoint.X);
	bsSend.Write(vPoint.Y);
	bsSend.Write(vPoint.Z);

	pNetowkManager->GetRPC4()->Call("SetCameraLookAt", &bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,false);
}

//----------------------------------------------------

void CPlayer::SetCameraBehindPlayer()
{
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);

	pNetowkManager->GetRPC4()->Call("SetCameraBehindPlayer", NULL,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,false);
}

//----------------------------------------------------

void CPlayer::SetCash(int Cash)
{
	BitStream bsSend;
	bsSend.Write(Cash);
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);
	pNetowkManager->GetRPC4()->Call("Script_SetPlayerCash",&bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,false);
	m_iMoney = Cash;
}

//----------------------------------------------------

int CPlayer::GetCash()
{
	return m_iMoney;
}

float CPlayer::GetGravity()
{
	return m_iGravity;
}

//----------------------------------------------------

void CPlayer::SetGravity(float amount)
{
	BitStream bsSend;
	bsSend.Write(amount);
	SystemAddress playerid = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(m_bytePlayerID);
	m_iGravity = amount;
	pNetowkManager->GetRPC4()->Call("Script_SetPlayerGravity",&bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,playerid,false);
}
