//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

extern CNetworkManager *pNetowkManager;
extern CScripts *pScripts;

using namespace RakNet;

SQInteger sq_setScriptAuthor(SQVM * pVM)
{
	for(int i = 0; i < MAX_SCRIPTS; i++)
	{
		if(pScripts->GetScript(i))
		{
			if(pScripts->GetScript(i)->GetVM() == pVM)
			{
				CScript * pScript = pScripts->GetScript(i);
				const SQChar * szAuthor;
				sq_getstring(pVM, -1, &szAuthor);
				pScript->SetScriptAuthor(szAuthor);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_setScriptVersion(SQVM * pVM)
{
	for(int i = 0; i < MAX_SCRIPTS; i++)
	{
		if(pScripts->GetScript(i))
		{
			if(pScripts->GetScript(i)->GetVM() == pVM)
			{
				CScript * pScript = pScripts->GetScript(i);
				const SQChar * szVersion;
				sq_getstring(pVM, -1, &szVersion);
				pScript->SetScriptVersion(szVersion);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq__call(SQVM * pVM)
{
	int iTop = sq_gettop(pVM) - 1;

	const char * szFunc;

	sq_getstring(pVM, -iTop, &szFunc);

	int iArgCount = iTop - 1;
	SQObjectPtr * pArguments = NULL;
	if(iArgCount > 0)
	{
		pArguments = new SQObjectPtr[iArgCount];
		for (int i = 1; i < iTop; i++)
			pArguments[i - 1] = stack_get(pVM, -iTop + i);
	}
	
	pScripts->Call(szFunc, iArgCount, pArguments);
	return 1;
}
