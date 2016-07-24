//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

// OS Dependent Includes
#ifdef WIN32
#include <windows.h>
#endif

// OS Independent Includes
#include <stdio.h>
#include <string>

// OS Dependent Defines
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "system.h"
#define SOCKET int
#define closesocket close
#endif

// OS Independent Defines
#define MAX_BUFFER 8192
#define DEFAULT_USER_AGENT "JENKSTA"

// Status codes
#define STATUS_NONE 0 // TODO: Rename to STATUS_IDLE?
#define STATUS_INVALID 1
#define STATUS_GET_DATA 2 // TODO: Rename to STATUS_GETTING_DATA?
#define STATUS_GOT_DATA 3

// Error codes
#define ERROR_NONE 0
#define ERROR_SOCKET_PREPARE_FAILED 1
#define ERROR_INVALID_HOST 2
#define ERROR_CONNECTION_FAILED 3
#define ERROR_SEND_FAILED 4
#define ERROR_NO_HEADER 5

class CHttpClient
{
private:
	SOCKET m_sSocket;
	long m_lReceiveTimeout;
	bool m_bConnected;
	std::string m_strHost;
	unsigned short m_usPort;
	int m_iStatus;
	std::string m_strHeader;
	std::string m_strData;
	int m_iLastError;

	void ResetVars();

public:
	CHttpClient();
	~CHttpClient();

	void         SetReceiveTimeout(long lReceiveTimeout);
	long         GetReceiveTimeout();
	bool         IsConnected();
	bool         IsInvalid();
	bool         GettingData();
	bool         GotData();
	const char * GetHeader();
	const char * GetData();
	int          GetLastError();
	bool         Connect(char * szHost, unsigned short usPort);
	void         Disconnect();
	bool         Write(char * szData, int iLen);
	int          Read(char * szBuffer, int iLen);
	bool         Get(char * szPath, char * szUserAgent = DEFAULT_USER_AGENT);
	void         Process();
};
