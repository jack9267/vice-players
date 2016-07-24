//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

//#pragma twice <- that's a joke btw.

#pragma once

#define MAX_CMD_INPUT   64
#define MAX_CMDS		128
#define MAX_CMD_STRLEN  32
#define CMD_CHARACTER   '/'

typedef void (__cdecl *CMDPROC)(PCHAR);

//----------------------------------------------------

class CCmdWindow
{
private:

	CHAR		m_szInputBuffer[MAX_CMD_INPUT+1];
	BOOL		m_bEnabled;
	float		m_fDrawStartX;
	float		m_fDrawStartY;
	float		m_fDrawPadding;
	DWORD		m_dwDrawColor;
	DWORD		m_dwBackgroundColor;
	CD3DFont	*m_pD3DFont;
	IDirect3DDevice8 *m_pD3DDevice;

	// Command procedure stuff.
	CMDPROC		m_pDefaultCmd;	 // used when no command specifier was
								 // used (ie. a normal chat message)
	CMDPROC		m_pCmds[MAX_CMDS];
	CHAR        m_szCmdNames[MAX_CMDS][MAX_CMD_STRLEN+1];
	int			m_iCmdCount;

public:

	void Draw();

	void Enable();
	void Disable();
	BOOL IsEnabled() { return m_bEnabled; };
	void ToggleEnabled() {
		if(m_bEnabled) {
			Disable();
		} else {
			Enable();
		}
	}
	
	void DeleteDeviceObjects();

	void AddChar(char Char);
	void BackSpace();
	void ProcessInput();

	CMDPROC GetCmdHandler(PCHAR szCmdName);
	void AddDefaultCmdProc(CMDPROC cmdDefault);
	void AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler);

	CCmdWindow(IDirect3DDevice8 *pD3DDevice);
	~CCmdWindow();
};

//----------------------------------------------------
// EOF