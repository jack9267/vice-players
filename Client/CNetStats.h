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

class CNetStats
{
private:

	float m_dwLastTotalBytesSent;
	float m_dwLastTotalBytesRecv;
	DWORD m_dwLastUpdateTick;
	float m_dwBPSUpload;
	float m_dwBPSDownload;

public:
	CNetStats();
	~CNetStats() {};

	void Draw();
};
