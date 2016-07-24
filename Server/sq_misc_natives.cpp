//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: Christopher
//               adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "StdInc.h"

extern CNetworkManager *pNetowkManager;
extern CPlugins *pPlugins;
using namespace RakNet;


SQInteger sq_setGameTime(SQVM * pVM)
{
	SQInteger h, m;

	sq_getinteger(pVM, -2, &h);
	sq_getinteger(pVM, -1, &m);

	pNetowkManager->GetPlayerManager()->SetGameTime(h, m);
	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_getMaxPlayers(SQVM * pVM)
{
	sq_pushinteger(pVM, pNetowkManager->GetMaxPlayers());
	return 1;
}

SQInteger sq_getTickCount(SQVM * pVM)
{
	sq_pushinteger(pVM, GetTickCount());
	return 1;
}

SQInteger sq_createObject(SQVM * pVM)
{
	SQInteger model;
	Vector3 vecPos, vecRot;
	sq_getinteger(pVM, -7, &model);
	sq_getfloat(pVM, -6, &vecPos.X);
	sq_getfloat(pVM, -5, &vecPos.Y);
	sq_getfloat(pVM, -4, &vecPos.Z);
	sq_getfloat(pVM, -3, &vecRot.X);
	sq_getfloat(pVM, -2, &vecRot.Y);
	sq_getfloat(pVM, -1, &vecRot.Z);

	EntityId object = pNetowkManager->GetObjectManager()->New(model, &vecPos, &vecRot);

	sq_pushinteger(pVM, object);
	return 1;
}

SQInteger sq_destroyObject(SQVM * pVM)
{
	SQInteger objId;

	sq_getinteger(pVM, -1, &objId);

	pNetowkManager->GetObjectManager()->Delete(objId);

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_createPickup(SQVM * pVM)
{
	SQInteger model, type;
	Vector3 vecPos;
	sq_getinteger(pVM, -5, &model);
	sq_getinteger(pVM, -4, &type);
	sq_getfloat(pVM, -3, &vecPos.X);
	sq_getfloat(pVM, -2, &vecPos.Y);
	sq_getfloat(pVM, -1, &vecPos.Z);

	EntityId pickup = pNetowkManager->GetPickupManager()->New(model, type, &vecPos);

	sq_pushinteger(pVM, pickup);
	return 1;
}

SQInteger sq_destroyPickup(SQVM * pVM)
{
	SQInteger pickupId;

	sq_getinteger(pVM, -1, &pickupId);

	pNetowkManager->GetPickupManager()->Delete(pickupId);

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_isPluginLoaded(SQVM * pVM)
{
	const char * szName;
	sq_getstring(pVM, -1, &szName);

	sq_pushinteger(pVM, pPlugins->IsPluginLoaded(szName));
	return 1;
}

SQInteger sq_setModeName(SQVM * pVM)
{
	const char * szMode;
	sq_getstring(pVM, -1, &szMode);

	pNetowkManager->m_szModeName = szMode;

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_setMapName(SQVM * pVM)
{
	const char * szMap;
	sq_getstring(pVM, -1, &szMap);

	pNetowkManager->m_szMapName = szMap;

	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_createCheckpoint(SQVM * pVM)
{
	Vector3 vecPos;
	float fRadius;

	sq_getfloat(pVM, -1, &fRadius);
	sq_getfloat(pVM, -2, &vecPos.Z);
	sq_getfloat(pVM, -3, &vecPos.Y);
	sq_getfloat(pVM, -4, &vecPos.X);

	pNetowkManager->GetCheckpoints()->New(vecPos, 0, fRadius);
	
	sq_pushinteger(pVM, 0);
	return 1;
}

SQInteger sq_destroyCheckpoint(SQVM * pVM)
{
	SQInteger cpId;

	sq_getinteger(pVM, -1, &cpId);

	pNetowkManager->GetCheckpoints()->Delete(cpId);

	sq_pushbool(pVM, true);
	return 1;
}