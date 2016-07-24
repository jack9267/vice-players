//----------------------------------------------------
//
// VC-P Multiplayer Modification For GTA:VC
// Copyright VC-P Team 2010
//
// File Author(s): kyeman
//				   bpeterson
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"

using namespace RakNet;

extern CNetworkManager *pNetowkManager;
extern char     *szAdminPass;
extern CRcon    *pRcon;
extern CScripts	*pScripts;
#ifndef WIN32
#	define stricmp strcasecmp
#endif

#define NETGAME_VERSION 7

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2

void FilterInvalidNickChars(PCHAR szString);

DWORD dwKeys[128];
DWORD dwLastKeyEvent[2];

//----------------------------------------------------
// Sent by a client who's wishing to join us in our
// multiplayer-like activities.

void ClientJoin(RakNet::BitStream *bitStream, Packet *packet)
{
	RakNet::BitStream bsReject;
	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	CVehicleManager *pVehicleManager = pNetowkManager->GetVehicleManager();

	CHAR szPlayerName[MAX_PLAYER_NAME];
	CHAR szSerial[20];
	EntityId playerID;
	BYTE byteVersion;
	BYTE byteNickLen;
	BYTE byteRejectReason;

	bitStream->Read(byteVersion);

	if(byteVersion != NETGAME_VERSION) {
		byteRejectReason = REJECT_REASON_BAD_VERSION;
		bsReject.Write(byteRejectReason);
		pNetowkManager->GetRPC4()->Call("ConnectionRejected", &bsReject,HIGH_PRIORITY,RELIABLE,0,packet->guid,false);
		return;
	}

	bitStream->Read(byteNickLen);
	bitStream->Read(szPlayerName,byteNickLen);
	szPlayerName[byteNickLen] = '\0';

	bitStream->Read(szSerial, 20);

	FilterInvalidNickChars(szPlayerName);
	byteNickLen = strlen(szPlayerName);

	if(byteNickLen==0 || byteNickLen > 16 || pPlayerManager->IsNickInUse(szPlayerName)) {
		byteRejectReason = REJECT_REASON_BAD_NICKNAME;
		bsReject.Write(byteRejectReason);
		pNetowkManager->GetRPC4()->Call("ConnectionRejected", &bsReject,HIGH_PRIORITY,RELIABLE,0,packet->guid,false);
		return;
	}

	playerID = (BYTE)packet->guid.systemIndex;
	
	// Add this client to the player pool.
	pPlayerManager->New(playerID, szPlayerName, szSerial);

	// Send this client back an 'InitGame' RPC
	RakNet::BitStream bsInitGame;
	bsInitGame.Write((char*)&pNetowkManager->m_vecInitPlayerPos, sizeof(Vector3));
	bsInitGame.Write((char*)&pNetowkManager->m_vecInitCameraPos, sizeof(Vector3));
	bsInitGame.Write((char*)&pNetowkManager->m_vecInitCameraLook, sizeof(Vector3));
	bsInitGame.Write(pNetowkManager->m_WorldBounds[0]);
	bsInitGame.Write(pNetowkManager->m_WorldBounds[1]);
	bsInitGame.Write(pNetowkManager->m_WorldBounds[2]);
	bsInitGame.Write(pNetowkManager->m_WorldBounds[3]);
	bsInitGame.Write(pNetowkManager->m_byteFriendlyFire);
	bsInitGame.Write(pNetowkManager->m_byteShowOnRadar);
	bsInitGame.Write(playerID);
	pNetowkManager->GetRPC4()->Call("InitGame", &bsInitGame,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->guid,false);

	// Send this client ServerJoins for every existing player
	RakNet::BitStream pbsExistingClient;

	for(EntityId x = 0; x < MAX_PLAYERS; x++)
	{
		if(x != playerID && pPlayerManager->GetSlotState(x))
		{
			pbsExistingClient.Reset();
			pbsExistingClient.Write(x);
			size_t sNameLength = strlen(pPlayerManager->GetPlayerName(x));
			pbsExistingClient.Write(sNameLength);
			pbsExistingClient.Write(pPlayerManager->GetPlayerName(x), sNameLength);
			pNetowkManager->GetRPC4()->Call("ServerJoin", &pbsExistingClient, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false);

			// Now also spawn the player for them if they're active.
			CPlayer * pSpawnPlayer = pPlayerManager->GetAt(x);

			// Is this player spawned?
			if(pSpawnPlayer->IsActive())
			{
				// Spawn this player for the new player
				pSpawnPlayer->SpawnForPlayer(playerID);
			}
		}
	}

	// Spawn all existing vehicles for player.
	CVehicle * pVehicle;

	for(EntityId x = 0; x < MAX_VEHICLES; x++)
	{
		if(pVehicleManager->GetSlotState(x))
		{
			pVehicle = pVehicleManager->GetAt(x);

			// Is this vehicle spawned?
			if(pVehicle->IsActive())
			{
				// Spawn this for the new player
				pVehicle->SpawnForPlayer(playerID);
			}
		}
	}

	// Spawn all objects for the player
	pNetowkManager->GetObjectManager()->InitForPlayer(playerID);

	// Create all texts for player
	pNetowkManager->GetTextManager()->InitForPlayer(playerID);

	// Create all checkpoints for player
	pNetowkManager->GetCheckpoints()->InitForPlayer(playerID);

	// Create all pickups for player
	pNetowkManager->GetPickupManager()->InitForPlayer(playerID);
}

//----------------------------------------------------
// Sent by client with global chat text

void Chat(RakNet::BitStream *bitStream, Packet *packet)
{
	CHAR szText[256];
	BYTE byteTextLen;
	CPlayerManager *pPool = pNetowkManager->GetPlayerManager();

	bitStream->Read(byteTextLen);
	bitStream->Read(szText,byteTextLen);
	szText[byteTextLen] = '\0';

	if(!pPool->GetSlotState((BYTE)packet->guid.systemIndex)) return;

	if(!pScripts->onPlayerText((BYTE)packet->guid.systemIndex, szText))
		return;
	
	logprintf("[%s]: %s",
		pPool->GetPlayerName((BYTE)packet->guid.systemIndex),
		szText);

	pRcon->OutputChatMessage(pPool->GetPlayerName((BYTE)packet->guid.systemIndex),
		szText);

	// Now pass it off to all the other clients.
	RakNet::BitStream bsSend;
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	bsSend.Write(playerID);
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);

	pNetowkManager->GetRPC4()->Call("Chat", &bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,true);
}

//----------------------------------------------------
// Sent by client with command text

void ChatCommand(RakNet::BitStream *bitStream, Packet *packet)
{
	CHAR szText[256];
	BYTE byteTextLen;
	CPlayerManager *pPool = pNetowkManager->GetPlayerManager();

	EntityId playerID = (BYTE)packet->guid.systemIndex;

	bitStream->Read(byteTextLen);
	bitStream->Read(szText,byteTextLen);
	szText[byteTextLen] = '\0';

	if(!pPool->GetSlotState((BYTE)packet->guid.systemIndex)) return;

	pScripts->onPlayerCommand(playerID, szText);
}


//----------------------------------------------------
// Sent by client who wishes to request a class from
// the gamelogic handler.

void RequestClass(RakNet::BitStream *bitStream, Packet *packet)
{
	// TODO: Make this cancelable sometime
	BYTE byteOutcome = 1;
	int iRequestedClass;
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	bitStream->Read(iRequestedClass);

	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;
	
	iRequestedClass = pNetowkManager->GetGameLogic()->HandleSpawnClassRequest(playerID, iRequestedClass);

	RakNet::BitStream bsSpawnRequestReply;
	CPlayer * pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
	PLAYER_SPAWN_INFO * SpawnInfo = pPlayer->GetSpawnInfo();

	// TODO: This call should decide the outcome
	pScripts->onPlayerRequestClass(playerID, iRequestedClass);

	bsSpawnRequestReply.Write(byteOutcome);
	if(byteOutcome) {
		bsSpawnRequestReply.Write(iRequestedClass);
		bsSpawnRequestReply.Write((char *)SpawnInfo, sizeof(PLAYER_SPAWN_INFO));

		pNetowkManager->GetRPC4()->Call("RequestClass", &bsSpawnRequestReply ,HIGH_PRIORITY, RELIABLE, 0, packet->guid, false);
	}
}

//----------------------------------------------------
// Sent by client when they're spawning/respawning.

void Spawn(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
	pPlayer->Spawn();

	pScripts->onPlayerSpawn(playerID);
}

//----------------------------------------------------
// Sent by client when they die.

void Death(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);

	BYTE byteDeathReason;
	BYTE byteWhoWasResponsible;

	bitStream->Read(byteDeathReason);
	bitStream->Read(byteWhoWasResponsible);

	if(pPlayer) {
		pPlayer->HandleDeath(byteDeathReason,byteWhoWasResponsible);
		pScripts->onPlayerDeath(playerID, byteWhoWasResponsible, byteDeathReason);
	}
}

//----------------------------------------------------
// Sent by client when a vehicle is dead.

void VehicleDeath(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	EntityId vehicleID;

	bitStream->Read(vehicleID);

	CVehicleManager * pVehicleManager = pNetowkManager->GetVehicleManager();
	CVehicle * pVehicle = pVehicleManager->GetAt(vehicleID);
	if(pVehicle) {
		pVehicleManager->FlagForRespawn(vehicleID);

		pScripts->onVehicleDeath(vehicleID);
	}
}

//----------------------------------------------------
// Sent by client when they want to enter a
// vehicle gracefully.

void EnterVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
	EntityId vehicleID;
	BYTE bytePassenger;

	bitStream->Read(vehicleID);
	bitStream->Read(bytePassenger);
	pPlayer->EnterVehicle(vehicleID,bytePassenger);
	
	pScripts->onPlayerEnterVehicle(playerID, vehicleID);
	logprintf("%u enters vehicle %u",playerID,vehicleID);
}

//----------------------------------------------------
// Sent by client when they want to exit a
// vehicle gracefully.

void ExitVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);
	EntityId vehicleID;

	bitStream->Read(vehicleID);
	pPlayer->ExitVehicle(vehicleID);

	pScripts->onPlayerExitVehicle(playerID, vehicleID);
	logprintf("%u exits vehicle %u",playerID,vehicleID);
}

//----------------------------------------------------
// Sent by client when they want score and ping information.

void UpdateScoreAndPing(RakNet::BitStream *bitStream, Packet *packet)
{
	RakNet::BitStream bsSend;
	EntityId playerID = (EntityId)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	RakPeerInterface * pRak = pNetowkManager->GetRakPeer();

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(pPlayerManager->GetSlotState(i))
		{
			bsSend.Write(i);
			bsSend.Write(pPlayerManager->GetScore(i));
			bsSend.Write(pRak->GetLastPing(pRak->GetSystemAddressFromIndex(i)));
			
			if(pPlayerManager->IsAdmin(playerID))
			{
				bsSend.Write((unsigned long)pRak->GetSystemAddressFromIndex(i).binaryAddress);
			}
			else
			{
				bsSend.Write((unsigned long)0UL);
			}
		}
	}

	pNetowkManager->GetRPC4()->Call("UpdateScoreAndPing", &bsSend, HIGH_PRIORITY, RELIABLE, 0, packet->guid, false);
}

//----------------------------------------------------

void Admin(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	char szPassTest[65];
	int iPassLen;

	bitStream->Read(iPassLen);
	if(iPassLen > 64) return;
	bitStream->Read(szPassTest,iPassLen);

	szPassTest[iPassLen] = '\0';
	
	if(!strcmp(szPassTest,szAdminPass)) {
		pPlayerManager->SetAdmin(playerID);
	}
}

//----------------------------------------------------

void KickPlayer(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	BYTE byteKickPlayer;

	if(pPlayerManager->IsAdmin(playerID)) {
		bitStream->Read(byteKickPlayer);	
		pNetowkManager->KickPlayer(byteKickPlayer);
	}
}

//----------------------------------------------------

void BanIPAddress(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();

	char ban_ip[256];
	int iBanIpLen;

	if(pPlayerManager->IsAdmin(playerID)) {
		bitStream->Read(iBanIpLen);
		bitStream->Read(ban_ip,iBanIpLen);
		ban_ip[iBanIpLen] = 0;

		pNetowkManager->AddBan(ban_ip);
	}
}

void KeyEvent(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (EntityId)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	BYTE type;

	DWORD dwKey;
	bool state;

	bitStream->Read(dwKey);
	state = bitStream->ReadBit();
	
	if(dwKeys[dwKey] != state)
	{
		if(state == true)
			type = 0;
		else
			type = 2;
	}
	else
	{
		type = 1;
	}

	dwLastKeyEvent[0] = dwKey;
	dwLastKeyEvent[1] = state;
	dwKeys[dwKey] = state;

	pScripts->onPlayerKeyEvent(playerID, type, (char*)&dwKey);
}

void CheckpointEvent(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	EntityId cpId;
	bool state; // true - entered, false - left.
	bitStream->Read(cpId);
	bitStream->Read(state);

	if(state)
		pScripts->onCheckpointEnter(playerID, cpId);
	else
		pScripts->onCheckpointLeave(playerID, cpId);

}

//----------------------------------------------------
// Remote client has had damage inflicted upon them

void InflictDamage(RakNet::BitStream * bitStream, Packet * packet)
{
	EntityId playerID = (EntityId)packet->guid.systemIndex;
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();
	CVehicleManager * pVehicleManager = pNetowkManager->GetVehicleManager();

	if(!pPlayerManager->GetSlotState(playerID))
	{
		return;
	}

	bool bPlayerVehicleDamager;
	EntityId damagerID;
	int iWeapon;
	float fUnk;
	int iPedPieces;
	BYTE byteUnk;

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

	BitStream bsSend;
	bsSend.Write(playerID);
	
	if(bPlayerVehicleDamager)
	{
		bsSend.Write1();
	}
	else
	{
		bsSend.Write0();
	}

	bsSend.Write(damagerID);
	bsSend.Write(iWeapon);
	bsSend.Write(fUnk);
	bsSend.Write(iPedPieces);
	bsSend.Write(byteUnk);
	pNetowkManager->GetRPC4()->Call("InflictDamage", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true);
	logprintf("InflictDamage(%d, %d, %d, %d, %f, %d, %d)", playerID, bPlayerVehicleDamager, damagerID, iWeapon, fUnk, iPedPieces, byteUnk);
}

void Pause(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;

	if(!pNetowkManager->GetPlayerManager()->GetSlotState(playerID)) return;

	CPlayer	*pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerID);

	int iPause;

	bitStream->Read(iPause);

	pPlayer->SetPause(iPause);

	if(iPause)
		pScripts->onPlayerPause(playerID);
	else
		pScripts->onPlayerUnpause(playerID);
}

//----------------------------------------------------

void RegisterRPCs()
{
	pNetowkManager->GetRPC4()->RegisterFunction("ClientJoin", ClientJoin);
	pNetowkManager->GetRPC4()->RegisterFunction("Chat", Chat);
	pNetowkManager->GetRPC4()->RegisterFunction("ChatCommand", ChatCommand);
	pNetowkManager->GetRPC4()->RegisterFunction("RequestClass", RequestClass);
	pNetowkManager->GetRPC4()->RegisterFunction("Spawn", Spawn);
	pNetowkManager->GetRPC4()->RegisterFunction("Death", Death);
	pNetowkManager->GetRPC4()->RegisterFunction("VehicleDeath", VehicleDeath);
	pNetowkManager->GetRPC4()->RegisterFunction("EnterVehicle", EnterVehicle);
	pNetowkManager->GetRPC4()->RegisterFunction("ExitVehicle", ExitVehicle);
	pNetowkManager->GetRPC4()->RegisterFunction("UpdateScoreAndPing", UpdateScoreAndPing);
	pNetowkManager->GetRPC4()->RegisterFunction("Admin", Admin);
	pNetowkManager->GetRPC4()->RegisterFunction("KickPlayer", KickPlayer);
	pNetowkManager->GetRPC4()->RegisterFunction("BanIPAddress", BanIPAddress);
	pNetowkManager->GetRPC4()->RegisterFunction("KeyEvent", KeyEvent);
	pNetowkManager->GetRPC4()->RegisterFunction("CheckpointEvent", CheckpointEvent);
	pNetowkManager->GetRPC4()->RegisterFunction("InflictDamage", InflictDamage);
	pNetowkManager->GetRPC4()->RegisterFunction("Pause", Pause);
}

//----------------------------------------------------

void UnRegisterRPCs()
{
	pNetowkManager->GetRPC4()->UnregisterFunction("ClientJoin");
	pNetowkManager->GetRPC4()->UnregisterFunction("Chat");
	pNetowkManager->GetRPC4()->UnregisterFunction("ChatCommand");
	pNetowkManager->GetRPC4()->UnregisterFunction("RequestClass");
	pNetowkManager->GetRPC4()->UnregisterFunction("Spawn");
	pNetowkManager->GetRPC4()->UnregisterFunction("Death");
	pNetowkManager->GetRPC4()->UnregisterFunction("VehicleDeath");
	pNetowkManager->GetRPC4()->UnregisterFunction("EnterVehicle");
	pNetowkManager->GetRPC4()->UnregisterFunction("ExitVehicle");
	pNetowkManager->GetRPC4()->UnregisterFunction("UpdateScoreAndPing");
	pNetowkManager->GetRPC4()->UnregisterFunction("Admin");
	pNetowkManager->GetRPC4()->UnregisterFunction("KickPlayer");
	pNetowkManager->GetRPC4()->UnregisterFunction("BanIPAddress");
	pNetowkManager->GetRPC4()->UnregisterFunction("KeyEvent");
	pNetowkManager->GetRPC4()->UnregisterFunction("CheckpointEvent");
	pNetowkManager->GetRPC4()->UnregisterFunction("InflictDamage");
	pNetowkManager->GetRPC4()->UnregisterFunction("Pause");
}

//----------------------------------------------------
