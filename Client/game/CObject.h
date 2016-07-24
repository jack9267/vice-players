//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): Christopher
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "CGame.h"
#include "CPhysical.h"

//-----------------------------------------------------------

class CObject : public CPhysical
{
private:
	DWORD		m_dwGTAId;

public:
	// Constructor/Destructor.	
	CObject(int iModel, Vector3 * vecPos, Vector3 * vecRot);
	~CObject();

	void SetObjectTargetable( );
	int HasObjectBeenDamaged( );
};

//-----------------------------------------------------------
