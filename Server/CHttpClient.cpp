//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

CHttpClient::CHttpClient()
{
	// If windows startup winsock
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	// Invalidate the socket handle
	m_sSocket = INVALID_SOCKET;

	ResetVars();
}

CHttpClient::~CHttpClient()
{
	// If the socket handle is valid close the socket
	if(m_sSocket != INVALID_SOCKET)
	{
		closesocket(m_sSocket);
	}

	// If windows cleanup winsock
#ifdef WIN32
	WSACleanup();
#endif
}

void CHttpClient::ResetVars()
{
	// Set the timeout amount
	m_lReceiveTimeout = 1;

	// Set the connected flag to false
	m_bConnected = false;

	// Set the status to none
	m_iStatus = STATUS_NONE;

	// Reset the header and data
	m_strHeader.clear();
	m_strData.clear();

	// Set the last error to none
	m_iLastError = ERROR_NONE;
}

void CHttpClient::SetReceiveTimeout(long lReceiveTimeout)
{
	// Set the receive timeout amount
	m_lReceiveTimeout = lReceiveTimeout;
}

long CHttpClient::GetReceiveTimeout()
{
	return m_lReceiveTimeout;
}

bool CHttpClient::IsConnected()
{
	return m_bConnected;
}

bool CHttpClient::GettingData()
{
	return (m_iStatus == STATUS_GET_DATA);
}

bool CHttpClient::GotData()
{
	return (m_iStatus == STATUS_GOT_DATA);
}

bool CHttpClient::IsInvalid()
{
	return (m_iStatus == STATUS_INVALID);
}

const char * CHttpClient::GetHeader()
{
	return m_strHeader.c_str();
}

const char * CHttpClient::GetData()
{
	return m_strData.c_str();
}

int CHttpClient::GetLastError()
{
	return m_iLastError;
}

bool CHttpClient::Connect(char * szHost, unsigned short usPort)
{
	// Prepare the socket
	m_sSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(m_sSocket == INVALID_SOCKET)
	{
		// Failed to prepare the socket, set the last error
		m_iLastError = ERROR_SOCKET_PREPARE_FAILED;
		return false;
	}

	// Get the host
	hostent * heHost = gethostbyname(szHost);

	if(heHost == NULL)
	{
		// Failed to get the host, set the last error
		m_iLastError = ERROR_INVALID_HOST;
		return false;
	}

	// Prepare a socket address
	sockaddr_in sinAddress;
	sinAddress.sin_family = AF_INET;
	sinAddress.sin_port = htons(usPort);
	sinAddress.sin_addr = *((in_addr *)heHost->h_addr);
	memset(&sinAddress.sin_zero, 0, (sizeof(CHAR) * 8));

	// Set the socket receive timeout amount
	timeval tvTimeout;
	tvTimeout.tv_sec = m_lReceiveTimeout;
	tvTimeout.tv_usec = 0;
	setsockopt(m_sSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tvTimeout, sizeof(timeval));

	// Try to connect
	if(connect(m_sSocket, (sockaddr *)&sinAddress, sizeof(sockaddr)) < 0)
	{
		// Disconnect
		Disconnect();

		// Connection failed, set the last error
		m_iLastError = ERROR_CONNECTION_FAILED;
		return false;
	}

	// Set the connected flag to true
	m_bConnected = true;

	// Set the host
	m_strHost.assign(szHost);

	// Set the port
	m_usPort = usPort;

	return true;
}

void CHttpClient::Disconnect()
{
	// Is the socket valid?
	if(m_sSocket != INVALID_SOCKET)
	{
		// Close the socket
		closesocket(m_sSocket);

		// Invalidate the socket
		m_sSocket = INVALID_SOCKET;
	}
}

bool CHttpClient::Write(char * szData, int iLen)
{
	// Try to send
	if(send(m_sSocket, szData, iLen, 0) == SOCKET_ERROR)
	{
		// Send failed, set the last error
		m_iLastError = ERROR_SEND_FAILED;
		return false;
	}

	return true;
}

int CHttpClient::Read(char * szBuffer, int iLen)
{
	// Try to receive
	return recv(m_sSocket, szBuffer, iLen, 0);
}

bool CHttpClient::Get(char * szPath, char * szUserAgent)
{
	// NOTE: After User-Agent\r\n: Referer: http://%s\r\n

	// Reset the header and data
	m_strHeader.clear();
	m_strData.clear();

	// Prepare the GET command
	char szGet[MAX_BUFFER];
	sprintf(szGet, "GET %s HTTP/1.0\r\n" \
				   "Host: %s\r\n" \
				   "User-Agent: %s\r\n\r\n", 
				   szPath, m_strHost.c_str(), szUserAgent);

	// Send the GET command
	if(!Write(szGet, strlen(szGet)))
	{
		// Send failed
		return false;
	}

	// Set the status to get data
	m_iStatus = STATUS_GET_DATA;

	return true;
}

void CHttpClient::Process()
{
	// Are we not in idle status?
	if(m_iStatus != STATUS_NONE)
	{
		switch(m_iStatus)
		{
		case STATUS_GET_DATA:
			{
				// Prepare a buffer
				char szBuffer[MAX_BUFFER];
				memset(szBuffer, 0, sizeof(szBuffer));

				// Try to read
				if(Read(szBuffer, sizeof(szBuffer)))
				{
					// Get the buffer size
					size_t sBufferSize = strlen(szBuffer);

					// Make sure we have data in the buffer
					if(!sBufferSize)
					{
						return;
					}

					// Is the header empty?
					if(m_strHeader.empty())
					{
						// Make sure we have enough data in the buffer
						if(sBufferSize < 4)
						{
							// We don't have a header, set the status
							m_iStatus = STATUS_INVALID;

							// Set the last error
							m_iLastError = ERROR_NO_HEADER;
							return;
						}

						// Get the header size from the buffer
						size_t sHeaderSize = -1;

						for(size_t i = 0; i < (strlen(szBuffer) - 4); i++)
						{
							if(szBuffer[i] == '\r' && szBuffer[i + 1] == '\n' && 
								szBuffer[i + 2] == '\r' && szBuffer[i + 3] == '\n')
							{
								sHeaderSize = (i + 4);
							}
						}

						// Check if we have a valid header size
						if(sHeaderSize == -1)
						{
							// We don't have a header, set the status
							m_iStatus = STATUS_INVALID;

							// Set the last error
							m_iLastError = ERROR_NO_HEADER;
							return;
						}

						// Set the data
						m_strData.append(szBuffer + sHeaderSize);

						// Null terminate the buffer at the end of the header
						szBuffer[sHeaderSize] = '\0';

						// Set the header
						m_strHeader.assign(szBuffer);
					}
					else
					{
						// Append the buffer directly to the data
						m_strData.append(szBuffer);
					}
				}
				else
				{
					m_iStatus = STATUS_GOT_DATA;
				}
			}
			break;
		}
	}
}
