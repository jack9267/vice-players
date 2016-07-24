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

#include "StdInc.h"
#include "CEntity.h"
#include "util.h"
#include "address.h"
#include "CWorld.h"

extern CGame * pGame;

//----------------------------------------------------------

ENTITY_TYPE * CEntity::GetEntity()
{
	return m_pEntity;
}

//----------------------------------------------------------

void CEntity::SetEntity(ENTITY_TYPE * pEntity)
{
	m_pEntity = pEntity;
}

//----------------------------------------------------------

void CEntity::GetMatrix(MATRIX4X4 * matMatrix)
{
	if(m_pEntity)
	{
		memcpy(matMatrix, &m_pEntity->placeable.matMatrix, sizeof(MATRIX4X4));
	}
}

//-----------------------------------------------------------

void CEntity::SetMatrix(MATRIX4X4 matMatrix)
{
	if(m_pEntity)
	{
		memcpy(&m_pEntity->placeable.matMatrix, &matMatrix, sizeof(MATRIX4X4));
	}
}

//-----------------------------------------------------------

void CEntity::GetPosition(Vector3 * vecPosition)
{
	if(m_pEntity)
	{
		memcpy(vecPosition, &m_pEntity->placeable.matMatrix.vPos, sizeof(Vector3));
	}
}

//-----------------------------------------------------------

void CEntity::SetPosition(Vector3 vecPosition)
{
	if(m_pEntity)
	{
		memcpy(&m_pEntity->placeable.matMatrix.vPos, &vecPosition, sizeof(Vector3));
	}
}

//-----------------------------------------------------------

void CEntity::SetHeading(float fHeading)
{
	if(m_pEntity)
	{
		PLACEABLE * pPlaceable = &m_pEntity->placeable;
		DWORD dwFunc = FUNC_CPlaceable__SetHeading;
		_asm
		{
			push fHeading
			mov ecx, pPlaceable
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CEntity::Teleport(float fX, float fY, float fZ)
{
	if(m_pEntity)
	{
		ENTITY_TYPE * pEntity = m_pEntity;

		// Call Teleport
		_asm
		{
			mov ecx, pEntity
			mov ebx, [ecx]
			push fZ
			push fY
			push fX
			call [ebx+2Ch]
		}
	}
}

//-----------------------------------------------------------

WORD CEntity::GetModelIndex()
{
	if(m_pEntity)
	{
		return m_pEntity->wModelIndex;
	}

	return 0;
}

//-----------------------------------------------------------

void CEntity::SetModelIndex(WORD wModelIndex)
{
	if(m_pEntity)
	{
		// Is the model not loaded?
		if(!pGame->IsModelLoaded(wModelIndex))
		{
			// Request the model
			pGame->RequestModel(wModelIndex);

			// Load all requested models
			pGame->LoadRequestedModels();

			// Wait for the model to load
			while(!pGame->IsModelLoaded(wModelIndex))
			{
				Sleep(1);
			}
		}

		ENTITY_TYPE * pEntity = m_pEntity;

		// Call SetModelIndex
		DWORD dwModelIndex = wModelIndex;
		_asm
		{
			mov ecx, pEntity
			mov ebx, [ecx]
			push dwModelIndex
			call [ebx+0Ch]
		}
	}
}

//-----------------------------------------------------------

BOOL CEntity::IsOnScreen()
{
	if(m_pEntity)
	{
		return CWorld::IsEntityOnScreen((ENTITY_TYPE *)m_pEntity);
	}

	return FALSE;
}

//-----------------------------------------------------------

float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
	float fDistance = 0.0f;

	if(m_pEntity)
	{
		ENTITY_TYPE * pEntity = m_pEntity;
		DWORD dwFunc = FUNC_CEntity__GetDistanceFromCentreOfMassToBaseOfModel;
		_asm
		{
			mov ecx, pEntity
			call dwFunc
			fstp fDistance
		}
	}

	return fDistance;
}

//-----------------------------------------------------------
