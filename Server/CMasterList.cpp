//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: jenksta
//               adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

CMasterList::CMasterList(char * szAddress, char * szPath, int iServerPort)
{
	m_strAddress = szAddress;	
	char port[5];
	itoa(iServerPort, port, 10);
	m_strPostPath = szPath;
	m_strPostPath += "/add.php?port=";
	m_strPostPath += port;
	m_dwLastPulse = 0;

	// TODO: Check that we can access the master list address?
	// TODO: Download the table for confirmation that it listed correctly?
	// TODO: If the result is 'banned' turn off the listed option?
}

CMasterList::~CMasterList()
{
	
}

void CMasterList::Process()
{
	DWORD dwTick = GetTickCount();

	if((dwTick - m_dwLastPulse) > 50000)
	{
		CHttpClient httpClient;
		httpClient.Connect((char *)m_strAddress.c_str(), 80);
		httpClient.Get((char *)m_strPostPath.c_str());
		m_dwLastPulse = dwTick;
		logprintf("Master-server query.");
	}
}
