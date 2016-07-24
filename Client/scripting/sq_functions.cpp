//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: Christopher
//               adamix
//				 bpeterson	
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"
extern CNetworkManager *pNetowkManager;

using namespace RakNet;

int sq_register_natives(SQVM * pVM)
{
	RegisterFunction(pVM, "setScriptAuthor", sq_setScriptAuthor, 2 , ".s");
	RegisterFunction(pVM, "setScriptVersion", sq_setScriptVersion, 2, ".s");
	RegisterFunction(pVM, "addChatMessage", sq_addChatMessage, 2, ".s");
	RegisterFunction(pVM, "call", sq__call, -1, NULL);
	RegisterFunction(pVM, "toggleCheatCodes", sq_toggleCheatCodes, 2, ".b");
	RegisterFunction(pVM, "getVersion", sq_getVersion, -1, NULL);

	// Timers
	RegisterFunction(pVM, "setTimer", sq_setTimer, -1, NULL);
	RegisterFunction(pVM, "killTimer", sq_killTimer, -1, NULL);
	RegisterFunction(pVM, "isTimerActive", sq_isTimerActive, -1, NULL);

	// Graphic
	RegisterFunction(pVM, "getResolution", sq_getResolution, -1, NULL);
	RegisterFunction(pVM, "drawText", sq_drawText, 8, ".nnsiiii");
	RegisterFunction(pVM, "drawBox", sq_drawBox, 9, ".nnnniiii");
	RegisterFunction(pVM, "showCursor", sq_showCursor, 2, ".b");
	RegisterFunction(pVM, "showMessage", sq_showMessage, 3, ".ss");

	return 1;
}
