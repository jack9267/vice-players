//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

//-----------------------------------------------------------

void GameInstallPatches();
void GameInstallHooks();

LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);

//-----------------------------------------------------------

CGame::CGame()
{
	m_pInternalCamera = new CCamera();
	m_pInternalPlayer = NULL;

	// Skip the logo and title screens
	Unprotect(0x5FFFAB, 1);
	*(BYTE *)0x5FFFAB = 0x5;

	// Nop for the menu process game state hook
	Unprotect(0x6003B3, 0xA);
	memset((PVOID)0x6003B3, 0x90, 0xA); // nop * 10

	// Reset all player ped pointer records
	InitPlayerPedPtrRecords();

	// Reset all players keys
	GameKeyStatesInit();

	// Reset all players aim
	GameAimSyncInit();

	// Set the unhandled exception filter
	SetUnhandledExceptionFilter(exc_handler);
	
	// Install all patches
	GameInstallPatches();

	// Install all hooks
	GameInstallHooks();

	m_bInputsEnabled = true;
}

//-----------------------------------------------------------

CPlayerPed *CGame::NewPlayer( int iSystemAddress, int iModel,
							  float fPosX, float fPosY, float fPosZ,
							  float fRotation )
{
	CPlayerPed *pPlayerNew = new CPlayerPed(iSystemAddress,iModel,fPosX,fPosY,fPosZ,fRotation);
	return pPlayerNew;
}

//-----------------------------------------------------------

CVehicle *CGame::NewVehicle(int iType, float fPosX, float fPosY,
							 float fPosZ, float fRotation)
{
	CVehicle *pVehicleNew = new	CVehicle(iType,fPosX,fPosY,fPosZ,fRotation);
	return pVehicleNew;
}

//-----------------------------------------------------------

int CGame::GetWeaponModelFromWeapon(int iWeaponID)
{
	return GameGetWeaponModelFromWeapon(iWeaponID);
}

//-----------------------------------------------------------

BYTE byteDisableInput[] = { 0xB8, 0x02, 0x00, 0x00, 0x00, 0xC3 };
BYTE byteEnableInput[] = { 0x8B, 0x4C, 0x24, 0x04, 0x8B, 0x54 };

void CGame::ToggleKeyInputsDisabled(BOOL bDisable)
{
	Unprotect(0x602510, 6);

	if(bDisable)
	{
		memcpy((PVOID)0x602510, byteDisableInput, 6);
	}
	else
	{
		memcpy((PVOID)0x602510, byteEnableInput, 6);
	}

}

//-----------------------------------------------------------

void GameInstallPatches()
{
	// Patch to prevent game stopping during a pause
	// (Credits to Luke)
	Unprotect(0x4A3DF7, 5);
	memset((PVOID)0x4A3DF7, 0x90, 5); // nop * 5

	// Patch for GetPlayerTableFromPlayerActor()
	Unprotect(0x531D40, 8);
	memset((PVOID)0x531D40, 0x90, 8); // nop * 8

	// Patch for SetCurrentWeapon fixes fucking FindPlayerPed shit.4FF970
	Unprotect(0x4FF970, 1);
	*(BYTE *)0x4FF970 = 0x9D; // turns mov [eax+.. to mov [ebp+..

	/* Player weapon pickups.
	Unprotect(0x4F6538, 1);
	*(BYTE *)0x4F6538 = 0x75; */

	// Patch CUserDisplay::Process() to not call CPlacename::Process()
	Unprotect(0x4D1405, 5);
	memset((PVOID)0x4D1405, 0x90, 5); // nop * 5

	// Patch CPed::RefreshSkin() so that it doesn't try to set any
	// animations 50D96A
	Unprotect(0x50D96A, 5);
	memset((PVOID)0x50D96A, 0x90, 5); // nop * 5

	// Patch clear weapons to not set armed weapon to fists.4FF767
	Unprotect(0x4FF767, 7);
	memset((PVOID)0x4FF767, 0x90, 7); // nop * 7

	/* DoDriveByShootings CWeapon::Update call. 5C9817
	VirtualProtect((PVOID)0x5C9817,8,PAGE_EXECUTE_READWRITE,&dwVP);
	memset((PVOID)0x5C9817,0x90,8); // nop * 8
	VirtualProtect((PVOID)0x5C9817,8,dwVP,&dwVP2);*/

	// Patch to modify the loadsc0 txd
	Unprotect(0x6D5E9C, 16);
	strcpy((PCHAR)0x6D5E9C, "ldvcmp0");
	Unprotect(0x68E594, 16);
	strcpy((PCHAR)0x68E594, "ldvcmp0");
	Unprotect(0x68E6F4, 16);
	strcpy((PCHAR)0x68E6F4, "ldvcmp0");

	// Patch to increase vehicle pool limit from 110 to 200
	Unprotect(0x4C02E4, 128); // 128, wtf?
	*(BYTE *)0x4C02E4 = 0x6A;
	*(BYTE *)0x4C02E5 = 0x00; // push 0 (unused param)
	*(BYTE *)0x4C02E6 = 0x68;
	*(BYTE *)0x4C02E7 = 0xC8;
	*(BYTE *)0x4C02E8 = 0x00;
	*(BYTE *)0x4C02E9 = 0x00;
	*(BYTE *)0x4C02EA = 0x00; // push 200

	// Patch for cAudioManager::GetPedCommentSfx() 5EA1FC
	Unprotect(0x5EA1FC, 1);
	*(BYTE *)0x5EA1FC = 0x75; // jnz

	/* ProcessVehicleOneShots (Reverse logic for CPed::IsPlayer())
	VirtualProtect((PVOID)0x5EB6CD,1,PAGE_EXECUTE_READWRITE,&dwVP);
	*(BYTE *)0x5EB6CD = 0x85; // jnz
	VirtualProtect((PVOID)0x5EB6CD,1,dwVP,&dwVP2);*/

	// For passenger engine audio hack (applied later).
	Unprotect(0x5F2175, 2);

	// For 537723 bug (not checking in vehicle)
	Unprotect(0x537723, 2);
	*(BYTE *)0x537723 = 0x90;
	*(BYTE *)0x537724 = 0x90;

	// For Bike/Passenger bug (test driver for 0)
	Unprotect(0x5C91F5, 4);
	*(BYTE *)0x5C91F5 = 0x85;
	*(BYTE *)0x5C91F6 = 0xC9; // test eax, eax to test ecx, ecx
	*(BYTE *)0x5C91F8 = 0x09; // jz +9 - exit if 0

	// For Boat driveby bug (test driver for 0)
	Unprotect(0x5C9558, 4);
	*(BYTE *)0x5C9558 = 0x85;
	*(BYTE *)0x5C9559 = 0xC9; // test eax, eax to test ecx, ecx
	*(BYTE *)0x5C955B = 0x09; // jz +9 - exit if 0

	// Hack to prevent replays.
	Unprotect(0x4A45C3, 5);
	memset((PVOID)0x4A45C3, 0x90, 5); // nop * 5

	// Hack to get rid of the evasive dive.
	Unprotect(0x4F6A20, 3);
	*(BYTE *)0x4F6A20 = 0xC2;
	*(BYTE *)0x4F6A21 = 0x08;
	*(BYTE *)0x4F6A22 = 0x00;

	// Hack to get rid of the weapon pickup message (Tab)
	Unprotect(0x440B2C, 5);
	memset((PVOID)0x440B2C, 0x90, 5); // nop * 5

	// Don't load the main scm
	Unprotect(0x4506D1, 2);
	*(BYTE *)0x4506D1 = 0xEB; // jmp
	*(BYTE *)0x4506D2 = 0x41; // +41h

	// Disable CPopulation::AddPed
	Unprotect(0x53B600, 3);
	*(BYTE *)0x53B600 = 0x31; // xor
	*(BYTE *)0x53B601 = 0xC0; // eax, eax
	*(BYTE *)0x53B602 = 0xC3; // retn

	// Stop time advancing on death
	Unprotect(0x42BD69, 15);
	memset((PVOID)0x42BD69, 0x90, 15); // nop * 15

	// Shorten island loading time
	Unprotect(0x40DFE4, 0xA);
	memset((PVOID)0x40DFE4, 0x90, 0xA); // nop * 10
	Unprotect(0x40DFF4, 0x38);
	memset((PVOID)0x40DFF4, 0x90, 0x38); // nop * 56

	// Disable taxi cash
	Unprotect(0x5B8AB6, 1);
	*(BYTE *)0x5B8AB6 = 0;

	// Disable golf clubs from caddys
	Unprotect(0x5B89C0, 1);
	*(BYTE *)0x5B89C0 = 0xEB;

	// Disable shotguns from police cars
	Unprotect(0x5B895F, 1);
	*(BYTE *)0x5B895F = 0xEB;

	// Disable health increases from vehicle model 146 (ambulance?)
	Unprotect(0x5B8A4B, 2);
	memset((PVOID)0x5B8A4B, 0x90, 2); // nop * 2

	// Disable armour increases from vehicle model 157 (enforcer?)
	Unprotect(0x5B8A5A, 6);
	memset((PVOID)0x5B8A5A, 0x90, 6);

	// Disable menu after alt + tab
	Unprotect(0x4A4FFC, 7);
	memset((PVOID)0x4A4FFC, 0x90, 7); // nop * 7

	// Allow camera movement in vehicles (Test)
	Unprotect(0x4A45FA, 5);
	*(DWORD *)0x4A45FA = 0x9090C030; // xor al, al, nop nop
	*(BYTE *)(0x4A45FA + 4) = 0x90;  // nop
	Unprotect(0x4A4601, 1);
	*(BYTE *)0x4A4601 = 0x75; // jz to jnz

	// Disable plane traffic
	Unprotect(0x4A4515, 5);
	memset((PVOID)0x4A4515, 0x90, 5); // nop * 5
}

//-----------------------------------------------------------

void CGame::StartGame()
{
	GameToggleCheatCodes(false);

	*(DWORD *)VAR_Menu__IsActive = 0;
	*(DWORD *)VAR_Menu__StartGame = 1;
}

//-----------------------------------------------------------

BOOL CGame::IsMenuActive()
{
	return *(BOOL *)VAR_Menu__IsActive;
}

//-----------------------------------------------------------

int CheckModel(int iModelID)
{
	if(iModelID < 0) {
		DWORD * dwUnknown = (DWORD *)0x7D1DE0;
		return dwUnknown[-7 * iModelID];
	}
	return iModelID;
}

//-----------------------------------------------------------

void CGame::RequestModel(int iModelID)
{
	DWORD dwModelID = CheckModel(iModelID);
	DWORD dwFlags = 0x16;
	DWORD dwFunc = FUNC_RequestModel;
	_asm
	{
		push dwFlags
		push dwModelID
		call dwFunc
		add esp, 8
	}
}

//-----------------------------------------------------------

void CGame::LoadRequestedModels()
{
	DWORD dwFunc = FUNC_LoadRequestedModels;
	_asm
	{
		push 0
		call dwFunc
		add esp, 4
	}
}

//------------------------------------------------------------

BOOL CGame::IsModelLoaded(int iModelID)
{
	DWORD dwModelID = CheckModel(iModelID);
	BYTE * byteModelInfo = (BYTE *)VAR_ModelInfo;
	if(byteModelInfo[20 * dwModelID] == 1) {
		return TRUE;
	} else {
		return FALSE;
	}
}

//-----------------------------------------------------------

void CGame::DisplayHud(BOOL bSwitch)
{
	if(bSwitch)
		*(BYTE *)ADDR_HUD_SWITCH = 1;
    else
		*(BYTE *)ADDR_HUD_SWITCH = 0;
}

//-----------------------------------------------------------

void CGame::ToggleHud(bool bToggle)
{
	*(bool *)VAR_Menu__HudMode = bToggle;
}

//-----------------------------------------------------------

void CGame::HideRadar(bool bToggle)
{
	*(bool *)ADDR_Toggle_Radar = bToggle;
}
	
//-----------------------------------------------------------

void CGame::ToggleFrameLimiterState(BOOL bState)
{
	if(bState) {
		*(BYTE *)VAR_Menu__FrameLimiter = 1;
	} else {
		*(BYTE *)VAR_Menu__FrameLimiter = 0;
	}
}

//-----------------------------------------------------------

BOOL CGame::GetFrameLimiterState()
{
	return *(BYTE *)VAR_Menu__FrameLimiter;
}

//-----------------------------------------------------------

void CGame::SetDrawDistance(DWORD dwDrawDistance)
{
	*(DWORD *)VAR_Menu__DrawDistance = dwDrawDistance;
}

//-----------------------------------------------------------

DWORD CGame::GetDrawDistance()
{
	return *(DWORD *)VAR_Menu__DrawDistance;
}

//-----------------------------------------------------------

void CGame::SetBrightness(DWORD dwBrightness)
{
	*(DWORD *)VAR_Menu__Brightness = dwBrightness;
}

//-----------------------------------------------------------

DWORD CGame::GetBrightness()
{
	return *(DWORD *)VAR_Menu__Brightness;
}

//-----------------------------------------------------------

void CGame::SetWidescreen(BOOL bWidescreen)
{
	if(bWidescreen) {
		*(BYTE *)VAR_Menu__Widescreen = 1;
	} else {
		*(BYTE *)VAR_Menu__Widescreen = 0;
	}
}

//-----------------------------------------------------------

BOOL CGame::GetWidescreen()
{
	return *(BYTE *)VAR_Menu__Widescreen;
}

//-----------------------------------------------------------

void CGame::DisplayTextMessage(PCHAR szText)
{

}

//-----------------------------------------------------------

void CGame::PlaySound(int iSoundId, Vector3 vPosition)
{
	ScriptCommand(&play_sound, vPosition.X, vPosition.Y, vPosition.Z, iSoundId);
}

//-----------------------------------------------------------

void CGame::FadeScreen(int iType, int iTime)
{
	ScriptCommand(&fade, iTime, iType);
}


//-----------------------------------------------------------

void CGame::SetGameTime(int iHour, int iMinute)
{
	DWORD dwFunc = CClock__SetTime;
	_asm
	{
		push iMinute
		push iHour
		call dwFunc
		add esp, 8
	}
}

//-----------------------------------------------------------

void CGame::SetCash(int Cash)
{
	*(int *)VAR_PlayerCash = Cash;
}

//-----------------------------------------------------------

void CGame::SetGravity(float fGravity)
{
	*(float *)VAR_GlobalGravity = fGravity;
}

//-----------------------------------------------------------

float CGame::GetGravity()
{
	return *(float *)VAR_GlobalGravity;
}

//-----------------------------------------------------------

int CGame::CreateMarker(int iType, float fX, float fY, float fZ, int iFlag1, int iFlag2)
{
	DWORD dwFunc = FUNC_CreateMarker;
	int iMarkerId;
	_asm
	{
		push iFlag2
		push iFlag1
		push fZ
		push fY
		push fX
		push iType
		call dwFunc
		add esp, 18h
		mov iMarkerId, eax
	}
	return iMarkerId;
}

//-----------------------------------------------------------

void CGame::ShowMarker(int iMarkerId, int iType)
{
	DWORD dwFunc = FUNC_ShowMarker;
	_asm
	{
		push iType
		push iMarkerId
		call dwFunc
		add esp, 8
	}
}

//-----------------------------------------------------------

void CGame::SetMarkerIcon(int iMarkerId, int iIconId)
{
	DWORD dwFunc = FUNC_SetMarkerIcon;
	_asm
	{
		push iIconId
		push iMarkerId
		call dwFunc
		add esp, 8
	}
}

//-----------------------------------------------------------

void CGame::SetMarkerColor(int iMarkerId, int iColorId)
{
	DWORD dwFunc = FUNC_SetMarkerColor;
	_asm
	{
		push iColorId
		push iMarkerId
		call dwFunc
		add esp, 8
	}
}

//-----------------------------------------------------------
