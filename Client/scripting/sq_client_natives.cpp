//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

extern CGame *pGame;
extern CNetworkManager *pNetowkManager;
extern CChatWindow *pChatWindow;

using namespace RakNet;

SQInteger sq_addChatMessage(SQVM * pVM)
{
	const char * message;
	sq_getstring(pVM, -1, &message);
	pChatWindow->AddDebugMessage("%s", message);
	return 1;
}

SQInteger sq_toggleCheatCodes(SQVM * pVM)
{
	SQBool toggle;
	sq_getbool(pVM, -1, &toggle);
	GameToggleCheatCodes(toggle != 0);
	return 1;
}

SQInteger sq_getVersion(SQVM * pVM)
{
	sq_pushstring(pVM, CLIENT_VERSION, -1);
	return 1;
}