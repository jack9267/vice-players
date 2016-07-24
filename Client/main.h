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

#include <stdio.h>
#include <string>
#include "../raknet/WindowsIncludes.h"

#define MAX_PLAYER_NAME		24
#define MAX_NETWORK_PLAYERS	50
#define MAX_SETTINGS_STRING 128
#define CLIENT_VERSION "(Alpha-2)"
#define PROJECT_WEBSITE "www.vc-p.com"

// NOTE: Must ALWAYS be unsigned
typedef unsigned char EntityId;

#define INVALID_ENTITY_ID 255

typedef struct _GAME_SETTINGS {
	BOOL bDebug;
	CHAR szConnectHost[MAX_SETTINGS_STRING+1];
	CHAR szConnectPort[MAX_SETTINGS_STRING+1];
	CHAR szConnectPass[MAX_SETTINGS_STRING+1];
	CHAR szNickName[MAX_SETTINGS_STRING+1];
} GAME_SETTINGS;

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString);
void InitSettings();


void logprintf(char * format, ...);
//----------------------------------------------------
// EOF

