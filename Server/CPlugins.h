//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------
#pragma once
#include "../vendor/squirrel/squirrel.h"
#include "CConfig.h"
#include "CPlugin.h"

#define MAX_PLUGINS 32

//
typedef void                     (* OnPluginLoad_t)(const char * szName);
typedef void                     (* OnScriptLoad_t)(SQVM * pVM);
typedef void                     (* OnScriptUnload_t)(SQVM * pVM);
typedef void                     (* OnPulse_t)();
//

class CPlugins
{
public:
	CPlugins();
	~CPlugins();

	CPlugin * GetScript(int pluginID) { 
		if(!m_pPlugins[pluginID]) return NULL;
		return m_pPlugins[pluginID]; 
	};

	bool LoadPlugin(const char * szName);
	bool UnloadPlugin(const char * szName);
	void LoadFromConfig(CConfig * pConfig);

	bool IsPluginLoaded(const char * szName);

	void OnPulse();
	void OnPluginLoad(const char * szName);
	void OnPluginUnload(const char * szName);
	void OnScriptLoad(SQVM * pVM);
	void OnScriptUnload(SQVM *pVM);
private:
	CPlugin * m_pPlugins[MAX_PLUGINS];
};