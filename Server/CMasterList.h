//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: jenksta, adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include <string>
#include "CHttpClient.h"

class CMasterList
{
private:
	std::string m_strAddress;
	std::string m_strPostPath;
	DWORD m_dwLastPulse;

public:
	CMasterList(char * szAddress, char * szPath, int iServerPort);
	~CMasterList();
	
	void Process();
};
