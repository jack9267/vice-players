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
extern CPlayerPed	 *pLocalPlayer;
extern CScripts		 *pScripts;

using namespace RakNet;
extern CNetworkManager* pNetowkManager;

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerJoin(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	CHAR szPlayerName[MAX_PLAYER_NAME];
	EntityId playerID;
	UINT uiNameLength;

	memset(szPlayerName,0,MAX_PLAYER_NAME);

	bitStream->Read(playerID);
	bitStream->Read(uiNameLength);
	bitStream->Read(szPlayerName,uiNameLength);
	szPlayerName[uiNameLength] = '\0';

	// Add this client to the player pool.
	pPlayerManager->New(playerID, szPlayerName);
}

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerQuit(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	EntityId playerID;
	BYTE byteReason;

	bitStream->Read(playerID);
	bitStream->Read(byteReason);

	// Delete this client from the player pool.
	pPlayerManager->Delete(playerID,byteReason);
}


//----------------------------------------------------
// Server is giving us basic init information.

void InitGame(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	EntityId myPlayerID;

	bitStream->Read((char *)&pNetowkManager->m_vecInitPlayerPos, sizeof(Vector3));
	bitStream->Read((char *)&pNetowkManager->m_vecInitCameraPos, sizeof(Vector3));
	bitStream->Read((char *)&pNetowkManager->m_vecInitCameraLook, sizeof(Vector3));
	bitStream->Read(pNetowkManager->m_WorldBounds[0]);
	bitStream->Read(pNetowkManager->m_WorldBounds[1]);
	bitStream->Read(pNetowkManager->m_WorldBounds[2]);
	bitStream->Read(pNetowkManager->m_WorldBounds[3]);
	bitStream->Read(pNetowkManager->m_byteFriendlyFire);
	bitStream->Read(pNetowkManager->m_byteShowOnRadar);
	bitStream->Read(myPlayerID);

	pPlayerManager->SetLocalPlayerID(myPlayerID);

	pGame->FadeScreen(1, 0);
	pNetowkManager->InitGameLogic();
	pNetowkManager->SetGameState(GAMESTATE_CONNECTED);
}

//----------------------------------------------------
// Remote player has sent a chat message.

void Chat(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID;
	BYTE byteTextLen;
	CHAR szText[256];

	bitStream->Read(playerID);
	bitStream->Read(byteTextLen);
	bitStream->Read(szText,byteTextLen);

	szText[byteTextLen] = '\0';

	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	if(playerID == pPlayerManager->GetLocalPlayerID()) {
		pChatWindow->AddChatMessage(pNetowkManager->GetPlayerManager()->GetLocalPlayerName(),
			pPlayerManager->GetLocalPlayer()->GetTeamColorAsARGB(),szText);
	}
	else
	{
		CRemotePlayer *pRemotePlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
		if(pRemotePlayer) {
			pRemotePlayer->Say(szText);	
		}
	}
}

//----------------------------------------------------
// This should be rewritten as a packet instead of
// an RPC.

void Passenger(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID;
	EntityId vehicleID;
	BYTE byteSeat;

	bitStream->Read(playerID);
	bitStream->Read(vehicleID);
	bitStream->Read(byteSeat);
	
	CRemotePlayer * pRemotePlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);

	if(pRemotePlayer) {
		pRemotePlayer->StorePassengerData(vehicleID, byteSeat);
	}	
}

//----------------------------------------------------
// Reply to our class request from the server.

void RequestClass(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteOutcome;
	int iRequestedClass;
	PLAYER_SPAWN_INFO SpawnInfo;
	CLocalPlayer * pPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();
	CSpawnSelection * pGameLogic = pNetowkManager->GetGameLogic();

	bitStream->Read(byteOutcome);
	if(byteOutcome) {
		bitStream->Read(iRequestedClass);
		bitStream->Read((char *)&SpawnInfo, sizeof(PLAYER_SPAWN_INFO));

		pGameLogic->SetSelectedClass(iRequestedClass);
		pPlayer->SetSpawnInfo(&SpawnInfo);
		pGameLogic->HandleClassSelectionOutcome(&SpawnInfo, byteOutcome);
	}
}

//----------------------------------------------------
// Remote client is spawning.

void Spawn(RakNet::BitStream *bitStream, Packet *packet)
{
	CRemotePlayer *pRemotePlayer;

	EntityId playerID=0;
	BYTE byteTeam=0;
	BYTE byteSkin=0;
	Vector3 vecPos;
	float fRotation=0;
	BYTE byteOnGround=0;
	int iSpawnWeapons1,iSpawnWeapons2,iSpawnWeapons3;
	int iSpawnWeaponsAmmo1,iSpawnWeaponsAmmo2,iSpawnWeaponsAmmo3;

	bitStream->Read(playerID);
	bitStream->Read(byteTeam);
	bitStream->Read(byteSkin);
	bitStream->Read(vecPos.X);
	bitStream->Read(vecPos.Y);
	bitStream->Read(vecPos.Z);
	bitStream->Read(fRotation);
	bitStream->Read(iSpawnWeapons1);
	bitStream->Read(iSpawnWeaponsAmmo1);
	bitStream->Read(iSpawnWeapons2);
	bitStream->Read(iSpawnWeaponsAmmo2);
	bitStream->Read(iSpawnWeapons3);
	bitStream->Read(iSpawnWeaponsAmmo3);

	pRemotePlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);

	if(pRemotePlayer) {
		pRemotePlayer->SpawnPlayer(byteTeam,byteSkin,&vecPos,fRotation,
			iSpawnWeapons1,iSpawnWeaponsAmmo1,
			iSpawnWeapons2,iSpawnWeaponsAmmo2,
			iSpawnWeapons3,iSpawnWeaponsAmmo3);
	}		
}

//----------------------------------------------------
// Remote client is dead.

void Death(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID=0;
	BYTE byteReason;
	BYTE byteWhoKilled;
	BYTE byteScoringModifier;

	bitStream->Read(playerID);
	bitStream->Read(byteReason);
	bitStream->Read(byteWhoKilled);
	bitStream->Read(byteScoringModifier);

	CRemotePlayer *pRemotePlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
	if(pRemotePlayer) {
		pRemotePlayer->HandleDeath(byteReason,byteWhoKilled,byteScoringModifier);
	}
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void EnterVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID=0;
	EntityId vehicleID=0;
	BYTE bytePassenger=0;

	bitStream->Read(playerID);
	bitStream->Read(vehicleID);
	bitStream->Read(bytePassenger);

	CRemotePlayer *pRemotePlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
	CVehicleManager *pVehicleManager = pNetowkManager->GetVehicleManager();

	if(pRemotePlayer) {
		if(!bytePassenger) {
			pRemotePlayer->GetPlayerPed()->EnterVehicleAsDriver(pVehicleManager->FindGtaIDFromID(vehicleID));
		} else {
			pRemotePlayer->GetPlayerPed()->EnterVehicleAsPassenger(pVehicleManager->FindGtaIDFromID(vehicleID));
		}
	}
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void ExitVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID=0;
	EntityId vehicleID=0;

	bitStream->Read(playerID);
	bitStream->Read(vehicleID);

	CRemotePlayer *pRemotePlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
	CVehicleManager *pVehicleManager = pNetowkManager->GetVehicleManager();

	if(pRemotePlayer) {
		pRemotePlayer->GetPlayerPed()->ExitCurrentVehicle();
	}
}

//----------------------------------------------------

void VehicleSpawn(RakNet::BitStream *bitStream, Packet *packet)
{
	CVehicleManager *pVehicleManager = pNetowkManager->GetVehicleManager();
	EntityId vehicleID=0;
	BYTE byteVehicleType;
	Vector3 vecPos;
	Vector3 vecSpawnPos;
	float fRotation;
	float fSpawnRotation;
	float fHealth;
	int iColor1, iColor2;

	bitStream->Read(vehicleID);
	bitStream->Read(byteVehicleType);
	bitStream->Read(vecPos.X);
	bitStream->Read(vecPos.Y);
	bitStream->Read(vecPos.Z);
	bitStream->Read(fRotation);
	bitStream->Read(iColor1);
	bitStream->Read(iColor2);
	bitStream->Read(fHealth);
	bitStream->Read(vecSpawnPos.X);
	bitStream->Read(vecSpawnPos.Y);
	bitStream->Read(vecSpawnPos.Z);
	bitStream->Read(fSpawnRotation);

	pVehicleManager->New(vehicleID,byteVehicleType,
		&vecPos,fRotation,iColor1,iColor2,&vecSpawnPos,fSpawnRotation);
	//pVehicleManager->GetAt(vehicleID)->SetHealth(fHealth);
}

//----------------------------------------------------

void VehicleDestroy(RakNet::BitStream *bitStream, Packet *packet)
{
	CVehicleManager *pVehicleManager = pNetowkManager->GetVehicleManager();
	EntityId vehicleID=0;

	bitStream->Read(vehicleID);

	pVehicleManager->Delete(vehicleID);
}

//----------------------------------------------------

void UpdateScoreAndPing(RakNet::BitStream *bitStream, Packet *packet)
{	
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	EntityId playerID;
	int iPlayerScore;
	int iPlayerPing;
	unsigned long ulIp;

	while(bitStream->Read(playerID))
	{
		bitStream->Read(iPlayerScore);
		bitStream->Read(iPlayerPing);
		bitStream->Read(ulIp);

		if(pPlayerManager->GetSlotState(playerID) || playerID == pPlayerManager->GetLocalPlayerID())
		{
			pPlayerManager->UpdateScore(playerID, iPlayerScore);
			pPlayerManager->UpdatePing(playerID, iPlayerPing);
			pPlayerManager->UpdateIPAddress(playerID, ulIp);
		}
	}
}

//----------------------------------------------------

void ConnectionRejected(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteRejectReason;

	bitStream->Read(byteRejectReason);

	if(byteRejectReason==REJECT_REASON_BAD_VERSION) {
		pChatWindow->AddInfoMessage("CONNECTION REJECTED");
		pChatWindow->AddInfoMessage("YOU'RE USING AN INCORRECT VERSION!");
	}
	else if(byteRejectReason==REJECT_REASON_BAD_NICKNAME)
	{
		pChatWindow->AddInfoMessage("CONNECTION REJECTED");
		pChatWindow->AddInfoMessage("YOUR NICKNAME IS INVALID");
	}

	if(pNetowkManager) {
		pNetowkManager->Shutdown();
	}
}

//----------------------------------------------------

void SetGameTime(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE h, m;
	bitStream->Read(h);
	bitStream->Read(m);

	pGame->SetGameTime(h, m);
}

//----------------------------------------------------

void SetCameraPosition(RakNet::BitStream *bitStream, Packet *packet)
{
	Vector3 vPos;
	bitStream->Read(vPos.X);
	bitStream->Read(vPos.Y);
	bitStream->Read(vPos.Z);

	pGame->GetCamera()->SetPosition(vPos);
}

//----------------------------------------------------

void SetCameraRotation(RakNet::BitStream *bitStream, Packet *packet)
{
	Vector3 vRot;
	bitStream->Read(vRot.X);
	bitStream->Read(vRot.Y);
	bitStream->Read(vRot.Z);

	pGame->GetCamera()->SetRotation(vRot);
}

//----------------------------------------------------

void SetCameraLookAt(RakNet::BitStream *bitStream, Packet *packet)
{
	Vector3 vPoint;
	bitStream->Read(vPoint.X);
	bitStream->Read(vPoint.Y);
	bitStream->Read(vPoint.Z);

	pGame->GetCamera()->LookAtPoint(vPoint, 1);
}

//----------------------------------------------------

void SetCameraBehindPlayer(RakNet::BitStream *bitStream, Packet *packet)
{
	pGame->GetCamera()->SetBehindPlayer();
}

//----------------------------------------------------

void UploadClientScript(RakNet::BitStream *bitStream, Packet *packet)
{
	long uiLengthName;
	long uiLength;
	long uiScriptLength;
	char szScriptName[256];
	std::string str = "vc-p/clientscripts/";

	bitStream->Read(uiLengthName);
	bitStream->Read(uiLength);
	bitStream->Read(uiScriptLength);

	char *szScript = new char[uiLength+1];
	bitStream->Read(szScriptName, uiLengthName);
	bitStream->Read(szScript, uiLength);

	szScriptName[uiLengthName] = '\0';
	szScript[uiLength] = '\0';
	str.append(szScriptName);

	FILE *f = fopen(str.c_str(), "wb");
	if(f)
	{
		fprintf(f, "%s", szScript);
		rewind(f);
		fseek(f, 0, SEEK_END);
		long length = ftell(f);
		rewind(f);
		fclose(f);
		if(length >= uiScriptLength)
		{
			int iSlot = pScripts->LoadScript(szScriptName);
			pScripts->onInit(iSlot);
		}
	}
}

void ObjectSpawn(RakNet::BitStream *bitStream, Packet *packet)
{
	CObjectManager *pObjectManager = pNetowkManager->GetObjectManager();
	EntityId ObjectID=0;
	int iModel;
	Vector3 vecPos, vecRot;
	bitStream->Read(ObjectID);
	bitStream->Read(iModel);
	bitStream->Read(vecPos);
	bitStream->Read(vecRot);

	pObjectManager->New(ObjectID, iModel, vecPos, vecRot);
}

void ObjectDestroy(RakNet::BitStream *bitStream, Packet *packet)
{
	CObjectManager *pObjectManager = pNetowkManager->GetObjectManager();
	EntityId objId=0;

	bitStream->Read(objId);

	pObjectManager->Delete(objId);
}

void PickupSpawn(RakNet::BitStream *bitStream, Packet *packet)
{
	CPickupManager *pPickupManager = pNetowkManager->GetPickupManager();
	EntityId PickupID=0;
	int iModel, iType;
	Vector3 vecPos;
	bitStream->Read(PickupID);
	bitStream->Read(iModel);
	bitStream->Read(iType);
	bitStream->Read((char*)&vecPos, sizeof(Vector3));

	pPickupManager->New(PickupID, iModel, iType, &vecPos);
}

void PickupDestroy(RakNet::BitStream *bitStream, Packet *packet)
{
	CPickupManager *pPickupManager = pNetowkManager->GetPickupManager();
	EntityId PickupID=0;

	bitStream->Read(PickupID);

	pPickupManager->Delete(PickupID);
}
//----------------------------------------------------

// ============= Scripting RPC's ====================//

// SetHealth
void Script_SetHealth(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	float playerHealth;

	bitStream->Read(playerHealth);

	pPlayer->SetHealth(playerHealth);
}

// SetVehicleHealth
void Script_SetVehicleHealth(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	float newHealth;
	bitStream->Read(vehicle);
	bitStream->Read(newHealth);
	
	CVehicle *pVehicle = pNetowkManager->GetVehicleManager()->GetAt(vehicle);
	pVehicle->SetHealth(newHealth);
}

// SetArmour
void Script_SetArmour(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	float playerArmour;

	bitStream->Read(playerArmour);

	pPlayer->SetArmour(playerArmour);
}

// SetPlayerPos
void Script_SetPos(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	Vector3 playerPos;

	bitStream->Read((char *)&playerPos, sizeof(Vector3));

	pPlayer->Teleport(playerPos.X, playerPos.Y, playerPos.Z);
}

// SetPlayerTurnSpeed
void Script_SetTurnSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	Vector3 speed;

	bitStream->Read((char *)&speed, sizeof(Vector3));

	pPlayer->SetTurnSpeed(speed);
}

// SetPlayerTurnSpeed
void Script_SetMoveSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	Vector3 speed;

	bitStream->Read((char *)&speed, sizeof(Vector3));

	pPlayer->SetMoveSpeed(speed);
}

// PutPlayerInVehicle
void Script_PutInVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int vehID;

	bitStream->Read(vehID);

	pPlayer->PutDirectlyInVehicle(pNetowkManager->GetVehicleManager()->FindGtaIDFromID(vehID));
}

// GiveWeapon
void Script_GivePlayerWeapon(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int weaponID;
	int ammoAMT;

	bitStream->Read(weaponID);
	bitStream->Read(ammoAMT);

	pPlayer->GiveWeapon(weaponID, ammoAMT);
}

// SetSkin
void Script_SetPlayerSkin(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int skinID;

	bitStream->Read(skinID);

	pPlayer->SetModel(skinID);
}

// SetZAngle
void Script_SetPlayerZAngle(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	float fRotation;

	bitStream->Read(fRotation);
	pPlayer->SetRotation(fRotation);
}

// setAction
void Script_SetPlayerAction(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	BYTE Action;

	bitStream->Read(Action);
	pPlayer->SetAction(Action);
}

// setPlayerRotation
void Script_SetPlayerRotation(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	float playerRot;

	bitStream->Read(playerRot);
	pPlayer->SetRotation(playerRot);
}

// resetPlayerWeapons
void Script_ResetWeapons(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	pPlayer->ClearAllWeapons();
}

// setArmedWeapon
void Script_SetArmedWeapon(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int playerWeapon;

	bitStream->Read(playerWeapon);

	pPlayer->SetArmedWeapon(playerWeapon);
}

// removePlayerFromVehicle
void Script_RemoveFromVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	pPlayer->ExitCurrentVehicle();
}

// togglecontrols
void Script_ToggleControls(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int ControlValue;

	bitStream->Read(ControlValue);

	pPlayer->TogglePlayerControllable(ControlValue);
}

// send message to client
void Script_ClientMessage(RakNet::BitStream *bitStream, Packet *packet)
{
	DWORD dwColor;
	CHAR szMessage[256];
	UINT uiLength;

	bitStream->Read(dwColor);
	bitStream->Read(uiLength);
	bitStream->Read(szMessage,uiLength);
	szMessage[uiLength] = '\0';

	pChatWindow->AddClientMessage(dwColor,szMessage);
}

// createText
void CreateText(RakNet::BitStream *bitStream, Packet *packet)
{
	CTextManager *pTextManager = pNetowkManager->GetTextManager();
	EntityId TextID=0;
	DWORD dwColor;
	CHAR szFontName[64], szMessage[256];
	UINT uiFLength, uiLength;
	int size;
	float posX, posY;

	bitStream->Read(TextID);
	bitStream->Read(dwColor);
	bitStream->Read(uiFLength);
	bitStream->Read(szFontName,uiFLength);
	szFontName[uiFLength] = '\0';
	bitStream->Read(size);
	bitStream->Read(posX);
	bitStream->Read(posY);
	bitStream->Read(uiLength);
	bitStream->Read(szMessage,uiLength);
	szMessage[uiLength] = '\0';

	pTextManager->New(TextID, dwColor, szFontName, size, posX, posY, szMessage);
}

void DestroyText(RakNet::BitStream *bitStream, Packet *packet)
{
	CTextManager *pTextManager = pNetowkManager->GetTextManager();
	EntityId text;

	bitStream->Read(text);

	pTextManager->Delete(text);
}

//----------------------------------------------------
// Remote client has had damage inflicted upon them

void InflictDamage(RakNet::BitStream * bitStream, Packet * packet)
{
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	CVehicleManager * pVehicleManager = pNetowkManager->GetVehicleManager();
	EntityId playerID;
	bool bPlayerVehicleDamager;
	EntityId damagerID;
	int iWeapon;
	float fUnk;
	int iPedPieces;
	BYTE byteUnk;

	bitStream->Read(playerID);

	if(!pVehicleManager->GetAt(playerID))
	{
		return;
	}

	bPlayerVehicleDamager = bitStream->ReadBit();
	bitStream->Read(damagerID);

	if((bPlayerVehicleDamager && !pPlayerManager->GetSlotState(damagerID)) || (!bPlayerVehicleDamager && !pVehicleManager->GetSlotState(damagerID)))
	{
		return;
	}

	bitStream->Read(iWeapon);
	bitStream->Read(fUnk);
	bitStream->Read(iPedPieces);
	bitStream->Read(byteUnk);

	CRemotePlayer * pPlayer = pPlayerManager->GetAt(playerID);

	if(pPlayer)
	{
		pPlayer->InflictDamage(bPlayerVehicleDamager, damagerID, iWeapon, fUnk, iPedPieces, byteUnk);
	}
}

//----------------------------------------------------

// Script_toggleTextForPlayer
void Script_toggleTextForPlayer(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId textId;
	bool show;

	bitStream->Read(textId);
	bitStream->Read(show);

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->Show(show);
	}
}

void Script_SetText(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId textId;
	int len;

	bitStream->Read(textId);
	bitStream->Read(len);
	char * szText = new char[len+1];
	bitStream->Read(szText, len);
	szText[len] = '\0';

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->SetText(szText);
	}
}

void Script_SetTextPosition(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId textId;
	float fPosX, fPosY;

	bitStream->Read(textId);
	bitStream->Read(fPosX);
	bitStream->Read(fPosY);

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->SetPosition(fPosX, fPosY);
	}
}

void Script_SetTextColor(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId textId;
	DWORD color;

	bitStream->Read(textId);
	bitStream->Read(color);

	if(pNetowkManager->GetTextManager()->GetSlotState(textId))
	{
		pNetowkManager->GetTextManager()->GetAt(textId)->SetColor(color);
	}
}

// setworldbounds
void Script_WorldBounds(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	float LowX;
	float LowY;
	float HighX;
	float HighY;

	bitStream->Read(LowX);
	bitStream->Read(LowY);
	bitStream->Read(HighX);
	bitStream->Read(HighY);

	pPlayer->EnforceWorldBoundries(HighX, LowX, HighY, LowY);
}

// SetVehicleColor
void Script_SetVehicleColor(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	int color1, color2;
	bitStream->Read(vehicle);
	bitStream->Read(color1);
	bitStream->Read(color2);
	
	CVehicle *pVehicle = pNetowkManager->GetVehicleManager()->GetAt(vehicle);
	pVehicle->SetColor(color1, color2);
}

// SetVehiclePos
void Script_SetVehiclePos(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	Vector3 pos;
	bitStream->Read(vehicle);
	bitStream->Read((char *)&pos, sizeof(Vector3));

	CVehicleManager *pPool = pNetowkManager->GetVehicleManager();
	if(pPool->GetSlotState(vehicle))
	{
		pPool->GetAt(vehicle)->SetPosition(pos);
	}
}

// SetVehicleTurnSpeed
void Script_SetVehicleTurnSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	Vector3 speed;
	bitStream->Read(vehicle);
	bitStream->Read((char *)&speed, sizeof(Vector3));

	CVehicleManager *pPool = pNetowkManager->GetVehicleManager();
	if(pPool->GetSlotState(vehicle))
	{
		pPool->GetAt(vehicle)->SetTurnSpeed(speed);
	}
}

// SetVehicleMoveSpeed
void Script_SetVehicleMoveSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	Vector3 speed;
	bitStream->Read(vehicle);
	bitStream->Read((char *)&speed, sizeof(Vector3));

	CVehicleManager *pPool = pNetowkManager->GetVehicleManager();
	if(pPool->GetSlotState(vehicle))
	{
		pPool->GetAt(vehicle)->SetMoveSpeed(speed);
	}
}

// SetVehicleColor
void Script_DestroyVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	bitStream->Read(vehicle);
	
	pNetowkManager->GetVehicleManager()->Delete(vehicle);
}

// Play sound
void Script_PlaySound(RakNet::BitStream *bitStream, Packet *packet)
{
	int sound;
	Vector3 vPos;

	bitStream->Read(sound);
	bitStream->Read(vPos.X);
	bitStream->Read(vPos.Y);
	bitStream->Read(vPos.Z);

	pGame->PlaySound(sound, vPos);
}

// Fade
void Script_FadeScreen(RakNet::BitStream *bitStream, Packet *packet)
{
	int type, time;

	bitStream->Read(type);
	bitStream->Read(time);

	pGame->FadeScreen(type, time);
}

// setItemFlashing Native.
void Script_FlashItem(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int item;

	bitStream->Read(item);

	pPlayer->Flash(item);
}

// forceClassSelection Native.
void Script_forceClassSelection(RakNet::BitStream *bitStream, Packet *packet)
{
	CLocalPlayer *pLocalPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();
	pNetowkManager->GetGameLogic()->HandleClassSelection(pLocalPlayer);
}

// togglePlayerBleeding Native.
void Script_togglePlayerBleeding(RakNet::BitStream *bitStream, Packet *packet)
{
	int player, toggle;

	bitStream->Read(player);
	bitStream->Read(toggle);

	if(player == pNetowkManager->GetPlayerManager()->GetLocalPlayerID())
	{
		CLocalPlayer * pPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();
		pPlayer->GetPlayerPed()->SetActorBleeding(toggle);
	} 
	else if(pNetowkManager->GetPlayerManager()->GetSlotState(player))
	{
		if(player != pNetowkManager->GetPlayerManager()->GetLocalPlayerID())
		{
			CRemotePlayer * pPlayer = pNetowkManager->GetPlayerManager()->GetAt(player);
			pPlayer->GetPlayerPed()->SetActorBleeding(toggle);
		}
	}
}

// popVehicleTrunk
void Script_popVehicleTrunk(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;

	bitStream->Read(vehicle);
	
	CVehicle *pVehicle = pNetowkManager->GetVehicleManager()->GetAt(vehicle);
	pVehicle->PopTrunk();
}

// setSkyColor
void Script_setSkyColor(RakNet::BitStream *bitStream, Packet *packet)
{
	int iColor, iFade;

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	bitStream->Read(iColor);
	bitStream->Read(iFade);

	pPlayer->SetSkyColor(iColor, iFade);
}

// SetPlayerCash
void Script_setPlayerCash(RakNet::BitStream *bitStream, Packet *packet)
{
	int Cash;
	bitStream->Read(Cash);
	pGame->SetCash(Cash);
}

// toggleDriveByState
void Script_toggleDriveByState(RakNet::BitStream *bitStream, Packet *packet)
{
	int iToggle;

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	bitStream->Read(iToggle);

	pPlayer->SetDrivebyState(iToggle);
}

void Script_ClientCall(RakNet::BitStream *bitStream, Packet *packet)
{
	pScripts->Call(bitStream);
}

// toggleCellPhone
void Script_toggleCellPhone(RakNet::BitStream *bitStream, Packet *packet)
{
	int iToggle;
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	bitStream->Read(iToggle);

	pPlayer->SetCellAction(iToggle);
}

// SetCameraShakeIntensity
void Script_SetCameraShakeIntensity(RakNet::BitStream *bitStream, Packet *packet)
{
	int iIntensity;

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	bitStream->Read(iIntensity);

	pPlayer->SetCameraShakeIntensity(iIntensity);
}

// SetPlayerGravity
void Script_setPlayerGravity(RakNet::BitStream *bitStream, Packet *packet)
{
	float amount;
	bitStream->Read(amount);
	pGame->SetGravity(amount);
}

void CreateCheckpoint(RakNet::BitStream *bitStream, Packet *packet)
{
	Vector3 vecPos;
	float fRadius;
	EntityId cpId;
	bitStream->Read(cpId);
	bitStream->Read((char*)&vecPos, sizeof(Vector3));
	bitStream->Read(fRadius);

	pNetowkManager->GetCheckpoints()->New(cpId, vecPos, 0, fRadius);
}

void DestroyCheckpoint(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId cpId;
	bitStream->Read(cpId);

	pNetowkManager->GetCheckpoints()->Delete(cpId);
}

void RegisterRPCs()
{
	pNetowkManager->GetRPC4()->RegisterFunction("ServerJoin",ServerJoin);
	pNetowkManager->GetRPC4()->RegisterFunction("ServerQuit",ServerQuit);	
	pNetowkManager->GetRPC4()->RegisterFunction("InitGame",InitGame);
	pNetowkManager->GetRPC4()->RegisterFunction("Chat",Chat);
	pNetowkManager->GetRPC4()->RegisterFunction("RequestClass",RequestClass);
	pNetowkManager->GetRPC4()->RegisterFunction("Spawn",Spawn);
	pNetowkManager->GetRPC4()->RegisterFunction("Death",Death);
	pNetowkManager->GetRPC4()->RegisterFunction("EnterVehicle",EnterVehicle);
	pNetowkManager->GetRPC4()->RegisterFunction("ExitVehicle",ExitVehicle);
	pNetowkManager->GetRPC4()->RegisterFunction("VehicleSpawn",VehicleSpawn);
	pNetowkManager->GetRPC4()->RegisterFunction("VehicleDestroy",VehicleDestroy);
	pNetowkManager->GetRPC4()->RegisterFunction("UpdateScoreAndPing",UpdateScoreAndPing);
	pNetowkManager->GetRPC4()->RegisterFunction("ConnectionRejected",ConnectionRejected);
	pNetowkManager->GetRPC4()->RegisterFunction("Passenger",Passenger);
	pNetowkManager->GetRPC4()->RegisterFunction("SetGameTime",SetGameTime);
	pNetowkManager->GetRPC4()->RegisterFunction("SetCameraPosition",SetCameraPosition);
	pNetowkManager->GetRPC4()->RegisterFunction("SetCameraRotation",SetCameraRotation);
	pNetowkManager->GetRPC4()->RegisterFunction("SetCameraLookAt",SetCameraLookAt);
	pNetowkManager->GetRPC4()->RegisterFunction("SetCameraBehindPlayer",SetCameraBehindPlayer);
	pNetowkManager->GetRPC4()->RegisterFunction("UploadClientScript",UploadClientScript);
	pNetowkManager->GetRPC4()->RegisterFunction("ObjectSpawn", ObjectSpawn);
	pNetowkManager->GetRPC4()->RegisterFunction("ObjectDestroy", ObjectDestroy);
	pNetowkManager->GetRPC4()->RegisterFunction("CreateText", CreateText);
	pNetowkManager->GetRPC4()->RegisterFunction("DestroyText", DestroyText);
	pNetowkManager->GetRPC4()->RegisterFunction("InflictDamage", InflictDamage);
	pNetowkManager->GetRPC4()->RegisterFunction("CreateCheckpoint", CreateCheckpoint);
	pNetowkManager->GetRPC4()->RegisterFunction("DestroyCheckpoint", DestroyCheckpoint);
	pNetowkManager->GetRPC4()->RegisterFunction("PickupSpawn", PickupSpawn);
	pNetowkManager->GetRPC4()->RegisterFunction("PickupDestroy", PickupDestroy);

	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetHealth",Script_SetHealth);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetArmour",Script_SetArmour);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetPos",Script_SetPos);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetTurnSpeed",Script_SetTurnSpeed);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetMoveSpeed",Script_SetMoveSpeed);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_PutInVehicle",Script_PutInVehicle);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_GivePlayerWeapon",Script_GivePlayerWeapon);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetPlayerSkin",Script_SetPlayerSkin);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetPlayerZAngle",Script_SetPlayerZAngle);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetPlayerAction",Script_SetPlayerAction);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetPlayerRotation",Script_SetPlayerRotation);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetArmedWeapon",Script_SetArmedWeapon);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_RemoveFromVehicle",Script_RemoveFromVehicle);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_ToggleControls",Script_ToggleControls);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_ClientMessage",Script_ClientMessage);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_WorldBounds",Script_WorldBounds);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetVehicleHealth",Script_SetVehicleHealth);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetVehicleColor",Script_SetVehicleColor);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetVehiclePos",Script_SetVehiclePos);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetVehicleTurnSpeed",Script_SetVehicleTurnSpeed);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetVehicleMoveSpeed",Script_SetVehicleMoveSpeed);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_DestroyVehicle",Script_DestroyVehicle);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_PlaySound",Script_PlaySound);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_FadeScreen",Script_FadeScreen);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_ClientCall", Script_ClientCall);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_forceClassSelection",Script_forceClassSelection);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_togglePlayerBleeding",Script_togglePlayerBleeding);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_FlashItem",Script_FlashItem);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_popVehicleTrunk", Script_popVehicleTrunk);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_setSkyColor", Script_setSkyColor);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetPlayerCash", Script_setPlayerCash);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_toggleDriveByState", Script_toggleDriveByState);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_toggleCellPhone", Script_toggleCellPhone);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetCameraShakeIntensity", Script_SetCameraShakeIntensity);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetPlayerGravity", Script_setPlayerGravity);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_toggleTextForPlayer", Script_toggleTextForPlayer);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetText", Script_SetText);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetTextPosition", Script_SetTextPosition);
	pNetowkManager->GetRPC4()->RegisterFunction("Script_SetTextColor", Script_SetTextColor);
}

//----------------------------------------------------

void UnRegisterRPCs()
{
	pNetowkManager->GetRPC4()->UnregisterFunction("ServerJoin");
	pNetowkManager->GetRPC4()->UnregisterFunction("ServerQuit");
	pNetowkManager->GetRPC4()->UnregisterFunction("InitGame");
	pNetowkManager->GetRPC4()->UnregisterFunction("Chat");
	pNetowkManager->GetRPC4()->UnregisterFunction("RequestClass");
	pNetowkManager->GetRPC4()->UnregisterFunction("Spawn");
	pNetowkManager->GetRPC4()->UnregisterFunction("Death");
	pNetowkManager->GetRPC4()->UnregisterFunction("EnterVehicle");
	pNetowkManager->GetRPC4()->UnregisterFunction("ExitVehicle");
	pNetowkManager->GetRPC4()->UnregisterFunction("VehicleSpawn");
	pNetowkManager->GetRPC4()->UnregisterFunction("VehicleDestroy");
	pNetowkManager->GetRPC4()->UnregisterFunction("UpdateScoreAndPing");
	pNetowkManager->GetRPC4()->UnregisterFunction("ConnectionRejected");
	pNetowkManager->GetRPC4()->UnregisterFunction("Passenger");
	pNetowkManager->GetRPC4()->UnregisterFunction("SetGameTime");
	pNetowkManager->GetRPC4()->UnregisterFunction("SetCameraPosition");
	pNetowkManager->GetRPC4()->UnregisterFunction("SetCameraRotation");
	pNetowkManager->GetRPC4()->UnregisterFunction("SetCameraLookAt");
	pNetowkManager->GetRPC4()->UnregisterFunction("SetCameraBehindPlayer");
	pNetowkManager->GetRPC4()->UnregisterFunction("UploadClientScript");
	pNetowkManager->GetRPC4()->UnregisterFunction("ObjectSpawn");
	pNetowkManager->GetRPC4()->UnregisterFunction("ObjectDestroy");
	pNetowkManager->GetRPC4()->UnregisterFunction("CreateText");
	pNetowkManager->GetRPC4()->UnregisterFunction("DestroyText");
	pNetowkManager->GetRPC4()->UnregisterFunction("InflictDamage");
	pNetowkManager->GetRPC4()->UnregisterFunction("CreateCheckpoint");
	pNetowkManager->GetRPC4()->UnregisterFunction("DestroyCheckpoint");
	pNetowkManager->GetRPC4()->UnregisterFunction("PickupSpawn");
	pNetowkManager->GetRPC4()->UnregisterFunction("PickupDestroy");

	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetHealth");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetArmour");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetPos");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_PutInVehicle");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_GivePlayerWeapon");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetPlayerSkin");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetPlayerZAngle");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetPlayerAction");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetPlayerRotation");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetArmedWeapon");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_RemoveFromVehicle");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_ToggleControls");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_ClientMessage");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_WorldBounds");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetVehicleHealth");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetVehicleColor");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_DestroyVehicle");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_PlaySound");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_FadeScreen");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_ClientCall");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_forceClassSelection");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_togglePlayerBleeding");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_FlashItem");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_popVehicleTrunk");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_setSkyColor");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetPlayerCash");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_toggleDriveByState");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_toggleCellPhone");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetCameraShakeIntensity");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetPlayerGravity");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_toggleTextForPlayer");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetText");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetTextPosition");
	pNetowkManager->GetRPC4()->UnregisterFunction("Script_SetTextColor");
}

//----------------------------------------------------
