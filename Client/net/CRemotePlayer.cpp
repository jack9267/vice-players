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

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

using namespace RakNet;
extern CNetworkManager* pNetowkManager;

//----------------------------------------------------

CRemotePlayer::CRemotePlayer()
{
	m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
	m_playerID = INVALID_ENTITY_ID;
	m_bIsActive = FALSE;
	m_bIsWasted = FALSE;
	m_pPlayerPed = NULL;
	m_wKeys = 0;
	m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
	m_iJustSpawned = 0;
	m_fRotation = 0.0f;
	m_vehicleID = 0;
	m_bytePassengerSeat = 0;
	m_bHasAim = false;
}

//----------------------------------------------------

CRemotePlayer::~CRemotePlayer()
{
	if(m_pPlayerPed) {
		if(m_pPlayerPed->IsInVehicle()) {
			m_pPlayerPed->RemoveFromVehicleAndPutAt(0.0f,0.0f,0.0f);
		}
		m_pPlayerPed->SetHealth(0.0f);
		m_pPlayerPed->Destroy();
		delete m_pPlayerPed;
	}
}

//----------------------------------------------------

void CRemotePlayer::Process()
{
	CPlayerManager *pPool = pNetowkManager->GetPlayerManager();
	CVehicleManager *pVehicleManager = pNetowkManager->GetVehicleManager();
	CLocalPlayer *pLocalPlayer = pPool->GetLocalPlayer();

	if(m_bIsActive)
	{
		if(m_pPlayerPed != NULL)
		{
			// skip these frames if they've only just spawned.
			if(m_iJustSpawned != 0) {
				m_iJustSpawned--;
				return;
			}

			HandleVehicleEntryExit();

			if(m_byteUpdateFromNetwork == UPDATE_TYPE_FULL_ONFOOT)
			{					
				//m_pPlayerPed->CheckAndRepairInvisProblems();

				// Handle dead shit
				if(m_byteHealth == 0) {

					//pChatWindow->AddDebugMessage("%s has 0 health so killing\n",
						//pNetowkManager->GetPlayerManager()->GetAt(m_playerID));

					m_wKeys = 0;
					m_vehicleID = 0;
					m_pPlayerPed->SetKeys(m_wKeys);
					m_pPlayerPed->SetHealth(0.0f);
					m_bIsWasted = TRUE;
					m_bIsActive = FALSE;
					return;
				}

				// Update the ingame player.
				UpdateOnFootPosition(m_matWorld.vPos);

				if(m_bHasAim)
				{
					// Update aiming.
					m_pPlayerPed->SetCurrentAim(&m_Aim);
					m_bHasAim = false;
				}

				// update current weapon
				if(m_pPlayerPed->GetCurrentWeapon() != m_byteCurrentWeapon) {
					m_pPlayerPed->SetArmedWeapon(m_byteCurrentWeapon);

					// they don't have the new weapon, give it to them
					if(m_pPlayerPed->GetCurrentWeapon() != m_byteCurrentWeapon) {
						m_pPlayerPed->GiveWeapon(m_byteCurrentWeapon, 9999);
						m_pPlayerPed->SetArmedWeapon(m_byteCurrentWeapon);
					}
				}

				m_pPlayerPed->SetShootingFlags(m_byteShootingFlags);

				m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
			}
			else if(m_byteUpdateFromNetwork == UPDATE_TYPE_FULL_INCAR)
			{
				CVehicle *pVehicle = pVehicleManager->GetAt(m_vehicleID);
				
				if(pVehicle) {
					UpdateInCarMatrixAndSpeed(&m_matWorld, &m_vecMoveSpeed, &m_vecTurnSpeed);
					pVehicle->SetHealth(m_fVehicleHealth);
				}

				m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
			}

			m_pPlayerPed->SetKeys(m_wKeys);
			m_pPlayerPed->SetRotation(m_fRotation);
			m_pPlayerPed->SetHealth(100.0f);
			m_pPlayerPed->SetArmour(100.0f);
		}
	}
	else {
		m_vehicleID = 0; // make sure that's always reset.
	}
}

//----------------------------------------------------

void CRemotePlayer::HandleVehicleEntryExit()
{
	CVehicleManager *pVehicleManager = pNetowkManager->GetVehicleManager();

	if(m_vehicleID == 0 && m_pPlayerPed->IsInVehicle())
	{
		m_pPlayerPed->RemoveFromVehicleAndPutAt(m_matWorld.vPos.X,
			m_matWorld.vPos.Y,m_matWorld.vPos.Z);
	}	
	else if((m_vehicleID != 0) && !m_pPlayerPed->IsInVehicle())
	{
		// must force in
		CVehicle * pVehicle = pVehicleManager->GetAt(m_vehicleID);

		if(pVehicle && pVehicle->GetHealth() > 0.0f) {
			if(!m_bIsAPassenger) {
				m_pPlayerPed->PutDirectlyInVehicle(pVehicleManager->FindGtaIDFromID(m_vehicleID));
			} else {
				m_pPlayerPed->ForceIntoPassengerSeat(pVehicleManager->FindGtaIDFromID(m_vehicleID),m_bytePassengerSeat);
			}
		}
	}
}

//----------------------------------------------------

void CRemotePlayer::UpdateOnFootPosition(Vector3 vPos)
{
	m_pPlayerPed->SetPosition(vPos);
}

//----------------------------------------------------

void CRemotePlayer::StoreOnFootFullSyncData(PLAYER_SYNC_DATA * pPlayerSyncData)
{
	m_vehicleID = 0;
	m_wKeys = pPlayerSyncData->wKeys;
	memcpy(&m_matWorld.vPos, &pPlayerSyncData->vecPos, sizeof(Vector3));
	m_fRotation = pPlayerSyncData->fRotation;
	m_byteCurrentWeapon = pPlayerSyncData->byteCurrentWeapon;
	m_byteShootingFlags = pPlayerSyncData->byteShootingFlags;
	m_byteHealth = pPlayerSyncData->byteHealth;
	m_byteArmour = pPlayerSyncData->byteArmour;
	m_byteUpdateFromNetwork = UPDATE_TYPE_FULL_ONFOOT;
}

//----------------------------------------------------

void CRemotePlayer::StoreAimSyncData(CAMERA_AIM * pAim)
{ 
	m_bHasAim = true;
	memcpy(&m_Aim, pAim, sizeof(CAMERA_AIM));
};

//----------------------------------------------------

void CRemotePlayer::UpdateInCarMatrixAndSpeed(MATRIX4X4 * matWorld, Vector3 * vecMoveSpeed, Vector3 * vecTurnSpeed)
{
	MATRIX4X4 matVehicle;
	CVehicle * pVehicle = pNetowkManager->GetVehicleManager()->GetAt(m_vehicleID);
	float fDif;

	if(pVehicle) {
		pVehicle->GetMatrix(&matVehicle);

		memcpy(&matVehicle.vLookRight, &matWorld->vLookRight, sizeof(Vector3));
		memcpy(&matVehicle.vLookUp, &matWorld->vLookUp, sizeof(Vector3));

		if(matWorld->vPos.X >= matVehicle.vPos.X) {
			fDif = matWorld->vPos.X - matVehicle.vPos.X;
		} else {
			fDif = matVehicle.vPos.X - matWorld->vPos.X;
		}

		if(fDif > 0.1f) {		
			matVehicle.vPos.X = matWorld->vPos.X;
		}
		
		if(matWorld->vPos.Y >= matVehicle.vPos.Y) {
			fDif = matWorld->vPos.Y - matVehicle.vPos.Y;
		} else {
			fDif = matVehicle.vPos.Y - matWorld->vPos.Y;
		}

		if(fDif > 0.1f) {		
			matVehicle.vPos.Y = matWorld->vPos.Y;
		}
		
		if(matWorld->vPos.Z >= matVehicle.vPos.Z) {
			fDif = matWorld->vPos.Z - matVehicle.vPos.Z;
		} else {
			fDif = matVehicle.vPos.Z - matWorld->vPos.Z;
		}

		if(fDif > 0.1f) {		
			matVehicle.vPos.Z = matWorld->vPos.Z;
		}

		pVehicle->SetMatrix(matVehicle);
		pVehicle->SetMoveSpeed(*vecMoveSpeed);
		pVehicle->SetTurnSpeed(*vecTurnSpeed);
	}
}

//----------------------------------------------------

void CRemotePlayer::StoreInCarFullSyncData(VEHICLE_SYNC_DATA * pVehicleSyncData)
{
	m_vehicleID = pVehicleSyncData->vehicleID;
	m_wKeys = pVehicleSyncData->wKeys;
	memcpy(&m_matWorld.vLookRight, &pVehicleSyncData->vecRoll, sizeof(Vector3));
	memcpy(&m_matWorld.vLookUp, &pVehicleSyncData->vecDirection, sizeof(Vector3));
	memcpy(&m_matWorld.vPos, &pVehicleSyncData->vecPos, sizeof(Vector3));
	memcpy(&m_vecMoveSpeed, &pVehicleSyncData->vecMoveSpeed, sizeof(Vector3));
	memcpy(&m_vecTurnSpeed, &pVehicleSyncData->vecTurnSpeed, sizeof(Vector3));
	m_fVehicleHealth = UNPACK_VEHICLE_HEALTH(pVehicleSyncData->byteVehicleHealth);
	m_byteHealth = pVehicleSyncData->bytePlayerHealth;
	m_byteArmour = pVehicleSyncData->bytePlayerArmour;
	m_bIsInVehicle = TRUE;
	m_bIsAPassenger = FALSE;
	m_byteUpdateFromNetwork = UPDATE_TYPE_FULL_INCAR;
}

//----------------------------------------------------

void CRemotePlayer::StorePassengerData(EntityId vehicleID, BYTE byteSeat)
{
	m_bIsInVehicle = TRUE;
	m_bIsAPassenger = TRUE;
	m_vehicleID = vehicleID;
	m_bytePassengerSeat = byteSeat;
}

//----------------------------------------------------

BOOL CRemotePlayer::SpawnPlayer( BYTE byteTeam, BYTE byteSkin, 
								 Vector3 * vecPos, float fRotation, int iSpawnWeapon1,
								 int iSpawnWeapon1Ammo, int iSpawnWeapon2, 
								 int iSpawnWeapon2Ammo, int iSpawnWeapon3,
								 int iSpawnWeapon3Ammo )
{
	if(m_pPlayerPed != NULL) {
		m_pPlayerPed->Destroy();
		delete m_pPlayerPed;
	}

	CPlayerPed *pGamePlayer = pGame->NewPlayer(m_playerID+2,byteSkin,vecPos->X,
		vecPos->Y,vecPos->Z,fRotation);
	
	if(pGamePlayer) 
	{
		if(pNetowkManager->m_byteShowOnRadar) {
			pGamePlayer->ShowMarker(byteTeam);
		}

		m_pPlayerPed = pGamePlayer;
		m_bIsActive = TRUE;
		m_iJustSpawned = 30;
		m_vehicleID = 0;
		m_fRotation = fRotation;
		m_byteTeam = byteTeam;
		m_byteHealth = 100;

		if(iSpawnWeapon3 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon3,9999);
		}
		if(iSpawnWeapon2 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon2,9999);
		}		
		if(iSpawnWeapon1 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon1,9999);
		}

		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------

void CRemotePlayer::HandleDeath(BYTE byteReason, BYTE byteWhoKilled, BYTE byteScoringModifier)
{

	char * szPlayerName = pNetowkManager->GetPlayerManager()->GetPlayerName(m_playerID);
	char * szWhoKilledName;

	if(byteWhoKilled != INVALID_ENTITY_ID) {
		szWhoKilledName = pNetowkManager->GetPlayerManager()->GetPlayerName(byteWhoKilled);
	}

	switch(byteScoringModifier) {

		case VALID_KILL:
			pChatWindow->AddInfoMessage(">> %s was killed by %s",szPlayerName,szWhoKilledName);
			break;

		case TEAM_KILL:
			pChatWindow->AddInfoMessage(">> %s killed team-mate %s",szWhoKilledName,szPlayerName);
			break;

		case SELF_KILL:
			if(byteReason == WEAPON_DROWN) {
				pChatWindow->AddInfoMessage(">> %s drowned",szPlayerName);
			}
			else {
				pChatWindow->AddInfoMessage(">> %s died",szPlayerName);
			}
			break;
	}

	if(m_vehicleID != 0) {
		m_bIsWasted = TRUE;
		m_bIsActive = FALSE;
		m_vehicleID = 0;
	}
}

//----------------------------------------------------

void CRemotePlayer::Say(char *szText)
{
	char * szPlayerName = pNetowkManager->GetPlayerManager()->GetPlayerName(m_playerID);
	pChatWindow->AddChatMessage(szPlayerName,GetTeamColorAsARGB(),szText);
}


//----------------------------------------------------

float CRemotePlayer::GetDistanceFromRemotePlayer(CRemotePlayer *pFromPlayer)
{
	Vector3 vecThisPlayer;
	Vector3 vecFromPlayer;
	float  fSX,fSY;

	if(!pFromPlayer->IsActive()) return 10000.0f; // very far away
	if(!m_pPlayerPed) return 10000.0f; // very far away
	
	m_pPlayerPed->GetPosition(&vecThisPlayer);
	pFromPlayer->GetPlayerPed()->GetPosition(&vecFromPlayer);
	
	fSX = (vecThisPlayer.X - vecFromPlayer.X) * (vecThisPlayer.X - vecFromPlayer.X);
	fSY = (vecThisPlayer.Y - vecFromPlayer.Y) * (vecThisPlayer.Y - vecFromPlayer.Y);
	
	return (float)sqrt(fSX + fSY);	
}

//----------------------------------------------------

float CRemotePlayer::GetDistanceFromLocalPlayer()
{
	Vector3 vecThisPlayer;
	Vector3 vecFromPlayer;
	float  fSX,fSY;

	CLocalPlayer *pLocalPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();
	CPlayerPed *pLocalPlayerPed = pLocalPlayer->GetPlayerPed();

	if(!m_pPlayerPed) return 10000.0f; // very far away
	if(!pLocalPlayerPed) return 10000.0f; // very far away
	
	m_pPlayerPed->GetPosition(&vecThisPlayer);
	pLocalPlayerPed->GetPosition(&vecFromPlayer);
	
	fSX = (vecThisPlayer.X - vecFromPlayer.X) * (vecThisPlayer.X - vecFromPlayer.X);
	fSY = (vecThisPlayer.Y - vecFromPlayer.Y) * (vecThisPlayer.Y - vecFromPlayer.Y);
	
	return (float)sqrt(fSX + fSY);	
}

//----------------------------------------------------

DWORD CRemotePlayer::GetTeamColorAsRGBA()
{
	return TranslateColorCodeToRGBA(m_byteTeam);
}

//----------------------------------------------------

DWORD CRemotePlayer::GetTeamColorAsARGB()
{
	return (TranslateColorCodeToRGBA(m_byteTeam) >> 8) | 0xFF000000;	
}

//----------------------------------------------------

extern bool bDamageDisabled;

void CRemotePlayer::InflictDamage(bool bPlayerVehicleDamager, EntityId damagerID, int iWeapon, float fUnk, int iPedPieces, BYTE byteUnk)
{
	DWORD dwEntity;

	if(bPlayerVehicleDamager)
	{
		dwEntity = (DWORD)pNetowkManager->GetPlayerManager()->GetAt(damagerID)->GetPlayerPed()->GetEntity();		
	}
	else
	{
		dwEntity = (DWORD)pNetowkManager->GetVehicleManager()->GetAt(damagerID)->GetVehicle();
	}

	// Enable CPed::InflictDamage for remote players
	bDamageDisabled = false;

	// TODO: Move to CPlayerPed
	DWORD dwFunc = FUNC_CPed__InflictDamage;
	_asm
	{
		push byteUnk
		push iPedPieces
		push fUnk
		push iWeapon
		push dwEntity
		call dwFunc
		add esp, 10h
	}

	// Disable CPed::InflictDamage for remote players
	bDamageDisabled = true;
}

//----------------------------------------------------
