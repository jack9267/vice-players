//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"
extern CNetworkManager *pNetowkManager;

void logprintf(char * format, ...);
void fatal_exit(char * szError);

CScripts::CScripts()
{
	// reset all script slots
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		m_pScripts[i] = NULL;
	}
}

CScripts::~CScripts()
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i] != NULL) delete m_pScripts[i];
		m_pScripts[i] = NULL;
	}
}

bool CScripts::LoadScript(const char * szScriptName)
{
	// make sure a script with the same name isn't already loaded
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(!m_pScripts[i]) continue;
		if(!strcmp(m_pScripts[i]->GetScriptName(), szScriptName)) {
			// a script with the same name already exists
			return false;
		}
	}

	// find a free script slot
	int iSlot = -1;

	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(!m_pScripts[i]) {
			// found a free slot
			iSlot = i;
			break;
		}
	}

	if(iSlot == -1) {
		// no free script slot found
		return false;
	}

	if(m_pScripts[iSlot] == NULL)
	{
		m_pScripts[iSlot] = new CScript(szScriptName);
	}
	// script loaded successfully
	return true;
}

bool CScripts::UnloadScript(const char * szScriptName)
{
	// find the script slot
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(!strcmp(m_pScripts[i]->GetScriptName(), szScriptName)) {
			// found the script slot, unload the script

			delete m_pScripts[i];

			m_pScripts[i] = NULL;
			// script unloaded successfully
			return true;
		}
	}

	// script not found
	return false;
}

bool CScripts::LoadFromConfig(CConfig * pConfig)
{
	int iScriptCount = pConfig->GetConfigArrayCount("SCRIPT");
		iScriptCount++;
	int iScriptsLoaded = 0;

	for(int i = 1; i < iScriptCount; i++) {
		char * szScriptName = pConfig->GetConfigEntryAsString("SCRIPT", i);

		if(!LoadScript(szScriptName)) {
			logprintf("Failed to load script %s.", szScriptName);
		} else {
			iScriptsLoaded++;
		}
	}

	return (iScriptsLoaded > 0);
}

void CScripts::Call(const char * szFunc, int iArgCount, SQObjectPtr * pArguments)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, szFunc, -1);

			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				if(pArguments != NULL)
				{
					for(int j = 0; j < iArgCount; ++j)
						sq_pushobject(pVM, pArguments[j]);
						//pVM->Push(pArguments[j]);
				}
				sq_call(pVM, iArgCount + 1, true, true);

			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerInit()
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerInit", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerExit()
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerExit", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerPulse()
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerPulse", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerConnect(int playerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerConnect", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerDisconnect(int playerId, int reason)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerDisconnect", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the reason onto the stack
				sq_pushinteger(pVM, reason);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

bool CScripts::onPlayerText(int playerId, const char *text)
{
	bool ret = true;
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerText", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, true, true)))
				{
					SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if(result == false) ret = false;
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
	return ret;
}

void CScripts::onPlayerCommand(int playerId, const char *command)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerCommand", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the text onto the stack
				sq_pushstring(pVM, command, -1);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerSpawn(int playerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerSpawn", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerRequestClass(int playerId, int classid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerRequestClass", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the class onto the stack
				sq_pushinteger(pVM, classid);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerDeath(int playerId, int killerId, int reason)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerDeath", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the killer id onto the stack
				sq_pushinteger(pVM, killerId);

				// Push the death reason onto the stack
				sq_pushinteger(pVM, reason);

				// Call the function
				sq_call(pVM, 4, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerEnterVehicle(int playerId, int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerEnterVehicle", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerExitVehicle(int playerId, int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerExitVehicle", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

bool CScripts::onRconCommand(const char *command, const char *arg)
{
	bool found = false;
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onRconCommand", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the command onto the stack
				sq_pushstring(pVM, command, -1);

				// Push the command arguments onto the stack
				sq_pushstring(pVM, arg, -1);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, true, true)))
				{
					SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if(result) found = true;
				}

			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
	return found;
}

void CScripts::onPlayerSync(int playerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerSync", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerDamage(int playerId, int oldhp, int newhp)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerDamage", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the player old health onto the stack
				sq_pushinteger(pVM, oldhp);

				// Push the player new health onto the stack
				sq_pushinteger(pVM, newhp);

				// Call the function
				sq_call(pVM, 4, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onVehicleCreate(int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleCreate", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onVehicleDestroy(int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleDestroy", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onVehicleSync(int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleSync", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onVehicleDamage(int vehicleId, float oldhp, float newhp)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleDamage", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleId);

				// Push the player old health onto the stack
				sq_pushfloat(pVM, oldhp);

				// Push the player new health onto the stack
				sq_pushfloat(pVM, newhp);

				// Call the function
				sq_call(pVM, 4, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onVehicleSpawn(int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleSpawn", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onVehicleDeath(int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onVehicleDeath", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onBan(const char *szText)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onBan", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the ban mask onto the stack
				sq_pushstring(pVM, szText, -1);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onKick(int playerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onKick", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onTimerCreate(int timerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onTimerCreate", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the timer id onto the stack
				sq_pushinteger(pVM, timerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onTimerDestroy(int timerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onTimerDestroy", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the timer id onto the stack
				sq_pushinteger(pVM, timerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerKeyEvent(int playerId, BYTE type, char * key)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerKeyEvent", -1);

			std::string szType;
			if(type == 0)
				szType = "press";
			else if(type == 1)
				szType = "hold";
			else if(type == 2)
				szType = "release";

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, playerId);

				// push the event type
				sq_pushstring(pVM, szType.c_str(), -1);

				// Push the char of the key
				sq_pushstring(pVM, key, -1);

				// Call the function
				sq_call(pVM, 4, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}


void CScripts::onCheckpointEnter(int playerId, int cpId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onCheckpointEnter", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the checkpoint id onto the stack
				sq_pushinteger(pVM, cpId);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onCheckpointLeave(int playerId, int cpId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onCheckpointLeave", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the checkpoint id onto the stack
				sq_pushinteger(pVM, cpId);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerPause(int playerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerPause", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id id onto the stack
				sq_pushinteger(pVM, playerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerUnpause(int playerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerUnpause", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id id onto the stack
				sq_pushinteger(pVM, playerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}