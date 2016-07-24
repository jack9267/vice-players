//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#pragma once
#include "../../vendor/squirrel/squirrel.h"
#include "../../vendor/squirrel/sqvm.h"
#include "CScript.h"
#include "../CTimerPool.h"

#define MAX_SCRIPTS 10

class CScripts
{
private:
	CScript * m_pScripts[MAX_SCRIPTS];
	char m_szScriptNames[MAX_SCRIPTS][256];

	CTimerPool * m_pTimerPool;

public:
	CScripts();
	~CScripts();

	int LoadScript(const char * szScriptName);
	bool UnloadScript(const char * szScriptName);

	CTimerPool * GetTimerPool() { return  m_pTimerPool; };

	CScript * GetScript(int scriptID) { 
		if(!m_pScripts[scriptID]) return NULL;
		return m_pScripts[scriptID]; 
	};

	void Call(const char * szFunc, int iArgCount, SQObjectPtr * pArguments);
	void Call(RakNet::BitStream * bitStream);

	void onInit(int iScript);
	void onExit();
	void onPulse();
	void onRender();
/*	bool onRconCommand(const char *command, const char *arg);
	void onBan(const char *szText);
	void onKick(int playerId);

	void onPlayerConnect(int playerId);
	void onPlayerDisconnect(int playerId, int reason);
	bool onPlayerText(int playerId, const char *text);
	void onPlayerCommand(int playerId, const char *command);
	void onPlayerSpawn(int playerId);
	void onPlayerRequestClass(int playerId, int classid);
	void onPlayerDeath(int playerId, int killerId, int reason);
	void onPlayerEnterVehicle(int playerId, int vehicleid);
	void onPlayerExitVehicle(int playerId, int vehicleid);
	void onPlayerSync(int playerId);
	void onPlayerDamage(int playerId, int oldhp, int newhp);

	void onVehicleCreate(int vehicleid);
	void onVehicleDestroy(int vehicleid);
	void onVehicleSync(int vehicleid);
	void onVehicleDamage(int vehicleId, float oldhp, float newhp);
	void onVehicleSpawn(int vehicleid);
	void onVehicleDeath(int vehicleid);
*/
	void onTimerCreate(int timerId);
	void onTimerDestroy(int timerId);
};