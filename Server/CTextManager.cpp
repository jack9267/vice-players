//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): bpeterson
//				   adamix
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "StdInc.h"


extern CNetworkManager *pNetowkManager;

CTextManager::CTextManager()
{
	for(EntityId textID = 0; textID < MAX_TEXTS; textID++) 
	{
		m_bTextSlotState[textID] = FALSE;
		m_pTexts[textID] = NULL;
	}
}

CTextManager::~CTextManager()
{
	// Loop through all pickup slots
	for(EntityId i = 0; i < MAX_TEXTS; i++)
	{
		// Is this pickup slot used?
		if(m_bTextSlotState[i])
		{
			// Delete this pickup
			Delete(i);
		}
	}
}

EntityId CTextManager::New(DWORD color, const char * szFontName, int iSize, float posX, float posY, const char * szText)
{
	EntityId TextID;

	for(TextID=1; TextID <= MAX_TEXTS; TextID++)
	{
		if(m_bTextSlotState[TextID] == FALSE) break;
	}

	if(TextID == MAX_TEXTS) return -1;		

	m_pTexts[TextID] = new CText(color, szFontName, iSize, posX, posY, szText);

	if(m_pTexts[TextID])
	{
		m_pTexts[TextID]->SetID(TextID);
		m_bTextSlotState[TextID] = TRUE;
		m_pTexts[TextID]->InitForWorld();
		return TextID;
	}
	return -1;
}

void CTextManager::InitForPlayer(EntityId playerId)
{
	for(EntityId id = 0; id < MAX_TEXTS; id++)
	{
		if(m_pTexts[id] == NULL)
			continue;

		m_pTexts[id]->InitForPlayer(playerId);
	}
}

void CTextManager::InitForWorld()
{
	for(EntityId id = 0; id < MAX_TEXTS; id++)
	{
		if(m_pTexts[id] == NULL)
			continue;

		m_pTexts[id]->InitForWorld();
	}
}

EntityId CTextManager::GetFreeSlot()
{
	for(EntityId id = 0; id < MAX_TEXTS; id++)
	{
		if(m_pTexts[id] == NULL)
			return id;
	}
	return false;
}

bool CTextManager::Delete(EntityId TextID)
{
	// Is this slot used?
	if(!GetSlotState(TextID))
	{
		// It's not used
		return false;
	}

	// Delete this text
	delete m_pTexts[TextID];
	m_pTexts[TextID] = NULL;

	return true;
}
