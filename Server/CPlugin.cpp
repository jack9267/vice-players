//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): jenksta
//                 adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

extern CNetworkManager *pNetowkManager;

#ifndef WIN32
#include "dlfcn.h"
#endif

CPlugin::CPlugin(const char *szName)
{
	m_pLibrary = NULL;
	m_szName = szName;
}

bool CPlugin::Load()
{
	if(!m_pLibrary)
	{
		std::string szPath = "plugins/";
		szPath += m_szName;
	#ifdef WIN32
		szPath += ".dll";
		m_pLibrary = LoadLibrary(szPath.c_str());
	#else
		szPath += ".so";
		m_pLibrary = dlopen(szPath.c_str(), RTLD_LAZY);
	#endif
		if(!m_pLibrary)
		{
			return false;
		}
		return true;
	}
	return false;
}

CPlugin::~CPlugin()
{
	if(m_pLibrary)
	{
		OnPluginUnload_t pfn = (OnPluginUnload_t) GetProcedureAddress("OnPluginUnload");

		if(pfn)
			pfn();

#ifdef WIN32
		FreeLibrary((HMODULE)m_pLibrary);
#else
		dlclose(m_pLibrary);
#endif
		m_pLibrary = NULL;
	}
}


void * CPlugin::GetProcedureAddress(const char * szProcName)
{
	if(m_pLibrary)
	{
		void * pProcAddress = NULL;
#ifdef WIN32
		pProcAddress = (void *)GetProcAddress((HMODULE)m_pLibrary, szProcName);
#else
		pProcAddress = dlsym(m_pLibrary, szProcName);
#endif
		return pProcAddress;
	}

	return NULL;
}
