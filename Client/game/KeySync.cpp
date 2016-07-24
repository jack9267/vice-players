//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

//-----------------------------------------------------------

extern CGame * pGame;

GTA_CONTROLSET * pGcsInternalKeys = (GTA_CONTROLSET *)ADDR_KEYSTATES;

struct 
{
	GTA_CONTROLSET gcsControlState;
	BYTE byteDriveByLeft;
	BYTE byteDriveByRight;
} SavedKeys;

GTA_CONTROLSET gcsRemotePlayerKeys[MAX_PLAYERS];

//-----------------------------------------------------------

void GameKeyStatesInit()
{
	memset(&SavedKeys, 0, sizeof(SavedKeys));
	memset(&gcsRemotePlayerKeys, 0, sizeof(gcsRemotePlayerKeys));
}

//-----------------------------------------------------------

void GameStoreLocalPlayerKeys()
{
	memcpy(&SavedKeys.gcsControlState, pGcsInternalKeys, sizeof(GTA_CONTROLSET));

	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		// save current drive by state
		SavedKeys.byteDriveByLeft = pCamera->GetDriveByLeft();
		SavedKeys.byteDriveByRight = pCamera->GetDriveByRight();
	}
}

//-----------------------------------------------------------

void GameSetLocalPlayerKeys()
{
	memcpy(pGcsInternalKeys, &SavedKeys.gcsControlState, sizeof(GTA_CONTROLSET));

	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		// restore the drive by state also
		pCamera->SetDriveByLeft(SavedKeys.byteDriveByLeft);
		pCamera->SetDriveByRight(SavedKeys.byteDriveByRight);
	}
}

//-----------------------------------------------------------

void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET * pGcsKeyStates)
{
	memcpy(&gcsRemotePlayerKeys[iPlayer], pGcsKeyStates, sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameSetRemotePlayerKeys(int iPlayer)
{
	memcpy(pGcsInternalKeys, &gcsRemotePlayerKeys[iPlayer], sizeof(GTA_CONTROLSET));

	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		if(gcsRemotePlayerKeys[iPlayer].wKeys1[KEY_INCAR_LOOKL]) {
			pCamera->SetDriveByLeft(1);
		} else {
			pCamera->SetDriveByLeft(0);
		}	

		if(gcsRemotePlayerKeys[iPlayer].wKeys1[KEY_INCAR_LOOKR]) {
			pCamera->SetDriveByRight(1);
		} else {
			pCamera->SetDriveByRight(0);
		}
	}
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetInternalKeys()
{
	return pGcsInternalKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetLocalPlayerKeys()
{
	return &SavedKeys.gcsControlState;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetPlayerKeys(int iPlayer)
{
	return &gcsRemotePlayerKeys[iPlayer];
}

//-----------------------------------------------------------

void GameResetPlayerKeys(int iPlayer)
{
	memset(&gcsRemotePlayerKeys[iPlayer], 0, sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameResetLocalKeys()
{
	memset(pGcsInternalKeys, 0, sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------
