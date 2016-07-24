//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"

//-----------------------------------------------------------

extern CGame * pGame;

CAMERA_AIM caLocalPlayerAim;
CAMERA_AIM caRemotePlayerAim[MAX_PLAYERS];

//----------------------------------------------------------

void GameAimSyncInit()
{
	memset(&caLocalPlayerAim, 0, sizeof(CAMERA_AIM));
	memset(caRemotePlayerAim, 0, (sizeof(CAMERA_AIM) * MAX_PLAYERS));
}

//----------------------------------------------------------

void GameStoreLocalPlayerAim()
{
	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		memcpy(&caLocalPlayerAim, pCamera->GetAim(), sizeof(CAMERA_AIM));
	}
}

//----------------------------------------------------------

void GameSetLocalPlayerAim()
{
	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		pCamera->SetAim(&caLocalPlayerAim);
	}
}

//----------------------------------------------------------

void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim)
{
	memcpy(&caRemotePlayerAim[iPlayer], caAim, sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void GameSetRemotePlayerAim(int iPlayer)
{
	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		pCamera->SetAim(&caRemotePlayerAim[iPlayer]);
	}
}

//----------------------------------------------------------
