//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

#ifndef WIN32
#	define stricmp strcasecmp
#endif

//----------------------------------------------------------

CConfig::~CConfig()
{
	int i=0;
	while(i != iNumEntries) {
		free(m_szDirectiveName[i]);
		free(m_szDirectiveData[i]);
		i++;
	}
}

//----------------------------------------------------------

int CConfig::GetConfigArrayCount(char *szDirectiveName)
{
	int i=1;
	while(GetConfigEntryAsString(szDirectiveName,i) != NULL) i++;
	return (i - 1);
}

//----------------------------------------------------------

int CConfig::GetConfigEntryAsInt(char *szDirectiveName, int iArrayIndex)
{
	int i=0;
	char szSearchName[MAX_CONFIG_STRSIZE];
	int iCount=1;

	strcpy(szSearchName,szDirectiveName);

	while(i != iNumEntries) {
		if(!StringCompI(szSearchName,m_szDirectiveName[i])) {
			if(iArrayIndex == 0 || iCount == iArrayIndex) {
				return atoi(m_szDirectiveData[i]);
			}
			iCount++;
		}
		i++;
	}
	return (-1);
}

//----------------------------------------------------------

char * CConfig::GetConfigEntryAsString(char *szDirectiveName, int iArrayIndex)
{
	int i=0;
	char szSearchName[MAX_CONFIG_STRSIZE];
	int iCount=1;

	strcpy(szSearchName,szDirectiveName);

	while(i != iNumEntries) {
		if(!StringCompI(szSearchName,m_szDirectiveName[i])) {
			if(iArrayIndex == 0 || iCount == iArrayIndex) {
				return m_szDirectiveData[i];
			}
			iCount++;
		}
		i++;
	}
	return NULL;
}

//----------------------------------------------------------

int CConfig::GetConfigEntryAsBool(char *szDirectiveName, int iArrayIndex)
{
	int i=0;
	char szSearchName[MAX_CONFIG_STRSIZE];
	int iCount=1;

	strcpy(szSearchName,szDirectiveName);

	while(i != iNumEntries) {
		if(!StringCompI(szSearchName,m_szDirectiveName[i])) {
			if(iArrayIndex == 0 || iCount == iArrayIndex) {
				if(!StringCompI(m_szDirectiveData[i],"TRUE")) return 1;
				else return 0;
			}
			iCount++;
		}
		i++;
	}
	return (-1);
}

//----------------------------------------------------------

void CConfig::AddConfigEntry(char * szName, char * szData)
{
	m_szDirectiveName[iNumEntries] = (char *)malloc(strlen(szName) + 1);
	m_szDirectiveData[iNumEntries] = (char *)malloc(strlen(szData) + 1);

	strcpy(m_szDirectiveName[iNumEntries],szName);
	strcpy(m_szDirectiveData[iNumEntries],szData);

	iNumEntries++;
}

//----------------------------------------------------------

int CConfig::ReadFile(char * szFileName)
{
	char	szReadBuffer[MAX_CONFIG_STRSIZE];
	char	szDirectiveName[MAX_CONFIG_STRSIZE];
	char	szDirectiveData[MAX_CONFIG_STRSIZE];

	char	*szReadPtr;
	int		iDirectiveLength;
	int		iDirectiveDataLength;
	int		iCurrentLine = 1;

	FILE	*fReadFile = fopen(szFileName,"r");

	if(!fReadFile) {
		sprintf(m_szErrorString,"Can't open %s for reading.",szFileName);
		return CCONF_ERR_FILE;
	}

	while(!feof(fReadFile)) {
		fgets(szReadBuffer,MAX_CONFIG_STRSIZE,fReadFile);
		szReadPtr = szReadBuffer;
		iDirectiveLength = 0;
		iDirectiveDataLength = 0;

		// Skip any leading whitespace
		while(*szReadPtr == ' ' || *szReadPtr == '\t') szReadPtr++;

		// Check for comment char, blank line or a key name. Key names
		// are currently resevered for future use.
		if( *szReadPtr == '\0' || *szReadPtr == ';' ||
			*szReadPtr == '\n' || *szReadPtr == '[' ) {

			iCurrentLine++;
			continue;
		}

		// Parse out the directive name
		while( *szReadPtr != '\0' &&
			 *szReadPtr != ' ' &&
			 *szReadPtr != '=' &&
			 *szReadPtr != '\n' &&
			 *szReadPtr != '\t' &&
			 *szReadPtr != ';' &&
			 *szReadPtr != '\r') {
			szDirectiveName[iDirectiveLength] = toupper(*szReadPtr);
			iDirectiveLength++;
			szReadPtr++;
		}

		if(iDirectiveLength == 0) {
			iCurrentLine++;
			continue;
		}

		szDirectiveName[iDirectiveLength] = '\0';

		// Skip any whitespace
		while(*szReadPtr == ' ' || *szReadPtr == '\t') szReadPtr++;

		// The config entry is delimited by '='
		if(*szReadPtr != '=') {
			sprintf(m_szErrorString,"Parse error on line %d. No value for config entry.",iCurrentLine);
			fclose(fReadFile);
			return CCONF_ERR_PARSE;
		}

		// The rest is the directive data
		*szReadPtr++;

		// Skip any whitespace
		while(*szReadPtr == ' ' || *szReadPtr == '\t') szReadPtr++;

		while( *szReadPtr != '\0' &&
			 *szReadPtr != '\n' &&
			 *szReadPtr != ';' ) {
			if(*szReadPtr != '\"') {
				szDirectiveData[iDirectiveDataLength] = *szReadPtr;
				iDirectiveDataLength++;
			}
			szReadPtr++;
		}

		if(iDirectiveDataLength == 0) {
			sprintf(m_szErrorString,"Parse error on line %d. No data for config entry.",iCurrentLine);
			fclose(fReadFile);
			return CCONF_ERR_PARSE;
		}

		szDirectiveData[iDirectiveDataLength] = '\0';

		// cleanup any trailing whitespace on the directive data.
		iDirectiveDataLength--;
		while(szDirectiveData[iDirectiveDataLength] == ' '  ||
			  szDirectiveData[iDirectiveDataLength] == '\t' ||
			  szDirectiveData[iDirectiveDataLength] == '\r')
		{
			szDirectiveData[iDirectiveDataLength] = '\0';
			iDirectiveDataLength--;
		}

		AddConfigEntry(szDirectiveName,szDirectiveData);
		iCurrentLine++;
	}

	fclose(fReadFile);
	return CCONF_ERR_SUCCESS;
}

//----------------------------------------------------------

int CConfig::StringCompI(const char *s1, const char *s2)
{
	return stricmp(s1, s2);
}

//----------------------------------------------------------
