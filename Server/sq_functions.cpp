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
	RegisterFunction(pVM, "setScriptAuthor", sq_setScriptAuthor, 2, ".s");
	RegisterFunction(pVM, "setScriptVersion", sq_setScriptVersion, 2, ".s");
	RegisterFunction(pVM, "kickPlayer", sq_kickPlayer, 2, ".n");
	RegisterFunction(pVM, "banIP", sq_banIP, 2, ".s");
	RegisterFunction(pVM, "setGameTime", sq_setGameTime, 3, ".nn");
	RegisterFunction(pVM, "getMaxPlayers", sq_getMaxPlayers, 1, NULL);
	RegisterFunction(pVM, "getTickCount", sq_getTickCount, 1, NULL);
	RegisterFunction(pVM, "playSound", sq_playSound, 6, ".nnnnn");
	RegisterFunction(pVM, "fadeScreen", sq_fadeScreen, 3, ".nnn");
	RegisterFunction(pVM, "forceClassSelection", sq_forceClassSelection, 2, ".n");
	RegisterFunction(pVM, "isConnected", sq_isConnected, 2, ".n");
	RegisterFunction(pVM, "togglePlayerBleeding", sq_togglePlayerBleeding, 3, ".ni");
	RegisterFunction(pVM, "setItemFlashing", sq_setItemFlashing, 3, ".ni");
	RegisterFunction(pVM, "forceClassSelection", sq_forceClassSelection, 2, ".n");
	RegisterFunction(pVM, "popVehicleTrunk", sq_popVehicleTrunk, 2, ".n");
	RegisterFunction(pVM, "loadClientScript", sq_loadClientScript, 3, ".ns");
	RegisterFunction(pVM, "toggleDriveByState", sq_toggleDriveByState, 3, ".ni");
	RegisterFunction(pVM, "toggleCellPhone", sq_toggleCellPhone, 3, ".ni");
	RegisterFunction(pVM, "setCameraShakeIntensity", sq_setCameraShakeIntensity, 3, ".ni");
	RegisterFunction(pVM, "isPluginLoaded", sq_isPluginLoaded, 2, ".s");
	RegisterFunction(pVM, "call", sq__call, -1, NULL);
	RegisterFunction(pVM, "clientCall", sq_clientCall, -1, NULL);

	// Players
	RegisterFunction(pVM, "setPlayerTime", sq_setPlayerTime, 4, ".nnn");
	RegisterFunction(pVM, "getPlayerName", sq_getPlayerName, 2, ".n");
	RegisterFunction(pVM, "getPlayerIP", sq_getPlayerIP, 2, ".n");
	RegisterFunction(pVM, "setPlayerCameraPos", sq_setPlayerCameraPos, 5, ".nnnn");
	RegisterFunction(pVM, "setPlayerCameraRot", sq_setPlayerCameraRot, 5, ".nnnn");
	RegisterFunction(pVM, "setPlayerCameraLookAt", sq_setPlayerCameraLookAt, 5, ".nnnn");
	RegisterFunction(pVM, "setCameraBehindPlayer", sq_setCameraBehindPlayer, 2, ".n");
	RegisterFunction(pVM, "setPlayerGravity", sq_setPlayerGravity, 3, ".ni");
	RegisterFunction(pVM, "getPlayerGravity", sq_getPlayerGravity, 2, ".n");
	RegisterFunction(pVM, "getPlayerTeam", sq_getPlayerTeam, 2, ".n");
	RegisterFunction(pVM, "setPlayerAction", sq_setPlayerAction, 3, ".nn");
	RegisterFunction(pVM, "isPlayerInVehicle", sq_isPlayerInVehicle, 2, ".n");
	RegisterFunction(pVM, "getPlayerVehicleID", sq_getPlayerVehicleID, 2, ".n");
	RegisterFunction(pVM, "getPlayerTurnSpeed", sq_getPlayerTurnSpeed, 2, ".n");
	RegisterFunction(pVM, "getPlayerMoveSpeed", sq_getPlayerMoveSpeed, 2, ".n");
	RegisterFunction(pVM, "addPlayerClass", sq_addPlayerClass, 13, ".nnnnnnnnnnnn");
	RegisterFunction(pVM, "getPlayerHealth", sq_getPlayerHealth, 2, ".n");
	RegisterFunction(pVM, "setPlayerHealth", sq_setPlayerHealth, 3, ".nn");
	RegisterFunction(pVM, "getPlayerArmour", sq_getPlayerArmour, 2, ".n");
	RegisterFunction(pVM, "setPlayerArmour", sq_setPlayerArmour, 3, ".nn");
	RegisterFunction(pVM, "sendPlayerMessage", sq_sendPlayerMessage, 4, ".iis");
	RegisterFunction(pVM, "sendPlayerMessageToAll", sq_sendPlayerMessageToAll, 3, ".is");
	RegisterFunction(pVM, "setPlayerWorldBounds", sq_setPlayerWorldBounds, 5, ".n");
	RegisterFunction(pVM, "togglePlayerControls", sq_togglePlayerControls, 3, ".n");
	RegisterFunction(pVM, "removePlayerFromVehicle", sq_removePlayerFromVehicle, 2, ".n");
	RegisterFunction(pVM, "putPlayerInVehicle", sq_putPlayerInVehicle, 3, ".nn");
	RegisterFunction(pVM, "setPlayerArmedWeapon", sq_setPlayerArmedWeapon, 3, ".n");
	RegisterFunction(pVM, "givePlayerWeapon", sq_givePlayerWeapon, 4, ".iii");
	RegisterFunction(pVM, "resetPlayerWeapons", sq_resetPlayerWeapons, 2, ".n");
	RegisterFunction(pVM, "getPlayerRotation", sq_getPlayerRotation, 2, ".n");
	RegisterFunction(pVM, "setPlayerRotation", sq_setPlayerRotation, 3, ".nn");
	RegisterFunction(pVM, "getPlayerSkin", sq_getPlayerSkin, 2, ".n");
	RegisterFunction(pVM, "setPlayerSkin", sq_setPlayerSkin, 3, ".nn");
	RegisterFunction(pVM, "getPlayerPosition", sq_getPlayerPosition, 2, ".n");
	RegisterFunction(pVM, "setPlayerPosition", sq_setPlayerPosition, 5, ".nnnn");
	RegisterFunction(pVM, "setPlayerTurnSpeed", sq_setPlayerTurnSpeed, 5, ".nnnn");
	RegisterFunction(pVM, "setPlayerMoveSpeed", sq_setPlayerMoveSpeed, 5, ".nnnn");
	RegisterFunction(pVM, "setPlayerSkyColor", sq_setPlayerSkyColor, 4, ".nii");
	RegisterFunction(pVM, "setPlayerCash", sq_setPlayerCash, 3, ".ni");
	RegisterFunction(pVM, "getPlayerCash", sq_getPlayerCash, 2, ".n");
	RegisterFunction(pVM, "getPlayerSerial", sq_getPlayerSerial, 2, ".i");
	RegisterFunction(pVM, "isPlayerPaused", sq_isPlayerPaused, 2, ".n");

	// Vehicles
	RegisterFunction(pVM, "createVehicle", sq_createVehicle, 8, ".nnnnnnn");
	RegisterFunction(pVM, "destroyVehicle", sq_destroyVehicle, 2, ".n");
	RegisterFunction(pVM, "setVehicleHealth", sq_setVehicleHealth, 3, ".nn");
	RegisterFunction(pVM, "setVehicleColor", sq_setVehicleColor, 4, ".nnn");
	RegisterFunction(pVM, "getVehicleHealth", sq_getVehicleHealth, 2, ".n");
	RegisterFunction(pVM, "getVehicleColors", sq_getVehicleColors, 2, ".n");
	RegisterFunction(pVM, "getVehiclePosition", sq_getVehiclePosition, 2, ".n");
	RegisterFunction(pVM, "setVehiclePosition", sq_setVehiclePosition, 5, ".nnnn");
	RegisterFunction(pVM, "getVehicleTurnSpeed", sq_getVehicleTurnSpeed, 2, ".n");
	RegisterFunction(pVM, "getVehicleMoveSpeed", sq_getVehicleMoveSpeed, 2, ".n");
	RegisterFunction(pVM, "setVehicleTurnSpeed", sq_setVehicleTurnSpeed, 5, ".nnnn");
	RegisterFunction(pVM, "setVehicleMoveSpeed", sq_setVehicleMoveSpeed, 5, ".nnnn");

	// Timers
	RegisterFunction(pVM, "setTimer", sq_setTimer, -1, NULL);
	RegisterFunction(pVM, "killTimer", sq_killTimer, -1, NULL);
	RegisterFunction(pVM, "isTimerActive", sq_isTimerActive, -1, NULL);

	// Map/Mode names
	RegisterFunction(pVM, "setModeName", sq_setModeName, 2, ".s");
	RegisterFunction(pVM, "setMapName", sq_setMapName, 2, ".s");

	// Text functions
	RegisterFunction(pVM, "createText", sq_createText, 7, ".isiffs");
	RegisterFunction(pVM, "destroyText", sq_destroyText, 2, ".i");
	RegisterFunction(pVM, "toggleTextForPlayer", sq_toggleTextForPlayer, 4, ".nnb");
	RegisterFunction(pVM, "toggleTextForAll", sq_toggleTextForAll, 3, ".nb");
	RegisterFunction(pVM, "setTextForAll", sq_setTextForAll, 3, ".ns");
	RegisterFunction(pVM, "setTextForPlayer", sq_setTextForPlayer, 4, ".nns");
	RegisterFunction(pVM, "setTextColorForAll", sq_setTextColorForAll, 3, ".nn");
	RegisterFunction(pVM, "setTextColorForPlayer", sq_setTextColorForPlayer, 4, ".nnn");
	RegisterFunction(pVM, "setTextPositionForAll", sq_setTextPositionForAll, 4, ".nff");
	RegisterFunction(pVM, "setTextPositionForPlayer", sq_setTextPositionForPlayer, 5, ".nnff");

	// Checkpoints
	RegisterFunction(pVM, "createCheckpoint", sq_createCheckpoint, 5, ".ffff");
	RegisterFunction(pVM, "destroyCheckpoint", sq_destroyCheckpoint, 2, ".n");

	// Pickups
	RegisterFunction(pVM, "createPickup", sq_createPickup, 6, ".nnfff");
	RegisterFunction(pVM, "destroyPickup", sq_destroyPickup, 2, ".n");

	// Objects
	RegisterFunction(pVM, "createObject", sq_createObject, 8, ".nffffff");
	RegisterFunction(pVM, "destroyObject", sq_destroyObject, 2, ".n");

	return 1;
}

