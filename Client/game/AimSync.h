//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

#include "common.h"

void GameAimSyncInit();
void GameStoreLocalPlayerAim();
void GameSetLocalPlayerAim();
void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim);
void GameSetRemotePlayerAim(int iPlayer);

//----------------------------------------------------------