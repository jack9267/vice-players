//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "common.h"
#include "CEntity.h"

class CPhysical : public CEntity
{
public:
	PHYSICAL_TYPE * GetPhysical();

	void GetMoveSpeed(Vector3 * vecMoveSpeed);
	void SetMoveSpeed(Vector3 vecMoveSpeed);
	void GetTurnSpeed(Vector3 * vecMoveSpeed);
	void SetTurnSpeed(Vector3 vecMoveSpeed);
};