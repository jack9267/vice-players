//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

extern IDirect3DDevice8 *pD3DDevice;
extern CChatWindow * pChatWindow;
extern CGUI * pGUI;

CText::CText(EntityId TextID, DWORD color, char * szFontName, int iSize, float fRelativePosX, float fRelativePosY, char * szText)
{
	m_iID = TextID;
	m_dwColor = color;
	m_szText = szText;
	m_fRelativePosX = fRelativePosX;
	m_fRelativePosY = fRelativePosY;
	m_bShow = false;

	m_pFont = new CD3DFont(szFontName, iSize, 0);
	if(m_pFont)
	{
		m_pFont->InitDeviceObjects(pD3DDevice);
		m_pFont->RestoreDeviceObjects();
	}

}

CText::~CText()
{
	if(m_pFont)
	{
		m_pFont->DeleteDeviceObjects();
		delete m_pFont;
		m_pFont = NULL;
	}
}

void CText::Process()
{
	if(m_bShow)
	{
		if(!m_pFont) return;
		
		D3DDISPLAYMODE DisplayMode;
		pD3DDevice->GetDisplayMode(&DisplayMode);

		float fAbsaloutePosX = ((DisplayMode.Width / 100) * m_fRelativePosX);
		float fAbsaloutePosY = ((DisplayMode.Height / 100) * m_fRelativePosY);

		m_pFont->DrawText(fAbsaloutePosX, fAbsaloutePosY, m_dwColor, m_szText.c_str());
	}
}
