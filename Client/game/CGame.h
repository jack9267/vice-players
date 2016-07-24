//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "../main.h"
#include "address.h"
#include "common.h"
#include "CVehicle.h"
#include "CPlayerPed.h"
#include "CCamera.h"
#include "scripting.h"
#include "CText.h"

//-----------------------------------------------------------

#define FUNC_RequestModel 0x40E310
#define FUNC_LoadRequestedModels 0x40B5F0
#define VAR_ModelInfo 0x94DDD8
#define CClock__SetTime 0x487160
#define VAR_PlayerCash 0x94ADC8 // This is actually CPlayerInfo[0].Cash
#define FUNC_CreateMarker 0x4C3C80
#define FUNC_ShowMarker 0x4C3840
#define FUNC_SetMarkerIcon 0x4C3780
#define FUNC_SetMarkerColor 0x4C3930
#define VAR_GameState 0x9B5F08
#define VAR_GlobalGravity 0x68F5F0

enum eGameState
{
	GS_START,
	GS_SHOWLOGO,
	GS_LOGOPROCESS,
	GS_SHOWTITLES,
	GS_TITLESPROCESS,
	GS_PREINITGAME,
	GS_SELVIDEOMODE,
	GS_MENUPROCESS,
	GS_LOADGAME,
	GS_SHUTDOWN,
};

//-----------------------------------------------------------


class CGame
{
public:
	

	CPlayerPed *NewPlayer(int iSystemAddress, int iModel,float fPosX,float fPosY,float fPosZ,float fRotation);
	CVehicle *NewVehicle(int iType,float fPosX,float fPosY,float fPosZ,float fRotation);
		
	int		GetWeaponModelFromWeapon(int iWeaponID);
	void	ToggleKeyInputsDisabled(BOOL bDisable);
	void	SetInputsEnabled(bool bEnabled) { m_bInputsEnabled = bEnabled; };
	bool	IsInputsEnabled() { return m_bInputsEnabled; };
	void	StartGame();
	BOOL	IsMenuActive();
	void	RequestModel(int iModelID);
	void	LoadRequestedModels();
	BOOL	IsModelLoaded(int iModelID);
	void	DisplayHud(BOOL bSwitch);
	void	ToggleHud(bool bToggle);
	void	HideRadar(bool bToggle);
	void	ToggleFrameLimiterState(BOOL bState);
	BOOL    GetFrameLimiterState();
	void    SetDrawDistance(DWORD dwDrawDistance);
	DWORD   GetDrawDistance();
	void    SetBrightness(DWORD dwBrightness);
	DWORD   GetBrightness();
	void    SetWidescreen(BOOL bWidescreen);
	BOOL    GetWidescreen();
	void	DisplayTextMessage(PCHAR szText);
	void    PlaySound(int iSoundId, Vector3 vPosition);
	void    FadeScreen(int iType, int iTime);
	void	SetGameTime(int iHour, int iMinute);
	void	SetCash(int Cash);
	void	SetGravity(float fGravity);
	float	GetGravity();
	int     CreateMarker(int iType, float fX, float fY, float fZ, int iFlag1, int iFlag2);
	void    ShowMarker(int iMarkerId, int iType);
	void    SetMarkerIcon(int iMarkerId, int iIconId);
	void    SetMarkerColor(int iMarkerId, int iColorId);

	DWORD	GetD3DDevice() { return *(DWORD *)ADDR_ID3D8DEVICE; };
	void	SetD3DDevice(DWORD pD3DDevice) { *(DWORD *)ADDR_ID3D8DEVICE = pD3DDevice; };
	DWORD	GetD3D() { return *(DWORD *)ADDR_ID3D8DEVICE; };
	void	SetD3D(DWORD pD3D) {	*(DWORD *)ADDR_ID3D8 = pD3D; };
	HWND	GetMainWindowHwnd() { return *(HWND *)ADDR_HWND; };

	//-----------------------------------------------------------

	CPlayerPed  *FindPlayerPed() {
		if(m_pInternalPlayer==NULL)	m_pInternalPlayer = new CPlayerPed();
		return m_pInternalPlayer;
	};
	
	CCamera     *GetCamera() {	return m_pInternalCamera; };

	//-----------------------------------------------------------

	void EnablePassengerEngineAudio() {
		*(BYTE *)0x5F2175 = 0x3B;
		*(BYTE *)0x5F2176 = 0xC2;
	};

	void DisablePassengerEngineAudio() {
		*(BYTE *)0x5F2175 = 0x39;
		*(BYTE *)0x5F2176 = 0xF0;
	};

	//-----------------------------------------------------------

	eGameState GetGameState() {
		return *(eGameState *)VAR_GameState;
	}

	void SetGameState(eGameState state) {
		*(eGameState *)VAR_GameState = state;
	}

	//-----------------------------------------------------------

	CGame();
	~CGame() {};

private:

	CCamera			*m_pInternalCamera;
	CPlayerPed		*m_pInternalPlayer;

	bool			m_bInputsEnabled;
};

//-----------------------------------------------------------
