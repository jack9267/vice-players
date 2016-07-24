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
extern CNetworkManager		 *pNetowkManager;

DWORD dwPlayerIncrementer=2;
extern GAME_SETTINGS tSettings;
using namespace RakNet;

//----------------------------------------------------

void WriteCompression(ULONG *ulWrite)
{
	FILE *fileComp=fopen("ClientLayer.txt","w");

	int x=0;
	
	while(x!=256) {
		fprintf(fileComp,"0x%X,",ulWrite[x]);
		x++;
	}

	fclose(fileComp);
}

//----------------------------------------------------

void cmdDefaultCmdProc(PCHAR szCmd)
{
	if(pNetowkManager) {
		CLocalPlayer *pLocalPlayer;
		pLocalPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();
		pLocalPlayer->Say(szCmd);
	}
}

//----------------------------------------------------

void cmdQuit(PCHAR szCmd)
{
	if(pNetowkManager) {
		pNetowkManager->Shutdown();
	}

	ExitProcess(0);
}

//----------------------------------------------------

void cmdKill(PCHAR szCmd)
{
	pGame->FindPlayerPed()->SetHealth(0.0f);
}

//----------------------------------------------------

void cmdSavePos(PCHAR szCmd)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	FILE *fileOut;
	DWORD dwVehicleID;
	float fZAngle;

	fileOut = fopen("savedpositions.txt","a");
	if(!fileOut) {
		pChatWindow->AddDebugMessage("I can't open the savepositions.txt file for append.");
		return;
	}

	// incar savepos

	if(pPlayer->IsInVehicle()) {
		VEHICLE_TYPE *pVehicle = pPlayer->GetGtaVehicle();
		WORD wModelIndex = pVehicle->physical.entity.wModelIndex;
		Vector3 * vPos = &pVehicle->physical.entity.placeable.matMatrix.vPos;
		dwVehicleID = CPools::GetIndexFromVehicle(pVehicle);
		ScriptCommand(&get_car_z_angle,dwVehicleID,&fZAngle);

		fprintf(fileOut,"createVehicle(%u, %.4f, %.4f, %.4f, %.4f, %u, %u)\n", wModelIndex, vPos->X, vPos->Y, vPos->Z, 
			fZAngle, pVehicle->byteColors[0], pVehicle->byteColors[1]);

		fclose(fileOut);

		pChatWindow->AddDebugMessage("-> InVehicle position saved.");

		return;
	}

	// onfoot savepos
	Vector3 vPos;
	pPlayer->GetPosition(&vPos);
	fZAngle = pPlayer->GetRotation();

	fprintf(fileOut,"addPlayerClass(0, 0, %.4f, %.4f, %.4f, %.4f, 0, 0, 0, 0, 0, 0);\n", vPos.X, vPos.Y, vPos.Z, fZAngle);

	fclose(fileOut);

	pChatWindow->AddDebugMessage("-> OnFoot position saved.");
}

//----------------------------------------------------

void cmdCreateVehicle(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	if(!strlen(szCmd)){
		pChatWindow->AddDebugMessage("Usage: /vehicle (vehicle id).");
		return;
	}	
	int iVehicleType = atoi(szCmd);

	if((iVehicleType < 130) || (iVehicleType > 236))
	{
		pChatWindow->AddDebugMessage("Invalid vehicle ID.");
		return;
	}

	pGame->RequestModel(iVehicleType);
	pGame->LoadRequestedModels();
	while(pGame->IsModelLoaded(iVehicleType)==FALSE) Sleep(5);

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	if(pPlayer) 
	{
		Vector3 vPos;
		pPlayer->GetPosition(&vPos);
		CVehicle *pTestVehicle = pGame->NewVehicle(iVehicleType, (vPos.X - 5.0f), (vPos.Y - 5.0f), vPos.Z, 0.0f);
	}
}

//----------------------------------------------------

void cmdSkin(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	if(!strlen(szCmd)){	
		pChatWindow->AddDebugMessage("Usage: /skin [default.ide number]");
		return;
	}
	int iPlayerSkin = atoi(szCmd);

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	if(pPlayer)
	{
		pGame->RequestModel(iPlayerSkin);
		pGame->LoadRequestedModels();
		while(pGame->IsModelLoaded(iPlayerSkin)==FALSE) Sleep(2);
		pPlayer->SetModel(iPlayerSkin);
	}
}

//----------------------------------------------------

void cmdBuild(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	CCamera * pGameCamera = pGame->GetCamera();
	pGameCamera->Restore();
	pGame->DisplayHud(TRUE);
}

//----------------------------------------------------

void cmdAdmin(PCHAR szCmd)
{
	int iPassLen=0;
	RakNet::BitStream bsSend;

	if(pNetowkManager) {
		iPassLen = strlen(szCmd);
		if(iPassLen > 64) return;
		bsSend.Write(iPassLen);
		bsSend.Write(szCmd,iPassLen);
		pNetowkManager->GetRPC4()->Call("Admin",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------

void IP2String(DWORD ip, char *ret)
{
	BYTE p[4];

	p[0] = (BYTE)(ip & 0xFF); ip >>= 8;
	p[1] = (BYTE)(ip & 0xFF); ip >>= 8;	
	p[2] = (BYTE)(ip & 0xFF); ip >>= 8;
	p[3] = (BYTE)(ip & 0xFF);

	sprintf(ret,"%u.%u.%u.%u",p[0],p[1],p[2],p[3]);
}

//----------------------------------------------------

void cmdGetIP(PCHAR szCmd)
{
	if(!strlen(szCmd)) {
	    pChatWindow->AddDebugMessage("Usage: /getip [player_id]");
		return;
	}

	EntityId playerID;
	sscanf(szCmd,"%u",&playerID);
	
	if(pNetowkManager) {
		CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
		if(pPlayerManager->GetSlotState(playerID)) {

			char ret[30];
			DWORD ip = pPlayerManager->GetIPAddress(playerID);
			IP2String(ip,ret);
			pChatWindow->AddDebugMessage("%s is: %s",
				pPlayerManager->GetPlayerName(playerID),ret);

		} else {
			pChatWindow->AddDebugMessage("That player doesn't appear to be active.");
		}
	}
}

//----------------------------------------------------

void cmdGenComp(PCHAR szCmd)
{

}

//----------------------------------------------------

void cmdNewPlayer(PCHAR szCmd)
{
	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	pPlayerManager->New(1, "jaja");
	CRemotePlayer *pRemotePlayer = pPlayerManager->GetAt(1);
	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	BYTE byteSkin = (BYTE)pPlayerPed->GetModelIndex();
	Vector3 vPos;
	pPlayerPed->GetPosition(&vPos);
	float fRotation = pPlayerPed->GetRotation();
	pRemotePlayer->SpawnPlayer(255, byteSkin, &vPos, fRotation, 0,0, 0,0, 0,0);
	pRemotePlayer->SetReportedHealth(100);
	pRemotePlayer->SetReportedArmour(100);
}

//----------------------------------------------------

void cmdIsVehicle(PCHAR szCmd)
{
	if(pGame->FindPlayerPed()->IsInVehicle()) pChatWindow->AddDebugMessage("You are in a vehicle.");
	else pChatWindow->AddDebugMessage("You are NOT in a vehicle.");
}

void SetupCommands()
{
	pCmdWindow->AddDefaultCmdProc(cmdDefaultCmdProc);
	pCmdWindow->AddCmdProc("isvehicle", cmdIsVehicle);
	pCmdWindow->AddCmdProc("quit",cmdQuit);
	pCmdWindow->AddCmdProc("q",cmdQuit);
	pCmdWindow->AddCmdProc("admin",cmdAdmin);
	pCmdWindow->AddCmdProc("gencomp",cmdGenComp);
	pCmdWindow->AddCmdProc("kill",cmdKill);
	pCmdWindow->AddCmdProc("save",cmdSavePos);
	pCmdWindow->AddCmdProc("v",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("vehicle",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("skin",cmdSkin);
	pCmdWindow->AddCmdProc("build",cmdBuild);
	pCmdWindow->AddCmdProc("np", cmdNewPlayer);
}

//----------------------------------------------------