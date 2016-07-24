//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): bpeterson
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#pragma once

class CPickup
{
private:
	DWORD		m_dwGTAId;

public:
	CPickup(int iModel, int iType, Vector3 * vecPos);
	~CPickup();
};