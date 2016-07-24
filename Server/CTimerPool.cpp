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

#include "StdInc.h"


extern CNetworkManager *pNetowkManager;
extern CScripts *pScripts;
using namespace RakNet;

CTimerPool::CTimerPool()
{
	for(int i = 0; i < MAX_TIMERS; i++)
		m_Timers[i] = NULL;
}

CTimerPool::~CTimerPool()
{
	for(int i = 0; i < MAX_TIMERS; i++)
		Kill(i);
}

int CTimerPool::Set(SQVM * pScriptVM, SQObjectPtr pFunction, int iInterval, int iRepeations, int iArgCount, SQObjectPtr * pArguments)
{
	for(int i = 0; i < MAX_TIMERS; i++)
	{
		if(m_Timers[i] == NULL)
		{
			m_Timers[i] = new ScriptTimer;
			m_Timers[i]->pFunction = pFunction;
			m_Timers[i]->uInterval = iInterval;
			m_Timers[i]->iRemainingRepeations = iRepeations;
			m_Timers[i]->pScriptVM = pScriptVM;
			m_Timers[i]->uTick = GetTickCount();
			m_Timers[i]->iArgCount = iArgCount;
			m_Timers[i]->pArguments = pArguments;
			pScripts->onTimerCreate(i);
			return i;
		}
	}

	return -1;
}

bool CTimerPool::Kill(int iTimerId)
{
	if(iTimerId >= 0 && iTimerId < MAX_TIMERS)
	{
		if(m_Timers[iTimerId] != NULL)
		{
			if(m_Timers[iTimerId]->pArguments != NULL)
			{
				delete[] m_Timers[iTimerId]->pArguments;
				m_Timers[iTimerId]->pArguments = NULL;
			}
			delete m_Timers[iTimerId];
			m_Timers[iTimerId] = NULL;
			pScripts->onTimerDestroy(iTimerId);
			return true;
		}
	}

	return false;
}

void CTimerPool::Process()
{
	for(int i = 0; i < MAX_TIMERS; i++)
	{
		if(m_Timers[i] != NULL)
		{
			if((GetTickCount() - m_Timers[i]->uTick) > m_Timers[i]->uInterval)
			{
				// TODO: This should just use CSquirrel::Call
				HSQUIRRELVM vm = m_Timers[i]->pScriptVM;

				int iTopStart = sq_gettop(vm);
				sq_pushroottable(vm);

				if(m_Timers[i]->pArguments != NULL)
				{
					for(int j = 0; j < m_Timers[i]->iArgCount; ++j)
						vm->Push(m_Timers[i]->pArguments[j]);
				}

				SQObjectPtr retval;
				vm->Call(m_Timers[i]->pFunction, m_Timers[i]->iArgCount + 1, vm->_top - m_Timers[i]->iArgCount - 1, retval, true);
				sq_settop(vm, iTopStart);

				// Make sure the timer didn't kill itself
				if(m_Timers[i] != NULL)
				{
					// If this was the last repetition
					if(m_Timers[i]->iRemainingRepeations == 1)
					{
						Kill(i);
						continue;
					}
					else if(m_Timers[i]->iRemainingRepeations != -1) // If it is not infinite
					{
						--m_Timers[i]->iRemainingRepeations;
					}

					m_Timers[i]->uTick = GetTickCount();
				}
			}
		}
	}
}


void CTimerPool::HandleScriptUnload(SQVM * pScriptVM)
{
	for(int i = 0; i < MAX_TIMERS; i++)
	{
		if(m_Timers[i] != NULL)
		{
			if(m_Timers[i]->pScriptVM == pScriptVM)
			{
				Kill(i);
			}
		}
	}
}

bool CTimerPool::IsActive(int iTimerId)
{
	if(iTimerId < 0 || iTimerId > MAX_TIMERS)
	{
		return false;
	}

	return m_Timers[iTimerId] != NULL;
}