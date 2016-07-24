//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

#define CHAT_FONT					"Arial BOLD"
#define CHAT_FONT_SIZE				12

#define MAX_MESSAGE_LENGTH			128
#define MAX_LINE_LENGTH				MAX_MESSAGE_LENGTH / 2
#define MAX_DISP_MESSAGES			7

//----------------------------------------------------

class CChatWindow
{
private:

	BOOL				m_bEnabled;
	CHAR				m_szChatWindowBuffer[MAX_DISP_MESSAGES][MAX_MESSAGE_LENGTH];
	DWORD				m_dwChatColor[MAX_DISP_MESSAGES];

	DWORD				m_dwChatTextColor;
	DWORD				m_dwChatInfoColor;
	DWORD				m_dwChatDebugColor;
	DWORD				m_dwChatMessageColor;
	DWORD				m_dwChatBackgroundColor;

	float				m_fDrawStartX;
	float				m_fDrawStartY;
	float				m_fDrawPadding;
	SIZE				m_sizeText;
		
	void PushBack();
	void FilterInvalidChars(PCHAR szString);
	void AddToChatWindowBuffer(PCHAR szString, DWORD dwColor);
	
public:
	void Draw();
	void AddChatMessage(CHAR *szNick, DWORD dwNickColor, CHAR *szMessage);
	void AddInfoMessage(CHAR *szFormat, ...);
	void AddDebugMessage(CHAR *szFormat, ...);
	void AddClientMessage(DWORD dwColor, CHAR *szMessage);
		
	void DeleteDeviceObjects();

	void ToggleEnabled() { 
		m_bEnabled = !m_bEnabled;
	};

	CChatWindow(IDirect3DDevice8 *pD3DDevice);
	~CChatWindow();

	CD3DFont			*m_pD3DFont;
	IDirect3DDevice8	*m_pD3DDevice;
};

//----------------------------------------------------
// EOF