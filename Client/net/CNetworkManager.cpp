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

#define NETGAME_VERSION 7

using namespace RakNet;

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;
extern CScripts		 *pScripts;
RPC4				 *CNetworkManager::m_pRPC4;
extern bool D3DInited;

//----------------------------------------------------

void CompressVector1(Vector3 * vec, C_VECTOR1 * c1)
{
	c1->X = (short)(vec->X * 10000);
	c1->Y = (short)(vec->Y * 10000);
	c1->Z = (short)(vec->Z * 10000);
}

//----------------------------------------------------

void DecompressVector1(Vector3 * vec, C_VECTOR1 * c1)
{
	vec->X = (float)(c1->X / 10000.0);
	vec->Y = (float)(c1->Y / 10000.0);
	vec->Z = (float)(c1->Z / 10000.0);	
}

//----------------------------------------------------

CNetworkManager::CNetworkManager(PCHAR szHostOrIp, int iPort, 
				   PCHAR szPlayerName, PCHAR szPass)
{
	// Setup player pool
	m_pPlayerManager = new CPlayerManager();
	m_pPlayerManager->SetLocalPlayerName(szPlayerName);

	m_pVehicleManager = new CVehicleManager();

	m_pObjectManager = new CObjectManager();

	m_pTextManager = new CTextManager();
	
	m_pCheckpoints = new CCheckpoints();

	m_pPickupManager = new CPickupManager();

	m_pRakPeer = RakPeerInterface::GetInstance();
	m_pRPC4 = RPC4::GetInstance();

	RegisterRPCs();

	m_pRakPeer->Startup(1,&SocketDescriptor(),1);
	strcpy(m_szHostOrIp, szHostOrIp);
	m_iPort = iPort;
	strcpy(m_szPass, szPass);
	m_pRakPeer->AttachPlugin(m_pRPC4);

	if(pChatWindow)
	{
		pChatWindow->AddDebugMessage("Vice City: Players started " CLIENT_VERSION ".");
		pChatWindow->AddDebugMessage("You can visit the project website at " PROJECT_WEBSITE);
	}

	Connect();

	if(pChatWindow)
	{
		pChatWindow->AddDebugMessage("Connecting to %s:%d..",szHostOrIp,iPort);
	}

	m_pGameLogic = NULL;
	m_dwLastScoreUpdateTick = GetTickCount();
	m_uiLastRandSeed = 0;

	CPlayerPed *pPlayer = pGame->FindPlayerPed();
}

//----------------------------------------------------

CNetworkManager::~CNetworkManager()
{
	Shutdown();
	m_pRakPeer->DetachPlugin(m_pRPC4);
	UnRegisterRPCs();
	RPC4::DestroyInstance(m_pRPC4);
	RakPeerInterface::DestroyInstance(m_pRakPeer);
	delete m_pPlayerManager;
	delete m_pGameLogic;
}

//----------------------------------------------------

void CNetworkManager::InitGameLogic()
{
	m_pGameLogic = new CSpawnSelection();
}

//----------------------------------------------------

void CNetworkManager::Process()
{
	UpdateNetwork();

	if(GetGameState() == GAMESTATE_CONNECTED) {
		if(m_pPlayerManager) m_pPlayerManager->Process();
		if(m_pVehicleManager) m_pVehicleManager->Process();
		if(pScripts) pScripts->onPulse();
		if(m_pCheckpoints) m_pCheckpoints->Process();
	}

	// For syncing rand()
	/*if(m_pRakPeer->GetSynchronizedRandomInteger() != m_uiLastRandSeed) {
		m_uiLastRandSeed = m_pRakPeer->GetSynchronizedRandomInteger();
		srand(m_uiLastRandSeed);
	}*/
}

//----------------------------------------------------

void CNetworkManager::UpdateNetwork()
{
	Packet* pkt=NULL;

	while((pkt = m_pRakPeer->Receive()))
	{
		switch(pkt->data[0])
		{
			/*
		case ID_RSA_PUBLIC_KEY_MISMATCH:
			pChatWindow->AddDebugMessage("Failed to init encryption.");
			break;
			*/
		case ID_CONNECTION_BANNED:
			pChatWindow->AddDebugMessage("You're banned from the server.");
			Shutdown();
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			pChatWindow->AddDebugMessage("The server is full, Retrying...");
			Connect();
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			pChatWindow->AddDebugMessage("Disconnected.");
			Shutdown();
			pGame->FadeScreen(0, 0);
			break;
		case ID_CONNECTION_LOST:
			pChatWindow->AddDebugMessage("Lost connection to the server, Reconnecting...");
			Connect();
			pGame->FadeScreen(0, 0);
			break;
		case ID_INVALID_PASSWORD:
			pChatWindow->AddDebugMessage("Wrong server password.");
			break;
			/*
		case ID_MODIFIED_PACKET:
			m_pRakPeer->Shutdown(100);
			break;
			*/
		case ID_CONNECTION_ATTEMPT_FAILED:
			pChatWindow->AddDebugMessage("Failed to connect, Retrying...");
			Connect();
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			ConnectionSucceeded(pkt);
			break;
		case ID_PLAYER_SYNC:
			PlayerSync(pkt);
			break;
		case ID_VEHICLE_SYNC:
			VehicleSync(pkt);
			break;

		}

		m_pRakPeer->DeallocatePacket(pkt);		
	}

}

//----------------------------------------------------

void CNetworkManager::PlayerSync(Packet *p)
{
	CRemotePlayer * pPlayer;
	BitStream bsPlayerSync(p->data, p->length, FALSE);
	BYTE bytePlayerID;

	bsPlayerSync.IgnoreBytes(sizeof(MessageID));
	bsPlayerSync.Read(bytePlayerID);

	pPlayer = GetPlayerManager()->GetAt(bytePlayerID);
	if(pPlayer)
	{
		PLAYER_SYNC_DATA playerSyncData;
		CAMERA_AIM caAiming;

		bsPlayerSync.Read((char *)&playerSyncData, sizeof(PLAYER_SYNC_DATA));

		// read weather the bit stream has aim sync data or not
		bool bHasAimSync = bsPlayerSync.ReadBit();

		// read aim sync data if the bit stream has it
		if(bHasAimSync)
		{
			// read the aim sync data
			bsPlayerSync.Read((char *)&caAiming, sizeof(CAMERA_AIM));
		}

		// store the player sync data
		pPlayer->StoreOnFootFullSyncData(&playerSyncData);

		// store the aim sync data if the bit stream has it
		if(bHasAimSync)
		{
			pPlayer->StoreAimSyncData(&caAiming);
		}
	}
}

//----------------------------------------------------

void CNetworkManager::VehicleSync(Packet *p)
{
	CRemotePlayer * pPlayer;
	BitStream bsVehicleSync(p->data, p->length, FALSE);
	BYTE bytePlayerID;

	bsVehicleSync.IgnoreBytes(sizeof(MessageID));
	bsVehicleSync.Read(bytePlayerID);

	pPlayer = GetPlayerManager()->GetAt(bytePlayerID);
	if(pPlayer)
	{
		VEHICLE_SYNC_DATA vehicleSyncData;

		bsVehicleSync.Read((char *)&vehicleSyncData, sizeof(VEHICLE_SYNC_DATA));

		// store the vehicle sync data
		pPlayer->StoreInCarFullSyncData(&vehicleSyncData);
	}
}

//----------------------------------------------------

void CNetworkManager::ConnectionSucceeded(Packet *p)
{
	if(pChatWindow) {
		pChatWindow->AddDebugMessage("Connection success. Loading network game...");
	}

	m_iGameState = GAMESTATE_AWAIT_JOIN;

	BYTE byteVersion = NETGAME_VERSION;
	BYTE byteNameLen = (BYTE)strlen(m_pPlayerManager->GetLocalPlayerName());
	
	BitStream bsSend;
	bsSend.Write(byteVersion);
	bsSend.Write(byteNameLen);
	bsSend.Write(m_pPlayerManager->GetLocalPlayerName(),byteNameLen);

	unsigned long VolumeSerialNumber;
	char volumeSerial[20];
	char szWinDir[40];
	GetWindowsDirectoryA(szWinDir, 40);
	GetVolumeInformationA(szWinDir, NULL, 100, &VolumeSerialNumber, NULL, NULL, NULL, 100);
	itoa(VolumeSerialNumber, volumeSerial, 16);

	bsSend.Write(volumeSerial, 20);

	m_pRPC4->Call("ClientJoin",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------

void CNetworkManager::UpdatePlayerScoresAndPings()
{
	if( (GetTickCount() - m_dwLastScoreUpdateTick) > 1000 ) {
		m_dwLastScoreUpdateTick = GetTickCount();

		m_pRPC4->Call("UpdateScoreAndPing",NULL,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------

void CNetworkManager::Connect()
{
	if(m_pRakPeer) {
		m_pRakPeer->Connect(m_szHostOrIp, m_iPort, m_szPass, strlen(m_szPass));
		m_iGameState = GAMESTATE_CONNECTING;
	}
}

//----------------------------------------------------

bool CNetworkManager::IsConnected()
{
	if(GetGameState() == GAMESTATE_CONNECTED) {
		return true;
	}
	return false;
}

//----------------------------------------------------

void CNetworkManager::Shutdown()
{
	if(m_pRakPeer && m_iGameState == GAMESTATE_CONNECTED) {
		m_pRakPeer->Shutdown(500);
	}
	m_iGameState = GAMESTATE_DISCONNECTED;
}

//----------------------------------------------------