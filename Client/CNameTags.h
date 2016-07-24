//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

//----------------------------------------------------

class CNameTags
{
public:
	void Draw();

	CNameTags(IDirect3DDevice8 *pD3DDevice);
	~CNameTags();

	IDirect3DDevice8	*m_pD3DDevice;
};

//----------------------------------------------------
// EOF