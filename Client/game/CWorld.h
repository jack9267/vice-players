//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

#include "common.h"

#define FUNC_CWorld__Add 0x4DB3F0
#define FUNC_CWorld__Remove 0x4DB310
#define FUNC_CWorld__Clear 0x407090
#define FUNC_CWorld__ClearArea 0x4D38F0
#define FUNC_CWorld__FindGroundZFromCoords 0x4D5540
#define FUNC_CWorld__IsEntityOnScreen 0x4885D0
#define FUNC_CWorld__RemoveReferencesToDeletedObject 0x4D5090

class CWorld
{
public:
	static void Add(ENTITY_TYPE * pEntity);
	static void Remove(ENTITY_TYPE * pEntity);
	static void Clear();
	static void ClearArea(Vector3 * pArea, float fRadius = 4000.0f, bool bIncludeProjectilesAndParticles = true);
	static float FindGroundZFromCoords(float fX, float fY);
	static bool IsEntityOnScreen(ENTITY_TYPE * pEntity);
	static void RemoveReferencesToDeletedObject(ENTITY_TYPE * pEntity);
};
