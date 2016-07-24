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
#include "../GUI/CGUI.h"

class CSpawnSelection
{
private:
	BOOL m_bClearedToSpawn;
	int m_iSelectedClass;
	DWORD m_dwLastSpawnSelectionTick; // delays left and right selection
	DWORD m_dwInitialSelectionTick; // delays initial selection
	CGUIStaticText * m_pGUIText;

public:
	CSpawnSelection();
	~CSpawnSelection() {};

	void ProcessLocalPlayer(CLocalPlayer *pLocalPlayer);
	void HandleClassSelection(CLocalPlayer *pLocalPlayer);
	void HandleClassSelectionOutcome(PLAYER_SPAWN_INFO *pSpawnInfo, BOOL bOutcome);
	void SetSelectedClass(int iSelectedClass);
};

//----------------------------------------------------------