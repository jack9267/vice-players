//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"
#include "../detours/detours.h"

typedef IDirect3D8 * (WINAPI * Direct3DCreate8_t)(UINT SDKVersion);
typedef HRESULT	(WINAPI * DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);

Direct3DCreate8_t  m_pfnDirect3DCreate8 = NULL;
DirectInput8Create_t m_pfnDirectInput8Create = NULL;

IDirect3D8 * WINAPI Direct3DCreate8(UINT SDKVersion)
{
	// Create our device
	IDirect3D8 * pDevice = m_pfnDirect3DCreate8(SDKVersion);

	// Create our device hook
	IDirect3D8Hook * pDeviceHook = new IDirect3D8Hook(pDevice);

	// Return the device hook pointer
	return pDeviceHook;
}

HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{
	HRESULT hr = m_pfnDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	IDirectInput8 * pDInput = (IDirectInput8 *)*ppvOut;
	*ppvOut = new IDirectInput8Hook(pDInput);
	return hr;
}

void InstallD3D8Hook()
{
	if(!m_pfnDirect3DCreate8)
	{
		m_pfnDirect3DCreate8 = (Direct3DCreate8_t)DetourFunction(DetourFindFunction("d3d8.dll", "Direct3DCreate8"), (PBYTE)Direct3DCreate8);
	}

	if(!m_pfnDirectInput8Create)
	{
		m_pfnDirectInput8Create = (DirectInput8Create_t)DetourFunction(DetourFindFunction("dinput8.dll", "DirectInput8Create"), (PBYTE)DirectInput8Create);
	}
}

void UninstallD3D8Hook()
{
	if(m_pfnDirect3DCreate8)
	{
		DetourRemove((PBYTE)m_pfnDirect3DCreate8, (PBYTE)Direct3DCreate8);
	}

	if(m_pfnDirectInput8Create)
	{
		DetourRemove((PBYTE)m_pfnDirectInput8Create, (PBYTE)DirectInput8Create);
	}
}