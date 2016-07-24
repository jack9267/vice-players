//-----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#pragma once



#define SAFE_DELETE(mem) if(mem) { delete mem; mem = NULL; }

#define MAX_PLAYER_NAME		24
#define MAX_PLAYERS			50
#define MAX_VEHICLES		200

// NOTE: Must ALWAYS be unsigned
typedef unsigned char EntityId;

#define INVALID_ENTITY_ID 255

#define DEFAULT_CONFIG_FILE "server.ini"
#define DEFAULT_MAX_PLAYERS 32
#define DEFAULT_LISTEN_PORT 8192
#define DEFAULT_RCON_PORT	8190
#define DEFAULT_RCON_MAXUSERS	8

//----------------------------------------------------

void logprintf(char * format, ...);

#ifndef _WIN32
#include <sys/time.h>
long GetTickCount();
char* itoa(int value, char* result, int base);
#endif

//----------------------------------------------------

