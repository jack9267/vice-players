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

#pragma once

#include "CGame.h"

//-----------------------------------------------------------

#define VAR_Camera								0x7E4688
#define FUNC_CCamera__Restore					0x46BC7D
#define FUNC_CCamera__PutBehindPlayer			0x46BADE
#define FUNC_CCamera__SetTargetPoint			0x46A494
#define FUNC_CCamera__SetPositionAndRotation	0x46BA72

//-----------------------------------------------------------

class CCamera
{
private:
	CAMERA_TYPE * m_pCamera;

public:
	CCamera();
	~CCamera();

	void          SetCamera(CAMERA_TYPE * pCamera);
	CAMERA_TYPE * GetCamera();
	void          SetBehindPlayer();
	void          SetPosition(Vector3 vecPosition);
	void          SetRotation(Vector3 vecRotation);
	void          LookAtPoint(Vector3 vecPoint, int iType);
	void          Restore();
	void          SetInFreeMode(bool bFreeMode);
	bool          IsInFreeMode();
	void          SetDriveByLeft(BYTE byteDriveByLeft);
	BYTE          GetDriveByLeft();
	void          SetDriveByRight(BYTE byteDriveByRight);
	BYTE          GetDriveByRight();
	void          SetAim(CAMERA_AIM * pAim);
	CAMERA_AIM *  GetAim();
};

//-----------------------------------------------------------
