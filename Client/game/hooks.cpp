//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

extern CNetworkManager * pNetowkManager;
extern CGame *    pGame;
BOOL              bScriptInited=FALSE;

#define NUDE void _declspec(naked) 

// TODO: hooks.h
#define VAR_CPlayerPed__VFTable 0x694D70
#define VAR_CBike__VFTable 0x6D7B34
#define VAR_CBoat__VFTable 0x69B0B4
#define VAR_CAutomobile__VFTable 0x69AD90
#define FUNC_CPlayerPed__ProcessControl 0x537270
#define FUNC_CBike__ProcessControl 0x60E3E0
#define FUNC_CBoat__ProcessControl 0x59FE90
#define FUNC_CAutomobile__ProcessControl 0x593030
#define CALLBACK_CRunningScript__Process 0x450245
#define FUNC_EnterCarAnimCallback 0x5128E0
#define FUNC_RadarTranslateColor 0x4C3050

//-----------------------------------------------------------

PED_TYPE	 * _pPlayer = NULL;
VEHICLE_TYPE * _pVehicle = NULL;
DWORD		 * _pEntity = NULL;
int			 _iWeapon = 0;
float		 _fUnk = 0;
int			 _iPedPieces = 0;
BYTE		 _byteUnk = 0;
DWORD	     dwStackFrame = 0;
DWORD	     dwCurPlayerActor = 0;
BYTE	     byteCurPlayer = 0;
DWORD        dwRGBARadar = 0;
int		     iRadarColor1 = 0;
BYTE	     byteSavedCameraMode = 0;
BYTE	   * pbyteCameraMode = (BYTE *)0x7E481C;
BYTE	   * pbyteCurrentPlayer = (BYTE *)0xA10AFB; 
BYTE	     byteUnkSave = 0;
DWORD        dwVFTable = 0;
DWORD        dwFunc = 0;
DWORD        dwObjectiveActor = 0;
DWORD        dwObjectiveType = 0;
DWORD        dwObjectiveEntity = 0;
DWORD        dwLastObjectiveVehicle = 0;
bool         bDamageDisabled = true;

extern GTA_CONTROLSET * pGcsInternalKeys;

//-----------------------------------------------------------

void _stdcall DoOnFootWorldBoundsStuff()
{
	if(pNetowkManager) {
		CLocalPlayer *pLocalPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();
		CPlayerPed *pPlayerPed = pGame->FindPlayerPed();

		// Make sure they don't leave their worldy confines.
		if(pLocalPlayer->IsActive() && !pPlayerPed->IsInVehicle()) {
			if(pPlayerPed->EnforceWorldBoundries(
				pNetowkManager->m_WorldBounds[0],pNetowkManager->m_WorldBounds[1],
				pNetowkManager->m_WorldBounds[2],pNetowkManager->m_WorldBounds[3]))
			{
				pGcsInternalKeys->wKeys1[KEY_ONFOOT_JUMP] = 0xFF;
				pGcsInternalKeys->wKeys2[KEY_ONFOOT_JUMP] = 0xFF;
				pPlayerPed->SetArmedWeapon(0);
				//pGame->DisplayTextMessage("Stay within the world boundries");
			}
		}
	}
}

//-----------------------------------------------------------

void _stdcall SwitchContext(DWORD dwPedPtr, bool bPrePost)
{
	if(dwPedPtr)
	{
		byteCurPlayer = FindPlayerNumFromPedPtr(dwPedPtr);

		// Is this player not the local player?
		if(byteCurPlayer != 0)
		{
			// Is this before the ProcessControl call or after?
			if(bPrePost)
			{
				// Key switching
				GameStoreLocalPlayerKeys(); // Save local player's keys
				GameSetRemotePlayerKeys(byteCurPlayer); // Set remote player's keys

				// Camera mode switching
				byteSavedCameraMode = *pbyteCameraMode; // Save the local player's camera mode
				*pbyteCameraMode = 4; // Set the current camera mode to onfoot mouse looking mode

				// Aim switching
				GameStoreLocalPlayerAim(); // Save local player's aim
				GameSetRemotePlayerAim(byteCurPlayer); // Set remote player's aim

				// Current player index switching
				*pbyteCurrentPlayer = byteCurPlayer; // Set the player index to the current player
			}
			else
			{
				// Current player index switching
				*pbyteCurrentPlayer = 0; // Set the player index to the local player

				// Aim switching
				GameSetLocalPlayerAim(); // Restore the local player's aim

				// Camera mode switching
				*pbyteCameraMode = byteSavedCameraMode; // Restore the local player's camera mode
					
				// Key switching
				GameSetLocalPlayerKeys(); // Restore the local player's keys
			}
		}
		else
		{
			// Its the local player
		}
	}
}

//-----------------------------------------------------------
// A hook function that switches keys for
// CPlayerPed::ProcessControl(void)

NUDE CPlayerPed__ProcessControl_Hook()
{
	_asm
	{
		mov dwCurPlayerActor, ecx
		pushad
	}

	SwitchContext(dwCurPlayerActor, true);

	// call the internal CPlayerPed::ProcessControl
	dwFunc = FUNC_CPlayerPed__ProcessControl;
	_asm
	{
		popad
		call dwFunc
		pushad
	}

	SwitchContext(dwCurPlayerActor, false);

	_asm
	{
		popad
		ret
	}
}

//-----------------------------------------------------------
// Hook for C*::ProcessControl(void)

NUDE Vehicle_ProcessControl_Hook()
{
	_asm
	{
		mov _pVehicle, ecx
		mov eax, [ecx]
		mov dwVFTable, eax
		pushad
	}
	
	if(dwVFTable == VAR_CBike__VFTable)
	{
		dwFunc = FUNC_CBike__ProcessControl;
	}
	else if(dwVFTable == VAR_CBoat__VFTable)
	{
		dwFunc = FUNC_CBoat__ProcessControl;
	}
	else if(dwVFTable == VAR_CAutomobile__VFTable)
	{
		dwFunc = FUNC_CAutomobile__ProcessControl;
	}
	else
	{
		_asm
		{
			popad
			ret
		}
	}

	if(_pVehicle)
	{
		SwitchContext((DWORD)_pVehicle->pDriver, true);
	}

	// call C*::ProcessControl
	_asm
	{
		popad
		call dwFunc
		pushad
	}

	if(_pVehicle)
	{
		SwitchContext((DWORD)_pVehicle->pDriver, false);
	}

	_asm
	{
		popad
		ret
	}
}

//-----------------------------------------------------------

void _stdcall DoVehicleEntryExitNotification(bool bEnterExit, DWORD dwVehicle, bool bPassenger)
{
	// Do we have a valid net game instance?
	if(pNetowkManager)
	{
		// Get a pointer to the local player ped
		CPlayerPed * pPlayerPed = pGame->FindPlayerPed();

		// Make sure we are not currently entering exiting a vehicle
		if((bEnterExit && dwLastObjectiveVehicle == dwVehicle && pPlayerPed->GetAction() == ACTION_GETTING_IN_VEHICLE) || (!bEnterExit && pPlayerPed->GetAction() == ACTION_EXITING_VEHICLE))
		{
			// Already entering or exiting a vehicle
			return;
		}

		// Get a pointer to the vehicle pool
		CVehicleManager * pVehicleManager = pNetowkManager->GetVehicleManager();

		// Get the vehicle's pool id
		EntityId vehicleID = pVehicleManager->FindIDFromGtaPtr((VEHICLE_TYPE *)dwVehicle);

		// Is the vehicle valid?
		if(vehicleID != INVALID_ENTITY_ID)
		{
			// Get a pointer to the local player
			CLocalPlayer * pLocalPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();

			// Are we entering the vehicle or exiting it?
			if(bEnterExit)
			{
				// Send the enter vehicle notification
				pLocalPlayer->SendEnterVehicleNotification(vehicleID, bPassenger);

				// Set the last objective vehicle pointer to this vehicle to avoid spamming enter vehicle objectives
				dwLastObjectiveVehicle = dwVehicle;
			}
			else
			{
				// Send the exit vehicle notification
				pLocalPlayer->SendExitVehicleNotification(vehicleID);
			}
		}
	}
}

//-----------------------------------------------------------
// Hooks CPed::SetObjective(eObjectiveType objectiveType, CEntity * pObjectiveEntity)

NUDE CPed__SetObjective_Hook()
{	
	_asm
	{
		mov dwObjectiveActor, ecx
		mov eax, [esp+4]
		mov dwObjectiveType, eax
		mov eax, [esp+8]
		mov dwObjectiveEntity, eax
		pushad
	}

	// TODO: If entering vehicle and vehicle is not in pool don't let them enter it
	if((PED_TYPE *)dwObjectiveActor == GamePool_FindPlayerPed())
	{
		if(dwObjectiveType == OBJECTIVE_TYPE_ENTER_CAR_AS_DRIVER)
		{
			DoVehicleEntryExitNotification(true, dwObjectiveEntity, false);
		}
		else if(dwObjectiveType == OBJECTIVE_TYPE_ENTER_CAR_AS_PASSENGER)
		{
			DoVehicleEntryExitNotification(true, dwObjectiveEntity, true);
		}
		else if(dwObjectiveType == OBJECTIVE_TYPE_EXIT_CAR)
		{
			DoVehicleEntryExitNotification(false, dwObjectiveEntity, false);
		}
	}

	dwFunc = (FUNC_CPed__SetObjective + 9);
	_asm
	{
		popad
		push ebx
		mov ebx, ecx
		mov eax, [ebx+244h]
		jmp dwFunc
	}
}

//-----------------------------------------------------------

NUDE RadarTranslateColor()
{
	_asm
	{
		mov eax, [esp+4]
		mov iRadarColor1, eax
	}

	TranslateColorCodeToRGBA(iRadarColor1); // return will still be in eax.

	_asm
	{
		ret
	}
}

//-----------------------------------------------------------
// ok, this bullshit procedure don't check the fucking
// vehicle pointer for 0 and caused the widely hated 5128fb crash.

NUDE EnterCarAnimCallback_Hook()
{
	_asm
	{
		mov _pPlayer, eax
		pushad
	}

	if(_pPlayer->pVehicle == 0 && _pPlayer != GamePool_FindPlayerPed())
	{
		_asm
		{
			popad
			ret
		}
	}

	dwFunc = (FUNC_EnterCarAnimCallback + 8);
	_asm
	{
		popad
		mov edx, [esp+4]
		mov eax, [esp+8]
		jmp dwFunc
	}
}

//-----------------------------------------------------------

#define NO_TEAM 255

BOOL _stdcall IsFriendlyFire(PED_TYPE *pPlayer,DWORD *pdwEnt, int iWeapon, float fUnk, int PedPiece, BYTE byteUnk)
{
	CPlayerManager *pPlayerManager;
	CVehicleManager *pVehicleManager;
	CRemotePlayer *pRemotePlayer;
	CVehicle *pVehicle;
	CLocalPlayer *pLocalPlayer;
	EntityId remotePlayerID;
	BYTE byteLocalPlayerTeam;

	if(pPlayer == GamePool_FindPlayerPed()) {
		if(pNetowkManager && pNetowkManager->GetFriendlyFire()) {
			pPlayerManager = pNetowkManager->GetPlayerManager();
			pLocalPlayer = pPlayerManager->GetLocalPlayer();
			byteLocalPlayerTeam = pLocalPlayer->GetTeam();

			if(pLocalPlayer->IsWasted() || (byteLocalPlayerTeam == NO_TEAM)) return FALSE;

			remotePlayerID = pPlayerManager->FindPlayerIDFromGtaPtr((PED_TYPE *)pdwEnt);

			if(remotePlayerID != INVALID_ENTITY_ID) {
				pRemotePlayer = pPlayerManager->GetAt(remotePlayerID);
				if(pRemotePlayer->GetTeam() == byteLocalPlayerTeam) {
					return TRUE;
				} else {
					return FALSE;
				}
			}

			pVehicleManager = pNetowkManager->GetVehicleManager();

			EntityId vehicleID = pVehicleManager->FindIDFromGtaPtr((VEHICLE_TYPE *)pdwEnt);

			if(vehicleID != INVALID_ENTITY_ID) {
				pVehicle = pVehicleManager->GetAt(vehicleID);
				if(pVehicle->GetDriver()) {
					remotePlayerID = pPlayerManager->FindPlayerIDFromGtaPtr((PED_TYPE *)pdwEnt);
					if(remotePlayerID != INVALID_ENTITY_ID) {
						pRemotePlayer = pPlayerManager->GetAt(remotePlayerID);
						if(pRemotePlayer->GetTeam() == byteLocalPlayerTeam) {
							return TRUE;
						}
					}
				} else {
					return FALSE;
				}
			}
		}
	}

	return FALSE;	
}

//-----------------------------------------------------------

//extern CChatWindow * pChatWindow;

#ifdef DAMAGE_SYNC
// NOTE: Only problem i can see with this is getting damaged by entitys that have moved remotely since you were damaged
void DoInflictedDamageNotification(DWORD * pdwEntity, int iWeapon, float fUnk, int iPedPieces, BYTE byteUnk)
{
	// Get the player pool pointer
	CPlayerManager * pPlayerManager = pNetowkManager->GetPlayerManager();

	// Get the vehicle pool pointer
	CVehicleManager * pVehicleManager = pNetowkManager->GetVehicleManager();

	// Try and get a player id from the entity pointer
	EntityId playerID = pPlayerManager->FindPlayerIDFromGtaPtr((PED_TYPE *)pdwEntity);

	// Try and get a vehicle id from the entity pointer
	//EntityId vehicleID = pVehicleManager->FindIDFromGtaPtr((VEHICLE_TYPE *)pdwEntity);
	EntityId vehicleID = INVALID_ENTITY_ID; // was causing problems, disabled

	// Is the damager ourselves?
	if(playerID == INVALID_ENTITY_ID && vehicleID == INVALID_ENTITY_ID && (PED_TYPE *)pdwEntity == GamePool_FindPlayerPed())
	{
		playerID = pPlayerManager->GetLocalPlayerID();
	}

	//pChatWindow->AddDebugMessage("DoInflictedDamageNotification(0x%p (LPID: %d, LPP: 0x%p, Player: %d, Vehicle: %d), %d, %f, %d, %d)", pdwEntity, pPlayerManager->GetLocalPlayerID(), GamePool_FindPlayerPed(), playerID, vehicleID, iWeapon, fUnk, iPedPieces, byteUnk);

	// Do we have a valid player or vehicle id?
	if(playerID != INVALID_ENTITY_ID || vehicleID != INVALID_ENTITY_ID)
	{
		// Get a pointer to the local player
		CLocalPlayer * pLocalPlayer = pNetowkManager->GetPlayerManager()->GetLocalPlayer();

		// Send the inflicted damage notification
		pLocalPlayer->SendInflictedDamageNotification(playerID, vehicleID, iWeapon, fUnk, iPedPieces, byteUnk);
	}
}

//-----------------------------------------------------------

bool bSelf = false;
DWORD dwRet = 0;
#endif

NUDE CPed__InflictDamage_Hook()
{
	_asm
	{
		mov dwStackFrame, esp
#ifdef DAMAGE_SYNC
		mov eax, [esp]
		mov dwRet, eax
#endif
		mov _pPlayer, ecx
		mov eax, [esp+4]
		mov _pEntity, eax
		mov eax, [esp+8]
		mov _iWeapon, eax
		mov eax, [esp+12]
		mov _fUnk, eax
		mov eax, [esp+16]
		mov _iPedPieces, eax
		mov al, [esp+20]
		mov _byteUnk, al
		pushad
	}

	// TODO: Move this to DoInflictedDamageNotification then if DoInflictedDamageNotification returns false
	// cancel the damage infliction
	if(pNetowkManager)
	{
#ifdef DAMAGE_SYNC
		// Get weather or not the inflicted player is ourself
		bSelf = (_pPlayer == GamePool_FindPlayerPed());
#endif

		// If its ourselves and friendly fire or not ourselves and damage is disabled, don't inflict the damage
#ifdef DAMAGE_SYNC
		if((bSelf && IsFriendlyFire(_pPlayer, _pEntity, _iWeapon, _fUnk, _iPedPieces, _byteUnk)) || (!bSelf && bDamageDisabled))
#else
		if(IsFriendlyFire(_pPlayer, _pEntity, _iWeapon, _fUnk, _iPedPieces, _byteUnk))
#endif
		{
			//pChatWindow->AddDebugMessage("Not inflicting damage (ret: 0x%p, wep: %d, self?: %d, disabled?: %d)", dwRet, _iWeapon, bSelf, bDamageDisabled);
			_asm
			{
				popad
				mov esp, dwStackFrame
				xor al, al
				retn 14h
			}
		}
#ifdef DAMAGE_SYNC
		else
		{
			// Is it ourself?
			if(bSelf)
			{
				// Its ourself, notify the server of the inflicted damage
				DoInflictedDamageNotification(_pEntity, _iWeapon, _fUnk, _iPedPieces, _byteUnk);
			}
		}
#endif
	}

	dwFunc = (FUNC_CPed__InflictDamage + 6);
	_asm
	{
		popad
		mov esp, dwStackFrame
		fld ds:[0x694170]
		jmp dwFunc
	}
}

//-----------------------------------------------------------

void CRunningScript__Process()
{
	if(!bScriptInited)
	{
		// Code from VCMP.SCM (Minus the pickups and teleports)
		DWORD PLAYER_ACTOR = 0;
		DWORD PLAYER_CHAR = 0;
		int iPlayerNumber = 0;
		ScriptCommand(&name_thread, "MAIN");
		ScriptCommand(&set_current_time, 12, 0);
		ScriptCommand(&create_player, &iPlayerNumber, -1000.0, 195.5, 11.0, &PLAYER_ACTOR);
		ScriptCommand(&create_actor_from_player, &PLAYER_ACTOR, &PLAYER_CHAR);
		ScriptCommand(&create_forbidden_for_peds_cube, -100000.0f, -100000.0f, -100000.0f, 100000.0f, 100000.0f, 100000.0f);
		ScriptCommand(&create_forbidden_for_cars_cube, -100000.0f, -100000.0f, -100000.0f, 100000.0f, 100000.0f, 100000.0f);
		ScriptCommand(&set_max_wanted_level, 0);
		ScriptCommand(&toggle_player_controllable, PLAYER_ACTOR, 1);
		ScriptCommand(&force_weather, 0);
		ScriptCommand(&fade, 0, 0);
		bScriptInited = TRUE;
	}
}

//-----------------------------------------------------------

NUDE CRunningScript__Process_Hook()
{
	_asm
	{
		pushad
	}

	CRunningScript__Process();

	_asm
	{
		popad
		retn
	}
}

//-----------------------------------------------------------

void StartGame()
{
	pGame->StartGame();
	pGame->SetGameState(GS_MENUPROCESS);
}

//-----------------------------------------------------------

void InstallMethodHook(DWORD dwInstallAddress,DWORD dwHookFunction)
{
	Unprotect(dwInstallAddress, 4);
	*(DWORD *)dwInstallAddress = (DWORD)dwHookFunction;
}

//-----------------------------------------------------------

void InstallHook(DWORD dwInstallAddress, DWORD dwHookFunction, DWORD dwHookStorage, BYTE * pbyteJmpCode, int iJmpCodeSize)
{
	// Install the pointer to procaddr.
	Unprotect(dwHookStorage, 4);
	*(PDWORD)dwHookStorage = (DWORD)dwHookFunction;

	// Install the Jmp code.
	Unprotect(dwInstallAddress, iJmpCodeSize);
	memcpy((PVOID)dwInstallAddress, pbyteJmpCode, iJmpCodeSize);
}

//-----------------------------------------------------------

void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction)
{
	Unprotect(dwInstallAddress, 5);
	*(BYTE *)dwInstallAddress = 0xE8;
	*(DWORD *)(dwInstallAddress + 1) = (dwHookFunction - (dwInstallAddress + 5));
}

//-----------------------------------------------------------

void InstallJmpHook(DWORD dwInstallAddress, DWORD dwHookFunction)
{
	Unprotect(dwInstallAddress, 5);
	*(BYTE *)dwInstallAddress = 0xE9;
	*(DWORD *)(dwInstallAddress + 1) = (dwHookFunction - (dwInstallAddress + 5));
}

//-----------------------------------------------------------

void GameInstallHooks()
{	
	// Install hook for CPlayerPed::ProcessControl
	InstallMethodHook((VAR_CPlayerPed__VFTable + 0x20), (DWORD)CPlayerPed__ProcessControl_Hook);

	// Install hook for CBike::ProcessControl
	InstallMethodHook((VAR_CBike__VFTable + 0x20), (DWORD)Vehicle_ProcessControl_Hook);

	// Install hook for CBoat::ProcessControl
	InstallMethodHook((VAR_CBoat__VFTable + 0x20), (DWORD)Vehicle_ProcessControl_Hook);

	// Install hook for CAutomobile::ProcessControl
	InstallMethodHook((VAR_CAutomobile__VFTable + 0x20), (DWORD)Vehicle_ProcessControl_Hook);

	// Install hook for CPed::SetObjective
	InstallJmpHook(FUNC_CPed__SetObjective, (DWORD)CPed__SetObjective_Hook);
								
	// Install hook for RadarTranslateColor
	InstallJmpHook(FUNC_RadarTranslateColor, (DWORD)RadarTranslateColor);
	
	// Install hook for CPed::InflictDamage
	InstallJmpHook(FUNC_CPed__InflictDamage, (DWORD)CPed__InflictDamage_Hook);

	// Install hook for EnterCarAnimCallback
	// Update: Causing even more problems.
	InstallJmpHook(FUNC_EnterCarAnimCallback, (DWORD)EnterCarAnimCallback_Hook);

	// Install hook for CRunningScript::Process (thx Merlin)
	InstallCallHook(CALLBACK_CRunningScript__Process, (DWORD)CRunningScript__Process_Hook);

	// Install hook for menu process game state setting
	InstallCallHook(0x6003B3, (DWORD)StartGame);
}

//-----------------------------------------------------------
