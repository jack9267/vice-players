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

using namespace RakNet;

// setTimer(function, interval, repetitions, ...)
SQInteger sq_setTimer(SQVM * pVM)
{
	int iTop = sq_gettop(pVM) - 1;

	int interval;
	int repeations;
	sq_getinteger(pVM, -iTop + 1, &interval);
	sq_getinteger(pVM, -iTop + 2, &repeations);

	if(interval < 1 || repeations < -1 || repeations == 0)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	SQObjectPtr pFunction;
	pFunction = stack_get(pVM, -iTop);

	int iArgCount = iTop - 3;
	SQObjectPtr * pArguments = NULL;
	if(iArgCount > 0)
	{
		pArguments = new SQObjectPtr[iArgCount];

		for (int i = 3; i < iTop; i++)
			pArguments[i - 3] = stack_get(pVM, -iTop + i);
	}

	int iTimerId = pNetowkManager->GetTimerPool()->Set(pVM, pFunction, interval, repeations, iArgCount, pArguments);
	if(iTimerId != -1)
	{
		sq_pushinteger(pVM, iTimerId);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// isTimerActive(id)
SQInteger sq_isTimerActive(SQVM * pVM)
{
	int id;
	sq_getinteger(pVM, -1, &id);

	sq_pushbool(pVM, pNetowkManager->GetTimerPool()->IsActive(id));
	return 1;
}

// killTimer(id)
SQInteger sq_killTimer(SQVM * pVM)
{
	int id;
	sq_getinteger(pVM, -1, &id);

	sq_pushbool(pVM, pNetowkManager->GetTimerPool()->Kill(id));
	return 1;
}