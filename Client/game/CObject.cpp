#//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): Christopher
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

extern CGame *pGame;
extern CNetworkManager *pNetowkManager;
extern CChatWindow   *pChatWindow;

CObject::CObject(int iModel, Vector3 * vecPos, Vector3 * vecRot)
{
	// Reset the object index
	m_dwGTAId = 0;

	// Create the object
	ScriptCommand(&create_object, iModel, vecPos->X, vecPos->Y, vecPos->Z, &m_dwGTAId);
	
	// Did the object creation succeed?
	if(m_dwGTAId)
	{
		// Set the entity pointer
		SetEntity((ENTITY_TYPE *)CPools::GetObjectFromIndex(m_dwGTAId));

		// Set the object rotation
		ScriptCommand(&set_object_rotation, m_dwGTAId, vecRot->X, vecRot->Y, vecRot->Z);
	}
}

CObject::~CObject()
{
	// Do we have a valid object index?
	if(m_dwGTAId != 0)
	{
		// Destroy the object
		ScriptCommand(&destroy_object, m_dwGTAId);
	}
}

void CObject::SetObjectTargetable()
{
	ScriptCommand(&set_target_state, m_dwGTAId);
}

int CObject::HasObjectBeenDamaged()
{
	int iDamage = 0;
	ScriptCommand(&get_object_state, m_dwGTAId, &iDamage);
	return iDamage;
}
