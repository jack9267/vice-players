//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: Christopher
//               adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "main.h"
#include "CScripts.h"

SQInteger sq_setPlayerHealth(SQVM * v); // done
SQInteger sq_setPlayerArmour(SQVM * v); // done
SQInteger sq_setPlayerPosition(SQVM * v); // done
SQInteger sq_setPlayerTurnSpeed(SQVM * pVM); // done
SQInteger sq_setPlayerMoveSpeed(SQVM * pVM); // done
SQInteger sq_getPlayerPosition(SQVM * v); // done
SQInteger sq_setPlayerZAngle(SQVM * v); // done
SQInteger sq_forceClassSelection(SQVM * v); // done
SQInteger sq_getPlayerSkin(SQVM * v); // done
SQInteger sq_setPlayerSkin(SQVM * v); //done
SQInteger sq_toggleDriveByState(SQVM * pVM);
SQInteger sq_setPlayerAction(SQVM * v); // done
SQInteger sq_setPlayerRotation(SQVM * v); // done
SQInteger sq_getPlayerRotation(SQVM * v); //done
SQInteger sq_resetPlayerWeapons(SQVM * v); // done
SQInteger sq_setPlayerArmedWeapon(SQVM * v); // done
SQInteger sq_givePlayerWeapon(SQVM * v); // done
SQInteger sq_removePlayerFromVehicle(SQVM * v); // done
SQInteger sq_putPlayerInVehicle(SQVM * v); // done
SQInteger sq_togglePlayerControls(SQVM * v); // done
SQInteger sq_sendPlayerMessage(SQVM * v); // done
SQInteger sq_sendMessageAsPlayer(SQVM * v);
SQInteger sq_sendPlayerMessageToAll(SQVM * v);
SQInteger sq_setPlayerWorldBounds(SQVM * v); //done
SQInteger sq_setPlayerTime(SQVM * v); //done
SQInteger sq_getPlayerHealth(SQVM * v); //done
SQInteger sq_getPlayerArmour(SQVM * v);
SQInteger sq_getPlayerName(SQVM * v); // done
SQInteger sq_getPlayerIP(SQVM * v); // done
SQInteger sq_isConnected(SQVM * v); // done
SQInteger sq_isPlayerInVehicle(SQVM * v); // done
SQInteger sq_getPlayerVehicleID(SQVM * v); // done
SQInteger sq_setPlayerCameraPos(SQVM * v); // done
SQInteger sq_setPlayerCameraRot(SQVM * v); // done
SQInteger sq_setPlayerCameraLookAt(SQVM * v); // done
SQInteger sq_setCameraBehindPlayer(SQVM * v); // done
SQInteger sq_playSound(SQVM * v); // done
SQInteger sq_fadeScreen(SQVM * v); // done
SQInteger sq_kickPlayer(SQVM * v); // done
SQInteger sq_banIP(SQVM * v); // done
SQInteger sq_getPlayerTeam(SQVM * v); // done
SQInteger sq_getPlayerTurnSpeed(SQVM * v); // done
SQInteger sq_getPlayerMoveSpeed(SQVM * v); // done
SQInteger sq_addPlayerClass(SQVM * v); // done
SQInteger sq_forceClassSelection(SQVM * pVM);  // done
SQInteger sq_togglePlayerBleeding(SQVM * pVM);  // done
SQInteger sq_setItemFlashing(SQVM * pVM);  // done
SQInteger sq_setPlayerSkyColor(SQVM * pVM);  // done
SQInteger sq_setPlayerCash(SQVM * pVM);  // done
SQInteger sq_getPlayerCash(SQVM * pVM);  // done
SQInteger sq_setCameraShakeIntensity(SQVM * pVM);  // done
SQInteger sq_toggleCellPhone(SQVM * pVM);  // done
SQInteger sq_setPlayerGravity(SQVM * pVM);  // done
SQInteger sq_getPlayerGravity(SQVM * pVM);  // done
SQInteger sq_getPlayerSerial(SQVM * pVM); // done
SQInteger sq_isPlayerPaused(SQVM * pVM); // done