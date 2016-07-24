//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "StdInc.h"
#include "address.h"

//-----------------------------------------------------------

CCamera::CCamera()
{
	m_pCamera = (CAMERA_TYPE *)VAR_Camera;
}

//-----------------------------------------------------------

CCamera::~CCamera()
{
	
}

//-----------------------------------------------------------

void CCamera::SetCamera(CAMERA_TYPE * pCamera)
{
	m_pCamera = pCamera;
}

//-----------------------------------------------------------

CAMERA_TYPE * CCamera::GetCamera()
{
	return m_pCamera;
}

//-----------------------------------------------------------

void CCamera::SetBehindPlayer()
{
	CAMERA_TYPE * pCamera = m_pCamera;

	if(pCamera)
	{
		DWORD dwFunc = FUNC_CCamera__PutBehindPlayer;
		_asm
		{
			mov ecx, pCamera
				call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CCamera::SetPosition(Vector3 vecPosition)
{
	if(m_pCamera)
	{
		memcpy(&m_pCamera->vecPosition, &vecPosition, sizeof(Vector3));
		SetInFreeMode(false);
	}
}

//-----------------------------------------------------------

void CCamera::SetRotation(Vector3 vecRotation)
{
	if(m_pCamera)
	{
		memcpy(&m_pCamera->vecRotation, &vecRotation, sizeof(Vector3));
		SetInFreeMode(false);
	}
}

//-----------------------------------------------------------

void CCamera::LookAtPoint(Vector3 vecPoint, int iType)
{
	CAMERA_TYPE * pCamera = m_pCamera;

	if(pCamera)
	{
		/*if(fZ < -100.0) { // min ground point
			fZ = FindGroundForZCoord
		}*/
		Vector3 * pPoint = &vecPoint;
		DWORD dwFunc = FUNC_CCamera__SetTargetPoint;
		_asm
		{
			push 1
			push iType
			push pPoint
			mov ecx, pCamera
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CCamera::Restore()
{
	CAMERA_TYPE * pCamera = m_pCamera;

	if(pCamera)
	{
		DWORD dwFunc = FUNC_CCamera__Restore;
		_asm
		{
			mov ecx, pCamera
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CCamera::SetInFreeMode(bool bFreeMode)
{
	if(m_pCamera)
	{
		m_pCamera->byteInFreeMode = bFreeMode;
	}
}

//-----------------------------------------------------------

bool CCamera::IsInFreeMode()
{
	if(m_pCamera)
	{
		return (m_pCamera->byteInFreeMode != 0);
	}

	return false;
}

//-----------------------------------------------------------

void CCamera::SetDriveByLeft(BYTE byteDriveByLeft)
{
	if(m_pCamera)
	{
		m_pCamera->byteDriveByLeft = byteDriveByLeft;
	}
}

//-----------------------------------------------------------

BYTE CCamera::GetDriveByLeft()
{
	if(m_pCamera)
	{
		return m_pCamera->byteDriveByLeft;
	}

	return 0;
}

//-----------------------------------------------------------

void CCamera::SetDriveByRight(BYTE byteDriveByRight)
{
	if(m_pCamera)
	{
		m_pCamera->byteDriveByRight = byteDriveByRight;
	}
}

//-----------------------------------------------------------

BYTE CCamera::GetDriveByRight()
{
	if(m_pCamera)
	{
		return m_pCamera->byteDriveByRight;
	}

	return 0;
}

//-----------------------------------------------------------

void CCamera::SetAim(CAMERA_AIM * pAim)
{
	if(m_pCamera)
	{
		memcpy(&m_pCamera->aim, pAim, sizeof(CAMERA_AIM));
	}
}

//-----------------------------------------------------------

CAMERA_AIM * CCamera::GetAim()
{
	if(m_pCamera)
	{
		return &m_pCamera->aim;
	}

	return NULL;
}

//-----------------------------------------------------------
