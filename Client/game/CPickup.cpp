//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): bpeterson
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

CPickup::CPickup(int iModel, int iType, Vector3 * vecPos)
{
	// Reset the pickup index
	m_dwGTAId = 0;

	// Create the pickup
	ScriptCommand(&create_pickup, iModel, iType, vecPos->X, vecPos->Y, vecPos->Z, &m_dwGTAId);
}

CPickup::~CPickup()
{
	// Do we have a valid pickup index?
	if(m_dwGTAId != 0)
	{
		// Destroy the pickup
		ScriptCommand(&destroy_pickup, m_dwGTAId);
	}
}