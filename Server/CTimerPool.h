//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): MaVe
//                 jenksta
// License: See LICENSE in root directory
//
//-----------------------------------------------------
#pragma once
#include "main.h"
#include "CScripts.h"
#include "../vendor/squirrel/squirrel.h"
#include "../vendor/squirrel/sqvm.h"

#define MAX_TIMERS 100

struct ScriptTimer
{
	SQObjectPtr pFunction;
	unsigned int uInterval;
	int iRemainingRepeations;
	SQVM * pScriptVM;
	unsigned int uTick;
	int iArgCount;
	SQObjectPtr * pArguments;
};

class CTimerPool
{
private:
	ScriptTimer * m_Timers[MAX_TIMERS];
public:
	CTimerPool();
	~CTimerPool();
	void Process();

	int Set(SQVM * pScriptVM, SQObjectPtr pFunction, int iInterval, int iRepeations, int iArgCount, SQObjectPtr * pArguments);
	bool Kill(int iTimerId);
	void HandleScriptUnload(SQVM * pScriptVM);
	bool IsActive(int iTimerId);
};