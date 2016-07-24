//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): adamix
// License: See LICENSE in root directory
//
//----------------------------------------------------------
#pragma once

#define MAX_OBJECTS 100

class CObjectManager
{
public:
	CObjectManager();
	~CObjectManager();

	BOOL New(EntityId ObjectID, int iModel, Vector3 vecPos, Vector3 vecRot);
	BOOL Delete(EntityId ObjectID);
private:
	BOOL m_bObjectSlotState[MAX_OBJECTS];
	CObject * m_pObjects[MAX_OBJECTS];
};