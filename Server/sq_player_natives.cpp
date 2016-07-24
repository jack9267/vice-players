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

#include "StdInc.h"

extern CNetworkManager *pNetowkManager;

using namespace RakNet;

// Player funcs //

// setPlayerHealth
SQInteger sq_setPlayerHealth(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float newHealthValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newHealthValue);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newHealthValue);
		pNetowkManager->GetRPC4()->Call("Script_SetHealth",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerArmour
SQInteger sq_setPlayerArmour(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float newArmourValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newArmourValue);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newArmourValue);
		pNetowkManager->GetRPC4()->Call("Script_SetArmour",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerPos
SQInteger sq_setPlayerPosition(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	Vector3 pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((char *)&pVec, sizeof(Vector3));
		pNetowkManager->GetRPC4()->Call("Script_SetPos",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerTurnSpeed
SQInteger sq_setPlayerTurnSpeed(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	Vector3 pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((char *)&pVec, sizeof(Vector3));
		pNetowkManager->GetRPC4()->Call("Script_SetTurnSpeed",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerMoveSpeed
SQInteger sq_setPlayerMoveSpeed(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	Vector3 pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((char *)&pVec, sizeof(Vector3));
		pNetowkManager->GetRPC4()->Call("Script_SetMoveSpeed",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerPos
SQInteger sq_getPlayerPosition(SQVM * pVM)
{
	Vector3 pos;
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		CPlayer *pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress);
		pPlayer->GetPosition(&pos);

		sq_newarray(pVM, 0);
		sq_pushfloat(pVM, pos.X);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, pos.Y);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, pos.Z);
		sq_arrayappend(pVM, -2);
		sq_push(pVM, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerZAngle
SQInteger sq_setPlayerZAngle(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	float fZAngle;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getfloat(pVM, -1, &fZAngle);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(fZAngle);
		pNetowkManager->GetRPC4()->Call("Script_SetPlayerZAngle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerSkin
SQInteger sq_getPlayerSkin(SQVM *pVM)
{
	SQInteger playerSystemAddress;
	int iSkin;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		iSkin = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetSkin();

		sq_pushinteger(pVM, iSkin);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerSkin
SQInteger sq_setPlayerSkin(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newSkinID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newSkinID);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newSkinID);
		pNetowkManager->GetRPC4()->Call("Script_SetPlayerSkin",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);
		pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->SetSpawnSkin(newSkinID);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerAction
SQInteger sq_setPlayerAction(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newActionID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newActionID);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newActionID);
		pNetowkManager->GetRPC4()->Call("Script_SetPlayerAction",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerRotation
SQInteger sq_setPlayerRotation(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float newRot;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newRot);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newRot);
		pNetowkManager->GetRPC4()->Call("Script_SetPlayerRotation",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerRotation
SQInteger sq_getPlayerRotation(SQVM *pVM)
{
	SQInteger playerSystemAddress;
	float fRot;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		fRot = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetRotation();

		sq_pushfloat(pVM, fRot);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// resetPlayerWeapons
SQInteger sq_resetPlayerWeapons(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		pNetowkManager->GetRPC4()->Call("Script_ResetWeapons",NULL,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
// setPlayerArmedWeapon
SQInteger sq_setPlayerArmedWeapon(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newWeaponID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newWeaponID);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newWeaponID);
		pNetowkManager->GetRPC4()->Call("Script_SetArmedWeapon",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// givePlayerWeapon
SQInteger sq_givePlayerWeapon(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newWeaponID;
	SQInteger Ammo;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &Ammo);
    sq_getinteger(pVM, -1, &newWeaponID);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newWeaponID);
		bsSend.Write(Ammo);
		pNetowkManager->GetRPC4()->Call("Script_GivePlayerWeapon",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// removePlayerFromVehicle
SQInteger sq_removePlayerFromVehicle(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		pNetowkManager->GetRPC4()->Call("Script_RemoveFromVehicle",NULL,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// putPlayerInVehicle
SQInteger sq_putPlayerInVehicle(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger vehID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getinteger(pVM, -1, &vehID);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(vehID);

		pNetowkManager->GetRPC4()->Call("Script_PutInVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerControls
SQInteger sq_togglePlayerControls(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger toggleValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getinteger(pVM, -1, &toggleValue);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(toggleValue);
		pNetowkManager->GetRPC4()->Call("Script_ToggleControls",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// sendPlayerMessage
SQInteger sq_sendPlayerMessage(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger colourMessage;
	const char * messageValue;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &colourMessage);
	sq_getstring(pVM, -1, &messageValue);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((DWORD)colourMessage);
		bsSend.Write(strlen(messageValue));
		bsSend.Write(messageValue,strlen(messageValue));
		pNetowkManager->GetRPC4()->Call("Script_ClientMessage",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// sendMessageAsPlayer
SQInteger sq_sendMessageAsPlayer(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	const char * Message;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getstring(pVM, -1, &Message);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(Message);
		pNetowkManager->GetRPC4()->Call("Script_sendMessageAsPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// sendPlayerMessageToAll
SQInteger sq_sendPlayerMessageToAll(SQVM * pVM)
{
	SQInteger colourMessage;
	const char * messageValue;

	sq_getinteger(pVM, -2, &colourMessage);
	sq_getstring(pVM, -1, &messageValue);

	RakNet::BitStream bsSend;
	bsSend.Write(colourMessage);
	bsSend.Write(strlen(messageValue));
	bsSend.Write(messageValue,strlen(messageValue));

	CPlayerManager *pPlayerManager = pNetowkManager->GetPlayerManager();
	for(BYTE i = 0; i < MAX_PLAYERS; i++)
	{
		if(pPlayerManager->GetSlotState(i))
		{
			pNetowkManager->GetRPC4()->Call("Script_ClientMessage",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}
	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerWorldBounds
SQInteger sq_setPlayerWorldBounds(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float LowX;
	float LowY;
	float HighX;
	float HighY;

	sq_getinteger(pVM, -5, &playerSystemAddress);
	sq_getfloat(pVM, -1, &HighY);
	sq_getfloat(pVM, -2, &HighX);
	sq_getfloat(pVM, -3, &LowY);
	sq_getfloat(pVM, -4, &LowX);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(LowX);
		bsSend.Write(LowY);
		bsSend.Write(HighX);
		bsSend.Write(HighY);
		pNetowkManager->GetRPC4()->Call("Script_WorldBounds",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerTime
SQInteger sq_setPlayerTime(SQVM * pVM)
{
	SQInteger h, m;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &h);
	sq_getinteger(pVM, -1, &m);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->SetGameTime(h, m);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerHealth
SQInteger sq_getPlayerHealth(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		int health = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetHealth();

		sq_pushinteger(pVM, health);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerArmour
SQInteger sq_getPlayerArmour(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		int armour = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetArmour();

		sq_pushinteger(pVM, armour);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerName
SQInteger sq_getPlayerName(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		const char *pName = pNetowkManager->GetPlayerManager()->GetPlayerName(playerSystemAddress);

		sq_pushstring(pVM, pName, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerIP
SQInteger sq_getPlayerIP(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		const char *ip = pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress).ToString(false);

		sq_pushstring(pVM, ip, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isConnected
SQInteger sq_isConnected(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	BOOL connected = pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress);
	sq_pushbool(pVM, connected);
	return 1;
}

// isPlayerInVehicle
SQInteger sq_isPlayerInVehicle(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		BOOL inVehicle = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->IsInVehicle();
		sq_pushbool(pVM, inVehicle);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerVehicleID
SQInteger sq_getPlayerVehicleID(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		CPlayer *pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress);
		if(pPlayer->IsInVehicle())
		{
			BYTE vehID = pPlayer->GetVehicleID();
			sq_pushinteger(pVM, vehID);
		}
		else
			sq_pushbool(pVM, false);

		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraPos
SQInteger sq_setPlayerCameraPos(SQVM * pVM)
{
	SQFloat	X, Y, Z;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		Vector3 vPos;
		vPos.X = X;
		vPos.Y = Y;
		vPos.Z = Z;
		pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->SetCameraPos(vPos);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraRot
SQInteger sq_setPlayerCameraRot(SQVM * pVM)
{
	SQFloat	X, Y, Z;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		Vector3 vRot;
		vRot.X = X;
		vRot.Y = Y;
		vRot.Z = Z;
		pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->SetCameraRot(vRot);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraLookAt
SQInteger sq_setPlayerCameraLookAt(SQVM * pVM)
{
	SQFloat	X, Y, Z;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		Vector3 vPoint;
		vPoint.X = X;
		vPoint.Y = Y;
		vPoint.Z = Z;
		pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->SetCameraLookAt(vPoint);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setCameraBehindPlayer
SQInteger sq_setCameraBehindPlayer(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->SetCameraBehindPlayer();
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// playSound
SQInteger sq_playSound(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger sound;
	SQFloat	  X, Y, Z;

	sq_getinteger(pVM, -5, &playerSystemAddress);
	sq_getinteger(pVM, -4, &sound);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(sound);
		bsSend.Write(X);
		bsSend.Write(Y);
		bsSend.Write(Z);
		pNetowkManager->GetRPC4()->Call("Script_PlaySound",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// fadeScreen
SQInteger sq_fadeScreen(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger type, time;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &type);
	sq_getinteger(pVM, -1, &time);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(type);
		bsSend.Write(time);
		pNetowkManager->GetRPC4()->Call("Script_FadeScreen",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// kickPlayer
SQInteger sq_kickPlayer(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		pNetowkManager->KickPlayer(playerSystemAddress);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// banIP
SQInteger sq_banIP(SQVM * pVM)
{
	const char * banmask;
	sq_getstring(pVM, -1, &banmask);

	pNetowkManager->AddBan((char*)banmask);
	sq_pushbool(pVM, true);
	return 1;
}


// getPlayerTeam
SQInteger sq_getPlayerTeam(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		sq_pushbool(pVM, pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetTeam());
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerTurnSpeed
SQInteger sq_getPlayerTurnSpeed(SQVM * pVM)
{
	Vector3 speed;
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		CPlayer *pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress);
		pPlayer->GetTurnSpeed(&speed);

		sq_newarray(pVM, 0);
		sq_pushfloat(pVM, speed.X);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Y);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Z);
		sq_arrayappend(pVM, -2);
		sq_push(pVM, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerMoveSpeed
SQInteger sq_getPlayerMoveSpeed(SQVM * pVM)
{
	Vector3 speed;
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		CPlayer *pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress);
		pPlayer->GetMoveSpeed(&speed);

		sq_newarray(pVM, 0);
		sq_pushfloat(pVM, speed.X);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Y);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Z);
		sq_arrayappend(pVM, -2);
		sq_push(pVM, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// addPlayerClass
SQInteger sq_addPlayerClass(SQVM * pVM)
{
	int iTeam;
	int iModel;
	int iWeapons[3];
	int iAmmo[3];
	Vector3 vecPosition;
	float fRotation;

	sq_getinteger(pVM, -12, &iTeam);
	sq_getinteger(pVM, -11, &iModel);
	sq_getfloat(pVM, -10, &vecPosition.X);
	sq_getfloat(pVM, -9, &vecPosition.Y);
	sq_getfloat(pVM, -8, &vecPosition.Z);
	sq_getfloat(pVM, -7, &fRotation);
	sq_getinteger(pVM, -6, &iWeapons[0]);
	sq_getinteger(pVM, -5, &iAmmo[0]);
	sq_getinteger(pVM, -4, &iWeapons[1]);
	sq_getinteger(pVM, -3, &iAmmo[1]);
	sq_getinteger(pVM, -2, &iWeapons[2]);
	sq_getinteger(pVM, -1, &iAmmo[2]);

	int classid = pNetowkManager->GetGameLogic()->AddPlayerClass(iTeam, iModel, vecPosition.X, vecPosition.Y, vecPosition.Z, 
		fRotation, iWeapons[0], iAmmo[0], iWeapons[1], iAmmo[1], iWeapons[2], iAmmo[2]);

	sq_pushinteger(pVM, classid);
	return 1;
}


// Added by VC-Players team.
SQInteger sq_forceClassSelection(SQVM * pVM)
{
	SQInteger playerSystemAddress;		

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		pNetowkManager->GetRPC4()->Call("Script_forceClassSelection",NULL,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//-----------------------------------------------------------------

SQInteger sq_togglePlayerBleeding(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger toggle;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &toggle);

	RakNet::BitStream bsSend;
	bsSend.Write(playerSystemAddress);
	bsSend.Write(toggle);

	for(BYTE i = 0; i < MAX_PLAYERS; i++)
	{
		if(pNetowkManager->GetPlayerManager()->GetSlotState(i))
		{
			pNetowkManager->GetRPC4()->Call("Script_togglePlayerBleeding",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}

	sq_pushbool(pVM, true);
	return 1;
}

//-------------------------------------------------------------------

SQInteger sq_toggleDriveByState(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger toggle;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &toggle);

	RakNet::BitStream bsSend;
	bsSend.Write(playerSystemAddress);
	bsSend.Write(toggle);

	for(BYTE i = 0; i < MAX_PLAYERS; i++)
	{
		if(pNetowkManager->GetPlayerManager()->GetSlotState(i))
		{
			pNetowkManager->GetRPC4()->Call("Script_toggleDriveByState",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}

	sq_pushbool(pVM, true);
	return 1;
}

//-----------------------------------------------------------------

SQInteger sq_setItemFlashing(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger item;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &item);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(item);
		pNetowkManager->GetRPC4()->Call("Script_FlashItem",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//-----------------------------------------------------------------

SQInteger sq_setPlayerSkyColor(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger iColor;
	SQInteger iType;

	sq_getinteger(pVM, -3, &playerSystemAddress);
    sq_getinteger(pVM, -2, &iColor);
	sq_getinteger(pVM, -1, &iType);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;

		bsSend.Write(iColor);
		bsSend.Write(iType);

		pNetowkManager->GetRPC4()->Call("Script_setSkyColor",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//-----------------------------------------------------------------

SQInteger sq_setPlayerCash(SQVM * pVM)
{
	SQInteger playerSystemAddr;
	SQInteger iCash;
	sq_getinteger(pVM, -2, &playerSystemAddr);
	sq_getinteger(pVM, -1, &iCash);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddr))
	{
		CPlayer * pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddr);
		pPlayer->SetCash(iCash);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//-----------------------------------------------------------------

SQInteger sq_getPlayerCash(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		int cash = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetCash();

		sq_pushinteger(pVM, cash);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}


//-------------------------------------------------------------------

SQInteger sq_toggleCellPhone(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger toggle;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getinteger(pVM, -1, &toggle);

	RakNet::BitStream bsSend;
	bsSend.Write(playerSystemAddress);
	bsSend.Write(toggle);

	for(BYTE i = 0; i < MAX_PLAYERS; i++)
	{
		if(pNetowkManager->GetPlayerManager()->GetSlotState(i))
		{
			pNetowkManager->GetRPC4()->Call("Script_toggleCellPhone",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}

	sq_pushbool(pVM, true);
	return 1;
}

//-------------------------------------------------------------------

SQInteger sq_setCameraShakeIntensity(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger iIntensity;
	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &iIntensity);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		
		bsSend.Write(iIntensity);
		
		pNetowkManager->GetRPC4()->Call("Script_SetCameraShakeIntensity",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);
		
		sq_pushbool(pVM, true);

		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

//-----------------------------------------------------------------

SQInteger sq_setPlayerGravity(SQVM * pVM)
{
	SQInteger playerSystemAddr;
	
	float iGrav;
	sq_getinteger(pVM, -2, &playerSystemAddr);
	sq_getfloat(pVM, -1, &iGrav);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddr))
	{
		CPlayer * pPlayer = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddr);
		pPlayer->SetGravity(iGrav);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_getPlayerGravity(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		float amount = pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetGravity();

		sq_pushfloat(pVM, amount);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_getPlayerSerial(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		sq_pushstring(pVM, pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->GetSerial().c_str(), -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_isPlayerPaused(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		sq_pushbool(pVM, pNetowkManager->GetPlayerManager()->GetAt(playerSystemAddress)->IsPaused());
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}