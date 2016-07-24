//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"

void CWorld::Add(ENTITY_TYPE * pEntity)
{
	DWORD dwFunc = FUNC_CWorld__Add;
	_asm
	{
		push pEntity
		call dwFunc
		add esp, 4
	}
}

void CWorld::Remove(ENTITY_TYPE * pEntity)
{
	DWORD dwFunc = FUNC_CWorld__Remove;
	_asm
	{
		push pEntity
		call dwFunc
		add esp, 4
	}
}

void CWorld::Clear()
{
	DWORD dwFunc = FUNC_CWorld__Clear;
	_asm
	{
		call dwFunc
	}
}

void ClearArea(Vector3 * pArea, float fRadius, bool bIncludeProjectilesAndParticles)
{
	DWORD dwFunc = FUNC_CWorld__ClearArea;
	_asm
	{
		push bIncludeProjectilesAndParticles
		push fRadius
		push pArea
		call dwFunc
		add esp, 0Ch
	}
}

float CWorld::FindGroundZFromCoords(float fX, float fY)
{
	DWORD dwFunc = FUNC_CWorld__FindGroundZFromCoords;
	float fZ = 0.0f;
	_asm
	{
		push fY
		push fX
		call dwFunc
		fstp fZ
	}
	return fZ;
}

bool CWorld::IsEntityOnScreen(ENTITY_TYPE * pEntity)
{
	DWORD dwFunc = FUNC_CWorld__IsEntityOnScreen;
	bool bReturn = false;
	_asm
	{
		push pEntity
		call dwFunc
		mov bReturn, al
		add esp, 4
	}
	return bReturn;
}

void CWorld::RemoveReferencesToDeletedObject(ENTITY_TYPE * pEntity)
{
	DWORD dwFunc = FUNC_CWorld__RemoveReferencesToDeletedObject;
	_asm
	{
		push pEntity
		call dwFunc
		add esp, 4
	}
}
