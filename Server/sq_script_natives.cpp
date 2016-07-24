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

SQInteger sq_loadClientScript(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	const char * szScript;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getstring(pVM, -1, &szScript);

	if(pNetowkManager->GetPlayerManager()->GetSlotState(playerSystemAddress))
	{
		long nameSize;
		std::string str = "clientscripts/";
		std::string script;
		RakNet::BitStream bs;

		nameSize = strlen(szScript);
		str.append(szScript);

		FILE *f = fopen(str.c_str(), "rb");
		if(f)
		{
			char szInput[128];
			memset(szInput, '\0', 128);
			fseek(f, 0, SEEK_END);
			long fileSize = ftell(f);
			rewind(f);
			while(!feof(f))
			{
				fgets(szInput, 128, f);
				script.append(szInput);
			}
			bs.Write(nameSize);
			bs.Write(script.size());
			bs.Write(fileSize);
			bs.Write(szScript, nameSize);
			bs.Write(script.c_str(), script.size());
			pNetowkManager->GetRPC4()->Call("UploadClientScript",&bs,LOW_PRIORITY,UNRELIABLE,0,pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

			fclose(f);
			sq_pushbool(pVM, true);
			return 1;
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

SQInteger sq_clientCall(SQVM * pVM)
{
	int iTop = sq_gettop(pVM) - 1;
	int playerid;
	const char * szFunc;
	
	sq_getinteger(pVM, -iTop, &playerid);
	sq_getstring(pVM, -iTop + 1, &szFunc);

	int iArgCount = iTop - 2;
	SQObjectPtr * pArguments = NULL;
	if(iArgCount > 0)
	{
		pArguments = new SQObjectPtr[iArgCount];
		for (int i = 2; i < iTop; i++)
			pArguments[i - 2] = stack_get(pVM, -iTop + i);
	}
	
	BitStream bsSend;
	bsSend.Write(strlen(szFunc));
	bsSend.Write(iArgCount);
	bsSend.Write(szFunc,strlen(szFunc));
	printf("calling %s\n", szFunc);
	for(int i = 0; i < iArgCount; i++)
	{
		SQObject t = pArguments[i];
		SQObjectType type = t._type;
		bsSend.Write((int)type);
		if(type == OT_INTEGER)
			bsSend.Write((int)_integer(t));
		else if(type == OT_FLOAT)
			bsSend.Write((float)_float(t));
		else if(type == OT_BOOL)
			bsSend.Write((bool)(_integer(t) != 0));
		else if(type == OT_STRING)
		{
			std::string str = _stringval(t);
			int len = str.size();
			bsSend.Write(len);
			bsSend.Write(str.c_str(), len);
		}
	}

	pNetowkManager->GetRPC4()->Call("Script_ClientCall", &bsSend, LOW_PRIORITY, RELIABLE, 0, pNetowkManager->GetRakPeer()->GetSystemAddressFromIndex(playerid), false);
	return 1;
}