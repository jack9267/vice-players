//----------------------------------------------------
//
// GPL code license:
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): Christopher
//
//----------------------------------------------------------

#include "../../raknet/WindowsIncludes.h"

#include "../main.h"
#include "game.h"
#include "util.h"
#include "pools.h"
#include "keystuff.h"
#include "aimstuff.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CChatWindow   *pChatWindow;


CPlayerObjects::CPlayerObjects()
{
	m_dwGTAId = 1; // 0x001
}

CPlayerObjects::CPlayerObjects(int iModel, float fX, float fY,float fZ)
{
	m_dwGTAId=0;

	Create(iModel, fX, fY, fZ);
}

void CPlayerObjects::Create(int iModel, float fX, float fY, float fZ)
{
	ScriptCommand(&create_object, iModel, fX, fY, fZ, m_dwGTAId);
}

void CPlayerObjects::Delete()
{
	ScriptCommand(&destroy_object, m_dwGTAId);
}

void CPlayerObjects::SetObjectTargetable( )
{
	ScriptCommand(&set_target_state, m_dwGTAId);
}

int CPlayerObjects::HasObjectBeenDamaged( )
{
	int iDamage;

	ScriptCommand(&get_object_state, m_dwGTAId, iDamage);

	return iDamage;
}